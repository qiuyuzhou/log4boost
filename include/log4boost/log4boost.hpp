/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_LOGGING_H__
#define LOGGING_LOGGING_H__

/*! \mainpage Log4boost Lib Develop Documentation

This is a log framework patterned after log4j.

There are several ports/implementations of the Log4j API. Here's an incomplete list: 
- \b log4cxx http://log4boostapache.org/log4cxx/index.html The Apache version port for log4j.
But it depend on Apache port runtime. It's hard to use ANT to compile the Apache port runtime.
- \b log4cpp http://log4cpp.sourceforge.net/	The latest release is at 2002. There is nobody to maintain the lib now.
- \b log4cplus http://log4cplus.sourceforge.net/ The latest release is at 2004-02-10. There is nobody to maintain the lib now.


This implement depend on the boost lib. All its Multi-platform support is to achieve through boost. 
It's using std naming style.

This lib is designed to used in the server program which need running long time.
There is no new operation and no std::string using in the log calling.

\par Loggers, Appenders and Layouts

Like other log4j liked lib, this lib has three main components: loggers, appenders and layouts. 
These three types of components work together to enable developers to log messages according to message type and level,
and to control at runtime how these messages are formatted and where they are reported.

- \subpage logger_hierarchy
- \subpage log_priority
- \subpage appenders_and_layouts


This manual is divided in the following sections:
- \subpage log_macro 
- \subpage log_configure
- \subpage log_ndc


*/

/*! \page logger_hierarchy Logger hierarchy

The first and foremost advantage of any logging API over plain std::cout resides in its ability to disable certain log statements 
while allowing others to print unhindered. This capability assumes that the logging space, that is, 
the space of all possible logging statements, is categorized according to some developer-chosen criteria.

Loggers are named entities. Logger names are case-sensitive and they follow the hierarchical naming rule:

\par Named Hierarchy

	A logger is said to be an ancestor of another logger if its name followed by a dot is a prefix of the descendant logger name. 
	A logger is said to be a parent of a child logger if there are no ancestors between itself and the descendant logger. 

For example, the logger named "com.foo" is a parent of the logger named "com.foo.Bar". Similarly, 
"java" is a parent of "java.util" and an ancestor of "java.util.Vector". This naming scheme should be familiar to most developers.

The root logger resides at the top of the logger hierarchy. It is exceptional in two ways:

-# it always exists,
-# it can be retrieved by empty string("") name.

All loggers are instantiated and retrieved with the class static log4boost::logger::get_logger method. 
Invoking the class static log4boost::logger::get_logger method also can retrieve the root logger.


*/


/*! \page log_priority Log priority

Loggers may be assigned levels. The pre-defined levels: TRACE, DEBUG, INFO, WARN, ERROR and FATAL are defined in the log4boost::priority::priority_level enum.
It have a typedef log4boost::priority_level.

If a given logger is not assigned a level, then it inherits one from its closest ancestor with an assigned level.
To ensure that all loggers can eventually inherit a level, the root logger always has an assigned level.

It assumes that levels are ordered. For the standard levels, we have TRACE < DEBUG < INFO < WARN < ERROR < FATAL.

*/



/*! \page appenders_and_layouts Appenders and Layouts
TODO

*/


/*! \page log_macro Introduction the log macro
There are two kind macro used to log the message, which prefixed with LOG_ or L_.

The kind prefixed with LOG_ , has two parameters, the first is the name of logger which would be log to, the second is the message would be log.
The kind prefixed with L_ is more efficient, also has two parameters, the first is the concrete logger object, the second is same as LOG_.

The log macro list:
- LOG_DEBUG
- LOG_INFO
- LOG_WARN
- LOG_ERROR
- LOG_FATAL
- L_TRACE
- L_DEBUG
- L_INFO
- L_WARN
- L_ERROR
- L_FATAL

The using example:
\code
using namespace log4boost;

LOG_DEBUG("debug.macro","debug macro");
LOG_WARN("debug.macro","warn macro");

logger& l = logger::get_root();
L_DEBUG(l,"first 1");
L_DEBUG(l,"first 2");
logger& l1 = logger::get_logger("debug.n1");
L_DEBUG(l1,"first 1");
L_DEBUG(l1,"first 2");
L_INFO(l1,"first 3");

\endcode

In order to enable L_TRACE, should defind the LOGGING_ENABLE_LOG_TRACE preprocesser.

The L_DEBUG and LOG_DEBUG are disabled by default when compile using release config.

*/



