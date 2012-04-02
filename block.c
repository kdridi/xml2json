//============================================================================
// Name        : block.c
// Author      : Karim DRIDI
// Version     : 0.1
// Copyright   : Private
// Description : Block macros
//============================================================================

#include "block.h"

int main(int argc, char *argv[])
block_return(int, result, EXIT_FAILURE, {
	char path[1024];
	sprintf(path, "%s.c", argv[0]);
	block_memory_with_filename(result, path, {
		result = EXIT_SUCCESS;
		printf("size: %lu\n", result_length);
		printf("data:\n%s\n", result_data);
	})
})
