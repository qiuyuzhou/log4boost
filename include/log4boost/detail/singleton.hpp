/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef P2PTS_UTILITIES_SINGLETON_H__
#define P2PTS_UTILITIES_SINGLETON_H__

#include <list>
#include <log4boost/detail/mutex.hpp>

namespace log4boost
{
	namespace detail
	{
		class singleton_object
		{
		public:
			singleton_object();
			
			virtual void clean()=0;
		};

		class singleton_object_cleaner
		{
		public:
			virtual ~singleton_object_cleaner();

			static void add(singleton_object* o);

			static mutex& get_mutex();

			static void clean_all();
		private:
			std::list<singleton_object*>	objects_;

			mutable mutex	mutex_;
		};

		template<typename T>
		class singleton
			:public detail::singleton_object
		{
		public:

			static T* instance();
            static bool has_instance();

			static void instance( singleton<T>* obj );

			static singleton<T>* singleton_instance();

			virtual void clean();

		private:
			singleton(void){};

			T	instance_;

			static singleton<T>* singleton_;
		};	

		template<typename T>
		singleton<T>* singleton<T>::singleton_instance()
		{
			instance();
			return &(singleton_->instance_);
		}

		template<typename T>
		void singleton<T>::instance( singleton<T>* obj )
		{
			mutex::scoped_lock lock(detail::singleton_object_cleaner::get_mutex());
			if(singleton_==0)
			{
				singleton_ = obj;
			}
			else
			{
				singleton<T>* old = singleton_;
				singleton_ = obj;
				delete old;
			}
		}

		template<typename T> singleton<T>*
			singleton<T>::singleton_ = 0;

		template<typename T>
		T* singleton<T>::instance()
		{
			if(singleton_==0)
			{
				mutex::scoped_lock lock(detail::singleton_object_cleaner::get_mutex());
				if(singleton_==0)
				{
					singleton_ = new singleton<T>();
				}
			}
			return &(singleton_->instance_);
		}

        template<typename T>
        bool singleton<T>::has_instance()
        {
            return singleton_!=0;
        }

		template<typename T>
		void singleton<T>::clean()
		{
			delete this;
			singleton_ = 0;
		}
	}
}

#endif//P2PTS_UTILITIES_SINGLETON_H__
