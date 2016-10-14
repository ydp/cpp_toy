#include "alloc.h"

namespace X_STL
{
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	/// free_list是一个数组，数组的每个元素都是一个指向某个链接的指针
	/// 每个元素对应的链接内存大小不一样，以8的倍数依次递增: 8, 16, 32...
	alloc::obj *alloc::free_list[_NFREELIST] = {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};

	void *alloc::allocate(size_t n)
	{
		/// 这里的内存技巧主要是避免小块内存分配的低效率和内存碎片
		/// 所以对于大块内存就没必要折腾，直接malloc
		if (n > _MAX_BYTES)
		{
			return malloc(n);
		}
		int index = FREELIST_INDEX(n);
		obj *result = free_list[index];
		/// 一开始free_list的所有指针都初始化为0了
		/// 所以这里判断不为0就表示已有指针指向了内存链接
		if (result)
		{
			/// 每次把链表头结点分配出去，free_list槽指向下一个结点
			free_list[index] =  result->next_obj;
			return result;
		}
		else
		{ /// free_list这个槽并没有指向任何链表
			/// 从内存池获取
			void *r = refill(ROUND_UP(n));
			return r;
		}
	}

	void alloc::deallocate(void *p, size_t n)
	{
		if (n > _MAX_BYTES)
		{
			free(p);
			return;
		}
		else
		{
			/// 仍然是先找到槽
			int index = FREELIST_INDEX(n);
			obj *q = static_cast<obj*> (p);
			/// 指向链表头
			q->next_obj = free_list[index];
			/// 让槽指向新的链表头
			free_list[index] = q;
		}
	}

	void *alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
	{
		deallocate(p, old_sz);
		p = allocate(new_sz);
		return p;
	}

	void *alloc::refill(size_t n)
	{
		int nobjs = 20;
		char *chunk = chunk_alloc(n, nobjs);
		if (nobjs == 1)
		{
			/// 如果chunk_alloc返回空指针，这里也会返回空指针
			return chunk;
		}

		obj *result = (obj*)chunk;
		obj *next_obj = (obj*)(chunk + n);
		obj *current_obj = next_obj;
		free_list[FREELIST_INDEX(n)] = current_obj;
		for (int i = 2; i <= nobjs; ++i)
		{
			if (i == nobjs)
			{
				current_obj->next_obj = 0;
				break;
			}else
			{
				next_obj = (obj*)((char*)(next_obj)+n);
				current_obj->next_obj = next_obj;
				current_obj = next_obj;
			}
		}

		return result;
	}

	/// 这里才是真正从内存池分配内存的地方
	void *alloc::chunk_alloc(size_t size, int &nobjs)
	{
		char *result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return result;
		}else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			total_bytes = nobjs * size;
			result = start_free;
			start_free += total_bytes;
			return result;
		}else
		{
			if (bytes_left > 0)
			{
				int index = FREELIST_INDEX(bytes_left);
				((obj*)(start_free))->next_obj = free_list[index];
				free_list[index] = (obj*)(start_free);
			}
			///
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			start_free = static_cast<char*> (malloc(bytes_to_get));
			if (start_free == 0)
			{
				/// 申请内存失败，从比原先要分配内存的槽更大的槽上寻找是否有可用的链接空间，如果有则分配给内存池，相当于从大块链表上摘一些结点分到小块链表上
				for (int i = size; i <= _MAX_BYTES; i+=_ALIGN)
				{	
					int index = FREELIST_INDEX(i);
					/// 找到一个就返回
					if (free_list[index] != 0)
					{
						start_free = (char*)(free_list[index]);
						free_list[index] = free_list[index]->next_obj;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;
				return nullptr;
			}else
			{
				heap_size += bytes_to_get;
				end_free = start_free + bytes_to_get;
				return chunk_alloc(size, nobjs);
			}
		}
	}
}
