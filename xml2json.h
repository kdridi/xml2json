//============================================================================
// Name        : xml2json.h
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
} xml2json_buffer_t;

int xml2json_buffer(xml2json_buffer_t *result, xml2json_buffer_t buffer);
int xml2json_file(xml2json_buffer_t *result, const char *filename);
int xml2json_link(xml2json_buffer_t *result, const char *link);

#endif /* XML2JSON_H_ */
