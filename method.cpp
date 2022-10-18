#include "method.h"


Method::Method(int arg_k, int arg_N, double arg_r, vector<Item*>arg_data, vector<Item*> arg_queries): k(arg_k), N(arg_N), r(arg_r), data(arg_data), queries(arg_queries){}

Method::~Method(){}

vector<chrono::duration<double> > Method::get_time(){
    return time;
}

double Method::get_r(){
    return r;
}



bool Method::compare_distance(distance_item* item1, distance_item* item2){
    return item1->first < item2->first;
}

int Method ::find_item(vector<distance_item*> di_vec, Item* item){
    for (int i = 0; i < di_vec.size(); i++)
        if (di_vec[i]->second == item)
            return 1;
    return 0;
}
