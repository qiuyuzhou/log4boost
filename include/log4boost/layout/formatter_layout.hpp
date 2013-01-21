/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef LOGGING_FORMATTER_LAYOUT_H__
#define LOGGING_FORMATTER_LAYOUT_H__

#include <log4boost/config.hpp>
#include <log4boost/logging_event.hpp>
#include <log4boost/layout/layout.hpp>
#include <log4boost/ndc.hpp>
#include <log4boost/priority.hpp>

#include <log4boost/detail/thread_id.hpp>
#include <log4boost/detail/process_id.hpp>
#include <log4boost/detail/time_format_holder.hpp>


#include <time.h>
#include <vector>

#include <boost/function.hpp>


namespace log4boost
{
	namespace formatter
	{
		namespace detail
		{
			/**
			 *	The parent class of all formatter.
			 */
			template< typename char_type >
			struct basic_formatter_base
			{
				virtual void operator()( const logging_event &event, std::basic_ostream<char_type>& out ) = 0;
			};

			typedef basic_formatter_base<char> formatter_base;
			typedef basic_formatter_base<wchar_t> wformatter_base;
		}

		/**
		 *	Appends the thread id to the output.
		 */
		struct thread_id
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<log4boost::detail::thread_name::get();
			}
		};

		/**
		 *	Appends the process id to the output.
		 */
		struct process_id
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<log4boost::detail::process_name::get();
			}
		};

		/**
		 *	Appends the timestamp to the output.
		 */
		struct timestamp
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				::time_t val = ::time(0); 
				out<<val;
			}
		};

		/**
		@brief Appends the message with the time. You pass the format string at construction.

		The format can contain escape sequences:
		$dd - day, 2 digits
		$MM - month, 2 digits
		$yy - year, 2 digits
		$yyyy - year, 4 digits
		$hh - hour, 2 digits
		$mm - minute, 2 digits
		$ss - second, 2 digits

		Example: time("Today is $dd/$MM/$yyyy");

		Note: for a high precision clock, try high_precision_time (uses boost::date_time)

		*/
		struct time 
			: detail::formatter_base, detail::time_format_holder
		{
			time( const char* time_format )
				:detail::time_format_holder(time_format)
			{}

			void operator()( const logging_event &event, std::ostream& out );
		};

		/**
		@brief Appends the message with a high-precision time (. You pass the format string at construction.

		Internally, it uses boost::date_time::microsec_time_clock. So, our precision matches this class.

		The format can contain escape sequences:
		$dd - day, 2 digits
		$MM - month, 2 digits
		$yy - year, 2 digits
		$yyyy - year, 4 digits
		$hh - hour, 2 digits
		$mm - minute, 2 digits
		$ss - second, 2 digits
		$mili - milliseconds 
		$micro - microseconds (if the high precision clock allows; otherwise, it pads zeros)
		$nano - nanoseconds (if the high precision clock allows; otherwise, it pads zeros)


		Example: 

		@code
		high_precision_time("$mm:$ss:$micro");
		@endcode

		*/
		struct high_precision_time 
			: detail::formatter_base, detail::time_format_holder
		{
			high_precision_time( const char* time_format )
				:detail::time_format_holder(time_format)
			{}

			void operator()( const logging_event &event, std::ostream& out );
		};

		/**
		 *	Appends the nested diagnose context to the output.
		 */
		struct ndc
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out << event.get_ndc();
			}
		};

		/**
		 *	Appends the name of the logger to the output.
		 */
		struct logger_name
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out << event.get_logger_name();
			}
		};

		/**
		 *	Appends the message to the output.
		 */
		struct message
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out << event.get_message();
			}
		};

		/**
		 *	Appends the level of the priority to the output.
		 */
		struct priority
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<log4boost::priority::get_fix_length_name(event.get_priority());
			}
		};

		/**
		 *	Appends the the path of the file where logged the message.
		 */
		struct file
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<event.get_file_name();
			}
		};

		/**
		 *	Appends the the line number of the file where logged the message.
		 */
		struct line_no
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<event.get_line_no();
			}
		};

		/** @brief Prepends some info, and appends some info to an existing formatter.

		The syntax is simple: construct a spacer by passing the original formatter, and the text to space (prepend and append).
		Use:
		- @c % to mean the original formatter text
		- anything before @c "%" is prepended before
		- anything after @c "%" is appended after

		Examples:

		@code
		// prefix "[" before index, and append "] " after it.
		formatter::spacer( formatter::idx(), "[%] ");

		// prefix "{T" before thread_id, and append "} " after it
		formatter::spacer( formatter::thread_id(), "{T%} ");
		@endcode

		When adding a spacer formatter, you'll do something similar to:

		@code
		formatter_layout_p->add_formatter(formatter::spacer( formatter::thread_id(), "{T%} "));
		@endcode

		However, to make this even simpler, I allow an ever easier syntax:

		@code
		// equivalent to the above
		formatter_layout_p->add_formatter( formatter::thread_id(), "{T%} " );
		@endcode

		*/
		template< class original_formatter >
		struct spacer 
			: original_formatter
		{
			typedef original_formatter	spacer_base;
			typedef std::string			string_type;

			spacer( const original_formatter& fmter, const char* format_str ):spacer_base(fmter)
			{
				parse_format(format_str);
			}

			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<prefix_;
				spacer_base::operator()(event,out);
				out<<suffix_;
			}

		private:
			void parse_format( const string_type & format_str )
			{
				typedef typename string_type::size_type size_type;
				size_type msg_idx = format_str.find('%');
				if ( msg_idx != string_type::npos)
				{
					prefix_ = format_str.substr(0, msg_idx);
					suffix_ = format_str.substr( msg_idx + 1);
				}
				else
				{
					// no suffix
					prefix_ = format_str;
				}
			}

		private:
			string_type prefix_, suffix_;
		};

		/**
		 *	Prepends some pad to an existing formatter.
		 */
		template< typename original_formatter >
		struct width
			: original_formatter
		{
			width( const original_formatter& fmter, const std::streamsize at_least_size, const char pad_ch )
				:original_formatter(fmter)
				,at_least_size_(at_least_size)
				,pad_ch_(pad_ch)
			{
				if (at_least_size_>128)
				{
					at_least_size_ = 128;
				}
			}

			void operator()( const logging_event &event, std::ostream& out )
			{
				using namespace std;
				out.width( at_least_size_ );
				out.fill( pad_ch_ );
				original_formatter::operator()(event,out);
			}

		private:
			std::streamsize		at_least_size_;
			char			pad_ch_;
		};

		struct source_host_name
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<event.get_source_host_name();
			}
		};

		struct instance_name
			: detail::formatter_base
		{
			void operator()( const logging_event &event, std::ostream& out )
			{
				out<<event.get_instance_name();
			}
		};

	}

	/**
	 *	Format the event by pattern string, or construct the format by hand using member function add_formatter.
	 *
	 *	\par Format Pattern Specification:
	 *		A format specification, which consists of optional and required fields, has the following form:\n
	 *		\%{formatter name}\%[width:n]\n
	 *		The formatter name is the class name of the classes defined in namespace log4boost::formatter.
	 *
	 *	\par Formatter list:
	 *	- log4boost::formatter::thread_id
	 *	- log4boost::formatter::process_id
	 *	- log4boost::formatter::timestamp
	 *	- log4boost::formatter::time
	 *	- log4boost::formatter::high_precision_time
	 *	- log4boost::formatter::ndc
	 *	- log4boost::formatter::logger_name
	 *	- log4boost::formatter::message
	 *	- log4boost::formatter::priority
	 *	- log4boost::formatter::file
	 *	- log4boost::formatter::line_no
	 *	- log4boost::formatter::source_host_name
	 *	- log4boost::formatter::instance_name
	 *
	 *	\n All formatter above could be used in pattern string.
	 *
	 *	
	 *	\par The config options:
	 *	- format	\n Format pattern string. If it's "DEFAULT_FORMAT", "BASIC_FORMAT" or "SIMPLE_FORMAT", 
	 *				it will be replace by the concrete format string defined in the class.
	 *
	 *	\par The predefined format string:
	 *	- log4boost::formatter_layout::DEFAULT_FORMAT	\n \%time\%($hh:$mm:$ss) [\%priority\%width:5\%] \%logger_name\%width:15\% - \%message\%\\n
	 *	- log4boost::formatter_layout::BASIC_FORMAT		\n \%time\%($hh:$mm:$ss) [\%priority\%width:5\%] \%message\%\\n
	 *	- log4boost::formatter_layout::SIMPLE_FORMAT		\n [\%priority\%width:5\%] \%message\%\\n
	 */
	class LOG4BOOST_EXPORT formatter_layout
		:public layout
	{
	public:

		// %time%($hh:$mm:$ss) [%priority%] %logger_name%width:15% - %message%\\n
		static const char* DEFAULT_FORMAT;

		// %time%($hh:$mm:$ss) [%priority%] %message%\\n
		static const char* BASIC_FORMAT;

		// [%priority%] %message%\\n
		static const char* SIMPLE_FORMAT;

		formatter_layout( void );
		formatter_layout( const std::string& format_str );
		virtual ~formatter_layout(void);

		static formatter_layout* create( const std::string& format_str );
		static formatter_layout* create();

		virtual void format(const logging_event &event,std::ostream& out);

		typedef formatter::detail::formatter_base formatter_base;

		/**
		 *	Append a formatter.
		 */
		template<typename T>
		formatter_layout& add_formatter( T fmter)
		{
			formatters_.push_back(new T(fmter));
			return *this;
		}

		/**
		 *	Append a formatter with string surrounded. Implement by formatter::spacer.
		 *
		 *	@see formatter::spacer
		 */
		template<typename T>
		formatter_layout& add_formatter( T fmter, const char* format_str )
		{
			formatters_.push_back(new formatter::spacer<T>(fmter,format_str));
			return *this;
		}

		/**
		 *	Clear existed formatters.
		 */
		void clear();

		/**
		 *	Set the format through the format string.
		 *
		 *	@return True if parse the format string succeed,otherwise false.
		 */
		bool set_format( const std::string& format_str );

	private:
		typedef std::vector< formatter_base* >	formatter_array_t;
		formatter_array_t	formatters_;
	};

}

#endif//LOGGING_PATTERN_LAYOUT_H__
