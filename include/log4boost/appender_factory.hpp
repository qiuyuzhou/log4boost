/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_APPENDER_FACTORY__H__
#define LOGGING_APPENDER_FACTORY__H__

#include <string>
#include <memory>
#include <map>

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>
#include <log4boost/appender/appender.hpp>
#include <log4boost/appender_creator_base.hpp>

namespace log4boost
{

	/**
	 *	Appender factory used for configuring.
	 */
	class LOG4BOOST_EXPORT appender_factory
		: detail::noncopyable
	{
	public:
		typedef property_tree::ptree params_t;
		typedef shared_ptr<appender> (*create_function_t)(const std::string& name, const params_t& params);

		static appender_factory& get_instance();
		static void destroy_instance();
		void create(const params_t& params);

		void register_creator( appender_creator_base* creator );

	private:
		appender_factory(){};

		typedef std::map<std::string, shared_ptr<appender_creator_base> > creators_t;
		typedef creators_t::const_iterator const_iterator;

		creators_t creators_;
	};

}

#endif//LOGGING_APPENDER_FACTORY__H__
