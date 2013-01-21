/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"

#include "log4boost/writer/file_writer.hpp"


#include <boost/filesystem.hpp>


namespace log4boost
{

	file_writer::file_writer()
	{

	}

	void file_writer::open( const char* file_name, bool append )
	{
		if (file_stream_.is_open())
		{
			file_stream_.close();
		}

		std::ios_base::openmode open_mode_ = std::ios_base::out;
		if (!append)
		{
			open_mode_|=std::ios_base::trunc;
		}
		else
		{
			open_mode_|=std::ios_base::app;
		}

		boost::filesystem::path	filepath(file_name);
		if ( ! filepath.branch_path().empty() )
		{
			if ( ! boost::filesystem::exists(filepath.branch_path()) )
			{
				boost::filesystem::create_directories(filepath.branch_path());
			}
		}

		file_stream_.open(file_name,open_mode_);
		file_stream_ << std::left;
	}

	void file_writer::close()
	{
		if (file_stream_.is_open())
		{
			file_stream_.close();
		}
	}

	void file_writer::write( const char* str, std::size_t len )
	{
		file_stream_.write(str, len);

		// auto flush per 5 secs
		static time_t flush_time = 5; 
		{
			static time_t last_time = 0;
			if( last_time == 0 )
			{
				last_time = ::time(0);
			}
			else
			{
				time_t this_time = ::time(0);
				time_t dt = this_time - last_time;

				if( dt > flush_time )
				{
					file_stream_.flush();
					last_time = this_time;
				}
			}
		}
	}

}

