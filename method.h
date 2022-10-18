#ifndef METHOD_H
#define METHOD_H

#include <vector>
#include <chrono>
#include "input.h"
#include "utils.h"

using namespace std;

typedef pair<double,Item*> distance_item;
class Method {
    protected:
        int k;
        int N;
        double r;
        vector<Item*> data;
        vector<Item*> queries;
        vector<chrono::duration<double>> time;

        bool compare_dist(distance_item* item1, distance_item* item2);
    public:

    Method(int arg_k, int arg_N, double arg_r, vector<Item*>arg_data, vector<Item*> arg_queries);
    virtual ~Method();
    vector<chrono::duration<double>> get_time();
    double get_r();
    int find_item(vector<distance_item*> di_vec, Item* item);
    static bool compare_distance(distance_item* item1, distance_item* item2);

    virtual vector<distance_item*> find_nearest(Distance_Function dist_func)=0;
    virtual vector<vector<distance_item*>> find_N_nearest(Distance_Function dist_func)=0;
    virtual vector<vector<distance_item*>> find_in_range(Distance_Function dist_func)=0;


    //virtual vector<distance_item*> sort_distances(vector<distance_item*> distances);


};



#endif 
