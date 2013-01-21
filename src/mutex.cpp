//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2010 by Epic Games China & Titan Studios Inc.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "log4boost/detail/mutex.hpp"

#include <boost/thread/mutex.hpp>

namespace log4boost
{
	mutex::mutex()
	{
		BOOST_STATIC_ASSERT(sizeof(mutex_buffer_) >= sizeof(boost::mutex));
		new(&mutex_buffer_) boost::mutex;
	}

	mutex::~mutex()
	{
		get_mutex()->~mutex();
	}

	boost::mutex* mutex::get_mutex()
	{
		return reinterpret_cast<boost::mutex*>( &mutex_buffer_ );
	}

	mutex::scoped_lock::scoped_lock( mutex& mtx )
	{
		BOOST_STATIC_ASSERT(sizeof(lock_buffer_) >= sizeof(boost::unique_lock<boost::mutex>));

		new(&lock_buffer_) boost::unique_lock<boost::mutex>(*mtx.get_mutex());
	}

	mutex::scoped_lock::~scoped_lock()
	{
		reinterpret_cast<boost::unique_lock<boost::mutex>*>(&lock_buffer_)->~unique_lock();
	}

	void mutex::scoped_lock::lock()
	{
		reinterpret_cast<boost::unique_lock<boost::mutex>*>(&lock_buffer_)->lock();
	}

	void mutex::scoped_lock::unlock()
	{
		reinterpret_cast<boost::unique_lock<boost::mutex>*>(&lock_buffer_)->unlock();
	}
}
