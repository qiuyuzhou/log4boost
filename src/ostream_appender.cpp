/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/appender/ostream_appender.hpp"



namespace log4boost
{

	ostream_appender::ostream_appender( const std::string& name, std::ostream& stream, layout* _layout )
		:layout_appender(name,_layout)
		,stream_(stream)
	{
		stream_ << std::left;
	}

	ostream_appender::~ostream_appender(void)
	{
		close();
	}

	void ostream_appender::reopen()
	{
	}

	void ostream_appender::close()
	{
	}

	void ostream_appender::_append(const logging_event& event)
	{
		mutex::scoped_lock	lock(mutex_);
		_get_layout().format(event,stream_);
		stream_.flush();
		if(!stream_.good())
		{
		}
	}

	shared_ptr<ostream_appender> ostream_appender::create( const std::string& name, std::ostream& stream, layout* _layout /*= 0 */ )
	{
		shared_ptr<ostream_appender>	p( new ostream_appender( name, stream, _layout ) );
		appender::add_appender( p );
		return p;		
	}

}
