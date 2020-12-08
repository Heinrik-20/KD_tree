/*
	Function prototypes for data-related functions
*/
#define DEFAULTINDICESCOUNT 1
#define NOTYETFOUND (-1)

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* 
    The reference keys and data used for the problem. More flexible would be to
    get this from a file and store it in the struct. But at this point we don't 
    have that.
*/
static char *reference_keys[] = {
    "x coordinate",
    "y coordinate"
};

static char *reference_data[] = {
    "Census year", 
    "Block ID", 
    "Property ID", 
    "Base property ID", 
    "CLUE small area", 
    "Trading name",
    "Industry (ANZSIC4) code", 
    "Industry (ANZSIC4) description", 
    "Location"
};

void split_tokens(char *line, int **indices, int *index_count);
char **extract_tokens(char *line, int *indices, int index_count);

void split_tokens(char *line, int **indices, int *index_count){
    int in_quotes = 0;
    int progress = 0;
    int length = strlen(line);
    int allocated_indices;
    *index_count = 0;
    
    *indices = (int *) malloc(sizeof(int) * DEFAULTINDICESCOUNT);
    assert(*indices);
    allocated_indices = DEFAULTINDICESCOUNT;
    
    /* First csv value always starts at index 0. */
    (*indices)[0] = 0;
    *index_count = (*index_count) + 1; 
    
    for(progress = 0; progress < length; progress++){
        if(! in_quotes){
            if(line[progress] == '\"'){
                in_quotes = 1;
            } else if(line[progress] == ','){
                /* Split token */
                if(allocated_indices <= (*index_count)){
                    *indices = (int *) realloc(*indices, 
                        sizeof(int) * allocated_indices * 2);
                    assert(*indices);
                    allocated_indices = allocated_indices * 2;
                }
                (*indices)[*index_count] = progress + 1;
                *index_count = (*index_count) + 1; 
            }
            /* Otherwise just move on. */
        } else {
            if(line[progress] == '\"'){
                in_quotes = 0;
            }
            /* Otherwise just move on. */
        }
    }
    
    /* Resize down indices */
    if(allocated_indices > (*index_count)){
        *indices = (int *) realloc(*indices, sizeof(int) * (*index_count));
        assert(*indices);
    }
}

char **extract_tokens(char *line, int *indices, int index_count){
    /* Check lines with quotes and copy them to new allocations. */
    char **tokens = (char **) malloc(sizeof(char *) * index_count);
    assert(tokens);
    int i, j, k;
    int start;
    int end;
    int quote_pairs;
    for(i = 0; i < index_count; i++){
        start = indices[i];
        if(i < (index_count - 1)){
            end = indices[i + 1] - 2;
        } else {
            end = strlen(line) - 1;
        }
        if(line[start] == '\"' && line[end] == '\"'){
            start++;
            end--;
        }
        /* Work out how many pairs of quotes are present in the string. */
        quote_pairs = 0;
        for(j = 0; j < (end - start + 1); j++){
            if(line[start + j] == '\"'){
                quote_pairs++;
                /* Move ahead one character to skip second quote. */
                j++;
            }
        }
        
        tokens[i] = (char *) malloc(sizeof(char) * (end - start + 1 + 1 - quote_pairs));
        assert(tokens[i]);
        k = 0;
        for(j = 0; j < (end - start + 1); j++){
            tokens[i][k] = line[start + j];
            if(line[start + j] == '\"'){
                /* Next character will be a double quote */
                assert(line[start + j + 1] == '\"');
                j++;
            }
            k++;
        }
        tokens[i][k] = '\0';
    }
    
    return tokens;
}

struct data_mapping *get_data_map(char *header){
    int i, j;
    /* Split by comma. */
    int *indices = NULL;
    int index_count = 0;
    
    split_tokens(header, &indices, &index_count);
    
    char **tokens = extract_tokens(header, indices, index_count);
    
    /* Build map */
    struct data_mapping *return_map = (struct data_mapping *) 
        malloc(sizeof(struct data_mapping));
    assert(return_map);
    
    return_map->headers = tokens;
    return_map->header_count = index_count;
    return_map->coord_count = (int) (sizeof(reference_keys) / sizeof(reference_keys[0]));
    return_map->coord_locations = (int *) malloc(sizeof(int) * return_map->coord_count);
    assert(return_map->coord_locations);
    return_map->data_locations = (int *) malloc(sizeof(int) * 
        (index_count - return_map->coord_count));
    assert(return_map->data_locations);
    
    /* Work out which are keys and which are data. */
    for(i = 0; i < return_map->coord_count; i++){
        /* Mark not yet found. */
        (return_map->coord_locations)[i] = NOTYETFOUND;
    }
    for(i = 0; i < (index_count - return_map->coord_count); i++){
        /* Mark not yet found. */
        (return_map->data_locations)[i] = NOTYETFOUND;
    }
    
