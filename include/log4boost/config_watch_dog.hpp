/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef _LOG4BOOST_CONFIG_FILE_WATCHER_HPP__
#define _LOG4BOOST_CONFIG_FILE_WATCHER_HPP__


#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>
#include <log4boost/file_watch_dog.hpp>

namespace log4boost
{

	class LOG4BOOST_EXPORT config_file_watch_dog
		: public file_watch_dog
	{
	public:
		config_file_watch_dog( const boost::filesystem::path& file_path, const std::string& preconfig_string );
		config_file_watch_dog( const boost::filesystem::path& file_path );

		virtual void on_changed();

	protected:
		std::string	preconfig_string_;
		bool do_preconfig_;
	};


	class LOG4BOOST_EXPORT config_watch_dog
		: detail::noncopyable
	{
	public:

		static config_watch_dog* instance();

		void configure_and_watch( const boost::filesystem::path& file_path, const std::string& preconfig_string );

		void configure_and_watch( const boost::filesystem::path& file_path, const std::string& preconfig_string, unsigned long delay );

		void configure_and_watch( const boost::filesystem::path& file_path );

		void configure_and_watch( const boost::filesystem::path& file_path, unsigned long delay );

		void stop_watching();

		bool is_watching()const;

	private:

		boost::scoped_ptr<config_file_watch_dog>		config_file_watch_dog_;
	};

}

#endif//_LOG4BOOST_CONFIG_FILE_WATCHER_HPP__

