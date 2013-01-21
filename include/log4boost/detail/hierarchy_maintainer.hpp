/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef LOGGING_HIERARCHY_MAINTAINER_H__
#define LOGGING_HIERARCHY_MAINTAINER_H__

#include <string>
#include <map>
#include <log4boost/detail/mutex.hpp>

#include <log4boost/config.hpp>
#include <log4boost/detail/shared_ptr.hpp>


namespace log4boost
{
	class logger;

	namespace detail
	{

		/**
		*	The class maintain the hierarchy of the logger.
		*/
		class LOG4BOOST_EXPORT hierarchy_maintainer
		{
		public:
			hierarchy_maintainer(void);
			virtual ~hierarchy_maintainer(void);

            static bool has_instance();
			static hierarchy_maintainer* instance();

			logger&	get_root();
			logger& get_logger (const char* name, size_t name_count);
			void 	remove_all();
			void remove_all_appenders();

			void set_name( const std::string& name );

			const std::string& get_name()const;

		protected:
			logger* 	_get_instance (const char* name, size_t name_count);

		private:
			typedef size_t name_hash_t;
			typedef std::multimap<name_hash_t,shared_ptr<logger> > logger_map_t;
			typedef std::pair<logger_map_t::const_iterator, logger_map_t::const_iterator> logger_map_range_t;
			logger_map_t			logger_map_;
			mutable mutex			logger_map_mutex_;

			std::string				instance_name_;
		};


	}
}

#endif//LOGGING_HIERARCHY_MAINTAINER_H__
