/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#if defined(WIN32) || defined(WIN64)   // only available on Win32

#include "log4boost/appender/win32_debug_appender.hpp"

#include "log4boost/detail/stack_ostream.hpp"
#include "log4boost/layout_factory.hpp"
#include "ptree_inc.h"


namespace log4boost
{

	win32_debug_appender::win32_debug_appender(const std::string& name,layout* _layout)
		:layout_appender(name,_layout)
	{

	}

	win32_debug_appender::~win32_debug_appender()
	{
		close();
	}

	void win32_debug_appender::reopen()
	{
	}

	void win32_debug_appender::close()
	{
	}

	void win32_debug_appender::_append(const logging_event& event)
	{
		detail::basic_stack_ostream<char,LOGGING_MAX_MESSAGE_SIZE+512>	out;
		out << std::left;

		_get_layout().format(event,out.get_out());
		out.fill_end_null();
		::OutputDebugStringA(out.str());
	}

	shared_ptr<win32_debug_appender> win32_debug_appender::create( const std::string& name,layout* _layout/*=0*/ )
	{
		shared_ptr<win32_debug_appender> p( new win32_debug_appender( name, _layout ) );
		appender::add_appender( p );
		return p;
	}


	shared_ptr<appender> win32_debug_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		if(appender::get_appender(name))
			throw std::invalid_argument(std::string("Appender name \"")+name+"\" be existed.");

		std::auto_ptr<layout> l = layout_factory::get_instance().create(params.get_child("layout"));

		return win32_debug_appender::create(name,l.release());
	}

	std::string win32_debug_appender_creator::get_name() const
	{
		return "win32_debug_appender";
	}
}

#endif // WIN32

