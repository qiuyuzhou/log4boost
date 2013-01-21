/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_LOGGING_EVENT_H__
#define LOGGING_LOGGING_EVENT_H__

#include <string>

#include <log4boost/config.hpp>
#include <log4boost/priority.hpp>
#include <log4boost/date_time.hpp>


namespace log4boost
{
	namespace detail
	{
		class LOG4BOOST_EXPORT reference_count_base
		{
		public:
			reference_count_base()
				: ref_count_( 0 )
			{}
			reference_count_base( const reference_count_base& )
				:ref_count_( 0 )
			{}

			virtual ~reference_count_base(){}

			reference_count_base& operator=( const reference_count_base& )
			{
				return *this;
			}

			void inc(void)const{ ++ref_count_; }
			void des(void)const{ --ref_count_; }
			std::size_t count()const
			{
				return ref_count_;
			}
		private:
			mutable std::size_t ref_count_;
		};

		LOG4BOOST_EXPORT void intrusive_ptr_add_ref( const reference_count_base* pBase );
		LOG4BOOST_EXPORT void intrusive_ptr_release( const reference_count_base* pBase );

	}

	class LOG4BOOST_EXPORT logging_event : public detail::reference_count_base
	{
	public:
		logging_event(){};

		logging_event(const char* logger_name, std::size_t logger_name_length, const char* _message, std::size_t message_length
			, priority_level priority);

		logging_event(const char* logger_name, std::size_t logger_name_length, const char* _message, std::size_t message_length
			, priority_level priority, const char* _file, int _line );

		logging_event( const std::string& logger_name, const std::string& _message, priority_level priority );
		logging_event( const std::string& logger_name, const std::string& _message, priority_level priority, const char* _file, int _line );
	
		const std::string& get_logger_name()const
		{
			return logger_name;
		}

		const std::string& get_message()const
		{
			return message;
		}

		std::string& get_logger_name()
		{
			return logger_name;
		}

		std::string& get_message()
		{
			return message;
		}

		const std::string& get_thread_id()const;

		const std::string& get_process_id()const;

		const std::string& get_ndc()const;

		std::string& get_thread_id()
		{
			return thread_id;
		}

		std::string& get_process_id()
		{
			return process_id;
		}

		std::string& get_ndc()
		{
			return ndc;
		}

		priority_level get_priority()const
		{
			return priority;
		}

		void set_priority( priority_level level )
		{
			priority = level;
		}

		int	get_line_no()const
		{
			return line_no;
		}

		void set_line_no( int _line_no )
		{
			line_no = _line_no;
		}

		const std::string& get_file_name()const;

		std::string& get_file_name()
		{
			return file_name;
		}

		const date_time& get_precision_time()const;

		time_t	get_time()const;

		void set_precision_time( const date_time& _time )
		{
			precision_time_ = _time;
		}

		void set_time( time_t _time )
		{
			time_ = _time;
		}

		const std::string& get_instance_name()const;

		std::string& get_instance_name()
		{
			return instance_name_;
		}

		const std::string& get_source_host_name()const;

		std::string& get_source_host_name()
		{
			return source_host_name;
		}

	private:
		priority_level	 			priority;

		std::string					logger_name;
		std::string					message;
	
		std::string					thread_id;
		std::string					process_id;
		std::string					ndc;

		std::string					source_host_name;

		mutable std::string			file_name;
		const char*					file_name_ptr;
		int							line_no;

		mutable date_time			precision_time_;
		time_t						time_;
		mutable	std::string			instance_name_;
	};

	void save( const logging_event& event, std::ostream& out );
	void load( logging_event& event, const char* buffer, size_t size );

}


#endif//LOGGING_LOGGING_EVENT_H__
