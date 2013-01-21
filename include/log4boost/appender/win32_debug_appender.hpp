/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_WIN32_DEBUG_APPENDER_H___
#define LOGGING_WIN32_DEBUG_APPENDER_H___

#if defined(WIN32) || defined(WIN64)	// only use this on Win32

#include <log4boost/config.hpp>
#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/appender_creator_base.hpp>

namespace log4boost
{

	/**
	 *	\class win32_debug_appender
	 *	The appender would log the message to debugger of windows.
	 *	Only available on win32 platform.
	 *
	 *	\par The config options:
	 *	- \b layout			\n The layout used by the appender. Default is formatter_layout using DEFAULT_FORMAT.
	 */
	class LOG4BOOST_EXPORT win32_debug_appender
		:public layout_appender
	{
	public:
		virtual ~win32_debug_appender();

		virtual void reopen();

		virtual void close();

		static shared_ptr<win32_debug_appender>	create(const std::string& name,layout* _layout=0);
	protected:
		/**
		*	The appender constructor.
		*	@param	name			Appender name.
		*	@param	_layout			The layout object the appender should use.If give the NULL,
		*							it would use the default layout
		*/
		win32_debug_appender(const std::string& name,layout* _layout=0);

		virtual void _append(const logging_event& event);

	};

	class LOG4BOOST_EXPORT win32_debug_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};
}

#endif	// WIN32

#endif LOGGING_WIN32_DEBUG_APPENDER_H___
