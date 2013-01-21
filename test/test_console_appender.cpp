
#include <boost/test/included/test_exec_monitor.hpp>

#include "log4boost/log4boost.hpp"
#include "log4boost/appender/console_appender.hpp"
#include "log4boost/logger.hpp"

using namespace log4boost;

void test_console_appender_priority_color(void)
{
	boost::shared_ptr<console_appender> la = console_appender::create("priority_cout");
	logger::get("priority").add_appender(la).set_priority(priority::PL_ALL);

	LOG_DEBUG("priority","sdsfdfsfsdf");
	LOG_INFO("priority","sdsfdfsfsdf");
	LOG_WARN("priority","sdsfdfsfsdf");
	LOG_ERROR("priority","sdsfdfsfsdf");
	LOG_FATAL("priority","sdsfdfsfsdf");
}

void test_console_appender_fixed_color(void)
{
	boost::shared_ptr<console_appender> la = console_appender::create("fixed_cout");
	la->set_fixed_color( console_appender::BrightCyan, console_appender::DefaultColor );
	logger::get("fixed").add_appender(la).set_priority(priority::PL_ALL);

	LOG_DEBUG("fixed","sdsfdfsfsdf");
	LOG_INFO("fixed","sdsfdfsfsdf");
	LOG_WARN("fixed","sdsfdfsfsdf");
	LOG_ERROR("fixed","sdsfdfsfsdf");
	LOG_FATAL("fixed","sdsfdfsfsdf");

	la->set_fixed_color( console_appender::BrightRed, console_appender::DefaultColor );
	LOG_INFO("fixed","Bright Red with default background.");
	la->set_fixed_color( console_appender::BrightBlue, console_appender::DefaultColor );
	LOG_INFO("fixed","Bright Blue with default background.");
	la->set_fixed_color( console_appender::BrightGreen, console_appender::DefaultColor );
	LOG_INFO("fixed","Bright Green with default background.");
	la->set_fixed_color( console_appender::BrightYellow, console_appender::DefaultColor );
	LOG_INFO("fixed","Bright Yellow with default background.");
	la->set_fixed_color( console_appender::BrightMagenta, console_appender::DefaultColor );
	LOG_INFO("fixed","Bright Magenta with default background.");
	la->set_fixed_color( console_appender::BrightCyan, console_appender::DefaultColor );
	LOG_INFO("fixed","Bright Cyan with default background.");
	la->set_fixed_color( console_appender::White, console_appender::DefaultColor );
	LOG_INFO("fixed","White with default background.");

	la->set_fixed_color( console_appender::Red, console_appender::DefaultColor );
	LOG_INFO("fixed","Red with default background.");
	la->set_fixed_color( console_appender::Blue, console_appender::DefaultColor );
	LOG_INFO("fixed","Blue with default background.");
	la->set_fixed_color( console_appender::Green, console_appender::DefaultColor );
	LOG_INFO("fixed","Green with default background.");
	la->set_fixed_color( console_appender::Yellow, console_appender::DefaultColor );
	LOG_INFO("fixed","Yellow with default background.");
	la->set_fixed_color( console_appender::Magenta, console_appender::DefaultColor );
	LOG_INFO("fixed","Magenta with default background.");
	la->set_fixed_color( console_appender::Cyan, console_appender::DefaultColor );
	LOG_INFO("fixed","Cyan with default background.");
	la->set_fixed_color( console_appender::Grey, console_appender::DefaultColor );
	LOG_INFO("fixed","Grey with default background.");

	la->set_fixed_color( console_appender::BrightRed, console_appender::White );
	LOG_INFO("fixed","Bright Red with White background.");
	la->set_fixed_color( console_appender::BrightBlue, console_appender::White );
	LOG_INFO("fixed","Bright Blue with White background.");
	la->set_fixed_color( console_appender::BrightGreen, console_appender::White );
	LOG_INFO("fixed","Bright Green with White background.");
	la->set_fixed_color( console_appender::BrightYellow, console_appender::White );
	LOG_INFO("fixed","Bright Yellow with White background.");
	la->set_fixed_color( console_appender::BrightMagenta, console_appender::White );
	LOG_INFO("fixed","Bright Magenta with White background.");
	la->set_fixed_color( console_appender::BrightCyan, console_appender::White );
	LOG_INFO("fixed","Bright Cyan with White background.");

	la->set_fixed_color( console_appender::Red, console_appender::White );
	LOG_INFO("fixed","Red with White background.");
	la->set_fixed_color( console_appender::Blue, console_appender::White );
	LOG_INFO("fixed","Blue with White background.");
	la->set_fixed_color( console_appender::Green, console_appender::White );
	LOG_INFO("fixed","Green with White background.");
	la->set_fixed_color( console_appender::Yellow, console_appender::White );
	LOG_INFO("fixed","Yellow with White background.");
	la->set_fixed_color( console_appender::Magenta, console_appender::White );
	LOG_INFO("fixed","Magenta with White background.");
	la->set_fixed_color( console_appender::Cyan, console_appender::White );
	LOG_INFO("fixed","Cyan with White background.");
	la->set_fixed_color( console_appender::Grey, console_appender::White );
	LOG_INFO("fixed","Grey with White background.");

	la->set_fixed_color( console_appender::BrightRed, console_appender::Black );
	LOG_INFO("fixed","Bright Red with black background.");
	la->set_fixed_color( console_appender::BrightBlue, console_appender::Black );
	LOG_INFO("fixed","Bright Blue with black background.");
	la->set_fixed_color( console_appender::BrightGreen, console_appender::Black );
	LOG_INFO("fixed","Bright Green with black background.");
	la->set_fixed_color( console_appender::BrightYellow, console_appender::Black );
	LOG_INFO("fixed","Bright Yellow with black background.");
	la->set_fixed_color( console_appender::BrightMagenta, console_appender::Black );
	LOG_INFO("fixed","Bright Magenta with black background.");
	la->set_fixed_color( console_appender::BrightCyan, console_appender::Black );
	LOG_INFO("fixed","Bright Cyan with black background.");

	la->set_fixed_color( console_appender::Red, console_appender::Black );
	LOG_INFO("fixed","Red with black background.");
	la->set_fixed_color( console_appender::Blue, console_appender::Black );
	LOG_INFO("fixed","Blue with black background.");
	la->set_fixed_color( console_appender::Green, console_appender::Black );
	LOG_INFO("fixed","Green with black background.");
	la->set_fixed_color( console_appender::Yellow, console_appender::Black );
	LOG_INFO("fixed","Yellow with black background.");
	la->set_fixed_color( console_appender::Magenta, console_appender::Black );
	LOG_INFO("fixed","Magenta with black background.");
	la->set_fixed_color( console_appender::Cyan, console_appender::Black );
	LOG_INFO("fixed","Cyan with black background.");
	la->set_fixed_color( console_appender::Grey, console_appender::Black );
	LOG_INFO("fixed","Grey with black background.");

}

void test_utf8_console(void)
{
	boost::shared_ptr<console_appender> la = console_appender::create("root_cout");
	logger::get_root().add_appender(la).set_priority(priority::PL_ALL);
	la->set_utf8(true);

	const wchar_t* hello_word_string = L"\x4e16\x754c\xff0c\x4f60\x597d\xff01";

	WLOG_INFO("",hello_word_string);
}

int test_main( int, char*[] )
{
	test_console_appender_priority_color();
	test_console_appender_fixed_color();
	test_utf8_console();

	return 0;
}