    for(i = 0; i < index_count; i++){
        /* Assume a small number of keys. */
        for(j = 0; j < return_map->coord_count; j++){
            if(strcmp(tokens[i], reference_keys[j]) == 0){
                assert((return_map->coord_locations)[j] == NOTYETFOUND);
                (return_map->coord_locations)[j] = i;
                break;
            }
        }
        for(j = 0; j < (index_count - return_map->coord_count); j++){
            if(strcmp(tokens[i], reference_data[j]) == 0){
                assert((return_map->data_locations)[j] == NOTYETFOUND);
                (return_map->data_locations)[j] = i;
                break;
            }
        }
    }
    
    /* We should have got all the values from the header. */
    for(j = 0; j < return_map->coord_count; j++){
        assert((return_map->coord_locations)[j] != NOTYETFOUND);
    }
    
    for(j = 0; j < (index_count - return_map->coord_count); j++){
        assert((return_map->data_locations)[j] != NOTYETFOUND);
    }
    
    if(indices){
        free(indices);
    }
    
    return return_map;
}

void get_data(char *row, struct data_mapping *mapping, struct coord **coord, 
    struct data **data){
    int i;
    /* Split by comma. */
    int *indices = NULL;
    int index_count = 0;
    
    split_tokens(row, &indices, &index_count);
    
    char **tokens = extract_tokens(row, indices, index_count);
    
    assert(index_count == mapping->header_count);
    
    struct data *new_data = (struct data *) malloc(sizeof(struct data));
    assert(new_data);
    struct coord *new_coord = (struct coord *) malloc(sizeof(struct coord));
    assert(new_coord);
    
    int data_count = mapping->header_count - mapping->coord_count;
    
    new_data->data = (char **) malloc(sizeof(char *) * data_count);
    assert(new_data->data);
    new_coord->coordinates = (char **) malloc(sizeof(char *) * mapping->coord_count);
    assert(new_coord->coordinates);
    
    for(i = 0; i < data_count; i++){
        (new_data->data)[i] = tokens[(mapping->data_locations)[i]];
    }
    for(i = 0; i < mapping->coord_count; i++){
        (new_coord->coordinates)[i] = tokens[(mapping->coord_locations)[i]];
    }
    
    *coord = new_coord;
    *data = new_data;
    if(tokens){
        free(tokens);
    }
    if(indices){
        free(indices);
    }
}

/* extracts coordinates from stdin */
char **extract_coord(char *row, int items_read){
	char **items = (char **)malloc(sizeof(char *) * items_read);
	int count = 0, size = DEFAULTINDICESCOUNT, row_size = (int)strlen(row);
	int pos = 0;
	for (int i = 0;i < items_read;i++){
		items[i] = (char *)malloc(sizeof(char) * size);
		while (row[pos] != ' ' && row[pos] != '\0' && pos < row_size){
			if (count >= size){
				items[i] = (char *)realloc(items[i], sizeof(char) * size * 2);
				size = size * 2;
			}
			items[i][count] = row[pos];
			count += 1;
			pos += 1;
		}
		pos += 1;
		items[i][count] = '\0';
		count = 0;
		size = DEFAULTINDICESCOUNT;
	}
	return items;
}

/* New struct for stdin inputs */
struct coord *new_input(void){
	struct coord *input_coord = (struct coord *)malloc(sizeof(struct coord));
	assert(input_coord);
	return input_coord;
}

/* Free all coord structs */
void free_coord(struct coord **coord, int items){
	int i;
	for (i = 0;i < items;i++){
		free(((*coord)->coordinates)[i]);
		((*coord)->coordinates)[i] = NULL;
	}
    free((*coord)->coordinates);
    (*coord)->coordinates = NULL;
	free((*coord));
	(*coord) = NULL;
}

/* Free all data structs */
void free_data(struct data **data, int items){
	int i;
	for (i = 0;i < items;i++){
		free(((*data)->data)[i]);
		((*data)->data)[i] = NULL;
	}
    free((*data)->data);
    (*data)->data = NULL;
	free(*data);
	(*data) = NULL;
	return;
}

/* Free all data mapping structs */
void free_mapping(struct data_mapping **mapping){
	int i;
	for (i = 0;i < (*mapping)->header_count;i++){
		free((*mapping)->headers[i]);
		(*mapping)->headers[i] = NULL;
	}
	free((*mapping)->headers);
	(*mapping)->headers = NULL;
	free((*mapping)->coord_locations);
	(*mapping)->coord_locations = NULL;
	free((*mapping)->data_locations);
	(*mapping)->data_locations = NULL;
	free(*mapping);
	(*mapping) = NULL;
	return;
}