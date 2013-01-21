/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_FILE_APPENDER_H__
#define LOGGING_FILE_APPENDER_H__

#include <log4boost/config.hpp>
#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/appender_creator_base.hpp>

#include <log4boost/detail/mutex.hpp>
#include <boost/scoped_ptr.hpp>

#include <fstream>

namespace log4boost
{

	class file_writer;


	/**
	 	The appender would write the log to the file.
	 
	 	The config options:
	 	- \b filename			\n The name of log file.
	 	- \b append				\n If overwrite the file when open. Default is \e false.
	 	- \b flush_each_time	\n If flush the file each time when writing. Default is \e true.
		- \b layout				\n The layout class type the appender would be used. Default is formatter_layout using DEFAULT_FORMAT.
	 
		\verbatim
		log4boostappender.fileAppender=file_appender
		log4boostappender.fileAppender.filename=log.txt
		log4boostappender.fileAppender.append=1
		log4boostappender.fileAppender.flush_each_time=1
		\endverbatim
	 */
	class LOG4BOOST_EXPORT file_appender
		:public layout_appender
	{
	public:


		virtual ~file_appender();

		/**
		 *	Open the appender. It would open the file. If there is any error, it will throw a exception.
		 *	@param	file_name		The name of log file.
		 *	@param	append			If overwrite the file when open.
		 *	@param	flush_each_time	If flush the file each time when writing.
		 */
		void open( const char* file_name, bool append, bool flush_each_time = true );

		void close();

		void reopen();

		/**
		 *	Flush the file.
		 */
		void flush();

		bool get_flush_each_time() const { return flush_each_time_; }
		void set_flush_each_time(bool val) { flush_each_time_ = val; }

		static shared_ptr<file_appender>	create( const std::string& name, layout* _layout=0 );

	protected:
		/**
		*	The appender constructor.
		*	@param	name			Appender name.
		*	@param	_layout			The layout object the appender would be used.If give the NULL,
		*							it would use the default layout
		*/
		file_appender(const std::string& name, layout* _layout=0);

		void _append(const logging_event& event);

	protected:
		mutable mutex	mutex_;

		std::string				file_name_;
		bool					append_;
		bool					flush_each_time_;

		boost::scoped_ptr<file_writer>	writer_;
	};

	class LOG4BOOST_EXPORT file_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};
}


#endif//LOGGING_FILE_APPENDER_H__
