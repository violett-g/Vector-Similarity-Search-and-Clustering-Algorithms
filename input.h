
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>
#include "item.h"
#include "utils.h"
using namespace std;

class Input{
    private:
        int k; //number of hi functions
        int kLSH;
        int kCUBE;
        int L; //number of hashtables
        int N; //number of closest neighbours
        double R; //range
        int K; //
        int M; //max number of data to be checked
        int probes; //max number of edges on the hypercube
        string method; //
        string queryPath; 
        string outputPath;
        string configurationPath;
        string inputPath;

        vector<Item*> data; //structure to store datas
        vector<Item*> queries; //structure to store query points
       // vector<vector<int> > data; //structure to store datas
        //vector<vector <int> > queries; //structure to store query points
        
        //functions
        void parse_arguments(int argc, char* argv[]);
        // void find_neighbors(Distance_Function dist_func);
        // void read_data(); // function to read input data from file create item and store to heap
        // void read_query(); // function to read queries create query item and store to  heap
        // bool isNumber(const string& str); //function to check if a string is a number
    public:
    Input(int argc, char* argv[]);
    ~Input();
    void read_data();
    void read_query();
    void find_neighbors(Distance_Function dist_func);
    void more_queries(string new_queryPath);
    int read_cluster_file();
    int getkLSH();
    int getkCube();
    int getL();
    int getM();
    int getK();
    int getProbes();
    string getMethod();
    vector<Item*> getData();
    //testing
    void test_lsh(Distance_Function dist_func);
    void test_cube(Distance_Function dist_func);

};


#endif 