#ifndef _X_STL_ALLOC_H
#define _X_STL_ALLOC_H

#include <cstdlib>

namespace X_STL
{
	class alloc
	{
		private:
			static const int _ALIGN = 8;
			static const int _MAX_BYTES = 128;
			static const int _NFREELISTS = _MAX_BYTES/_ALIGN;
			
			/// free-list, 内存链表
			union obj
			{
				union obj *next_obj;
				char client_data[1];
			};
			static obj *free_list[_NFREELISTS];

			/// 内存池的边界和当前程序分配的堆大小
			static char *start_free;
			static char *end_free;
			static size_t heap_size;
			
			/// 上调至8的倍数
			/// 假设_ALIGN = 8, bytes为任意数
			/// bytes           10101110
			/// _ALIGN - 1      00000111  +
			///                 10110101  = ROUND_UP 向上
			/// ~(_ALIGIN - 1)  11111000  &
			///                 10110000  = ALIGN    取整
			static size_t ROUND_UP(size_t bytes)
			{
				return ((bytes + _ALIGN - 1) & ~(_ALIGN - 1));
			}

			static size_t FREELIST_INDEX(size_t bytes)
			{
				return ((bytes + _ALIGN - 1) / _ALIGN - 1);
			}
			/// 给free_list分配链表, n 一定是8的倍数 
			static void *refill(size_t n);
			/// size也是8的倍数， nobjs是分配的链表结点个数，总共内存= size * nobjs
			static char *chunk_alloc(size_t size, int &nobjs);

		public:
			/// 供allocater调用,真正实施内存申请的方法
			static void *allocate(size_t n);
			static void deallocate(void *p, size_t n);
			static void *reallocate(void *p, size_t old_sz, size_t new_sz);
	};

}

#endif
