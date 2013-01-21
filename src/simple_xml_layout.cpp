/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/logging_event.hpp"
#include "log4boost/layout/simple_xml_layout.hpp"
#include "log4boost/ndc.hpp"

#include "log4boost/detail/thread_id.hpp"
#include "log4boost/detail/process_id.hpp"

#include "ptree_inc.h"
#include "transform.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

namespace log4boost
{
	namespace detail
	{
		class ndc2ostream_with_escape_helper
			:public boost::static_visitor<>
		{
		public:
			ndc2ostream_with_escape_helper( std::ostream& out )
				:out_(out){}

			template <typename T>
			void operator()( T & operand ) const
			{
				out_ << operand;
			}

			void operator()( const std::string& operand ) const
			{
				appendEscapingCDATA( out_, operand );
			}

		private:
			std::ostream&	out_;
		};
	}

	void simple_xml_layout::format(const logging_event &event,std::ostream& out)
	{
		out	<<"<logging_event>"
			<<"<tag>"<<event.get_logger_name()<<"</tag>"
			<<"<message>";
		detail::appendEscapingCDATA(out,event.get_message());
		out	<<"</message>"
			<<"<ndc>"
			<<event.get_ndc()
			<<"</ndc>"
			<<"<priority>"<<log4boost::priority::get_name(event.get_priority())<<"</priority>"
			<<"<thread>"<<event.get_thread_id()<<"</thread>"
			<<"<process>"<<event.get_process_id()<<"</process>"
			<<"<timestamp>"<<event.get_precision_time().to_ptime()<<"</timestamp>"
			<<"</logging_event>";
	}


	std::auto_ptr<layout> create_simple_xml_layout(const property_tree::ptree& params)
	{
		return std::auto_ptr<layout>(new simple_xml_layout());
	}
}
