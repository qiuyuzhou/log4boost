/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef _LOG4BOOST_APPENDER_CREATOR_BASE_H__
#define _LOG4BOOST_APPENDER_CREATOR_BASE_H__

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>
#include <log4boost/appender/appender.hpp>

namespace log4boost
{

	class LOG4BOOST_EXPORT appender_creator_base : detail::noncopyable
	{
	public:
// Atlas-Modif [Begin]
		virtual ~appender_creator_base()
		{
		}
// Atlas-Modif [End]

		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params) = 0;
		virtual std::string get_name()const = 0;
	};

}


#endif//_LOG4BOOST_APPENDER_CREATOR_BASE_H__

