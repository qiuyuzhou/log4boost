/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/appender/console_appender.hpp"

#include <sstream>

#include "ptree_inc.h"
#include "log4boost/layout_factory.hpp"
#include "log4boost/configurator.hpp"

#include "log4boost/log4boost.hpp"

namespace log4boost
{

	console_appender::console_appender( const std::string& name, bool log2std_err, layout* _layout  )
		: layout_appender( name, _layout )
		, stream_(log2std_err?(&std::cerr):(&std::cout))
		, fixed_color_(false)
		, utf8_(false)
	{
#if defined(WIN32) || defined(WIN64)
		if (!log2std_err)
		{
			out_handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
		}
		else
		{
			out_handle_ = GetStdHandle(STD_ERROR_HANDLE);
		}

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(out_handle_,&csbi);
		defualt_attributes_ = csbi.wAttributes;
#else
		(*stream_) << std::left;
#endif
	}

	console_appender::~console_appender()
	{
	}

	void console_appender::_append( const logging_event& event )
	{
		mutex::scoped_lock	lock(mutex_);

		if ( fixed_color_ )
		{
			append_fixed_color( event );
		}
		else
		{
			append_color_by_priority( event );
		}
	}

	void console_appender::append_color_by_priority( const logging_event& event )
	{
		//Prepare the color
#if defined(WIN32) || defined(WIN64)
		static WORD 	attributes[] = {
			0
			,defualt_attributes_//TRACE
			,FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY//DEBUG
			,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY//INFO
			,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY//WARN
			,FOREGROUND_RED | FOREGROUND_INTENSITY//ERROR
			,FOREGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_BLUE //FATAL
		};
#else
		static const char*	color_codes[] = {
			""
			,""//TRACE
			,"\033[1;37m"//DEBUG
			,"\033[1;36m"//INFO
			,"\033[1;33m"//WARN
			,"\033[1;31m"//ERROR
			,"\033[1;35m"//FATAL
		};
#endif
#if defined(WIN32) || defined(WIN64)
		SetConsoleTextAttribute(out_handle_, attributes[event.get_priority()/10000]);
		if ( utf8_ )
		{
			wchar_t wbuf[LOGGING_MAX_MESSAGE_SIZE];
			char	buf[LOGGING_MAX_MESSAGE_SIZE*4];
			int len = MultiByteToWideChar( CP_UTF8, 0, event.get_message().c_str(), event.get_message().length(),wbuf,sizeof(wbuf) );
			len = WideCharToMultiByte( GetOEMCP(),0,wbuf,len,buf,sizeof(buf),NULL,NULL );
			buf[len] = 0;
			const_cast<logging_event&>(event).get_message().assign( buf, len );
		}
#else
		(*stream_) << color_codes[event.get_priority()/10000];
#endif
		_get_layout().format(event,*stream_);
#if defined(WIN32) || defined(WIN64)
		stream_->flush();
		SetConsoleTextAttribute(out_handle_, defualt_attributes_);
		stream_->flush();
#else
		*stream_ << "\033[0m";
		stream_->flush();
#endif
	}

	void console_appender::append_fixed_color( const logging_event& event )
	{
#if defined(WIN32) || defined(WIN64)
		SetConsoleTextAttribute( out_handle_, fixed_color_attributes_ );
		if ( utf8_ )
		{
			wchar_t wbuf[LOGGING_MAX_MESSAGE_SIZE];
			char	buf[LOGGING_MAX_MESSAGE_SIZE*4];
			int len = MultiByteToWideChar( CP_UTF8, 0, event.get_message().c_str(), event.get_message().length(),wbuf,sizeof(wbuf) );
			len = WideCharToMultiByte( GetOEMCP(),0,wbuf,len,buf,sizeof(buf),NULL,NULL );
			buf[len] = 0;
			const_cast<logging_event&>(event).get_message().assign( buf, len );
		}
#else
		*stream_ << fixed_color_escape_string_;
#endif
		_get_layout().format(event,*stream_);
#if defined(WIN32) || defined(WIN64)
		stream_->flush();
		SetConsoleTextAttribute( out_handle_, defualt_attributes_ );
#else
		*stream_ << "\033[0m";
		*stream_ << suffix_;
		stream_->flush();
#endif
	}

