#ifndef ATP_H
#define ATP_H
#include "State.h"

class ATP{
    State & s;
    Auth t;
public:

    ATP(State & s):s(s) {


    }

    Auth & prepareAuth(int dest){
        t.setLQMUpdateState(s.getLQMUpdateState());
        t.setLQMHash(s.getLQMHash());
        t.setSource(s.getNodeId());
        t.setDest(dest);
    }

    Auth & getAuth(){
        t.setLQMUpdateState(s.getLQMUpdateState());
        t.setLQMHash(s.getLQMHash());
        return t;
    }

    int received(Auth * f, int from, int to){

        s.setLQMUpdateState(f->getLQMUpdateState());
        s.setLQMHash(f->getLQMHash());

        if (f->getDest() == s.getNodeId()){
            return s.STATE_SEND_MESSAGE;
        }else if (to == s.getNodeId()){
            /* Must propagate AUTH */
            t.setSource(f->getSource());
            t.setDest(f->getDest());
            return s.STATE_ROUTE_AUTH;
        }
        return s.STATE_RECEIVE;
    }

    int getNext(unsigned char source, unsigned int dest){
//        IntVector v;
//        double cost;
//        s.getRoutingLQM().computePaths(source);

//        if (s.getRoutingLQM().getMostProbablePath(dest, v, cost)){
//            std::cerr << "Src: " << +source << " dest:" << dest << " Found path, cost:" << cost << std::endl;
//            v.print();
//            return v.at(1);
//        }else{
//            std::cerr << "ERRRORRRR" << std::endl;
//            return -1;
//        }

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
