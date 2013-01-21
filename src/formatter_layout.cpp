/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/layout/formatter_layout.hpp"

#include "log4boost/logger.hpp"
#include "escape_string.hpp"
#include "ptree_inc.h"

#include <boost/spirit.hpp>
#include <boost/spirit/include/classic_actor.hpp>
#include <boost/scoped_array.hpp>

namespace log4boost
{
	namespace formatter
	{
		struct const_str
			: detail::formatter_base
		{
		public:
			const_str( const std::string& str )
				:str_(str)
			{}

			void operator()( const logging_event &event, std::ostream& out )
			{
				out << str_;
			}

		private:
			std::string	str_;
		};

		void time::operator()( const logging_event &event, std::ostream& out )
		{
			::time_t val = event.get_time(); 

			char buffer[64];

			tm* details = localtime( &val);
			detail::time_format_holder::write_time( buffer, details->tm_mday, details->tm_mon + 1, details->tm_year + 1900
				, details->tm_hour, details->tm_min, details->tm_sec);	

			out << buffer;
		}

		void high_precision_time::operator()( const logging_event &event, std::ostream& out )
		{
			boost::posix_time::ptime val = event.get_precision_time().to_ptime();
#if defined(WIN32) || defined(WIN64)
			static boost::posix_time::time_duration tzd = boost::posix_time::seconds( _timezone );
			val += tzd;
#else//TODO
#endif

			char buffer[64];

			int nanosecs = (int)val.time_of_day().fractional_seconds();
			int digits = val.time_of_day().num_fractional_digits();
			switch ( digits)
			{
			case 0: break; // no high precision at all
			case 1: nanosecs *= 100000000; break;
			case 2: nanosecs *= 10000000; break;
			case 3: nanosecs *= 1000000; break;
			case 4: nanosecs *= 100000; break;
			case 5: nanosecs *= 10000; break;
			case 6: nanosecs *= 1000; break;
			case 7: nanosecs *= 100; break;
			case 8: nanosecs *= 10; break;
			case 9: break;
			default: 
				while ( digits > 9) 
				{ 
					nanosecs /= 10; digits--; 
				}
				break;
			}

			detail::time_format_holder::write_time( buffer, 
				val.date().day(),
				val.date().month(),
				val.date().year(),
				val.time_of_day().hours(),
				val.time_of_day().minutes(),
				val.time_of_day().seconds(),
				nanosecs / 1000000,
				nanosecs / 1000,
				nanosecs
				);

			out << buffer;
		}

	}

	namespace detail
	{
		struct formatter_parse_contexct
		{
			formatter_parse_contexct(formatter_layout& fmt)
				:fmt(fmt),pading(false){}

			formatter_layout& fmt;
			char	pad_ch;
			unsigned int at_least_size;
			bool	pading;
			std::string		time_fmt_str;
			std::string		const_str;
		};

		template< typename fomatter_type > 
		struct add_formatter_helper
		{
			add_formatter_helper( formatter_parse_contexct& ctx )
				:ctx_(ctx){}

			void operator()( char const* str, char const* end )const
			{
				if (!ctx_.const_str.empty())
				{
					ctx_.fmt.add_formatter(formatter::const_str(ctx_.const_str));
					ctx_.const_str.clear();
				}
				if (ctx_.pading)
				{
					ctx_.fmt.add_formatter(log4boost::formatter::width<fomatter_type>(fomatter_type(),ctx_.at_least_size,ctx_.pad_ch));
					ctx_.pading = false;
				}
				else
				{
					ctx_.fmt.add_formatter(fomatter_type());
				}
			}
		private:
			formatter_parse_contexct& ctx_;
		};

		template< typename fomatter_type > 
		struct add_time_formatter_helper
		{
			add_time_formatter_helper( formatter_parse_contexct& ctx )
				:ctx_(ctx){}

			void operator()( char const* str, char const* end )const
			{
				if (!ctx_.const_str.empty())
				{
					ctx_.fmt.add_formatter(formatter::const_str(ctx_.const_str));
					ctx_.const_str.clear();
				}
				if (ctx_.pading)
				{
					ctx_.fmt.add_formatter(
						log4boost::formatter::width<fomatter_type>(fomatter_type(ctx_.time_fmt_str.c_str()),ctx_.at_least_size,ctx_.pad_ch)
						);
					ctx_.pading = false;
				}
				else
				{
					ctx_.fmt.add_formatter(fomatter_type(ctx_.time_fmt_str.c_str()));
				}

			}
		private:
			formatter_parse_contexct& ctx_;
		};

