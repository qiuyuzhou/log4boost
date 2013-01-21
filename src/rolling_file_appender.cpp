/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
// Some code from boost log2 destination_rolling_file.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#include "stdafx.h"

#include "ptree_inc.h"
#include "log4boost/appender/rolling_file_appender.hpp"
#include "log4boost/writer/rolling_file_writer.hpp"
#include "log4boost/layout_factory.hpp"

#include <boost/filesystem/fstream.hpp>


namespace log4boost
{
	rolling_file_appender::rolling_file_appender( const std::string& name, layout* _layout )
		:layout_appender(name,_layout)
		,settings_("")
	{

	}

	rolling_file_appender::~rolling_file_appender()
	{
		close();
	}

	void rolling_file_appender::open( const rolling_file_settings& settings )
	{
		mutex::scoped_lock	lock(mutex_);
		settings_ = settings;
		writer_.reset( new rolling_file_writer() );
		writer_->open(settings);
	}

	void rolling_file_appender::_append( const logging_event& event )
	{
		mutex::scoped_lock	lock(mutex_);

		if ( writer_ )
		{
			writer_->ensure_out_file();

			::time_t val = event.get_time(); 
			tm* cur_tm = localtime( &val);
			writer_->check_renew_file_pre_log(cur_tm);
			_get_layout().format(event, writer_->out_stream() );
			if ( settings_.flush_each_time )
			{
				writer_->out_stream().flush();
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
							writer_->out_stream().flush();
							last_time = this_time;
						}
					}
				}
			}
			writer_->check_renew_file_post_log(cur_tm);
		}
	}

	void rolling_file_appender::flush()
	{
		mutex::scoped_lock	lock(mutex_);
		if ( writer_ )
		{
			writer_->out_stream().flush();
		}
	}

	void rolling_file_appender::close()
	{
		mutex::scoped_lock	lock(mutex_);
		if ( writer_ )
		{
			writer_->close();
			writer_.reset();
		}
	}

	void rolling_file_appender::reopen()
	{
		mutex::scoped_lock	lock(mutex_);
		if ( writer_ )
		{
			writer_->close();
			writer_->open(settings_);
		}
	}

	shared_ptr<rolling_file_appender> rolling_file_appender::create( const std::string& name, layout* _layout/*=0 */ )
	{
		shared_ptr<rolling_file_appender> p( new rolling_file_appender( name, _layout ) );
		appender::add_appender( p );
		return p;
	}


	shared_ptr<appender> rolling_file_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		std::string file_path			= params.get<std::string>("file_path");
		rolling_file_settings	settings(file_path);
		settings.extension_name		= params.get("extension_name",settings.extension_name);
		settings.initial_erase		= params.get("initial_erase",settings.initial_erase);
		settings.flush_each_time	= params.get("flush_each_time",settings.flush_each_time);
		settings.file_count			= params.get("file_count",settings.file_count);
		settings.max_size_bytes		= params.get("max_size_bytes",settings.max_size_bytes);
		settings.new_file_by_date	= params.get("new_file_by_date",settings.new_file_by_date);
		//settings.start_where_size_not_exceeded = params.get("start_where_size_not_exceeded",settings.start_where_size_not_exceeded);

		std::auto_ptr<layout> l_layout;

		boost::optional<const property_tree::ptree&> o = params.get_child_optional("layout");
		if (o)
		{
			l_layout = layout_factory::get_instance().create(*o);
		}

		shared_ptr<rolling_file_appender>	pAppender = rolling_file_appender::create(name.c_str(),l_layout.release());
		pAppender->open(settings);

		return pAppender;
	}

	std::string rolling_file_appender_creator::get_name() const
	{
		return "rolling_file_appender";
	}
}
