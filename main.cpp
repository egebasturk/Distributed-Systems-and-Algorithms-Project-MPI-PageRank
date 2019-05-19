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
    int rowTileWidth = dim / size;
    int myRowOffset = myRank * rowTileWidth;

    int* recvBuffer = (int*)malloc(rowTileWidth * dim * sizeof(int));
    MPI_Scatter(mat
            , dim * rowTileWidth, MPI_INT, recvBuffer
            , dim * rowTileWidth, MPI_INT, 0, MPI_COMM_WORLD);

}