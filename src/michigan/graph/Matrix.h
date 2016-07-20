#ifndef MATRIX_H_
#define MATRIX_H_

#include <iomanip>
#include <string.h>
#include <vector>
#include <limits>
#include <iostream>
#include <cmath>
#include <assert.h>
#include <sstream>

#define  FOR_IJ   for (int i = 0; i< this->size1(); i++){ for (int j = 0; j < this->size2(); j++){
#define  END_FOR_IJ }}

template <class T> class Row : public std::vector<T> {

public:
    Row(int size):std::vector<T>(size){

    }
    Row():std::vector<T>(){

    }
    template <class Q> int serialize(Q * p){
        for (int i = 0; i< this->size(); i++){
            p[i] = this->at(i);
        }
        return this->size();
    }

    template <class Q> int deserialize(Q * p){
        for (int i = 0; i< this->size(); i++){
            this->at(i) = p[i];
        }
        return this->size();
    }

    template <class Q> Row & multiply (Row<Q> x){
        for (int i = 0; i< this->size(); i++){
            this->at(i)*=x.at(i);
        }
        return *this;

    }
    template <class Q> Row & sum (Row<Q> x){
        for (int i = 0; i< this->size(); i++){
            this->at(i)+=x.at(i);
        }
        return *this;

    }
    template <class Q> Row & multiply (Q x){
        for (int i = 0; i< this->size(); i++){
            this->at(i)*=x;
        }
        return *this;

    }
    template <class Q> Row & sum (Q x){
        for (int i = 0; i< this->size(); i++){
            this->at(i)+=x;
        }
        return *this;
    }

    void print(const char * name = 0){
        fprintf(stderr, "\n");
        if (name!=0){
            std::cerr << name << ":" << std::endl;
        }
        for (int i = 0; i< this->size(); i++){
            fprintf(stderr, "%5d ", this->at(i));
        }
        fprintf(stderr, "\n");
    }
    std::string toString( ){
        std::ostringstream oss;
        for (int i = 0; i< this->size(); i++){
            oss << +this->at(i) <<" ";
        }
        return oss.str();
    }


};

