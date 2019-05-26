
#include "../GMLParser/gml_parser.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <numeric>
#include <limits>
#include <iostream>
#define DEBUG_PRINT 0
#define BETA 0.8f
#define FLAG_ITERATION_STOP -27
#define MAX_ITERATIONS 50  

using namespace std;
using namespace GMLParser;

//computes the sumation of 1 nodes rank 
double sum_node_rank(vector<double> currentRanks, vector<vector<int>> adjlist, int node, vector<int> outdegrees){
    double sum = 0;
    int size = adjlist[node].size();

    for (unsigned int i = 0; i < size; i++)
    {   
        //update sum by run of the neighbour node divided by outdegree of that neighbour
        sum = sum + currentRanks[adjlist[node][i]]/ (double) outdegrees[adjlist[node][i]]; 
        
    }
    return sum;
}

//computes 1 iteration of rank values computation
void  performIteration( vector<double> &r_new,vector<double> &r_old, vector<vector<int>> &adjlist
        , vector<int> &outdegrees)
{
 
    for (int i = 0; i < adjlist.size(); ++i) {
        double sum = 0;
        for (int j = 0; j < adjlist[i].size(); ++j) {
            if (outdegrees[adjlist[i][j]] != 0)
                sum += r_old[adjlist[i][j]] / (double) outdegrees[adjlist[i][j]];
        }
        r_new[i] = sum * BETA;
       
    }
}


//checks if computation has converged or not
bool converges(vector<double> &before, vector<double> &after, double threshhold){
    double tmpSum1 = accumulate(before.begin(), before.end(), 0.0);
    double tmpSum2 = accumulate(after.begin(), after.end(), 0.0);
    if (fabs(tmpSum1 - tmpSum2) > threshhold)
        return false;
    return true;
}


double accumualtePartialSum( vector<double> r_vector){
    double sum = 0;
    for (int i = 0; i < r_vector.size(); ++i) {
        sum += r_vector[i];
    }
    return sum;
}

int
main()
{

    GraphAdjList x;
    double threshhold = (double)1/1000000;
    double global_leakage_sum;
    int i;

    int size,outdegrees_size;
    x = CreateAdjListFromFile("test_example.txt");
    //x = CreateAdjListFromFile("../GMLParser/web-Google.txt");
    size = x.adj_list.size();
    outdegrees_size = x.vertex_ids.size();

    vector<int> outdegrees(size);
    for (unsigned int i = 0; i < size; i++)
    {
        outdegrees[i] = x.vertex_ids[i].out_degree_;
    }
    
    vector<int> individual_sizes_of_adj_lists_in_graph(size);
    

    vector<double> r_new(size); 
    vector<double> r_old(size); 

    for(i = 0; i < size; i++){
        r_old[i] = (double)1/size;
    }

    int iteration = 0;
    int stop = FLAG_ITERATION_STOP;

    do{
        
        performIteration(r_new, r_old, x.adj_list, outdegrees);     //deals with one iteration in master (exactly same in children)
        
        double S = accumualtePartialSum(r_new);
        printf("The leakage %f\n",S);
        // NOTE: Since all processes have the smae I think each processor should add 1-S / size in the noraml computation;
        // the master should not do all this as this is parallellizable
        for (int i = 0; i < size; ++i) {
            r_new[i] += (double) (1 - S) / size;
        }
        iteration++;
    
    }while (!converges(r_old, r_new,threshhold) && iteration < MAX_ITERATIONS);
    
   
    cout.precision(std::numeric_limits<double>::max_digits10);
    for (int i = 0; i < r_new.size(); i++)
    {
            cout << i << "->" << r_new  [i] << endl;
    }
    return 0;
}


