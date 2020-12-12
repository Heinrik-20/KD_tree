/*
	Function prototypes for all items struct 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "items.h"

/* Allocates new struct to store coord and data struct */
struct items *new_items(void){
	struct items *new = (struct items *) malloc(sizeof(struct items));
	assert(new);
	new->coord = NULL;
	new->data = NULL;
	return new;
}

/* Insert coordinates and data into item structs */
struct items *insert_items(char *line, struct data_mapping *mapping){
	struct items *new = new_items();
	get_data(line, mapping, &(new->coord), &(new->data));
	return new;
}

/* Free allocated memory for items struct */ 
void free_items(struct items **items, struct data_mapping *mapping){
	free_coord(&((*items)->coord), mapping->coord_count);
	free_data(&((*items)->data), 
		(mapping->header_count) - (mapping->coord_count));
	free(*items);
	(*items) = NULL;
	return;
}