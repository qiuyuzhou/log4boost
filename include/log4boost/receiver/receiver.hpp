/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef __LOGGING_RECEIVER_HPP___
#define __LOGGING_RECEIVER_HPP___

#include <boost/signals.hpp>
#include <log4boost/detail/mutex.hpp>

#include <log4boost/detail/noncopyable.hpp>
#include <log4boost/logging_event.hpp>
#include <log4boost/detail/shared_ptr.hpp>


namespace log4boost
{


	class LOG4BOOST_EXPORT receiver 
		: detail::noncopyable
	{
	public:
		virtual ~receiver();
		virtual void close() = 0;

		boost::signal<void( logging_event& )>& signal_logging_event()
		{
			return signal_logging_event_;
		}

		const std::string& get_name()const
		{
			return name_;
		}

		static shared_ptr<receiver> get_receiver( const std::string& name );

		void enable_relog();
		void disable_relog();
		bool enabled_relog() const;

		mutex& get_mutex()
		{
			return mutex_;
		}

		static void remove_all();
		static void add_receiver( const shared_ptr<receiver>& p );
		static void remove_receiver( const std::string& name );
		static void close_all();

	protected:
		receiver( const std::string& name );

		void log( logging_event& event );

		boost::signal<void( logging_event& )>		signal_logging_event_;
		std::string										target_logger_name_;

		mutable mutex							mutex_;
	private:
		const std::string	name_;
		boost::signals::connection relog_signal_connection_;
	};

	typedef shared_ptr<receiver> receiver_ptr;

}


#endif//__LOGGING_RECEIVER_HPP___
