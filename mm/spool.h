#pragma once


#include "page_allocator.h"

#include <kernel/spin_lock.h>
#include <lib/list_entry.h>

namespace Kernel
{

namespace Core
{

class SPool
{
public:
    SPool();
    virtual ~SPool();

    void Setup(size_t size, PageAllocator* pageAllocator = nullptr);
    void* Alloc();
    void Free(void *ptr);

private:
    using ListEntry = Shared::ListEntry;

    bool CheckSize(size_t size);

    struct Page {
        ListEntry Link;
        u8 Data[1]; 
    };

    ulong Usage;
    size_t Size;
    ListEntry PageList;
    ListEntry BlockList;
    SpinLock Lock;
    PageAllocator* PageAllocer;
};

}
}