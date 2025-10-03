
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

int main() {
    // Print current working directory for debugging
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);

    // Attempt to open the file
    FILE* file = fopen("C:\\Users\\Dell\\OneDrive\\Documents\\c program assignment\\day1\\input.txt", "r");
    int i, j;
    if (!file) {
        perror("Error opening file"); // Prints specific file error
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

    // Calculate similarity score
    long similarity_score = 0;
    for (i = 0; i < count; i++) {
        int current = left_list[i];
        int occurrences = 0;
        for (j = 0; j < count; j++) {
            if (right_list[j] == current) {
                occurrences++;
            }
        }
        similarity_score += (long)current * occurrences;
    }

    // Output result
    printf("%ld\n", similarity_score);

    // Cleanup
    free(left_list);
    free(right_list);

    return 0;
}

