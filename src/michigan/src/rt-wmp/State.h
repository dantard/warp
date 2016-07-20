#ifndef STATE_H
#define STATE_H

#include "Containers.h"
#include "../graph/Dijkstra.h"
#include "Network.h"

class State{
    LQMat routingLQM, currentLQM;
    IntVector STATUS;
    Network & n;
    int num_of_nodes, node_id;
    signed char max_pri, max_pri_id;
    unsigned short lqm_update_state, lqm_hash;
public:
    enum nodeState{UNREACHED, REACHED, LOST};
    enum {STATE_RECEIVE, STATE_NEW_TOKEN, STATE_SEND_TOKEN, STATE_SEND_AUTH, STATE_SEND_MESSAGE, STATE_ROUTE_AUTH, STATE_ROUTE_MESSAGE};
    enum {LQM_US_IDLE, LQM_US_FORCE_LOOP, LQM_US_FORCED_LOOP,LQM_US_STORE, LQM_US_DONE};

    int state;

    State(int id, int num_of_nodes, Network & n):routingLQM(num_of_nodes), currentLQM(num_of_nodes), STATUS(num_of_nodes), n(n){
        this->node_id = id;
        this->num_of_nodes = num_of_nodes;
        lqm_update_state = 0;
        srand(node_id);
    }

    void setLQMUpdateState(unsigned short val){
        lqm_update_state = val;
    }
    unsigned short getLQMUpdateState(){
        return lqm_update_state;
    }

    void setLQMHash(unsigned short val){
        lqm_hash = val;
    }
    unsigned short getLQMHash(){
        return lqm_hash;
    }

    int getState(){
        return state;
    }

    void setState(int state){
        this->state = state;
    }

    LQMat & getRoutingLQM(){
        return routingLQM;
    }
    LQMat & getCurrentLQM(){
        return currentLQM;
    }
    IntVector & getStatus(){
        return STATUS;
    }
    int getNumOfNodes(){
        return num_of_nodes;
    }
    int getNodeId(){
        return node_id;
    }

    void setSubstate(int i){

    }
    void setSubstate(int i, int j){

    }

    int getLastIncomingSender(){

    }

    void setMyStatus(int status){
        STATUS[node_id] = status;
    }

    bool isLoopDone(){
        return STATUS.isDone();
    }

    void newLoop(){
        checkAndReplacePriority(-1,-1);
        STATUS.zero();
        STATUS[node_id] = State::REACHED;
    }

    void checkAndReplacePriority(signed char max_pri, signed char max_pri_id){
        signed char my_pri = rand() % 120;
        if (max_pri > my_pri){
            this->max_pri = max_pri;
            this->max_pri_id = max_pri_id;
        }else{
            this->max_pri = my_pri;
            this->max_pri_id = node_id;
        }
    }
    signed char getMaxPri(){
        return max_pri;
    }
    signed char getMaxPriId(){
        return max_pri_id;
    }

    void setMaxPri(){
        this->max_pri = max_pri;
    }
    void setMaxPriId(){
        this->max_pri_id = max_pri_id;
    }

    void updateNextLQM(){
        std::vector<unsigned char> rssi;
        n.getRssi(rssi);
        routingLQM.assignColumn(node_id, rssi);
    }
    void updateCurrentLQM(){
        std::vector<unsigned char> rssi;
        n.getRssi(rssi);
        currentLQM.assignColumn(node_id, rssi);
    }

};


#endif
