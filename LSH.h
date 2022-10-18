#ifndef LSH_H
#define LSH_H
#define HASHTABLE_FACTOR  16
#include <unordered_map>
#include <random>

#include "utils.h"
#include "method.h"
#include "hash.h"



class LSH: public Method{
    private:
        int N; //number of points 
        long int M;
        int L; //number of hashtables
        int k; //number of h_i functions
        int w; //window
        int d; //dimensions->to be extracted from coordinates
        int table_size; //size of hashtable

        //vector<unordered_multimap<unsigned int, Item*>> hash_tables;//structure to hold L hashtables
        vector<HashTable> hash_tables;
        vector<int> r_vec; //vector of random r's used to on g(p) function - random repetition
        //structures to store results
        vector<vector<distance_item*>> N_nearest; //vector(queries) of vectors(pair item and its corresponding distance)  for N nearest neighbors
        vector<vector<distance_item*>> range_nearest; //vector(queries) of vectors(pair item and its corresponding distance) for neighbors in range

    //functions
    public:
        LSH(int arg_k,int arg_L, int arg_N, double arg_r, vector<Item*>& arg_data, vector<Item*>& arg_queries);
        ~LSH();
        int getL();
        int getTableSize();
        vector<int> getR_vec();
        vector<HashTable> getHashTables();
        //void insert_point(Item* point, HashTable table);
        void build_hashTables();
        void multiply_radii();
        vector<distance_item*> find_nearest(Distance_Function dist_func);// returns the nearest neighbor(data) for each query
        vector<vector<distance_item*>> find_N_nearest(Distance_Function dist_func);//retursn N nearest neighbor for each query
        vector<vector<distance_item*>> find_in_range(Distance_Function dist_func);// returns all neighbours(data) inside range r for each query

//         vector<distancePair*> calculate_Nearest(Distance_Function dist_function);
//         // for each query (first vector), returns a vector of its N nearest neighbors
//         vector<vector<distancePair*>> calculate_N_Nearest(Distance_Function dist_function);
//         // for each query (first vector), returns a vector of its nearest neighbors within range r
//         vector<vector<distancePair*>> calculate_in_Range(Distance_Function dist_function);
// }
};

#endif