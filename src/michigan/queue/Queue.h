#include <map>
#include <vector>
#include <string.h>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <zlib.h>
#include <iostream>

#ifndef QUEUE
#define QUEUE

class Queue{

public:
    virtual int push(char * data, int size, int priority, int src, std::vector<int> dest, int deadline){
        fprintf(stderr,"%2d %s\n",priority, data);
    }
    virtual int pop(const char * & data, int & priority, int & src){

    }
    virtual void done(){

    }
    virtual int size(){

    }
};

//class PriorityQueue : public Queue {

//    std::mutex mtx;
//    sem_t sem;
//    int ts;
//    int header_size;

//    struct MessageHeader{
//        int mid;
//        char data[0];
//    };

//    struct Message{
//        int priority;
//        int src;
//        std::vector<int> dest;
//        int deadline;
//        std::vector<char> data;
//        int ts;
//    };

//    class mycomparison{
//        int type;
//    public:
//        mycomparison(int type = 0){
//            this->type = type;
//        }
//        bool operator()(const Message& lhs, const Message& rhs) const{
//            if (type == PRIORITY){
//                if (lhs.priority == rhs.priority){
//                    return lhs.ts > rhs.ts;
//                }
//                return lhs.priority < rhs.priority;
//            }else if (type == DEADLINE){
//                return lhs.priority > rhs.priority;
//            }
//        }
//    };

//    std::priority_queue<Message, std::vector<Message>, mycomparison> queue;

//public:

//    enum{PRIORITY, DEADLINE};

//    PriorityQueue(int sort = 0):queue(sort){
//        sem_init(&sem,0,0);
//        header_size = sizeof(MessageHeader);
//    }

//    virtual int push(char * data, int size, int priority, int src, std::vector<int> dest, int deadline){

//        std::cerr << "PQ: " << size << std::endl;

//        Message m;

//        m.data.resize(size + header_size);
//        MessageHeader * mh = (MessageHeader *) &m.data[0];
//        memcpy(mh->data, data, size);

//        m.ts = ts++;
//        m.priority = priority;
//        m.src = src;
//        m.dest = dest;

//        mtx.lock();
//        queue.push(m);
//        mtx.unlock();

//        sem_post(& sem);
//        return queue.size() - 1;
//    }

//    virtual int pop(const char * & data, int & priority, int & src){
//        sem_wait(&sem);
//        mtx.lock();
//        const Message & m = queue.top();
//        MessageHeader * mh = (MessageHeader *) &m.data[0];

//        priority = m.priority;
//        src = m.src;

//        data = &mh->data[0];
//        return m.data.size() - header_size;
//    }

//    virtual void done(){
//        queue.pop();
//        mtx.unlock();
//    }

//    virtual int size(){
//        return queue.size();
//    }
//};

//class MultiQueue: public Queue{
//protected:
//    struct MultiQueueHeader{
//      short port;
//      char data[0];
//    };
//};

//template <class T> class MultiDequeue: public MultiQueue {
//    std::vector<T> queues;
//public :
//    MultiDequeue(int number){
//      number = number > 0 ? number : 1;
//      queues.resize(number);
//    }
//    virtual int push(char * data, int size, int priority, int src, std::vector<int> dest, int deadline){

//        std::cerr << "MDQ: " << size << std::endl;


//        MultiQueueHeader * mqh = (MultiQueueHeader *) data;
//        int payload_size = size - sizeof(MultiQueueHeader);
//        return queues[mqh->port].push(mqh->data, payload_size, priority, src, dest, deadline);
//    }

//    virtual int push(int port, char * data, int size, int priority, int src, std::vector<int> dest, int deadline){
//        return queues[port].push(data, size, priority, src, dest, deadline);
//    }

//    int add(){
//        queues.resize(queues.size() + 1);
//        return queues.size() - 1;
//    }

//    virtual int pop(int port, const char * & data, int & priority, int & src){
//        return queues[port].pop(data, priority, src);
//    }
//    virtual int pop(const char * & data, int & priority, int & src){
//        return queues[0].pop(data, priority, src);
//    }
//};

//class MultiEnqueue: public MultiQueue {
//    Queue & queue;
//public :
//    MultiEnqueue(Queue & q):queue(q){

//    }
//    virtual int push(int port, char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

//        std::cerr << "MEQ: " << size << std::endl;

//        int header_size = sizeof(MultiQueueHeader);
//        char data[header_size + size];
//        MultiQueueHeader * mqh = (MultiQueueHeader *) data;
//        mqh->port = port;
//        memcpy(mqh->data, payload, size);
//        return queue.push(data, header_size + size, priority, src, dest, deadline);
//    }

//    virtual int pop(const char * & data, int & priority, int & src){
//        return queue.pop(data, priority, src);
//    }
//};

//class Compression: public Queue {
//protected:
//    struct CompressionHeader{
//      int uncompressed_size;
//      char data[0];
//    };
//    bool Compress(char * in, int in_size, char * out, int & out_len ){
//        uLongf out_size = in_size;
//        int res = compress( (Bytef*) out, &out_size,(Bytef*) in, (uLongf) in_size);
//        out_len = out_size;
//        return res == 0;
//    }

//    bool Uncompress(char * in, int in_size, char * out, int & out_len ){
//        uLongf out_size = out_len;
//        int res = uncompress( (Bytef*) out, &out_size,(Bytef*) in, (uLongf) in_size);
//        out_len = out_size;
//        return res == 0;
//    }
//};

