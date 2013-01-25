/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/program_options.hpp>

#include <iostream>
#include <string>

#include "log4boost/detail/shared_ptr.hpp"
#include "log4boost/writer/file_writer.hpp"
#include "log4boost/writer/rolling_file_writer.hpp"
#include "log4boost/writer/tcp_writer.hpp"
#include "log4boost/configurator.hpp"
#include "log4boost/logger.hpp"

#include "io.hpp"
#if defined(WIN32) || defined(WIN64)
#	include "io.h"
#else
#	define _read read
#endif

using namespace log4boost;
enum PIPES { READ, WRITE }; /* Constants 0 and 1 for READ and WRITE */

static int read_until_size( FILE* fp, void* pBuf, size_t request_size )
{
	int bytes_read = 0;

	do
	{
		int r = fread( (void*)((char*)pBuf + bytes_read), 1, request_size - bytes_read,fp );
		if ( r > 0 )
		{
			bytes_read += r;		
		}
		else
		{
			return r;
		}
	}
	while( bytes_read < request_size );
	return bytes_read;
}

int read_until_size( int fp, void* pBuf, size_t request_size )
{
	int bytes_read = 0;

	do
	{
		int r = _read( fp, (char*)pBuf + bytes_read, request_size - bytes_read );
		if ( r > 0 )
		{
			bytes_read += r;		
		}
		else
		{
			return r;
		}
	}
	while( bytes_read < request_size );
	return bytes_read;
}

/*
void stand_alone_agent( boost::program_options::variables_map& vm )
{
	std::string config_file = vm["config-file"].as<std::string>();
	std::string config_string = vm["config-string"].as<std::string>();
	
	properties_configure( config_file, config_string );

	int fdpipe[2];
	fdpipe[READ] = vm["pipe-read"].as<int>();
	fdpipe[WRITE] = vm["pipe-write"].as<int>();
	close(fdpipe[WRITE]);

	// process as length/buffer
	unsigned char log_priority;

	unsigned int size_logger;
	std::vector<char> buffer_logger;
	buffer_logger.resize(1024);

	unsigned int size_message;
	std::vector<char> buffer_message;
	buffer_message.resize(16*1024);

	int bytes_read = 0;

	while( true )
	{
		// read priority
		bytes_read = read_until_size( fdpipe[READ], (char*)&log_priority, 1 );
		if ( bytes_read <= 0 )
		{
			break;
		}

		priority_level log4boost_priority = priority_level::PL_DEBUG;
		switch ( log_priority )
		{
		case 0:
			log4boost_priority = priority_level::PL_DEBUG;
			break;
		case 1:
			log4boost_priority = priority_level::PL_INFO;
			break;
		case 2:
			log4boost_priority = priority_level::PL_WARN;
			break;
		case 3:
			log4boost_priority = priority_level::PL_ERROR;
			break;
		case 4:
			log4boost_priority = priority_level::PL_FATAL;
			break;
		}

		// read logger name
		bytes_read = read_until_size( fdpipe[READ], &size_logger, 4 );
		if ( bytes_read <= 0 )
		{
			break;
		}
		if ( size_logger > 0 )
		{
			if ( size_logger > 16*1024 )//
			{
				close(fdpipe[READ]);
				return;
			}
			if ( buffer_logger.size() < size_logger )
			{
				buffer_logger.resize( size_logger );
			}
			bytes_read = read_until_size( fdpipe[READ], &buffer_logger[0], size_logger );
			if ( bytes_read <= 0 )
			{
				break;
			}
		}

		// read logger message
		bytes_read = read_until_size( fdpipe[READ], &size_message, 4 );
		if ( bytes_read <= 0 )
		{
			break;
		}
		if ( size_message > 0 )
		{
			if ( size_message > 1024*1024 )//*Max size limit
			{
				close(fdpipe[READ]);
				return;
			}
			if ( buffer_message.size() < size_message )
			{
				buffer_message.resize( size_message );
			}
			bytes_read = read_until_size( fdpipe[READ], &buffer_message[0], size_message );
			if ( bytes_read <= 0 )
			{
				break;
			}
		}

		logger::get( &buffer_logger[0], size_logger ).log( log4boost_priority, &buffer_message[0], size_message, "", 0 );
	}

	close(fdpipe[READ]);
}
*/

