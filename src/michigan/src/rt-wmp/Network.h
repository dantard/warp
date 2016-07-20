#ifndef NETWORK_H
#define NETWORK_H

#include "Frame.h"
#include "../network/layer-1.h"
class Rssi{
    int average;
public :
    Rssi(){

    }
    void newValue(int value){

    }
    int getAverage(){
        return average;
    }
    void fake(int v){
        average = v;
    }
};


class Network{
    unsigned int serial;
    bool available_in_rx_buffer;
    int node_id, num_of_nodes;
    std::vector<Rssi> rssi;
public:
    struct NetHeader{
        unsigned char type, retries, from,to;
        unsigned int serial;
        char payload[0];
    };
    Network(int node_id, int num_of_nodes){
        this->node_id = node_id;
        this->num_of_nodes = num_of_nodes;
        sock_raw_init("lo", 0x5454);
        serial = 0;
        rssi.resize(num_of_nodes);
        available_in_rx_buffer = false;
        for (int i = 0 ; i< rssi.size(); i++){
            if (i!=node_id){
                rssi[i].fake(node_id+30);
            }else{
                rssi[i].fake(0);
            }
        }
    }

    int getRssi(int i){
        return rssi[i].getAverage();
    }

    void getRssi(std::vector<unsigned char> & v) {
        v.clear();
        for (int i = 0 ; i< rssi.size(); i++){
            v.push_back(rssi[i].getAverage());
        }
    }

    int sendFrame(Frame & t, int to){
        char * q = getRxBuffer();
        char * p = getTxBuffer();

        NetHeader * nep = (NetHeader *)p;
        NetHeader * neq = (NetHeader *)q;
        nep->from = node_id;
        nep->to = to;

        nep->serial = ++serial;
        nep->type   = t.getType();
        nep->retries = 3;
        int size = t.serialize(&nep->payload[0]);
        while (nep->retries > 0){

            send(size + sizeof(NetHeader));
            nep->retries = nep->retries - 1;

            int mine = 1, size;
            while (mine){
                size = receive(1000);
                mine = size > 0 && neq->from == node_id;
            }

//            std::cerr <<"TX Serial:" << nep->serial << std::endl;

            if (size > 0 && checkValid(neq)){
                if (neq->to == node_id){
                    available_in_rx_buffer = size;
                }
//                std::cerr << "ACKED!" << std::endl;
                return true;
            }
        }
        return false;
    }

    bool checkValid(NetHeader * ne){
        if (ne->serial > serial){
            return true;
        }
    }

    int receiveFrame(Frame * & frame, int & type, int & from, int &to, int timeout = 0){
        char * p = getRxBuffer();
        NetHeader * ne = (NetHeader *)p;

        int size = available_in_rx_buffer;

        if (size == 0){
            size = receive(timeout);
            if (size < 0){
                return -1;
            }
        }

        available_in_rx_buffer = 0;

        type = ne->type;
        serial = ne->serial;
        from = ne->from;
        to = ne->to;

        if (type == Frame::TOKEN){
            frame = new Token(num_of_nodes);
            frame->deserialize(&ne->payload[0]);
//            std::cerr <<"RX Serial:" << ne->serial << std::endl;
        }else if (type == Frame::AUTH){
            frame = new Auth();
            frame->deserialize(&ne->payload[0]);
        }else if (type == Frame::MESSAGE){
            frame = new Message();
            frame->deserialize(&ne->payload[0]);
        }
        if (rand() % 25 == 0){
//            std::cerr << "Cambioooo" << std::endl;
            rssi[ne->from].fake(20+rand()%20);
        }
        return 0;
    }

};

#endif
