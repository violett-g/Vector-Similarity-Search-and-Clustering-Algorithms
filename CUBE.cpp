#include "CUBE.h"
#include "hash.h"
#include "utils.h"

#include <iostream>

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
// #include <chrono> 


CUBE::CUBE(int arg_k, int arg_probes, int arg_M, int arg_N, double arg_r, vector<Item*>& arg_data, vector<Item*>& arg_queries):Method(arg_k,arg_N,arg_r,arg_data, arg_queries){
    M = arg_M; probes = arg_probes; k = arg_k; N=arg_N;

    build_hashTable();   
}
CUBE:: ~CUBE(){ 
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
    delete hash_table;
}

void CUBE::build_hashTable(){
    vector<Item*> points = data;

    HashTable* Htable = new HashTable(k,N,points[0]->get_d());
    hash_table = Htable;

    for(int point=0; point<points.size(); point++){ //each item(data)
           hash_table->insert_point_CUBE(points[point]);
    }
    //print for debuging
    // hash_table->print_HT();
    
}

vector<distance_item*> CUBE:: find_nearest(Distance_Function dist_func){}
vector<vector<distance_item*>> CUBE:: find_N_nearest(Distance_Function dist_func){
    int n= N;
    //inititialize N point with maximum possible distance
    for (int q=0; q<queries.size(); q++){
        vector<distance_item*> random_dist_item;
        random_dist_item.reserve(n);
        for (int i=0; i<n; i++){
            distance_item* di = new distance_item(numeric_limits<double>::max(), queries[q]);
            random_dist_item.push_back(di);
        }
        N_nearest.push_back(random_dist_item);
    }

    //check for real N nearest points
    for(int q=0; q<queries.size(); q++){
        auto start = chrono::system_clock::now();
        string f = hash_table->calculate_f(queries[q]);
        vector<string> vertices = hash_table->find_neighbors(f,probes);

        for(int p=0; p<probes; p++){
            int count=0;
            auto range = hash_table->cube_hash_table.equal_range(vertices[p]);
            for(auto point=range.first; point!=range.second && count<M; point++){
                double distance = dist_func(queries[q], point->second);
                if(distance<N_nearest[q][n-1]->first){
                    N_nearest[q][n-1]->first = distance; // replace 
                    N_nearest[q][n-1]->second = point->second;
                    if(N>1) sort(N_nearest[q].begin(), N_nearest[q].end(), compare_distance);
                }
                count++;
            }
        }
        auto end = chrono::system_clock::now();
        auto duration = end-start;
        time.push_back(duration);
    }
    //probes*count -> items that really checked , could be less than N 
    return N_nearest;
  
}

vector<vector<distance_item*>> CUBE ::find_in_range(Distance_Function dist_func){
    range_nearest.resize(queries.size());
    for(int q=0; q<queries.size(); q++){
            string f = hash_table->calculate_f(queries[q]);
            vector<string> vertices = hash_table->find_neighbors(f,probes);
            for(int p=0; p<probes; p++){ //check neighbor vertices of query vertex
                int count = 0;
                auto range = hash_table->cube_hash_table.equal_range(vertices[p]); //get points of the vertex
                for(auto point=range.first; point!=range.second && count < M; point++){
                    double distance = dist_func(queries[q], point->second);
                    if (distance < r){
                        distance_item* di = new distance_item(distance, point->second);
                        range_nearest[q].push_back(di);
                    }
                    count++;
                }
            }
        
        sort(range_nearest[q].begin(),range_nearest[q].end(), compare_distance);
    }
    return range_nearest;
}

void CUBE::multiply_radii(){
    r*2;
}