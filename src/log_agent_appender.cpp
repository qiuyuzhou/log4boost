/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/

#include "stdafx.h"
#include "log4boost/appender/log_agent_appender.hpp"
#include "log4boost/detail/stack_ostream.hpp"
#include "ptree_inc.h"
#include "log4boost/layout_factory.hpp"
#include "io.hpp"

#if defined(WIN32) || defined(WIN64)
#	include "io.h"
#else
#	define _read read
#	define _write write
#	define _close close
#	define _pipe pipe
#endif

#include <fcntl.h>

#include <boost/format.hpp>
#include <boost/program_options.hpp>


namespace log4boost
{

	static int write_until_size( int fp, const char* pBuf, size_t request_size )
	{
		int	bytes_write = 0;
		do
		{
			int r = _write( fp, (char*)pBuf + bytes_write, request_size - bytes_write );
			if ( r > 0 )
			{
				bytes_write += r;
			}
			else
			{
				return r;
			}
		}
		while( bytes_write < (int) request_size );

		return bytes_write;
	}

	enum PIPES { READ, WRITE }; /* Constants 0 and 1 for READ and WRITE */

	log_agent_appender::log_agent_appender( const std::string& name, layout* _layout /*= 0 */ )
		:layout_appender(name,_layout)
		,agent_settings_(wt_file)
		,process_full_log(false)
	{
		fdpipe_[0] = -1;
		fdpipe_[1] = -1;
	}

	log_agent_appender::~log_agent_appender()
	{
		close();
	}

	void log_agent_appender::reopen()
	{
		mutex::scoped_lock	lock(mutex_);

		close_i();
		reopen_i();
	}

	void log_agent_appender::close()
	{
		mutex::scoped_lock	lock(mutex_);
		close_i();
	}

	void log_agent_appender::_append( const logging_event& event )
	{
		detail::stack_ostream	buf;
		_get_layout().format(event,buf.get_out());
	
		mutex::scoped_lock	lock(mutex_);

		if ( fdpipe_[WRITE] < 0 )
		{
			return;
		}

		const char* p = buf.str();
		std::size_t	size = buf.length();
		if ( buf.length() == 0 )
		{
			return;
		}
		int bytes_write;

		if ( process_full_log )
		{
			// process as length/buffer
			char	size_buf[4];
			{
				char* p_size_buf = size_buf;
				io::write_uint32( size, p_size_buf );
			}

			bytes_write = write_until_size( fdpipe_[WRITE], size_buf, 4 );
			if ( bytes_write <= 0 )
			{
				::_close(fdpipe_[WRITE]);
				fdpipe_[WRITE] = -1;
				return;
			}
		}

		bytes_write = write_until_size( fdpipe_[WRITE], p, size );
		if ( bytes_write <= 0 )
		{
			::_close(fdpipe_[WRITE]);
			fdpipe_[WRITE] = -1;
			return;
		}
	}

	void log_agent_appender::open( const agent_settings& _agent_settings, const boost::any& writer_settings )
	{
		mutex::scoped_lock	lock(mutex_);

		agent_settings_		= _agent_settings;
		writer_settings_	= writer_settings;

		reopen_i();
	}
	void log_agent_appender::reopen_i()
	{
		if ( _pipe( fdpipe_, 1024*64, O_BINARY ) == -1 )
		{
			throw std::runtime_error("Create pipe failed!");
		}

		char arguments[16*1024];

		if ( agent_settings_.writer_type == wt_file )
		{
			const file_writer_settings& settings = boost::any_cast<file_writer_settings>(writer_settings_);

			sprintf(arguments, "log_agent.exe --writer-type=file_writer --file-path=\"%s\" --append=%d --pipe-read=%d  --pipe-write=%d --process-full-log=%d"
				, settings.file_path.c_str()
				, settings.append?1:0
				, fdpipe_[READ]
				, fdpipe_[WRITE]
				, process_full_log?1:0
				);
		}
		else if ( agent_settings_.writer_type == wt_rolling_file )
		{
			const rolling_file_settings&	settings = boost::any_cast<rolling_file_settings>(writer_settings_);
			
			sprintf(arguments, "log_agent.exe --writer-type=rolling_file_writer --rolling-file-path=\"%s\" --file-count=%d --extension-name=\"%s\" --max-size-bytes=%d --new-file-by-date=%d --initial-erase=%d  --pipe-read=%d --pipe-write=%d --process-full-log=%d"
				, settings.file_path.c_str()
				, settings.file_count
				, settings.extension_name.c_str()
				, settings.max_size_bytes
				, settings.new_file_by_date?1:0
				, settings.initial_erase?1:0
				, fdpipe_[READ]
				, fdpipe_[WRITE]
				, process_full_log?1:0
				);
		}
		else if ( agent_settings_.writer_type == wt_tcp )
		{
			process_full_log = true;
			const tcp_writer_settings&		settings = boost::any_cast<tcp_writer_settings>(writer_settings_);
			sprintf(arguments, "log_agent.exe --writer-type=tcp_writer --host=\"%s\" --port=%d --pipe-read=%d --pipe-write=%d --process-full-log=%d"
				, settings.host.c_str()
				, settings.port
				, fdpipe_[READ]
				, fdpipe_[WRITE]
				, process_full_log?1:0
				);
		}
		else
		{
			throw std::runtime_error("Unknown writer type");
		}

		namespace po = boost::program_options;
		std::vector<std::string> argsv = po::split_winmain(arguments);

		boost::scoped_array<const char*>	args(new const char*[ argsv.size() + 1 ] );
		for ( unsigned int i = 0; i < argsv.size(); ++i )
		{
			args[i] = argsv[i].c_str();
		}
		args[argsv.size()] = 0;

		if ( -1 == _spawnv(_P_DETACH,"log_agent.exe",args.get()) )
		{
			throw std::runtime_error( std::string("spawn log agent process failed! error string: ") + strerror(errno) );
		}
		::_close(fdpipe_[READ]);
		fdpipe_[READ] = -1;
	}
    
