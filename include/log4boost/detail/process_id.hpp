#ifndef LOGGING_PROCESS_ID_HPP__
#define LOGGING_PROCESS_ID_HPP__

#include <boost/config.hpp>

#if defined(WIN32) || defined(WIN64)
#	include <Windows.h>
#endif

#if defined(BOOST_HAS_UNISTD_H)
#	include <unistd.h>
#endif

namespace log4boost
{
	namespace detail
	{
		struct LOG4BOOST_EXPORT process_name
		{
			inline static const std::string& get();
		};

		const std::string& process_name::get()
		{
			static std::string	current_process_name_;

			if(current_process_name_.empty())
			{
#if defined(WIN32) || defined(WIN64)
				current_process_name_ = boost::lexical_cast<std::string>(::GetCurrentProcessId());
#elif defined(BOOST_HAS_UNISTD_H)
				current_process_name_ = boost::lexical_cast<std::string>(getpid());
#endif
			}
			return current_process_name_;
		}

	}
}

#endif//LOGGING_PROCESS_ID_HPP__

