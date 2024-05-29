#include <iostream>
#include <thread>


#include "RingBuffer.h"
#include "stopwatch.h"

void Stress() {
	RingBuffer<int> buffer(256);
	size_t N = 1'000'000;

	StopWatch watch;

	std::thread produser([&]() {
		for (int i = 0; i < N; i++) {
			while (!buffer.TryProduce(i)) {
				std::this_thread::yield();
			}
		}
	});

	long long diget = 0;

	std::thread consumer([&]() {
		for (int i = 0; i < N; i++) {
			int value;
			while (!buffer.TryConsume(value)) {
				std::this_thread::yield();
			}
			diget += value;
		}
	});

	produser.join();
	consumer.join();

	auto time_ms = watch.Elapsed();

	std::cout << "Diget: " << diget << "\n";
	std::cout << "Elapsed: " << time_ms << " ms" << std::endl;
}




int main() {

	while(true)
		Stress();

	return 0;
}