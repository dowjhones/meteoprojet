#include "array.h"
#include "avl.h"
#include "utils.h"

static avl_tree_t *avl_create() // Create an AVL tree
{
	avl_tree_t *tree = NULL;

	if ((tree = malloc(sizeof(avl_tree_t))) == NULL)
		return NULL;
	tree->root = NULL;
	return tree;
}

static avl_node_t *avl_create_node() // Create a empty node for the AVL tree
{
	avl_node_t *node = NULL;
	
	if ((node = malloc(sizeof(avl_node_t))) == NULL)
		return NULL;
	node->left = NULL
	node->right = NULL;
	node->data = malloc(sizeof(data_t));
	if (node->data == NULL)
		return (NULL);
	node->data->key = 0;
	node->data->value = 0;
    node->data->nb = 1;
	return node;	
}

// Get the height of a node recursivly
static int avl_node_height(avl_node_t *node)
{
	int height_left = 0;
	int height_right = 0;

	if (node->left)
		height_left = avl_node_height(node->left);
	if (node->right)
		height_right = avl_node_height(node->right);
	return ((height_right > height_left) ? ++height_right : ++height_left);
}

// Get the balance factor of a node
static int avl_balance_factor(avl_node_t *node)
{
	int bf = 0;

	if (node->left)
		bf += avl_node_height(node->left);
	if (node->right)
		bf -= avl_node_height(node->right);
	return (bf);
}

// Swap the node 2 times left
static avl_node_t *avl_rotate_leftleft(avl_node_t *node) 
{
 	avl_node_t *a = node; 
	avl_node_t *b = a->left; 
	
	a->left = b->right; 
	b->right = a; 
	return( b );
}

// Swap the node left and right
static avl_node_t *avl_rotate_leftright(avl_node_t *node)
{
	avl_node_t *a = node; 
	avl_node_t *b = a->left;
	avl_node_t *c = b->right; 
	
	a->left = c->right; 
	b->right = c->left; 
	c->left = b;
	c->right = a;
	return (c);
}

// Swap the node right and left
static avl_node_t *avl_rotate_rightleft(avl_node_t *node) 
{
	avl_node_t *a = node; 
	avl_node_t *b = a->right; 
	avl_node_t *c = b->left;
	
	a->right = c->left; 
	b->left = c->right; 
	c->right = b; 
	c->left = a;
	return (c);
}

// Swap the node 2 times right
static avl_node_t *avl_rotate_rightright(avl_node_t *node) 
{
	avl_node_t *a = node; 
	avl_node_t *b = a->right;
	
	a->right = b->left;
	b->left = a;
	return (b);
}

// Balance a node
static avl_node_t *avl_balance_node(avl_node_t *node)
{
	avl_node_t *newroot = NULL;
	int bf;

	if (node->left)
		node->left = avl_balance_node(node->left);
	if (node->right)
		node->right = avl_balance_node(node->right); 
	bf = avl_balance_factor(node); 
	if (bf >= 2) { 
		if (avl_balance_factor(node->left) <= -1)
			newroot = avl_rotate_leftright(node); 
		else  
			newroot = avl_rotate_leftleft(node);
	}
	else if (bf <= -2 ) { 
		if (avl_balance_factor(node->right) >= 1)
			newroot = avl_rotate_rightleft(node);
		else 
			newroot = avl_rotate_rightright(node);
	}
	else
		newroot = node; 
	return( newroot ); 
}

// Balance the tree
static void avl_balance( avl_tree_t *tree ) 
{
	avl_node_t *newroot = NULL;

	newroot = avl_balance_node(tree->root);
	if (newroot != tree->root)
		tree->root = newroot; 
}

// Add a new value in the tree
// balance: if true balance the tree after insertion
// reverse: ascending or descending order
// key: value to be sort
// value: value to be stored
static int avl_insert( avl_tree_t *tree, double key, int balance, int reverse, char *value)
{
	avl_node_t *node = NULL;
	avl_node_t *next = NULL;
	avl_node_t *last = NULL;

	// if the tree doesn't exist
	if( tree->root == NULL ) {
		node = avl_create_node();
		node->data->key = key; 
		node->data->value = NULL; 
		if (add_to_array(&node->data->value, value) == 1)
			return (1);
		tree->root = node; 
	}
	else {
		next = tree->root; 
		// Go add the end of the tree
		while (next != NULL) { 
			last = next; 
			// if the key is lower (or greather depend of the reverse operator) than the node key, go to left
			if (compare(key, next->data->key, reverse)) 
				next = next->left;
			// if the key is greather (or lower depend of the reverse operator) than the node key, go to right
			else if (compare(key, next->data->key, !reverse)) 
				next = next->right;
			// else if the new key is equal to the node key
			else if (key == next->data->key) { 
				// Don't create a new node but add the new value to the node values array
				if (add_to_array(&next->data->value, value) == 1)
					return (1);
				// Add 1 to the amount of values at this node and exit the function
                next->data->nb++;
                return (0);
			}
		}
		// Create a new node and add it in the tree
		node = avl_create_node();
		node->data->key = key; 
		node->data->value = NULL
		if (add_to_array(&node->data->value, value) == 1)
			return (1);
		if (compare(key, last->data->key, reverse))
			last->left = node;
		if (compare(key, last->data->key, !reverse))
			last->right = node;	
	}
	// if AVL sort mode we balance the tree
	if (balance) 
		avl_balance(tree);
	return (0);
}

static void avl_save_node( avl_node_t *node, int depth, FILE *f)
{
	if (node->left)
		avl_save_node(node->left, depth + 2, f);
	// For the node, write every values with his key in his temporarie output file
    for (int i = 0; i < node->data->nb; i++) {
	    fprintf(f, "%f;%s\n", node->data->key, node->data->value[i]);
	}
	if (node->right)
		avl_save_node(node->right, depth + 2, f);
}

// free every nodes of the tree
static void avl_free_node(avl_node_t *node)
{
    if (node == NULL)
        return;
    avl_free_node(node->left)
    avl_free_node(node->right);
	free_array(node->data->value);
	free(node->data);
    free(node);
}

// parse a buffer and create a sorted tree and write sorted values in the file
static int tree(FILE *f, char buffer[RAM], int balance, int reverse)
{
	double key = 0;
	char *line;
	char *key_buffer; 
	char *value; 
	avl_tree_t *tree = avl_create(); 
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
        avl_insert(tree, key, balance, reverse, value); 
		// go to the next line
        line = strtok_r(NULL, "\n", &p); 
    }
	// save tree in the file
	avl_save_node(tree->root, 0, f); 
	// free memory
	avl_free_node(tree->root); 
	free(tree);
	return (0);
}

// if avl mode
int avl(FILE *file, char buffer[RAM], int reverse) 
{
	return (tree(file, buffer, 1, reverse)); 
}

// if abr mode
int abr(FILE *file, char buffer[RAM], int reverse) 
{
	return (tree(file, buffer, 0, reverse)); 
}
