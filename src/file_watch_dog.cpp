/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/file_watch_dog.hpp"

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

namespace log4boost
{

	file_watch_dog::file_watch_dog( const boost::filesystem::path& file_path )
		:file_path_(file_path)
		,delay_(DEFAULT_DELAY)
	{
		namespace fs = boost::filesystem;
		if ( fs::exists( file_path_ ) )
		{
			last_write_time_ = fs::last_write_time( file_path_ );
		}
		else
		{
			last_write_time_ = 0;
		}
	}

	void file_watch_dog::start()
	{
		mutex::scoped_lock	lock( mutex_ );
		if ( thread_ )
		{
			return;
		}

		thread_.reset( new boost::thread( boost::bind(&file_watch_dog::svc,this) ) );
	}

	void file_watch_dog::stop()
	{
		{
			mutex::scoped_lock	lock( mutex_ );
			if ( !thread_ )
			{
				return;
			}
		}
		stop_condition_.notify_one();
		thread_->join();
		thread_.reset();
	}

	void file_watch_dog::svc()
	{
		mutex::scoped_lock	lock( mutex_ );
		while(true)
		{
			if ( stop_condition_.timed_wait( lock, boost::posix_time::milliseconds( delay_ ) ) )
			{
				break;
			}
			else
			{
				check();
			}
		}
	}

	void file_watch_dog::check()
	{
		namespace fs = boost::filesystem;
		if ( fs::exists( file_path_ ) )
		{
			std::time_t this_write_time = fs::last_write_time( file_path_ );
			if ( this_write_time > last_write_time_ )
			{
				last_write_time_ = this_write_time;
				on_changed();
			}
		}
	}




}

