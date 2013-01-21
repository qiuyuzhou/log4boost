/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include <string>
#include <stdexcept>

namespace log4boost
{
	namespace detail
	{
		std::string unescape_string(std::string const& s)
		{
			std::string ret;
			for (std::string::const_iterator i = s.begin(); i != s.end(); ++i)
			{
				if (*i != '\\')
				{
					ret += *i;
				}
				else
				{
					++i;
					if (i == s.end())
						throw std::runtime_error("invalid escaped string");

					switch ( *i )
					{
						case 'n':
							ret += "\n";
							break;
						case 't':
							ret += "\t";
							break;
						case 'r':
							ret += "\r";
							break;
						case '\'':
							ret += "\'";
							break;
						case '\"':
							ret += "\"";
							break;
						case '\\':
							ret += "\\";
							break;
						default:
							throw std::runtime_error("invalid escaped string");
							break;
					}
				}
			}
			return ret;
		}

	}
}

