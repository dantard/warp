#include "../graph/Matrix.h"
#include "../graph/Dijkstra.h"
#include "../rt-wmp/Frame.h"

#ifndef CONTAINER_H
#define CONTAINER_H

class IntVector : public Row<unsigned char>{

public:
    IntVector(int size):Row<unsigned char>(size){

    }
    IntVector():Row<unsigned char>(){

    }
    bool isDone(){
        int sum = 0;
        for (int i = 0; i < size(); i++){
            sum += at(i) > 0 ? 1 : 0;
        }
        return sum == size();
    }
    void replace(IntVector v){
         for (int i = 0; i < v.size(); i++){
             at(i) = v.at(i);
         }
    }
    void zero(){
        for (int i = 0; i < size(); i++){
            at(i) = 0;
        }
    }
};

class FloatVector : public Row<float>{
public:
    FloatVector(int size):Row<float>(size){

    }
};

class LQMat : public Matrix<unsigned char>{
    Dijkstra<double> * dij;
    Matrix<double> * helper;
public:
    LQMat(int size):Matrix<unsigned char>(size){
        helper = new Matrix<double>(size);
        dij = new Dijkstra<double>(*helper);
    }

    void computePaths(int src){
        helper->assign(*this);
        helper->cut(99).multiply(0.01).log10().multiply(-1);
        dij->calculateDistance(src);
    }

    bool getMostProbablePath(int dest,  IntVector & v, double & cost){
        v.clear();
        if (dij->getPath(dest, v)){
            cost = 1;
            for (int i = 0; i < v.size() - 1; i++){
                cost = cost * double(at(v[i],v[i+1])) / 100.0;
            }
            return true;
        }
        return false;
    }

    double getPathProbability(int dest){
        IntVector v;
        if (dij->getPath(dest, v)){
            double cost = 1;
            for (int i = 0; i < v.size() - 1; i++){
                cost = cost * double(at(v[i],v[i+1])) / 100.0;
            }
            return cost;
        }
        return -1;
    }
    void setRssi(int node_id, FloatVector & rssi){
        replaceRow(node_id, rssi);
    }
    unsigned short getHash(){
        unsigned short hash = 1;
        for (int i = 0; i< size(); i++){
            for (int j = 0; j < size(); j++){
              if (at(i,j)!=0){
                  hash *= at(i,j);
              }
            }
        }
        return (unsigned short) hash;
    }
    Matrix & print(const char * name = 0){
        Matrix::print(name);
        std::cerr << "HASH: " << getHash() << std::endl;
    }


};


#endif
