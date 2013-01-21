/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"

#include "transform.h"

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
		void appendEscapingCDATA(std::ostream& buf,const std::string& str)
		{
			using namespace std;

			static string	CDATA_START			= "<![CDATA[";
			static string	CDATA_END			= "]]>";
			static string	CDATA_PSEUDO_END	= "]]&gt;";
			static string	CDATA_EMBEDED_END	= CDATA_END + CDATA_PSEUDO_END + CDATA_START;
			static int		CDATA_END_LEN		= CDATA_END.size();

			if(!str.empty())
			{
				string::size_type end =	str.find(CDATA_END);
				if(end==string::npos)
				{
					buf<<str;
				}
				else
				{
					string::size_type start = 0;
					while(end!=string::npos)
					{
						buf.write( str.c_str() + start, end - start );
						buf	<< CDATA_EMBEDED_END;
						start = end + CDATA_END_LEN;
						if(start < str.size())
						{
							end = str.find(CDATA_END,start);
						}
						else
						{
							return;
						}
					}
				}
			}
		}

		void appendEscapingTags( std::ostream& buf,const std::string& input )
		{
			if(input.length() == 0 )
			{
				return;
			}

			char specials[] = { 0x22 /* " */, 0x26 /* & */, 0x3C /* < */, 0x3E /* > */, 0x00 };

			size_t	start = 0;
			size_t	special = input.find_first_of( specials, start );
			while( special != std::string::npos )
			{
				if ( special > start )
				{
					buf.write( input.c_str() + start, special - start );
				}
				switch( input[ special ] )
				{
				case 0x22:
					buf << "&quot;";
					break;
				case 0x26:
					buf << "&amp;";
					break;
				case 0x3C:
					buf << "&lt;";
					break;
				case 0x3E:
					buf << "&gt;";
					break;

				default:
					buf.put( input[ special ] );
					break;
				}

				start = special + 1;
				if ( special < input.size() )
				{
					special = input.find_first_of( specials, start );
				}
				else
				{
					special = std::string::npos;
				}
			}
			if ( start < input.size() )
			{
				buf.write( input.c_str() + start, input.size() - start );
			}
		}


	}
}
