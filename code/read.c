/*
	Reading in data from data set
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "data.h"
#include "utils.h"
#include "tree.h"
#include "map.h"

struct tree_map *read_file(char *filename){
	FILE *file = fopen(filename, "r");
	assert(file);
	struct tree_map *tree_map = NULL;
	
	char *line = NULL;
	size_t size = 0;
	
	if (getline_clean(&line, &size, file) != (-1)){
		tree_map = new_tree_map(line);
	}
	
	while (getline_clean(&line, &size, file) != (-1)){
		tree_map = insert_tree(tree_map, line);
	}
	
	if (line){
		free(line);
	}
	if (file){
		fclose(file);
	}
	return tree_map;
}