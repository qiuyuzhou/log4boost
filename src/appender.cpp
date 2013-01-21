/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/appender/appender.hpp"

#include "log4boost/logger.hpp"
#include "log4boost/detail/singleton.hpp"
#include "log4boost/detail/mutex.hpp"

#include <map>


namespace log4boost
{
	class appenders
	{
	public:
        static bool has_instance()
        {
            return detail::singleton<appenders>::has_instance();
        }

		static appenders* instance()
		{
			return detail::singleton<appenders>::instance();
		}

		void remove_all();
		void add( const shared_ptr<appender>& theappender );
		void remove( const std::string& name );

		shared_ptr<appender> get( const std::string& name );

		void reopen_all();

	private:
		typedef std::map<std::string,shared_ptr<appender> > appender_map_t;

		appender_map_t	appender_map_;
		mutex			mutex_;
	};

	void appenders::remove_all()
	{
		mutex::scoped_lock	lock(mutex_);
		for(appenders::appender_map_t::iterator i=appenders::instance()->appender_map_.begin()
			;i!=appenders::instance()->appender_map_.end()
			;++i)
		{
			i->second->close();
		}
		appender_map_.clear();
	}

	void appenders::add( const shared_ptr<appender>& theappender)
	{
		mutex::scoped_lock	lock(mutex_);
		appender_map_t::iterator itr = appender_map_.find(theappender->get_name());
		if ( itr != appender_map_.end() )
		{
			throw std::runtime_error( std::string("duplicate appender name: ") + theappender->get_name() );
		}
		else
		{
			appender_map_.insert(
				appender_map_t::value_type(
				theappender->get_name(),theappender
				)
				);
		}
	}

	void appenders::remove(const std::string& name)
	{
		mutex::scoped_lock	lock(mutex_);
		appender_map_.erase(name);
	}

	void appenders::reopen_all()
	{
		mutex::scoped_lock	lock(mutex_);

		for(appenders::appender_map_t::iterator i=appender_map_.begin()
			;i!=appender_map_.end()
			;++i)
		{
			i->second->reopen();
		}
	}

	shared_ptr<appender> appenders::get( const std::string& name )
	{
		mutex::scoped_lock	lock(mutex_);
		appenders::appender_map_t::iterator i = appender_map_.find(name);
		if(i!=appender_map_.end())
			return i->second;
		else
			return shared_ptr<appender>();
	}

	//------------------------------------------------------------------------------------------------------------------------

	appender::appender(const std::string& name)
		:name_(name)
		,threshold_(priority::PL_ALL)
	{
	}

	appender::~appender(void)
	{
	}

	void appender::do_append(const logging_event& event)
	{
		if(priority::PL_ALL==threshold_|| (event.get_priority() >= threshold_))
		{
			_append(event);
		}
	}

	void appender::reopen_all()
	{
		appenders::instance()->reopen_all();
	}

	void appender::remove_all()
	{
		logger::remove_all_appenders();

        if(appenders::has_instance())
        {
		    appenders::instance()->remove_all();
        }
	}

	shared_ptr<appender> appender::get_appender(const std::string& name)
	{
		return appenders::instance()->get(name);
	}

	void appender::add_appender( const shared_ptr<appender>& p )
	{
		appenders::instance()->add( p );
	}

	void appender::remove_appender( const std::string& name )
	{
		appenders::instance()->remove( name );
	}

}
