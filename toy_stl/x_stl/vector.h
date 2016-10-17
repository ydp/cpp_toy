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

            // template<class InputIterator>
            void insert(iterator position, iterator first, iterator last)
            {
                if (first != last)
                {
                    size_type n = last - first;
                    if (size_type(end_of_storage - finish) >= n)
                    {
                        const size_type elems_after = finish - position;
                        iterator old_finish = finish;
                        if (elems_after > n)
                        {
                            uninitialized_copy(finish - n, finish, finish);
                            finish += n;
                            copy_backward(position, old_finish, old_finish);
                            copy(first, last, position);
                        }else
                        {
                            uninitialized_copy(first + elems_after, last, finish);
                            finish += n - elems_after;
                            uninitialized_copy(position, old_finish, old_finish);
                            finish += elems_after;
                            copy(first, first + elems_after, position);
                        }
                    }else
                    {
                        const size_type old_size = size();
                        const size_type len = old_size + max(old_size, n);
                        iterator new_start = dataallocator::allocate(len);
                        iterator new_finish = new_start;
                        new_finish = uninitialized_copy(start, position, new_start);
                        new_finish = uninitialized_copy(first, last, new_finish);
                        new_finish = uninitialized_copy(position, finish, new_finish);

                        destroy(start, finish);
                        deallocate();
                        start = new_start;
                        finish = new_finish;
                        end_of_storage = new_start + len;
                    }
                }
            }

            iterator erase(iterator position)
            {
                if (pisition != end())
                {
                    copy(position + 1, finish, position);
                }
                
                --finish;
                destroy(finish);
                return position;
            }

            iterator erase(iterator first, iterator last)
            {
                iterator i = copy(last, finish, first);
                destroy(i, finish);
                finish = finish - (last - first);
                return first;
            }

            void resize(size_type new_size, const T &value)
            {
                if (new_size < size())
                {
                    erase(begin() + new_size, end());
                }else
                {
                    insert(end(), new_size - size(), value);
                }
            }

            void resize(size_type new_size)
            {
                resize(new_size, T());
            }

            void clear()
            {
                erase(begin(), end());
            }

    };


    /// 赋值
    template<class T, class alloc>
    vector<T, alloc>& vector<T, alloc>::operator=(const vector<T, alloc> &rhs)
    {
        if (this != &rhs)
        {
            const size_type len = rhs.size();
            if (len > capacity())
            {
                iterator tmp = allocate_and_copy(len, rhs.begin(), rhs.end());
                destroy(start, finish);
                deallocate();
                start = tmp;
                finish = tmp + len;
                end_of_storage = finish;
            }else if (len < size())
            {
                iterator tmp = copy(rhs.begin(), rhs.end(), start);
                destroy(tmp, finish);
                finish = start + len;
            }else
            {
                copy(rhs.begin(), rhs.begin() + size(), start);
                finish = uninitialized_copy(rhs.begin() + size(), rhs.end(), finish);
            }
        }
        return *this;
    }

    template<class T, class alloc>
    vector<T, alloc>& vector<T, alloc>::operator=(vector<T, alloc> &rhs)
    {
        if (this != &rhs)
        {
            destroy(start, finish);
            deallocate();
            start = rhs.start;
            finish = rhs.finish;
            end_of_storage = rhs.end_of_storage;
            rhs.start = rhs.finish = rhs.end_of_storage = 0;
        }
        return *this;
    }

    template<class T, class alloc>
    void vector<T, alloc>::swap(vector<T, alloc> &rhs)
    {
        if (this != &rhs)
        {
            X_STL::swap(start, rhs.start);
            X_STL::swap(finish, rhs.finish);
            X_STL::swap(end_of_storage, rhs.end_of_storage);
        }
    }

    template<class T, class alloc>
    inline bool operator==(const vector<T, alloc> &v1, const <vector, alloc> &v2)
    {
        return v1.size() == v2.size() && equal(v1.begin(), v2.end(), v2.begin());
    }

    template<class T, class alloc>
    inline bool operator!=(const vector<T, alloc> &v1, const <vector, alloc> &v2)
    {
        return !(v1 == v2);
    }

    template<class T, class alloc>
    inline bool operator<(const vector<T, alloc> &v1, const <vector, alloc> &v2)
    {
        return lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
    }

    template<class T, class alloc>
    inline bool operator>(const vector<T, alloc> &v1, const <vector, alloc> &v2)
    {
        return v2 < v1;
    }

    template<class T, class alloc>
    inline bool operator<=(const vector<T, alloc> &v1, const <vector, alloc> &v2)
    {
        return !(v1 > v2);
    }

    template<class T, class alloc>
    inline bool operator>=(const vector<T, alloc> &v1, const <vector, alloc> &v2)
    {
        return !(v1 < v2);
    }

    template<class T, class alloc>
    void vector<T, alloc>::insert(iterator position, size_type n, const T &value)
    {
        if (n != 0)
        {
            if (size_type(end_of_storage - finish) >= n)
            {
                T copy_value = value;
                const size_type elems_after = finish - position;
                iterator old_finish = finish;
                if (elems_after > n)
                {
                    uninitialized_copy(finish - n, finish, finish);
                    finish = finish + n;
                    copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, copy_value);
                }else
                {
                    finish = uninitialized_fill_n(finish, n - elems_after, copy_value);
                    finish = uninitialized_copy(position, old_finish, finish);
                    fill(position, old_finish, copy_value);
                }
            }else
            {
                const size_type old_size = size();
                const size_type new_size = old_size + max(old_size, n);

                iterator new_start = dataallocator::allocate(new_size);
                iterator new_finish = new_start;
                new_finish = uninitialized_copy(start, position, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, value);
                new_finish = uninitialized_copy(position, finish, new_finish);

                destroy(start, finish);
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = start + new_size;
            }
        }
    }

    template<class T, class alloc>
    void vector<T, alloc>::insert(iterator position, const T &value)
    {
        insert(position, 1, value);
    }

    template<class T, class alloc>
    void vector<T, alloc>::insert_aux(iterator position, const T &value)
    {
        if (finish != end_of_storage)
        {
            construct(finish, *(finish - 1));
            ++finish;
            X_STL::copy_backward(position, finish - 2, finish - 1);
            *position = value;
        }else
        {
            const size_type old_size = size();
            const size_type new_size = old_size == 0 ? 1 : 2 * old_size;

            iterator new_start = dataallocator::allocate(new_size);
            iterator new_finish = new_start;
            new_finish = uninitialized_copy(start, position, new_start);
            construct(new_finish, x);
            ++new_finish;
            new_finish = uninitialized_copy(position, finish, new_finish);

            destroy(begin(), end());
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }
}


#endif
