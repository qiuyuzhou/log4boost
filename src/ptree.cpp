/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "ptree.h"

namespace log4boost
{

	namespace property_tree
	{

		//--------------------------------------------------------------------------------

		namespace detail
		{
			template<typename T>
			struct ptr_deep_copy
			{
				T* operator()(const T* p)
				{
					return new T(*p);
				}
			};
		}

		//--------------------------------------------------------------------------------

		void ptree::clear()
		{
			for(container_type::iterator itr=m_container.begin()
				;itr!=m_container.end()
				;++itr)
			{
				delete *itr;
			}
			m_container.clear();
			m_data.clear();
		}

		ptree::ptree()
		{
		}


		ptree::ptree(const data_type& data)
		{
			m_data = data;
		}

		ptree::ptree(const ptree& rhs)
		{
			m_data = rhs.m_data;
			std::transform(
				rhs.m_container.begin()
				,rhs.m_container.end()
				,std::back_inserter(m_container)
				,detail::ptr_deep_copy<value_type>()
				);
		}


		ptree::~ptree()
		{
			clear();
		}

		//--------------------------------------------------------------------------------


		ptree::iterator
			ptree::begin()
		{
			return iterator(m_container.begin());
		}


		ptree::const_iterator
			ptree::begin() const
		{
			return const_iterator(m_container.begin());
		}


		ptree::iterator
			ptree::end()
		{
			return iterator(m_container.end());
		}


		ptree::const_iterator
			ptree::end() const
		{
			return const_iterator(m_container.end());
		}


		ptree::reverse_iterator
			ptree::rbegin()
		{
			return reverse_iterator(m_container.rbegin());
		}


		ptree::const_reverse_iterator
			ptree::rbegin() const
		{
			return const_reverse_iterator(m_container.rbegin());
		}


		ptree::reverse_iterator
			ptree::rend()
		{
			return reverse_iterator(m_container.rend());
		}


		ptree::const_reverse_iterator
			ptree::rend() const
		{
			return const_reverse_iterator(m_container.rend());
		}

		//--------------------------------------------------------------------------------


		ptree::size_type
			ptree::size() const
		{
			return m_container.size();
		}


		ptree::size_type
			ptree::max_size() const
		{
			return m_container.max_size();
		}


		bool ptree::empty() const
		{
			return m_container.empty();
		}



		ptree::data_type&
			ptree::data()
		{
			return m_data;
		}


		const ptree::data_type &
			ptree::data() const
		{
			return m_data;
		}



		ptree::value_type&
			ptree::front()
		{
			return *m_container.front();
		}


		const ptree::value_type&
			ptree::front() const
		{
			return *m_container.front();
		}


		ptree::value_type&
			ptree::back()
		{
			return *m_container.back();
		}


		const ptree::value_type&
			ptree::back() const
		{
			return *m_container.back();
		}

		//--------------------------------------------------------------------------------

		ptree & ptree::operator =(const ptree &rhs)
		{
			clear();
			m_data = rhs.m_data;
			std::transform(
				rhs.m_container.begin()
				,rhs.m_container.end()
				,std::back_inserter(m_container)
				,detail::ptr_deep_copy<value_type>()
				);
			return *this;
		}

		bool ptree::operator ==(const ptree &rhs) const
		{
			return true;
		}
		bool ptree::operator !=(const ptree &rhs) const
		{
			return true;
		}

		//--------------------------------------------------------------------------------


		ptree::iterator
			ptree::find(const key_type &key)
		{
			for(iterator itr=begin()
				;itr!=end()
				;++itr)
			{
				if(itr->first==key)
					return itr;
			}
			return end();
		}

		ptree::const_iterator
			ptree::find(const key_type &key) const
		{
			for(const_iterator itr=begin()
				;itr!=end()
				;++itr)
			{
				if(itr->first==key)
					return itr;
			}
			return end();
		}

		ptree::size_type
			ptree::count(const key_type &key) const
		{
			size_type n = 0;
			for(const_iterator itr=begin()
				;itr!=end()
				;++itr)
			{
				if(itr->first==key)
				{
					n++;
				}
			}
			return n;
		}


		ptree::iterator
			ptree::insert(iterator where, const value_type &value)
		{
			m_container.insert(where.base(),new value_type(value));
			return --where;
		}

		template<class It> void
			ptree::insert(iterator where, It first, It last)
		{
			for(It itr=first;itr!=last;++itr)
			{
				m_container.insert(where.base(),new value_type(*itr));
			}
		}


		ptree::iterator ptree::erase(iterator where)
		{
			iterator itr = where;
			std::advance(itr,1);
			delete *(where.base());
			m_container.erase(where.base());
			return itr;
		}

		ptree::size_type ptree::erase(const key_type &key)
		{
			size_type n = 0;
			iterator it = find(key);
			while(it!=end())
			{
				erase(it);
				n++;
			}
			return n;
		}

		ptree::iterator
			ptree::push_front(const value_type &value)
		{
			m_container.push_front(new value_type(value));
			return iterator(m_container.begin());
		}

		ptree::iterator
			ptree::push_back(const value_type &value)
		{
			m_container.push_back(new value_type(value));
			container_type::iterator back = m_container.end();
			--back;
			return iterator(back);
		}


		void ptree::pop_front()
		{
			delete m_container.front();
			m_container.pop_front();
		}

		void ptree::pop_back()
		{
			delete m_container.back();
			m_container.pop_back();
		}


		void ptree::swap(ptree &rhs)
		{
			m_data.swap(rhs.m_data);
			m_container.swap(rhs.m_container);
		}


