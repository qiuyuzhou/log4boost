/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/configurator.hpp"
#include <fstream>

#include <stdexcept>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/filesystem/fstream.hpp>

#include "ptree_inc.h"

#include "log4boost/logger.hpp"
#include "log4boost/appender_factory.hpp"
#include "log4boost/layout_factory.hpp"
#include "log4boost/log4boost.hpp"
#include "log4boost/config_watch_dog.hpp"

namespace log4boost
{

	configure_failure::configure_failure(const std::string& message)
		:std::runtime_error(std::string("configure failure : ")+message)
	{
	}

	static void create_logger(const std::string& logger_name,const std::string& logger_config)
	{
		using namespace boost;
		char_separator<char> sep(",;");
		tokenizer<char_separator<char> >	tok(logger_config,sep);
		std::vector<std::string>	toks;
		for(tokenizer<char_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
		{
			toks.push_back(*beg);
		}

		logger& l = logger::get(logger_name);
		
		if(toks.size()>=1)
		{
			priority::priority_level prior_level;
			prior_level = priority::get_level(boost::algorithm::to_upper_copy(toks[0]));
			l.set_priority(prior_level);
		}

		{
			l.remove_all_appender();
			for(unsigned int i=1;i<toks.size();++i)
			{
				shared_ptr<appender> p = appender::get_appender(toks[i]);
				if(p==0)
				{
					LOG_WARN( "loglog", boost::format("Appender called \'%1%\' havn't defined.")%toks[i] );
					continue;
				}
				l.add_appender(p);
			}
		}
	}

	static void create_logger(const std::string& logger_name,const property_tree::ptree& pt)
	{
		using namespace property_tree;

		std::string config = pt.data();

		if(!config.empty())
		{
			create_logger(logger_name,config);
		}
		
		for(ptree::const_iterator itr=pt.begin(),enditr=pt.end()
			;itr!=enditr;++itr)
		{
			create_logger(logger_name+'.'+itr->first,itr->second);
		}
	}

	static void set_additive(const std::string& logger_name,const property_tree::ptree& pt)
	{
		using namespace property_tree;

		std::string config = pt.data();

		if(!config.empty())
		{
			bool is_additive = boost::lexical_cast<bool>( config );
			logger::get( logger_name ).set_additive( is_additive );
		}

		for(ptree::const_iterator itr=pt.begin(),enditr=pt.end()
			;itr!=enditr;++itr)
		{
			create_logger(logger_name+'.'+itr->first,itr->second);
		}
	}

	static void configure(const property_tree::ptree& pt)
	{
		using namespace property_tree;

		const ptree& root_pt = pt;

		boost::optional<const ptree&> logging_pt = root_pt.get_child_optional("log4boost");
		if(!logging_pt)
		{
			return;
		}

		appender::remove_all();
		boost::optional<const ptree&> appender_pt = logging_pt->get_child_optional("appender");
		if(appender_pt)
		{
			appender_factory::get_instance().create(*appender_pt);
		}

		boost::optional<const ptree&> root_logger_pt = logging_pt->get_child_optional("rootlogger");
		if(root_logger_pt)
		{
			create_logger("",root_logger_pt->data());
		}

		boost::optional<const ptree&> logger_pt = logging_pt->get_child_optional("logger");
		if(logger_pt)
		{
			for(ptree::const_iterator itr=logger_pt->begin(),enditr=logger_pt->end()
				;itr!=enditr;++itr)
			{
				create_logger(itr->first,itr->second);
			}
		}

		boost::optional<const ptree&> additive_pt = logging_pt->get_child_optional("additive");
		if(additive_pt)
		{
			for(ptree::const_iterator itr=additive_pt->begin(),enditr=additive_pt->end()
				;itr!=enditr;++itr)
			{
				set_additive(itr->first,itr->second);
			}
		}

		layout_factory::destroy_instance();
		appender_factory::destroy_instance();
	}

	template <typename Ptree>
	void read_properties(std::istream& in,Ptree & pt)
	{
		using namespace std;
		using namespace boost;
		std::string line;

		while(!in.eof())
		{
			getline(in,line);
			algorithm::trim(line);
			if(line.empty())
				continue;
			if(line[0]=='#')
				continue;

			size_t pos = line.find('=');
			if(pos==string::npos)
			{
				throw std::runtime_error("Error format!");
			}

			string key,value;
			key = line.substr(0,pos);
			value = line.substr(pos+1);
			algorithm::trim(key);
			algorithm::trim(value);
			pt.put(key,value,true);
		}
	}

	void properties_configure( std::istream& config_file )
	{
		using namespace property_tree;
		try
		{
			ptree root_pt;
			read_properties(config_file,root_pt);

			configure(root_pt);
		}
		catch(const std::exception& e)//TOFIX
		{
			throw configure_failure(std::string("parse logging config file error:")+e.what());
		}

	}

	void properties_configure( const std::string& config_file_path )
	{
		boost::filesystem::ifstream config_file(config_file_path);

		if (!config_file) {
			throw configure_failure(std::string("Config File ") + config_file_path + " does not exist or is unreadable");
		}

		properties_configure(config_file);
	}

	void properties_configure( const std::string& config_file_path, const std::string& preconfig_string )
	{
		boost::filesystem::ifstream config_file(config_file_path);

		if (!config_file) {
			throw configure_failure(std::string("Config File ") + config_file_path + " does not exist or is unreadable");
		}

		properties_configure(config_file, preconfig_string);
	}

	void preconfig( std::string& config_string, const std::string& preconfig_string )
	{
		using namespace boost;
		char_separator<char> sep(";");
		tokenizer<char_separator<char> >	tok(preconfig_string,sep);
		for(tokenizer<char_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
		{
			std::string kv = *beg;
			boost::algorithm::trim(kv);
			if (kv.empty())
				continue;

			size_t pos = kv.find('=');
			if( pos == std::string::npos )
			{
				throw configure_failure("Error format of preconfig string!");
			}

			std::string key,value;
			key = kv.substr(0,pos);
			value = kv.substr(pos+1);
			boost::algorithm::trim(key);
			boost::algorithm::trim(value);

			boost::algorithm::replace_all(config_string,(boost::format("${%1%}")%key).str(),value);
		}

		boost::regex	r("\\$\\{(\\w+)\\}");
		boost::algorithm::replace_all_regex(config_string,r,std::string());
	}

	void properties_configure( std::istream& config_file, const std::string& preconfig_string )
	{
		using namespace property_tree;
		try
		{
			config_file.seekg(0,std::ios_base::end);
			std::size_t filesize = config_file.tellg();
			config_file.seekg(0,std::ios_base::beg);

			if (filesize==0)
				return;

			std::string config_string;
			config_string.resize(filesize);
			config_file.read(&config_string[0],filesize);

			preconfig(config_string,preconfig_string);
			config_string = config_string.c_str();

			std::istringstream iss(config_string);

			ptree root_pt;
			read_properties(iss,root_pt);

			configure(root_pt);
		}
		catch(const std::exception& e)//TOFIX
		{
			throw configure_failure(std::string("parse logging config file error:")+e.what());
		}
	}

	void properties_configure_and_watch( const std::string& config_file_path, unsigned long delay )
	{
		properties_configure( config_file_path );
		config_watch_dog::instance()->configure_and_watch( config_file_path, delay );
	}

	void properties_configure_and_watch( const std::string& config_file_path, const std::string& preconfig_string, unsigned long delay )
	{
		properties_configure( config_file_path, preconfig_string );
		config_watch_dog::instance()->configure_and_watch( config_file_path, preconfig_string, delay );
	}
}
