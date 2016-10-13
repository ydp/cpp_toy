#ifndef __JJLIST__
#define __JJLIST__

#include <iostream>
#include <cstddef>
#include <cassert>


namespace JJ
{

template <class T>
struct list_node
{
	typedef list_node<T>* pointer;
	pointer prev;
	pointer next;
	T data;
};

template <class T>
struct list_iterator
{
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef list_iterator iterator;

	list_iterator(list_node<T> *p = NULL) : node(p) {}

	reference operator*() { return node->data; }
	iterator& operator++() 
	{
		node = node->next;
		return *this;
	}
	iterator& operator++(int)
	{
		iterator tmp = *this;
		++*this;
		return tmp;
	}
	iterator& operator--()
	{
		node = node->prev;
		return *this;
	}
	iterator& operator--(int)
	{
		iterator tmp = *this;
		--*this;
		return tmp;
	}
	bool operator==(const iterator &rhs)
	{
		return node == rhs.node;
	}
	bool operator!=(const iterator &rhs)
	{
		return node != rhs.node;
	}

	list_node<T> *node;
};

template <class T>
class JJList
{
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef list_iterator<T> iterator;
	typedef const list_iterator<T> const_iterator;
	typedef T& reference;
	typedef const T& const_reference;
	typedef list_node<T>* link_type;

	JJList();
	JJList(size_type n, T value = T());
	// template class< T>
	// JJList(InputIterator first, InputIterator last);
	
	JJList(const JJList&);
	JJList& operator=(const JJList&);
	~JJList();

	bool empty() const { return node->next == node; }
	difference_type size() const;

	iterator begin() { return iterator(node->next); }
	iterator end() { return iterator(node); }
	iterator begin() const { return iterator(node->next); }
	iterator end() const { return iterator(node); }
	iterator rbegin() { return iterator(node->prev); } 
	iterator rbegin() const { return iterator(node->prev); }

	iterator insert(iterator position, const T& x);

	void swap(JJList&);
	void push_back(const T& x);
	void pop_back()
	{
		iterator tmp = end();
		erase(--tmp);
	}

	void push_front(const T& x);
	void pop_front()
	{
		erase(begin());
	}

	void reverse();
	void merge(JJList &);
	void sort();

	void remove(const T& x);
	iterator erase(iterator position);
	void clear();

	/// TODO:
	void splice(iterator position, JJList & x)
	{
		if(!x.empty())
		{
			transfer(position, x.begin(), x.end());
		}
	}
	
	void splice(iterator position, JJList&, iterator i)
	{
		iterator j = i;
		++j;
		if (position == i || position == j) 
		{
			return;
		}
		transfer(position, i, j);
	}

	void splice(iterator position, JJList&, iterator first, iterator last)
	{
		if (first != last)
		{
			transfer(position, first, last);
		}
	}

	void print();

protected:
	list_node<T> *node;
	static std::allocator<list_node<T> > alloc;

	link_type get_node()
	{
		link_type p = alloc.allocate(1);
		return p;
	}
	void put_node(link_type p)
	{
		alloc.deallocate(p, 1);
	}
	link_type create_node(const T& x)
	{
		link_type p = get_node();
		new(&p->data) T(x);
		return p;
	}
	void destroy_node(link_type p)
	{
		(&p->data)->~T();
		put_node(p);
	}
	void empty_initialize()
	{
		node = get_node();
		node->prev = node;
		node->next = node;
	}

