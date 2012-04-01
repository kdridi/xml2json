//============================================================================
// Name        : librss.cpp
// Author      : Karim DRIDI
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================


#include <stdlib.h>
#include <stdio.h>

#include "xml2json.h"

int main(int argc, char *argv[]) {
	int result = EXIT_FAILURE;
	if (argc == 2) {
		xml2json_buffer_t buffer = { 0 };
		if (xml2json_file(&buffer, argv[1]) == 0) {
			printf("%s\n", buffer.data);
			result = EXIT_SUCCESS;
			free(buffer.data);
			buffer.data = NULL;
		}
	}
	return result;
}
