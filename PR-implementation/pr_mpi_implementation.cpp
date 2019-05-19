#include <mpi.h>
#include <stdio.h>

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
        
        //broadcast the matrix
        for (int i = 0; i < x.adj_list.size(); ++i)
        {     
            MPI_Bcast(&x.adj_list[i][0], x.adj_list[i].size(), MPI_INT, 0 , MPI_COMM_WORLD);       
        }
        //std::vector<std::vector<int>>


    }
 

    // Finalize the MPI environment.
    MPI_Finalize();
}