//class Compressor: public Compression {
//    Queue & queue;
//public :
//    Compressor(Queue & q):queue(q){

//    }
//    virtual int push(char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

//        std::cerr << "CMP: " << size << std::endl;


//        int header_size = sizeof(CompressionHeader);
//        char data[header_size + size];
//        CompressionHeader * ch = (CompressionHeader *) data;

//        ch->uncompressed_size = size;
//        if (!Compress(payload, size, ch->data, size)){
//            memcpy(ch->data, payload, size);
//            ch->uncompressed_size = 0;
//        }
//        return queue.push(data, header_size + size, priority, src, dest, deadline);
//    }

//    virtual int pop(const char * & data, int & priority, int & src){
//        return queue.pop(data, priority, src);
//    }
//};

//class Uncompressor: public Compression {
//    Queue & queue;
//public :
//    Uncompressor(Queue & q):queue(q){

//    }
//    virtual int push(char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

//        std::cerr << "UNCMP: " << size << std::endl;


//        CompressionHeader * ch = (CompressionHeader *) payload;
//        if (ch->uncompressed_size > 0){
//            char buff[ch->uncompressed_size];
//            Uncompress(ch->data, size - sizeof(CompressionHeader),buff, ch->uncompressed_size);
//            return queue.push(buff, ch->uncompressed_size, priority, src, dest, deadline);
//        }else{
//            return queue.push(ch->data, size - sizeof(CompressionHeader), priority, src, dest, deadline);
//        }
//    }

//    virtual int pop(const char * & data, int & priority, int & src){
//        return queue.pop(data, priority, src);
//    }
//};

//class Fragmentation: public Queue{
//protected:

//    Queue & q;
//    int frame_size;
//    int header_size;
//    int payload_size;

//    struct FragmentHeader{
//        int frag_id;
//        int msg_size;
//        int msg_uid;
//        char data[0];
//    };
//    Fragmentation(Queue & q, int fs):q(q),frame_size(fs){
//        header_size = sizeof(FragmentHeader);
//        payload_size = frame_size - header_size;
//    }

//    bool Compress(char * in, int in_size, char * out, int & out_len ){
//        uLongf out_size = in_size;
//        int res = compress( (Bytef*) out, &out_size,(Bytef*) in, (uLongf) in_size);
//        out_len = out_size;
//        return res == 0;
//    }

//    bool Uncompress(char * in, int in_size, char * out, int & out_len ){
//        uLongf out_size = out_len;
//        int res = uncompress( (Bytef*) out, &out_size,(Bytef*) in, (uLongf) in_size);
//        out_len = out_size;
//        return res == 0;
//    }
//};


//class Fragmenter: public Fragmentation{
//   int uid;
//public:

//    Fragmenter(Queue & queue, int frame_size):Fragmentation(queue,frame_size){}

//    virtual int push(char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

//        std::cerr << "FRAG: " << size << std::endl;

//        char frag_data[frame_size];
//        FragmentHeader * p = (FragmentHeader *) frag_data;
//        int num_frag = ceil(float(size)/float(payload_size));

//        for (int i = 0; i<num_frag; i++){

//            int this_frag_size = size - i*payload_size > payload_size ? payload_size : size - i*payload_size;

//            p->frag_id = i;
//            p->msg_size = size;
//            p->msg_uid = src << 24 + uid;

//            fprintf(stderr,"-> id:%d frag_size:%d msg_size:%d uid:%d pl_size:%d\n",i, this_frag_size, p->msg_size, p->msg_uid, payload_size);

//            memcpy(&p->data[0], &payload[i*payload_size], this_frag_size);
//            q.push(frag_data, header_size + this_frag_size, priority, src, dest, deadline);
//        }
//        uid++;
//        return 1;
//    }

//    virtual int pop(const char * & data, int & priority, int & src){
//        return q.pop(data,priority, src);
//    }
//};

//class Defragmenter: public Fragmentation{

//   struct Message{
//       int msg_size;
//       int msg_uid;
//       std::vector<char> data;
//       std::vector<char> frags;
//   };

//   std::map<int, Message> map;

//public:

//    Defragmenter(Queue & queue, int frame_size): Fragmentation(queue, frame_size){

//    }

//    virtual int push(char * data, int size, int priority, int src, std::vector<int> dest, int deadline){

//        std::cerr << "DEFG: " << size << std::endl;

//        FragmentHeader * p = (FragmentHeader *) data;
//        int num_frag = ceil(float(p->msg_size)/float(payload_size));
//        int this_frag_size = size - header_size;

//        fprintf(stderr,"<- id:%d frag_size:%d msg_size:%d uid:%d\n",p->frag_id, this_frag_size, p->msg_size, p->msg_uid);

//        Message & m = map[p->msg_uid];
//        m.data.resize(p->msg_size);
//        m.msg_size = p->msg_size;
//        m.frags.resize(num_frag);
//        m.frags[p->frag_id] = 1;
//        memcpy(&m.data[p->frag_id*payload_size], p->data, this_frag_size);

//        int sum = 0;
//        for (int i = 0; i< m.frags.size(); i++){
//            sum += m.frags[i];
//        }

//        if (sum == m.frags.size()){
//            q.push(&m.data[0], m.msg_size, priority, src, dest, deadline);
//        }
//        return 1;
//    }

//    virtual int pop(const char * & data, int & priority, int & src){
//        return q.pop(data,priority, src);
//    }
//};
#endif
