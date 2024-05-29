#pragma once

#include <atomic>

class AtomicCounter
{
public:
	void Increment();
	size_t Get();

private:
	char x[128];
    std::atomic<size_t> count_{0};
};

