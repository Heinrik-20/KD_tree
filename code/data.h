#ifndef DATA_H
#define DATA_H

struct data {
    char **data;
};

struct coord {
    char **coordinates;
};

struct data_mapping {
    char **headers;
    int header_count;
    int *coord_locations;
    int *data_locations;
    int coord_count;
};

struct data_mapping *get_data_map(char *header);

void get_data(char *row, struct data_mapping *mapping, struct coord **coord,
	struct data **data);

char **extract_coord(char *row, int items_read);

struct coord *new_input(void);

struct min_dist *new_min_dist(double init_size);

void free_coord(struct coord **coord, int items);

void free_data(struct data **data, int items);

void free_mapping(struct data_mapping **mapping);

#endif