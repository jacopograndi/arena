#ifndef JSONPARSE_H
#define JSONPARSE_H

#define JSMN_STATIC
#include <jsmn.h>

#define MAXTOKENS 2048

void substr_token(char *json, char *temp, jsmntok_t *t);

void json_parse_array(char *json, void *temp, jsmntok_t *t, int r, char type);
void json_dump_array (char *str, void *arr, int len, char type);

int json_parse_subtokens (char *json, jsmntok_t *t, int r, int i);
/*

typedef struct { char key[32]; int i; } pair_ci;
typedef struct { char key[32]; float i; } pair_cf;


int json_parse_dict_ci_init (char *json, pair_ci *dict, jsmntok_t *t, int r);
int json_parse_dict_cf_init (char *json, pair_cf *dict, jsmntok_t *t, int r);
int json_parse_dict_ci (char *json, pair_ci *dict);
int json_parse_dict_cf (char *json, pair_cf *dict);

#define MAXLISTLEN 64
#define MAXOBJLEN 32

typedef struct { pair_ci pairs[MAXOBJLEN]; int len; } list_ci;
typedef struct { pair_cf pairs[MAXOBJLEN]; int len; } list_cf;

int json_parse_list_ci (char *json, list_ci *list);
int json_parse_list_cf (char *json, list_cf *list);
*/
#endif