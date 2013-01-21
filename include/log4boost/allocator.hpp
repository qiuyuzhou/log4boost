#pragma once

#include <log4boost/config.hpp>

namespace log4boost
{
	typedef void* (*allocate_func)(size_t);
	typedef void (*free_func)(void*);

	extern void LOG4BOOST_EXPORT set_allocator( allocate_func al, free_func fr );

	struct StringAllocator
	{

	};
}
