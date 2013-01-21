/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_LAYOUT_H__
#define LOGGING_LAYOUT_H__

#include <string>

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>

namespace log4boost
{
	class logging_event;

	/**
	 *	The layout interface class,use for formating the log message.
	 */
	class LOG4BOOST_EXPORT layout
		:public detail::noncopyable
	{
	public:
		layout(void);
		virtual ~layout(void);

		/**
		 *	Format the logging event to the ostream.
		 *	@param	event	The event would be log.
		 *	@param	out		The ostream would be log into.
		 */
		virtual void format(const logging_event &event,std::ostream& out) = 0;
	};


}

#endif//LOGGING_LAYOUT_H__
