
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/appender/file_appender.hpp"
#include "log4boost/logger.hpp"

void test_file_appender(void)
{
	using namespace log4boost;

	boost::shared_ptr<file_appender> fa1 = file_appender::create("fileAppenderAppend");
	fa1->open("./log_test_file_appender_append.txt",true);

	boost::shared_ptr<file_appender> fa2 = file_appender::create("fileAppenderNotAppend");
	fa2->open("./log_test_file_appender_not_append.txt",false);

	logger::get_root().add_appender(fa1);
	logger::get_root().add_appender(fa2);

	logger& l = logger::get_root();
	l.set_priority(priority::PL_DEBUG);
	L_DEBUG(l,"first 1");
	L_DEBUG(l,"first 2");
	L_DEBUG(l,"first 3");
}


int test_main( int, char*[] )
{
	test_file_appender();	
	return 0;
}
