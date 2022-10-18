 #include "exhaustive.h"
 #include <iostream>
 #include <algorithm>

Exhaustive::Exhaustive(int arg_k, int arg_N, double arg_r, vector<Item*>& arg_data, vector<Item*>& arg_queries):Method(arg_k,arg_N,arg_r,arg_data, arg_queries){}


Exhaustive::~Exhaustive(){
    //needed
    //to be deleted-> vector of vectors
    for(int q=0; q<all_distances.size(); q++){
        for(int d=0; d<all_distances[q].size(); d++){
            delete all_distances[q][d];
        }
    }
}

void Exhaustive::calculate_all_distances(Distance_Function dist_func){
    
    all_distances.reserve(queries.size()); //reseve space = length of the queries vector
    time.resize(queries.size());
    //build the structure to store distances of each data from each query
    for(int q=0; q<queries.size(); q++){//traverse the queries and create the vectors to keep the pair distance,data
        auto start = chrono::system_clock::now();
        vector<distance_item*>distances_of_query;
        distances_of_query.reserve(data.size());

        for(int d=0; d<data.size(); d++){//calculate distance for each data
            distance_item* new_dist_item = new distance_item(dist_func( queries[q], data[d]), data[d] );
            distances_of_query.push_back(new_dist_item);
        }
        sort(distances_of_query.begin(), distances_of_query.end(), compare_distance);
        all_distances.push_back(distances_of_query);
        auto end = chrono::system_clock::now();
        auto duration = end-start;
        time[q] += duration;
        //sort(all_distances[q].begin(), all_distances[q].end(), compare_distance);

    }
}


vector<distance_item*> Exhaustive::find_nearest(Distance_Function dist_func){
    if (all_distances.empty()) calculate_all_distances(dist_func); //if we haven't already calculated all distances, do it

    vector<distance_item*> NN; //hold a single nearest neighbor for each query
    for(int q=0; q<queries.size(); q++){
        auto start = chrono::system_clock::now();
        distance_item* current_NN = all_distances[q][0]; //set as nearest neighbor the first item(data) in the vector
        for(int d=0; d<data.size(); d++){ //traverse the vector of data (distance,item) and update the NN every time a shorter distance is found
            if(all_distances[q][d]->first< current_NN->first){
               current_NN = all_distances[q][d]; //update
            }
        }
        NN.push_back(current_NN);

        auto end = chrono::system_clock::now();
        auto duration = end-start;
        time[q] += duration;
    }
    return NN;
}


//function to sort distances in a vector
vector<distance_item*> Exhaustive:: sort_distances(vector<distance_item*> distances){
    vector<distance_item*> sorted = distances;
    for(int i=0; i<sorted.size(); i++){
        for(int j=0; j<sorted.size()-i-1; j++){
            if(distances[j]->first < distances[j+1]->first)
               iter_swap(sorted.begin() + j, sorted.begin() + (j+1));
        }
    }
    return sorted;
}

vector<vector<distance_item*>> Exhaustive::find_N_nearest(Distance_Function dist_func){
    if (all_distances.empty()) calculate_all_distances(dist_func); //if we haven't already calculated all distances, do it
    vector<vector<distance_item*>> N_nearest_neighbors;
    for(int q=0; q<queries.size(); q++){
        vector<distance_item*> N_distances;
        for(int d=0; d<N; d++){
            N_distances.push_back(all_distances[q][d]);
        }
        N_nearest_neighbors.push_back(N_distances);
    }
    return N_nearest_neighbors;
}



/*---------------------------- W -------------------------------*/

void Exhaustive::calculate_all_distances_data(Distance_Function dist_func){
    
    all_distances_data.reserve(data.size()); //reseve exactly as much space as the length of the queries vector
    //time.resize(queries.size());
    //build the structure to store distances of each data from each query
    for(int d=0; d<data.size(); d++){//traverse the queries and crete the vectors to keep the pair distance,data
        vector<distance_item*>distances_of_data;
        distances_of_data.reserve(data.size());

        for(int d2=0; d2<data.size(); d2++){//calculate distance for each data
            distance_item* new_dist_item = new distance_item(dist_func( data[d], data[d2]), data[d2] );
            distances_of_data.push_back(new_dist_item);
        }
        sort(distances_of_data.begin(), distances_of_data.end(), compare_distance);
        all_distances_data.push_back(distances_of_data);
    }
}


// vector<distance_item*> Exhaustive::find_nearest_data(Distance_Function dist_func){
//     if (all_distances.empty()) calculate_all_distances(dist_func); //if we haven't already calculated all distances, do it
//     // for (int b=0; b< all_distances.size(); b++){
//         for (int bd=0; bd<data.size(); bd++){
//         }
//     // }
//     vector<distance_item*> NN; //hold a single nearest neighbor for each query
//     for(int q=0; q<queries.size(); q++){
//         auto start = chrono::system_clock::now();
//         distance_item* current_NN = all_distances[q][0]; //set as nearest neighbor the first item(data) in the vector
//         for(int d=0; d<data.size(); d++){ //traverse the vector of data (distance,item) and update the NN every time a shorter distance is found
//             if(all_distances[q][d]->first< current_NN->first){
//                current_NN = all_distances[q][d]; //update
//             }
//         }
//         NN.push_back(current_NN);

//         auto end = chrono::system_clock::now();
//         auto duration = end-start;
//         time[q] += duration;
//     }
//     return NN;
// }


// vector<vector<distancePair*>> Exact::calculate_N_Nearest(Distance_Function distance_Function){
//     if (allDistances.empty()) calculate_allDistances(distance_Function); //if we haven't already calculated all distances, do it

//     vector<vector<distancePair*>> initialDistances = allDistances;
//     vector<vector<distancePair*>> NNearestNeighbors;
//     distancePair* dummyDistancePair = new distancePair(numeric_limits<double>::max(),NULL);
//     for(int neighbor_num=0; neighbor_num<N; neighbor_num++){
//         vector<distancePair*> singleNearestNeighbors = calculate_Nearest(distance_Function);
//         NNearestNeighbors.push_back(singleNearestNeighbors);
//         for(int query_num=0; query_num<Queries.size(); query_num++){
//             allDistances[query_num][singleNearestNeighbors[query_num]->second->get_order()] = dummyDistancePair;
//         }
//     }
//     allDistances = initialDistances;
//     delete dummyDistancePair;
//     return NNearestNeighbors;
// }


vector<vector<distance_item*>> Exhaustive::find_in_range(Distance_Function dist_func){

    vector<vector<distance_item*>> rangeN; //stores the neighbors in the range
    for(int q=0; q<queries.size(); q++){
        vector<distance_item*> rangeN_query;
        for(int d=0; d<data.size(); d++){ //traverse all data
            if(all_distances[q][d]->first< r){ //if a distance of data is inside the range
                rangeN_query.push_back(all_distances[q][d]);
            }
        }
        rangeN.push_back(rangeN_query);
    }
    return rangeN;
}

