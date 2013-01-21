/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_STACK_OSTREAM_HPP__
#define LOGGING_STACK_OSTREAM_HPP__

#include <boost/iostreams/stream.hpp>

namespace log4boost
{
	namespace detail
	{
		/**
		 *	Like a ostream. There's no new,use memory on stack.
		 */
		template<typename char_type,std::size_t buffer_size = LOGGING_MAX_MESSAGE_SIZE>
		class basic_stack_ostream
		{
		public:
			typedef boost::iostreams::stream_buffer<boost::iostreams::basic_array_sink<char_type> >	array_stream_buffer_t;
			typedef std::basic_ostream<char_type> out_t;

			basic_stack_ostream()
				:array_stream_buffer_(buffer_)
				,out_(&array_stream_buffer_)
			{}

			out_t&	get_out()
			{
				return out_;
			}

			const char_type* str()const
			{
				return buffer_;
			}

			std::size_t	length()
			{
				return (std::size_t)out_.tellp();
			}

			inline void fill_end_null()
			{
				std::size_t pos = out_.tellp();
				if (pos<buffer_size)
				{
					buffer_[pos] = 0;
				}
				else
				{
					buffer_[buffer_size-1] = 0;
				}
			}

			template<typename T>
			basic_stack_ostream<char_type,buffer_size>& operator<<( const T& val )
			{
				out_<<val;
				return *this;
			}

		private:
			char_type				buffer_[buffer_size];
			array_stream_buffer_t	array_stream_buffer_;
			out_t		out_;
		};

		// if the stack here is too big(4096 * 16), it will cause stack overflow in other place
		typedef basic_stack_ostream<char, 4096 * 2>		stack_ostream;
		typedef basic_stack_ostream<wchar_t, 4096 * 2>	wstack_ostream;

	//	template<typename char_type,std::size_t buffer_size = LOGGING_MAX_MESSAGE_SIZE>
	//	class basic_stack_istream
	//	{
	//	public:
	//		typedef boost::iostreams::stream_buffer<boost::iostreams::basic_array_source<char_type> >	array_stream_buffer_t;
	//		typedef std::basic_istream<char_type> in_t;

	//		basic_stack_istream()
	//			:array_stream_buffer_(buffer_)
	//			,in_(&array_stream_buffer_)
	//		{}

	//		in_t&	get_istream()
	//		{
	//			return in_;
	//		}

	//		const char_type* str()const
	//		{
	//			return buffer_;
	//		}

	///*		std::size_t	length()
	//		{
	//			return in_.tellp();
	//		}*/

	//		template<typename T>
	//		basic_stack_istream<char_type,buffer_size>& operator>>( T& val )
	//		{
	//			in_ >> val;
	//			return *this;
	//		}

	//	private:
	//		char_type				buffer_[buffer_size];
	//		array_stream_buffer_t	array_stream_buffer_;
	//		in_t		in_;
	//	};
	}
}


#endif//LOGGING_STACK_OSTREAM_HPP__

