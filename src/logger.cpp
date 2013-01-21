/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/logger.hpp"
#include "log4boost/ndc.hpp"
#include "utf8.hpp"

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace log4boost
{
	namespace detail
	{
		bool							g_shutdown = false;
	}

	logger& logger::get_root()
	{
		if ( detail::g_shutdown )
		{
			return get_dummy();
		}
		else
		{
			return detail::hierarchy_maintainer::instance()->get_root();
		}
	}

	logger& logger::get_dummy()
	{
		static logger dummy_logger("dummy",5,0,priority::PL_OFF);
		return dummy_logger;
	} 

	logger::logger(const char* name,size_t name_count,logger* parent,priority_level priority)
		:name_(name, name_count)
		,parent_(parent)
		,priority_(priority)
		,is_additive_(true)
	{
		BOOST_STATIC_ASSERT( sizeof(mutex_buffer_) >= sizeof(mutex_type) );
		new(&mutex_buffer_) mutex_type;
		
		if ( name_ == "loglog" )
		{
			is_additive_ = false;
		}
	}

	logger::~logger(void)
	{
		get_mutex().~mutex_type();
	}

	logger::mutex_type& logger::get_mutex()
	{
		return *reinterpret_cast<mutex_type*>( &mutex_buffer_ );
	}

	logger& logger::get(const char* name, size_t name_count)
	{
		if ( detail::g_shutdown )
		{
			return get_dummy();
		}
		else
		{
			return detail::hierarchy_maintainer::instance()->get_logger(name, name_count);
		}
	}

	logger& logger::get(const std::string& name)
	{
		return get(name.c_str(), name.size());
	}

	logger& logger::set_priority (priority_level priority)
	{
		priority_ = priority;
		return *this;
	}

	priority_level logger::get_priority () const
	{
		return priority_;
	}

	priority_level logger::get_chained_priority()const
	{
		if ( priority_ != priority::PL_NOTSET )
		{
			return priority_;
		}
		else
		{
			priority_level pl = priority::PL_ALL;
			const logger* c = this;
			while(c!=0)
			{
				priority_level tmp = c->get_priority();
				if (tmp!=priority::PL_NOTSET)
				{
					return tmp;
				}
				c = c->get_parent();
			}
			return pl;
		}
	}

	bool logger::is_priority_enabled (priority_level priority) const
	{
		if(priority>=get_chained_priority())
			return true;
		else
			return false;
	}

	logger& logger::call_appenders(const logging_event& event)
	{
		if (is_priority_enabled(event.get_priority()))
		{
			{
				boost::shared_lock<mutex_type> lock(get_mutex());

				for(appender_set_t::iterator i=appenders_.begin()
					;i!=appenders_.end()
					;++i)
				{
					(*i)->do_append(event);
				}
			}
			if(this->get_additive()&&get_parent()!=0)
			{
				get_parent()->call_appenders(event);
			}
		}
		return *this;
	}

	logger& logger::add_appender( const shared_ptr<appender>& theappender)
	{
		boost::unique_lock<mutex_type> lock(get_mutex());

		if(theappender!=0)
		{
			appender_set_t::iterator i = appenders_.find(theappender);
			if(i!=appenders_.end())
			{
				return *this;
			}
			else
			{
				for ( appender_set_t::iterator itr = appenders_.begin();
					itr != appenders_.end(); ++itr )
				{
					if ( (*itr)->get_name() == theappender->get_name() )
					{
						appenders_.erase(itr);
						break;
					}
				}
				appenders_.insert(theappender);
			}
			return *this;
		}
		else
		{
			throw std::invalid_argument("NULL appender");
		}
	}

	logger& logger::remove_appender( const shared_ptr<appender>& theappender )
	{
		boost::unique_lock<mutex_type> lock(get_mutex());

		if(theappender!=0)
		{
			appender_set_t::iterator i = appenders_.find(theappender);
			if ( i != appenders_.end() )
			{
				appenders_.erase(i);
			}

			return *this;
		}
		else
		{
			throw std::invalid_argument("NULL appender");
		}
	}

	logger& logger::remove_all_appender()
	{
		boost::unique_lock<mutex_type> lock(get_mutex());

		appenders_.clear();
		return *this;
	}

	logger& logger::log(priority_level priority,const std::string& message)
	{
		if(is_priority_enabled(priority))
		{
			logging_event	event(get_name().c_str(),get_name().length(),message.c_str(),message.length(),priority);
			call_appenders(event);
		}
		return *this;
	}

	logger& logger::log( priority_level priority,const std::wstring& message )
	{
		if(is_priority_enabled(priority))
		{
			char	buf[LOGGING_MAX_MESSAGE_SIZE*4];
			char* end_itr = detail::wchar_utf8(message.begin(),message.end(),buf);
			*end_itr = 0;

			logging_event	event(get_name().c_str(),get_name().length(),buf,end_itr-buf,priority);
			call_appenders(event);
		}
		return *this;
	}

	logger& logger::log( priority_level priority,const std::string& message,const char* file,int line )
	{
		if(is_priority_enabled(priority))
		{
			logging_event	event(get_name().c_str(),get_name().length(),message.c_str(),message.length(),priority,file,line);
			call_appenders(event);
		}
		return *this;
	}

	logger& logger::log( priority_level priority,const std::wstring& message,const char* file,int line )
	{
		if(is_priority_enabled(priority))
		{
			char	buf[LOGGING_MAX_MESSAGE_SIZE*4];
			char* end_itr = detail::wchar_utf8(message.begin(),message.end(),buf);
			*end_itr = 0;

			logging_event	event(get_name().c_str(),get_name().length(),buf,end_itr-buf,priority,file,line);
			call_appenders(event);
		}
		return *this;
	}

	logger& logger::log( priority_level priority,const char* message,std::size_t message_length ,const char* file,int line )
	{
		if(is_priority_enabled(priority))
		{
			logging_event	event(get_name().c_str(),get_name().length(),message,message_length,priority,file,line);
			call_appenders(event);
		}
		return *this;
	}

	logger& logger::log( priority_level priority,const wchar_t* message,std::size_t message_length ,const char* file,int line )
	{
		if(is_priority_enabled(priority))
		{
			char	buf[LOGGING_MAX_MESSAGE_SIZE*4];
			char* end_itr = detail::wchar_utf8(message,message+message_length,buf);
			*end_itr = 0;

			logging_event	event(get_name().c_str(),get_name().length(),buf,(end_itr-buf),priority,file,line);
			call_appenders(event);
		}
		return *this;
	}

	const std::string& logger::get_instance_name()
	{
		return detail::hierarchy_maintainer::instance()->get_name();
	}

	void logger::set_instance_name( const std::string& name )
	{
		detail::hierarchy_maintainer::instance()->set_name( name );
	}

	void logger::remove_all()
	{
        if(detail::hierarchy_maintainer::has_instance())
        {
		    detail::hierarchy_maintainer::instance()->remove_all();
        }
	}

	void logger::remove_all_appenders()
	{
        if(detail::hierarchy_maintainer::has_instance())
        {
		    detail::hierarchy_maintainer::instance()->remove_all_appenders();
        }
	}

}
