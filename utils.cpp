#include "utils.h"
#include "item.h"
#include <iostream>
#include <cmath>
#include <string>
#include <cstring> 

using namespace std;



double euclidean_distance(Item* item1, Item* item2){
    vector<int> item1_coordinates = item1->get_coordinates();
    vector<int> item2_coordinates = item2->get_coordinates();

    int total = 0;
    int diff = 0;
    for (int i=1; i<item1->get_length(); i++){
        diff = item1_coordinates[i] - item2_coordinates[i];
        total = total + (diff*diff);
    }
    return sqrt(total);

}

/* Compute inner product of two vectors */
float vector_product(vector<int>& vec1, vector<float>& vec2 ){
    float product = 0.0;

    for(int i = 0; i < vec2.size(); i++)
        product += (float)vec1[i+1] * vec2[i];

    return product;
} 

int gen_vector_product(vector<int>& vec1, vector<int>& vec2 ){
    int product = 0;

    for(int i = 0; i < vec2.size(); i++) {
        product += vec1[i] * vec2[i];
    }

    return product;
}


/* It implements the modulo operation using bitwise operations (works with negative numbers as well) */
long int mod(long int a, long int b){
    return a & (b - 1);
}

bool isNumber(const string& str){
    for (char const &c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}


