/*
 * Copyright (c) 2015 Yahoo! Inc. All rights reserved.
 */

#ifndef INCLUDE_LOCKFREEBLOCKINGQUEUE_H_
#define INCLUDE_LOCKFREEBLOCKINGQUEUE_H_

#include <atomic>
/*
 * A lockfree blocking queue refer from:
 * http://www.drdobbs.com/parallel/measuring-parallel-performance-optimizin/212201163?pgno=1
 */

template <typename T>
class LockFreeBlockingQueue : public Queue<T> {
 public:
  LockFreeBlockingQueue() {
    Node* t = new Node();
    first = t;
    divider.store(t, std::memory_order_relaxed);
    last.store(t, std::memory_order_relaxed);
  }
  ~LockFreeBlockingQueue() {
    while (first != nullptr) {  // release the list
      Node* tmp = first;
      first = tmp->next;
      delete tmp;
    }
  }

  std::string GetName() { return "LockFreeBlockingQueue"; };

  bool offer(const T& t) {
    Node* last_real = last.load(std::memory_order_acquire);
    last_real->next = new Node(t);                           // add the new item
    last.store(last_real->next, std::memory_order_release);  // publish it

    Node* divider_real = divider.load(std::memory_order_acquire);
    while (first != divider_real) {  // trim unused nodes
      Node* tmp = first;
      first = first->next;
      delete tmp;
    }
    return true;
  }

  bool poll(T& result) {
    Node* divider_real = divider.load(std::memory_order_acquire);
    Node* last_real = last.load(std::memory_order_acquire);
    if (divider_real != last_real) {       // if queue is nonempty
      result = divider_real->next->value;  // C: copy it back
      divider.store(divider_real->next, std::memory_order_release);
      return true;  // and report success
    }
    return false;  // else report empty
  }

 private:
  struct Node {
    Node() : next(nullptr) {}
    Node(T val) : value(val), next(nullptr) {}
    T value;
    Node* next;
  };
  Node* first;                       // for producer only
  std::atomic<Node*> divider, last;  // shared
};
#endif  // INCLUDE_LOCKFREEBLOCKINGQUEUE_H_
