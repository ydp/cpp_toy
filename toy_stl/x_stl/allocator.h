#ifndef _X_STL_ALLOCATOR_H
#define _X_STL_ALLOCATOR_H

#include "alloc.h"

namespace X_STL
{

	template<class T>
	class allocator
	{
		public:
			typedef T  value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;

			static T *allocate();
			static T *allocate(size_t n);
			static void deallocate(T *p);
			static void deallocate(T *p, size_t n);
	};

	template<class T>
	T *allocator<T>::allocate()
	{
		return static_cast<T*> (alloc::allocate(sizeof(T)));
	}

	template<class T>
	T *allocator<T>::allocate(size_t n)
	{
		if (n == 0) return 0;
		return static_case<T*> (alloc::allocate(sizeof(T) * n));
	}

	template<class T>
	void allocator<T>::deallocate(T *p)
	{
		alloc::deallocate(static_cast<void *> (p), sizeof(T));
	}

	template<class T>
	void allocator<T>::deallocate(T *p, size_t n)
	{
		if (n == 0) return;
		alloc::deallocate(static_cast<void *> (p), sizeof(T) * n);
	}



}
#endif
