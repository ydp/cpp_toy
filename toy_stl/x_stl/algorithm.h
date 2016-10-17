#ifndef _X_STL_ALGORITHM_H
#define _X_STL_ALGORITHM_H

#include <string.h>
#include "iterator.h"
#include "typetraits.h"


namespace X_STL
{
	template<class T>
	inline const T& max(const T &a, const T &b)
	{
		return a > b ? a : b;
	}

	template<class T>
	inline const T& min(const T &a, const T &b)
	{
		return a > b ? b : a;
	}

	template<class T>
	inline void swap(T &a, T &b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	/// 填充函数，所有元素填充同一个值
	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T &value)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
	}

	
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, T &value)
	{
		for (; n > 0; --n, ++first)
		{
			*first = value;
		}
		return first;
	}

	template<class InputIterator, class OutputIterator>
	struct _copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator dest)
		{
			return _copy_t(first, last, dest, iterator_category(first));
		}
	};

	template<class T>
	struct _copy_dispatch<T*, T*>
	{
		T* operator()(T *first, T* last, T *dest)
		{
			typedef typename _type_traits<T>::has_trivial_assignment_operator t;
			return _copy_t(first, last, dest, t());
		}
	};

	template<class T>
	struct _copy_dispatch<const T*, T*>
	{
		T* operator()(const T *first, const T *last, T *dest)
		{
			typedef typename _type_traits<T>::has_trivial_assignment_operator t;
			return _copy_t(first, last, dest, t());
		}
	};

	/// 把一段元素拷贝到另一个位置
	template<class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator dest)
	{
		return _copy_dispatch<InputIterator, OutputIterator>()(first, last, dest);
	}

	inline char* copy(const char *first, const char *last, char *dest)
	{
		memmove(dest, first, last - first);
		return dest + (last - first);
	}

	inline wchar_t* copy(const wchar_t *first, const wchar_t *last, wchar_t *dest)
	{
		memmove(dest, first, sizeof(wchar_t*) * (last-first));
		return dest + (last - first);
	}


	template<class InputIterator, class OutputIterator>
	inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator dest, input_iterator_tag)
	{
		for (; first != last; ++first)
		{
			*dest = *first;
		}
		return dest;
	}

	template<class RandomAccessIterator, class OutputIterator>
	inline OutputIterator _copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator dest, random_access_iterator_tag)
	{
		return _copy_d(first, last, dest, difference_type(first));
	}

	template<class RandomAccessIterator, class OutputIterator, class Distance>
	inline OutputIterator _copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator dest, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++dest, ++first)
		{
			*dest = *first;
		}
		return dest;
	}

	template<class T>
	inline T* _copy_t(const T *first, const T *last, T *dest, _true_type)
	{
		memmove(dest, first, sizeof(T) * (last - first));
		return dest;
	}

	template<class T>
	inline T* _copy_t(const T *first, const T *last, T *dest, _false_type)
	{
		return _copy_d(first, last, dest, (ptrdiff_t*)0);
	}

	template<class BidirectionalIterator, class Distance>
	inline BidirectionalIterator _copy_backward(BidirectionalIterator first,
												BidirectionalIterator last,
												BidirectionalIterator dest,
												bidirectional_iterator_tag,
												Distance*)
	{
		while (first != last)
		{
			*(--dest) = *(--last);
		}
		return dest;
	}

	template<class RandomAccessIterator, class BidirectionalIterator, class Distance>
	inline BidirectionalIterator _copy_backward(RandomAccessIterator first,
												RandomAccessIterator last,
												BidirectionalIterator dest,
												random_access_iterator_tag,
												Distance*)
	{
		for (Distance n = last - first; n > 0; --n)
		{
			*(--dest) = *(--last);
		}
		return dest;
	}

	template<class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
												BidirectionalIterator1 last,
												BidirectionalIterator2 dest)
	{
		return _copy_backward(first, last, dest, iterator_category(first), differenct_type(first));
	}

	template<class InputIterator1, class InputIterator2>
	inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	
	{
		while (first1 != last1)
		{
			if (*first1 != *first2)
			{
				return false;
			}
			++first1, ++first2;
		}
		return true;
	}

	template<class InputIterator1, class InputIterator2, class BinaryPredicate>
	inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred)
	{
		while (first1 != last1)
		{
			if (!binary_pred(*first1, *first2))
			{
				return false;
			}
			++first1, ++first2;
		}
		return true;
	}

	template<class InputIterator1, class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				return true;
			}
			if (*first1 > *first2)
			{
				return false;
			}
		}
		return first1 == last1 && first2 != last2;
	}

	template<class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare cmp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (cmp(*first1, *first2))
			{
				return true;
			}
			if (cmp(*first2, *first1))
			{
				return false;
			}
		}
		return first1 == last1 && first2 != last2;
	}

	template<class InputIterator, class T>
	T accumulate(InputIterator first, InputIterator last, T init)
	{
		while (first != last)
		{
			init = init + *first;
			++first;
		}
		return init;
	}

	template<class InputIterator, class T, class BinaryOperation>
	T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation op)
	{
		while (first != last)
		{
			init = op(init, *first);
			++first;
		}
		return init;
	}

	template<class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T &value)
	{
		while (first != last && *first != value)
		{
			++first;
		}
		return first;
	}

	template<class ForwardIterator, class T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T &value)
	{
		return _lower_bound(first, last, value, differenct_type(first), iterator_category(first));
	}

	template<class ForwardIterator, class T, class Distance>
	inline ForwardIterator _lower_bound(ForwardIterator first,
										ForwardIterator last,
										const T &value,
										Distance*,
										forward_iterator_tag)
	{
		Distance len = distance(first, last);
		Distance half;
		ForwardIterator mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first;
			advance(mid, half);
			if (*mid < value)
			{
				first = mid;
				++first;
				len = len - half - 1;
			}else
			{
				len = half;
			}
		}
		return first;
	}

	template<class ForwardIterator, class T, class Distance>
	inline ForwardIterator _lower_bound(ForwardIterator first,
										ForwardIterator last,
										const T &value,
										Distance*,
										random_access_iterator_tag)
	{
		Distance len = distance(first, last);
		Distance half;
		ForwardIterator mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first + half;
			if (*mid < value)
			{
				first = mid + 1;
				len = len - half -1;
			}else
			{
				len = half;
			}
		}
		return first;
	}

}

#endif
