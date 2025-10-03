
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

// Function to swap two integers
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Bubble sort for sorting arrays
void bubble_sort(int* arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

int main() {
    // Print current working directory for debugging
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);

    // Attempt to open the file
    FILE* file = fopen("C:\\Users\\Dell\\OneDrive\\Documents\\c program assignment\\day1\\input.txt", "r");
    int i;
    if (!file) {
        perror("Error opening file"); // Prints specific file error (e.g., "No such file or directory")
        return 1;
    }

    // Arrays to store left and right lists
    int* left_list = malloc(1000 * sizeof(int));
    int* right_list = malloc(1000 * sizeof(int));
    int count = 0;
    int max_size = 1000;

    // Read pairs of numbers
    int left, right;
    while (fscanf(file, "%d %d", &left, &right) == 2) {
        if (count >= max_size) {
            max_size *= 2;
            left_list = realloc(left_list, max_size * sizeof(int));
            right_list = realloc(right_list, max_size * sizeof(int));
        }
        left_list[count] = left;
        right_list[count] = right;
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("Empty input file\n");
        free(left_list);
        free(right_list);
        return 1;
    }

    // Sort both lists
    bubble_sort(left_list, count);
    bubble_sort(right_list, count);

    // Calculate total distance
    long total_distance = 0;
    for (i = 0; i < count; i++) {
        int diff = left_list[i] - right_list[i];
        if (diff < 0) diff = -diff; // Absolute value
        total_distance += diff;
    }

    // Output result
    printf("%ld\n", total_distance);

    // Cleanup
    free(left_list);
    free(right_list);

    return 0;
}
