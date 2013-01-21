//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2010 by Epic Games China & Titan Studios Inc.
//------------------------------------------------------------------------------
#ifndef LOGGING_DATE_TIME_H__
#define LOGGING_DATE_TIME_H__

#include <log4boost/config.hpp>

#include <string>

namespace boost { namespace posix_time { class ptime; } }

namespace log4boost
{
	class LOG4BOOST_EXPORT date_time
	{
	public:

		date_time();

		static date_time from_ptime( const boost::posix_time::ptime& pt );
		boost::posix_time::ptime to_ptime() const;

		std::string to_iso_extended_string() const;
		int64_t milliseconds_since_epoch() const;

	private:
		int64_t rep_value;
	};
};

#endif // LOGGING_DATE_TIME_H__
