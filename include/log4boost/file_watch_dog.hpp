/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef _LOG4BOOST_FILE_WATCH_DOG_HPP__
#define _LOG4BOOST_FILE_WATCH_DOG_HPP__

#include <boost/thread.hpp>
#include <log4boost/detail/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/filesystem/path.hpp>


#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>
#include <log4boost/detail/mutex.hpp>

namespace log4boost
{

	class LOG4BOOST_EXPORT file_watch_dog
		: detail::noncopyable
	{
	public:
		file_watch_dog( const boost::filesystem::path& file_path );

		enum
		{
			DEFAULT_DELAY = 60000
		};

		void start();
		void stop();

		bool is_watching()const
		{
			return thread_;
		}

		void set_delay( unsigned long delay )
		{
			delay_ =delay;
		}

		const boost::filesystem::path& get_file_path()const
		{
			return file_path_;
		}

	protected:

		virtual void on_changed() = 0;

	private:

		void check();

		void svc();

		boost::filesystem::path				file_path_;

		boost::scoped_ptr<boost::thread>	thread_;
		mutex								mutex_;

		unsigned long						delay_;
		boost::condition					stop_condition_;
		std::time_t							last_write_time_;
	};



}




#endif//_LOG4BOOST_FILE_WATCH_DOG_HPP__

