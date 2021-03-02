#ifndef JSONPARSE_H
#define JSONPARSE_H

#define JSMN_STATIC
#include "jsmn.h"

#define MAXTOKENS 2048

void substr_token(char *json, char *temp, jsmntok_t *t);

void json_parse_array(char *json, void *temp, jsmntok_t *t, int r, char type);
void json_dump_array (char *str, void *arr, int len, char type);

int json_parse_subtokens (char *json, jsmntok_t *t, int r, int i);

#endif