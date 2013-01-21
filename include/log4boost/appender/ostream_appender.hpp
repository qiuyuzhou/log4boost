/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_OSTREAM_APPENDER_H__
#define LOGGING_OSTREAM_APPENDER_H__

#include <ostream>
#include <log4boost/detail/mutex.hpp>

#include <log4boost/config.hpp>
#include <log4boost/detail/mutex.hpp>
#include <log4boost/appender/layout_appender.hpp>

namespace log4boost
{

	/**
	 *	The appender would write the log to the specified ostream using the specified layout.
	 */
	class LOG4BOOST_EXPORT ostream_appender
		:public layout_appender
	{
	public:
		virtual ~ostream_appender(void);

		virtual void reopen();

		virtual void close();

		static shared_ptr<ostream_appender>	create( const std::string& name, std::ostream& stream, layout* _layout = 0 );
	protected:
		ostream_appender( const std::string& name, std::ostream& stream, layout* _layout = 0 );

		virtual void _append(const logging_event& event);

		mutable mutex	mutex_;

		std::ostream& stream_;
	};

}

#endif//LOGGING_OSTREAM_APPENDER_H__
