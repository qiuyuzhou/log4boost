/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef __LOG4BOOST_FILE_WRITER_HPP_82056_
#define __LOG4BOOST_FILE_WRITER_HPP_82056_

#include <log4boost/config.hpp>

#include <log4boost/writer/writer.hpp>

#include <fstream>


namespace log4boost
{

	class LOG4BOOST_EXPORT file_writer
		: public writer
	{
	public:
		file_writer();

		void open( const char* file_name, bool append );

		virtual void close();

		virtual void write( const char* str, std::size_t len );

		inline void flush()
		{
			file_stream_.flush();
		}
		std::ostream& out_stream()
		{
			return file_stream_;
		}
	private:
		std::ofstream			file_stream_;
	};

}


#endif//__LOG4BOOST_FILE_WRITER_HPP_82056_

