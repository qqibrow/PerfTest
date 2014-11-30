#ifndef QQIBROW_LOCKFREEBLOCKINGQUEUE_H_
#define QQIBROW_LOCKFREEBLOCKINGQUEUE_H_

#include <boost/atomic.hpp>

template <typename T>
class LockFreeBlockingQueue : public BlockingQueue<T> {
public:
    LockFreeBlockingQueue() {
        Node* t = new Node();
        first = t;
        divider.store(t, boost::memory_order_relaxed);
        last.store(t, boost::memory_order_relaxed);
    }
    ~LockFreeBlockingQueue() {
        while( first != nullptr ) {   // release the list
            Node* tmp = first;
            first = tmp->next;
            delete tmp;
        }
    }

    virtual T take() {
        T return_value;
        while(!Consume(return_value));
        return return_value;
    }

    virtual void put(T value) {
        Produce(value);
    }
private:
    void Produce( const T& t ) {
        Node* last_real = last.load(boost::memory_order_relaxed);
        last_real->next = new Node(t);    // add the new item
        last.store(last_real->next, boost::memory_order_relaxed); // publish it

        Node* divider_real = divider.load(boost::memory_order_relaxed);
        while( first != divider_real ) { // trim unused nodes
            Node* tmp = first;
            first = first->next;
            delete tmp;
        }
    }

    bool Consume( T& result ) {
        Node* divider_real = divider.load(boost::memory_order_relaxed);
        Node* last_real = last.load(boost::memory_order_relaxed);
        if( divider_real != last_real ) {         // if queue is nonempty
            result = divider_real->next->value;  // C: copy it back
            divider.store(divider_real->next, boost::memory_order_relaxed);
            return true;              // and report success
        }
        return false;               // else report empty
    }

private:
    struct Node {
        Node() : next(nullptr) {}
        Node( T val ) : value(val), next(nullptr) { }
        T value;
        Node* next;
    };
    Node* first;             // for producer only
    boost::atomic<Node*> divider, last;         // shared
};


template<>
bool LockFreeBlockingQueue<long>::Consume(long& result) {
    Node* divider_real = divider.load(boost::memory_order_relaxed);
    Node* last_real = last.load(boost::memory_order_relaxed);
    if( divider_real != last_real ) {         // if queue is nonempty
        result = divider_real->next->value;  // C: copy it back
        divider.store(divider_real->next, boost::memory_order_relaxed);
        return true;              // and report success
    }
    return false;               // else report empty
}

template<>
void LockFreeBlockingQueue<long>::Produce( const long& t ) {
    Node* last_real = last.load(boost::memory_order_relaxed);
    last_real->next = new Node(t);    // add the new item
    last.store(last_real->next, boost::memory_order_relaxed); // publish it

    Node* divider_real = divider.load(boost::memory_order_relaxed);
    while( first != divider_real ) { // trim unused nodes
        Node* tmp = first;
        first = first->next;
        delete tmp;
    }
}
#endif