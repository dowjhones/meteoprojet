#include "utils.h"

// Free the array
void free_array(char **array)
{
    if (array == NULL)
        return;
    for (int i = 0; array[i] != NULL; i++)
        free(array[i]);
    free(array);
}

// Get the number of elements in the array
int get_array_len(char **array)
{
    int i = 0;

    if (array == NULL)
        return (0);
    for(; array[i] != NULL; i++);
    return (i);
}

// Add a new char * in the char **
// If array isn't null, fill the copie array with the current array values 
// add the value at the end of the new array
int add_to_array(char ***array, char *msg)
{
    int len = get_array_len(*array);
    int tmp_len;
    char **new_array = malloc(sizeof(char *) * (len + 2)); // Allocate the memory for the new array

    if (new_array == NULL) {
        MEMORY_ERR();
        return (1);
    }
    // Write current array data in the new array
    for (int i = 0; i < len; i++) {
        tmp_len = strlen((*array)[i]);
        new_array[i] = malloc(sizeof(char) * (tmp_len + 1));
        if (new_array[i] == NULL) { 
            MEMORY_ERR();
            return (1);
        }
        for (int j = 0; j < tmp_len; j++) 
            new_array[i][j] = (*array)[i][j]; 
        new_array[i][tmp_len] = '\0';
    }
    tmp_len = strlen(msg); 
    new_array[len] = malloc(sizeof(char) * (tmp_len + 1));
    if (new_array[len] == NULL) {
        MEMORY_ERR();
        return (1);
    }
    // Add the new string to the new array
    for (int i = 0; i < tmp_len; i++)
        new_array[len][i] = msg[i];
    new_array[len][tmp_len] = '\0'; 
    new_array[len + 1] = NULL; 
    free_array(*array); 
    *array = new_array; // Set the array pointer to the new array
    return (0);
}
