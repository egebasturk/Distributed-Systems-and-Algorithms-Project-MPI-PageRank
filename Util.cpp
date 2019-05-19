#include "Util.h"


int getLineCount(FILE* fptr)
{
    int readNum;
    int count = 0;
    for (char c = getc(fptr); c != EOF; c = getc(fptr))
        if (c == '\n')
            count++;
    rewind(fptr); // Don't effect the location
    if ( count != 0 )
        count++;
    return count;
}

int readMatrixFromFile(int** arr, const char* input_filename, int *N)
{
    FILE *fptr;
    fptr = fopen(input_filename, "r");
    int lineCount = getLineCount(fptr);
    *N = lineCount;

    if (fptr == NULL)
    {
        printf("Error reading file");
        return 0;
    } else
    {
        int index = 0;
        int read_num;

        int* return_array = (int*)malloc(*N * *N * sizeof(int));
        while (fscanf(fptr, "%d", &read_num) == 1)
        {
            return_array[index] = read_num;
            index++;
        }
        *arr = (int*)return_array;
        return index--;
    }
}
int readVectorFromFile(int** arr, const char* input_filename, int *N)
{
    FILE *fptr;
    fptr = fopen(input_filename, "r");
    int lineCount = getLineCount(fptr);
    *N = lineCount;

    if (fptr == NULL)
    {
        printf("Error reading file");
        return 0;
    } else
    {
        int index = 0;
        int read_num;

        int* return_array = (int*)malloc(*N * sizeof(int));
        while (fscanf(fptr, "%d", &read_num) == 1)
        {
            return_array[index] = read_num;
            index++;
        }
        *arr = return_array;
        return index--;
    }
}