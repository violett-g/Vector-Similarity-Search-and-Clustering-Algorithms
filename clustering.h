#ifndef CLUSTERING_H
#define CLUSTERING_H
#include <unordered_map>
#include <random>
#include <vector>
#include <math.h>
#include "utils.h"
#include "method.h"
#include "item.h"
#include "LSH.h"


// #define K 3

 typedef pair<bool,Item*> is_point_clustered;

class Cluster {
    private:
        int id; //cluster number - from 1 to k
        Item* centroid;
        vector<Item*> clustered_points;
        Item* prev_centroid;
        //maybe replace with pair tuple (vector<distitem*>)
    public:
        Cluster();
        ~Cluster();
        int getId();
        Item* getCentroid();
        vector<Item*> getClusteredPoints();
        Item* getPrevCentroid();
        void setId(int);
        void setCentroid(Item *);
        void setClusteredPoints(vector<Item *>);
        void setPrevCentroid(Item*);
        void printCluster();

};


class Algorithm {
    private:
    string algorithm;
    public:
    Algorithm(); 
    ~Algorithm(); 
    void initialize(vector<Item *>, vector<Cluster>, int);
    //not implemented here, but must be implemented in children classes
    virtual void addtoCluster(Item*, vector<Cluster>, int); 
    virtual void reCenter(vector<Cluster>, int);
    bool converge(vector<Cluster>, int);
    virtual void execAlgorithm(vector<Item*>, vector<Cluster>, int);
};


class Lloyd:public Algorithm {
    public:
    Lloyd(); //call algorithm constructor
    ~Lloyd(); //call algorithm destructor
    void addtoCluster(Item*, vector<Cluster>, int); 
    void reCenter(vector<Cluster>, int);
    // bool converge();
    void execAlgorithm(vector<Item*>, vector<Cluster>, int);
};

class RangeSearchLSH:public Algorithm {
  
    private:
        //lsh includes L hash tables
        LSH lsh;
        vector<is_point_clustered*> already_assigned; 
    public:
    RangeSearchLSH(); //call algorithm constructor
    ~RangeSearchLSH(); //call algorithm destructor
    void addtoCluster(Item*, vector<Cluster>, int); 
    void reCenter(vector<Cluster>, int); //same method used in lloyd?
    virtual void execAlgorithm(vector<Item*>, vector<Cluster>, int); //hash items during this
    vector<Item*> rangeSearch(Item*, float);
};


// class RangeSearchCube:Algorithm {
//     private:
//         typedef pair<bool,Item*> already_assigned;
//     public:
//     RangeSearchCube(); //call algorithm constructor
//     ~RangeSearchCube(); //call algorithm destructor
//     void addtoCluster(Item* point);
//     void reCenter(); //same method used in lloyd?
//     void execAlgorithm(vector<Item*> points); //hash items during this
//     void rangeSearch();
// };


class Clustering {
    private:
    int K;
    vector<Cluster> clusters;
    Algorithm algorithm_used;
    // Exhaustive exhaustive; //lloyds: non centroids as queries
    public:
    Clustering(Algorithm, int); //includes centers first initializing and min distance calculation
    ~Clustering();
    int getK();
    void execClustering(vector<Item*>, int);
    Cluster* getClusters();
    Algorithm getAlgUsed();
};





#endif