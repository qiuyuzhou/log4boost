/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef LOGGING_ESCAPE_STRING_HPP_INCLUDED
#define LOGGING_ESCAPE_STRING_HPP_INCLUDED

#include <string>

namespace log4boost
{
	namespace detail
	{
		std::string LOG4BOOST_EXPORT unescape_string(std::string const& s);

	}
}

#endif // LOGGING_ESCAPE_STRING_HPP_INCLUDED

