//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2010 by Epic Games China & Titan Studios Inc.
//------------------------------------------------------------------------------
#ifndef LOGGING_MUTEX_H__
#define LOGGING_MUTEX_H__

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>

#include <memory>
#include <type_traits>

namespace boost { class mutex; }

namespace log4boost
{
	
	class LOG4BOOST_EXPORT mutex
		: detail::noncopyable
	{
	public:
		mutex();
		~mutex();

		struct LOG4BOOST_EXPORT scoped_lock
		{
			explicit scoped_lock( mutex& mtx );
			~scoped_lock();

			void lock();
			void unlock();

		private:
			std::aligned_storage<8*sizeof(void*), sizeof(void*)>::type lock_buffer_;
		};

	private:
		boost::mutex* get_mutex();
		std::aligned_storage<8*sizeof(void*), sizeof(void*)>::type mutex_buffer_;
	};
}

#endif//LOGGING_MUTEX_H__
