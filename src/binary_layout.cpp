/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/layout/binary_layout.hpp"

#include "log4boost/logging_event.hpp"

#include "log4boost/detail/thread_id.hpp"
#include "log4boost/detail/process_id.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "io.hpp"

namespace log4boost
{

	void binary_layout::format( const logging_event &event,std::ostream& out )
	{
		std::ostringstream	o;
		save( event, o );
		std::string buf = o.str();
		std::ostream_iterator<char> out_itr(out);
		io::write_uint32( buf.size(), out_itr );
		out.write( buf.data(), buf.size() );
	}

	std::auto_ptr<layout> create_binary_layout(const property_tree::ptree& params)
	{
		return std::auto_ptr<layout>(new binary_layout());
	}
}

