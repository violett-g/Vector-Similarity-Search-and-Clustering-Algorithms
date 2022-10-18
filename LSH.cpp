#include "hash.h"
#include "utils.h"
#include "LSH.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
// #include <chrono> 




LSH::LSH(int arg_k, int arg_L, int arg_N, double arg_r, vector<Item*>& arg_data, vector<Item*>& arg_queries):Method(arg_k,arg_N,arg_r,arg_data, arg_queries){
    L = arg_L;
    k = arg_k;
    N = arg_N;
    table_size = data.size() / HASHTABLE_FACTOR;

    //generate random r's used in g(p) finction
    default_random_engine generator;
    uniform_real_distribution<> distribution(0, 100);
    //vector<int> r_vec;
    for(int i=0; i<k; i++){
        int r = distribution(generator);
        r_vec.push_back(r);
    }
    build_hashTables();
}

LSH::~LSH(){ 
    //deallocations
  //  vector<vector<distance_item*>> N_nearest; //vector(queries) of vectors(pair item and its corresponding distance)  for N nearest neighbors
//        vector<vector<distance_item*>> range_nearest; //vector(queries) of vectors(pair item and its corresponding distance) for neighbors in range
    for(int q=0; q<N_nearest.size(); q++){
        for (int n=0; n<N_nearest[q].size(); n++){
            delete N_nearest[q][n];
        }
    }
    for(int q=0; q<range_nearest.size(); q++){
        for (int n=0; n<range_nearest[q].size(); n++){
            delete range_nearest[q][n];
        }
    }
}

// void LSH::insert_point(Item* point, HashTable table){
//     unsigned int g = calculate_g(point,r_vec) % table_size;
//     table.insert(make_pair(g,point)); 
// }

void LSH::build_hashTables(){
    // hash_tables.resize(L);
    vector<Item*> points = data;

    for(int i=0; i<L; i++){ //create L hash tables
        HashTable Htable = HashTable(k,N,points[0]->get_d());
        hash_tables.push_back(Htable);
    }
    for(int point=0; point<points.size(); point++){ //each item(data)
        for(int table=0; table<L; table++){
            hash_tables[table].insert_point_LSH(points[point],r_vec, table_size);
            //unsigned int g = calculate_g(points[point], r_vec) % table_size;  // calculate g for point
            //hash_tables[table].insert(make_pair(g,points[point]));
        }
    }

    //print for debuging
    // for(int l=0; l<L; l++){
    //     hash_tables[l].print_HT();
    // }
    
}

int LSH::getL()
{
    return L;
}

 vector<int> LSH::getR_vec() {
     return r_vec;
 }

int LSH::getTableSize()
{
    return table_size;
}
vector<HashTable> LSH::getHashTables() {
    return hash_tables;
}

vector<distance_item*> LSH:: find_nearest(Distance_Function dist_func){}

// }
//     for (int q = 0; q < queries.size(); q++)
//     {
//         auto start = chrono::system_clock::now();
//         for (int table = 0; table < L; table++)     // for i from 1 to L do
//         {
//             vector<unsigned char> pixels = Queries[q]->get_pixels();
//             unsigned int g = calculate_g(pixels, table) % ht_size;
//             auto range = hash_tables[table].equal_range(g);
            
//             for (auto p = range.first; p != range.second; p++)  // for each item p in bucket gi(q) do
//             {
//                 double dist = dist_function(Queries[q], p->second);  // dist(q,p)
//                 if (dist < best_nearest_candidates[q][k_neighbors-1]->first && !find_In_Neighbors(best_nearest_candidates[q], p->second))    // if dist(q,p) < db, where db k-th best distance
//                 {
//                     best_nearest_candidates[q][k_neighbors-1]->first = dist;
//                     best_nearest_candidates[q][k_neighbors-1]->second = p->second;
                    
//                     if (k_neighbors > 1)
//                         sort(best_nearest_candidates[q].begin(), best_nearest_candidates[q].end(), compare_distancePair);
//                 }

//                 if (hash_tables[table].count(g) > 10* L)    // if large number of retrieved items
//                     break;
//             }
        //}
vector<vector<distance_item*>> LSH:: find_N_nearest(Distance_Function dist_func){
    int n = N;
    //N_nearest.resize(queries.size());
    for (int q=0; q<queries.size(); q++){
        vector<distance_item*> random_dist_item;
        random_dist_item.reserve(n);
        for (int i=0; i<n; i++){
            distance_item* di = new distance_item(numeric_limits<double>::max(), queries[q]);
            random_dist_item.push_back(di);
        }
        N_nearest.push_back(random_dist_item);
    }
    for(int q=0; q<queries.size(); q++){
        auto start = chrono::system_clock::now();
        for(int t=0; t<L; t++){
            unsigned int g = hash_tables[t].calculate_g(queries[q], r_vec) % table_size;
            auto range = hash_tables[t].hash_table.equal_range(g); //all points with kew g = all point in the same bucket as query
            for( auto p=range.first; p!=range.second; p++){
                double distance = dist_func(queries[q], p->second);
                if(distance<N_nearest[q][n-1]->first && !find_item(N_nearest[q], p->second)){ // check if the last item in the sorted vector has greater distance and there is no other item with same distance
                    N_nearest[q][n-1]->first = distance; // replace 
                    N_nearest[q][n-1]->second = p->second;
                    if(N>1) sort(N_nearest[q].begin(), N_nearest[q].end(), compare_distance);
                    if (hash_tables[t].hash_table.count(g) > 10* L)//break if large number of items->optimization for big data
                    break;
                }
            }
        }
        
        
        auto end = chrono::system_clock::now();
        auto duration = end-start;
        time.push_back(duration);
    }
    return N_nearest;
}

vector<vector<distance_item*>> LSH ::find_in_range(Distance_Function dist_func){
    range_nearest.resize(queries.size());
    for(int q=0; q<queries.size(); q++){
        for(int t=0; t<L; t++){
            unsigned int g = hash_tables[t].calculate_g(queries[q], r_vec) % table_size;
            auto range = hash_tables[t].hash_table.equal_range(g); //all points with kew g = all point in the same bucket as query
            //auto range = hash_tables[t].find_points(g);
            for( auto p=range.first; p!=range.second; p++){
                double distance = dist_func(queries[q], p->second);
                if(distance < r && !find_item(range_nearest[q], p->second)){
                    distance_item* di = new distance_item(distance, p->second);
                    range_nearest[q].push_back(di);
                    if (hash_tables[t].hash_table.count(g) > 10* L)//break if large number of items->optimization for big data
                    break;
                }
            }
        }
        sort(range_nearest[q].begin(),range_nearest[q].end(), compare_distance);
    }
    return range_nearest;
}

void LSH::multiply_radii(){
    r*2;
}
