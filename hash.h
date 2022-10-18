#ifndef HASH_H
#define HASH_H
#define HASHTABLE_FACTOR  16

#include <unordered_map>
#include <vector>
#include "item.h"
#include "input.h"

using namespace std;

class HashTable {
    private:
        // int table_size; //size of hashtable
        int N; 
        long int M;
        int L; //number of hashtables
        int k; //number of h_i functions
        int w; //window
        int d; //dimensions->to be extracted from coordinates
        //int table_size; //size of hashtable
       
        vector<vector<float>> v_vec; //vector of random v's used on h(p) function - each v is a d-vector
        vector<float> t_vec; // vector of random t's used on h(p) function - random displacement 
        vector<unordered_map<int, int>> f_map; // structure that maps an h function to 0 or 1 (HyperCube)


        //AmplifiedHashFunction gFunction;
        //vector<vector<Item*>> buckets;
    public:
        unordered_multimap<unsigned int, Item*> hash_table;// hashtables for LSH, maps a point to a g(p)
        unordered_multimap<string, Item*> cube_hash_table; //hashtable for hypercube, maps a point to an f function = bucket number

        HashTable( int arg_k, int arg_N, int arg_d);
        ~HashTable();
        int calculate_h_i(Item* point, vector<float> v_vec, float t); //function to calculate h_i(p) function
        
        long int calculate_g(Item* point, vector<int> r_vec); //function to calculate g(p) function
        void insert_point_LSH(Item* point, vector<int> r_vec, int table_size);
        
        string calculate_f(Item* point);
        void insert_point_CUBE(Item* point);   
        void Hamming_distance(string vertex,int length, int hamming_distance, vector<string>& vertices);
        vector<string> find_neighbors(string vertex, int probes);
        
        //void Hamming_distance(string vertex,int length, int hamming_distance, vector<string>& vertices);


        
        void print_HT();
        // auto find_points(unsigned int g);
        //Item* point searchNN(Item* queryPoint);
};

// class AmplifiedHashFunction {
//     private:
//     int k; //number of hi functions
//     HashFunction hi[k];

//     public:
//     AmplifiedHashFunction(int k);
//     ~AmplifiedHashFunction();
//     int execOutterHashing(Item* point);
// };

// class HashFunction {
//     private:
//     int R; 
//     public:
//     HashFunction();
//     ~HashFunction();
//     execInnerHashing();
// };

#endif