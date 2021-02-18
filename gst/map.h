#ifndef MAP_H
#define MAP_H

typedef struct {
    int *t;
    int sx;
    int sy;
    int ts;
} map;

int ptoi (map *m, int *p);
int xytoi (map *m, int x, int y);
void map_init (map *m, int sx, int sy, int ts);
void map_destroy (map *m);

#endif