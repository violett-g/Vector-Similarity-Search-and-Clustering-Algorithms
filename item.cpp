#include "item.h"

Item::Item(vector<int> arg_coordinates): coordinates(arg_coordinates){
    length = arg_coordinates.size();
    d = arg_coordinates.size()-1;
}

Item::~Item(){
    

}

vector<int> Item::get_coordinates(){
    return coordinates;
}

int Item::get_length(){
    return length;
}

int Item:: get_id(){
    return coordinates[0];
}

int Item:: get_d(){
    return d;
}