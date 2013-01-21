/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/detail/hierarchy_maintainer.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/detail/singleton.hpp"

namespace log4boost
{
	namespace detail
	{
		namespace
		{
			static size_t hash_logger_name(const char* name, size_t name_count)
			{
				// hash name[0:name_count] to size_t value by pseudorandomizing transform
				size_t Val = 2166136261U;
				size_t First = 0;
				size_t Last = name_count;
				size_t Stride = 1 + Last / 10;

				if (Stride < Last)
					Last -= Stride;
				for(; First < Last; First += Stride)
					Val = 16777619U * Val ^ (size_t)name[First];
				return (Val);
			}

			static const char* memrchr( const char* buf, int c, size_t count )
			{
				for ( const char* it = buf + count - 1; it >= buf; --it )
				{
					if (*it == c)
						return it;
				}
				return 0;
			}
		}

		hierarchy_maintainer::hierarchy_maintainer(void)
		{
		}

		hierarchy_maintainer::~hierarchy_maintainer(void)
		{
		}

		logger& 	hierarchy_maintainer::get_logger (const char* name, size_t name_count)
		{
			mutex::scoped_lock	lock(logger_map_mutex_);
			return *_get_instance(name, name_count);
		}

		logger* 	hierarchy_maintainer::_get_instance (const char* name, size_t name_count)
		{
			const size_t name_hash = hash_logger_name(name, name_count);

			{
				logger_map_range_t range = logger_map_.equal_range(name_hash);
				for(logger_map_t::const_iterator i = range.first; i != range.second; ++i)
				{
					// Name hash matches, check actual logger name
					logger* l = (*i).second.get();
					if(0==l->get_name().compare(0, std::string::npos, name, name_count))
					{
						return l;
					}
				}
			}

			logger* result;

			if(!name_count)
			{
				result = new logger(name, name_count,0,priority::PL_INFO);
			}
			else
			{
				logger* parent = 0;
				const char* child_name = memrchr( name, '.', name_count );
				if ( child_name )
				{
					size_t parent_name_count = child_name - name;
					parent = _get_instance(name, parent_name_count);
				}
				else
				{
					parent = _get_instance("", 0);
				}
				result = new logger(name, name_count,parent,priority::PL_NOTSET);
			}
			logger_map_.insert(
				logger_map_t::value_type(name_hash,shared_ptr<logger>(result))
				);

			return result;
		}

		void hierarchy_maintainer::remove_all()
		{
			mutex::scoped_lock	lock(logger_map_mutex_);
			logger_map_.clear();
		}

        bool hierarchy_maintainer::has_instance()
        {
            return singleton<hierarchy_maintainer>::has_instance();
        }

		hierarchy_maintainer* hierarchy_maintainer::instance()
		{
			return singleton<hierarchy_maintainer>::instance();
		}

		const std::string& hierarchy_maintainer::get_name() const
		{
			return instance_name_;
		}

		void hierarchy_maintainer::set_name( const std::string& name )
		{
			instance_name_ = name;
		}

		void hierarchy_maintainer::remove_all_appenders()
		{
			mutex::scoped_lock	lock(logger_map_mutex_);

			for ( logger_map_t::iterator itr = logger_map_.begin(); itr!=logger_map_.end() ; ++itr )
			{
				itr->second->remove_all_appender();
			}
		}

		logger& hierarchy_maintainer::get_root()
		{
			return get_logger("", 0);
		}
	}
}
