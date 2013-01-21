/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"

#include "log4boost/layout_factory.hpp"
#include "ptree_inc.h"

#include <stdexcept>

#include "log4boost/log4boost.hpp"
#include "log4boost/detail/mutex.hpp"


namespace log4boost
{
	std::auto_ptr<layout> create_simple_xml_layout(const property_tree::ptree& params);
	std::auto_ptr<layout> create_formatter_layout(const property_tree::ptree& params);
	std::auto_ptr<layout> create_binary_layout(const property_tree::ptree& params);
	std::auto_ptr<layout> create_log4j_xml_layout( const property_tree::ptree& params );



	static layout_factory* layout_factory_ = 0;

	layout_factory& layout_factory::get_instance()
	{
		if (!layout_factory_)
		{
			static mutex	instance_mutex;
			mutex::scoped_lock lock(instance_mutex);
			if (!layout_factory_)
			{
				std::auto_ptr<layout_factory> af(new layout_factory);

				af->register_creator("simple_xml_layout",create_simple_xml_layout);
				af->register_creator("formatter_layout",create_formatter_layout);
				af->register_creator("binary_layout",create_binary_layout);
				af->register_creator("log4j_xml_layout", create_log4j_xml_layout);

				layout_factory_ = af.release();
			}
		}
		return *layout_factory_;
	}

	void layout_factory::destroy_instance()
	{
		delete layout_factory_;
		layout_factory_ = 0;
	}

	std::auto_ptr<layout> layout_factory::create(const params_t& params)
	{
		try
		{
			std::string class_name = params.data();

			const_iterator i = creators_.find(class_name);
			if (i == creators_.end())
			{
				LOG_ERROR("loglog","There is no layout with type name '" << class_name << "'");
				return std::auto_ptr<layout>();
			}

			return (*i->second)(params);
		}
		catch(const property_tree::ptree_error& e)
		{
			throw std::invalid_argument(std::string("Layout params error! - ")+e.what());
		}
	}

	void layout_factory::register_creator(const std::string& class_name, create_function_t create_function)
	{
		const_iterator i = creators_.find(class_name);
		if (i != creators_.end())
			throw std::invalid_argument("Layout creator for type name '" + class_name + "' already registered");

		creators_[class_name] = create_function;
	}

}
