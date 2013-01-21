
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/appender/ipc_appender.hpp"
#include "log4boost/layout/binary_layout.hpp"
#include "log4boost/logger.hpp"

using namespace log4boost;

int test_main( int, char*[] )
{

	logger::get_root().add_appender(
		ipc_appender::create("ipc_appender","logging_test",512,1024,new binary_layout()));

	logger& l = logger::get_root();
	l.set_priority(priority::PL_DEBUG);
	L_DEBUG(l,"first 1");
	L_DEBUG(l,"first 2");
	L_DEBUG(l,"first 3");
	logger& l1 = logger::get("debug.n1");
	L_DEBUG(l1,"first 1");
	L_DEBUG(l1,"first 2");
	L_DEBUG(l1,"first 3");
	logger& l2 = logger::get("debug.n2");
	L_DEBUG(l2,"first 1");
	L_DEBUG(l2,"first 2");
	L_DEBUG(l2,"first 3");



	return 0;
}

