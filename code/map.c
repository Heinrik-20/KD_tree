/*
	Function prototypes for all map related functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "data.h"
#include "items.h"
#include "tree.h"
#include "map.h"

/* Allocate memory for new map struct */
struct tree_map *new_tree_map(char *header){
	struct tree_map *new = (struct tree_map *) malloc(sizeof(struct tree_map));
	assert(new);
	new->mapping = get_data_map(header);
	new->KD_tree = NULL;
	return new;
}

/* Insert data into tree */
struct tree_map *insert_tree(struct tree_map *tree_map, char *line){
	struct items *items = insert_items(line, tree_map->mapping);
	struct tree *new = new_node();
	new->items = items;
	tree_map->KD_tree = recursive_insert(tree_map->KD_tree, new, 0);
	return tree_map;
}

/* Free allocated memory for map struct*/
void free_tree_map(struct tree_map **tree_map){
	free_tree(&((*tree_map)->KD_tree), (*tree_map)->mapping);
	free_mapping(&((*tree_map)->mapping));
	free(*tree_map);
	(*tree_map) = NULL;
	return;
}