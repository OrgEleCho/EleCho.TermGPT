#pragma once
#include <deque>
#include <mutex>

template<typename T>
class ThreadSafeDeque {
 public:
  ThreadSafeDeque() = default;
  ~ThreadSafeDeque() = default;

  ThreadSafeDeque(const ThreadSafeDeque&) = delete;
  ThreadSafeDeque& operator=(const ThreadSafeDeque&) = delete;

  void push_front(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    deque_.push_front(std::move(value));
  }

  void push_back(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    deque_.push_back(std::move(value));
  }

  bool try_pop_front(T& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (deque_.empty()) {
      return false;
    }
    value = std::move(deque_.front());
    deque_.pop_front();
    return true;
  }

  bool try_pop_back(T& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (deque_.empty()) {
      return false;
    }
    value = std::move(deque_.back());
    deque_.pop_back();
    return true;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return deque_.empty();
  }

 private:
  mutable std::mutex mutex_;
  std::deque<T> deque_;
};