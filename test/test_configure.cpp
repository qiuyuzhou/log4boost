
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/thread.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/configurator.hpp"

void test_configure(void)
{
	using namespace log4boost;

	properties_configure("logging_conf.properties");


	//logger::get_root().add_appender(
	//	new udp_remote_appender("udp_remote_appender",new simple_xml_layout(),"127.0.0.1",6636));

	logger& l = logger::get_root();
	L_DEBUG(l,"debug 1");
	L_DEBUG(l,"debug 2");
	L_INFO(l,"info 3");
	L_WARN(l,"warn 4");
	L_ERROR(l,"error 5");

	Sleep(5000);

	logger& l1 = logger::get("debug.n1");
	L_DEBUG(l1,"debug 1");
	L_DEBUG(l1,"debug 2");
	L_INFO(l1,"info 3");
	L_WARN(l1,"warn 4");
	L_ERROR(l1,"error 5");

}

void test_reconfigure(void)
{
	using namespace log4boost;

	properties_configure("logging_conf.properties");
}


int test_main( int, char*[] )
{
	test_configure();
	//test_reconfigure();
	return 0;
}
