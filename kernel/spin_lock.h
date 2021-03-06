#pragma once

#include "asm.h"
#include "preempt.h"

#include <lib/lock.h>

namespace Kernel
{

namespace Core
{

class SpinLock
	: public Shared::LockInterface
	, public Shared::SharedLockInterface
{
public:
	SpinLock()
		: RawLock(0)
	{
	}

	virtual void Lock(ulong& flags) override
	{
		flags = GetRflags();
		InterruptDisable();
		PreemptDisable();
		SpinLockLock(&RawLock);
	}

	virtual void Unlock(ulong flags) override
	{
		SpinLockUnlock(&RawLock);
		SetRflags(flags);
		PreemptEnable();
	}

	virtual void SharedLock(ulong& flags) override
	{
		Lock(flags);
	}

	virtual void SharedUnlock(ulong flags) override
	{
		Unlock(flags);
	}

	virtual ~SpinLock()
	{
	}
private:
	ulong RawLock;
};

}

}