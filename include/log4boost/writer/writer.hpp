/* Copyright (C) 2007 - 2010  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOG4BOOST_WRITER_HPP__32466_
#define LOG4BOOST_WRITER_HPP__32466_

#include <log4boost/config.hpp>


namespace log4boost
{

	class LOG4BOOST_EXPORT writer
	{
	public:
		virtual ~writer(){}

		virtual void close() = 0;
		virtual void write( const char* str, std::size_t len ) = 0;
	};


}


#endif//LOG4BOOST_WRITER_HPP__32466_
