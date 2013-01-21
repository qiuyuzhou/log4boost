/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"

#include "log4boost/receiver/tcp_receiver.hpp"

#include "log4boost/log4boost.hpp"
#include "io.hpp"

#include <boost/asio.hpp>

namespace log4boost
{

	class tcp_receiver_connection;

	class tcp_receiver_connection_manager
		: private boost::noncopyable
	{
	public:
		typedef boost::shared_ptr<tcp_receiver_connection>	connection_ptr;

		/// Add the specified connection to the manager and start it.
		void start(connection_ptr c);

		/// Stop the specified connection.
		void stop(connection_ptr c);

		/// Stop all connections.
		void stop_all();

		void finish_stop( connection_ptr c );

	private:
		/// The managed connections.
		std::set<connection_ptr> connections_;
		std::set<connection_ptr> connections_stop_pending_;
	};

	class tcp_receiver_connection
		: public boost::enable_shared_from_this<tcp_receiver_connection>
	{
	public:
		/// Construct a connection with the given io_service.
		explicit tcp_receiver_connection(boost::asio::io_service& io_service,
			tcp_receiver_connection_manager& manager, boost::function<void( logging_event& )> handler );

		/// Get the socket associated with the connection.
		boost::asio::ip::tcp::socket& socket()
		{
			return socket_;
		}

		/// Start the first asynchronous operation for the connection.
		void start();

		/// Stop all asynchronous operations associated with the connection.
		void stop();

	private:
		enum {
			MAX_PACKET_SIZE = 1024*1024
		};

		void start_read_head();

		void handle_read_head(const boost::system::error_code& e,
			std::size_t bytes_transferred);
		/// Handle completion of a read operation.
		void handle_read(const boost::system::error_code& e,
			std::size_t bytes_transferred);

		/// Handle completion of a write operation.
		void handle_write(const boost::system::error_code& e);

		void handle_heart_beat_timer( const boost::system::error_code& e );
		void handle_heart_beat_write( const boost::system::error_code& e );

	private:
		boost::asio::ip::tcp::socket	socket_;
		tcp_receiver_connection_manager&	manager_;

		/// Buffer for incoming data.

		boost::array<char, 4> head_buffer_;
		size_t packet_size_;
		std::string				packet_buffer_;

		boost::function<void( logging_event& )>	event_handler_;

		boost::array<char, 4> heart_beat_packet_;
		boost::asio::deadline_timer	heart_beat_timer_;

		bool					closed_;
	};

	tcp_receiver_connection::tcp_receiver_connection( 
		boost::asio::io_service& io_service
		, tcp_receiver_connection_manager& manager
		, boost::function<void( logging_event& )> handler )
		: socket_( io_service )
		, manager_( manager )
		, event_handler_( handler )
		, heart_beat_timer_( io_service )
		, closed_( true )
	{
		std::fill( heart_beat_packet_.begin(), heart_beat_packet_.end(), 0 );
	}

	void tcp_receiver_connection::start()
	{
		LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::start");

		closed_ = true;
		start_read_head();

		heart_beat_timer_.expires_from_now( boost::posix_time::seconds(1) );
		heart_beat_timer_.async_wait( 
			boost::bind( &tcp_receiver_connection::handle_heart_beat_timer, this, boost::asio::placeholders::error ) 
			);
	}

	void tcp_receiver_connection::stop()
	{
		LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::stop");
		closed_ = false;
		boost::system::error_code	ec;
		heart_beat_timer_.cancel(ec);
		socket_.close(ec);
	}

