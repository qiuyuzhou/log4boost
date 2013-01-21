/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#define WIN32_LEAN_AND_MEAN


#define _CRT_SECURE_NO_WARNINGS 1
#define _SCL_SECURE_NO_WARNINGS 1



//boost
#include <boost/cstdint.hpp>
#include <boost/bind.hpp>
//#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/array.hpp>

//STD
#include <string>
#include <stack>
#include <set>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <memory>

#include <boost/asio.hpp>

#include "log4boost/config.hpp"