	void console_appender::set_fixed_color( color foreground_color, color background_color )
	{
#if defined(WIN32) || defined(WIN64)
		static WORD 	foreground_colors[] = {
			0 // Black
			,FOREGROUND_RED//Red
			,FOREGROUND_GREEN//green
			,FOREGROUND_GREEN | FOREGROUND_RED//yellow
			,FOREGROUND_BLUE//Blue
			,FOREGROUND_RED | FOREGROUND_BLUE//Magenta
			,FOREGROUND_BLUE | FOREGROUND_GREEN//Cyan
			,FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN//White
			,FOREGROUND_RED | FOREGROUND_INTENSITY//BrightRed
			,FOREGROUND_GREEN | FOREGROUND_INTENSITY//BrightGreen
			,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY//yellow
			,FOREGROUND_BLUE | FOREGROUND_INTENSITY//BrightBlue
			,FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY//BrightMagenta
			,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY//BrightCyan
			,FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY//BrightWhite
			,0
		};

		static WORD 	background_colors[] = {
			0 // Black
			,BACKGROUND_RED//Red
			,BACKGROUND_GREEN//green
			,BACKGROUND_GREEN | BACKGROUND_RED//yellow
			,BACKGROUND_BLUE//Blue
			,BACKGROUND_RED | BACKGROUND_BLUE//Magenta
			,BACKGROUND_BLUE | BACKGROUND_GREEN//Cyan
			,BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN//White
			,BACKGROUND_RED | BACKGROUND_INTENSITY//BrightRed
			,BACKGROUND_GREEN | BACKGROUND_INTENSITY//BrightGreen
			,BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY//BrightYellow
			,BACKGROUND_BLUE | BACKGROUND_INTENSITY//BrightBlue
			,BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY//BrightMagenta
			,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY//BrightCyan
			,BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY//BrightWhite
			,0
		};
		if ( foreground_color == DefaultColor && background_color == DefaultColor )
		{
			fixed_color_attributes_ = defualt_attributes_;
		}
		else
		{
			fixed_color_attributes_	= foreground_colors[foreground_color] | background_colors[background_color];
		}
#else
		static const char*	foreground_colors[] = {
			"30"//Black
			,"31"//Red
			,"32"//Green
			,"33"//Yellow
			,"34"//Blue
			,"35"//Magenta
			,"36"//Cyan
			,"37"//White
			,"1;31"//BrightRed
			,"1;32"//BrightGreen
			,"1;33"//BrightYellow
			,"1;34"//BrightBlue
			,"1;35"//BrightMagenta
			,"1;36"//BrightCyan
			,"1;37"//BrightWhite
			,""//DefaultColor
		};
		static const char*	background_colors[] = {
			"40"//Black
			,"41"//Red
			,"42"//Green
			,"43"//Yellow
			,"44"//Blue
			,"45"//Magenta
			,"46"//Cyan
			,"47"//White
			,"1;41"//BrightRed
			,"1;42"//BrightGreen
			,"1;43"//BrightYellow
			,"1:44"//BrightBlue
			,"1:45"//BrightMagenta
			,"1:46"//BrightCyan
			,"1;47"//BrightWhite
			,""//DefaultColor
		};
		if ( foreground_color == DefaultColor && background_color == DefaultColor )
		{
			fixed_color_escape_string_.clear();
		}
		else if ( foreground_color == DefaultColor )
		{
			std::ostringstream oss;
			oss << "\033[" << background_colors[background_color] << "m";
			fixed_color_escape_string_ = oss.str();
		}
		else if ( background_color == DefaultColor )
		{
			std::ostringstream oss;
			oss << "\033[" << foreground_colors[foreground_color] << "m";
			fixed_color_escape_string_ = oss.str();
		}
		else
		{
			std::ostringstream oss;
			oss << "\033[" << foreground_colors[foreground_color] << ";" << background_colors[background_color] << "m";
			fixed_color_escape_string_ = oss.str();
		}

#endif
		fixed_color_ = true;
	}

