/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_TRANSFORM_HPP_INCLUDED
#define LOGGING_TRANSFORM_HPP_INCLUDED

#include <string>
#include <sstream>

namespace log4boost
{
	namespace detail
	{

		/**
		* Ensures that embeded CDEnd strings (]]>) are handled properly
		* within message, NDC and throwable tag text.
		*
		* @param buf std::ostream holding the XML data to this point.  The
		* initial CDStart (<![CDATA[) and final CDEnd (]]>) of the CDATA
		* section are the responsibility of the calling method.
		* @param str The std::string that is inserted into an existing CDATA Section within buf.  
		* */
		void appendEscapingCDATA(std::ostream& buf,const std::string& str);


		void appendEscapingTags( std::ostream& buf,const std::string& input );

	}
}


#endif//LOGGING_TRANSFORM_HPP_INCLUDED
