#include <stdlib.h>
#include <stdio.h>
#include <units.h>
#include <string.h>

#include <map.h>

int ptoi (map *m, int *p) { return p[0]+p[1]*m->sx; }
int xytoi (map *m, int x, int y) { return x+y*m->sx; }

void map_init (map *m, int sx, int sy, int ts) {
    m->t = (int*)malloc(sizeof(int)*sx*sy);
    m->sx = sx; m->sy = sy; m->ts = ts;
    memset(m->t, 0, sizeof(int)*sx*sy);
}

void map_destroy (map *m) {
    free(m->t);
}