#include "AtomicCounter.h"

void AtomicCounter::Increment()
{
	count_.fetch_add(1, std::memory_order_release);
}

size_t AtomicCounter::Get()
{
	return count_.load(std::memory_order_acquire);
}
