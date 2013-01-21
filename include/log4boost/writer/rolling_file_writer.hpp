/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef __LOG4BOOST_ROLLING_FILE_WRITER_HPP_230650__
#define __LOG4BOOST_ROLLING_FILE_WRITER_HPP_230650__


#include <log4boost/config.hpp>

#include <log4boost/writer/writer.hpp>
#include <log4boost/rolling_file_settings.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>

#include <fstream>

namespace log4boost
{

	class LOG4BOOST_EXPORT rolling_file_writer
		: public writer
	{
	public:
		rolling_file_writer();

		void open( const rolling_file_settings& settinges );

		virtual void close();
		virtual void write( const char* str, std::size_t len );

		void check_renew_file_pre_log(const tm* cur_tm = NULL);
		void check_renew_file_post_log(const tm* cur_tm = NULL);

		std::ostream& out_stream()
		{
			ensure_out_file();
			return *out_file_;
		}
	private:
		void ensure_out_file();

		void restart();
		boost::filesystem::path file_name(int idx);
		boost::filesystem::path file_name_by_date(const tm& file_tm, bool cur_file);
		void recreate_file();
		bool rename_file_with_retry(const boost::filesystem::path & from_path, const boost::filesystem::path & to_path);

	private:
		boost::scoped_ptr< std::ofstream > out_file_;
		rolling_file_settings	settinges_;
		tm		last_tm_;
		boost::filesystem::path date_file_;
		boost::filesystem::path date_tmp_file_;

		friend class rolling_file_appender;
	};

}


#endif//__LOG4BOOST_ROLLING_FILE_WRITER_HPP_230650__

