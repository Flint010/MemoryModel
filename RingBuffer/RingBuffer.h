#pragma once

#include <vector>
#include <atomic>

template<typename T>
class RingBuffer
{
	struct Slot {
		T value;
	};

public:
	explicit RingBuffer(const size_t capacity);

	bool TryProduce(T value);
	bool TryConsume(T& value);

private:
	bool isEmpty(size_t head, size_t tail) const;
	bool isFull(size_t head, size_t tail) const;
	size_t Next(size_t index) const;

private:
	std::vector<Slot> buffer_;

	std::atomic<size_t> head_{ 0 };
	std::atomic<size_t> tail_{ 0 };
};

template<typename T>
inline RingBuffer<T>::RingBuffer(const size_t capacity)
	: buffer_(capacity + 1)
{	
}

template<typename T>
inline bool RingBuffer<T>::TryProduce(T value)
{
	const auto current_head = head_.load(std::memory_order_acquire);
	const auto current_tail = tail_.load(std::memory_order_relaxed);

	if (isFull(current_head, current_tail)) {
		return false;
	}

	buffer_[current_tail].value = std::move(value);
	tail_.store(Next(current_tail), std::memory_order_release);
	return true;
}

template<typename T>
inline bool RingBuffer<T>::TryConsume(T& value)
{
	const auto current_head = head_.load(std::memory_order_relaxed);
	const auto current_tail = tail_.load(std::memory_order_acquire);

	if (isEmpty(current_head, current_tail)) {
		return false;
	}

	value = std::move(buffer_[current_head].value);
	head_.store(Next(current_head), std::memory_order_release);
	return true;
}

template<typename T>
inline bool RingBuffer<T>::isEmpty(size_t head, size_t tail) const
{
	return head == tail;
}

template<typename T>
inline bool RingBuffer<T>::isFull(size_t head, size_t tail) const
{
	return Next(tail) == head;
}

template<typename T>
inline size_t RingBuffer<T>::Next(size_t index) const
{
	return (index + 1) % buffer_.size();
}