void appender_agent( boost::program_options::variables_map& vm )
{
	if ( vm.count("writer-type") == 0 )
	{
		throw std::runtime_error("Must specify options 'write-type,pipe-read,pipe-write'");
	}
	std::string writer_type = vm["writer-type"].as<std::string>();

	bool process_full_log = vm["process-full-log"].as<bool>();
	//int fdpipe[2];
	//fdpipe[READ] = vm["pipe-read"].as<int>();
	//fdpipe[WRITE] = vm["pipe-write"].as<int>();
	//close(fdpipe[WRITE]);

	shared_ptr<writer>	writer_ptr;
	if ( writer_type == "file_writer" )
	{
		std::string log_file_path;
		if ( !vm.count("file-path") )
		{
			throw std::runtime_error("Must specify log file path.");
		}
		log_file_path = vm["file-path"].as<std::string>();
		bool append = vm["append"].as<bool>();

		shared_ptr<file_writer> file_writer_ptr = shared_ptr<file_writer>(new file_writer());
		file_writer_ptr->open(log_file_path.c_str(),append);

		writer_ptr = file_writer_ptr;
	}
	else if( writer_type == "rolling_file_writer")
	{
		if ( !vm.count("rolling-file-path") )
		{
			throw std::runtime_error("Must specify rolling file path.");
		}
		rolling_file_settings	settings( vm["rolling-file-path"].as<std::string>() );

		settings.flush_each_time = false;
		settings.extension_name = vm["extension-name"].as<std::string>();
		settings.max_size_bytes = vm["max-size-bytes"].as<unsigned int>();
		settings.file_count		= vm["file-count"].as<unsigned int>();
		settings.initial_erase	= vm["initial-erase"].as<bool>();
		settings.new_file_by_date = vm["new-file-by-date"].as<bool>();

		shared_ptr<rolling_file_writer> rolling_file_writer_ptr = shared_ptr<rolling_file_writer>(new rolling_file_writer());
		rolling_file_writer_ptr->open(settings);
		writer_ptr = rolling_file_writer_ptr;
	}
	else if ( writer_type == "tcp_writer" )
	{
		if ( !vm.count("host") || !vm.count("port") )
		{
			throw std::runtime_error("Must specify rolling host and port to connect.");
		}

		std::string host = vm["host"].as<std::string>();
		uint16_t port = vm["port"].as<uint16_t>();

		shared_ptr<tcp_writer> tcp_writer_ptr( new tcp_writer(10) );
		tcp_writer_ptr->open(host,port);

		writer_ptr = tcp_writer_ptr;
	}
	else
	{
		throw std::runtime_error("Unknown writer type");
	}

	{
		if ( process_full_log )
		{
			// process as length/buffer
			std::vector<char>	buffer;
			buffer.resize(16*1024);

			char	size_buffer[4];
			size_t	size;
			int bytes_read = 0;

			while( true )
			{
				bytes_read = read_until_size( 0, size_buffer, 4 );
				if ( bytes_read <= 0 )
				{
					break;
				}
				{
					char* p_size_buffer = size_buffer;
					size = io::read_uint32(p_size_buffer);
				}
				if ( size > 1024*1024 /*Max size limit*/)
				{
					close(0);
					writer_ptr->close();
					return;
				}
				if ( buffer.size() < size )
				{
					buffer.resize( size );
				}

				bytes_read = read_until_size( 0, &buffer[0], size );
				if ( bytes_read <= 0 )
				{
					break;
				}

				writer_ptr->write( &buffer[0], size );
			}
		}
		else
		{
			// process as data stream
			char buffer[1024*4];
			while ( true ) 
			{
				int bytes_read = read( 0, buffer, sizeof(buffer) );
				if ( bytes_read <= 0 )
				{
					break;
				}
				writer_ptr->write( buffer, bytes_read );
			} 
		}

		writer_ptr->close();
		close(0);
	}
}

int agent_main(int argc, char *argv[])
{
	try
	{
		namespace po = boost::program_options;
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")

			("standalone-mode", po::value<bool>()->default_value(false), "If it is true, it works as a standalone logger, otherwise, it is just an appender." )
			("config-file", po::value<std::string>(), "Specify the config file path if it is in standalone mode." )
			("config-string", po::value<std::string>(), "Specify the config string if it is in standalone mode." )

			("writer-type", po::value<std::string>(), "Support types: file_writer, rolling_file_writer, tcp_writer" )
			//("pipe-read", po::value<int>(), "Input pipe file desc" )
			//("pipe-write", po::value<int>(), "Output pipe file desc" )

			("process-full-log", po::value<bool>(), "Whether it can be processed as data stream" )

			("file-path", po::value<std::string>(), "Set the log file path")
			("append", po::value<bool>()->default_value(true), "If it's false, overwrite the file when open it. Use it with file writer.")

			("rolling-file-path", po::value<std::string>(), "Set the log file path")
			("file-count",po::value<unsigned int>()->default_value(10), "How many files has a rolling file, by default, 10.")
			("new-file-by-date",po::value<bool>()->default_value(false),"If true, it renew the file everyday (by default, false).")
			("max-size-bytes",po::value<unsigned int>()->default_value(1024 * 1024), "Maximum size in bytes, by default 1Mb")
			("initial-erase",po::value<bool>()->default_value(false),"If true, it initially erases all files from the rolling file (by default, false).")
			("extension-name",po::value<std::string>()->default_value("log"), "The name to be used as suffix for the files, default is \"log\"")

			("host", po::value<std::string>(), "The ip or domain name of the tcp server.")
			("port", po::value<uint16_t>(), "The port of the tcp server.")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if ( vm["standalone-mode"].as<bool>() )
		{
			//stand_alone_agent( vm );
		}
		else
		{
			appender_agent( vm );
		}
	}
	catch ( const std::runtime_error& e )
	{
		std::cerr << e.what();
		return -1;
	}
	catch ( const boost::program_options::error& e )
	{
		std::cerr << e.what();
		return -1;
	}
	catch(...)
	{
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	return agent_main( argc, argv );
};

