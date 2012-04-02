//============================================================================
// Name        : api.h
// Author      : Karim DRIDI
// Version     : 0.1
// Copyright   : Private
// Description : XML2JSON Utility
//============================================================================

#ifndef XML2JSON_H_
#define XML2JSON_H_

#include <stdlib.h>

typedef struct {
	char *data;
	size_t size;
} json_buffer_t;

int xml2json_buffer(json_buffer_t *result, json_buffer_t buffer);
int xml2json_file(json_buffer_t *result, const char *filename);
int xml2json_link(json_buffer_t *result, const char *link);

int opml2json_buffer(json_buffer_t *result, json_buffer_t buffer);
int opml2json_file(json_buffer_t *result, const char *filename);
int opml2json_link(json_buffer_t *result, const char *link);

#endif /* XML2JSON_H_ */
