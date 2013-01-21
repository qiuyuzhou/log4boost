/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_UDP_REMOTE_APPENDER_H__
#define LOGGING_UDP_REMOTE_APPENDER_H__

#include <boost/cstdint.hpp>
#include <boost/thread/tss.hpp>

#include <log4boost/config.hpp>
#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/appender_creator_base.hpp>

#include <boost/cstdint.hpp>
#include <boost/thread/tss.hpp>

namespace log4boost
{
	class udp_appender_imp;
	
	/**
	 *	Send the log to remote socket using UDP protocol.
	 *
	 *	\par The config options:
	 *	- \b host or \b ip	\n The host where the log would be send to.
	 *	- \b port			\n The UDP port.
	 *	- \b layout			\n The layout used by the appender. Default is formatter_layout using DEFAULT_FORMAT.
	 */
	class LOG4BOOST_EXPORT udp_appender
		:public layout_appender
	{
	public:

		virtual ~udp_appender(void);

		virtual void reopen();

		virtual void close();

		static shared_ptr<udp_appender>	create(	const std::string& name,layout* l,const std::string& host
			,uint16_t port);

	protected:
		/**
		*	The appender constructor.
		*	
		*	\param name	The name of the appender.
		*	\param l	The layout used by the appender.
		*	\param host	The host where the log would be send to. Could be host name or IP.
		*	\param port	The UDP port.
		*/
		udp_appender(
			const std::string& name
			,layout* l
			,const std::string& host
			,uint16_t port
			);

		virtual void _append(const logging_event& event);

		std::string		host_;
		uint16_t	port_;

		boost::thread_specific_ptr<udp_appender_imp>		imp_;
	};

	class LOG4BOOST_EXPORT udp_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};
}

#endif//LOGGING_UDP_REMOTE_APPENDER_H__
