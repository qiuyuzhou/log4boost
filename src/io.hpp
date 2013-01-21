/*

Copyright (c) 2003, Arvid Norberg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef LOGGING_IO_HPP_INCLUDED
#define LOGGING_IO_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <string>
#include <algorithm>
#include <iterator>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace log4boost
{
	namespace io
	{
		template <class T> struct type {};

		// reads an integer from a byte stream
		// in big endian byte order and converts
		// it to native endianess
		template <class T, class InIt>
		inline T read_impl(InIt& start, type<T>)
		{
			T ret = 0;
			for (int i = 0; i < (int)sizeof(T); ++i)
			{
				ret <<= 8;
				ret |= static_cast<unsigned char>(*start);
				++start;
			}
			return ret;
		}

		template <class T, class InIt>
		inline T read_impl(InIt& start, const InIt& end, type<T> )
		{
			if ( start + sizeof(T) > end )
			{
				throw std::runtime_error("read_impl: out of bounds");
			}

			T ret = 0;
			for (int i = 0; i < (int)sizeof(T); ++i)
			{
				ret <<= 8;
				ret |= static_cast<unsigned char>(*start);
				++start;
			}
			return ret;
		}

		template <class T, class OutIt>
		inline void write_impl(T val, OutIt& start)
		{
			for (int i = (int)sizeof(T)-1; i >= 0; --i)
			{
				*start = static_cast<unsigned char>((val >> (i * 8)) & 0xff);
				++start;
			}
		}

		// -- adaptors

		template <class InIt>
		boost::int64_t read_int64(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<boost::int64_t>()); }

		template <class InIt>
		boost::uint64_t read_uint64(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<boost::uint64_t>()); }

		template <class InIt>
		boost::uint32_t read_uint32(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<boost::uint32_t>()); }

		template <class InIt>
		boost::int32_t read_int32(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<boost::int32_t>()); }

		template <class InIt>
		boost::int16_t read_int16(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<boost::int16_t>()); }

		template <class InIt>
		uint16_t read_uint16(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<uint16_t>()); }

		template <class InIt>
		boost::int8_t read_int8(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<boost::int8_t>()); }

		template <class InIt>
		boost::uint8_t read_uint8(InIt& start, const InIt& end)
		{ return read_impl(start, end, type<boost::uint8_t>()); }



		template <class InIt>
		boost::int64_t read_int64(InIt& start)
		{ return read_impl(start, type<boost::int64_t>()); }

		template <class InIt>
		boost::uint64_t read_uint64(InIt& start)
		{ return read_impl(start, type<boost::uint64_t>()); }

		template <class InIt>
		boost::uint32_t read_uint32(InIt& start)
		{ return read_impl(start, type<boost::uint32_t>()); }

		template <class InIt>
		boost::int32_t read_int32(InIt& start)
		{ return read_impl(start, type<boost::int32_t>()); }

		template <class InIt>
		boost::int16_t read_int16(InIt& start)
		{ return read_impl(start, type<boost::int16_t>()); }

		template <class InIt>
		uint16_t read_uint16(InIt& start)
		{ return read_impl(start, type<uint16_t>()); }

		template <class InIt>
		boost::int8_t read_int8(InIt& start)
		{ return read_impl(start, type<boost::int8_t>()); }

		template <class InIt>
		boost::uint8_t read_uint8(InIt& start)
		{ return read_impl(start, type<boost::uint8_t>()); }



		template <class OutIt>
		void write_uint64(boost::uint64_t val, OutIt& start)
		{ write_impl(val, start); }

		template <class OutIt>
		void write_int64(boost::int64_t val, OutIt& start)
		{ write_impl(val, start); }

		template <class OutIt>
		void write_uint32(boost::uint32_t val, OutIt& start)
		{ write_impl(val, start); }

		template <class OutIt>
		void write_int32(boost::int32_t val, OutIt& start)
		{ write_impl(val, start); }

		template <class OutIt>
		void write_uint16(uint16_t val, OutIt& start)
		{ write_impl(val, start); }

		template <class OutIt>
		void write_int16(boost::int16_t val, OutIt& start)
		{ write_impl(val, start); }

		template <class OutIt>
		void write_uint8(boost::uint8_t val, OutIt& start)
		{ write_impl(val, start); }

		template <class OutIt>
		void write_int8(boost::int8_t val, OutIt& start)
		{ write_impl(val, start); }

		
		template <class OutIt>
		void write_string_16(const std::string& val,OutIt& start)
		{
			write_uint16((uint16_t)val.size(),start);
			std::copy(val.begin(),val.end(),start);
		}

		template <class OutIt>
		void write_string_32(const std::string& val,OutIt& start)
		{
			write_uint32((boost::uint32_t)val.size(),start);
			std::copy(val.begin(),val.end(),start);
		}

		template <class OutIt>
		void write_string_8(const std::string& val,OutIt& start)
		{
			write_uint8((boost::uint8_t)val.size(),start);
			std::copy(val.begin(),val.end(),start);
		}

		template <class InIt>
		void read_string_8(std::string& val,InIt& start)
		{
			size_t size = read_uint8(start);
			InIt itr = start;
			std::advance(start,size);
			val.assign(itr,start);
		}

		template <class InIt>
		void read_string_16(std::string& val,InIt& start)
		{
			size_t size = read_uint16(start);
			InIt itr = start;
			std::advance(start,size);
			val.assign(itr,start);
		}

		template <class InIt>
		void read_string_32(std::string& val,InIt& start)
		{
			size_t size = read_uint32(start);
			InIt itr = start;
			std::advance(start,size);
			val.assign(itr,start);
		}

		template <class InIt>
		void read_string_8(std::string& val,InIt& start,const InIt& end)
		{
			size_t size = read_uint8(start);
			InIt itr = start;
			std::advance(start,size);
			if (start>end)
			{
				throw std::runtime_error("read_string_8: out of bounds");
			}
			val.assign(itr,start);
		}

		template <class InIt>
		void read_string_16(std::string& val,InIt& start,const InIt& end)
		{
			size_t size = read_uint16(start);
			InIt itr = start;
			std::advance(start,size);
			if (start>end)
			{
				throw std::runtime_error("read_string_16: out of bounds");
			}
			val.assign(itr,start);
		}

		template <class InIt>
		void read_string_32(std::string& val,InIt& start,const InIt& end)
		{
			size_t size = read_uint32(start);
			InIt itr = start;
			std::advance(start,size);
			if (start>end)
			{
				throw std::runtime_error("read_string_32: out of bounds");
			}
			val.assign(itr,start);
		}

		template <class OutIt>
		void write_ptime(const boost::posix_time::ptime& time,OutIt& start)
		{
			write_int16((boost::int16_t)time.date().year(),start);
			write_uint8((boost::uint8_t)time.date().month(),start);
			write_uint8((boost::uint8_t)time.date().day(),start);
			
			write_uint8((boost::uint8_t)time.time_of_day().hours(),start);
			write_uint8((boost::uint8_t)time.time_of_day().minutes(),start);
			write_uint8((boost::uint8_t)time.time_of_day().seconds(),start);
			write_int32((boost::int32_t)time.time_of_day().fractional_seconds(), start );
		}
		
		template <class InIt>
		boost::posix_time::ptime read_ptime(InIt& start)
		{
			int year = read_int16(start);
			int month = read_uint8(start);
			int day   = read_uint8(start);
			boost::gregorian::date d(year,month,day);

			int hour = read_uint8(start);
			int minute = read_uint8(start);
			int second = read_uint8(start);
			long fractional_seconds = read_int32(start);
			boost::posix_time::time_duration	td(hour,minute,second,fractional_seconds);
			return boost::posix_time::ptime(d,td);
        }

		template <class InIt>
		boost::posix_time::ptime read_ptime(InIt& start,const InIt& end)
		{
			int year = read_int16(start,end);
			int month = read_uint8(start,end);
			int day   = read_uint8(start,end);
			boost::gregorian::date d(year,month,day);

			int hour = read_uint8(start,end);
			int minute = read_uint8(start,end);
			int second = read_uint8(start,end);
			long fractional_seconds = read_int32(start,end);
			boost::posix_time::time_duration	td(hour,minute,second,fractional_seconds);
			return boost::posix_time::ptime(d,td);
		}
	}
}

#endif // LOGGING_IO_HPP_INCLUDED
