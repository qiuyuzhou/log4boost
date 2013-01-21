/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_CONFIG_H___
#define LOGGING_CONFIG_H___

#ifdef _WINDLL
#	ifndef LOG4BOOST_SHARED
#		define LOG4BOOST_SHARED
#	endif//LOG4BOOST_SHARED
#endif//_WINDLL

#ifdef log4boost_EXPORTS
#	ifndef LOG4BOOST_BUILD_DLL
#		define LOG4BOOST_BUILD_DLL
#	endif//LOG4BOOST_BUILD_DLL
#	ifndef LOG4BOOST_SHARED
#		define LOG4BOOST_SHARED
#	endif//LOG4BOOST_SHARED
#endif//logging_EXPORTS


#ifdef LOG4BOOST_SHARED
#	if defined(WIN32) || defined(WIN64)
#		ifdef LOG4BOOST_BUILD_DLL
#			define LOG4BOOST_EXPORT __declspec(dllexport)
#		else
#			define LOG4BOOST_EXPORT __declspec(dllimport)
#		endif
#	else
#		define LOG4BOOST_EXPORT
#	endif//WIN32
#else
#	define LOG4BOOST_EXPORT 
#endif

#ifndef LOG4BOOST_DISABLE_ALL_LOG
#	define LOG4BOOST_DISABLE_ALL_LOG 0
#endif//LOGGING_DISABLE_ALL


#if defined(LOG4BOOST_SHARED) && (defined(WIN32) || defined(WIN64))

#pragma warning(disable:4251) // C4251: 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#pragma warning(disable:4275) // C4275: non DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'

#endif

namespace log4boost
{
	namespace property_tree
	{
		class ptree;
	}

	//typedef unsigned short uint16_t;
	//typedef signed long long int64_t;
}

#ifdef LOGGING_MAX_MESSAGE_SIZE
#	undef LOGGING_MAX_MESSAGE_SIZE
#endif//LOGGING_MAX_MESSAGE_SIZE

#define LOGGING_MAX_MESSAGE_SIZE 4096

#ifndef LOGGING_USE_STACK_OSTREAM
#	define LOGGING_USE_STACK_OSTREAM 1
#endif

#endif//LOGGING_CONFIG_H___
