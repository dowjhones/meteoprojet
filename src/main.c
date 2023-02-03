#include "meteo.h"
#include "list.h"
#include "avl.h"

// Init sort function pointers list
// Create options from args
// Create X temp sorted files with the selected sort method
// Merge all files in a sorted output file
// Free all memory allocated
int main(int argc, const char **argv)
{
    int file_nb; 
    external_t **files; 
    int f; 
    options_t *options; 
    // init the function pointer list
    function_pointer_t **functions = get_functions(6, "--abr", &abr, "--avl", &avl, "--tab", &list); 

    if (functions == NULL) 
        return (4);
    options = malloc(sizeof(options_t)); 
    if (options == NULL)
        return (4);
    options->input = NULL;
    options->output = NULL;
    options->reverse = 0;
    options->f = NULL;
    // check fof args
    for (int i = 1; i < argc; i++) { 
        if (!strcmp(argv[i], "-r")) 
            options->reverse = 1; 
        else if (!strcmp("-f", argv[i]) && i < argc - 1) { 
            options->input = fopen(argv[i + 1], "r"); 
            if (options->input == NULL) { 
                free_options(options); 
                return (2);
            }
            i++;
        }
        else if (!strcmp("-o", argv[i]) && i < argc - 1) { 
            options->output = fopen(argv[i + 1], "w"); 
            if (options->output == NULL) { 
                free_options(options);
                return (3);
            }
            i++;
        }
        else {
            f = functions_contains(functions, argv[i]); 
            if (f != -1 &&  options->f != NULL) { 
                free_functions(functions); 
                free_options(options);
                return (1);
            }
            if (f != -1)
                options->f = functions[f]->f;
            else {
                free_functions(functions);
                free_options(options);
                return (1);
            }
        }
    }
    // if not sort mode, select avl by default
    if (options->f == NULL)
        options->f = &avl;
    if (options->input == NULL || options->output == NULL) {
        free_functions(functions);
        free_options(options);
        return (1);
    }
    // create sorted temp files
    file_nb = merge_sort(options); 
    if (file_nb == -1) {
        return (4);
    }
    // open every sorted files
	files = create_files(file_nb);
    if (files == NULL)
        return (4);
    // merge temp files in the output file
    if (sort_files(files, options) == -1) 
        return (4);
    // free memory
    free_functions(functions); 
	free_files(files);
    free(options);
	return (0);
}
