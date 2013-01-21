#include "stdafx.h"

#include "log4boost/appender/tcp_appender.hpp"
#include "log4boost/detail/stack_ostream.hpp"
#include "log4boost/layout/log4j_xml_layout.hpp"
#include "log4boost/layout_factory.hpp"
#include "ptree_inc.h"

#include <boost/iostreams/stream.hpp>

#include <log4boost/writer/tcp_writer.hpp>


namespace log4boost
{



	//----------------------------------------------------------------------------------------------------

	tcp_appender::tcp_appender( const std::string& name, layout* l, const std::string& host, uint16_t port  )
		: layout_appender( name, l )
		, host_( host )
		, port_( port )
		, reconnection_delay_( 10 )
	{
	}

	tcp_appender::~tcp_appender()
	{
		close();
	}

	void tcp_appender::reopen()
	{

	}

	void tcp_appender::close()
	{
		mutex::scoped_lock	lock(mutex_);
		if ( writer_.get() )
		{
			writer_->close();
			writer_.reset();
		}
	}

	void tcp_appender::_append( const logging_event& event )
	{
		mutex::scoped_lock	lock(mutex_);
		if ( writer_.get() )
		{
			detail::basic_stack_ostream<char,LOGGING_MAX_MESSAGE_SIZE*2>	out;

			_get_layout().format( event, out.get_out() );

			writer_->write( out.str(), out.length() );
		}
	}

	void tcp_appender::set_reconnection_delay( int seconds )
	{
		mutex::scoped_lock	lock(mutex_);
		reconnection_delay_ = seconds;

		if ( writer_.get() )
		{
			writer_->set_reconnection_delay( reconnection_delay_ );
		}
	}

	shared_ptr<tcp_appender> tcp_appender::create( const std::string& name, layout* l, const std::string& host ,uint16_t port )
	{
		shared_ptr<tcp_appender> p( new tcp_appender( name, l, host, port ) );
		appender::add_appender( p );
		return p;
	}

	void tcp_appender::open()
	{
		mutex::scoped_lock	lock(mutex_);
		if ( writer_.get() == 0 )
		{
			writer_.reset( new tcp_writer( reconnection_delay_ ) );
			writer_->open( host_, port_ );
		}
		else
		{
			throw std::logic_error("The tcp appender has been opened.");
		}
	}


	shared_ptr<appender> tcp_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		std::string host;

		host = params.get<std::string>("host","");
		if(host.empty())
			host = params.get<std::string>("ip","127.0.0.1");
		int port = params.get<int>("port",4448);

		if(port<=0||port>0xffff)
			throw std::range_error("TCP port out of range.");

		std::auto_ptr<layout> l = layout_factory::get_instance().create(params.get_child("layout"));

		if(appender::get_appender(name))
			throw std::invalid_argument(std::string("Appender name \"")+name+"\" be existed.");

		shared_ptr<tcp_appender> pAppender = tcp_appender::create(name,l.release(),host,port);
		pAppender->open();
		return pAppender;
	}

	std::string tcp_appender_creator::get_name() const
	{
		return "tcp_appender";
	}
}

