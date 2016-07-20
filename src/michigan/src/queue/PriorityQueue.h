#ifndef PRIORITYQUEUE
#define PRIORITYQUEUE

#include "Queue.h"

class PriorityQueue : public Queue {

    std::mutex mtx;
    sem_t sem;
    int ts;
    int header_size;

    struct MessageHeader{
        int mid;
        char data[0];
    };

    struct Message{
        int priority;
        int src;
        std::vector<int> dest;
        int deadline;
        std::vector<char> data;
        int ts;
    };

    class mycomparison{
        int type;
    public:
        mycomparison(int type = 0){
            this->type = type;
        }
        bool operator()(const Message& lhs, const Message& rhs) const{
            if (type == PRIORITY){
                if (lhs.priority == rhs.priority){
                    return lhs.ts > rhs.ts;
                }
                return lhs.priority < rhs.priority;
            }else if (type == DEADLINE){
                return lhs.priority > rhs.priority;
            }
        }
    };

    std::priority_queue<Message, std::vector<Message>, mycomparison> queue;

public:

    enum{PRIORITY, DEADLINE};

    PriorityQueue(int sort = 0):queue(sort){
        sem_init(&sem,0,0);
        header_size = sizeof(MessageHeader);
    }

    virtual int push(char * data, int size, int priority, int src, std::vector<int> dest, int deadline){

        std::cerr << "PQ: " << size << std::endl;

        Message m;

        m.data.resize(size + header_size);
        MessageHeader * mh = (MessageHeader *) &m.data[0];
        memcpy(mh->data, data, size);

        m.ts = ts++;
        m.priority = priority;
        m.src = src;
        m.dest = dest;

        mtx.lock();
        queue.push(m);
        mtx.unlock();

        sem_post(& sem);
        return queue.size() - 1;
    }

    virtual int pop(const char * & data, int & priority, int & src){
        sem_wait(&sem);
        mtx.lock();
        const Message & m = queue.top();
        MessageHeader * mh = (MessageHeader *) &m.data[0];

        priority = m.priority;
        src = m.src;

        data = &mh->data[0];
        return m.data.size() - header_size;
    }

    virtual void done(){
        queue.pop();
        mtx.unlock();
    }

    virtual int size(){
        return queue.size();
    }
};
#endif