		void ptree::reverse()
		{
			m_container.reverse();
		}


		//--------------------------------------------------------------------------------


		//template<class Type>
		//	Type
		//	ptree::get_value() const
		//{
		//	return boost::lexical_cast<Type>(m_data);
		//}

		//template<class Type>
		//	Type
		//	ptree::get_value(const Type &default_value) const
		//{
		//	if(m_data.empty())
		//	{
		//		return default_value;
		//	}
		//	else
		//	{
		//		return boost::lexical_cast<Type>(m_data);
		//	}
		//}

		std::basic_string<ptree::char_type>
			ptree::get_value(const char_type *default_value)const
		{
			if(m_data.empty())
			{
				return std::basic_string<char_type>(default_value);
			}
			else
			{
				return m_data;
			}
		}

		//template<class Type>
		//	boost::optional<Type>
		//		ptree::get_value_optional() const
		//{
		//	if(m_data.empty())
		//	{
		//		return boost::optional<Type>();
		//	}
		//	else
		//	{
		//		return boost::optional<Type>(boost::lexical_cast<Type>(m_data));
		//	}
		//}

		boost::optional<ptree&>
			ptree::get_child_optional(const key_type &path)
		{
			//		return get_child_optional('.',path);
			key_type::size_type n = path.find('.');
			if (n != key_type::npos)
			{
				key_type head = path.substr(0, n);
				key_type tail = path.substr(n + 1, key_type::npos);
				iterator it = find(head);
				if (it != end())
					return it->second.get_child_optional(tail);
				else
					return boost::optional<ptree&>();
			}
			else
			{
				iterator it = find(path);
				if (it != end())
					return it->second;
				else
					return boost::optional<ptree&>();
			}
		}

		boost::optional<const ptree&>
			ptree::get_child_optional(const key_type &path)const
		{
			ptree *nc_this = const_cast<ptree *>(this);
			boost::optional<ptree&> tmp = nc_this->get_child_optional(path);
			if(tmp)
				return boost::optional<const ptree&>(tmp.get());
			else
				return boost::optional<const ptree&>();
		}

		ptree& ptree::get_child(const key_type &path)
		{
			if (boost::optional<ptree&> result = get_child_optional(path))
				return result.get();
			else
				throw ptree_bad_path("key \"" + path + "\" does not exist");
		}

		const ptree& ptree::get_child(const key_type &path)const
		{
			if (boost::optional<const ptree&> result = get_child_optional(path))
				return result.get();
			else
				throw ptree_bad_path("key \"" + path + "\" does not exist");
		}

		ptree& ptree::get_child(const key_type &path,ptree &default_value)
		{
			//		return get_child('.',path,default_value);
			if (boost::optional<ptree&> result = get_child_optional(path))
				return result.get();
			else
				return default_value;
		}

		const ptree& ptree::get_child(const key_type &path,const ptree &default_value)const
		{
			if (boost::optional<const ptree&> result = get_child_optional(path))
				return result.get();
			else
				return default_value;
		}


		//template<class Type>
		//	Type ptree::get(const key_type& path)const
		//{
		//	const ptree& tmp = get_child(path);
		//	return tmp.get_value<Type>();
		//}

		//template<class Type>
		//	Type ptree::get(const key_type& path,const Type& default_value)const
		//{
		//	boost::optional<const ptree&> tmp = get_child_optional(path);
		//	if(tmp)
		//		return tmp->get_value<Type>();
		//	else
		//		return default_value;
		//}

		std::string ptree::get(const key_type& path,const char_type *default_value)const
		{
			boost::optional<const ptree&> tmp = get_child_optional(path);
			if(tmp)
				return tmp->get_value(default_value);
			else
				return default_value;
		}

		//template<class Type>
		//	boost::optional<Type> ptree::get_optional(const key_type& path)const
		//{
		//	boost::optional<const ptree&> tmp = get_child_optional(path);
		//	if(tmp)
		//		return boost::optional<Type>(tmp->get_value<Type>());
		//	else
		//		return boost::optional<Type>();
		//}


		ptree& ptree::put_child(const key_type &path, const ptree &value, bool do_not_replace)
		{
			key_type::size_type n = path.find('.');
			if (n == key_type::npos)
			{
				if (do_not_replace)
					return push_back(value_type(path, value))->second;
				else
				{
					iterator it = find(path);
					if (it == end())
						return push_back(value_type(path, value))->second;
					else
					{
						it->second = value;
						return it->second;
					}
				}
			}
			else
			{
				key_type head = path.substr(0, n);
				key_type tail = path.substr(n + 1, key_type::npos);
				iterator it = find(head);
				if (it == end())
					it = push_back(value_type(head, ptree()));
				return it->second.put_child(tail, value, do_not_replace);
			}
		}


		//template<class Type>
		//	ptree& ptree::put(const key_type &path,const Type &value,bool do_not_replace)
		//{
		//	boost::optional<ptree&> child;
		//	if (!do_not_replace)
		//	{
		//		if(child = get_child_optional(path))
		//		{
		//			child.get().put_value(value);
		//			return *child;
		//		}
		//	}

		//	{
		//		ptree& child2
		//			= put_child(path,ptree(), do_not_replace);
		//		child2.put_value(value);
		//		return child2;
		//	}
		//}


		//template<class Type>
		//	void ptree::put_value(const Type &value)
		//{
		//	clear();
		//	m_data = boost::lexical_cast<std::string>(value);
		//}
	}

}
