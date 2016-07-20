#include "State.h"
#include "Frame.h"
#include "../graph/Matrix.h"
#include "../graph/Dijkstra.h"
#include "../queue/PriorityQueue.h"
#include "../../../utils/Argo.h"
#include "Network.h"
//#include "../network/layer-1.h"

int main(int argc, char *argv[]){
    Argo a;
    int num_of_nodes, node_id;
    a.addInt('n',"num-of-nodes",num_of_nodes,3,"Number of nodes");
    a.addIntMandatory('i',"node-id",node_id,0,"Node id");
    a.process(argc, argv);

    std::cerr << "Sniffer is starting" << std::endl;

    Network n(node_id, num_of_nodes);
    State s(node_id, num_of_nodes, n);

    char * p = getRxBuffer();
    Network::NetHeader * ne = (Network::NetHeader *) p;
    while (true){

        Frame * f;
        int type, from,to;
        n.receiveFrame(f,type, from, to);
        std::cerr<< "F:" << +from << " T:" << +to << " Serial:" << ne->serial << " Type:"<< type << " " <<f->print() << std::endl;
    }


}
