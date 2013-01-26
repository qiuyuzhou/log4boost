/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"

#include "log4boost/appender_factory.hpp"
#include "log4boost/log4boost.hpp"

#include "ptree_inc.h"

#include "log4boost/appender/console_appender.hpp"
#include "log4boost/appender/file_appender.hpp"
//#include "log4boost/appender/ipc_appender.hpp"
#include "log4boost/appender/rolling_file_appender.hpp"
#include "log4boost/appender/udp_appender.hpp"
#if WIN32 || WIN64
    #include "log4boost/appender/win32_debug_appender.hpp"
#endif
#include "log4boost/appender/tcp_appender.hpp"
#include "log4boost/appender/log_agent_appender.hpp"


#include <stdexcept>
//#include <typeinfo.h>


namespace log4boost
{

	static appender_factory* appender_factory_ = 0;

	void appender_factory::register_creator( appender_creator_base* creator )
	{
		std::string class_name = creator->get_name();
		const_iterator i = creators_.find(class_name);
		if (i != creators_.end())
			throw std::invalid_argument("Appender creator for type name '" + class_name + "' already registered");

		creators_[class_name].reset( creator );
	}

	void appender_factory::create(const params_t& params)
	{
		try
		{
			for(params_t::const_iterator itr = params.begin()
				;itr!=params.end()
				;++itr)
			{
				std::string appender_name = itr->first;
				const params_t& param = itr->second;
				std::string class_name = param.data();
				const_iterator i = creators_.find(class_name);
				if (i == creators_.end())
				{
					LOG_ERROR( "loglog", "There is no supporting appender with the type name '" << class_name << "'." );
					continue;
				}

				(*i->second).create_appender(appender_name,param);
			}
		}
		catch(const property_tree::ptree_error& e)
		{
			throw std::invalid_argument(std::string("Appender params error! - ")+e.what());
		}
	}

	appender_factory& appender_factory::get_instance()
	{
		
		if (!appender_factory_)
		{
			static mutex	instance_mutex;
			mutex::scoped_lock lock(instance_mutex);
			if (!appender_factory_)
			{
				std::auto_ptr<appender_factory> af(new appender_factory);

				af->register_creator( new udp_appender_creator() );
				af->register_creator( new file_appender_creator() );
#if defined(WIN32) || defined(WIN64)
				af->register_creator( new win32_debug_appender_creator() );
#endif//WIN32
				af->register_creator( new log_agent_appender_creator() );
				//af->register_creator( new console_appender_creator() );
				//af->register_creator( new ipc_appender_creator() );
				af->register_creator( new rolling_file_appender_creator() );
				af->register_creator( new tcp_appender_creator() );

				appender_factory_ = af.release();
			}
		}
		return *appender_factory_;
	}

	void appender_factory::destroy_instance()
	{
		delete appender_factory_;
		appender_factory_ = 0;
	}

}
