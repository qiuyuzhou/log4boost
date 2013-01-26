
//#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/thread.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/appender/tcp_appender.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/layout/log4j_xml_layout.hpp"
#include "log4boost/layout/binary_layout.hpp"

#include "log4boost/appender/console_appender.hpp"


void test_tcp_appender(void)
{
	using namespace log4boost;

	log4boost::shared_ptr<tcp_appender> apd = tcp_appender::create("tcp_appender",new log4j_xml_layout(),"127.0.0.1",4448);
	apd->open();
	logger::get_root().add_appender(apd);


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
	L_DEBUG(l,"first 4");
}

void test_tcp_appender_with_bianry_layout(void)
{
	using namespace log4boost;

	log4boost::shared_ptr<tcp_appender> apd = tcp_appender::create("tcp_appender",new binary_layout(),"127.0.0.1",4448);
	apd->open();
	logger::get_root().add_appender(apd);


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
	L_DEBUG(l,"first 4");
}



int main( int, char*[] )
{
	log4boost::logger::get("loglog").add_appender( log4boost::console_appender::create("loglog_console") );
	log4boost::logger::get("loglog").set_priority( log4boost::priority::PL_ALL );

	//test_tcp_appender();
	test_tcp_appender_with_bianry_layout();
#if defined(WIN32)||defined(WIN64)
	Sleep(1000);
#endif
	log4boost::shutdown();
	return 0;
}
