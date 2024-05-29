RingBuffer implements the "synchronized with" guarantee using the release-acquire binding and,
as a consequence of transitivity, the "happened before" guarantee for buffer data. 
Synchronization occurs using atomics and its barriers memory_order_release and memory_order_acquired