
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/appender/win32_debug_appender.hpp"
#include "log4boost/logger.hpp"

void test_win32_debug_appender(void)
{
	using namespace log4boost;

	logger::get_root().add_appender(win32_debug_appender::create("fileAppenderAppend"));

	logger& l = logger::get_root();
	l.set_priority(priority::PL_DEBUG);
	L_DEBUG(l,"first 1");
	L_DEBUG(l,"first 2");
	L_DEBUG(l,"first 3");
}


int test_main( int, char*[] )
{
	test_win32_debug_appender();	
	return 0;
}