	void tcp_receiver_connection::handle_read_head( const boost::system::error_code& e, std::size_t bytes_transferred )
	{
		if ( !e )
		{
			char* p = head_buffer_.c_array();
			packet_size_ = io::read_uint32( p );

			LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::handle_read_head - OK, packet size:"<< packet_size_);


			if ( packet_size_ > MAX_PACKET_SIZE )
			{
				LOG_WARN("loglog.tcp_receiver", "packet_size_ > MAX_PACKET_SIZE");
				manager_.stop( shared_from_this() );
				socket_.get_io_service().post( boost::bind( &tcp_receiver_connection_manager::finish_stop, &manager_, shared_from_this() ) );
				return;
			}
			packet_buffer_.clear();
			packet_buffer_.resize( packet_size_ );
			
			boost::asio::async_read( socket(), boost::asio::buffer( (char*)packet_buffer_.data(), packet_buffer_.size() )
				, boost::bind( &tcp_receiver_connection::handle_read, this,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
				);
		}
		else if ( e != boost::asio::error::operation_aborted )
		{
			LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::handle_read_head - operation_aborted");

			manager_.stop( shared_from_this() );
			socket_.get_io_service().post( boost::bind( &tcp_receiver_connection_manager::finish_stop, &manager_, shared_from_this() ) );
		}
		else
		{
			LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::handle_read_head - ohter error: " << e.message() );

			socket_.get_io_service().post( boost::bind( &tcp_receiver_connection_manager::finish_stop, &manager_, shared_from_this() ) );
		}
	}

	void tcp_receiver_connection::handle_read( const boost::system::error_code& e, std::size_t bytes_transferred )
	{
		if ( !e )
		{
			LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::handle_read - OK");

			if ( bytes_transferred != packet_buffer_.size() )
			{
				LOG_WARN("loglog.tcp_receiver", "bytes_transferred != packet_buffer_.size()");
				manager_.stop( shared_from_this() );
				socket_.get_io_service().post( boost::bind( &tcp_receiver_connection_manager::finish_stop, &manager_, shared_from_this() ) );
				return;
			}

			try
			{
				logging_event	le;
				load( le,packet_buffer_.data(), packet_buffer_.size() );

				event_handler_( le );
			}
			catch( const std::exception& e )
			{
				LOG_WARN("loglog.tcp_receiver", "exception when load event, " << e.what() );
				manager_.stop( shared_from_this() );
				socket_.get_io_service().post( boost::bind( &tcp_receiver_connection_manager::finish_stop, &manager_, shared_from_this() ) );
				return;
			}

			start_read_head();
		}
		else if ( e != boost::asio::error::operation_aborted )
		{
			LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::handle_read - " << e.message() );

			manager_.stop( shared_from_this() );
			socket_.get_io_service().post( boost::bind( &tcp_receiver_connection_manager::finish_stop, &manager_, shared_from_this() ) );
		}
		else
		{
			LOG_DEBUG("loglog.tcp_receiver","tcp_receiver_connection::handle_read - operation_aborted");

			manager_.stop( shared_from_this() );
			socket_.get_io_service().post( boost::bind( &tcp_receiver_connection_manager::finish_stop, &manager_, shared_from_this() ) );
		}
	}

	void tcp_receiver_connection::start_read_head()
	{
		boost::asio::async_read( socket(), boost::asio::buffer( head_buffer_ )
			, boost::bind( &tcp_receiver_connection::handle_read_head, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) 
			);
	}

	void tcp_receiver_connection::handle_heart_beat_timer( const boost::system::error_code& e )
	{
		if ( !e )
		{
			boost::asio::async_write( socket(), boost::asio::buffer(heart_beat_packet_)
				, boost::bind( &tcp_receiver_connection::handle_heart_beat_write, this, boost::asio::placeholders::error )
				);
		}
		else if ( e != boost::asio::error::operation_aborted )
		{
			if ( !closed_ )
			{
				heart_beat_timer_.expires_from_now( boost::posix_time::seconds(1) );
				heart_beat_timer_.async_wait( 
					boost::bind( &tcp_receiver_connection::handle_heart_beat_timer, this, boost::asio::placeholders::error ) 
					);
			}
		}
	}

	void tcp_receiver_connection::handle_heart_beat_write( const boost::system::error_code& e )
	{
		if ( !e )
		{
			heart_beat_timer_.expires_from_now( boost::posix_time::seconds(1) );
			heart_beat_timer_.async_wait( 
				boost::bind( &tcp_receiver_connection::handle_heart_beat_timer, this, boost::asio::placeholders::error ) 
				);
		}
		else if ( e != boost::asio::error::operation_aborted )
		{
		}
	}

	void tcp_receiver_connection_manager::start(connection_ptr c)
	{
		LOG_INFO("loglog.tcp_receiver","tcp_receiver_connection_manager::start");
		connections_.insert(c);
		c->start();
	}

	void tcp_receiver_connection_manager::stop(connection_ptr c)
	{
		LOG_INFO("loglog.tcp_receiver","tcp_receiver_connection_manager::stop");
		c->stop();
		connections_stop_pending_.insert( c );
		connections_.erase( c );
	}

	void tcp_receiver_connection_manager::stop_all()
	{
		LOG_INFO("loglog.tcp_receiver","tcp_receiver_connection_manager::stop_all");

		std::for_each(connections_.begin(), connections_.end(),
			boost::bind(&tcp_receiver_connection::stop, _1));
        for ( std::set<connection_ptr>::const_iterator itr = connections_.begin();
                itr != connections_.end(); ++itr )
        {
            connections_stop_pending_.insert(*itr);
        }
		connections_.clear();
	}

	void tcp_receiver_connection_manager::finish_stop( connection_ptr c )
	{
		LOG_INFO("loglog.tcp_receiver","tcp_receiver_connection_manager::finish_stop");
		connections_stop_pending_.erase( c );
	}

	//----------------------------------------------------------------------------------------------------

	class tcp_receiver_impl
	{
	public:
		tcp_receiver_impl( tcp_receiver& _receiver, boost::asio::io_service* io_service_p );

		void open( const std::string& host, uint16_t port );
		void close();

	protected:
		boost::asio::io_service& get_io_service()
		{
			if ( p_external_io_service_ )
			{
				return *p_external_io_service_;
			}
			else
			{
				return *io_service_;
			}
		}
		void svc();

		/// Handle completion of an asynchronous accept operation.
		void handle_accept(const boost::system::error_code& e);


	private:

		tcp_receiver&	receiver_;

		boost::scoped_ptr<boost::asio::io_service>			io_service_;
		boost::asio::io_service*		p_external_io_service_;

		boost::scoped_ptr<boost::asio::ip::tcp::acceptor>	acceptor_;


		boost::scoped_ptr<boost::thread>	thread_;

		typedef boost::shared_ptr<tcp_receiver_connection>	connection_ptr;
		connection_ptr new_connection_;

		tcp_receiver_connection_manager		connection_manager_;

		bool		closed_;
	};


	tcp_receiver_impl::tcp_receiver_impl( tcp_receiver& _receiver, boost::asio::io_service* io_service_p )
		: receiver_( _receiver )
		, p_external_io_service_( io_service_p )
		, closed_(true)
	{
		if ( !io_service_p )
		{
			io_service_.reset( new boost::asio::io_service() );
		}
		acceptor_.reset( new boost::asio::ip::tcp::acceptor( get_io_service() ) );
	}

	void tcp_receiver_impl::open( const std::string& host, uint16_t port )
	{
		LOG_DEBUG( "loglog.tcp_receiver", "tcp_receiver_impl::open - host: " << host << " port:" << port );

		boost::asio::ip::tcp::endpoint	listen_endpoint;
		if (host.empty())
		{
			listen_endpoint = boost::asio::ip::tcp::endpoint( boost::asio::ip::address_v4::any(), port );
		}
		else
		{
			boost::asio::ip::tcp::resolver	resolver(get_io_service());
			boost::asio::ip::tcp::resolver::query	query( host, boost::lexical_cast<std::string>(port) );
			boost::asio::ip::tcp::resolver::iterator itr = resolver.resolve(query);
			boost::asio::ip::tcp::resolver::iterator	end_itr;

			if (itr!=end_itr)
			{
				listen_endpoint = *itr;
			}
			else
			{
				throw std::runtime_error( std::string("Can't resolve the host: ") + host );
			}
		}
		acceptor_->open( listen_endpoint.protocol() );
		acceptor_->set_option( boost::asio::ip::tcp::acceptor::reuse_address(true) );
		acceptor_->bind( listen_endpoint );
		acceptor_->listen();

		new_connection_.reset( new tcp_receiver_connection( get_io_service(), connection_manager_ 
			, boost::bind( boost::ref( receiver_.signal_logging_event() ), _1 )
			) );
		acceptor_->async_accept( new_connection_->socket(),
			boost::bind(&tcp_receiver_impl::handle_accept, this,
			boost::asio::placeholders::error));

		if ( io_service_ )
		{
			LOG_DEBUG( "loglog.tcp_receiver", "tcp_receiver_impl::open - launch the thread running the io_service" );
			thread_.reset( new boost::thread( boost::bind(&tcp_receiver_impl::svc,this) ) );
		}
		closed_ = false;
	}

	void tcp_receiver_impl::close()
	{
		LOG_DEBUG( "loglog.tcp_receiver", "tcp_receiver_impl::close");

		if ( thread_ )
		{
			closed_ = true;
			get_io_service().post( boost::bind( &boost::asio::ip::tcp::acceptor::close, acceptor_.get() ) );
			get_io_service().post( boost::bind( &tcp_receiver_connection_manager::stop_all, &connection_manager_ ) );
			thread_->join();
			thread_.reset();
		}
		else
		{
			closed_ = true;
			acceptor_->close();
			connection_manager_.stop_all();
		}
	}

	void tcp_receiver_impl::svc()
	{
		LOG_DEBUG( "loglog.tcp_receiver", "tcp_receiver_impl::svc() - Begin");
		get_io_service().run();
		LOG_DEBUG( "loglog.tcp_receiver", "tcp_receiver_impl::svc() - End");
	}

	void tcp_receiver_impl::handle_accept( const boost::system::error_code& e )
	{
		if (!e)
		{
			LOG_INFO( "loglog.tcp_receiver", "tcp_receiver_impl::handle_accept - new connection accepted");

			connection_manager_.start(new_connection_);
			new_connection_.reset(new tcp_receiver_connection( get_io_service()
				, connection_manager_
				, boost::bind( boost::ref( receiver_.signal_logging_event() ), _1 )
				) );
			acceptor_->async_accept(new_connection_->socket(),
				boost::bind(&tcp_receiver_impl::handle_accept, this,
				boost::asio::placeholders::error));
		}
		else if ( e != boost::asio::error::operation_aborted )
		{
			LOG_WARN( "loglog.tcp_receiver", "tcp_receiver_impl::handle_accept - failed! " << e.value() << e.message() << "  Try again" );
			acceptor_->async_accept(new_connection_->socket(),
				boost::bind(&tcp_receiver_impl::handle_accept, this,
				boost::asio::placeholders::error));
		}
	}
	//----------------------------------------------------------------------------------------------------


	tcp_receiver::tcp_receiver( const std::string& name )
		: receiver( name )
	{

	}

	void tcp_receiver::open( const std::string& host, uint16_t port, boost::asio::io_service* io_service_p /*= 0 */ )
	{
		mutex::scoped_lock	lock(mutex_);

		impl_.reset( new tcp_receiver_impl( *this, io_service_p ) );
		impl_->open( host, port );
	}

	void tcp_receiver::close()
	{
		mutex::scoped_lock	lock(mutex_);
		if (impl_)
		{
			impl_->close();
			impl_.reset();
		}
	}

	shared_ptr<tcp_receiver> tcp_receiver::create( const std::string& name )
	{
		shared_ptr<tcp_receiver>	p( new tcp_receiver( name ) );

		receiver::add_receiver( p );

		return p;
	}



}

