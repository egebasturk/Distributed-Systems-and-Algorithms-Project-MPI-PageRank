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

    int size, colSize;
    GraphAdjList x;

    /// Master reads and broadcasts size
    if (world_rank == 0) {
        x = CreateAdjListFromFile("test_example.txt");
        size = x.adj_list.size();
    }
    /// All call broadcast
    MPI_Bcast(&size,1, MPI_INT, 0 , MPI_COMM_WORLD);
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
    int remainder = size % world_size;
    partitions[0] += remainder;


    if (world_rank == 0)
    {
        vector<double> r_before_init(size, (double) 1 / size);
        //broadcast the matrix
        for (int i = 1, j = 1, counter = 0; i < x.adj_list.size(); ++i)
        {   
            colSize = x.adj_list[i].size();
            //MPI_Bcast(&colSize, 1, MPI_INT, 0 , MPI_COMM_WORLD);
            //MPI_Bcast(&x.adj_list[i][0], x.adj_list[i].size(), MPI_INT, 0 , MPI_COMM_WORLD);
            if (counter > partitions[j])
            {
                counter = 0;
                j++;
            }
            MPI_Send(&x.adj_list[i][0], x.adj_list[i].size(), MPI_INT, j, 0, MPI_COMM_WORLD);
            counter++;
        }
        //int offset;
        int msgSize = size/world_size;
        vector<double> r_after(msgSize); 
        vector<double> r_before(size, (double) 1 / size);
        //r_before  = r_before_init;
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
        vector<vector<int>>adjList(size);

        for (int i = 0; i < partitions[world_rank]; i++){
            //MPI_Bcast(&colSize, 1, MPI_INT, 0 , MPI_COMM_WORLD);
            adjList[i] = vector<int>(individual_sizes_of_adj_lists_in_graph[world_rank]);
            //MPI_Bcast(&adjList[i][0], colSize, MPI_INT, 0 , MPI_COMM_WORLD);
            MPI_Recv(&adjList[i][0], colSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
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