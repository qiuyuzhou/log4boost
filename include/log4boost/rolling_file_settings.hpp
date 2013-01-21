#ifndef __LOG4BOOST_ROLLING_FILE_SETTING_HPP__2075__
#define __LOG4BOOST_ROLLING_FILE_SETTING_HPP__2075__


#include <log4boost/config.hpp>

#include <ios>
#include <string>


namespace log4boost
{

	/** 
	Settings you can pass to the rolling file.
	*/
	struct rolling_file_settings {

		rolling_file_settings( const std::string& file_path ) 
			: max_size_bytes(1024 * 1024)
			, new_file_by_date(false)
			, file_path(file_path)
			, file_count(10)
			, initial_erase(false)
			, flush_each_time(true)
			, extra_flags(std::ios_base::out)
			, extension_name("log")
		{}

		/// Maximum size in bytes, by default 1Mb.
		int max_size_bytes;
		/// Create a new file when a new day coming
		bool new_file_by_date;
		/// How many files has a rolling file, by default, 10.
		int file_count;
		/// If true, it initially erases all files from the rolling file (by default, false).
		bool initial_erase;

		/// If true, always flush after write (by default, true).
		bool flush_each_time;

		/// Just in case you have some extra flags to pass, when opening each file.
		std::ios_base::openmode extra_flags;

		/// The path where the file would be created,and the prefix of the filename.
		std::string				file_path;

		/// The name to be used as suffix for the files.
		std::string				extension_name;
	};

}


#endif//__LOG4BOOST_ROLLING_FILE_SETTING_HPP__2075__

