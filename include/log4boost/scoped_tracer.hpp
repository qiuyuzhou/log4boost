#ifndef ___LOGGING_SCPED_TRACER_HPP___
#define ___LOGGING_SCPED_TRACER_HPP___

#include <log4boost/logger.hpp>
#include <log4boost/detail/stack_ostream.hpp>
#include <log4boost/detail/noncopyable.hpp>

namespace log4boost
{

	template< typename charT >
	class basic_scoped_tracer : detail::noncopyable
	{
	public:
		typedef charT	char_type;
		basic_scoped_tracer( logger& l )
		{
			begin_ostream_.get_out() << "Trace begin - ";
			end_ostream_.get_out() << "Trace end - ";
		}

		~basic_scoped_tracer()
		{
			logger_.log( priority::PL_TRACE, end_ostream_.str(), end_ostream_.length() );
		}

		template<typename T>
		inline basic_scoped_tracer& operator << ( const T& val )
		{
			begin_ostream_.get_out() << val;
			end_ostream_.get_out() << val;
			return *this;
		}

		inline void log( void )
		{
			logger_.log( priority::PL_TRACE, begin_ostream_.str(), begin_ostream_.length() );
		}

	private:
		logger&	logger_;
		detail::basic_stack_ostream<char_type>	begin_ostream_;
		detail::basic_stack_ostream<char_type>	end_ostream_;
	};

	typedef basic_scoped_tracer<char>		stack_ostream;
	typedef basic_scoped_tracer<wchar_t>	wstack_ostream;

}

#endif //___LOGGING_SCPED_TRACER_HPP___
