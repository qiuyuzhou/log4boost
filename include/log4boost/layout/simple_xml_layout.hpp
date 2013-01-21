/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_SIMPLE_XML_LAYOUT_H___
#define LOGGING_SIMPLE_XML_LAYOUT_H___

#include <log4boost/config.hpp>
#include <log4boost/layout/layout.hpp>

namespace log4boost
{

	/**
	 *	A layout simple serialize the logging event to a XML format stirng.
	 */
	class LOG4BOOST_EXPORT simple_xml_layout
		:public layout
	{
	public:
		virtual void format(const logging_event &event,std::ostream& out);
	};

}

#endif//LOGGING_SIMPLE_XML_LAYOUT_H___
