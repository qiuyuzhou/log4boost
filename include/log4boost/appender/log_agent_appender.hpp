/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef LOGGING_LOG_AGENT_APPENDER_H___
#define LOGGING_LOG_AGENT_APPENDER_H___

#include <log4boost/config.hpp>

#include <log4boost/detail/mutex.hpp>
#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/appender_creator_base.hpp>
#include <log4boost/rolling_file_settings.hpp>

#include <boost/any.hpp>


namespace log4boost
{

	/**
	 *	The appender output the log to another process through shared memory. The agent in another process
	 *	is responsible for writing log from shared memory to file. The agent process would terminate after
	 *	the calling of appender close member function or the main process has been exited.
	 *
	 *	\par The config options:
	 *	 - \b layout				\n The layout class type the appender would be used. Default is formatter_layout using DEFAULT_FORMAT.
	 *	 - \b writer_type			\n The writer should be used by the agent.Could be: file_writer,rolling_file_writer,tcp_writer
	 *
	 *	\par The config options for file writer:
	 *	- \b file_path				\n The file name and path of log file.
	 *	- \b append					\n If overwrite the file when open. Default is \e false.
	 *
	 *	\par The config options for rolling file writer:
	 *	 - \b file_path				\n The path where the file would be created,and the prefix of the filename.
	 *	 - \b extension_name		\n The name to be used as suffix for the files, default is "log"
	 *	 - \b initial_erase			\n If true, it initially erases all files from the rolling file (by default, false).
	 *	 - \b file_count			\n How many files has a rolling file, by default, 10.
	 *	 - \b max_size_bytes		\n Maximum size in bytes, by default 1Mb.
	 *	 - \b new_file_by_date		\n Recreate a new file when a new day coming :-)
	 *
	 *  \par The config options for tcp writer:
	 *		- \b host	The ip or domain name of the tcp server.
	 *		- \b port	The port of the tcp server.
	 */
	class LOG4BOOST_EXPORT log_agent_appender
		: public layout_appender
	{
	public:
		enum writer_types{
			wt_file,
			wt_rolling_file,
			wt_tcp,
		};

		struct LOG4BOOST_EXPORT agent_settings
		{
			agent_settings( writer_types writer_type )
				:writer_type(writer_type)
			{
				assert(writer_type==wt_file||writer_type==wt_rolling_file||wt_tcp);
			}

			writer_types				writer_type;
		};

		struct LOG4BOOST_EXPORT file_writer_settings
		{
			file_writer_settings( const std::string& file_path )
				:file_path(file_path)
				,append(true)
			{}

			/// The path where the file would be created,and the prefix of the filename.
			std::string				file_path;

			/// If overwrite the file when open.
			bool					append;
		};

		struct LOG4BOOST_EXPORT tcp_writer_settings
		{
			tcp_writer_settings()
				:port(0)
			{}

			tcp_writer_settings( const std::string& host, uint16_t port )
				:host(host)
				,port(port)
			{}

			std::string		host;
			uint16_t	port;
		};

		virtual ~log_agent_appender();

		void open( const agent_settings& _agent_settings, const boost::any& writer_settings );

		virtual void reopen();

		virtual void close();

		static shared_ptr<log_agent_appender>	create( const std::string& name, layout* _layout=0 );

	private:
		log_agent_appender( const std::string& name, layout* _layout = 0 );
		virtual void _append(const logging_event& event);

		void reopen_i();
		void close_i();


		agent_settings	agent_settings_;
		boost::any		writer_settings_;
		bool			process_full_log;

		mutable mutex	mutex_;
		int fdpipe_[2];
	};



	class LOG4BOOST_EXPORT log_agent_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};
}




#endif//LOGGING_LOG_AGENT_APPENDER_H___

