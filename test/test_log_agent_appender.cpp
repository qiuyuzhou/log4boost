
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/appender/log_agent_appender.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/configurator.hpp"
#include "log4boost/layout/log4j_xml_layout.hpp"

#include <boost/thread.hpp>


void log_agent_appender_with_file_writer_test()
{
	using namespace log4boost;

	boost::shared_ptr<log_agent_appender>	apd = log_agent_appender::create("laWithFile");
	log_agent_appender::agent_settings	agentSettings(log_agent_appender::wt_file);
	log_agent_appender::file_writer_settings	logFileSettings("log_agent_appender_file.log");

	apd->open(agentSettings,logFileSettings);
	logger& l = logger::get_root();
	l.add_appender(apd);

	l.set_priority(priority::PL_DEBUG);
	L_DEBUG(l,"first 1");
	L_DEBUG(l,"first 2");
	L_DEBUG(l,"first 3");

	boost::this_thread::sleep( boost::posix_time::seconds(5) );

	L_DEBUG(l,"second 1");
	L_DEBUG(l,"second 2");
	L_DEBUG(l,"second 3");
}

void log_agent_appender_with_rolling_file_writer_test()
{
	using namespace log4boost;

	boost::shared_ptr<log_agent_appender>	apd = log_agent_appender::create("laWithRollingFile");
	log_agent_appender::agent_settings	agentSettings(log_agent_appender::wt_rolling_file);
	rolling_file_settings rollingFileSettings("log_agent_appender_rolling_file");
	rollingFileSettings.max_size_bytes = 10;

	apd->open(agentSettings,rollingFileSettings);
	logger& l = logger::get_root();
	l.add_appender(apd);

	l.set_priority(priority::PL_DEBUG);
	L_DEBUG(l,"first 1");
	L_DEBUG(l,"first 2");
	L_DEBUG(l,"first 3");

	boost::this_thread::sleep( boost::posix_time::seconds(5) );

	L_DEBUG(l,"second 1");
	L_DEBUG(l,"second 2");
	L_DEBUG(l,"second 3");
}

void log_agent_appender_with_tcp_writer_test()
{
	using namespace log4boost;

	boost::shared_ptr<log_agent_appender>	apd = log_agent_appender::create("laWithRollingFile",new log4j_xml_layout());
	log_agent_appender::agent_settings	agentSettings(log_agent_appender::wt_tcp);
	log_agent_appender::tcp_writer_settings	settings("127.0.0.1",4448);

	apd->open(agentSettings,settings);
	logger& l = logger::get_root();
	l.add_appender(apd);

	l.set_priority(priority::PL_DEBUG);
	L_DEBUG(l,"first 1");
	L_DEBUG(l,"first 2");
	L_DEBUG(l,"first 3");

	L_DEBUG(l,"second 1");
	L_DEBUG(l,"second 2");
	L_DEBUG(l,"second 3");
}

void test_log_agent_appender_with_config_file()
{
	using namespace log4boost;

	properties_configure("logging_conf_for_log_agent_appender.properties");

	logger& l = logger::get_root();
	L_DEBUG(l,"first 1");
	L_DEBUG(l,"first 2");
	L_DEBUG(l,"first 3");

	boost::this_thread::sleep( boost::posix_time::seconds(5) );

	L_DEBUG(l,"second 1");
	L_DEBUG(l,"second 2");
	L_DEBUG(l,"second 3");
}

int test_main( int, char*[] )
{
	//log_agent_appender_with_file_writer_test();
	//log_agent_appender_with_rolling_file_writer_test();
	//log_agent_appender_with_tcp_writer_test();
	test_log_agent_appender_with_config_file();
    return 0;
}
