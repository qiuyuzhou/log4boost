//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2010 by Epic Games China & Titan Studios Inc.
//------------------------------------------------------------------------------
#ifndef LOGGING_NONCOPYABLE_H__
#define LOGGING_NONCOPYABLE_H__

#include <log4boost/config.hpp>

namespace log4boost
{
	namespace detail
	{
		class LOG4BOOST_EXPORT noncopyable
		{
		protected:
			noncopyable() {}
			~noncopyable() {}
		private:
			noncopyable( const noncopyable& );
			const noncopyable& operator=( const noncopyable& );
		};
	}
}

#endif//LOGGING_NONCOPYABLE_H__
