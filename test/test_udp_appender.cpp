
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/appender/udp_appender.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/layout/simple_xml_layout.hpp"
#include "log4boost/layout/binary_layout.hpp"

void test_udp_appender(void)
{
	using namespace log4boost;

	logger::get_root().add_appender(
		udp_appender::create("udp_appender",new simple_xml_layout(),"127.0.0.1",6636));

	logger::get_root().add_appender(
//		udp_appender::create("udp_appender_binary",new binary_layout(),"127.0.0.1",6637));
		udp_appender::create("udp_appender",new binary_layout(),"127.0.0.1",6637));

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

}


int test_main( int, char*[] )
{
	test_udp_appender();
	return 0;
}
