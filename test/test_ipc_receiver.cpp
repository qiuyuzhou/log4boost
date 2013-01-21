
#include <boost/test/included/test_exec_monitor.hpp>

#include <log4boost/logger.hpp>
#include <log4boost/receiver/ipc_receiver.hpp>
#include <log4boost/appender/console_appender.hpp>

#include <boost/thread.hpp>

using namespace log4boost;


int test_main( int, char*[] )
{
	
	logger::get_root().add_appender( console_appender::create("console"));
	logger::get_root().set_priority( priority::PL_ALL );

	boost::shared_ptr<ipc_receiver> r = ipc_receiver::create("ipc");
	r->enable_relog("logging_test");
	r->open("logging_test");


	boost::this_thread::sleep( boost::posix_time::seconds(15) );

	return 0;
}

