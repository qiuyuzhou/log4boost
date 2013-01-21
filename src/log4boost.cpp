/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/log4boost.hpp"

#include "log4boost/logger.hpp"
#include "log4boost/appender/appender.hpp"
#include "log4boost/receiver/receiver.hpp"

namespace log4boost
{
	namespace detail
	{
		extern bool g_shutdown;
	}

	void shutdown()
	{
		detail::g_shutdown = true;
		logger::remove_all();
		appender::remove_all();
		receiver::remove_all();
	}

}


