/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/logging_event.hpp"

#include "io.hpp"
#include "log4boost/logger.hpp"
#include "log4boost/ndc.hpp"
#include "log4boost/detail/process_id.hpp"
#include "log4boost/detail/thread_id.hpp"

#include <boost/asio.hpp>

#include <iterator>

namespace log4boost
{
	namespace detail
	{

		void intrusive_ptr_add_ref( const reference_count_base* pBase )
		{
			pBase->inc();
		}

		void intrusive_ptr_release( const reference_count_base* pBase )
		{
			pBase->des();
			if ( pBase->count() == 0 )
			{
				delete pBase;
			}
		}
	}

	logging_event::logging_event( const char* logger_name, std::size_t logger_name_length, const char* _message, std::size_t message_length
		, priority_level priority )
		:logger_name(logger_name,logger_name_length)
		,priority(priority)
		,line_no(-1)
		,file_name_ptr(0)
		,time_(0)
	{
		message.reserve(128);
		message.assign( _message, message_length );
	}

	logging_event::logging_event( const char* logger_name, std::size_t logger_name_length, const char* _message, std::size_t message_length 
		, priority_level priority, const char* _file, int _line )
		:logger_name( logger_name, logger_name_length )
		,priority( priority )
		,line_no( _line )
		,file_name_ptr(_file)
		,time_(0)
	{
		message.reserve(128);
		message.assign( _message, message_length );
	}

	logging_event::logging_event( const std::string& logger_name, const std::string& _message, priority_level priority )
		:logger_name(logger_name)
		,priority(priority)
		,line_no(-1)
		,file_name_ptr(0)
		,time_(0)
	{
		message.reserve(128);
		message.assign( _message );
	}

	logging_event::logging_event( const std::string& logger_name, const std::string& _message, priority_level priority, const char* _file, int _line )
		:logger_name( logger_name )
		,priority( priority )
		,line_no( _line )
		,file_name_ptr(_file)
		,time_(0)
	{
		message.reserve(128);
		message.assign( _message );
	}

	const std::string& logging_event::get_thread_id() const
	{
		if ( thread_id.empty() )
		{
			return detail::thread_name::get();
		}
		else
		{
			return thread_id;
		}
	}

	const std::string& logging_event::get_process_id() const
	{
		if ( process_id.empty() )
		{
			return detail::process_name::get();
		}
		else
		{
			return process_id;
		}
	}

	const std::string& logging_event::get_ndc() const
	{
		if ( this->ndc.empty() && !ndc::empty() )
		{
			return ndc::get();
		}
		else
		{
			return this->ndc;
		}
	}

	const std::string& logging_event::get_file_name() const
	{
		if ( file_name.empty() && file_name_ptr!=0 )
		{
			file_name.reserve( 256 );
			file_name.assign( file_name_ptr );
		}
		return file_name;
	}

	const date_time& logging_event::get_precision_time() const
	{
		if (precision_time_.to_ptime().is_not_a_date_time())
		{
			precision_time_ = date_time::from_ptime( boost::posix_time::microsec_clock::universal_time() );
		}
		return precision_time_;
	}

	time_t logging_event::get_time() const
	{
		if ( 0==time_ )
		{
			return ::time(0);
		}
		else
		{
			return time_;
		}
	}

	const std::string& logging_event::get_instance_name()const
	{
		if ( instance_name_.empty() )
		{
			return logger::get_instance_name();
		}
		else
		{
			return instance_name_;
		}
	}

	const std::string& logging_event::get_source_host_name() const
	{
		static std::string cachedHostName = boost::asio::ip::host_name();
		if ( source_host_name.empty() )
		{
			return cachedHostName;
		}
		else
		{
			return source_host_name;
		}
	}

	template< typename OutIt >
	static void save( const logging_event& event, OutIt& out_itr )
	{
		io::write_string_16( event.get_source_host_name(), out_itr );
		io::write_string_16( event.get_instance_name(), out_itr );
		io::write_string_16( event.get_logger_name(), out_itr );
		io::write_uint32( event.get_priority(), out_itr );
		io::write_string_16( event.get_message(), out_itr );
		io::write_ptime( event.get_precision_time().to_ptime(), out_itr );
		io::write_uint64( event.get_time(), out_itr );
		io::write_string_16( event.get_ndc(), out_itr );
		io::write_string_8( event.get_process_id(), out_itr );
		io::write_string_8( event.get_thread_id(), out_itr );
		io::write_string_16( event.get_file_name(), out_itr );
		io::write_uint32( event.get_line_no(), out_itr );
	}

	template< typename InIt >
	static void load( logging_event& event, InIt& in_itr, const InIt& end_itr )
	{
		io::read_string_16( event.get_source_host_name(), in_itr, end_itr );
		io::read_string_16( event.get_instance_name(), in_itr, end_itr );
		io::read_string_16( event.get_logger_name(), in_itr, end_itr );
		event.set_priority( static_cast<priority_level>( io::read_uint32( in_itr, end_itr ) ) );
		io::read_string_16( event.get_message(), in_itr, end_itr );
		event.set_precision_time( date_time::from_ptime( io::read_ptime( in_itr, end_itr ) ) );
		event.set_time( static_cast<time_t>(io::read_uint64( in_itr, end_itr ) ) );
		io::read_string_16( event.get_ndc(), in_itr, end_itr );
		io::read_string_8( event.get_process_id(), in_itr, end_itr );
		io::read_string_8( event.get_thread_id(), in_itr, end_itr );
		io::read_string_16( event.get_file_name(), in_itr, end_itr );
		event.set_line_no( io::read_uint32( in_itr, end_itr ) );
	}

	void save( const logging_event& event, std::ostream& out )
	{
		std::ostream_iterator<char> out_itr(out);
		save( event, out_itr );
	}

	void load( logging_event& event, const char* buffer, size_t size )
	{
		load( event, buffer, buffer + size );
	}

}
