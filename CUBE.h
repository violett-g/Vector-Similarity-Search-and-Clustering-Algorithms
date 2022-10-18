#ifndef CUBE_H
#define CUBE_H

#include "method.h"
#include "hash.h"
#include "utils.h"

#include <random>
#include <unordered_map>

class CUBE: public Method{
    private:
        int k;
        int w;
        int d;
        int N;
        int M; // max number of points to be checked
        int probes;
        int table_size; //size of hashtable

        HashTable* hash_table; // hashtable to store items
        
        //structures to store results
        vector<vector<distance_item*>> N_nearest; //vector(queries) of vectors(pair item and its corresponding distance)  for N nearest neighbors
        vector<vector<distance_item*>> range_nearest; //vector(queries) of vectors(pair item and its corresponding distance) for neighbors in range
    public:
        CUBE(int arg_k, int arg_probes, int arg_M, int arg_N, double arg_r, vector<Item*>& arg_data, vector<Item*>& arg_queries);
        //CUBE(int arg_k, int arg_probes,int arg_M, int arg_N, double arg_r, vector<Item*>& arg_data, vector<Item*>& arg_queries);
        ~CUBE();

        void build_hashTable();
        void multiply_radii();
        vector<distance_item*> find_nearest(Distance_Function dist_func);// returns the nearest neighbor(data) for each query
        vector<vector<distance_item*>> find_N_nearest(Distance_Function dist_func);//retursn N nearest neighbor for each query
        vector<vector<distance_item*>> find_in_range(Distance_Function dist_func);// returns all neighbours(data) inside range r for each query

}; 
#endif