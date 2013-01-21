/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef LOGGING_BINARY_LAYOUT_HPP__4386725_
#define LOGGING_BINARY_LAYOUT_HPP__4386725_

#include <log4boost/config.hpp>
#include <log4boost/layout/layout.hpp>

namespace log4boost
{

	class LOG4BOOST_EXPORT binary_layout
		: public layout
	{
	public:
		binary_layout(void){}
		virtual ~binary_layout(void){}

		virtual void format(const logging_event &event,std::ostream& out);

	};

}


#endif//LOGGING_BINARY_LAYOUT_HPP__4386725_
