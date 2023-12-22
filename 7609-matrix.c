#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int **matrix1; // first matrix
int **matrix2; // second matrix
int **output;  // result of matrix1 x matrix2

// dimensions
int m, n, n2, k; // n should be = n2 to be valid matrix multiplication

struct TArgs
{ // arguments to pass to function
    int row;
    int col;
};

// Function to compute a single element in the result matrix
void *multiply(void *args)
{
    struct TArgs *tArgs = (struct TArgs *)args;
    int row = tArgs->row;
    int col = tArgs->col;

    output[row][col] = 0;
    for (int p = 0; p < n; p++)
    {
        output[row][col] += matrix1[row][p] * matrix2[p][col];
    }

    free(args); // Free the allocated memory for thread arguments
    pthread_exit(NULL);
}

void *multiplyRow(void *args)
{
    struct TArgs *tArgs = (struct TArgs *)args;
    int row = tArgs->row;

    for (int col = 0; col < k; col++)
    {
        output[row][col] = 0;
        for (int p = 0; p < n; p++)
        {
            output[row][col] += matrix1[row][p] * matrix2[p][col];
        }
    }

    free(args);
    pthread_exit(NULL);
}

int **allocateMatrix(int row, int col)
{
    // Allocate memory
    int **matrix = (int **)malloc(row * sizeof(int *));
    for (int i = 0; i < row; i++)
    {
        matrix[i] = (int *)malloc(col * sizeof(int));
    }
    return matrix;
}
void freeMatrix(int **matrix, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void readFromFile(char *filename)
{
    FILE *file = fopen(filename, "r");
    // error opening file
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    // read dimensions of matrix1
    fscanf(file, "%d %d", &m, &n);
    // check if multiplication is valid
    if (m <= 0 || n <= 0)
    {
        fprintf(stderr, "Error: Invalid matrix dimensions for multiplication\n");
        exit(EXIT_FAILURE);
    }
    matrix1 = allocateMatrix(m, n);

    // read elements of matrix1
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%d", &matrix1[i][j]);
        }
    }
    // repeat the same for matrix2
    fscanf(file, "%d %d", &n2, &k);
    // check is multiplication is valid
    if (n2 <= 0 || k <= 0 || n != n2)
    {
        fprintf(stderr, "Error: Invalid matrix dimensions for multiplication\n");
        exit(EXIT_FAILURE);
    }
    matrix2 = allocateMatrix(n, k);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            fscanf(file, "%d", &matrix2[i][j]);
        }
    }
    // close file
    fclose(file);
}

void multiplyByElement()
{
    pthread_t threads[m][k];

    // Create threads for each element output matrix
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            struct TArgs *args = (struct TArgs *)malloc(sizeof(struct TArgs));
            args->row = i;
            args->col = j;

            if (pthread_create(&threads[i][j], NULL, multiply, (void *)args) != 0)
            {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            pthread_join(threads[i][j], NULL);
        }
    }
}

void multiplyRows()
{
    pthread_t threads[m];

    // Create threads for each row of the result matrix
    for (int i = 0; i < m; i++)
    {
        struct TArgs *args = (struct TArgs *)malloc(sizeof(struct TArgs));
        args->row = i;

        if (pthread_create(&threads[i], NULL, multiplyRow, (void *)args) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < m; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void printResult(int **matrix)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            printf("%d ", output[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{ // user didn't enter filename
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    readFromFile(argv[1]);

    // allocate memory for result matrix
    output = allocateMatrix(m, k);

    // start time
    clock_t start_time = clock();
    // Multiply matrices
    multiplyByElement();
    // end time
    clock_t end_time = clock();

    printf("By element \n");
    printResult(output);

    // Calculate elapsed time in seconds
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", elapsed_time);

    // start time
    start_time = clock();
    // Multiply matrices
    multiplyRows();
    // end time
    end_time = clock();

    printf("By row \n");
    printResult(output);

    // Calculate elapsed time in seconds
    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", elapsed_time);

    // Free allocated memory
    freeMatrix(matrix1, m);
    freeMatrix(matrix2, n);
    freeMatrix(output, m);
    return 0;
}
