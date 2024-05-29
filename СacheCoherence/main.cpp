#include "iostream"
#include <thread>
#include <vector>
#include <array>
#include <functional>

#include "AtomicCounter.h"
#include "stopwatch.h"


class SharedCounter {
private:
	static const size_t kShared = 8;
public:

	void Increment() {
		size_t index = GetSharedIndex();
		shared_[index].Increment(); // инкрементим в разные €чейки пам€ти
	};

	size_t Get() {
		size_t res = 0;
		for (auto& x : shared_) {
			res += x.Get();
		}
		return res;
	}

private:
	static size_t GetSharedIndex() {
		static std::hash<std::thread::id> hasher;

		auto t_id = std::this_thread::get_id();
		return hasher(t_id) % kShared;
	}

private:
	std::array<AtomicCounter, kShared> shared_;
};


void Stress() {

	std::vector<std::thread> threads;
	//AtomicCounter counter;
	SharedCounter counter;


	StopWatch watch;
	for (int i = 0; i < 10; i++) {
		threads.emplace_back([&counter]() {
			for (int i = 0; i < 1000000; i++) {
				counter.Increment();
			}
		});
	}

	for (auto &t : threads) {
		t.join();
	}
	auto elapsed = watch.Elapsed();

	std::cout << "Elapsed: " << elapsed << " ms\n";
	std::cout << counter.Get() << "\n" << std::endl;
}


void StoreBuffering() {
	// без использовани€ атомиков нету гарантий, что компил€тор или процессор не переставит инструкции местами (synchronization order), 
	// и как итог мы можем получить r1 === r2 == 0. (Ќо на x86 все работает и так, но гарантий нету)
	// ≈сли исполльзовать барьеры memory_order_release и memory_order_acquire, мы лишаемс€ synchronization order, а значит мы можем получит два нул€ 
	// » эти барьеры не дадут нам гарантий ни synchronized with и, как итог, ни happens befor

	std::atomic<int> x(0);
	std::atomic<int> y(0);
	//int x(0); 
	//int y(0);

	int r1 = 0;
	int r2 = 0;


	auto f1 = [&]() {
		x.store(1);
		//x = 1;
		r1 = y.load();
		//r1 = y;
	};

	auto f2 = [&]() {
		y.store(1);
		//y = 1;
		r2 = x.load();
		//r2 = x;
	};

	std::thread tr2(f2);
	std::thread tr1(f1);
	

	tr1.join();
	tr2.join();


	if (r1 == 0 && r2 == 0) {
		std::cout << "Error!!" << " r1 == r2 == 0" << std::endl;
		abort();
	}
	else {
		std::cout << "r1 = " << r1 << "  r2 = " << r2 << std::endl;
	}

}



int main() {

	//for (int i = 0; i < 15; i++)
	//	Stress();


	while (true)
	{
		StoreBuffering();
	}
		


	return 0;
}