template <class T> class Matrix{

protected:
    T ** a;
    int c;

public:
    Matrix(int c){
        init(c);
    }
    Matrix(){
        c = 0;
    }
    template <class Q> Matrix(Matrix<Q> & m){
        assign(m);

    }

    void init(int c){
        a = new T*[c];
        for (int i = 0; i< c; i++){
            a[i] = new T[c];
        }
        this->c = c;
        clear();
    }

    Matrix & randomize(){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
               a[i][j] = rand()% std::numeric_limits<T>::max();
            }
        }
        return *this;
    }

    bool equals(Matrix<T> & m, T threshold = 0){
        int sum = 0;
        for (int i = 0; i< size(); i++){
            for (int j = 0; j < size(); j++){
               sum += ((at(i,j) - m(i,j))>= -threshold) && ((at(i,j) - m(i,j)) <= threshold) ? 1:0;
            }
        }
        return sum == size()*size();
    }

    Matrix & symmetrizeAvg(T perc_max=50, T perc_min=50){
        for (int i = 0; i< size(); i++){
            for (int j = i; j < size(); j++){
                T min = a[i][j] < a[j][i]? a[i][j] : a[j][i];
                T max = a[i][j] > a[j][i]? a[i][j] : a[j][i];
                a[i][j] = (T)(perc_min*min + perc_max*max)/(T) 100;
            }
        }
        return *this;
    }


    Row<T> getRow(int j){
        Row<T> row;
        for (int i = 0; i < c; i++){
            row.push_back(a[i][j]);
        }
        return row;
    }

    Row<T> getColumn(int j){
        Row<T> row;
        for (int i = 0; i < c; i++){
            row.push_back(a[j][i]);
        }
        return row;
    }

    Matrix& log10(){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
               if (a[i][j] > 0){
                   a[i][j] = log(a[i][j]);
               }
            }
        }
        return *this;
    }

    template <class Q> void assign(Matrix<Q> & m){
        if (c == 0){
            init(m.size());
        }
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
               a[i][j] = (T) m(i,j);
            }
        }
    }

    template <class Q> Matrix & sum (Q x){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
                a[i][j]+=x;
            }
        }
        return *this;
    }

    template <class Q> Matrix & multiply (Q x){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
                if (a[i][j]!=0){
                    a[i][j]*=x;
                }
            }
        }
        return *this;
    }

    template <class Q> Matrix & sum (Matrix<Q> & x){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
                if (a[i][j]!=0){
                    a[i][j]+=x.at(i,j);
                }
            }
        }
        return *this;
    }
    template <class Q> Matrix & subtractFrom (Q x){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
                if (a[i][j]!=0){
                    a[i][j]= x - a[i][j];
                }
            }
        }
        return *this;
    }
    template <class Q> Matrix & multiply (Matrix<Q> & x){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
                a[i][j]*=x.at(i,j);
            }
        }
        return *this;
    }
    template <class Q> Matrix & cut (Q x){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
                if (a[i][j] > x){
                    a[i][j] = x;
                }
            }
        }
        return *this;
    }

    template <class Q> Matrix & operator+= (Matrix<Q> & x){
        this->sum(x);
    }

    template <class Q> Matrix & operator*= (Matrix<Q> & x){
        this->multiply(x);
    }
    template <class Q> Matrix & replaceRow(int r, Row<Q> & x){
        assert(x.size()==this->size());
        for (int i = 0; i<this->size(); i++){
            a[r][i] = (T) x[i];
        }
        return *this;
    }
    template <class Q> Matrix & replaceColumn(int c, Row<Q> & x){
        assert(x.size()==this->size());
        for (int i = 0; i<this->size(); i++){
            a[i][c] = (T) x[i];
        }
    }

    T& operator()(int row, int col){
        return a[row][col];
    }
    Matrix& operator()(int row, int col, T val, bool sym){
        return put(row,col,val,sym);

    }
    void assignColumn(int id, std::vector<T> data){
        for (int i = 0; i<this->size(); i++){
            this->at_element(i, id) = data[i];
        }
    }
    void assignRow(int id, std::vector<T> data){
        for (int i = 0; i<this->size(); i++){
            this->at_element(id, i) = data[i];
        }
    }

    template <class Q> Q getNeighbors(int vertex){
        Q neighbors(size());
        for (int i = 0; i < this->size(); i++){
            if (this->at_element(i,vertex)>0){
                neighbors[i] = 1;
            }else{
                neighbors[i] = 0;
            }
        }
    }

    Matrix & put(int i, int j, T val, bool symmetric = false){
        a[i][j] = val;
        if (symmetric){
            a[j][i] = val;
        }
        return *this;
    }

    T & at(int i, int j){
        return a[i][j];
    }

    T & at_element(int i, int j){
        return at(i,j);
    }

    int size(){
        return c;
    }

    Matrix & inverse(){
        for (int i = 0; i< c; i++){
            for (int j = 0; j< c; j++){
                if (a[i][j]!=0){
                    a[i][j]=1.0/a[i][j];
                }
            }
        }
        return *this;
    }

    Matrix & print(const char * name = 0){
        std::cerr.precision(2);
        fprintf(stderr, "\n");
        if (name!=0){
            std::cerr << name << ":" << std::endl;
        }
        fprintf(stderr, "      ");
        for (int i = 0; i< size(); i++){
            fprintf(stderr, "%5d ", i);
        }
         fprintf(stderr, "\n    -");
        for (int i = 0; i< size(); i++){
            fprintf(stderr, "------");
        }
        fprintf(stderr, "\n");
        for (int i = 0; i< size(); i++){
            fprintf(stderr, "%3d |", i);
            for (int j = 0; j < size(); j++){
                std::cerr << std::setw(6) << +this->at_element(i,j);
            }
            fprintf(stderr, "\n");
        }
        return * this;
    }

    std::string toString( ){
        std::ostringstream oss;
        for (int i = 0; i< size(); i++){
            for (int j = 0; j < size(); j++){
               oss << +this->at_element(i,j) << " ";
            }
        }
        return oss.str();
    }

    void symmetrize(){
        for (int i = 0; i< size(); i++){
            for (int j = i; j < size(); j++){
                a[i][j] = a[i][j] < a[i][j]? a[i][j] : a[i][j];
            }
        }
    }

    template <class Q> int serialize(Q * p){
        int k = 0;
        for (int i = 0; i< this->size(); i++){
            for (int j = 0; j < this->size(); j++){
                if (i!=j){
                    p[k] = (Q) this->at(i,j);
                    k = k + 1;
                }
            }
        }
        return k;
    }
    template <class Q> int deserialize(Q * p){
        int k = 0;
        for (int i = 0; i< this->size(); i++){
            for (int j = 0; j < this->size(); j++){
                if (i!=j){
                    this-> at(i,j) = (T) p[k];
                    k = k + 1;
                }
            }
        }
        return k;
    }


    bool makePrim(){
        char done[c];
        T min;
        int x,y, found, ne = 0;

        memset(done, 0, c);
        done[0] = 1;

        std::vector<std::pair<int,int> >prim;
        while (ne < c - 1){
            found = 0;
            for (int i = 0; i< size(); i++){
                if (done[i]){
                    min = std::numeric_limits<T>::max();
                    for (int j = 0; j < size(); j++){
                        if (!done[j]){
                            if (a[i][j] > 0 && min > a[i][j]){
                                min = a[i][j];
                                x=i;
                                y=j;
                                found = 1;
                            }
                        }
                    }
                }
            }
            if (found){
                ne += 1;
                done[y] = 1;
                prim.push_back(std::pair<int,int>(x,y));
            }else{
                return false;
            }
        }

        clear();
        for (int i = 0; i< prim.size(); i++){
            this->put(prim[i].first, prim[i].second,1,true);
        }
        return true;
    }

    Matrix&clear(){
        for (int i = 0; i< size(); i++){
            for (int j = 0; j < size(); j++){
                a[i][j] = 0;
            }
        }
    }

    ~Matrix(){
        for (int i = 0; i< c; i++){
            free(a[i]);
        }
        free(a);
    }
};




#endif