#include <log4boost/config.hpp>
#include <log4boost/logger.hpp>
#include <log4boost/detail/stack_ostream.hpp>

#ifdef LOGGING_ENABLE_LOG_TRACE
#	include <log4boost/scoped_tracer.hpp>
#endif


#define ARRAY_OSTREAM(x) log4boost::detail::stack_ostream out;out<<x;out.fill_end_null();
#define W_ARRAY_OSTREAM(x) log4boost::detail::wstack_ostream out;out<<x;out.fill_end_null();


#if LOG4BOOST_DISABLE_ALL_LOG

#	define GET_LOGGER(l,x) do{}while(false)

#	define LOGGER(x) do{}while(false)


#	define LOG_DEBUG(l,x)	do{}while(false)
#	define LOG_INFO(l,x)	do{}while(false)
#	define LOG_WARN(l,x)	do{}while(false)
#	define LOG_ERROR(l,x)	do{}while(false)
#	define LOG_FATAL(l,x)	do{}while(false)

#	define L_TRACE(l,x)		do{}while(false)
#	define L_DEBUG(l,x)		do{}while(false)
#	define L_INFO(l,x)		do{}while(false)
#	define L_WARN(l,x)		do{}while(false)
#	define L_ERROR(l,x)		do{}while(false)
#	define L_FATAL(l,x)		do{}while(false)

#	define WLOG_DEBUG(l,x)	do{}while(false)
#	define WLOG_INFO(l,x)	do{}while(false)
#	define WLOG_WARN(l,x)	do{}while(false)
#	define WLOG_ERROR(l,x)	do{}while(false)
#	define WLOG_FATAL(l,x)	do{}while(false)

#	define L_TRACE(l,x)		do{}while(false)
#	define WL_DEBUG(l,x)	do{}while(false)
#	define WL_INFO(l,x)		do{}while(false)
#	define WL_WARN(l,x)		do{}while(false)
#	define WL_ERROR(l,x)	do{}while(false)
#	define WL_FATAL(l,x)	do{}while(false)

#else

#	define GET_LOGGER(l,x) log4boost::logger& l=log4boost::logger::get(x);do{}while(false)

#	define LOGGER(x) log4boost::logger::get_logger(x)

#	if !defined(LOGGING_DISABLE_LOG_FILE_AND_LINE)
#		define LOG_BASE(p,l,x)	do { GET_LOGGER(_l,l); if(!_l.is_priority_enabled(p));else{ ARRAY_OSTREAM(x);_l.log( p ,out.str(),out.length(),__FILE__,__LINE__); } } while(false)
#		define WLOG_BASE(p,l,x)	do { GET_LOGGER(_l,l); if(!_l.is_priority_enabled(p));else{ W_ARRAY_OSTREAM(x);_l.log( p ,out.str(),out.length(),__FILE__,__LINE__); } } while(false)
#	else
#		define LOG_BASE(p,l,x)	do { GET_LOGGER(_l,l); if(!_l.is_priority_enabled(p));else{ ARRAY_OSTREAM(x);_l.log( p ,out.str(),out.length()); } } while(false)
#		define WLOG_BASE(p,l,x)	do { GET_LOGGER(_l,l); if(!_l.is_priority_enabled(p));else{ W_ARRAY_OSTREAM(x);_l.log( p ,out.str(),out.length()); } } while(false)
#	endif

#	if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#		define LOG_DEBUG(l,x)	LOG_BASE(log4boost::priority::PL_DEBUG,l,x)
#		define WLOG_DEBUG(l,x)	WLOG_BASE(log4boost::priority::PL_DEBUG,l,x)
#	else
#		define LOG_DEBUG(l,x)	do{}while(false)
#		define W_LOG_DEBUG(l,x)	do{}while(false)
#	endif//_DEBUG

