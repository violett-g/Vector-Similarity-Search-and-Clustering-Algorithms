#include <iostream>
#include "input.h"
#include "item.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]){
    Input myInput(argc,argv);
    #if LSH_FLAG || HYPERCUBE_FLAG
    myInput.find_neighbors(euclidean_distance);
    #elif CLUSTER_FLAG
    Input clusterInput(argc,argv);
    clusterInput.read_data();
    clusterInput.read_cluster_file();
    clusterInput.parse_arguments();

   
    if (clusterInput.getMethod().compare("Classic") == 0)
    {
        Clustering clustering(Lloyd(),clusterInput.getK());
        clustering.execClustering(clusterInput.getData(), clusterInput.getK());
    }
   else if (clusterInput.getMethod().compare("LSH") == 0)
   {
        Clustering clustering(RangeSearchLSH(), clusterInput.getK());
        clustering.execClustering(clusterInput.getData(), clusterInput.getK());
   }
    #endif

    //loop
    string more;
    string queryPath;
    while(true){
        cout << "Would you like to proccess a new queryset? (yes/no)" << endl;
        cin >> more;
        if(more == "yes"){
            cout << "Type the new file path for the new queryset" << endl;
            cin >> queryPath;
            myInput.more_queries(queryPath);
            myInput.find_neighbors(euclidean_distance);
        }
        else { break;}
    }

    //testing
    //myInput.read_data();
    //myInput.find_neighbors(euclidean_distance);
    // myInput.test_lsh(euclidean_distance);
    //myInput.test_cube(euclidean_distance);

}
