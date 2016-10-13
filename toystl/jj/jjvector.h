#ifndef __JJVECTOR__
#define __JJVECTOR__

#include <iostream>
#include <algorithm>
#include <memory>

#include "destroy.h"

namespace JJ 
{

template <class T>
class JJVector
{
public:
	typedef T value_type;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;

	JJVector();
	JJVector(size_type n, T value = T());
	JJVector(iterator begin, iterator end);
	~JJVector();

	JJVector(const JJVector&);
	JJVector& operator=(const JJVector&);

	bool empty() const { return begin() == end(); }
	size_type size() const { return (size_type)(finish - start); }
	size_type capacity() const {return (size_type)(end_of_storage - start); }
	iterator begin() { return start; }
	const_iterator begin() const { return start; }
	iterator end() { return end; }
	const_iterator end() const { return end; }

	reference operator[](size_type i) { return *(start + i); }
	const_reference operator[](size_type i) const { return *(start + i); }

	void insert(iterator position, size_type n, const T& value);
	void push_back(const T& value);
	void pop_back();

	void erase(iterator first, iterator last);
	void clear();

	void reserve(size_type n);

protected:
	iterator start;
	iterator finish;
	iterator end_of_storage;

private:
	static std::allocator<T> alloc;
};

template <class T>
std::allocator<T> JJVector<T>::alloc;

template <class T>
JJVector<T>::JJVector()
	: start(NULL), finish(NULL), end_of_storage(NULL)
{
}

template <class T>
JJVector<T>::JJVector(size_type n, T value)
{
	start = alloc.allocate(n);
	end_of_storage = finish = start + n;
	for (iterator i = start; i != finish; ++i)
	{
		alloc.construct(i, value);
	}
}

template <class T>
JJVector<T>::JJVector(iterator begin, iterator end)
{
	const size_type n = end - begin;

	start = alloc.allocate(n);
	finish = end_of_storage = start + n;

	std::uninitialized_copy(begin, end, start);
}

template <class T>
JJVector<T>::~JJVector()
{
	::destroy(start, finish);

	alloc.deallocate(start, end_of_storage - start);
}

template <class T>
JJVector<T>::JJVector(const JJVector& rhs)
{
	start = alloc.allocate(rhs.capacity());
	std::uninitialized_copy(rhs.start, rhs.finish, start);
	finish = start + (rhs.finish - rhs.start);
	end_of_storage = start + (rhs.end_of_storage - rhs.start);
}

template <class T>
JJVector<T>& JJVector<T>::operator=(const JJVector& rhs)
{
	start = alloc.allocate(rhs.capacity());
	std::uninitialized_copy(rhs.start, rhs.finish, start);
	finish = start + (rhs.finish - rhs.start);
	end_of_storage = start + (rhs.end_of_storage - rhs.start);

	return *this;
}

template <class T>
void JJVector<T>::insert(iterator position, size_type n, const T& value)
{
	if (n <= end_of_storage - finish)
	{
		if (n <= finish - position)
		{
			std::uninitialized_copy(finish - n, finish ,finish);
			std::copy(position, finish - n, position + n);
			std::fill_n(position, n, value);
		}else
		{
			std::uninitialized_fill_n(finish, n - (finish - position), value);
			std::uninitialized_copy(position, finish, position + n);
			std::fill(position, finish, value);
		}
		finish += n;
	}else
	{ // relocate
		pointer new_start(NULL), new_finish(NULL);
		size_type old_type = end_of_storage - start;
		size_type new_size = old_type + std::max(old_type, n);
		new_start = alloc.allocate(new_size);

		new_finish = std::uninitialized_copy(start, position, new_start);
		std::uninitialized_fill_n(new_finish, n, value);
		new_finish += n;
		new_finish = std::uninitialized_copy(position, finish, new_finish);

		alloc.deallocate(start, end_of_storage - start);
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + new_size;
	}
}

template <class T>
void JJVector<T>::push_back(const T& value)
{
	insert(end(), 1, value);
}

template <class T>
void JJVector<T>::pop_back()
{
	alloc.destroy(--finish);
}

template <class T>
void JJVector<T>::erase(iterator first, iterator last)
{
	iterator old_finish = finish;
	finish = std::copy(last, finish, first);
	::destroy(finish, old_finish);
}

template <class T>
void JJVector<T>::clear()
{
	erase(start, finish);
}

template <class T>
void JJVector<T>::reserve(size_type n)
{
	if (capacity() < n)
	{
		iterator new_start = alloc.allocate(n);
		std::uninitialized_copy(start, finish, new_start);

		::destroy(start, finish);
		alloc.deallocate(start, size());

		const size_type old_size = finish - start;
		start = new_start;
		finish = new_start + old_size;
		end_of_storage = new_start + n;
	}
}


}
#endif /// end of JJVector
