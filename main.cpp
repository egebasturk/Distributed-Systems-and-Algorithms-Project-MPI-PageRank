#include "Util.h"
using namespace std;
int* matrixVectorMultParallelMPI(int* mat, int* vector, int dim, int size, int myRank);

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
    int* matrix = NULL;
    if (rank == 0)
    {
        string s1 = "../mat1_4x4";
        string s2 = "../vect1_4";
        readMatrixFromFile(&matrix, s1.c_str(), &N);
        readVectorFromFile(&vector, s2.c_str(), &N);
    }
    /*for (int i = 0; i < N * N; ++i) {
        printf("%d\t", matrix[i]);
    }
    printf("\n");
    for (int i = 0; i < N; ++i) {
        printf("%d\t", vector[i]);
    }*/
    // TODO: Bcast according to read dimension
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);   // Bcast array dimension
    matrixVectorMultParallelMPI(matrix, vector, N, size, rank);


    // TODO: Cleanup
    MPI_Finalize();
}
int* matrixVectorMultParallelMPI(int* mat, int* vector, int dim, int size, int myRank)
{
    /// Dividing the matrix into rows
    int rowTileWidth = dim / size;
    printf("RTW: %d\n", rowTileWidth);
    int myRowOffset = myRank * rowTileWidth;

    int* recvVecBuffer;
    int* recvMatBuffer = (int*)malloc(rowTileWidth * dim * sizeof(int)); // Allocated 1 continuous array for all rows
    if (myRank == 0)
        recvVecBuffer = vector;
    else
        recvVecBuffer = (int*)malloc(dim * sizeof(int));
    int* outVecBuffer = (int*)malloc(dim * sizeof(int));
    int* computedVector = (int*)calloc( sizeof(int), rowTileWidth);
    // TODO: This only accepts divisible dimension. use scatterV later
    /// Master scatters, so master should have it
    MPI_Scatter(mat
            , dim * rowTileWidth, MPI_INT, recvMatBuffer
            , dim * rowTileWidth, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Rank: %d\n", myRank);
    MPI_Barrier(MPI_COMM_WORLD);
    for (int k = 0; k < dim * rowTileWidth; ++k) {
        printf("%d \t", recvMatBuffer[k]);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(recvVecBuffer
            , dim, MPI_INT, 0, MPI_COMM_WORLD);
    printf("\n");

    for (int l = 0; l < dim; ++l) {
        printf("%d\t", recvVecBuffer[l]);
    }
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);

    /// Compute
    for (int i = 0; i < rowTileWidth; ++i) {
        for (int j = 0; j < dim; ++j) {
            computedVector[i] += recvMatBuffer[(i) * dim + j]
                    * recvVecBuffer[j];
            printf("Index: %d\n",(myRowOffset + i) * dim + j);
        }
        printf("%d\t", computedVector[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(computedVector, rowTileWidth, MPI_INT, outVecBuffer, rowTileWidth
            , MPI_INT, myRank,  MPI_COMM_WORLD);
    printf("Rank: %d", myRank);
}