#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int size;
int *array;

struct TArgs
{
    int *arr;
    int left;
    int right;
};

void merge(int arr[], int low, int mid, int high)
{
    int i, j, k;
    int size_left = mid - low + 1;
    int size_right = high - mid;

    // Create temporary arrays
    int left[size_left], right[size_right];

    // Copy data 
    for (i = 0; i < size_left; i++)
        left[i] = arr[low + i];
    for (j = 0; j < size_right; j++)
        right[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr
    i = 0;    
    j = 0;    
    k = low; 
    while (i < size_left && j < size_right)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of left[]
    while (i < size_left)
    {
        arr[k] = left[i];
        i++;
        k++;
    }

    // Copy the remaining elements of right[]
    while (j < size_right)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void *mergeSort(void *args)
{
    struct TArgs *tArgs = (struct TArgs *)args;
    int *arr = tArgs->arr;
    int left = tArgs->left;
    int right = tArgs->right;

    if (left < right)
    {
        int mid = (left + right) / 2;
        // Create threads for the two halves
        pthread_t leftThread, rightThread;
        struct TArgs leftArgs = {arr, left, mid};
        struct TArgs rightArgs = {arr, mid + 1, right};

        pthread_create(&leftThread, NULL, mergeSort, (void *)&leftArgs);
        pthread_create(&rightThread, NULL, mergeSort, (void *)&rightArgs);

        // Wait for the threads to finish
        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }

    pthread_exit(NULL);
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
    fscanf(file, "%d", &size);
    array = (int *)malloc(size * sizeof(int));
    // Read the array from the file
    for (int i = 0; i < size; i++)
    {
        fscanf(file, "%d", &array[i]);
    }
    // Close the file
    fclose(file);
}

int main(int argc, char *argv[])
{
    // user didn't enter filename
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    readFromFile(argv[1]);

    // Create arguments for the initial call
    struct TArgs initialArgs = {array, 0, size - 1};


    clock_t start_time = clock();
    // Create a thread for the initial merge sort
    pthread_t thread;
    pthread_create(&thread, NULL, mergeSort, (void *)&initialArgs);

    // Wait for the initial thread to finish
    pthread_join(thread, NULL);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", elapsed_time);
    // Print the sorted array
    printf("Sorted array: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
    return 0;
}