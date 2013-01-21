/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_CONFIGURATOR_H__
#define LOGGING_CONFIGURATOR_H__

#include <string>
#include <iostream>
#include <stdexcept>

#include <log4boost/config.hpp>


/*! \page log_configure Configure the logging system via config file
The config format is similar to log4j. Also use properties file format.

\verbatim
log4boostappender.[appender name]=[appender class name]
log4boostappender.[appender name].[appender parameter name]=[appender parameter value]

log4boostrootlogger=[logger parameters]
log4boostlogger.[logger name]=[logger parameters]
log4boostadditive.[logger name]=[ 1 | 0 ]
\endverbatim

The format of [logger parameters] is: [ priority ],[appender name list]

An example:

\verbatim
log4boost.appender.fileAppender=file_appender
log4boost.appender.fileAppender.filename=log.txt
log4boost.appender.fileAppender.append=1
log4boost.appender.fileAppender.flush_each_time=1

log4boost.appender.coutAppender=ostream_appender
log4boost.appender.coutAppender.ostream=stdout
log4boost.appender.coutAppender.layout=formatter_layout
log4boost.appender.coutAppender.layout.format=%time%($hh:$mm:$ss) [%priority%width:5%] %logger_name%width:16% - %message%\n

log4boost.logger.debug.n1=DEBUG,fileAppender,coutAppender
log4boost.logger.debug=DEBUG,fileAppender,coutAppender

log4boost.additive.debug=0

\endverbatim

To load config file like this:

\code
#include "log4boost/log4boosthpp"
#include "log4boost/logger.hpp"
#include "log4boost/configurator.hpp"

void test_configurator(void)
{
	using namespace log4boost;

	properties_configure("logging_conf.properties");
}

\endcode

\par All appender the lib provided:
- log4boost::file_appender
- log4boost::rolling_file_appender
- log4boost::console_appender
- log4boost::win32_debug_appender
- log4boost::ipc_appender
- log4boost::udp_appender
- log4boost::tcp_appender
- log4boost::log_agent_appender

\par All layout the lib provided:
- log4boost::formatter_layout
- log4boost::simple_xml_layout
- log4boost::log4j_xml_layout
- log4boost::binary_layout

*/


namespace log4boost
{

	/**
	 *	The exception class for throwing when the configure failed.
	 */
	class LOG4BOOST_EXPORT configure_failure
		:public std::runtime_error
	{
	public:
		configure_failure(const std::string& message);
	};

	/**
	 *	Load the properties config from file for the logging system.
	 */
	LOG4BOOST_EXPORT void properties_configure(const std::string& config_file_path);

	/**
	 *	Load the properties config from stream for the logging system.
	 */
	LOG4BOOST_EXPORT void properties_configure(std::istream& config_file);

	/**
	 *	Load the properties config from file for the logging system.
	 *	Replace all ${KEY} with the value of the "KEY" before configureing.If the value is not existed, then replace it with empty.
	 *	@param preconfig_string is a list of "key=value" which separate by ";".
	 */
	LOG4BOOST_EXPORT void properties_configure( const std::string& config_file_path, const std::string& preconfig_string );

	/**
	 *	Load the properties config from stream for the logging system.
	 *	Replace all ${KEY} with the value of the "KEY" before configureing.If the value is not existed, then replace it with empty.
	 *	@param preconfig_string is a list of "key=value" which separate by ";".
	 */
	LOG4BOOST_EXPORT void properties_configure( std::istream& config_file, const std::string& preconfig_string );


	/**
	 *	Load the properties config from file for the logging system with pre-configuring.
	 *	Every "delay" milliseconds it will check the file. If it have been changed, then do a re-configuring.
	 *	@param delay The unit is milliseconds.
	 */
	LOG4BOOST_EXPORT void properties_configure_and_watch(const std::string& config_file_path, unsigned long delay );

	/**
	 *	Load the properties config from file for the logging system with pre-configuring.
	 *	Replace all ${KEY} with the value of the "KEY" before configureing.If the value is not existed, then replace it with empty.
	 *	Every "delay" milliseconds it will check the file. If it have been changed, then do a re-configuring.
	 *	@param preconfig_string is a list of "key=value" which separate by ";".
	 *	@param delay The unit is milliseconds.
	 */
	LOG4BOOST_EXPORT void properties_configure_and_watch(const std::string& config_file_path, const std::string& preconfig_string, unsigned long delay );

}

#endif//LOGGING_CONFIGURATOR_H__
