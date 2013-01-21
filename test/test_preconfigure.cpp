
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/configurator.hpp"


void test_configure_with_preconfig(void)
{
	using namespace log4boost;

	properties_configure("logging_conf_with_preconfig.properties","LOG_NAME=log.txt;DDD=fff;eee=fddd");

	logger& l = logger::get_root();
	L_DEBUG(l,"debug 1");
	L_DEBUG(l,"debug 2");
	L_INFO(l,"info 3");
	L_WARN(l,"warn 4");
	L_ERROR(l,"error 5");
	logger& l1 = logger::get("debug.n1");
	L_DEBUG(l1,"debug 1");
	L_DEBUG(l1,"debug 2");
	L_INFO(l1,"info 3");
	L_WARN(l1,"warn 4");
	L_ERROR(l1,"error 5");
};


int test_main( int, char*[] )
{
	test_configure_with_preconfig();
	return 0;
}
