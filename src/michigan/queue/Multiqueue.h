#ifndef MULTIQUEUE
#define MULTIQUEUE

#include "Queue.h"

class MultiQueue: public Queue{
protected:
    struct MultiQueueHeader{
      short port;
      char data[0];
    };
};

template <class T> class MultiDequeue: public MultiQueue {
    std::vector<T> queues;
public :
    MultiDequeue(int number){
      number = number > 0 ? number : 1;
      queues.resize(number);
    }
    virtual int push(char * data, int size, int priority, int src, std::vector<int> dest, int deadline){

        MultiQueueHeader * mqh = (MultiQueueHeader *) data;
        int payload_size = size - sizeof(MultiQueueHeader);

        std::cerr << "MDQ: " << size << " PORT: "<< mqh->port << std::endl;


        return queues[mqh->port].push(mqh->data, payload_size, priority, src, dest, deadline);
    }

    virtual int push(int port, char * data, int size, int priority, int src, std::vector<int> dest, int deadline){
        return queues[port].push(data, size, priority, src, dest, deadline);
    }

    int add(){
        queues.resize(queues.size() + 1);
        return queues.size() - 1;
    }

    virtual int pop(int port, const char * & data, int & priority, int & src){
        return queues[port].pop(data, priority, src);
    }
    virtual int pop(const char * & data, int & priority, int & src){
        return queues[0].pop(data, priority, src);
    }
};

class MultiEnqueue: public MultiQueue {
    Queue & queue;
public :
    MultiEnqueue(Queue & q):queue(q){

    }
    virtual int push(int port, char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

        std::cerr << "MEQ: " << size << " PORT: "<< port << std::endl;

        int header_size = sizeof(MultiQueueHeader);
        char data[header_size + size];
        MultiQueueHeader * mqh = (MultiQueueHeader *) data;
        mqh->port = port;
        memcpy(mqh->data, payload, size);
        return queue.push(data, header_size + size, priority, src, dest, deadline);
    }

    virtual int pop(const char * & data, int & priority, int & src){
        return queue.pop(data, priority, src);
    }
};
#endif
