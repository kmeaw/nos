#pragma once

#include <include/types.h>
#include <boot/grub.h>

namespace Kernel
{

namespace Core
{

class MemoryMap final
{
public:
    static MemoryMap& GetInstance()
    {
        static MemoryMap instance;

        return instance;
    }
    ~MemoryMap();

    bool AddRegion(u64 addr, u64 len, u32 type);

    bool FindRegion(ulong base, ulong limit, ulong& start, ulong& end);

private:
    MemoryMap(const MemoryMap& other) = delete;
    MemoryMap(MemoryMap&& other) = delete;
    MemoryMap& operator=(const MemoryMap& other) = delete;
    MemoryMap& operator=(MemoryMap&& other) = delete;

    MemoryMap();

    struct MemoryRegion
    {
        u64 Addr;
        u64 Len;
        u32 Type;
    };

    MemoryRegion Region[16];
    size_t Size;
};

}

}