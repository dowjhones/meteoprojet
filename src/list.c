#include "list.h"
#include "array.h"

// Get a node from its index
linked_list_t *get_node_from_index(linked_list_t *list, int index)
{
    while (index > 0) {
        list = list->next; 
        index--; 
    }
    return (list);
}

int get_list_size(linked_list_t *list)
{
    int i = 0;

    while (list != NULL) {
        list = list->next;
        i++;
    }
    return (i);
}

int add_to_list(linked_list_t **list,  data_t *data, int index)
{
    linked_list_t *node = *list;
    linked_list_t *new = malloc(sizeof(linked_list_t));

    if (new == NULL) {
        MEMORY_ERR();
        return (1);
    }
    new->next = NULL
    new->data = data;
    if (node == NULL) {
        *list = new;
        return (0);
    }
    if (index == 0) {
        new->next = node;
        *list = new;
        return (0);
    }
    while (node->next != NULL && index > 1) { 
        node = node->next; 
        index--;
    }
    node->next = new;
    return (0);
}

void free_list(linked_list_t *list)
{
    linked_list_t *prev;

    while (list != NULL) {
        prev = list; 
        list = list->next; 
        for (int i = 0; prev->data->value[i] != NULL; i++)
            free(prev->data->value[i]); 
        free(prev->data->value);
        free(prev->data); 
        free(prev);
    }
}

data_t *create_data(double key, char *value)
{
    data_t *data = malloc(sizeof(data_t));
	if (data == NULL)
		return (NULL);
	data->key = key;
	data->value = NULL;
    if (add_to_array(&data->value, value) == 1)
		return (NULL);
    data->nb = 1;
    return (data);
}

linked_list_t *list_insert(linked_list_t **list, double key, int reverse, char *value)
{
    data_t *data;
    linked_list_t *node = *list; 
    int index = 0;
    if (node == NULL) {
        data = create_data(key, value);
        if (data == NULL)
            return (NULL);
        add_to_list(list, data, 0);
    }
    else { 
        while(node != NULL) {
            if (node->data->key == key) { 
                node->data->nb++; 
                if (add_to_array(&node->data->value, value) == 1)
					return (NULL);
                return (*list); 
            }
            if (compare(node->data->key, key, reverse)) {
                data = create_data(key, value); 
                add_to_list(list, data, index);
                return (*list); 
            }
            node = node->next; 
            index++; 
        }
        data = create_data(key, value);
        add_to_list(list, data, index); 
    }
    return (*list);
}

// save linked list in the file
void list_save(linked_list_t *list, FILE *f) 
{
    while (list != NULL) { 
        // For the node, write every values with his key in his temporarie output file
        for (int i = 0; i < list->data->nb; i++) 
	        fprintf(f, "%f;%s\n", list->data->key, list->data->value[i]); 
        list = list->next;
    }
}

// parse a buffer and create a sorted linkedlist and write sorted values in the file
int list(FILE *file, char buffer[RAM], int reverse) 
{
	double key = 0; 
	char *line; 
    char *key_buffer;
	char *value;
	linked_list_t *list = NULL;
    char *p;

    // split the buffer by lines
    line = strtok_r(buffer, "\n",  &p);
    while (line != NULL) { 
		if (line[0] == '\n') 
			continue;
        // split the line by ; one time to get the key to be sorted and the value to be saved
		// key is the first column of the line
		key_buffer = strtok(line, ";"); 
        // value is the rest of the line
    	value = strtok(NULL, "");
        // get a double value from the key string
		str_to_double(&key, key_buffer); 
        // add the new value in the tree
        list_insert(&list, key, reverse, value);
        // go to the next line
        line = strtok_r(NULL, "\n", &p); 
    }
    // save linked list in the file
	list_save(list, file); 
    // free memory
	free_list(list); 
	return (0);
}