#ifndef MAP_H
#define MAP_H

struct tree_map {
	struct data_mapping *mapping;
	struct tree *KD_tree;
};

struct tree_map *new_tree_map(char *header);

struct tree_map *insert_tree(struct tree_map *tree_map, char *line);

void free_tree_map(struct tree_map **tree_map);

#endif 