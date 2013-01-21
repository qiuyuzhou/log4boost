/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef PROPERTY_TREE_H__
#define PROPERTY_TREE_H__

#include <log4boost/config.hpp>

#include <string>
#include <list>
#include <utility>
#include <iterator>
#include <boost/optional.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/lexical_cast.hpp>

namespace log4boost
{
	namespace property_tree
	{

		class LOG4BOOST_EXPORT ptree
		{
		public:
			typedef char						char_type;
			typedef std::string					key_type;
			typedef std::string					data_type;
			typedef std::pair<std::string,ptree>	value_type;

		private:
			typedef std::list<value_type* > container_type;

		public:
			typedef boost::indirect_iterator<container_type::iterator>
				iterator;
			typedef boost::indirect_iterator<container_type::const_iterator>
				const_iterator;
			typedef boost::indirect_iterator<container_type::reverse_iterator>
				reverse_iterator;
			typedef boost::indirect_iterator<container_type::const_reverse_iterator>
				const_reverse_iterator;
			typedef container_type::size_type
				size_type;

		public:
			ptree();
			explicit ptree(const data_type& data);
			ptree(const ptree& rhs);
			~ptree();

			///////////////////////////////////////////////////////////////////////////
			// Iterator access

			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;
			reverse_iterator rbegin();
			const_reverse_iterator rbegin() const;
			reverse_iterator rend();
			const_reverse_iterator rend() const;

			///////////////////////////////////////////////////////////////////////////
			// Data access

			size_type size() const;
			size_type max_size() const;
			bool empty() const;

			data_type &data();
			const data_type &data() const;

			value_type &front();
			const value_type &front() const;
			value_type &back();
			const value_type &back() const;

			///////////////////////////////////////////////////////////////////////////
			// Operators

			ptree &operator =(const ptree &rhs);

			bool operator ==(const ptree &rhs) const;
			bool operator !=(const ptree &rhs) const;

			///////////////////////////////////////////////////////////////////////////
			// Container operations

			iterator find(const key_type &key);
			const_iterator find(const key_type &key) const;

			size_type count(const key_type &key) const;


			iterator insert(iterator where, const value_type &value);
			template<class It> void insert(iterator where, It first, It last);

			iterator erase(iterator where);
			size_type erase(const key_type &key);
			template<class It> iterator erase(It first, It last);

			iterator push_front(const value_type &value);
			iterator push_back(const value_type &value);

			void pop_front();
			void pop_back();

			void clear();

			void swap(ptree &rhs);

			void reverse();

			///////////////////////////////////////////////////////////////////////////
			// ptree operations

			// Get child ptree with default separator
			boost::optional<ptree&>
				get_child_optional(const key_type &path);
			boost::optional<const ptree&>
				get_child_optional(const key_type &path)const;
			ptree& get_child(const key_type &path);
			const ptree& get_child(const key_type &path)const;
			ptree& get_child(const key_type &path,ptree &default_value);
			const ptree& get_child(const key_type &path,const ptree &default_value)const;


			// Get value from data of ptree
			template<class Type>
			Type get_value() const;
			template<class Type>
			Type get_value(const Type &default_value) const;
			std::string
				get_value(const char_type *default_value)const;
			template<class Type>
			boost::optional<Type> get_value_optional() const;

			// Get value from data of child ptree (default path separator)
			template<class Type>
			Type get(const key_type& path)const;
			template<class Type>
			Type get(const key_type& path,const Type& default_value)const;
			std::string get(const key_type& path,const char_type *default_value)const;
			template<class Type>
			boost::optional<Type> get_optional(const key_type& path)const;

			// Put value in data of ptree
			template<class Type>
			void put_value(const Type &value);

			// Put child ptree with default separator
			ptree& put_child(const key_type &path,const ptree &value,bool do_not_replace = false);

			// Put value in data of child ptree (default path separator)
			template<class Type>
			ptree &put(const key_type &path,const Type &value,bool do_not_replace = false);

		private:

			data_type m_data;
			container_type m_container;
		};





		// Base error class
		class ptree_error: public std::runtime_error
		{
		public:
			ptree_error(const std::string &what):std::runtime_error(what){}
		};

		// Bad data
		class ptree_bad_data: public ptree_error
		{
		public:

			ptree_bad_data(const std::string &what):ptree_error(what){}
		};

		// Bad path
		class ptree_bad_path: public ptree_error
		{
		public:

			ptree_bad_path(const std::string &what):ptree_error(what){}
		};




		//--------------------------------------------------------------------------------


		template<class It>
		ptree::iterator ptree::erase(It first, It last)
		{
			for(It itr=first;itr!=last;++itr)
			{
				delete *(itr.base());
				m_container.erase(itr.base());
			}
		}

		//--------------------------------------------------------------------------------


		template<class Type>
		Type
			ptree::get_value() const
		{
			return boost::lexical_cast<Type>(m_data);
		}

		template<class Type>
		Type
			ptree::get_value(const Type &default_value) const
		{
			if(m_data.empty())
			{
				return default_value;
			}
			else
			{
				return boost::lexical_cast<Type>(m_data);
			}
		}


		template<class Type>
		boost::optional<Type>
			ptree::get_value_optional() const
		{
			if(m_data.empty())
			{
				return boost::optional<Type>();
			}
			else
			{
				return boost::optional<Type>(boost::lexical_cast<Type>(m_data));
			}
		}

		template<class Type>
		Type ptree::get(const key_type& path)const
		{
			const ptree& tmp = get_child(path);
			return tmp.get_value<Type>();
		}

		template<class Type>
		Type ptree::get(const key_type& path,const Type& default_value)const
		{
			boost::optional<const ptree&> tmp = get_child_optional(path);
			if(tmp)
				return tmp->get_value<Type>();
			else
				return default_value;
		}

		template<class Type>
		boost::optional<Type> ptree::get_optional(const key_type& path)const
		{
			boost::optional<const ptree&> tmp = get_child_optional(path);
			if(tmp)
				return boost::optional<Type>(tmp->get_value<Type>());
			else
				return boost::optional<Type>();
		}


		template<class Type>
		ptree& ptree::put(const key_type &path,const Type &value,bool do_not_replace)
		{
			if (!do_not_replace)
			{
				boost::optional<ptree&> child;
				if(child = get_child_optional(path))
				{
					child.get().put_value(value);
					return *child;
				}
			}

			{
				ptree& child2
					= put_child(path,ptree(), do_not_replace);
				child2.put_value(value);
				return child2;
			}
		}


		template<class Type>
		void ptree::put_value(const Type &value)
		{
			clear();
			m_data = boost::lexical_cast<std::string>(value);
		}
	}
}
#endif//PROPERTY_TREE_H__