#	define LOG_INFO(l,x)	LOG_BASE(log4boost::priority::PL_INFO,l,x)
#	define LOG_WARN(l,x)	LOG_BASE(log4boost::priority::PL_WARN,l,x)
#	define LOG_ERROR(l,x)	LOG_BASE(log4boost::priority::PL_ERROR,l,x)
#	define LOG_FATAL(l,x)	LOG_BASE(log4boost::priority::PL_FATAL,l,x)

#	define WLOG_INFO(l,x)	WLOG_BASE(log4boost::priority::PL_INFO,l,x)
#	define WLOG_WARN(l,x)	WLOG_BASE(log4boost::priority::PL_WARN,l,x)
#	define WLOG_ERROR(l,x)	WLOG_BASE(log4boost::priority::PL_ERROR,l,x)
#	define WLOG_FATAL(l,x)	WLOG_BASE(log4boost::priority::PL_FATAL,l,x)



#	if !defined(LOGGING_DISABLE_LOG_FILE_AND_LINE)
#		define L_BASE(p,l,x)	do { if(!l.is_priority_enabled(p));else{ ARRAY_OSTREAM(x);l.log(p,out.str(),out.length(),__FILE__,__LINE__); } }while(false)
#		define WL_BASE(p,l,x)	do { if(!l.is_priority_enabled(p));else{ W_ARRAY_OSTREAM(x);l.log(p,out.str(),out.length(),__FILE__,__LINE__); } }while(false)
#	else
#		define L_BASE(p,l,x)	do { if(!l.is_priority_enabled(p));else{ ARRAY_OSTREAM(x);l.log(p,out.str(),out.length()); } }while(false)
#		define WL_BASE(p,l,x)	do { if(!l.is_priority_enabled(p));else{ W_ARRAY_OSTREAM(x);l.log(p,out.str(),out.length()); } }while(false)
#	endif

#	if defined(LOGGING_ENABLE_LOG_TRACE) && (defined(_DEBUG) || !defined(NDEBUG))
#		define L_TRACE(l,x)	log4boost::scoped_tracer	logging_scoped_tracer(l) << x ;logging_scoped_tracer.log();do{}while(false)
#		define WL_TRACE(l,x)	log4boost::wscoped_tracer	logging_scoped_tracer(l) << x ;logging_scoped_tracer.log();do{}while(false)
#	else
#		define L_TRACE(l,x)	do{}while(false)
#		define WL_TRACE(l,x)	do{}while(false)
#	endif

#	if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#		define L_DEBUG(l,x)	L_BASE(log4boost::priority::PL_DEBUG,l,x)
#		define WL_DEBUG(l,x)	WL_BASE(log4boost::priority::PL_DEBUG,l,x)
#	else
#		define L_DEBUG(l,x)	do{}while(false)
#		define WL_DEBUG(l,x)	do{}while(false)
#	endif//_DEBUG

#	define L_INFO(l,x)	L_BASE(log4boost::priority::PL_INFO,l,x)
#	define L_WARN(l,x)	L_BASE(log4boost::priority::PL_WARN,l,x)
#	define L_ERROR(l,x)	L_BASE(log4boost::priority::PL_ERROR,l,x)
#	define L_FATAL(l,x)	L_BASE(log4boost::priority::PL_FATAL,l,x)

#	define WL_INFO(l,x)	WL_BASE(log4boost::priority::PL_INFO,l,x)
#	define WL_WARN(l,x)	WL_BASE(log4boost::priority::PL_WARN,l,x)
#	define WL_ERROR(l,x)	WL_BASE(log4boost::priority::PL_ERROR,l,x)
#	define WL_FATAL(l,x)	WL_BASE(log4boost::priority::PL_FATAL,l,x)

#endif//LOG4BOOST_DISABLE_ALL_LOG


namespace log4boost
{

	LOG4BOOST_EXPORT void shutdown();

}

#endif//LOGGING_LOGGING_H__
