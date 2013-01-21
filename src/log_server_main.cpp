/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <string>

#include "log4boost/log4boost.hpp"
#include "log4boost/receiver/tcp_receiver.hpp"
#include "log4boost/appender/console_appender.hpp"
#include "log4boost/configurator.hpp"

#include <boost/asio.hpp>


int main(int argc, char *argv[])
{
	// set the cur dir as exe dir...
	if ( argc >= 1 )
	{
		boost::filesystem::path file_path( argv[0] );
		boost::filesystem::current_path( file_path.branch_path() );
	}

	namespace po = boost::program_options;
	std::string help_str = 
		"Log Server\n\n"
		"Please use tcp_appender with binary_layout in log4boost library to send log to the log server.\nUse log_agent_appender with the tcp_writer and binary_layout is also work.\n\n"
		"When log server running, you could enter the 'q' and press 'enter' key, then the log server would quit.\n\n"
		"Allowed options:";
	
	std::string example_str =
		"Example:\n"
		"  log_server --tcp-listen-host=127.0.0.1 --tcp-listen-port=4448 --log-config-file=log_config.properties\n";

	po::options_description desc(help_str);
	desc.add_options()
		("help", "produce this help message")
		("debug","Print all log from the server.")
		("tcp-listen-host",po::value<std::string>(),"Specify the ip or domain name which the log server should be listened on.")
		("tcp-listen-port",po::value<boost::uint16_t>(),"Specify the port which the log serve should be listened on.")
		("log-config-file",po::value<std::string>(),"Specify the path of config file for log4boost.")
		;

	po::variables_map vm;

	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch( const boost::program_options::unknown_option& e )
	{
		std::cout << e.what() << "\n\n"
			<< desc << "\n"
			<< example_str << std::endl;
		return -1;
	}

	if ( vm.count("help") )
	{
		std::cout << desc << std::endl;
		return 0;
	}

	if ( vm.count("debug") )
	{
		//log4boost::logger::get("loglog").add_appender( log4boost::console_appender::create("loglog_console") );
		//log4boost::logger::get("loglog").set_priority( log4boost::priority::PL_ALL );
	}
	else
	{
		//log4boost::logger::get("loglog").add_appender( log4boost::console_appender::create("loglog_console") );
		//log4boost::logger::get("loglog").set_priority( log4boost::priority::PL_INFO );

		if ( vm.count("tcp-listen-host") == 0 || vm.count( "tcp-listen-port") == 0 || vm.count("log-config-file") == 0 )
		{
			std::cout << desc << "\n"
				<< example_str << std::endl;
			return -1;
		}
	}

	std::string host = "127.0.0.1";
	boost::uint16_t	port = 4448;


	try
	{
		if ( vm.count("tcp-listen-host") )
		{
			host = vm["tcp-listen-host"].as<std::string>();
		}
		if ( vm.count( "tcp-listen-port") )
		{
			port = vm["tcp-listen-port"].as<boost::uint16_t>();
		}


		if ( vm.count("log-config-file") )
		{
			std::string log_config_file_path = vm["log-config-file"].as<std::string>();
			log4boost::properties_configure_and_watch( log_config_file_path, 3000 );
			LOG_INFO("log_server","Configure by file: " << log_config_file_path );
		}

		LOG_INFO("log_server","Listen on Host: " << host << " Port: " << port );

		boost::asio::io_service	io_service;

		log4boost::shared_ptr<log4boost::tcp_receiver> tcp_r = log4boost::tcp_receiver::create( "tcp" );

		tcp_r->open(host,port,0);
		tcp_r->enable_relog();

		std::cout << "Log Server Started\n"; 

		std::string line;
		while( true )
		{
			std::getline( std::cin, line );
			if ( line == "q" )
			{
				break;
			}
		}
		tcp_r->close();
		log4boost::shutdown();
	}
	catch( const std::exception& e )
	{
		std::cerr << "Catch exception:\n"
			<< e.what()
			<< std::endl;

		return -1;
	}

	return 0;
}

