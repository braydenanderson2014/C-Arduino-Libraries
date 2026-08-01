# SimpleChannel

SimpleChannel is a fixed-capacity queue for message passing between tasks.

## Why this exists

- Gives you bounded producer-consumer communication.
- Avoids unbounded dynamic allocation in task handoff paths.
- Works on RTOS-backed targets and simpler cooperative targets.

## API

- `bool send(const T& item, unsigned long timeoutMs = 0)`
- `bool trySend(const T& item)`
- `bool receive(T& out, unsigned long timeoutMs = 0)`
- `bool tryReceive(T& out)`
- `size_t size() const`
- `static constexpr size_t capacity()`
- `bool empty() const`
- `bool full() const`

## Notes

`timeoutMs = 0` means wait forever on RTOS targets.

Example usage:

```cpp
SimpleChannel<int, 16> channel;
channel.send(42);
int value = 0;
if (channel.receive(value, 100)) {
  // Use value.
}
```
