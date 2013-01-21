/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_PRIORITY_H__
#define LOGGING_PRIORITY_H__

#include "log4boost/config.hpp"

namespace log4boost
{
	namespace priority
	{
		enum priority_level{
			PL_OFF=100000,
			PL_FATAL=60000,
			PL_ERROR=50000,
			PL_WARN=40000,
			PL_INFO=30000,
			PL_DEBUG=20000,
			PL_TRACE=10000,
			PL_ALL=0,
			PL_NOTSET=-1
		};
		
		LOG4BOOST_EXPORT const char* get_name(priority_level priority);

		LOG4BOOST_EXPORT priority_level get_level(const std::string& name);

		LOG4BOOST_EXPORT const char* get_fix_length_name(priority_level priority);

	};

	typedef priority::priority_level priority_level;

}


#endif//LOGGING_PRIORITY_H__
