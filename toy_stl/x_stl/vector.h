#ifndef _X_STL_VECTOR_H
#define _X_STL_VECTOR_H

#include "algorithm.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace X_STL
{

	template<class T, class alloc = allocator<T> >
	class vector
	{
		public:
			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T* iterator;
			typedef const T* const_iterator;
			typedef T& reference;
			typedef const T& const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef alloc = dataallocator;

		protected:
			iterator start;
			iterator finish;
			iterator end_of_storage;

			void insert_aux(iterator position, const T& value);

			void deallocate()
			{
				if (start)
				{
					dataallocator::deallocate(start, end_of_storage - start);
				}
			}

			iterator allocate_and_fill(size_type n, const T &value)
			{
				iterator dest = dataallocator::allocate(n);

				uninitialized_fill_n(dest, n, x);
				return dest;
			}

			template<class InputIterator>
			void allocate_and_copy(InputIterator first, InputIterator last)
			{
				start = dataallocator::allocate(last - first);
				finish = uninitialized_copy(first, last, start);
				end_of_storage = finish;
			}

			template<class InputIterator>
			void allocate_and_copy(size_type n, InputIterator frist, InputIterator last)
			{
				iterator result = dataallocator::allocate(n);
				uninitialized_copy(first, last, result);
				return result;
			}

			void fill_initialize(size_type n, const T &value)
			{
				start = allocate_and_fill(n, value);
				finish = start + n;
				end_of_storage = finish;
			}

		public:
			// 构造函数
			vector() : start(0), finish(0), end_of_storage(0) {}
			vector(size_type n, const T &value) { fill_initialize(n, value);  }
			vector(int n, const T &value) { fill_initialize(n, value);  }
			vector(long n, const T &value) { fill_initialize(n, value);  }
			explicit vector(size_type n) { fill_initialize(n, T());  }
			template<class InputIterator>
			vector(InputIterator first, InputIterator last) { allocate_and_copy(first, last);  }

			// 析构函数
			~vector()
			{
				destroy(start, finish);
				deallocate();
			}

			bool empty() const { return begin() == end(); }
			size_type size() const {  return size_type(finish - start); }
			size_type capacity() const { return size_type(end_of_storage - start);  }
			void reserve(size_type n)
			{
				if (capacity() < n)
				{
					const size_type old_size = size();
					iterator tmp = allocate_and_copy(n, start, finish);
					destroy(start, finish);
					deallocate();
					start = tmp;
					finish = tmp + old_size;
					end_of_storage = tmp + n;
				}
			}

			void shrink_to_fit()
			{
				iterator new_start = dataallocater::allocate(size());
				finish = uninitialized_copy(start, finish, new_start);
				deallocate();
				start = new_start;
				end_of_storage = finish;
			}

			vector<T, alloc>& operator=(const vector<T, alloc> &rhs);
			vector<T, alloc>& operator=(vector<T, alloc> &&rhs);
			void swap(vector &v);

			reference operator[](const difference_type n) { return *(begin() + n); }
			const reference operator[](const difference_type n) const { return *(cbegin() + n); }
			reference front() { return *begin(); }
			reference back() { return *(end() - 1); }
			pointer data() { return start;}

			iterator begin() { return start; }
			iterator end() {  return finish; }
			const_iterator cbegin() const { return start; }
			const_iterator cend() const {  return finish; }

			void pop_back()
			{
				--finish;
				destroy(finish);
			}

			void push_back(const T &value)
			{
				if (finish != end_of_storage)
				{
					construct(finish, value);
					++finish;
				}else
				{
					insert_aux(end(), value);
				}
			}

			void insert(iterator position, size_type n, const T &value);
			void insert(iterator position, const T &value);




	};

}


#endif
