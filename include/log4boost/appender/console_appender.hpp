/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef _LOGGING_CONSOLE_APPENDER_HPP__45848__
#define _LOGGING_CONSOLE_APPENDER_HPP__45848__

#include <log4boost/config.hpp>

#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/appender_creator_base.hpp>

#include <log4boost/detail/mutex.hpp>

namespace log4boost
{

	/**
	 *	The appender would write the log to the console using the specified layout with different color depend on priority.
	 *
	 *	\par The config options:
	 *	- \b target		\n Only could be "stdout" and "stderr".
	 *	- \b layout		\n The layout class type the appender would be used.
	 *	- \b fixed		\n 1 or 0.
	 *	- \b foreground_color	\n
	 *	- \b background_color	\n
	 */
	class LOG4BOOST_EXPORT console_appender 
		: public layout_appender
	{
	public:
		enum color{ 
			Black, Red, Green, Yellow, 
			Blue, Magenta, Cyan, Grey, 
			BrightRed, BrightGreen, BrightYellow, 
			BrightBlue, BrightMagenta, BrightCyan, White, DefaultColor
		};

		virtual ~console_appender();

		virtual void reopen(){}

		virtual void close(){}

		void set_fixed_color( color foreground_color, color background_color );
		void set_priority_color();

		/**
		 *	Sets the value of the <b>target</b> property.
		 *	@param target Recognized values are "stdout" and "stderr". Any other value will be ignored.
		 */
		void set_target( const std::string& target );

		void set_utf8( bool value );

		/**
		 *	Allocates a new console for the calling process if it don't has a console.
		 *	Only for WIN32, it's useful for win32 GUI application.
		 */
		void alloc_console();

		static shared_ptr<console_appender> create( const std::string& name, bool log2std_err = false, layout* _layout = 0 );

	private:
		console_appender( const std::string& name, bool log2std_err = false, layout* _layout = 0 );

		virtual void _append( const logging_event& event );

		inline void append_color_by_priority( const logging_event& event );
		inline void append_fixed_color( const logging_event& event );


		std::ostream* stream_;
		mutex	mutex_;
		bool	fixed_color_;
		std::string		prefix_;
		std::string		suffix_;
		bool			utf8_;
#if defined(WIN32) || defined(WIN64)
		HANDLE	out_handle_;
		WORD	defualt_attributes_;
		WORD	fixed_color_attributes_;
#else
		std::string	fixed_color_escape_string_;
#endif
	};

	class LOG4BOOST_EXPORT console_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};


}

#endif//_LOGGING_CONSOLE_APPENDER_HPP__45848__
