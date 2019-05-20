#include <mpi.h>
#include "../GMLParser/gml_parser.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
using namespace GMLParser;



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
vector<double>  performIteration( vector<double> r, size_t size, vector<vector<int>> adjlist){
    for (unsigned int i = 0; i < adjlist.size(); i++)
    {
        r[i] = sum_node_rank(r,adjlist,i);  
    }
    return r;
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
        auto x = CreateAdjListFromFile("test_gml.txt");
        int i;
        size_t size = x.adj_list.size();
        vector<double> r_after(size); 
        vector<double> r_before(size); 

        for(i = 0; i < size; i++){
            r_before[i] = (double)1/size;
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
            
            int offset;
            int msgSize = size/world_size;
            for (int j = 1; j < world_size; j++){
                offset = (j-1)*size/world_size;
                MPI_Send(&r_before[offset],msgSize,MPI_DOUBLE,j,0,MPI_COMM_WORLD);
            }
            for (int j = 1; j < world_size; j++){
                offset = (j-1)*size/world_size;
                MPI_Recv(&r_after[0],msgSize,MPI_DOUBLE,0,j,MPI_COMM_WORLD,MPI_Status* status);
            }  
    
        }
        
        

    } else {
        int size,colSize;

        //recieve the initial size of the matrix
        MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        printf("Hello %d\n",size);
        vector<vector<int>>adjList(size);

        //receive the whole matrix
        for (int i = 0; i < size; i++){
            MPI_Bcast(&colSize, 1, MPI_INT, 0 , MPI_COMM_WORLD);
            adjList[i] = vector<int>(colSize); 
            MPI_Bcast(&adjList[i][0], colSize, MPI_INT, 0 , MPI_COMM_WORLD);
        }

        int offset;
        int msgSize = size/world_size;
        
        offset = world_rank*size/world_size; 
        vector<double> r_before(msgSize);

        MPI_Recv(&r_before[0],msgSize,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_Status* status);

        for (int i = 0; i < msgSize; i++ ){
            r_after = performIteration(r_before,colSize,adjList);
        }
        
        MPI_Send(&r_after[0],msgSize,MPI_DOUBLE,0,world_rank,MPI_COMM_WORLD);


    }
 

    // Finalize the MPI environment.
    MPI_Finalize();
}
