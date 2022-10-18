#ifndef ITEM_H
#define ITEM_H
#include <vector>

using namespace std;

class Item {
    private:
        vector<int> coordinates;
        int length;
        int d; //dimensions
    public:
        Item(vector<int> arg_coordinates);
        ~Item();
        vector<int> get_coordinates();
        int get_length();
        int get_id();
        int get_d();
};

#endif