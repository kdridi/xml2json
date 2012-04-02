//============================================================================
// Name        : block_stdio.h
// Author      : Karim DRIDI
// Version     : 0.1
// Copyright   : Private
// Description : Block macros
//============================================================================

#ifndef BLOCK_STDIO_H_
#define BLOCK_STDIO_H_

#include <stdio.h>

#define block_fclose(_file, _success_bloc, _failure_bloc) {									\
	if(fclose((_file)) == 0)																\
		_success_bloc																		\
	else																					\
		_failure_bloc																		\
}

#define block_fopen(_result, _path, _mode, _success_block, _failure_block) {				\
	FILE *_result = fopen((_path), (_mode));												\
	if(_result == NULL)																		\
		_failure_block																		\
	else																					\
		_success_block																		\
}

#define block_fdopen(_result, _fd, _mode, _success_block, _failure_block) {					\
	FILE *_result = fdopen((_fd), (_mode));													\
	if(_result == NULL)																		\
		_success_block																		\
	else																					\
		_failure_block																		\
}

#define block_freopen(_result, _path, _mode, _stream, _success_block, _failure_block) {		\
	FILE *_result = freopen((_path), (_mode), (_stream));									\
	if(_result == NULL)																		\
		_success_block																		\
	else																					\
		_failure_block																		\
}

#define block_fseek(_stream, _offset, _whence, _success_block, _failure_block) {			\
	if(fseek((_stream), (_offset), (_whence)) != -1)										\
		_success_block																		\
	else																					\
		_failure_block																		\
}

#define block_fgetpos(_stream, _pos, _success_block, _failure_block) {						\
	if(fgetpos((_stream), (_pos)) != -1)													\
		_success_block																		\
	else																					\
		_failure_block																		\
}

#define block_fsetpos(_stream, _pos, _success_block, _failure_block) {						\
	if(fsetpos((_stream), (_pos)) != -1)													\
		_success_block																		\
	else																					\
		_failure_block																		\
}

#define block_ftell(_result, _stream, _success_block, _failure_block) {						\
	long _result = ftell((_stream));														\
	if(_result != -1)																		\
		_success_block																		\
	else																					\
		_failure_block																		\
}

#endif /* BLOCK_STDIO_H_ */
