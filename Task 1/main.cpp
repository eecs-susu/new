#include <iostream>
#include <string>
#include <thread>

const char TEXT[] = "It's just simple text";

void SendMessage(std::string *const message_chan, bool *sended, int delay) {
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  *message_chan = TEXT;
  printf("Sender: %s\n", message_chan->c_str());
  *sended = true;
}

void ReceiveMessage(const std::string *message_chan, bool *sended, int delay) {
  while (!(*sended)) {
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  printf("Receiver: %s\n", message_chan->c_str());
}

int main(int argc, char const *argv[]) {
  int sender_delay, receiver_delay;

  printf("Enter delay for sender thread (ms): ");
  scanf("%d", &sender_delay);

  printf("Enter delay for receiver thread (ms): ");
  scanf("%d", &receiver_delay);

  std::string message_chan;
  bool sended = false;

  std::thread sender_thread(SendMessage, &message_chan, &sended, sender_delay);
  std::thread receiver_thread(ReceiveMessage, &message_chan, &sended,
                              receiver_delay);

  sender_thread.join();
  receiver_thread.join();

  return 0;
}
