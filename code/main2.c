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
#define STG2_ITEMS 3

int main(int argc, char **argv){
	if (argc < 3){
		fprintf(stderr, "Not enough arguments\n");
		fprintf(stderr, "Please run with format\n");
		fprintf(stderr, "\tdict datafile outputfile < queryfile\n");
	}
	
	char *line = NULL;
	size_t size = 0;
	
	struct tree_map *tree_map = read_file(argv[1]); // reads data into tree
	
	FILE *output_file = fopen(argv[2], "w"); // opens output file
	assert(output_file);
	
	int count = 0, found = 0; // flag for finding one node in radius
	struct coord *input_coord = NULL; // stdin coordinates
	while (getline_clean(&line, &size, stdin) != (-1)){
		input_coord = new_input();
		input_coord->coordinates = extract_coord(line, STG2_ITEMS);
		search_radius(tree_map->KD_tree, input_coord, tree_map->mapping, &count,
			output_file, &found);
		if (! found){ // when nothing is found
			fprintf(output_file, "%s %s %s --> NOTFOUND\n", 
				input_coord->coordinates[0], input_coord->coordinates[1],
				input_coord->coordinates[2]);
		}
		printf("%s %s %s --> %d\n", input_coord->coordinates[0], 
			input_coord->coordinates[1], input_coord->coordinates[2], count);
		free_coord(&input_coord, STG2_ITEMS);
		count = 0;
	}
	
    free_tree_map(&tree_map); // free the whole map
	
    if (input_coord){
        free_coord(&input_coord, STG2_ITEMS);
    }
    
	if (output_file){
		fclose(output_file);
	}
	
	if (line){
		free(line);
		line = NULL;
	}
	
	return 0;
}

