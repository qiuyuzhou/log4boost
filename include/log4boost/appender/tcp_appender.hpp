#ifndef LOGGING_TCP_APPENDER_HPP__34334__
#define LOGGING_TCP_APPENDER_HPP__34334__

#include <log4boost/config.hpp>
#include <log4boost/detail/mutex.hpp>
#include <log4boost/appender/layout_appender.hpp>
#include <log4boost/layout/layout.hpp>
#include <log4boost/appender_creator_base.hpp>

#include <memory>


namespace log4boost
{

	class tcp_writer;

	/**
	 *	The appender used to output the log to a tcp server. If you want to output the log to the 
	 *	Chainsaw, please use it with log4j_xml_layout.
	 *
	 *  \par The config options:
	 *		- \b host	The ip or domain name of the tcp server.
	 *		- \b port	The port of the tcp server.
	 *	
	 *	Warning: If you use the tcp_appender, you must call log4boost::shutdown() before exit main
	 *			 thread in order to close the appender instance. Otherwise, the system would 
	 *			 terminated the background thread used by the appender.
	 */
	class LOG4BOOST_EXPORT tcp_appender
		: public layout_appender
	{
	public:
		virtual ~tcp_appender();

		virtual void reopen();

		virtual void close();

		static shared_ptr<tcp_appender>	create(	const std::string& name, layout* l, const std::string& host
			,uint16_t port);


		void set_reconnection_delay( int seconds );
		int	get_reconnection_delay()
		{
			return reconnection_delay_;
		}

		void open();

	protected:
		tcp_appender( const std::string& name, layout* l, const std::string& host, uint16_t port );

		virtual void _append( const logging_event& event );


	private:
		std::string		host_;
		uint16_t	port_;
		
		std::auto_ptr<tcp_writer>	writer_;

		int				reconnection_delay_;

		mutable	mutex	mutex_;
	};


	class LOG4BOOST_EXPORT tcp_appender_creator
		: public appender_creator_base
	{
	public:
		virtual	shared_ptr<appender> create_appender(const std::string& name, const property_tree::ptree& params);
		virtual std::string get_name()const;
	};

}



#endif//LOGGING_TCP_APPENDER_HPP__34334__

