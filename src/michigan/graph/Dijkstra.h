#include <iostream>
#include "Matrix.h"
#ifndef DIJKSTRA_H
#define DIJKSTRA_H
using namespace std;

template <class T> class Dijkstra{
private:
    Matrix<T> & adjMatrix;
    Row<int> predecessor;
    Row<T> distance;
    Row<char> mark;

    int source;
    int numOfVertices;
public:

    Dijkstra(Matrix<T> & m): adjMatrix(m), predecessor(m.size()), distance(m.size()), mark(m.size()){
        numOfVertices = m.size();
    }
    void set(Matrix<T> & m){
        adjMatrix = m;

    }
    void initialize(){
        for(int i=0;i<numOfVertices;i++) {
            mark[i] = false;
            predecessor[i] = -1;
            distance[i] = std::numeric_limits<T>::max();
        }
        distance[source]= 0;
    }

    int getClosestUnmarkedNode(){
        T minDistance = std::numeric_limits<T>::max();
        int closestUnmarkedNode;
        for(int i=0;i<numOfVertices;i++) {
            if((!mark[i]) && ( minDistance >= distance[i])) {
                minDistance = distance[i];
                closestUnmarkedNode = i;
            }
        }
        return closestUnmarkedNode;
    }

    void calculateDistance(int source){
        this->source = source;
        initialize();
        //int minDistance = std::numeric_limits<T>::max();
        int closestUnmarkedNode;
        int count = 0;
        while(count < numOfVertices) {
            closestUnmarkedNode = getClosestUnmarkedNode();
            mark[closestUnmarkedNode] = true;
            for(int i=0;i<numOfVertices;i++) {
                if((!mark[i]) && (adjMatrix(closestUnmarkedNode,i)>0) ) {
                    if(distance[i] > distance[closestUnmarkedNode]+adjMatrix(closestUnmarkedNode,i)) {
                        distance[i] = distance[closestUnmarkedNode]+adjMatrix(closestUnmarkedNode,i);
                        predecessor[i] = closestUnmarkedNode;
                    }
                }
            }
            count++;
        }
    }

    template <class Q> bool getPath(int dest, Row<Q> & vec){
        if(dest == source){
            vec.push_back(dest);
        }else if(predecessor[dest] == -1){
            return false;
        } else {
            this->getPath(predecessor[dest], vec) ;
            vec.push_back(dest);
        }
        return true;
    }
    double getCost(int dest){
        return (double) distance[dest];
    }

};
#endif

//int main(){
//    Dijkstra G;
//    G.read();
//    G.calculateDistance();
//    G.output();
//    G.printPath(2);
//    return 0;
//}

