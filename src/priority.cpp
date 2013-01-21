/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"

#include "log4boost/priority.hpp"

namespace log4boost
{
	namespace priority
	{
		const char* get_name(priority_level priority)
		{
			static const char* priority_name[]=
			{
				"",
				"TRACE",
				"DEBUG",
				"INFO",
				"WARN",
				"ERROR",
				"FATAL",
			};

			if(priority==PL_ALL)
			{
				return "ALL";
			}else if(priority==PL_OFF)
			{
				return "OFF";
			}

			int p = priority/10000;

			return priority_name[p];
		}

		const char* get_fix_length_name(priority_level priority)
		{
			static const char* priority_name[]=
			{
				"",
				"TRACE",
				"DEBUG",
				"INFO ",
				"WARN ",
				"ERROR",
				"FATAL",
			};

			if(priority==PL_ALL)
			{
				return "ALL";
			}else if(priority==PL_OFF)
			{
				return "OFF";
			}

			int p = priority/10000;

			return priority_name[p];
		}

		priority::priority_level get_level(const std::string& name)
		{
			if(name=="FATAL")
				return PL_FATAL;
			else if(name=="ERROR")
				return PL_ERROR;
			else if(name=="WARN")
				return PL_WARN;
			else if(name=="INFO")
				return PL_INFO;
			else if(name=="DEBUG")
				return PL_DEBUG;
			else if(name=="ALL")
				return PL_ALL;
			else if(name=="OFF")
				return PL_OFF;
			else
				return PL_DEBUG;
		}
	}

}
