/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/ndc.hpp"

namespace log4boost
{

	ndc::ndc(void)
	{
	}

	ndc::~ndc(void)
	{
	}

	void ndc::clear()
	{
		context_stack_t& stack = get_stack();
		while(!stack.empty())
		{
			stack.pop();
		}
	}

	size_t ndc::get_depth()
	{
		context_stack_t& stack = get_stack();
		return stack.size();
	}

	const ndc::context_type & ndc::get()
	{
		context_stack_t& stack = get_stack();
		if(!stack.empty())
		{
			return stack.top();
		}
		throw std::logic_error("ndc is empty");
	}

	void ndc::pop()
	{
		context_stack_t& stack = get_stack();
		if(!stack.empty())
		{
			stack.pop();
		}
	}

	bool ndc::empty()
	{
		return get_stack().empty();
	}

	void ndc::push( const context_type& _context )
	{
		get_stack().push( _context );
	}

}