		bool parse_format_str( formatter_layout& fmt, const std::string& format_str )
		{
			using detail::add_formatter_helper;
			using detail::add_time_formatter_helper;
			using detail::formatter_parse_contexct;
			using namespace log4boost::formatter;
			using namespace boost::spirit;

			formatter_parse_contexct ctx(fmt);

			rule<> padding_p = str_p("width:") 
				>> uint_parser<unsigned, 10, 1, 3>()[assign_a(ctx.at_least_size)][assign_a(ctx.pad_ch,' ')]
				>> !( ch_p(':') >> anychar_p[assign_a(ctx.pad_ch)] ) 
				>> ch_p('%')[assign_a(ctx.pading,true)];


			rule<> thread_id_p = (str_p("%thread_id%") >> (!padding_p)) [add_formatter_helper<thread_id>(ctx)];
			rule<> process_id_p = (str_p("%process_id%") >> (!padding_p)) [add_formatter_helper<process_id>(ctx)];
			rule<> timestamp_p = (str_p("%timestamp%") >> (!padding_p)) [add_formatter_helper<timestamp>(ctx)];
			rule<> source_host_name_p = (str_p("%source_host_name%") >> (!padding_p)) [add_formatter_helper<source_host_name>(ctx)];
			rule<> instance_name_p = (str_p("%instance_name%") >> (!padding_p)) [add_formatter_helper<instance_name>(ctx)];

			rule<> time_p = (str_p("%time%")[clear_a(ctx.time_fmt_str)]
				>> ( confix_p("(",(*anychar_p)[assign_a(ctx.time_fmt_str)],")")
				|confix_p("[",(*anychar_p)[assign_a(ctx.time_fmt_str)],"]")
				|confix_p("{",(*anychar_p)[assign_a(ctx.time_fmt_str)],"}")
				|confix_p("$",(*anychar_p)[assign_a(ctx.time_fmt_str)],"$"))
				>> (!padding_p) 
				)[add_time_formatter_helper<time>(ctx)];

			rule<> high_precision_time_p = (str_p("%high_precision_time%")[clear_a(ctx.time_fmt_str)] 
				>> ( confix_p("(",(*anychar_p)[assign_a(ctx.time_fmt_str)],")")
				|confix_p("[",(*anychar_p)[assign_a(ctx.time_fmt_str)],"]")
				|confix_p("{",(*anychar_p)[assign_a(ctx.time_fmt_str)],"}")
				|confix_p("$",(*anychar_p)[assign_a(ctx.time_fmt_str)],"$")) 
				>> (!padding_p)
				)[add_time_formatter_helper<high_precision_time>(ctx)];

			rule<> ndc_p = (str_p("%ndc%") >> (!padding_p)) [add_formatter_helper<formatter::ndc>(ctx)];
			rule<> logger_name_p = (str_p("%logger_name%") >> (!padding_p)) [add_formatter_helper<logger_name>(ctx)];
			rule<> message_p = (str_p("%message%") >> (!padding_p)) [add_formatter_helper<message>(ctx)];
			rule<> priority_p = (str_p("%priority%") >> (!padding_p)) [add_formatter_helper<formatter::priority>(ctx)];
			rule<> file_p = (str_p("%file%") >> (!padding_p)) [add_formatter_helper<formatter::file>(ctx)];
			rule<> line_no_p = (str_p("%line_no%") >> (!padding_p)) [add_formatter_helper<formatter::line_no>(ctx)];


			rule<> format_str_p = *(thread_id_p|process_id_p|timestamp_p
				|source_host_name_p|instance_name_p
				|time_p|high_precision_time_p|ndc_p
				|logger_name_p|message_p|priority_p|file_p|line_no_p
				|anychar_p[push_back_a(ctx.const_str)]);

			bool result = parse(format_str.c_str(),format_str_p).full;

			if ( result && !ctx.const_str.empty() )
			{
				ctx.fmt.add_formatter(formatter::const_str(ctx.const_str));
				ctx.const_str.clear();
			}
			return result;
		}
	}


	formatter_layout::formatter_layout( void )
	{
	}

	formatter_layout::formatter_layout(const std::string& format_str)
	{
		set_format(format_str);
	}

	void formatter_layout::format(const logging_event &event,std::ostream& out)
	{
		for(formatter_array_t::const_iterator itr=formatters_.begin(),end_itr=formatters_.end()
			;itr!=end_itr;++itr)
		{
			(**itr)(event,out);
		}
	}

	formatter_layout::~formatter_layout( void )
	{
		for(formatter_array_t::iterator itr=formatters_.begin(),end_itr=formatters_.end()
			;itr!=end_itr;++itr)
		{
			delete (*itr);
		}
	}

	void formatter_layout::clear()
	{
		for(formatter_array_t::iterator itr=formatters_.begin(),end_itr=formatters_.end()
			;itr!=end_itr;++itr)
		{
			delete (*itr);
		}
		formatters_.clear();
	}

	bool formatter_layout::set_format( const std::string& format_str )
	{
		clear();
		return detail::parse_format_str(*this,format_str);
	}

	formatter_layout* formatter_layout::create( const std::string& format_str )
	{
		return new formatter_layout(format_str);
	}

	formatter_layout* formatter_layout::create()
	{
		return new formatter_layout();
	}

	std::auto_ptr<layout> create_formatter_layout(const property_tree::ptree& params)
	{
		std::string ptn = params.get<std::string>("format","DEFAULT_FORMAT");

		if(ptn=="DEFAULT_FORMAT")
		{
			ptn = formatter_layout::DEFAULT_FORMAT;
		}
		else if(ptn=="SIMPLE_FORMAT")
		{
			ptn = formatter_layout::SIMPLE_FORMAT;
		}
		else if(ptn=="BASIC_FORMAT")
		{
			ptn = formatter_layout::BASIC_FORMAT;
		}

		ptn = detail::unescape_string(ptn);

		std::auto_ptr<layout> l(new formatter_layout(ptn));
		return l;
	}

	const char* formatter_layout::DEFAULT_FORMAT= "%time%($hh:$mm:$ss) [%priority%] %logger_name%width:15% - %message%\n";
	const char* formatter_layout::BASIC_FORMAT	= "%time%($hh:$mm:$ss) [%priority%] %message%\n";
	const char* formatter_layout::SIMPLE_FORMAT	= "[%priority%] %message%\n";

}
