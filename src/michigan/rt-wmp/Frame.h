#ifndef FRAME_H
#define FRAME_H
#include <sstream>
#include "Containers.h"

class Frame{
public:
    unsigned char type;
    unsigned short lqm_update_state, lqm_hash;

    enum{TOKEN, AUTH, MESSAGE};

    Frame(){        
    }

    template <class T> int put(char * p, T var){
        memcpy(p, (char*) & var, sizeof(T));
        return sizeof(T);
    }
    template <class T> int get(char * p, T & var){
        memcpy((char*) & var, p, sizeof(T));
        return sizeof(T);
    }


    void setLQMHash(unsigned short val){
        lqm_hash = val;
    }
    unsigned short getLQMHash(){
        return lqm_hash;
    }

    void setLQMUpdateState(unsigned short val){
        lqm_update_state = val;
    }

    unsigned short getLQMUpdateState(){
        return lqm_update_state;
    }

    virtual int deserialize(char * p){
        int k=0;
        k += get<unsigned char> (&p[k], type);
        k += get<unsigned short> (&p[k], lqm_update_state);
        k += get<unsigned short> (&p[k], lqm_hash);
        return k;
    }

    virtual int serialize(char * p){
        int k=0;
        k += put<unsigned char> (&p[k], type);
        k += put<unsigned short> (&p[k], lqm_update_state);
        k += put<unsigned short> (&p[k], lqm_hash);

        return k;
    }

    virtual std::string print(){
        std::ostringstream oss;
        oss << "hash:" << lqm_hash << " LQM upd:"<< lqm_update_state;
        return oss.str();
    }

    int getType(){
         return type;
    }
};

class Token : public Frame{
public:
    signed char max_pri, max_pri_id;
    unsigned char has_lqm;
    unsigned int age;
    LQMat LQM;
    IntVector STATUS;

    Token(int num_of_nodes):Frame(),LQM(num_of_nodes), STATUS(num_of_nodes){
        max_pri = max_pri_id = -1;
        age = 0;
        type = Frame::TOKEN;
        has_lqm = 1;
    }

    bool hasLQM(){
        return has_lqm;
    }

    int serialize(char * p){
        int k = Frame::serialize(p);
        k += put<signed char> (&p[k], max_pri);
        k += put<signed char> (&p[k], max_pri_id);
        k += put<unsigned int> (&p[k], age);
        k += put<unsigned char> (&p[k], has_lqm);
        k += STATUS.serialize(&p[k]);
        if (has_lqm){
            k += LQM.serialize(&p[k]);
        }
        return k;
    }

    int deserialize(char * p){
        int k = Frame::deserialize(p);
        k += get<signed char> (&p[k], max_pri);
        k += get<signed char> (&p[k], max_pri_id);
        k += get<unsigned int> (&p[k], age);
        k += get<unsigned char> (&p[k], has_lqm);
        k += STATUS.deserialize(&p[k]);
        if (has_lqm){
            k += LQM.deserialize(&p[k]);
        }
    }

    virtual std::string print(){
        std::ostringstream oss;
        oss << Frame::print();
        oss <<" max_pri:" << +max_pri << " Max_pri_id:" << +max_pri_id << " Age:" << +age << " STAT:";
        oss << STATUS.toString();
        if (has_lqm){
            oss << " LQM:" << LQM.toString();
        }
        return oss.str();
    }

    LQMat & getLQM(){
        return LQM;
    }
    IntVector & getStatus(){
        return STATUS;
    }
    void setStatus(int id, int val){
        STATUS[id] = val;
    }
    void setHasLQM(int val){
        has_lqm = val;
    }
};


class Auth : public Frame{
public:
    unsigned char src;
    unsigned int  dest;

    Auth(){
       type = Frame::AUTH;
    }
    int serialize(char * p){
        int k = Frame::serialize(p);
        k += put<unsigned char>(&p[k], src);
        k += put<unsigned int> (&p[k], dest);
        return k;
    }
    int deserialize(char * p){
        int k = Frame::deserialize(p);
        k += get<unsigned char>(&p[k], src);
        k += get<unsigned int> (&p[k], dest);
        return k;
    }

    virtual std::string print(){
        std::ostringstream oss;
        oss << Frame::print() <<" Src:" << +src << " Dest:" << dest;
        return oss.str();
    }

    void setSource(unsigned char src){
        this->src = src;
    }
    unsigned char getSource(){
        return src;
    }
    void setDest(unsigned int src){
        this->dest = src;
    }
    unsigned int getDest(){
        return dest;
    }

};
class Message : public Frame{
public:
    unsigned char src;
    unsigned int  dest;

    Message(){
       type = Frame::MESSAGE;
    }
    int serialize(char * p){
        int k = Frame::serialize(p);
        k += put<unsigned char>(&p[k], src);
        k += put<unsigned int> (&p[k], dest);
        return k;
    }
    int deserialize(char * p){
        int k = Frame::deserialize(p);
        k += get<unsigned char>(&p[k], src);
        k += get<unsigned int> (&p[k], dest);
        return k;
    }
    virtual std::string print(){
        std::ostringstream oss;
        oss << Frame::print() <<" Src:" << +src << " Dest:" << dest;
        return oss.str();
    }
    void setSource(unsigned char src){
        this->src = src;
    }
    unsigned char getSource(){
        return src;
    }
    void setDest(unsigned int src){
        this->dest = src;
    }
    unsigned int getDest(){
        return dest;
    }
};
#endif
