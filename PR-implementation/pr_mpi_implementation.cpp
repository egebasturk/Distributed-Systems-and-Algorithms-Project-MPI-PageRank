#include <mpi.h>
#include "../GMLParser/gml_parser.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <numeric>
#include <limits>
#define DEBUG_PRINT 0
#define BETA 0.8f
#define FLAG_ITERATION_STOP -27
#define MAX_ITERATIONS 50   
using namespace std;
using namespace GMLParser;

//computes 1 iteration of rank values computation
/*vector<double>*/void  performIteration( vector<double> &r_new,vector<double> &r_old, vector<vector<int>> &adjlist
        , vector<int> &partitions, vector<int> &displacements, vector<int> &individual_sizes_of_adj_lists_in_graph
        , int my_rank, vector<int> &outdegrees)
{
    //vector<double> tmp_vector (r_new.size());
    for (int i = displacements[my_rank]; i < displacements[my_rank] + partitions[my_rank]; ++i) {
        double sum = 0;
        for (int j = 0; j < individual_sizes_of_adj_lists_in_graph[i]; ++j) {
            if (outdegrees[adjlist[i][j]] != 0)
                sum += r_old[adjlist[i][j]] / (double) outdegrees[adjlist[i][j]];
        }
        r_new[i] = sum * BETA;// + (1.0f - BETA) / (double) r_new.size();
       
    }
    //return tmp_vector;
}


//checks if computation has converged or not
bool converges(vector<double> &before, vector<double> &after, double threshhold){
    /*for (unsigned int i = 0; i < before.size(); i++){
        if ( fabs(before[i] - after[i]) > threshhold ) return false;
    }*/
    double tmpSum1 = accumulate(before.begin(), before.end(), 0.0);
    double tmpSum2 = accumulate(after.begin(), after.end(), 0.0);
    if (fabs(tmpSum1 - tmpSum2) > threshhold)
        return false;
    return true;
}


double accumualtePartialSum( vector<int> &partitions, vector<int> &displacements, int my_rank, vector<double> &r_vector){
    double sum = 0;
    for (int i = displacements[my_rank]; i < displacements[my_rank] + partitions[my_rank]; ++i) {
        sum += r_vector[i];
    }
    return sum;
}

