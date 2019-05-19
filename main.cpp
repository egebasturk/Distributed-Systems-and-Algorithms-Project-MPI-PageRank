#include <iostream>
#include "Util.h"

int main (int argc, char *argv[])
{
    /// MPI initialization
    MPI_Status s;
    int size, rank, N;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    int color;


    // TODO: read these
    int* vector = NULL;
    int** matrix = NULL;
    // TODO: Bcast according to read dimension
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);   // Bcast array dimension


    // TODO: Cleanup
    MPI_Finalize();
}
int* matrixVectorMultParallelMPI(int** mat, int* vector, int dim, int size, int myRank)
{
    /// Dividing the matrix into rows
    int rowTileWidth = dim / size;
    int myRowOffset = myRank * rowTileWidth;

    int** recvMatBuffer = (int**)malloc(rowTileWidth * dim * sizeof(int)); // Allocated 1 continuous array for all rows
    int* recvVecBuffer = (int*)malloc(dim * sizeof(int));
    int* outVecBuffer = (int*)malloc(dim * sizeof(int));
    int* computedVector = (int*)malloc(rowTileWidth * sizeof(int));
    // TODO: This only accepts divisible dimension. use scatterV later
    /// Master scatters, so master should have it
    MPI_Scatter(*mat
            , dim * rowTileWidth, MPI_INT, *recvMatBuffer
            , dim * rowTileWidth, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector
            , dim, MPI_INT, 0, MPI_COMM_WORLD);

    /// Compute
    for (int i = 0; i < rowTileWidth; ++i) {
        for (int j = 0; j < dim; ++j) {
            computedVector[i] += recvMatBuffer[i][j] * recvVecBuffer[j];
        }
    }

    MPI_Gather(computedVector, rowTileWidth, MPI_INT, outVecBuffer, rowTileWidth
            , MPI_INT, myRank,  MPI_COMM_WORLD);

}