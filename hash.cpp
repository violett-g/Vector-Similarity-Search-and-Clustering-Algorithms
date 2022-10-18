#include "hash.h"
#include "utils.h"


#include <iostream>
#include <ctime>
#include <random>
#include <cmath>
#include <fstream>

HashTable::HashTable(int arg_k, int arg_N, int arg_d) 
            :  k(arg_k),N(arg_N),d(arg_d), w(1060){
    //initialization
    
    M = pow(2, 32) - 5;
    //table_size = data.size() / HASHTABLE_FACTOR;
    
    /* Distribute random v~N(0,1)* and random real number t(displacement)*/
    
    default_random_engine generator;
	// get v
	normal_distribution<float> distribution(0.0, 1.0);
    for(int j=0; j<k; j++){ // k since we need k v's to calculate g(p)
        vector<float> v_temp;
        for(int i = 0; i < d; i++){ 
		    float v= distribution(generator);
            v_temp.push_back(v);
        }
        v_vec.push_back(v_temp);
    }
	

	// get t
	uniform_real_distribution<> real_random(0.0, w - 0.000001);
    for(int i=0; i<d; i++){ // k since we need k t's to calculate g(p)
        float t = real_random(generator);
        t_vec.push_back(t);
    }
	

    /*----------------------------*/

    //buckets(size);
    //gFunction = amplified;
    f_map.resize(k);
};

HashTable::~HashTable() {
    //deconstruction
    f_map.clear();

};

int HashTable::calculate_h_i(Item* point,vector<float> v_vec, float t){
    //floor([ ( p * v ) + t) ] / w)
    vector<int> coordinates = point->get_coordinates();
    float pv = vector_product(coordinates,v_vec);
    float h_result = (pv+t)/ (float)w;
    return (int)h_result; //no need to calculate floor since casting to int is equivalent
}

long int HashTable::calculate_g(Item* point, vector<int> r_vec){
    //generate k r's ------->go to lsh
    // default_random_engine generator;
    // uniform_real_distribution<> distribution(0, 100);
    // vector<int> r_vec;
    // for(int i=0; i<k; i++){
    //     int r = distribution(generator);
    //     r_vec.push_back(r);
    // }
    //calculate k h(p) for point
  
    vector<int> hi_vec;
    for(int i=0; i<k; i++){
        int hi = calculate_h_i(point,v_vec[i],t_vec[i]);
        hi_vec.push_back(hi);
    }

    //ri*hi
    vector<int> ri_hi;
    for(int i=0; i<k; i++){
        int hr = gen_vector_product(hi_vec,r_vec);
        ri_hi.push_back(hr); 
    }

    //add
    int result = 0; 
    for(int i=0; i<k; i++){
        result += ri_hi[i];
    }
    //mod M
    long int g_result = mod(result,M);
    return g_result;
}

void HashTable::insert_point_LSH(Item* point, vector<int> r_vec, int table_size){
    long int g = calculate_g(point,r_vec) % table_size;
    hash_table.insert(make_pair(g,point)); 
}

string HashTable::calculate_f(Item* point){
    string bit_string;
    default_random_engine generator;
	uniform_int_distribution<int> distribution(0, 1);
    for(int i=0; i<k; i++){
        int h_i = calculate_h_i(point,v_vec[i],t_vec[i]);
        if(f_map[i].count(h_i) == 0){ // if h function isn't already maped
            f_map[i].insert(make_pair(h_i, distribution(generator))); // map randomly (0 or 1), add to structure
        }
        bit_string += to_string(f_map[i].find(h_i)->second); //concatenate the coresponding bit (0 or 1) oh h_i to the bit string 
    }
    return bit_string;

}

void HashTable::insert_point_CUBE(Item* point){ 
    string f = calculate_f(point);
    cube_hash_table.insert(make_pair(f,point)); 
}

void HashTable:: Hamming_distance(string vertex,int length, int hamming_distance, vector<string>& vertices){
    if(length < 0 ) return;
    if(hamming_distance == 0){
        vertices.push_back(vertex);
        return;
    }

    vertex[length] = vertex[length] == '0' ? '1' : '0';
    Hamming_distance(vertex,length-1, hamming_distance-1, vertices);
    
    vertex[length] = vertex[length] == '0' ? '1' : '0';
    Hamming_distance(vertex,length-1, hamming_distance, vertices);    
}

//returns a vector with all the neighbor vertices of a vertex in all possible hamming distances (restrained by probes)
vector<string> HashTable::find_neighbors(string vertex , int probes){
    vector<string> neighbor_vertices;
    neighbor_vertices.push_back(vertex); //same vertex is also consider a neighbor since it should be checked
    int length = vertex.length(); //length of the the bit string of the vertex defines the number of itterations for hamming distance calculation
    int hamming_dist = length;

    string  new_vertex = vertex;
    for(int d=1; d<=hamming_dist && neighbor_vertices.size() < probes; ++d ){
        Hamming_distance(new_vertex, length-1, d, neighbor_vertices);
    }
    return neighbor_vertices;
}









// auto HashTable :: find_points(unsigned int g){
//     auto range = hash_table.equal_range(g); //all points with kew g = all point in the same bucket as query
//     return range;
// }
// void HashTable::print_HT(){
//      for( const auto& n : hash_table ) {
//         cout << "Key:[" << n.first << "] Value:[" << n.second->get_id() << "]\n";
//     }
// }


// void HashTable::insertPoint(Item* point) {
//     int bucketNum = gFunction.execOutterHashing(point);
//     vector<Item*> targetBucket = buckets[bucketNum];
//     targetBucket.push_back(point);
// };

//  Item* point searchNN(Item* queryPoint) {
//     float minDistance;
//     float distance;
//     Item* nearest;
//     int bucketNum = gFunction.execOutterHashing(point);
//     vector<Item*> targetBucket = buckets[bucketNum];
//     targetBucket.push_back(point);
//     for(Item* tempPoint : targetBucket)
//         //calculate distance
//         if (distance < minDistance)
//         {
//             minDistance = distance;
//             nearest = tempPoint;
//         }
//         //check < r too
//     return nearest;
//  };

// AmplifiedHashFunction::AmplifiedHashFunction(int funcNumber, HashFunction hashFunc[funcNumber]) {
//     //initialization
//     k = funcNumber;
//     for (int i=0; i<k; i++)
//     {
//         hi[i] = hashFunc[i];
//     }
// }

// AmplifiedHashFunction::~AmplifiedHashFunction() {
//     //deconstruction
// };

// AmplifiedHashFunction::execOutterHashing(Item* point){
    
// }
