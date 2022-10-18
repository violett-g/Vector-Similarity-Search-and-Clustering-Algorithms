#include <iostream>
#include <unordered_map>
#include <random>
#include <vector>
#include <math.h>
#include "utils.h"
#include "method.h"
#include "item.h"
#include "clustering.h"

#define VALUE 50

using namespace std;


Algorithm::Algorithm() {
       cout << "Alg constructor pending" <<endl;
  }


Algorithm::~Algorithm() {
        cout << "Memory deallocated" << endl;
}

void Algorithm::addtoCluster(Item* point, vector<Cluster> clusters, int K)
{ }

void Algorithm::execAlgorithm(vector<Item*> point, vector<Cluster> clusters, int K)
{ }


void Algorithm::reCenter(vector<Cluster> clusters, int K)
{

}


Clustering::Clustering(Algorithm algorithm, int C)
{
    algorithm_used = algorithm;
    K = C;
    for (int i=0; i<K; ++i)
    {
        clusters.push_back(Cluster());
    }
}


Clustering::~Clustering()
{
    //deallocations
    cout << "Memory deallocated" << endl;
}

Cluster::Cluster()
{
    cout << "Cluster constructor pending" <<endl;
}

Cluster::~Cluster()
{
    cout << "Memory deallocated" << endl;
}


void Clustering::execClustering(vector<Item *> points, int K)
{
    algorithm_used.execAlgorithm(points, clusters, K);
    for (Cluster cluster: clusters)
    {
        cluster.printCluster();
    }
}

int Clustering::getK()
{
    return K;
}
//read points to cluster from cluster.config
//check arguments

int Cluster::getId()
{
    return id;
}
Item *Cluster::getCentroid()
{
    return centroid;
}
vector<Item *> Cluster::getClusteredPoints()
{
    return clustered_points;
}
Item *Cluster::getPrevCentroid()
{
    return prev_centroid;
}

void Cluster::setId(int argId)
{
    id = argId;
}
void Cluster::setCentroid(Item *centre)
{
    centroid = centre;
}
void Cluster::setClusteredPoints(vector<Item *> clustered)
{
    clustered_points = clustered;
}
void Cluster::setPrevCentroid(Item *prev)
{
    prev_centroid = prev;
}

void Cluster::printCluster()
{
    cout << "Cluster with id:" << id << endl;
    for (Item* point: clustered_points)
    {
        cout << "item id:" << point->get_id() << endl;
    }
    cout << endl;
}

void Algorithm::initialize(vector<Item *> non_centroids, vector<Cluster> clusters, int K)
{
    int generator;
    //centroids first random initialization
    for (int i = 0; i < K; ++i)
    {
        generator = rand() % non_centroids.size();
        // non_centroids[generator];
        clusters[i].setId(i + 1);
        clusters[i].setCentroid(non_centroids[generator]);
        clusters[i].setPrevCentroid(NULL);
    }
}

bool Algorithm::converge(vector<Cluster> clusters, int K)
{
    for (int i = 0; i < K; ++i)
    {
        if (clusters[i].getCentroid() == clusters[i].getPrevCentroid())
        {
            //sugklisi
            //centroids are the same as in previous iteration, we are done
            cout << "Centroids are the same as in previous iteration. Converge is made." <<endl;
            return true;
        }
    }
    return false;
}

void Lloyd::addtoCluster(Item *point, vector<Cluster> clusters, int K)
{
    //calculate distance of non_centroid point to centroids
    vector<distance_item*> min_distances;
    double min_distance = 100000;
    double point_distance;
    min_distance = 100000;
    int centroid;
    for (int i = 0; i < K; ++i)
    {
        point_distance = euclidean_distance(point, clusters[i].getCentroid());
        if (point_distance < min_distance)
        {
            min_distance = point_distance;
            centroid = clusters[i].getId();
        }
    }
    //add to cluster with the nearest centroid found above
    vector<Item *> tmp = clusters[centroid - 1].getClusteredPoints();
    tmp.push_back(point);
    clusters[centroid - 1].setClusteredPoints(tmp);
    //store distances in a struct, following the pair format
    // min_distances.push_back(new distance_item(min_distance, point));
}

void Lloyd::reCenter(vector<Cluster> clusters, int K)
{
    Item *candidate;
    double distance;
    double min_distance;
    Item *mean_centroid;
    double sum = 0;
    double new_coordinate;

    //centroids initialization
    for (int i = 0; i < K; ++i)
    {
        //for every cluster
        vector<int> new_centroid;
        for (int j = 0; j < clusters[i].getCentroid()->get_d(); ++j)
        {
            //for every dimension
            sum = 0;
            for (int k = 0; k < clusters[i].getClusteredPoints().size(); ++k)
            {
                //for every point
                sum = sum + clusters[i].getClusteredPoints()[k]->get_coordinates()[j];
            }
            new_coordinate = ceil(sum / clusters[i].getClusteredPoints().size());
            // add dimension calculated to the centroid point
            new_centroid.push_back(new_coordinate);
        }
        //initialize centroid as Item with coordinates calculated above
        clusters[i].setPrevCentroid(clusters[i].getCentroid());
        clusters[i].setCentroid(new Item(new_centroid));
    }
}

