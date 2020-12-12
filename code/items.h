#ifndef LLIST_H
#define LLIST_H

struct items {
	int dimension;  // 0 for x coordinate, 1 for y coordinate
	struct coord *coord;
	struct data *data;
};

struct items *new_items(void);

struct items *insert_items(char *line, struct data_mapping *mapping);

void free_items(struct items **items, struct data_mapping *mapping);

#endif