#ifndef FRAGMENTATION
#define FRAGMENTATION

#include "Queue.h"
#include <math.h>

class Fragmentation: public Queue{
protected:

    Queue & q;
    int frame_size;
    int header_size;
    int payload_size;

    struct FragmentHeader{
        int frag_id;
        int msg_size;
        int msg_uid;
        char data[0];
    };
    Fragmentation(Queue & q, int fs):q(q),frame_size(fs){
        header_size = sizeof(FragmentHeader);
        payload_size = frame_size - header_size;
    }
};

class Fragmenter: public Fragmentation{
   int uid;
public:

    Fragmenter(Queue & queue, int frame_size):Fragmentation(queue,frame_size){}

    virtual int push(char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

        std::cerr << "FRAG: " << size << std::endl;

        char frag_data[frame_size];
        FragmentHeader * p = (FragmentHeader *) frag_data;
        int num_frag = ceil(float(size)/float(payload_size));


        std::cerr << "FRAG: " << size << "NF:" << num_frag << std::endl;


        for (int i = 0; i<num_frag; i++){

            int this_frag_size = size - i*payload_size > payload_size ? payload_size : size - i*payload_size;

            p->frag_id = i;
            p->msg_size = size;
            p->msg_uid = src << 24 + uid;

            fprintf(stderr,"-> id:%d frag_size:%d msg_size:%d uid:%d pl_size:%d\n",i, this_frag_size, p->msg_size, p->msg_uid, payload_size);

            memcpy(&p->data[0], &payload[i*payload_size], this_frag_size);
            q.push(frag_data, header_size + this_frag_size, priority, src, dest, deadline);
        }
        uid++;
        return 1;
    }

    virtual int pop(const char * & data, int & priority, int & src){
        return q.pop(data,priority, src);
    }
};

class Defragmenter: public Fragmentation{

   struct Message{
       int msg_size;
       int msg_uid;
       std::vector<char> data;
       std::vector<char> frags;
   };

   std::map<int, Message> map;

public:

    Defragmenter(Queue & queue, int frame_size): Fragmentation(queue, frame_size){

    }

    virtual int push(char * data, int size, int priority, int src, std::vector<int> dest, int deadline){

        std::cerr << "DEFG: " << size << std::endl;

        FragmentHeader * p = (FragmentHeader *) data;
        int num_frag = ceil(float(p->msg_size)/float(payload_size));
        int this_frag_size = size - header_size;

        fprintf(stderr,"<- id:%d frag_size:%d msg_size:%d uid:%d\n",p->frag_id, this_frag_size, p->msg_size, p->msg_uid);

        Message & m = map[p->msg_uid];
        m.data.resize(p->msg_size);
        m.msg_size = p->msg_size;
        m.frags.resize(num_frag);
        m.frags[p->frag_id] = 1;
        memcpy(&m.data[p->frag_id*payload_size], p->data, this_frag_size);

        int sum = 0;
        for (int i = 0; i< m.frags.size(); i++){
            sum += m.frags[i];
        }

        if (sum == m.frags.size()){
            q.push(&m.data[0], m.msg_size, priority, src, dest, deadline);
        }
        return 1;
    }

    virtual int pop(const char * & data, int & priority, int & src){
        return q.pop(data,priority, src);
    }
};
#endif
