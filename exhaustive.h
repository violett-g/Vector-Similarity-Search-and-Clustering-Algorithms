#ifndef EXHAUSTIVE_H
#define EXHAUSTIVE_H

#include <vector>
#include "method.h"

class Exhaustive: public Method{
    private:

        vector<vector<distance_item*>> all_distances; // vector of vectors that stores the distance of each data from each query (vector of queries,for each query a vector with the pairs distance item)
        void calculate_all_distances(Distance_Function distance_Function);// calculates all distances of each data from each query
        vector<vector<distance_item*>> all_distances_data; // for each data stores the distances of the other data (used for w calculation)

    public:
        //vector<vector<distance_item*>> all_distances; // vector of vectors that stores the distance of each data from each query (vector of queries,for each query a vector with the pairs distance item)
        Exhaustive(int arg_k, int arg_N, double arg_r, vector<Item*>& arg_data, vector<Item*>& arg_queries);
        ~Exhaustive();
        vector<distance_item*> find_nearest(Distance_Function dist_func);// returns the nearest neighbor(data) for each query
        vector<vector<distance_item*>> find_N_nearest(Distance_Function dist_func);//retursn N nearest neighbor for each query
        vector<vector<distance_item*>> find_in_range(Distance_Function dist_func);// returns all neighbours(data) inside range r for each query
        vector<distance_item*> sort_distances(vector<distance_item*> distances);
        void calculate_all_distances_data(Distance_Function dist_func);

};

#endif