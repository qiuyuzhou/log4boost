/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/config_watch_dog.hpp"

#include "log4boost/detail/singleton.hpp"
#include "log4boost/configurator.hpp"
#include "log4boost/log4boost.hpp"

namespace log4boost
{


	config_file_watch_dog::config_file_watch_dog( const boost::filesystem::path& file_path, const std::string& preconfig_string )
		: file_watch_dog( file_path )
		, preconfig_string_( preconfig_string )
		, do_preconfig_(true)
	{
	}

	config_file_watch_dog::config_file_watch_dog( const boost::filesystem::path& file_path )
		: file_watch_dog( file_path )
		, do_preconfig_(false)
	{

	}

	void config_file_watch_dog::on_changed()
	{
		if ( do_preconfig_ )
		{
			LOG_INFO("loglog","config_file_watch_dog - Log config file '" << get_file_path().string() 
				<< "' have been changed. Reload the config file with the preconfig string '" << preconfig_string_ << "'." );
			properties_configure( get_file_path().string(), preconfig_string_ );
		}
		else
		{
			LOG_INFO("loglog","config_file_watch_dog - Log config file '" << get_file_path().string() << "' have been changed. Reload the config file.");
			properties_configure( get_file_path().string() );
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------

	void config_watch_dog::configure_and_watch( const boost::filesystem::path& file_path, const std::string& preconfig_string )
	{
		if (config_file_watch_dog_)
		{
			config_file_watch_dog_->stop();
		}
		config_file_watch_dog_.reset( new config_file_watch_dog( file_path, preconfig_string ) );
		config_file_watch_dog_->start();
	}

	void config_watch_dog::configure_and_watch( const boost::filesystem::path& file_path, const std::string& preconfig_string, unsigned long delay )
	{
		if (config_file_watch_dog_)
		{
			config_file_watch_dog_->stop();
		}
		config_file_watch_dog_.reset( new config_file_watch_dog( file_path, preconfig_string ) );
		config_file_watch_dog_->set_delay( delay );
		config_file_watch_dog_->start();
	}

	void config_watch_dog::configure_and_watch( const boost::filesystem::path& file_path )
	{
		if (config_file_watch_dog_)
		{
			config_file_watch_dog_->stop();
		}
		config_file_watch_dog_.reset( new config_file_watch_dog( file_path ) );
		config_file_watch_dog_->start();
	}

	void config_watch_dog::configure_and_watch( const boost::filesystem::path& file_path, unsigned long delay )
	{
		if (config_file_watch_dog_)
		{
			config_file_watch_dog_->stop();
		}
		config_file_watch_dog_.reset( new config_file_watch_dog( file_path ) );
		config_file_watch_dog_->set_delay( delay );
		config_file_watch_dog_->start();
	}

	config_watch_dog* config_watch_dog::instance()
	{
		return detail::singleton<config_watch_dog>::instance();
	}

	void config_watch_dog::stop_watching()
	{
		if (config_file_watch_dog_)
		{
			config_file_watch_dog_->stop();
			config_file_watch_dog_.reset();
		}
	}

	bool config_watch_dog::is_watching() const
	{
		if (config_file_watch_dog_)
		{
			return config_file_watch_dog_->is_watching();
		}
		else
		{
			return false;
		}
	}

}

