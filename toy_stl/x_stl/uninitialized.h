#ifndef _X_STL_UNINITIALIZED_H
#define _X_STL_UNINITIALIZED_H

#include "algorithm.h"
#include "typetraits.h"
#include "iterator.h"
#include "construct.h"


namespace X_STL
{
	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, _true_type)
	{
		return copy(first, last, dest);
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, _false_type)
	{
		ForwardIterator cur = dest;
		for (; first != last; ++first, ++cur)
		{
			construct(&*cur, *first);
		}
		return cur;
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
	{
		typedef typename _type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _uninitialized_copy_aux(first, last, dest, isPODType());
	}

	inline char* uninitialized_copy(const char *first, const char *last, char *dest)
	{
		memmove(dest, first, last - first);
		return dest + (last - first);
	}

	inline wchar_t* uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *dest)
	{
		memmove(dest, first, sizeof(wchar_t) * (last - first));
		return dest + (last - first);
	}

	
	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &value, _true_type)
	{
		fill(first, last, value);
	}

	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &value, _false_type)
	{
		for (; first != last; ++first)
		{
			construct(first, value);
		}
	}

	template<class ForwardIterator, class T>
	void uninitialzied_fill(ForwardIterator first, ForwardIterator last, const T &value)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_uninitialzied_fill_aux(first, last, value, isPODType());
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &value, _true_type)
	{
		return fill_n(first, n, value);
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &value, _false_type)
	{
		int i = 0;
		for (; i != n; ++i)
		{
			construct((T*)(first + i), value);
		}
		return (first + i);
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &value)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first, n, value, isPODType());
	}


}

#endif
