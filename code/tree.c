/*
	All K-D tree related functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "data.h"
#include "tree.h"
#include "items.h"
#include "utils.h"

#define EPSILON 0.000000001
#define STG2_ITEMS 3

/* Allocates memory for K-D tree*/
struct tree *new_node(void){
	struct tree *return_tree = (struct tree *)malloc(sizeof(struct tree));
	assert(return_tree);
	return_tree->next = NULL;
	return_tree->left = NULL;
	return_tree->right = NULL;
	return return_tree;
}

/* Recursively insert new node into tree */
struct tree *recursive_insert(struct tree *parent, struct tree *new, 
	int dimension){
	if (! parent){
		new->items->dimension = dimension;
		return new;
	}
	double c;
	int c0, c1;
	c = atof(parent->items->coord->coordinates[dimension]) - 
		atof(new->items->coord->coordinates[dimension]);
	c0 = strcmp(parent->items->coord->coordinates[0], 
				new->items->coord->coordinates[0]);
	c1 = strcmp(parent->items->coord->coordinates[1],
				new->items->coord->coordinates[1]);
	if ((c0 == 0) && (c1 == 0)){ // for exact match
		new->items->dimension = dimension;
		new->next = parent->next;
		parent->next = new;
	}
	else if (c >= (-1)*EPSILON){ // anything larger 
		if (dimension){
			parent->left = recursive_insert(parent->left, new, 0);
		}
		else {
			parent->left = recursive_insert(parent->left, new, 1);
		}
	}
	else { // anything smaller 
		if (dimension){
			parent->right = recursive_insert(parent->right, new, 0);
		}
		else {
			parent->right = recursive_insert(parent->right, new, 1);
		}
	}
	return parent;
}

/* Struct for storing stage 1 results */
struct min_dist *new_min_dist(double init_size){
	struct min_dist *result = (struct min_dist *)malloc(sizeof(struct min_dist));
	assert(result);
	result->list = NULL;
	result->dist_from_input = init_size;
	return result;
}

/* Searching for closest business establishment */
struct min_dist *search_closest(struct tree *parent, struct coord *coord, 
	struct min_dist *min_dist, int *count){
	double distance, d_x, d_y, diff;
	
	if (! parent){
		return min_dist;
	}
	
	*count += 1;
	int dim = parent->items->dimension;
	
	d_x = atof(parent->items->coord->coordinates[0]) - 
	atof(coord->coordinates[0]);
	d_y = atof(parent->items->coord->coordinates[1]) - 
	atof(coord->coordinates[1]);
	distance = sqrt((d_x * d_x) + (d_y * d_y));
	diff = distance - (min_dist->dist_from_input);
	
	if (diff < EPSILON){
		min_dist->dist_from_input = distance;
		min_dist->list = parent;
	}
	
	if (dim){ // comparing y coordinates
		if (d_y < EPSILON) {
			min_dist = search_closest(parent->right, coord, min_dist, count);
			distance = (d_y * d_y) - (min_dist->dist_from_input * min_dist
			->dist_from_input);
			if (distance < EPSILON){ // new distance > difference of y coordinates
				min_dist = search_closest(parent->left, coord, min_dist, count);
			}
		}
		else {
			min_dist = search_closest(parent->left, coord, min_dist, count);
			distance = (d_y * d_y) - (min_dist->dist_from_input * min_dist
			->dist_from_input);
			if (distance < EPSILON){ // new distance > difference of y coordinates
				min_dist = search_closest(parent->right, coord, min_dist, count);
			}	
		}	
	}
	else { // compaing x coordinates
		if (d_x < EPSILON){
			min_dist = search_closest(parent->right, coord, min_dist, count);
			distance = (d_x * d_x) - (min_dist->dist_from_input * min_dist
			->dist_from_input);
			if (distance < EPSILON){ // new distance > difference of x coordinates
				min_dist = search_closest(parent->left, coord, min_dist, count);
			}
		}
		else {
			min_dist = search_closest(parent->left, coord, min_dist, count);			
			distance = (d_x * d_x) - (min_dist->dist_from_input * min_dist
			->dist_from_input);
			if (distance < EPSILON){ // new distance > difference of x coordinates
				min_dist = search_closest(parent->right, coord, min_dist, count);
			}	
		}	
	}
	
	return min_dist;
}

