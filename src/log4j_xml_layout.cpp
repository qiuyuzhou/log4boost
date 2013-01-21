/* Copyright (C) 2007 - 2009  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"

#include "log4boost/layout/log4j_xml_layout.hpp"
#include "log4boost/logging_event.hpp"
#include "transform.h"

#include "boost/date_time/local_time/local_time.hpp"

namespace log4boost
{

	

	log4j_xml_layout::log4j_xml_layout()
	{

	}

	void log4j_xml_layout::format( const logging_event &event,std::ostream& out )
	{
		static boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970,1,1));
		out << "<log4j:event logger=\"";
		detail::appendEscapingTags( out, event.get_logger_name() );
		/** The number of milliseconds elapsed from 1/1/1970 until logging event was created. */
		boost::posix_time::time_duration td = (event.get_precision_time().to_ptime() - time_t_epoch);
		out << "\" timestamp=\"" << td.total_milliseconds();
		out << "\" level=\"";
		detail::appendEscapingTags( out, priority::get_name( event.get_priority() ) );
		out << "\" thread=\"";
		detail::appendEscapingTags( out, event.get_thread_id() );
		out << "\">\n";

		out << "<log4j:message><![CDATA[";
		detail::appendEscapingCDATA( out, event.get_message() );
		out << "]]></log4j:message>\n";

		if ( !event.get_ndc().empty() )
		{
			out << "<log4j:NDC><![CDATA[";
			detail::appendEscapingCDATA( out, event.get_ndc() );
			out << "]]></log4j:NDC>\n";
		}

		out << "<log4j:properties>\n";
		
		out << "<log4j:data name=\"application\" value=\"";
		detail::appendEscapingTags( out, event.get_instance_name() );
		out << "\"/>\n";

		out << "</log4j:properties>\n";

		out << "</log4j:event>" << "\n\n";
	}


	std::auto_ptr<layout> create_log4j_xml_layout( const property_tree::ptree& params )
	{
		return std::auto_ptr<layout>( new log4j_xml_layout() );
	}
}

