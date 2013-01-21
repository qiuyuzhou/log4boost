

#include <memory>
#include <stdio.h>

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/progress.hpp>

#include "log4boost/appender/ostream_appender.hpp"
#include "log4boost/appender/udp_appender.hpp"
#include "log4boost/appender/console_appender.hpp"
#include "log4boost/layout/simple_xml_layout.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/layout/layout.hpp"
#include "log4boost/layout/formatter_layout.hpp"
#include "log4boost/log4boost.hpp"

#include "log4boost/detail/stack_ostream.hpp"
#include <sstream>

using namespace log4boost;

void logging_test()
{
	logger& root = logger::get_root();

	//layout_appender* la = new ostream_appender("clog",std::clog);
	//la->set_layout(new formatter_layout());	
	//la->get_layout<formatter_layout>()->set_format("%timestamp% [ %logger_name%pad:20% ] - %message%\n");
	//root.add_appender(la);
	//udp_remote_appender* ura = new udp_remote_appender("udp",new simple_xml_layout(),"127.0.0.1",6636);
	//root.add_appender(ura);

	root.log(priority::PL_ERROR,"test 1 error!");

	logger::get("debug.test1.test2").log(priority::PL_ERROR,"debug mesage!");
	logger::get("debug.test1").set_priority(priority::PL_DEBUG);
	for(int i=0;i<10;i++)
	{
		logger::get("debug.test1.test2").log(priority::PL_DEBUG,"debug mesage!");
	}
}

void logging_macro_test()
{
	LOG_DEBUG("debug.macro","debug macro");
	LOG_WARN("debug.macro","warn macro");
}

void logging_formatter_layout()
{
	boost::shared_ptr<layout_appender> la = console_appender::create("cout");
	logger::get_root().add_appender(la);
	logger::get_root().set_priority(priority::PL_ALL);

	LOG_DEBUG("n1","sdsfdfsfsdf");
	LOG_INFO("n2.n3","sdsfdfsfsdf");
	LOG_WARN("","sdsfdfsfsdf");
	LOG_ERROR("","sdsfdfsfsdf");
	LOG_FATAL("","sdsfdfsfsdf");
}

int test_main( int, char*[] )
{
	//logging_test();
	//logging_macro_test();
	logging_formatter_layout();
	return 0;
}

