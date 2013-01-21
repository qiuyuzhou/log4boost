/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_PTREE_INC___H___
#define LOGGING_PTREE_INC___H___

#ifdef LOGGING_USE_BOOST_PROPERTY_TREE

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

namespace log4boost
{
	using namespace boost::property_tree;
}

#else

#include "ptree.h"


#endif


#endif//LOGGING_PTREE_INC___H___
