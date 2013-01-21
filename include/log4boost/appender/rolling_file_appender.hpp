/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/

// Some code from boost log2 destination_rolling_file.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details


#ifndef __LOGGING_ROLL_FILE_APPENDER_HPP___
#define __LOGGING_ROLL_FILE_APPENDER_HPP___

#include <log4boost/config.hpp>
#include <log4boost/detail/mutex.hpp>
#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/appender_creator_base.hpp>
#include <log4boost/rolling_file_settings.hpp>

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

namespace log4boost
{

	class rolling_file_writer;

	/**
	 *	The appender would write the log to a file using specified file name with a index suffix.
	 *	If the file reach the specified size, the index increase, then write to a new file.
	 *	When count of file reach the specified, the index rolling to 0.
	 *
	 *	\par The config options:
		 - \b file_path						\n The path where the file would be created,and the prefix of the filename.
		 - \b layout						\n The layout class type the appender would be used. Default is formatter_layout using DEFAULT_FORMAT.
		 - \b extension_name				\n The name to be used as suffix for the files, default is "log"
		 - \b initial_erase					\n If true, it initially erases all files from the rolling file (by default, false).
		 - \b flush_each_time				\n If true, always flush after write (by default, true).
		 - \b file_count					\n How many files has a rolling file, by default, 10.
		 - \b max_size_bytes				\n Maximum size in bytes, by default 1Mb.
	 *	 - \b new_file_by_date				\n Recreate a new file when a new day coming :-)
	 */
	class rolling_file_appender
		: public layout_appender
	{
	public:

		virtual ~rolling_file_appender();
		/**
		 *	Open the rolling_file_appender.
		 *	@param	settinges	The rolling file settings.
		 */
		void open( const rolling_file_settings& settings );

		void close();

		void reopen();

		/**
		*	Flush the file.
		*/
		void flush();


		static shared_ptr<rolling_file_appender>	create( const std::string& name, layout* _layout=0 );

	protected:
		rolling_file_appender( const std::string& name, layout* _layout=0 );

		void _append(const logging_event& event);

	private:
		mutable mutex	mutex_;
		boost::scoped_ptr<rolling_file_writer>	writer_;
		rolling_file_settings	settings_;
	};

	class LOG4BOOST_EXPORT rolling_file_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};

}


#endif//__LOGGING_ROLL_FILE_APPENDER_HPP___
