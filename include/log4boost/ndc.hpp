/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_NDC_H__
#define LOGGING_NDC_H__

#include <string>
#include <stack>
#include <vector>
#include <boost/thread/tss.hpp>
#include <boost/variant.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <log4boost/config.hpp>
#include <log4boost/detail/noncopyable.hpp>

/*! \page log_ndc Nested diagnosed context

The nested diagnosed context is thread specific. Each thread has its own stack which store the context.

Now the ndc could push multi-type context, the type list is:
- boost::int32_t
- boost::uint32_t
- boost::int64_t
- boost::uint64_t
- float
- double
- std::string


Use nested diagnosed context like this:
\code
{
	log4boost::ndc::push(15);

	// do somethings...

	log4boost::ndc::pop();
}
\endcode

A more safe way, it's safe for exception. It ensure ndc::pop would be called.
\code
{
	log4boost::scoped_ndc	sndc(10);

	// do somethings...

}
\endcode


*/


namespace log4boost
{

	/**
	 *	Nested diagnosed context.
	 */
	class LOG4BOOST_EXPORT ndc
	{
	public:
		ndc(void);
		~ndc(void);
		typedef std::string	context_type;

		static void clear();
		static size_t get_depth();
		static const context_type & get();
		static void pop();

		template<typename T>
		static void push( T _context )
		{
			get_stack().push( _context );
		}

		static void push( const context_type& _context );

		static bool empty();

	private:
		typedef std::stack<context_type,std::vector<context_type,boost::pool_allocator<context_type> > > context_stack_t;
		inline static context_stack_t& get_stack();
		static boost::thread_specific_ptr<ndc::context_stack_t>	stack_;
	};

	ndc::context_stack_t& ndc::get_stack()
	{
		static boost::thread_specific_ptr<context_stack_t> stack_;
		if(stack_.get()==0)
		{
			stack_.reset(new context_stack_t);
		}

		return *stack_;
	}

	class LOG4BOOST_EXPORT scoped_ndc 
		: detail::noncopyable
	{
	public:
		scoped_ndc( const ndc::context_type& _context )
		{
			ndc::push( _context );
		}

		~scoped_ndc( void )
		{
			ndc::pop();
		}
	};

}

#endif//LOGGING_NDC_H__
