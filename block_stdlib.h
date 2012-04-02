//============================================================================
// Name        : block_stlib.h
// Author      : Karim DRIDI
// Version     : 0.1
// Copyright   : Private
// Description : Block macros
//============================================================================

#ifndef BLOCK_STDLIB_H_
#define BLOCK_STDLIB_H_

#include <stdlib.h>

#define block_calloc(_result, _nmemb, _size, _succes_block, _failure_clock) {	\
	void *_result = calloc((_nmemb), (_size));									\
	if(_result != NULL)															\
		_succes_block															\
	else																		\
		_failure_clock															\
}

#define block_malloc(_result, _size, _succes_block, _failure_clock) {			\
	void *_result = malloc((_size));											\
	if(_result != NULL)															\
		_succes_block															\
	else																		\
		_failure_clock															\
}

#define block_free(_ptr, _block) {												\
	free((_ptr));																\
	(_ptr) = NULL;																\
	_block																		\
}

#define block_realloc(_ptr, _size, _block) {									\
	realloc((_ptr), (_size));													\
	_block																		\
}

#define block_mkstemp(_template, _success_block, _failure_block) {				\
	if(mkstemp((_template)) == -1)												\
		_failure_block															\
	else																		\
		_success_block															\
}

#define block_mkostemp(_template, _flags, _success_block, _failure_block) {		\
	if(mkostemp((_template), (_flags)) == -1)									\
		_failure_block															\
	else																		\
		_success_block															\
}

#endif /* BLOCK_STDLIB_H_ */
