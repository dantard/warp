#ifndef COMPRESSION
#define COMPRESSION

#include "Queue.h"

class Compression: public Queue {
protected:
    struct CompressionHeader{
      int uncompressed_size;
      char data[0];
    };
    bool Compress(char * in, int in_size, char * out, int & out_len ){
        uLongf out_size = in_size;
        int res = compress( (Bytef*) out, &out_size,(Bytef*) in, (uLongf) in_size);
        out_len = out_size;
        return res == 0;
    }

    bool Uncompress(char * in, int in_size, char * out, int & out_len ){
        uLongf out_size = out_len;
        int res = uncompress( (Bytef*) out, &out_size,(Bytef*) in, (uLongf) in_size);
        out_len = out_size;
        return res == 0;
    }
};

class Compressor: public Compression {
    Queue & queue;
public :
    Compressor(Queue & q):queue(q){

    }
    virtual int push(char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

        int header_size = sizeof(CompressionHeader);
        char data[header_size + size];
        CompressionHeader * ch = (CompressionHeader *) data;

        ch->uncompressed_size = size;
        if (!Compress(payload, size, ch->data, size)){
            memcpy(ch->data, payload, size);
            ch->uncompressed_size = 0;
        }
        return queue.push(data, header_size + size, priority, src, dest, deadline);
    }

    virtual int pop(const char * & data, int & priority, int & src){
        return queue.pop(data, priority, src);
    }
};

class Uncompressor: public Compression {
    Queue & queue;
public :
    Uncompressor(Queue & q):queue(q){

    }
    virtual int push(char * payload, int size, int priority, int src, std::vector<int> dest, int deadline){

        CompressionHeader * ch = (CompressionHeader *) payload;
        if (ch->uncompressed_size > 0){
            char buff[ch->uncompressed_size];
            Uncompress(ch->data, size - sizeof(CompressionHeader),buff, ch->uncompressed_size);
            return queue.push(buff, ch->uncompressed_size, priority, src, dest, deadline);
        }else{
            return queue.push(ch->data, size - sizeof(CompressionHeader), priority, src, dest, deadline);
        }
    }

    virtual int pop(const char * & data, int & priority, int & src){
        return queue.pop(data, priority, src);
    }
};

#endif
