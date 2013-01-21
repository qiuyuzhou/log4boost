/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/appender/layout_appender.hpp"
#include "log4boost/layout/formatter_layout.hpp"

namespace log4boost
{

	layout_appender::layout_appender(const std::string& name,layout* _layout)
		:appender(name)
		,layout_(_layout)
	{
		if(!layout_)
			layout_ = new formatter_layout(formatter_layout::DEFAULT_FORMAT);
	}

	layout_appender::~layout_appender(void)
	{
		delete layout_;
	}

	void layout_appender::set_layout (layout *thelayout)
	{
		if(thelayout!=layout_)
		{
			layout* old_layout = layout_;
			layout_ = (thelayout==0)?new formatter_layout():thelayout;
			delete old_layout;
		}
	}

	layout& layout_appender::_get_layout()const
	{
		return *layout_;
	}

}
