/*
 * xml2json.h
 *
 *  Created on: 1 avr. 2012
 *      Author: kdridi
 */

#ifndef XML2JSON_H_
#define XML2JSON_H_

#include <stdlib.h>

typedef struct {
	char *data;
	size_t size;
} xml2json_buffer_t;


int xml2json_buffer(xml2json_buffer_t *result, xml2json_buffer_t buffer);
int xml2json_file(xml2json_buffer_t *result, const char *filename);

#endif /* XML2JSON_H_ */
