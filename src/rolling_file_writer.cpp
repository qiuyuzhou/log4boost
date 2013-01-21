/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"

#include "log4boost/writer/rolling_file_writer.hpp"
#include "log4boost/logging_event.hpp"

#include <boost/filesystem.hpp>

namespace log4boost
{


	rolling_file_writer::rolling_file_writer()
		:settinges_("")
	{
		namespace fs = boost::filesystem;

        /*
		if ( fs::path::default_name_check_writable() )
		{
			// so that we don't get exceptions
			fs::path::default_name_check( fs::no_check);
		}
        */
	}

	bool rolling_file_writer::rename_file_with_retry(const boost::filesystem::path & from_path, const boost::filesystem::path & to_path)
	{
		namespace fs = boost::filesystem;
		bool done_rename = false;
		int try_count = 20;

		// hack to avoid file get locked by other process like SVN/anti-virus
		while ( !done_rename && try_count )
		{
			try
			{
				fs::rename(from_path, to_path);
				done_rename = true;
			}
			catch (const fs::filesystem_error& e)
			{
				//std::cout << "...Rename Fail! from: " << from_path << " to: " << to_path << " retry: " << try_count << std::endl; 

				try_count--;
				boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
			}
		}

		//if ( !done_rename )
		//{
		//	std::cout << "!!!Rename Fail! from: " << from_path << " to: " << to_path << std::endl; 
		//}
		//else
		//{
		//	std::cout << "Rename Success! from: " << from_path << " to: " << to_path << std::endl; 
		//}

		return done_rename;
	}

	void rolling_file_writer::close()
	{
		if (out_file_)
		{
			out_file_->close();
			out_file_.reset();

			if ( settinges_.new_file_by_date )
			{
				rename_file_with_retry( date_tmp_file_, date_file_ );
			}
		}
	}

	void rolling_file_writer::ensure_out_file()
	{
		if (!out_file_)
		{
			time_t cur_time = ::time(0);
			last_tm_ = *localtime( &cur_time );
			recreate_file();
		}
	}

	void rolling_file_writer::write( const char* str, std::size_t len )
	{
		ensure_out_file();

		check_renew_file_pre_log();
		out_file_->write(str,len);
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
					out_file_->flush();
					last_time = this_time;
				}
			}
		}
		check_renew_file_post_log();
	}

	void rolling_file_writer::open( const rolling_file_settings& settinges )
	{
		settinges_ = settinges;

		boost::filesystem::path	filepath( settinges_.file_path );

		if ( !filepath.branch_path().empty() )
		{
			if ( ! boost::filesystem::exists(filepath.branch_path()) )
			{
				boost::filesystem::create_directories(filepath.branch_path());
			}
		}

		time_t cur_time = ::time(0);
		last_tm_ = *localtime( &cur_time );

		restart();
	}

	void rolling_file_writer::restart()
	{
		namespace fs = boost::filesystem;

		if ( settinges_.initial_erase) 
		{
			if ( !settinges_.new_file_by_date )
			{
				for ( int idx = 0; idx < settinges_.file_count; ++idx)
					if ( fs::exists( file_name(idx) ))
						fs::remove( file_name(idx) );
			}
		}

		//recreate_file();
	}

	void rolling_file_writer::recreate_file()
	{
		namespace fs = boost::filesystem;

		if ( settinges_.new_file_by_date )
		{
			date_file_ = file_name_by_date( last_tm_, false);
			date_tmp_file_ = file_name_by_date( last_tm_, true);
			if ( fs::exists(date_tmp_file_) )
			{
				if ( fs::exists(date_file_) || settinges_.initial_erase )
				{
					fs::remove(date_tmp_file_);
				}
			}
			if ( fs::exists(date_file_) )
			{
				if ( settinges_.initial_erase )
				{
					fs::remove(date_file_);
				}
				else
				{
					rename_file_with_retry(date_file_, date_tmp_file_);
				}
			}
			out_file_.reset(new std::ofstream( date_tmp_file_.string().c_str()
				,settinges_.extra_flags | std::ios_base::out | std::ios_base::app));

			*out_file_ << std::left;
		}
		else
		{
			if ( fs::exists(file_name(0)) )
			{
				if ( fs::file_size( file_name(0) )  > settinges_.max_size_bytes )
				{
					for ( int i = settinges_.file_count - 1; i >= 0; --i )
					{
						boost::filesystem::path path = file_name(i);

						if ( i == settinges_.file_count - 1)
						{
							if ( fs::exists(path) )
							{
								fs::remove(path);
							}
						}
						else
						{
							if ( fs::exists(path) )
							{
								rename_file_with_retry(path,file_name(i+1));
							}
						}
					}
				}
			}
			
			out_file_.reset(new std::ofstream( file_name(0).string().c_str()
				,settinges_.extra_flags | std::ios_base::out | std::ios_base::app));

			*out_file_ << std::left;
		}
	}

	boost::filesystem::path rolling_file_writer::file_name( int idx )
	{
		std::ostringstream out; 
		if ( idx > 0)
		{
			out << settinges_.file_path << '.' << (idx); 
			if (!settinges_.extension_name.empty())
			{
				out << '.' << settinges_.extension_name;
			}
		}
		else
		{
			out << settinges_.file_path;
			if (!settinges_.extension_name.empty())
			{
				out << '.' << settinges_.extension_name;
			}
		}
		boost::filesystem::path	filepath( out.str() );
		return filepath;
	}

	boost::filesystem::path rolling_file_writer::file_name_by_date( const tm& file_tm, bool cur_file )
	{
		char buffer[4096];
		sprintf( buffer, "%s_%04d%02d%02d_%02d%02d%02d"
			, settinges_.file_path.c_str()
			, file_tm.tm_year + 1900
			, file_tm.tm_mon + 1
			, file_tm.tm_mday
			, file_tm.tm_hour
			, file_tm.tm_min
			, file_tm.tm_sec
			);

		std::ostringstream out; 
		out << buffer;

		if (cur_file)
		{
			out << ".tmp";
		}
		if (!settinges_.extension_name.empty())
		{
			out << '.' << settinges_.extension_name;
		}
		boost::filesystem::path	filepath( out.str() );
		return filepath;
	}

	void rolling_file_writer::check_renew_file_pre_log(const tm* cur_tm)
	{
		if ( settinges_.new_file_by_date )
		{
			const tm* new_tm;
			if ( cur_tm )
			{
				new_tm = cur_tm;
			}
			else
			{
				time_t val = ::time(0);
				new_tm = localtime( &val);
			}
			if ( new_tm->tm_year > last_tm_.tm_year
				|| new_tm->tm_year == last_tm_.tm_year && new_tm->tm_mon > last_tm_.tm_mon
				|| new_tm->tm_year == last_tm_.tm_year && new_tm->tm_mon == last_tm_.tm_mon && new_tm->tm_mday > last_tm_.tm_mday )
			{
				out_file_->close();
				rename_file_with_retry(date_tmp_file_, date_file_);
				last_tm_ = *new_tm;
				recreate_file();
			}
		}
	}

	void rolling_file_writer::check_renew_file_post_log(const tm* cur_tm)
	{
		if ( !settinges_.new_file_by_date )
		{
			if ( out_file_->tellp() > settinges_.max_size_bytes)
			{
				out_file_->close();
				recreate_file();
			}
		}
	}

}


