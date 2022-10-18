
#include "input.h"
#include "item.h"
#include "utils.h"
#include "exhaustive.h"
#include "LSH.h"
#include "method.h"
#include "CUBE.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <ctype.h>

using namespace std;

class Item;


Input::Input(int argc, char *argv[])
{
    //     //Default
#if LSH_FLAG //running ./lsh
    k = 4;
    L = 5;
    N = 1;
    R = 10000;
    parse_arguments(argc, argv);
    read_data();
    read_query();

#elif HYPERCUBE_FLAG //running ./cube
    k = 14;
    M = 10;
    probes = 2;
    N = 1;
    R = 10000;
    parse_arguments(argc, argv);
    read_data();
    read_query();

#elif CLUSTER_FLAG //running ./cluster
    parse_arguments(argc, argv);
    if (method == "LSH")
    {
        K = 4;
        L = 3;
        kLSH = 4;
        M = 10;
        // k = 4;
        // L = 3;
        //R = 900;
    } //If CLUSTER is called with LSH method
    else if (method == "Hypercube")
    {
        K = 4;
        L = 3;
        kCUBE = 3;
        M = 10;
        probes = 2;
        // k = 14;
        // M = 10;
        // probes = 2;
        // R = 900;
    } //If CLUSTER is called with HYPERCUBE method
    else if (method == "Classic")
    {
    } //If CLUSTER is called with CLASSIC method
#endif
}

Input::~Input()
{
    for (int i = 0; i < data.size(); i++)
    {
        delete data[i];
    }
    for (int i = 0; i < queries.size(); i++)
    {
        delete queries[i];
    }
}

int Input::getkLSH()
{
    return kLSH;
}
int Input::getkCube()
{
    return kCUBE;
}
int Input::getL()
{
    return L;
}
int Input::getM()
{
    return M;
}
int Input::getK()
{
    return K;
}
int Input::getProbes()
{
    return probes;
}

vector<Item*> Input::getData()
{
    return data;
}

string Input::getMethod()
{
    return method;
}
void Input::parse_arguments(int argc, char *argv[])
{

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            inputPath = argv[i + 1];
            cout << "input: " << inputPath << endl;
        }
        else if (strcmp(argv[i], "-k") == 0)
        {
            k = atoi(argv[i + 1]);
            cout << "k: " << k << endl;
        }
        else if (strcmp(argv[i], "-q") == 0)
        {
            queryPath = argv[i + 1];
            cout << "query: " << queryPath << endl;
        }
        else if (strcmp(argv[i], "-L") == 0)
            L = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-o") == 0)
            outputPath = argv[i + 1];
        else if (strcmp(argv[i], "-N") == 0)
        {
            N = atoi(argv[i + 1]);
            cout << "N: " << N << endl;
        }
        else if (strcmp(argv[i], "-R") == 0)
        {
            R = atoi(argv[i + 1]);
            cout << "R: " << R << endl;
        }
        else if (strcmp(argv[i], "-M") == 0)
            M = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-probes") == 0)
            probes = atoi(argv[i + 1]);
        //else if(strcmp(argv[i], "-complete") == 0) /////////////////////////////////////////////////////////
        else if (strcmp(argv[i], "-m") == 0)
            method = argv[i + 1];
        else if (strcmp(argv[i], "-c") == 0)
            configurationPath = argv[i + 1];
    }
    //checks for missing obligatory parameters -> input file path and query file path
    if (inputPath.empty() || outputPath.empty())
    {
        cout << "Please provide dataset and/or output path" << endl;
        exit(EXIT_FAILURE);
    }
    // #if CLUSTER_FLAG
    //     if (configPath.empty() || method.empty()){
    //         cout << "Missing configuration path and/or method arguments" << endl;
    //         exit(EXIT_FAILURE);
    //     }
    // #else
    if (queryPath.empty())
    {
        cout << "Please provide query set path" << endl;
        exit(EXIT_FAILURE);
    }
    // #endif
}

// bool Input :: isNumber(const string& str){
//     for (char const &c : str) {
//         if (isdigit(c) == 0) return false;
//     }
//     return true;
// }

