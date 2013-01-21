/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "log4boost/detail/singleton.hpp"



namespace log4boost
{
	namespace detail
	{
		singleton_object::singleton_object()
		{
			singleton_object_cleaner::add(this);
		}


		singleton_object_cleaner::~singleton_object_cleaner()
		{
			for(std::list<singleton_object*>::iterator itr=objects_.begin()
				;itr!=objects_.end()
				;++itr)
			{
				(*itr)->clean();
			}
			objects_.clear();
		}

		static singleton_object_cleaner	singleton_object_cleaner_;

		void singleton_object_cleaner::add(singleton_object* o)
		{
			singleton_object_cleaner_.objects_.push_front(o);
		}

		mutex& singleton_object_cleaner::get_mutex()
		{
			return singleton_object_cleaner_.mutex_;
		}

		void singleton_object_cleaner::clean_all()
		{
			for(std::list<singleton_object*>::iterator itr=singleton_object_cleaner_.objects_.begin()
				;itr!=singleton_object_cleaner_.objects_.end()
				;++itr)
			{
				(*itr)->clean();
			}
			singleton_object_cleaner_.objects_.clear();
		}
	}



}
