/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef LOGGING_LAYOUT_APPENDER_H__
#define LOGGING_LAYOUT_APPENDER_H__

#include <log4boost/config.hpp>
#include <log4boost/appender/appender.hpp>
#include <log4boost/layout/layout.hpp>

#include <type_traits>

namespace log4boost
{

	/**
	 *	The interface for appender would use the layout to format the log message.
	 */
	class LOG4BOOST_EXPORT layout_appender
		:public appender
	{
	public:
		virtual ~layout_appender(void);

		/**
		 *	Set the layout object the appender used.
		 *	The old layout will be deleted.
		 */
		void set_layout (layout *thelayout=0);

		/**
		 *	Get the layout pointer and cast it to the specified type.
		 */
		template< typename T >
		T* get_layout()
		{
			BOOST_STATIC_ASSERT( (std::is_base_of<layout,T>::value) );
			return (T*)layout_;
		}

	protected:
		/**
		*	The appender constructor.
		*	@param	name			Appender name.
		*	@param	_layout			The layout object the appender should use.If give the NULL,
		*							it would use the default layout
		*/
		layout_appender(const std::string& name, layout* _layout = 0);

		layout& _get_layout()const;
	private:
		layout* layout_;
	};

}

#endif//LOGGING_LAYOUT_APPENDER_H__