void Input::read_data()
{
    ifstream dataFile;
    dataFile.open(inputPath); //open data file

    if (!dataFile.is_open())
    { //check if file is opened
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    //vector<Item*> data; //create a structure to store data
    //vector<vector<int>> data;
    while (!dataFile.eof())
    { //read file till the end
        string token;
        while (getline(dataFile, token, '\n'))
        {                            //read line (line = item)
            vector<int> coordinates; //vector to store coordinates of a vector
            string temp;
            stringstream X(token);
            while (getline(X, temp, ' '))
            { //read coordinates
                if (isNumber(temp) == 0)
                {
                    break;
                }
                int coordinate = stoi(temp);       //convert string to int
                coordinates.push_back(coordinate); //store to structure
            }
            Item *newItem = new Item(coordinates); //create an item and store to data structure
            data.push_back(newItem);
            //data.push_back(coordinates);
        }
        // // Displaying the 2D vector
        // for (int i = 0; i < data.size(); i++) {
        //     vector<int> temp_vec = data[i]->get_coordinates();
        //     for (int j = 0; j < temp_vec.size(); j++)
        //         cout << temp_vec[j] << " ";
        //     cout << endl;
        // }
    }
    cout << endl;
    dataFile.close();
}

void Input::read_query()
{
    string line;
    ifstream queryFile;
    queryFile.open(queryPath); //open data file

    if (!queryFile.is_open())
    { //check if file is opened
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    //vector<Item*> data; //create a structure to store data
    //vector<vector<int>> data;
    while (!queryFile.eof())
    { //read file till the end
        string token;
        while (getline(queryFile, token, '\n'))
        {                            //read line (line = item)
            vector<int> coordinates; //vector to store coordinates of a vector
            string temp;
            stringstream X(token);
            while (getline(X, temp, ' '))
            { //read coordinates
                if (isNumber(temp) == 0)
                {
                    break;
                }
                int coordinate = stoi(temp);       //convert string to int
                coordinates.push_back(coordinate); //store to structure
            }
            Item *newItem = new Item(coordinates); //create an item and store to data structure
            queries.push_back(newItem);
            //queries.push_back(coordinates);
        }
        // Displaying the 2D vector
        // for (int i = 0; i < queries.size(); i++) {
        //     vector<int> temp_vec = queries[i]->get_coordinates();
        //     for (int j = 0; j < temp_vec.size(); j++)
        //         cout << temp_vec[j] << " ";
        //     cout << endl;
        // }
    }
    cout << endl;
    queryFile.close();
}

int Input::read_cluster_file()
{
    ifstream clustFile("cluster.conf");
    char *c;
    if (!clustFile.is_open())
    {
        cerr << "Could not open the file'" << endl;
        return EXIT_FAILURE;
    }
    // while (!clustFile.eof())
    // {
    string line;
    string sub;
    string firstsub;
    while (getline(clustFile, line, '\n'))
    {
        firstsub = line.substr(0, line.find(":"));
        sub = line.substr(line.find(":") + 1);
        if (firstsub == "number_of_clusters")
        {
            K = stoi(sub);
        }
        else if (firstsub == "number_of_vector_hash_tables")
        {
            L = stoi(sub);
        }
        else if (firstsub == "number_of_vector_hash_functions")
        {
            kLSH = stoi(sub);
        }
        else if (firstsub == "max_number_M_hypercube")
        {
            M = stoi(sub);
        }
        else if (firstsub == "number_of_hypercube_dimensions")
        {
            kCUBE = stoi(sub);
        }
        else if (firstsub == "number_of_probes")
        {
            probes = stoi(sub);
        }
    }
    clustFile.close();

    return EXIT_SUCCESS;
}

void Input::find_neighbors(Distance_Function dist_func)
{
    //create output file
    ofstream output;
    output.open(outputPath);
    if (!output.is_open())
    {
        cout << "Output file do not open" << endl;
    }

    Exhaustive *myExhaustive;
    myExhaustive = new Exhaustive(k, N, R, data, queries);
    vector<vector<distance_item *>> Exhaustive_NN = myExhaustive->find_N_nearest(dist_func);

    //for w
    // myExhaustive->calculate_all_distances_data(dist_func);
    // for(int d=0; d<1000; d++){
    //     cout << myExhaustive->all_distances_data[d][1]-> first << endl;
    // }

    // for(int q=0; q<queries.size(); q++){
    //     output << "query: " << q << endl;
    //     for(int i=0; i<Exhaustive_NN[q].size(); i++){
    //         output<< "--" << Exhaustive_NN[q][i]->first << " " << Exhaustive_NN[q][i]->second  << endl;
    //     }
    //     for(int i=0; i<myExhaustive->all_distances[q].size(); i++){
    //         ;
    //         output << myExhaustive->all_distances[q][i]->first << " " << myExhaustive->all_distances[q][i]->second  << endl;
    //     }
    // }
    ///////////

    vector<chrono::duration<double>> Exhaustive_time = myExhaustive->get_time();

    Method *myMethod; //lsh or hypercube
#if LSH_FLAG
    myMethod = new LSH(k, L, N, R, data, queries);
    method = "LSH";
#elif HYPERCUBE_FLAG
    myMethod = new CUBE(k, probes, M, N, R, data, queries);
    method = "Hypercube";
#endif

    //call functions to find nearest
    // vector<distance_item*> neighbors= myMethod->find_nearest(dist_func);
    vector<vector<distance_item *>> N_neighbors = myMethod->find_N_nearest(dist_func);
    vector<vector<distance_item *>> range_neighbors = myMethod->find_in_range(dist_func);
    //get duration of used method (lsh or hypercube)
    vector<chrono::duration<double>> method_time = myMethod->get_time();

    //OUTPUT + statistics
    for (int q = 0; q < queries.size(); q++)
    {
        output << "Query: " << q + 1 << endl;
        for (int n = 0; n < N_neighbors[q].size(); n++)
        {
            output << "Nearest neighbor-" << n + 1 << " " << N_neighbors[q][n]->second->get_id() << endl;
            output << "distance" << method << ":" << N_neighbors[q][n]->first << endl;
            output << "distanceTrue: " << Exhaustive_NN[q][n]->first << endl;
        }
        output << "t" << method << ":" << method_time[q].count() << endl;
        output << "tTrue" << Exhaustive_time[q].count() << endl;
        output << endl;
    }
    for (int q = 0; q < queries.size(); q++)
    {
        //range
        output << "Query: " << q + 1 << endl;
        output << "R-near neighbors:" << endl;
        for (int n = 0; n < range_neighbors[q].size(); n++)
        {
            output << range_neighbors[q][n]->second->get_id() << endl;
        }
        output << endl;
    }

    delete myMethod;
    delete myExhaustive;
    output.close();
}

void Input::more_queries(string new_queryPath)
{
    queryPath = new_queryPath;
    for (int i = 0; i < queries.size(); i++)
    { //Deallocate the query images from the heap
        delete queries[i];
    }
    queries.clear(); //Removes all elements from the vector so that it gets filled with the new Queries
    read_query();
}