/* Search for all business establishments within given radius */
void search_radius(struct tree *parent, struct coord *coord,
	struct data_mapping *mapping, int *count, FILE *output, int *found){
	
	if (! parent){
		return;
	}
	
	*count += 1;
	
	double d_x, d_y, distance, diff;
	double radius = atof(coord->coordinates[2]);
	int dim = parent->items->dimension;
	
	d_x = atof(parent->items->coord->coordinates[0]) - 
	atof(coord->coordinates[0]);
	d_y = atof(parent->items->coord->coordinates[1]) - 
	atof(coord->coordinates[1]);
	distance = sqrt((d_x * d_x) + (d_y * d_y));
	diff = distance - radius;
	
	if (diff < EPSILON){ // prints if parent within radius
		*found = 1;
		print_result(coord, mapping, parent, output, STG2_ITEMS);
	}
	
	if (dim){ // y coordinates
		if (d_y < EPSILON) {
			search_radius(parent->right, coord, mapping, count, output, found);
			if (fabs(d_y) < radius){ // radius > difference in y coordinates
				search_radius(parent->left, coord, mapping, count, output, found);
			}
		}
		else {
			search_radius(parent->left, coord, mapping, count, output, found);
			if (fabs(d_y) < radius){ // radius > difference in y coordinates
				search_radius(parent->right, coord, mapping, count, output, found);
			}	
		}	
	}
	else { // x coordinates
		if (d_x < EPSILON){
			search_radius(parent->right, coord, mapping, count, output, found);
			if (fabs(d_x) < radius){ // radius > difference in x coordinates
				search_radius(parent->left, coord, mapping, count, output, found);
			}
		}
		else {
			search_radius(parent->left, coord, mapping, count, output, found);
			if (fabs(d_x) < radius){ // radius > difference in x coordinates
				search_radius(parent->right, coord, mapping, count, output, found);
			}	
		}	
	}
	return;
}

/* Print results to output file */
void print_result(struct coord *input_coord, struct data_mapping *mapping,
	struct tree *result, FILE *output, int input_size){
	struct tree *tmp = result;
	int i, coord_index = 0;
	while (tmp){
		for (i = 0;i < input_size;i++){
			fprintf(output, "%s ", input_coord->coordinates[i]);
		}
		fprintf(output, "-->");
		for (i = 0;i < mapping->header_count;i++){
			if (i == (mapping->data_locations)[i - coord_index]){
				fprintf(output, " %s: %s ||", mapping->headers[i], 
					(tmp->items->data->data)[i - coord_index]);
			}
			else {
				fprintf(output, " %s: %s ||", mapping->headers[i],
					tmp->items->coord->coordinates[i - i + coord_index]);
				coord_index += 1;
			}
		}
		fprintf(output, "\n");
		coord_index = 0;
		tmp = tmp->next;
	}
}

/* Free allocated memory for K-D tree */
void free_tree(struct tree **parent, struct data_mapping *mapping){
	if (! (*parent)){
		return;
	}
	free_tree(&((*parent)->left), mapping);
	free_tree(&((*parent)->right), mapping);
	struct tree *tmp = (*parent)->next, *prev;
	while (tmp){
		prev = tmp;
		tmp = tmp->next;
		free_tree_node(&prev, mapping);
	}
	free_tree_node(parent, mapping);
	return;
}

/* Free one node of K-D tree */
void free_tree_node(struct tree **tree, struct data_mapping *mapping){
	if (! (*tree)) {
		return;
	}
	free_items(&((*tree)->items), mapping);
	free(*tree);
	(*tree) = NULL;
	return;
}