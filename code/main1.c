#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "map.h"
#include "data.h"
#include "tree.h"
#include "items.h"
#include "read.h"
#include "utils.h"

#define LARGE_DIST 10000.00
#define STG1_ITEMS 2
#define EPSILON 0.000000001

int main(int argc, char **argv){
	if (argc < 3){
		fprintf(stderr, "Not enough arguments\n");
		fprintf(stderr, "Please run with format\n");
		fprintf(stderr, "\tdict datafile outputfile < queryfile\n");
	}
	
	char *line = NULL;
	size_t size = 0;
	
	struct tree_map *tree_map = read_file(argv[1]); // reading data into tree
	
	FILE *output_file = fopen(argv[2], "w"); // opens output file
	assert(output_file);
	
	int count = 0;
	struct coord *input_coord = NULL; // for storing stdin inputs
	struct min_dist *result = NULL; // for storing results
	while (getline_clean(&line, &size, stdin) != (-1)){
		input_coord = new_input();
		result = new_min_dist(LARGE_DIST);
		input_coord->coordinates = extract_coord(line, STG1_ITEMS);
		result = search_closest(tree_map->KD_tree, input_coord, result, &count);
		printf("%s %s --> %d\n", input_coord->coordinates[0], 
			input_coord->coordinates[1], count);
		print_result(input_coord, tree_map->mapping, result->list, output_file, STG1_ITEMS);
		free_coord(&input_coord, STG1_ITEMS);
		free(result);
		result = NULL;
		count = 0;
	}
    
	free_tree_map(&tree_map); // frees the whole tree
	
	if (output_file){
		fclose(output_file);
	}
	
	if (line){
		free(line);
		line = NULL;
	}
    
	return 0;
}