	void log_agent_appender::close_i()
	{
		if ( fdpipe_[WRITE] >= 0)
		{
			::_close(fdpipe_[WRITE]);
			fdpipe_[WRITE] = -1;
		}
		if( fdpipe_[READ] >= 0 )
		{
			::_close(fdpipe_[READ]);
			fdpipe_[READ] = -1;
		}
	}

	shared_ptr<log_agent_appender> log_agent_appender::create( const std::string& name, layout* _layout/*=0 */ )
	{
		shared_ptr<log_agent_appender> p( new log_agent_appender( name, _layout ) );
		appender::add_appender( p );
		return p;
	}

	shared_ptr<appender> log_agent_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		std::string writer_type = params.get<std::string>("writer_type");

		std::auto_ptr<layout> l_layout;
		boost::optional<const property_tree::ptree&> o = params.get_child_optional("layout");
		if (o)
		{
			l_layout = layout_factory::get_instance().create(*o);
		}

		shared_ptr<log_agent_appender>	pAppander;
		if ( writer_type == "file_writer" )
		{
			log_agent_appender::agent_settings	agent_settings(log_agent_appender::wt_file);

			std::string file_path			= params.get<std::string>("file_path");
			log_agent_appender::file_writer_settings	writer_settings(file_path);
			writer_settings.append		= params.get<bool>("append",true);
			
			pAppander = log_agent_appender::create( name, l_layout.release() );

			pAppander->open(agent_settings,writer_settings);
		}
		else if ( writer_type == "rolling_file_writer")
		{
			log_agent_appender::agent_settings	agent_settings(log_agent_appender::wt_rolling_file);

			std::string file_path			= params.get<std::string>("file_path");
			rolling_file_settings	writer_settings(file_path);
			writer_settings.extension_name		= params.get("extension_name",writer_settings.extension_name);
			writer_settings.initial_erase		= params.get("initial_erase",writer_settings.initial_erase);
			writer_settings.flush_each_time		= params.get("flush_each_time",writer_settings.flush_each_time);
			writer_settings.file_count			= params.get("file_count",writer_settings.file_count);
			writer_settings.max_size_bytes		= params.get("max_size_bytes",writer_settings.max_size_bytes);
			writer_settings.new_file_by_date	= params.get("new_file_by_date",writer_settings.new_file_by_date);

			pAppander = log_agent_appender::create( name, l_layout.release() );

			pAppander->open(agent_settings,writer_settings);
		}
		else if ( writer_type == "tcp_writer")
		{
			log_agent_appender::agent_settings	agent_settings(log_agent_appender::wt_tcp);

			log_agent_appender::tcp_writer_settings	writer_settings;
			writer_settings.host	= params.get<std::string>("host");
			writer_settings.port	= params.get<uint16_t>("port");

			pAppander = log_agent_appender::create( name, l_layout.release() );

			pAppander->open(agent_settings,writer_settings);
		}
		else
		{
			throw std::runtime_error( std::string("Unknown writer type:<") + writer_type + ">" );
		}

		return pAppander;
	}

	std::string log_agent_appender_creator::get_name() const
	{
		return "log_agent_appender";
	}
}