	void transfer(iterator position, iterator first, iterator last);
};


template <class T>
std::allocator<list_node<T> > JJList<T>::alloc;

template <class T>
JJList<T>::JJList()
{
	empty_initialize();
}

template <class T>
JJList<T>::JJList(size_type n, T value)
{
	empty_initialize();
	for (size_type i(0); i != n; ++i)
	{
		insert(begin(), value);
	}
}

template <class T>
void JJList<T>::clear()
{
	link_type p = node->next;
	link_type next(NULL);
	while (p != node)
	{
		next = p->next;
		destroy_node(p);
		p = next;
	}
	node->prev = node;
	node->next = node;
}

template <class T>
JJList<T>::~JJList()
{
	clear();
	destroy_node(node);
	node = NULL;
}

template <class T>
JJList<T>::JJList(const JJList& rhs)
{
	empty_initialize();
	for (iterator it = rhs.begin(); it != rhs.end(); ++it)
	{
		insert(end(), *it);
	}
}

template <class T>
JJList<T>& JJList<T>::operator=(const JJList& rhs)
{
	clear();
	for (iterator it = rhs.begin(); it != rhs.end(); ++it)
	{
		insert(end(), *it);
	}
	return *this;
}

template <class T>
typename JJList<T>::difference_type JJList<T>::size() const
{
	difference_type len(0);
	link_type p = node->next;
	for (; p != node; p = p->next, ++len) ;
	return len;
}

template <class T>
typename JJList<T>::iterator JJList<T>::insert(iterator position, const T& x)
{
	link_type new_node = create_node(x);

	new_node->next = position.node;
	new_node->prev = position.node->prev;
	position.node->prev->next = new_node;
	position.node->prev = new_node;

	return iterator(new_node);
}

template <class T>
inline void JJList<T>::push_back(const T& x)
{
	insert(end(), x);
}

template <class T>
inline void JJList<T>::push_front(const T& x)
{
	insert(begin(), x);
}

template <class T>
typename JJList<T>::iterator JJList<T>::erase(iterator position)
{
	link_type next_node = link_type(position.node->next);
	link_type prev_node = link_type(position.node->prev);

	prev_node->next = next_node;
	next_node->prev = prev_node;
	destroy_node(position.node);

	return iterator(next_node);
}

template <class T>
void JJList<T>::remove(const T& x)
{
	link_type prev_node(node);
	link_type cur(node->next);
	link_type next_node(NULL);

	while(cur != node)
	{
		if (cur->data == x)
		{
			next_node = cur->next;
			prev_node->next = next_node;
			next_node->prev = prev_node;

			destroy_node(cur);
			cur = next_node;
		}else
		{
			prev_node = cur;
			cur = cur->next;
		}
	}
}

template <class T>
void JJList<T>::transfer(iterator position, iterator first, iterator last)
{
	if (position != last)
	{
		link_type first_node = first.node;
		link_type rear_node = last.node->prev;

		first_node->prev->next = last.node;
		last.node->prev = first_node->prev;

		first_node->prev = position.node->prev;
		rear_node->next = position.node;
		position.node->prev->next = first_node;
		position.node->prev = rear_node;
	}
}

template <class T>
void JJList<T>::swap(JJList &rhs)
{
	if (this->node != rhs.node)
	{
		link_type tmp = node;
		node = rhs.node;
		rhs.node = tmp;
	}
}

template <class T>
void JJList<T>::reverse()
{
	if (node->next == node || node->next->next == node)
	{
		return;
	}
	iterator first = begin(), old(NULL);
	++first;
	while (first != end())
	{
		old = first;
		++first;
		transfer(begin(), old, first);
	}
}

template <class T>
void JJList<T>::merge(JJList &x)
{
	iterator first1 = this->begin();
	iterator last1 = this->end();
	iterator first2 = x.begin();
	iterator last2 = x.end();

	while(first1 != last1 && first2 != last2)
	{
		if (*first2 < *first1)
		{
			iterator next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}else
		{
			++first1;
		}
	}
	if (first2 != last2)
	{
		transfer(end(), first2, last2);
	}
	assert(x.empty());
}

template <class T>
void JJList<T>::print()
{
	iterator cur = begin();
	while (cur != end())
	{
		std::cout << *cur << " ";
		++cur;
	}
	std::cout << std::endl;
}

template <class T>
void JJList<T>::sort()
{
	if (node->next == node || node->next->next == node)
	{
		return;
	}

	JJList<T> carry;
	JJList<T> counter[64];
	int fill = 0;
	while (!empty())
	{
		carry.splice(carry.begin(), *this, begin());

		int i = 0;
		while(i < fill && !counter[i].empty())
		{
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill) ++fill;
	}

	for (int i = 1; i < fill; ++i)
	{
		counter[i].merge(counter[i-1]);
	}
	swap(counter[fill-1]);
}



}
#endif
