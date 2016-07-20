#ifndef MTP_H
#define MTP_H
#include "State.h"

class MTP{
    State & s;
    Message t;
public:

    MTP(State & s):s(s) {


    }

    Message & prepareMessage(int dest){
        t.setLQMUpdateState(s.getLQMUpdateState());
        t.setLQMHash(s.getLQMHash());
        t.setSource(s.getNodeId());
        t.setDest(dest);
    }

    Message & getMessage(){
        t.setLQMUpdateState(s.getLQMUpdateState());
        t.setLQMHash(s.getLQMHash());
        return t;
    }

    int received(Message * f, int from, int to){

        s.setLQMUpdateState(f->getLQMUpdateState());
        s.setLQMHash(f->getLQMHash());

        if (f->getDest() == s.getNodeId()){
            return s.STATE_NEW_TOKEN;
        }else if (to == s.getNodeId()){
            /* Must propagate MESSAGE */
            t.setSource(f->getSource());
            t.setDest(f->getDest());
            return s.STATE_ROUTE_MESSAGE;
        }
        return s.STATE_RECEIVE;
    }


//    int received(Message * f, int from, int to){

//        if (to == s.getNodeId()){
//            s.setLQMUpdateState(f->getLQMUpdateState());
//            s.setLQMHash(f->getLQMHash());
//            //            std::cerr << "RECEIVED MSG from " << +from <<std::endl;
//            return s.STATE_NEW_TOKEN;
//        }
//        return s.STATE_RECEIVE;
//    }

    int getNext(unsigned char source, unsigned int dest){

        if (s.getNodeId() == 0){
            return 1;
        }
        if (s.getNodeId() == 1){
            return 2;
        }
        if (s.getNodeId() == 2){
            return 0;
        }
    }
};

#endif
