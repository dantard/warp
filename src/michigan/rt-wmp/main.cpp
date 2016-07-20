#include "State.h"
#include "Frame.h"
#include "../graph/Matrix.h"
#include "../graph/Dijkstra.h"
#include "../queue/PriorityQueue.h"
#include "../../../utils/Argo.h"
#include "Network.h"
#include "PAP.h"
#include "ATP.h"
#include "MTP.h"




int main(int argc, char *argv[]){
    Argo a;
    int num_of_nodes, node_id;
    a.addInt('n',"num-of-nodes",num_of_nodes,3,"Number of nodes");
    a.addIntMandatory('i',"node-id",node_id,0,"Node id");
    a.process(argc, argv);

    std::cerr << "Node " << node_id << " is starting" << std::endl;

    PriorityQueue q;

    Network n(node_id,num_of_nodes);
    State s(node_id, num_of_nodes, n);
    PAP pap(s);
    ATP atp(s);
    MTP mtp(s);

    if (node_id == 0){
        s.setState(s.STATE_NEW_TOKEN);
    }else{
        s.setState(s.STATE_RECEIVE);
    }

    while (true){
//        std::cerr << "State: " << s.getState() << std::endl;

        if (s.getState() == s.STATE_NEW_TOKEN){
            s.newLoop();
            s.setState(s.STATE_SEND_TOKEN);
        }else if(s.getState() == s.STATE_RECEIVE){

            int type, from, to;
            Frame * f;

            int res = n.receiveFrame(f,type, from, to);
            if (res == 0){
                if (type == Frame::TOKEN){
                    int state = pap.received( (Token*) f , from, to);
                    s.setState(state);
                }else if (type == Frame::AUTH){
                    int state = atp.received( (Auth*) f , from, to);
                    s.setState(state);
                }else if (type == Frame::MESSAGE){
                    int state = mtp.received( (Message*) f , from, to);
                    s.setState(state);
                }
            }

        }else if(s.getState() == s.STATE_SEND_TOKEN){
            std::vector<unsigned char> rssi;
            //s.updateNextLQM();
            Token & t = pap.getToken();
            n.sendFrame(t, pap.getNext());
            s.setState(s.STATE_RECEIVE);
        }else if(s.getState() == s.STATE_SEND_AUTH){
            Auth & t = atp.prepareAuth(s.getMaxPriId());
            int to = atp.getNext(t.getSource(), t.getDest());
            n.sendFrame(t, to);
            s.setState(s.STATE_RECEIVE);
        }else if(s.getState() == s.STATE_SEND_MESSAGE){
            Message & t = mtp.prepareMessage(0);
            int to = mtp.getNext(t.getSource(), t.getDest());
            n.sendFrame(t, to);
            s.setState(s.STATE_RECEIVE);
        }else if(s.getState() == s.STATE_ROUTE_AUTH){
            Auth & t = atp.getAuth();
            int to = atp.getNext(t.getSource(), t.getDest());
            n.sendFrame(t, to);
            s.setState(s.STATE_RECEIVE);
        }else if(s.getState() == s.STATE_ROUTE_MESSAGE){
            Message & t = mtp.getMessage();
            int to = mtp.getNext(t.getSource(), t.getDest());
            n.sendFrame(t, to);
            s.setState(s.STATE_RECEIVE);
        }
        usleep(100000);
    }


}
