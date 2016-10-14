#ifndef __DESTROY__
#define __DESTROY__

template <class T>
void destroy(T* pointer)
{
	pointer->~T();
}

template <class ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last)
{
	for (ForwardIterator it = first; it != last; ++it)
	{
		destroy(&*it);
	}
}

#endif
