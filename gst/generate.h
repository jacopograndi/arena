#ifndef GENERATE_H
#define GENERATE_H

#include "info.h"

#define GENERATE_UNIT_MAX_ATTEMPTS 32

void generate_init ();
int generate_unit (infos *info, info_unit *u, float cost);

#endif