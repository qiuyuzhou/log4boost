/* Copyright (C) 2007 - 2009  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_LOG4J_XML_LAYOUT_HPP___
#define LOGGING_LOG4J_XML_LAYOUT_HPP___

#include <log4boost/config.hpp>

#include "log4boost/layout/layout.hpp"

namespace log4boost
{

	/**
	 *	A layout serialize the logging event to a log4j XML format string, could be sent to chainsaw.
	 */
	class LOG4BOOST_EXPORT log4j_xml_layout :
		public layout
	{
	public:
		log4j_xml_layout();


		/**
		*	Format the logging event to the ostream.
		*	@param	event	The event would be log.
		*	@param	out		The ostream would be log into.
		*/
		virtual void format(const logging_event &event,std::ostream& out);;

	private:
		
	};




}


#endif//LOGGING_LOG4J_XML_LAYOUT_HPP___
