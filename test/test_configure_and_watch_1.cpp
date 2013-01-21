
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/configurator.hpp"
#include "log4boost/config_watch_dog.hpp"

void test_configure_and_watch()
{
	using namespace log4boost;

	properties_configure_and_watch("logging_conf_with_preconfig.properties","LOG_NAME=log.txt;DDD=fff;eee=fddd",3000);

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

	boost::this_thread::sleep( boost::posix_time::seconds(30) );
	BOOST_CHECK( config_watch_dog::instance()->is_watching() );
	config_watch_dog::instance()->stop_watching();
	BOOST_CHECK( config_watch_dog::instance()->is_watching() == false );
}


int test_main( int, char*[] )
{
	test_configure_and_watch();
	return 0;
}
