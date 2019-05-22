#include <mpi.h>
#include "../GMLParser/gml_parser.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
using namespace GMLParser;



//computes the sumation of 1 nodes rank 
double sum_node_rank(vector<double> currentRanks, vector<vector<int>> adjlist, int node){
    double sum = 0;
    int size = adjlist.size();

    for (unsigned int i = 0; i < size; i++)
    {   
        //if (node == i) continue;
        if (find(adjlist[i].begin(), adjlist[i].end(), node) != adjlist[i].end()){
            sum = sum + currentRanks[i]/ adjlist[i].size(); 
        }
    }

    return sum;
}

//computes 1 iteration of rank values computation
vector<double>  performIteration( vector<double> r,vector<double> ranks, size_t size, vector<vector<int>> adjlist, int offset){
    for (unsigned int i = 0; i < size; i++)
    {
        r[i] = sum_node_rank(ranks,adjlist,offset + i);  
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


int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    double threshhold = (double)1/1000000;
    int maxIter = 50;

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0){
        auto x = CreateAdjListFromFile("test_example.txt");
        int i;
        size_t size = x.adj_list.size();
         
        vector<double> r_before_init(size); 

        for(i = 0; i < size; i++){
            r_before_init[i] = (double)1/size;
        }
        
        //pass the size
        MPI_Bcast(&size,1, MPI_INT, 0 , MPI_COMM_WORLD);

         int colSize;
        //broadcast the matrix
        for (int i = 0; i < x.adj_list.size(); ++i)
        {   
            colSize = x.adj_list[i].size();

            MPI_Bcast(&colSize, 1, MPI_INT, 0 , MPI_COMM_WORLD);
            MPI_Bcast(&x.adj_list[i][0], x.adj_list[i].size(), MPI_INT, 0 , MPI_COMM_WORLD);  

        }
        //int offset;
        int msgSize = size/world_size;
        vector<double> r_after(msgSize); 
        vector<double> r_before(size); 
        r_before  = r_before_init;
        int iteration = 1;
        int stop = -27;

        do{
            MPI_Bcast(&iteration, 1, MPI_INT, 0 , MPI_COMM_WORLD);              //tell children if they should stop or not
            r_before_init = r_before;                                           //update rank vector
            MPI_Bcast(&r_before_init.front(), size, MPI_DOUBLE, 0 , MPI_COMM_WORLD);    //let all have the rank vector
            MPI_Scatter(&r_before_init.front(),msgSize,MPI_DOUBLE, &r_after.front(),msgSize,MPI_DOUBLE,0,MPI_COMM_WORLD); // only send the parts of the matrix that each processor needs to compute
            r_after = performIteration(r_after,r_before_init,msgSize,x.adj_list,0);     //deals with one iteration in master (exactly same in children)
            MPI_Gather(&r_after.front(), msgSize, MPI_DOUBLE, &r_before.front(), msgSize, MPI_DOUBLE, 0,MPI_COMM_WORLD);  //gather results into r_before (master included) 
            iteration++;   
        }while (!converges(r_before, r_before_init,threshhold) && iteration < maxIter);
        
        //stop children as well
        MPI_Bcast(&stop, 1, MPI_INT, 0 , MPI_COMM_WORLD);

        //output the results
        for (unsigned int i = 0; i < r_before.size(); i++)
        {
                printf("%d -> ", i  );
                printf("%f, ", r_before[i]);
                printf("\n");
        }

    } else {
        int size,colSize;

        //recieve the initial size of the matrix
        MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        vector<vector<int>>adjList(size);
     
        //receive the whole matrix
        for (int i = 0; i < size; i++){
            MPI_Bcast(&colSize, 1, MPI_INT, 0 , MPI_COMM_WORLD);
            adjList[i] = vector<int>(colSize); 
            MPI_Bcast(&adjList[i][0], colSize, MPI_INT, 0 , MPI_COMM_WORLD);
        }

        //int offset;
        int msgSize = size/world_size;
        vector<double> r_before(msgSize);
        vector<double> r_after(msgSize);
        vector<double> ranks(size);
        int iteration = 1;
        int offset = world_rank* msgSize;

        do{
            MPI_Bcast(&iteration, 1, MPI_INT, 0 , MPI_COMM_WORLD);

            if (iteration > 0){
                MPI_Scatter(&r_before.front(),msgSize,MPI_DOUBLE, &r_after.front(),msgSize,MPI_DOUBLE,0,MPI_COMM_WORLD);
                
                MPI_Bcast(&ranks.front(), size, MPI_DOUBLE, 0 , MPI_COMM_WORLD);                

                r_after = performIteration(r_after,ranks,msgSize,adjList,offset);

                MPI_Gather(&r_after.front(), msgSize, MPI_DOUBLE, &r_before.front(), msgSize, MPI_DOUBLE, 0,MPI_COMM_WORLD);
        
            }
          
        }  while (iteration > 0);
      
       
    }
 
    // Finalize the MPI environment.
    MPI_Finalize();
}
