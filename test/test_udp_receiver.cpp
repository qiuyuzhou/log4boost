
#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/asio.hpp>
#include <log4boost/log4boost.hpp>
#include <log4boost/logger.hpp>
#include <log4boost/receiver/udp_receiver.hpp>
#include <log4boost/appender/console_appender.hpp>
#include <log4boost/detail/singleton.hpp>
#include <boost/thread.hpp>

using namespace log4boost;


int test_main( int, char*[] )
{

	logger::get_root().add_appender( console_appender::create("console"));
	logger::get_root().set_priority( priority::PL_ALL );

	boost::shared_ptr<udp_receiver> r = udp_receiver::create( "udp" );
	//r->enable_relog("logging_test");
	r->open("",6637);


	boost::this_thread::sleep( boost::posix_time::seconds(15) );

	r->close();

	return 0;
}

