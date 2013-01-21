/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_APPENDER_H__
#define LOGGING_APPENDER_H__

#include <string>
#include <set>

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>
#include <log4boost/detail/shared_ptr.hpp>
#include <log4boost/logging_event.hpp>

namespace log4boost
{

	/**
	 *	The appender interface for log output.
	 */
	class LOG4BOOST_EXPORT appender
		:detail::noncopyable
	{
	public:
		virtual ~appender(void);

		/**
		 *	Get the appender pointer by name if it's existed.
		 *	@return	If the appender of the name is existed, return the its pointer, otherwise 0.
		 */
		static shared_ptr<appender> get_appender(const std::string& name);

		static void reopen_all();

		static void remove_all();
		
		static void add_appender( const shared_ptr<appender>& p );
		static void remove_appender( const std::string& name );

		void do_append(const logging_event& event);

		virtual void reopen() = 0;

		virtual void close() = 0;

		/**
		 *	Set the lowest level of the priority will be log.
		 *	@param priority	The level of priority.
		 *	@see priority_level
		 */
		void set_threshold(int priority)
		{
			threshold_ = priority;
		}

		/**
		 *	Get the lowest level of the priority will be log.
		 *	@see priority_level
		 */
		int get_threshold()
		{
			return threshold_;
		}

		/**
		 *	Get the name of the appender.
		 */
		const std::string& get_name(void)const
		{
			return name_;
		}

	protected:
		appender(const std::string& name);

		virtual void _append(const logging_event& event) = 0;

	private:
		const std::string	name_;

		int					threshold_;
	};

	


}
#endif//LOGGING_APPENDER_H__
