#include "../GMLParser/gml_parser.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
using namespace GMLParser;

//Following the computation format from: 
// https://rstudio-pubs-static.s3.amazonaws.com/239261_8a607707294341c4b7e26acf728c28bd.html
// and this guy : https://www.youtube.com/watch?v=VpiyOxiVmCg
// each vertex u has incoming vertices v
// the pagerank of all v divided by outgoing degree of v is added to the sum for u.
// the sum for u becomes the new PR


//computes the sumation of 1 nodes rank 
double sum_node_rank(vector<double> currentRanks, GraphAdjList adjlist, int node){
    double sum = 0;
    int size = adjlist.adj_list.size();
    
    for (unsigned int i = 0; i < size; i++)
    {   
        //if (node == i) continue;
        if (find(adjlist.adj_list[i].begin(), adjlist.adj_list[i].end(), node) != adjlist.adj_list[i].end()){
            sum = sum + currentRanks[i]/ adjlist.adj_list[i].size();
        }
    }

    return sum;
}

//computes 1 iteration of rank values computation
vector<double>  performIteration( vector<double> r, size_t size, GraphAdjList adjlist){
    for (unsigned int i = 0; i < adjlist.adj_list.size(); i++)
    {
        r[i] = sum_node_rank(r,adjlist,i);  
    }
    return r;
}

//checks if computation has converged or not
bool converges(vector<double> before, vector<double> after,double threshhold){
    for (unsigned int i = 0; i < before.size(); i++){
        if ( fabs(before[i] - after[i]) > threshhold ) return false;
    }
    return true;
}

int
main()
{
    auto x = CreateAdjListFromFile("test_gml.txt");
    double threshhold = (double)1/1000000;
    int maxIter = 50;
    int i;
    size_t size = x.adj_list.size();
    vector<double> r_after(size); 
    vector<double> r_before(size); 

    for(i = 0; i < size; i++){
        r_before[i] = (double)1/size;
    }

    //for a maximum maxIter iterations compute pagerank
    for (unsigned int i = 0; i < maxIter; i++){
        r_after = performIteration(r_before,size,x);
        if (converges(r_after,r_before,threshhold)){
            i = maxIter;
        }

        for (auto& vertex : r_before)
        {
            printf("%f, ", vertex);
        }

        printf("\n");

        for (auto& vertex : r_after)
        {
            printf("%f, ", vertex);
        }
        printf("\n");
        r_before = r_after;
    }


    return 0;
}


