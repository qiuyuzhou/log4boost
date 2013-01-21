/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/receiver/receiver.hpp"

#include "log4boost/logger.hpp"

#include "log4boost/detail/singleton.hpp"
#include "log4boost/detail/shared_ptr.hpp"
#include "log4boost/detail/mutex.hpp"

#include <map>

#include <boost/bind.hpp>

namespace log4boost
{

	namespace detail
	{

		class LOG4BOOST_EXPORT receivers
		{
		public:
			receivers();
			~receivers();

			void remove_all();

			void add_receiver( const shared_ptr<receiver>& p );

			void remove_receiver( const std::string& name );

			shared_ptr<receiver> get_receiver( const std::string& name )const;

			void close_all();

		private:
			typedef std::map< std::string, shared_ptr<receiver> > receivers_t;
			receivers_t	receivers_;
			mutable mutex	mutex_;
		};

		void receivers::add_receiver( const shared_ptr<receiver>& p )
		{
			mutex::scoped_lock	lock(mutex_);

			std::pair<receivers_t::iterator, bool> ret = receivers_.insert( std::make_pair(p->get_name(),p) );
			if ( !ret.second )
			{
				throw std::runtime_error("Duplicate name of receiver!");
			}
		}

		void receivers::remove_receiver( const std::string& name )
		{
			mutex::scoped_lock	lock(mutex_);

			receivers_t::iterator itr = receivers_.find( name );
			if ( itr!=receivers_.end() )
			{
				itr->second->close();
				receivers_.erase( itr );
			}
		}

		void receivers::remove_all()
		{
			mutex::scoped_lock	lock(mutex_);

			for ( receivers_t::iterator itr = receivers_.begin() 
				; itr!=receivers_.end(); ++itr )
			{
				itr->second->close();
			}

			receivers_.clear();		
		}

		shared_ptr<receiver> receivers::get_receiver( const std::string& name )const
		{
			mutex::scoped_lock	lock(mutex_);

			receivers_t::const_iterator itr = receivers_.find( name );
			if ( itr!=receivers_.end() )
			{
				return itr->second;
			}
			else
			{
				return shared_ptr<receiver>();
			}
		}

		receivers::receivers()
		{

		}
		receivers::~receivers()
		{
			remove_all();
		}
	}

	//--------------------------------------------------------------------------------------------

	receiver::receiver( const std::string& name )
		:name_(name)
	{
	}

	receiver::~receiver()
	{
	}

	void receiver::log( logging_event& event )
	{
		//std::ostringstream target_logger_name_stream;
		//target_logger_name_stream << target_logger_name_;
		//if (!event.get_instance_name().empty())
		//{
		//	target_logger_name_stream << '.' << event.get_instance_name();
		//}
		//if (!event.get_logger_name().empty())
		//{
		//	target_logger_name_stream << '.' << event.get_logger_name();
		//}
		
		//event.get_logger_name() = target_logger_name_stream.str();
		logger::get( event.get_logger_name() ).call_appenders( event );
	}

	shared_ptr<receiver> receiver::get_receiver( const std::string& name )
	{
		return detail::singleton<detail::receivers>::instance()->get_receiver( name );
	}

	void receiver::enable_relog(/* const std::string& target_logger_name */)
	{
		mutex::scoped_lock	lock(mutex_);
		//target_logger_name_ = target_logger_name;

		if ( !relog_signal_connection_.connected() )
		{
			relog_signal_connection_ = signal_logging_event().connect(boost::bind(&receiver::log,this,_1));
		}
	}

	bool receiver::enabled_relog() const
	{
		mutex::scoped_lock	lock(mutex_);
		return relog_signal_connection_.connected();
	}

	void receiver::disable_relog()
	{
		mutex::scoped_lock	lock(mutex_);
		if ( relog_signal_connection_.connected() )
		{
			relog_signal_connection_.disconnect();
		}
	}

	void receiver::remove_all()
	{
        if(detail::singleton<detail::receivers>::has_instance())
        {
		    detail::singleton<detail::receivers>::instance()->remove_all();
        }
	}

	void receiver::add_receiver( const shared_ptr<receiver>& p )
	{
		detail::singleton<detail::receivers>::instance()->add_receiver( p );
	}

	void receiver::remove_receiver( const std::string& name )
	{
		detail::singleton<detail::receivers>::instance()->remove_receiver( name );
	}

}
