#ifndef TREE_H
#define TREE_H

struct tree{
	struct items *items;
	struct tree *next;
	struct tree *left;
	struct tree *right;
};

struct min_dist {
	double dist_from_input;
	struct tree *list;
};

struct tree *new_node(void);

struct tree *recursive_insert(struct tree *parent, struct tree *new,
	int dimension);

struct min_dist *new_min_dist(double init_size);

struct min_dist *search_closest(struct tree *parent, struct coord *coord, 
	struct min_dist *min_dist, int *count);

void search_radius(struct tree *parent, struct coord *coord, struct data_mapping
	*mapping, int *count, FILE *output, int *found);

void print_result(struct coord *input_coord, struct data_mapping *mapping,
	struct tree *result, FILE *output, int input_items);

void free_tree(struct tree **parent, struct data_mapping *mapping);

void free_tree_node(struct tree **tree, struct data_mapping *mapping);

#endif