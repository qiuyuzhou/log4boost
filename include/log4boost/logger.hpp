/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_LOGGER_H__
#define LOGGING_LOGGER_H__

#include <limits>
#include <string>
#include <type_traits>

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>
#include <log4boost/detail/shared_ptr.hpp>
#include <log4boost/logging_event.hpp>
#include <log4boost/detail/hierarchy_maintainer.hpp>
#include <log4boost/appender/appender.hpp>
#include <log4boost/priority.hpp>

namespace boost { class shared_mutex; }

namespace log4boost
{

	/**
	 *	The logger represent a log category.
	 */
	class LOG4BOOST_EXPORT logger
		:detail::noncopyable
	{
	public:
		virtual ~logger(void);

		static logger& get_root();

		static logger& get(const char* name, size_t name_count);
		static logger& get(const std::string& name);

		static logger& get_dummy();

		static const std::string& get_instance_name();
		static void set_instance_name( const std::string& name );
		static void remove_all();
		static void remove_all_appenders();

		const std::string& get_name()const
		{
			return name_;
		}

		bool has_appenders() const
		{
			return !appenders_.empty();
		}

		logger& set_priority (priority_level priority);

		priority_level get_priority () const;

		logger* 	get_parent ()
		{
			return parent_;
		}

		const logger* 	get_parent ()const
		{
			return parent_;
		}

		priority_level get_chained_priority()const;

		bool is_priority_enabled (priority_level priority) const;

		logger& call_appenders(const logging_event& event);

		bool get_additive()const
		{
			return is_additive_;
		}

		logger& set_additive(bool additivity)
		{
			is_additive_ = additivity;
			return *this;
		}

		logger& add_appender( const shared_ptr<appender>& theappender );

		logger& remove_appender( const shared_ptr<appender>& theappender );

		logger& remove_all_appender();

		boost::shared_mutex& get_mutex();

		logger& log(priority_level priority,const std::string& message);
		
		logger& log(priority_level priority,const std::wstring& message);

		logger& log(priority_level priority,const std::string& message,const char* file,int line);

		logger& log(priority_level priority,const std::wstring& message,const char* file,int line);

		logger& log(priority_level priority,const char* message,std::size_t message_length
			,const char* file,int line);

		logger& log(priority_level priority,const wchar_t* message,std::size_t message_length
			,const char* file,int line);

	protected:
		logger(const char* name,size_t name_count,logger* parent,priority_level priority=priority::PL_ALL);

	private:
		const std::string	name_;
		volatile priority_level		priority_;
		logger*				parent_;
		volatile bool				is_additive_;

		typedef boost::shared_mutex mutex_type;
		mutable std::aligned_storage<64*sizeof(void*), sizeof(void*)>::type	mutex_buffer_;

		typedef std::set<shared_ptr<appender> > appender_set_t;
		appender_set_t		appenders_;

		friend class detail::hierarchy_maintainer;
	};

}

#endif//LOGGING_LOGGER_H__