	void console_appender::alloc_console()
	{
#if defined(WIN32) || defined(WIN64)
		AllocConsole();
#else
		
#endif
	}

	void console_appender::set_utf8( bool value )
	{
		utf8_ = value;
	}

	shared_ptr<console_appender> console_appender::create( const std::string& name, bool log2std_err /*= false*/, layout* _layout /*= 0 */ )
	{
		shared_ptr<console_appender>	p( new console_appender(name,log2std_err,_layout) );
		appender::add_appender( p );

		return p;
	}

	void console_appender::set_target( const std::string& target )
	{
		
#if defined(WIN32) || defined(WIN64)
		if (target == "stdout")
		{
			mutex::scoped_lock	lock(mutex_);
			out_handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
		}
		else if (target == "stderr")
		{
			mutex::scoped_lock	lock(mutex_);
			out_handle_ = GetStdHandle(STD_ERROR_HANDLE);
		}
		else
		{
			LOG_WARN("loglog","console_appender_creator::create_appender - console_appender only support stdout and stderr");
		}
#else//WIN32
		if (target == "stdout")
		{
			mutex::scoped_lock	lock(mutex_);
			stream_ = &std::cout;
		}
		else if (target == "stderr")
		{
			mutex::scoped_lock	lock(mutex_);
			stream_ = &std::cerr;
		}
		else
		{
			LOG_WARN("loglog","console_appender_creator::create_appender - console_appender only support stdout and stderr");
		}
#endif//WIN32

	}

	void console_appender::set_priority_color()
	{
		fixed_color_ = false;
	}

	static console_appender::color get_color( const std::string& color_string )
	{
		std::string cs = boost::algorithm::to_lower_copy( color_string );

		typedef console_appender c;

		if ( cs == "black" )
			return c::Black;
		else if (cs == "red")
			return c::Red;
		else if (cs == "green")
			return c::Green;
		else if (cs == "yellow")
			return c::Yellow;
		else if (cs == "blue")
			return c::Blue;
		else if (cs == "magenta")
			return c::Magenta;
		else if (cs == "cyan")
			return c::Cyan;
		else if (cs == "grey")
			return c::Grey;
		else if (cs == "brightred")
			return c::BrightRed;
		else if (cs == "brightgreen")
			return c::BrightGreen;
		else if (cs == "brightyellow")
			return c::BrightYellow;
		else if (cs == "brightblue")
			return c::BrightBlue;
		else if (cs == "brightmagenta")
			return c::BrightMagenta;
		else if (cs == "brightcyan")
			return c::BrightCyan;
		else if (cs == "white")
			return c::White;
		else if (cs == "defaultcolor")
			return c::DefaultColor;
		else
			return c::DefaultColor;
	}


	shared_ptr<appender> console_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		if(appender::get_appender(name))
			throw std::invalid_argument(std::string("Appender name \"")+name+"\" existed.");

		std::auto_ptr<layout> l = layout_factory::get_instance().create(params.get_child("layout"));
		std::string target_type = params.get<std::string>("target","stdout");
		boost::algorithm::to_lower(target_type);
		bool log2std_err = false;
		if(target_type=="stdout")
		{
			log2std_err = false;
		}
		else if(target_type=="stderr")
		{
			log2std_err = true;
		}
		else
		{
			LOG_WARN("loglog","console_appender_creator::create_appender - console_appender only support stdout and stderr");
		}

		bool fixed = params.get("fixed",false);
		console_appender::color	fc,bc;
		if ( fixed )
		{
			std::string foreground_color = params.get<std::string>( "foreground_color", "default" );
			std::string background_color = params.get<std::string>( "background_color", "default" );
			fc = get_color(foreground_color);
			bc = get_color(background_color);
		}

		shared_ptr<console_appender> pAppender = console_appender::create( name, log2std_err, l.release() );

		if (fixed)
		{
			pAppender->set_fixed_color( fc, bc );
		}

		return pAppender;
	}

	std::string console_appender_creator::get_name() const
	{
		return "console_appender";
	}
}