int main(int argc, char** argv) {
  // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    double threshhold = (double)1/100000;

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size, colSize,outdegrees_size;
    double global_leakage_sum, local_leakage_sum;
    GraphAdjList x;
    
    /// Master reads and broadcasts size
    if (world_rank == 0) {
        x = CreateAdjListFromFile("test_example.txt");
        //x = CreateAdjListFromFile("../GMLParser/web-Google.txt");
        size = x.adj_list.size();
        outdegrees_size = x.vertex_ids.size();
    }

    /// All call broadcast
    MPI_Bcast(&size,1, MPI_INT, 0 , MPI_COMM_WORLD);
    MPI_Bcast(&outdegrees_size,1, MPI_INT, 0 , MPI_COMM_WORLD);

    vector<int> outdegrees(size);
    /// convert the outdegree information into contigous mem/array and broadcast it
    if (world_rank == 0){
        for (unsigned int i = 0; i < size; i++)
        {
            //outdegrees[x.vertex_ids[i].id_] = x.vertex_ids[i].out_degree_;
            outdegrees[i] = x.vertex_ids[i].out_degree_;
        }
    }
    MPI_Bcast(&outdegrees.front(),outdegrees_size, MPI_INT, 0 , MPI_COMM_WORLD);

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
    vector<int> displacements(size);
    int remainder = size % world_size;
    partitions[0] += remainder;

    displacements[0] = 0;
    for (int k = 1; k < size; ++k) {
        displacements[k] = displacements[k - 1] + partitions[k - 1];
    }
    vector<double> r_new(size);
    vector<double> r_old(size, (double) 1 / size);
    if (world_rank == 0)
    {
        /// send the partial matrix
        for (int i = displacements[1], j = 1, counter = 0; i < x.adj_list.size(); ++i)
        {   
            if (counter >= partitions[j])
            {
                counter = 0;
                j++;
            }
   
            MPI_Send(&x.adj_list[i][0], x.adj_list[i].size(), MPI_INT, j, 0, MPI_COMM_WORLD);
            counter++;
           
        }
        int iteration = 1;
        int stop = FLAG_ITERATION_STOP;

        do{
            MPI_Bcast(&iteration, 1, MPI_INT, 0 , MPI_COMM_WORLD);              //tell children if they should stop or not
            MPI_Bcast(&r_old.front(), size, MPI_DOUBLE, 0 , MPI_COMM_WORLD);    //let all have the rank vector
            
            performIteration(r_new, r_old, x.adj_list, partitions
                    , displacements, individual_sizes_of_adj_lists_in_graph
                    , world_rank, outdegrees);     //deals with one iteration in master (exactly same in children)
            
            local_leakage_sum =  accumualtePartialSum( partitions, displacements,world_rank ,r_new);
            MPI_Allreduce(&local_leakage_sum, &global_leakage_sum, 1, MPI_DOUBLE, MPI_SUM,MPI_COMM_WORLD);
            /// Everyone adds S to their parts
            for (int i = displacements[world_rank]; i < displacements[world_rank] + partitions[world_rank]; ++i) {
                    r_new[i] += (double) (1 - global_leakage_sum) / size;
            }

            /// Gather gathers onto the r_old so it will be updated automatically
            MPI_Gatherv(&r_new.front(), partitions[0], MPI_DOUBLE, &r_old.front(), &partitions.front()
                    , &displacements.front(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
            
            /*double S = accumulate(r_old.begin(), r_old.end(), 0.0);
           
            // NOTE: Since all processes have the smae I think each processor should add 1-S / size in the noraml computation;
            // the master should not do all this as this is parallellizable
            for (int i = 0; i < size; ++i) {
                r_old[i] += (double) (1 - S) / size;
            }*/
            iteration++;
       
            
        }while (!converges(r_old, r_new,threshhold) && iteration < MAX_ITERATIONS);
        
        //stop children as well
        MPI_Bcast(&stop, 1, MPI_INT, 0 , MPI_COMM_WORLD);
        
        //output the results
        cout.precision(std::numeric_limits<double>::max_digits10);
        for (int i = 0; i < r_old.size(); i++)
        {
                /*printf("%d -> ", i  );
                printf("%f, ", r_old[i]);
                printf("\n");*/
                cout << i << "->" << r_old[i] << endl;
        }

    } else {
      
        vector<vector<int>>adjList(size);
        
        ///receive the partial matrix
        for (int i = displacements[world_rank]; i < displacements[world_rank] + partitions[world_rank]; i++){
            adjList[i] = vector<int>(individual_sizes_of_adj_lists_in_graph[i]);
            MPI_Recv(&adjList[i][0], adjList[i].size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        int iteration = 1;
        
        do{
            MPI_Bcast(&iteration, 1, MPI_INT, 0 , MPI_COMM_WORLD);

            if (iteration > 0)
            {
                MPI_Bcast(&r_old.front(), size, MPI_DOUBLE, 0 , MPI_COMM_WORLD);
                 
                performIteration(r_new, r_old, adjList, partitions
                        , displacements, individual_sizes_of_adj_lists_in_graph
                        , world_rank, outdegrees);

                local_leakage_sum =  accumualtePartialSum( partitions, displacements,world_rank ,r_new);
                MPI_Allreduce(&local_leakage_sum, &global_leakage_sum, 1, MPI_DOUBLE, MPI_SUM,MPI_COMM_WORLD);

                for (int i = displacements[world_rank]; i < displacements[world_rank] + partitions[world_rank]; ++i) {
                    r_new[i] += (double) (1 - global_leakage_sum) / size;
                }

                //MPI_Gather(&r_new[displacements[world_rank]], partitions[world_rank]
                        //, MPI_DOUBLE, &r_old.front(), msgSize, MPI_DOUBLE, 0,MPI_COMM_WORLD);
                MPI_Gatherv(&r_new[displacements[world_rank]], partitions[world_rank], MPI_DOUBLE
                        , NULL, &partitions.front(), &displacements.front(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
            }
        }  while (iteration > 0);
        for (int j = 0; j < size; ++j) {
            adjList[j].clear();
        }
        adjList.clear();
    }

    if (world_rank == 0)
    {
        x.freeGrap();
    }
    partitions.clear();
    displacements.clear();
    individual_sizes_of_adj_lists_in_graph.clear();
    r_new.clear();
    r_old.clear();
    outdegrees.clear();
    // Finalize the MPI environment.
    MPI_Finalize();
}