void Lloyd::execAlgorithm(vector<Item *> points, vector<Cluster> clusters, int K)
{
    int count = 0;
    bool converged = false;
    //first assignment
    cout << "Executing LLoyd's algorithm." <<endl;
    for (Item *point : points)
    {
        addtoCluster(point, clusters, K);
    }
    while (converged == false)
    {
        if (count != 0)
        {
            reCenter(clusters, K);
            for (Item *p : points)
            {
                addtoCluster(p, clusters, K);
            }
        }
        converged = converge(clusters, K);
        count++;
    }
}

// initial_distance = min(dist between initial centroids)/2
// for centroids: range search with R = 2^(i-1)*initial_distance
// osa prokyptoyn, mpainoyn sto cluster -> clusters[i].clustered_points.push_back(point)
// mark assigned points (use flag field) i pair(point, centroid), if centroid!== null, unassigned
// if flag = assigned, keep assignment to centroid, point min distance
// new centroid, check converge
// repeat until converged

vector<Item *> RangeSearchLSH::rangeSearch(Item *centre, float R)
{
    
    long int g;
    int HTnumber = lsh.getL();
    vector<Item *> results;
    vector<Item *> dataset;

    for (int i = 0; i < HTnumber; ++i)
    {
        HashTable& hTable = lsh.getHashTables().at(i);
        g = hTable.calculate_g(centre, lsh.getR_vec()) % HASHTABLE_FACTOR;

        auto hash_bucket = hTable.hash_table.equal_range(g%lsh.getTableSize());

        double lim = R * HASHTABLE_FACTOR;
      
        // cout <<"hash"+ hash_bucket <<endl;
        for(auto Itr = hash_bucket.first; Itr != hash_bucket.second; ++Itr)
        {
                 if (Itr->second == centre)
            {
                if (euclidean_distance(Itr->second, centre) < lim)
                {
                    results.push_back(Itr->second);
                }
            }
        }
      
    }
    return dataset;
}

void RangeSearchLSH::addtoCluster(Item* point, vector<Cluster> clusters, int K)
{
  
    float R;
    int assigned = 0;
    bool done = false;
    vector<Item*> clusterset;
    double min_distance = 100000;
    double centroid_distance;
    // rangesearch, assign, mark assigned, check already assigned

//calculate mindistance between centroids
    for (int i=0; i<K; ++i)
    {
        for (int j=i+1; j<K; ++j)
        {
            euclidean_distance(clusters[i].getCentroid(), clusters[j].getCentroid());
            if (centroid_distance < min_distance)
            {
                min_distance = centroid_distance;
            }
        }
    }
    R = min_distance/2; //initialized
    while (done == false)
    {

        for (int i=0; i<K; ++i)
        {
            clusterset = rangeSearch(clusters[i].getCentroid(), R);
            clusters[i].setClusteredPoints(clusterset);
            assigned = assigned+clusters[i].getClusteredPoints().size();
        }
       if (assigned >= VALUE)
       {
           //most points have been assigned. we are done
           done = true;
       }   
        R=R*2;
    }
}

void RangeSearchLSH::reCenter(vector<Cluster> clusters, int K)
{
    Item *candidate;
    double distance;
    double min_distance;
    Item *mean_centroid;
    double sum = 0;
    double new_coordinate;

    //centroids initialization
    for (int i = 0; i < K; ++i)
    {
        //for every cluster
        vector<int> new_centroid;
        for (int j = 0; j < clusters[i].getCentroid()->get_d(); ++j)
        {
            //for every dimension
            sum = 0;
            for (int k = 0; k < clusters[i].getClusteredPoints().size(); ++k)
            {
                //for every point
                sum = sum + clusters[i].getClusteredPoints()[k]->get_coordinates()[j];
            }
            new_coordinate = ceil(sum / clusters[i].getClusteredPoints().size());
            // add dimension calculated to the centroid point
            new_centroid.push_back(new_coordinate);
        }
        //initialize centroid as Item with coordinates calculated above
        clusters[i].setPrevCentroid(clusters[i].getCentroid());
        clusters[i].setCentroid(new Item(new_centroid));
    }
}

void RangeSearchLSH::execAlgorithm(vector<Item *> points, vector<Cluster> clusters, int K)
{
    // points in L hashtables
    // (algorithm initialized, we have rand centroids)
    // for each centroid: addtocluster

     int count = 0;
    bool converged = false;
      cout << "Executing Range Search LSH algorithm." <<endl;
    //first assignment
    for (Item *point : points)
    {
        addtoCluster(point, clusters, K);
    }

    while (converged == false)
    {
        reCenter(clusters, K);
        for (Item *point : points)
        {
            addtoCluster(point, clusters, K);
        }
        converged = converge(clusters, K);
        count++;
    }
    if (converged == true)
    {
        //check unassigned points

        for (Item *point : points)
        {
            // for (is_point_clustered* p: already_assigned)
            // {
            //     if (already_assigned.second == point)
            //     {
            //         if (already.assigned.first === false)
            //         {
            //             //cluster push back to cluster with mindistance(centroid, point)
            //             addtoCluster(point, clusters);
            //         }
            //     }
            // }
        }
        //for every point
        //through vector of assigned items
        ///if not found, cluster push back to cluster with mindistance(centroid, point)
        //else next point
    }
};