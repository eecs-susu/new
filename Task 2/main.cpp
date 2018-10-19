#include <algorithm>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

template <typename T>
class SafeQueue {
  std::queue<T> queue_;
  int limit_;
  std::mutex mutex_;
  bool closed_;

  bool Empty() { return Lenght() == 0; }

  int Lenght() {
    mutex_.lock();
    int result = queue_.size();
    mutex_.unlock();
    return result;
  }

 public:
  explicit SafeQueue(int limit = 0) : limit_(limit), closed_(false) {}

  void Push(const T v) {
    bool pushed = false;
    while (!pushed) {
      if (Lenght() <= limit_) {
        pushed = true;
        queue_.push(v);
      }
      pushed |= closed_;
    }
  }

  T Pop() {
    bool popped = false;
    T result;

    while (!popped) {
      if (!Empty()) {
        popped = true;
        result = queue_.front();
        queue_.pop();
      }
      popped |= closed_;
    }

    return result;
  }

  void Close() {
    mutex_.lock();
    closed_ = true;
    mutex_.unlock();
  }

  bool Closed() {
    mutex_.lock();
    bool result = closed_;
    mutex_.unlock();
    return result;
  }
};

void generate_sequence(int lenght, SafeQueue<std::pair<int, int>> *queue,
                       int thread_id, int delay) {
  for (int i = 0; i < lenght; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    queue->Push(std::make_pair(thread_id, rand()));
  }
}

void handle_queue(SafeQueue<std::pair<int, int>> *queue, int delay) {
  while (!queue->Closed()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    std::pair<int, int> item = queue->Pop();
    printf("Item %d from thread %d\n", item.second, item.first);
  }
}

int main(int argc, char const *argv[]) {
  int first_generator_delay, second_generator_delay, handler_delay;

  printf("Enter delay for first generator (ms): ");
  scanf("%d", &first_generator_delay);

  printf("Enter delay for second generator (ms): ");
  scanf("%d", &second_generator_delay);

  printf("Enter delay for handler (ms): ");
  scanf("%d", &handler_delay);

  SafeQueue<std::pair<int, int>> queue(10);

  std::thread first_generator_thread(generate_sequence, 20, &queue, 1,
                                     first_generator_delay);
  std::thread second_generator_thread(generate_sequence, 20, &queue, 2,
                                      second_generator_delay);
  std::thread handler_thread(handle_queue, &queue, handler_delay);

  first_generator_thread.join();
  second_generator_thread.join();

  queue.Close();

  handler_thread.join();

  return 0;
}
