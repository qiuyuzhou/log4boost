#include "stdafx.h"
#include <cstdlib>

#include "log4boost/allocator.hpp"

#pragma warning( disable: 4073 ) ///< initializers put in library initialization area
#pragma init_seg( lib )
static log4boost::allocate_func g_alloc = &malloc;
static log4boost::free_func g_free = &free;

void log4boost::set_allocator( allocate_func al, free_func fr )
{
	g_alloc = al;
	g_free = fr;
}

#if defined(LOG4BOOST_BUILD_DLL)

void* operator new( size_t sz )
{
	return g_alloc(sz);
}

void operator delete( void* ptr )
{
	g_free(ptr);
}

void* operator new[]( size_t sz )
{
	return g_alloc(sz);
}

void operator delete[]( void* ptr )
{
	return g_free(ptr);
}

#endif
