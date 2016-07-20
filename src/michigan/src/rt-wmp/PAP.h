#ifndef PAP_H
#define PAP_H
#include "State.h"
class PAP{
    State & s;
    Token t;
public:

    PAP(State & s):s(s), t(Token(s.getNumOfNodes())){


    }

    Token & getToken(){
        t.max_pri =  s.getMaxPri();
        t.max_pri_id =  s.getMaxPriId();
        t.getStatus().replace(s.getStatus());
        t.getLQM().assign(s.getCurrentLQM());

        t.setLQMUpdateState(s.getLQMUpdateState());
        t.setLQMHash(s.getLQMHash());

        if (s.getLQMUpdateState() == 0){
            t.setHasLQM(0);
        }else{
            t.setHasLQM(1);
        }
        return t;
    }

    int received(Token * f, int from, int to){
        if (to == s.getNodeId()){

            s.getStatus().replace(f->getStatus());
            s.setMyStatus(s.REACHED);
            s.checkAndReplacePriority(f->max_pri, f->max_pri_id);

            /* I propagate the LQMUpdateState and LQMHash (which is the hash of the routing lqm being used */
            s.setLQMUpdateState(f->getLQMUpdateState());
            s.setLQMHash(f->getLQMHash());

            /* if they ask me to store, I store the LQM as routing one */
            if (f->getLQMUpdateState() == State::LQM_US_STORE){
                s.getRoutingLQM().assign(f->getLQM());
                std::cerr << "Storing hash" << s.getRoutingLQM().getHash() << std::endl;
            }

            if (f->hasLQM()){
                /* Also I assume the received LQM as current (shared) LQM */
                s.getCurrentLQM().assign(f->getLQM());
            }

            /* Update with the new RSSI */
            s.updateCurrentLQM();

            /* If I have had some changes in my RSSIs or Routing LQM does coincide with current, ask for a forced loop to make other know */
            if (f->getLQMUpdateState() == State::LQM_US_IDLE){
                if (s.getCurrentLQM().getHash() != f->getLQMHash()
                        || s.getRoutingLQM().getHash() != f->getLQMHash()){
                    s.setLQMUpdateState(State::LQM_US_FORCE_LOOP);
                }
            }

            if (s.isLoopDone()){

                /* If someone has asked to force a loop, force new toke */
                if (f->getLQMUpdateState() == State::LQM_US_FORCE_LOOP){
                    s.setLQMUpdateState(State::LQM_US_FORCED_LOOP);
                    return s.STATE_NEW_TOKEN;

                /* If I am the last on a forced loop, ask to store the LQM */
                }else if (f->getLQMUpdateState() == State::LQM_US_FORCED_LOOP){
                    std::cerr << "Ask to store" << std::endl;
                    s.setLQMUpdateState(State::LQM_US_STORE);
                    s.getRoutingLQM().assign(f->getLQM());
                    std::cerr << "Storing hash" << s.getRoutingLQM().getHash() << std::endl;
                    return s.STATE_NEW_TOKEN;

                /* If I am the last on a store-request loop, return state to IDLE */
                }else if (f->getLQMUpdateState() == State::LQM_US_STORE){
                    std::cerr << "Store done" << std::endl;
                    s.setLQMUpdateState(State::LQM_US_IDLE);
                    s.getRoutingLQM().assign(f->getLQM());
                    s.setLQMHash(s.getCurrentLQM().getHash());
                    s.getRoutingLQM().print("STORE DONE");
                }

                signed char mpm_holder = s.getMaxPriId();
                if (mpm_holder == -1){
                    return s.STATE_NEW_TOKEN;
                }if (mpm_holder == s.getNodeId()){
                    return s.STATE_NEW_TOKEN;
                }else{
                    return s.STATE_SEND_AUTH;
                }
            }
            return s.STATE_SEND_TOKEN;
        }
    }

    int getNext(){

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
