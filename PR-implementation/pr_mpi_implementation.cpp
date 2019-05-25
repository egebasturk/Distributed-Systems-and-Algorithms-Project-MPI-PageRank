#include <mpi.h>
#include "../GMLParser/gml_parser.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <numeric>
#define DEBUG_PRINT 0
#define BETA 0.8f
using namespace std;
using namespace GMLParser;



//computes the sumation of 1 nodes rank 
double sum_node_rank(vector<double> currentRanks, vector<vector<int>> adjlist, int node,vector<int> outdegrees){
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
vector<double>  performIteration( vector<double> r_new,vector<double> r_old, vector<vector<int>> adjlist
        , vector<int> partitions, vector<int> displacements, vector<int> individual_sizes_of_adj_lists_in_graph
        , int my_rank)
{
    /*for (unsigned int i = 0; i < size; i++)
    {
        //r_new[i] = sum_node_rank(r_old,adjlist,offset + i, outdegrees);
        r_new[i] = sum_node_rank(r_old,adjlist,i, outdegrees);

    }
    return r_new;
     */
    for (int i = displacements[my_rank]; i < displacements[my_rank] + partitions[my_rank]; ++i) {
        int sum = 0;
        for (int j = 0; j < individual_sizes_of_adj_lists_in_graph[i]; ++j) {
            sum += r_old[adjlist[i][j]];
        }
    }
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

    int size, colSize,outdegrees_size;
    GraphAdjList x;
    
    /// Master reads and broadcasts size
    if (world_rank == 0) {
        x = CreateAdjListFromFile("test_example.txt");
        x = CreateAdjListFromFile("web-Google.txt");
        size = x.adj_list.size();
    }

    /// All call broadcast
    MPI_Bcast(&size,1, MPI_INT, 0 , MPI_COMM_WORLD);
    MPI_Bcast(&outdegrees_size,1, MPI_INT, 0 , MPI_COMM_WORLD);

    // Everyone will create this vector, master will fill it
    vector<int> individual_sizes_of_adj_lists_in_graph(size);
    /// Then master continues
    if (world_rank == 0) {
        for (int j = 0; j < size; ++j) {
            individual_sizes_of_adj_lists_in_graph[j] = x.adj_list[j].size();
        }
    }
     /// All call Broadcast
    MPI_Bcast(&individual_sizes_of_adj_lists_in_graph[0], size, MPI_INT, 0, MPI_COMM_WORLD);
    /// Calculate for load balancing
    int tmp_vector_sum = accumulate(individual_sizes_of_adj_lists_in_graph.begin(),
            individual_sizes_of_adj_lists_in_graph.end(), 0);

    vector<int> partitions(size, size / world_size);
    vector<int> displacements(size, size);
    int remainder = size % world_size;
    partitions[0] += remainder;

    displacements[0] = 0;
    for (int k = 1; k < size; ++k) {
        displacements[k] = displacements[k - 1] + partitions[k - 1];
    }
    
    if (world_rank == 0){
        int i;

        /// send the partial matrix
        for (int i = displacements[1], j = 1, counter = 0; i < x.adj_list.size(); ++i)
        {   
            if (counter >= partitions[j])
            {
                counter = 0;
                j++;
            }
#if DEBUG_PRINT
            printf("master sent to %d: index %d\n", j, i);
#endif
            MPI_Send(&x.adj_list[i][0], x.adj_list[i].size(), MPI_INT, j, 0, MPI_COMM_WORLD);
            counter++;
           
        }
        // message size for the rank vector computation
        int msgSize = size/world_size;
        vector<double> r_new(msgSize);
        vector<double> r_old(size, (double) 1 / size);;

        int iteration = 1;
        int stop = -27;

        do{
            
            MPI_Bcast(&iteration, 1, MPI_INT, 0 , MPI_COMM_WORLD);              //tell children if they should stop or not
            MPI_Bcast(&r_old.front(), size, MPI_DOUBLE, 0 , MPI_COMM_WORLD);    //let all have the rank vector
            
            r_new = performIteration(r_new, r_old, x.adj_list, partitions, displacements, individual_sizes_of_adj_lists_in_graph, world_rank);     //deals with one iteration in master (exactly same in children)
            
            //MPI_Gather(&r_new.front(), msgSize, MPI_DOUBLE, &r_old.front(), msgSize, MPI_DOUBLE, 0,MPI_COMM_WORLD);  //gather results into r_old (master included)
            int* recv_buffer = (int*)malloc(partitions[world_rank] * sizeof(int));
            MPI_Gatherv(&r_new.front(), partitions[0], MPI_DOUBLE, &recv_buffer, &partitions.front()
                    , &displacements.front(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
            
            iteration++;   
       
            
        }while (!converges(r_old, r_old,threshhold) && iteration < maxIter);
        
        //stop children as well
        MPI_Bcast(&stop, 1, MPI_INT, 0 , MPI_COMM_WORLD);
        
        //output the results
        for (unsigned int i = 0; i < r_old.size(); i++)
        {
                printf("%d -> ", i  );
                printf("%f, ", r_old[i]);
                printf("\n");
        }

    } else {
      
        vector<vector<int>>adjList(size);
        
        ///receive the partial matrix
        for (int i = displacements[world_rank]; i < displacements[world_rank] + partitions[world_rank]; i++){
            adjList[i] = vector<int>(individual_sizes_of_adj_lists_in_graph[i]);
            MPI_Recv(&adjList[i][0], adjList[i].size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        //int offset;
        int msgSize = size/world_size;
        vector<double> r_old(size);
        vector<double> r_new(size);
        int iteration = 1;
        int offset = world_rank* msgSize;
        
        do{
            MPI_Bcast(&iteration, 1, MPI_INT, 0 , MPI_COMM_WORLD);

            if (iteration > 0)
            {
                MPI_Bcast(&r_old.front(), size, MPI_DOUBLE, 0 , MPI_COMM_WORLD);

                r_new = performIteration(r_new, r_old, adjList, partitions, displacements, individual_sizes_of_adj_lists_in_graph, world_rank);
     
                //MPI_Gather(&r_new[displacements[world_rank]], partitions[world_rank]
                        //, MPI_DOUBLE, &r_old.front(), msgSize, MPI_DOUBLE, 0,MPI_COMM_WORLD);
                MPI_Gatherv(&r_new[displacements[world_rank]], partitions[world_rank], MPI_DOUBLE
                        , NULL, 0, &displacements.front(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
            }
          
        }  while (iteration > 0);
      
       
    }
 
    // Finalize the MPI environment.
    MPI_Finalize();
}