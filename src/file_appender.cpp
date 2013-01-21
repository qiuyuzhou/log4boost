/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"

#include "log4boost/appender/file_appender.hpp"

#include "ptree_inc.h"
#include "log4boost/layout_factory.hpp"
#include "log4boost/writer/file_writer.hpp"

#include <boost/filesystem.hpp>

namespace log4boost
{
	file_appender::file_appender(const std::string& name
		,layout* _layout)
		:layout_appender(name,_layout)
		,append_(true)
		,flush_each_time_(true)
	{
	}

	file_appender::~file_appender()
	{
	}

	void file_appender::open( const char* file_name, bool append, bool flush_each_time /* = true */ )
	{
		mutex::scoped_lock	lock(mutex_);
		flush_each_time_ = flush_each_time;
		append_ = append;

		writer_.reset( new file_writer() );
		writer_->open(file_name,append);

	}

	void file_appender::close()
	{
		mutex::scoped_lock	lock(mutex_);
		writer_->close();
		writer_.reset();
	}

	void file_appender::reopen()
	{
		mutex::scoped_lock	lock(mutex_);
		close();
		writer_.reset( new file_writer() );
		writer_->open(file_name_.c_str(),append_);
	}

	void file_appender::flush()
	{
		mutex::scoped_lock	lock(mutex_);
		writer_->flush();
	}

	void file_appender::_append(const logging_event& event)
	{
		mutex::scoped_lock	lock(mutex_);
		if ( !writer_ )
		{
			return;
		}

		_get_layout().format(event, writer_->out_stream() );
		if (flush_each_time_)
		{
			writer_->flush();
		}
		else
		{
			// auto flush per 5 secs
			static time_t flush_time = 5; 
			{
				static time_t last_time = 0;
				if( last_time == 0 )
				{
					last_time = ::time(0);
				}
				else
				{
					time_t this_time = ::time(0);
					time_t dt = this_time - last_time;

					if( dt > flush_time )
					{
						writer_->flush();
						last_time = this_time;
					}
				}
			}

		}
	}

	shared_ptr<file_appender> file_appender::create( const std::string& name, layout* _layout/*=0 */ )
	{
		shared_ptr<file_appender> p( new file_appender( name, _layout ) );
		appender::add_appender( p );
		return p;
	}


	shared_ptr<appender> file_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		std::string file_name = params.get<std::string>("filename");
		bool		append = params.get<bool>("append",false);
		bool		flush_each_time = params.get<bool>("flush_each_time",true);

		std::auto_ptr<layout> l_layout;

		boost::optional<const property_tree::ptree&> o = params.get_child_optional("layout");
		if (o)
		{
			l_layout = layout_factory::get_instance().create(*o);
		}

		shared_ptr<file_appender>	pAppender( file_appender::create(name,l_layout.release()) );
		pAppender->open(file_name.c_str(),append,flush_each_time);
		return pAppender;
	}

	std::string file_appender_creator::get_name() const
	{
		return "file_appender";
	}
}
