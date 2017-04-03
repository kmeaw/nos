#pragma once

#include "stdlib.h"
#include "panic.h"
#include "error.h"
#include "lock.h"

namespace Kernel
{

namespace Core
{

template <typename T, size_t Capacity = Shared::PageSize, typename LockType = Shared::NopLock>
class RingBuffer final
{
public:
    RingBuffer()
    {
        Shared::AutoLock lock(Lock);

        Size = 0;
        StartIndex = 0;
        EndIndex = 0;
    }

    ~RingBuffer()
    {
        Shared::AutoLock lock(Lock);
    }

    bool Put(const T& value)
    {
        Shared::AutoLock lock(Lock);

        if (Size == Capacity)
        {
            return false;
        }

        size_t position = EndIndex;
        Buf[position] = value;
        EndIndex = (EndIndex + 1) % Capacity;
        Size++;
        return true;
    }

    bool IsEmpty()
    {
        Shared::AutoLock lock(Lock);

        return (Size == 0) ? true : false;
    }

    bool IsFull()
    {
        Shared::AutoLock lock(Lock);

        return (Size == Capacity) ? true : false;
    }

    size_t GetSize()
    {
        Shared::AutoLock lock(Lock);

        return Size;
    }

    size_t GetCapacity()
    {
        Shared::AutoLock lock(Lock);

        return Capacity;
    }

    T Get()
    {
        Shared::AutoLock lock(Lock);

        if (Size == 0)
        {
            return T();
        }

        size_t position = StartIndex;
        StartIndex = (StartIndex + 1) % Capacity;
        Size--;
        return Buf[position];
    }

private:
    RingBuffer(const RingBuffer& other) = delete;
    RingBuffer(RingBuffer&& other) = delete;
    RingBuffer& operator=(const RingBuffer& other) = delete;
    RingBuffer& operator=(RingBuffer&& other) = delete;

    size_t StartIndex;
    size_t EndIndex;
    size_t Size;
    T Buf[Capacity];

    LockType Lock;
};

}
}