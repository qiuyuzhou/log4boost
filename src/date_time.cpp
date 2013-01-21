//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2010 by Epic Games China & Titan Studios Inc.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "log4boost/date_time.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

namespace log4boost
{
	date_time::date_time()
	{
		const boost::posix_time::ptime pt;
		rep_value = boost::posix_time::posix_time_system::get_time_rep( pt.date(), pt.time_of_day() ).get_rep().as_number();
	}

	date_time date_time::from_ptime( const boost::posix_time::ptime& pt )
	{
		date_time dt;
		dt.rep_value = boost::posix_time::posix_time_system::get_time_rep( pt.date(), pt.time_of_day() ).get_rep().as_number();
		return dt;
	}

	boost::posix_time::ptime date_time::to_ptime() const
	{
		return boost::posix_time::ptime( boost::posix_time::ptime::time_rep_type(rep_value) );
	}

	std::string date_time::to_iso_extended_string() const
	{
		return boost::posix_time::to_iso_extended_string( to_ptime() );
	}

	int64_t date_time::milliseconds_since_epoch() const
	{
		static boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970,1,1));
		boost::posix_time::time_duration td = (to_ptime() - time_t_epoch);
		return td.total_milliseconds();
	}
}
