/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_LAYOUT_FACTORY__H__
#define LOGGING_LAYOUT_FACTORY__H__

#include <string>
#include <memory>
#include <map>

#include <log4boost/config.hpp>
#include <log4boost/layout/layout.hpp>
#include <log4boost/detail/noncopyable.hpp>

namespace log4boost
{

	/**
	 *	Layout factory for configuring.
	 */
	class LOG4BOOST_EXPORT layout_factory
		: detail::noncopyable
	{
	public:
		typedef property_tree::ptree params_t;
		typedef std::auto_ptr<layout> (*create_function_t)(const params_t& params);

		static layout_factory& get_instance();
		static void destroy_instance();
		std::auto_ptr<layout> create(const params_t& params);

		void register_creator(const std::string& class_name, create_function_t create_function);

	private:
		layout_factory(){};

		typedef std::map<std::string, create_function_t> creators_t;
		typedef creators_t::const_iterator const_iterator;

		creators_t creators_;
	};
}


#endif//LOGGING_LAYOUT_FACTORY__H__
