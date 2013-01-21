/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_IPC_APPENDER_H___
#define LOGGING_IPC_APPENDER_H___

#include <log4boost/config.hpp>
#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/appender_creator_base.hpp>

#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/scoped_ptr.hpp>


namespace log4boost
{
	/**
	 *	The appender would write the log to the IPC Component of boost message_queue.
	 *	It's high efficient.
	 *
	 *	\par The config options:
	 *	- \b ipc_name		\n The name of message queue used inner.
	 *	- \b max_msg		\n The max size of one log message can be write. Default is 512.
	 *	- \b max_msg_size	\n The max log message can buffered in message queue. Default is 1024.
	 *	- \b layout			\n The layout class type the appender would be used. Default is formatter_layout using DEFAULT_FORMAT.
	 */
	class LOG4BOOST_EXPORT ipc_appender:
		public layout_appender
	{
	public:


		virtual ~ipc_appender();

		virtual void reopen();

		virtual void close();

		static shared_ptr<ipc_appender>	create(const std::string& name,const std::string& ipc_name
			,unsigned int max_msg=512,unsigned int max_msg_size=1024, layout* _layout = 0 );
	private:
		/**
		*	The appender constructor.
		*	@param	name			Appender name.
		*	@param	ipc_name		The name of message queue used inner.
		*	@param	max_msg			The max size of one log message can be write.
		*	@param	max_msg_size	The max log message can buffered in message queue.
		*	@param	_layout			The layout object the appender would be used.If give the NULL,
		*							it would use the default layout
		*/
		ipc_appender(const std::string& name,const std::string& ipc_name
			,unsigned int max_msg=512,unsigned int max_msg_size=1024, layout* _layout = 0 );

		virtual void _append(const logging_event& event);

		std::string		ipc_name_;
		unsigned int	max_msg_;
		unsigned int	max_msg_size_;

		boost::scoped_ptr<boost::interprocess::message_queue>	
						message_queue_;
	};

	class LOG4BOOST_EXPORT ipc_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};

}

#endif//LOGGING_IPC_APPENDER_H___
