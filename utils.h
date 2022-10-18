#ifndef UTILS_H
#define UTILS_H

#include "item.h"
#include <string>
typedef double (*Distance_Function)(Item* d1, Item* d2);
double euclidean_distance(Item* item1, Item* item2); //function to calculate the euclidian distance of two vectors
float vector_product(vector<int>& vec1, vector<float>& vec2 );
int gen_vector_product(vector<int>& vec1, vector<int>& vec2 );
long int mod(long int a, long int b); //function to calculate mod that works with negative numbers
bool isNumber(const string& str); //function to check if a string is a number

#endif