//============================================================================
// Name        : main.c
// Author      : Karim DRIDI
// Version     : 0.1
// Copyright   : Private
// Description : XML2JSON Utility
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "xml2json.h"

int main(int argc, char *argv[]) {
  int result = EXIT_FAILURE;
  int usage = 1;

  if (argc == 3) {
    usage = 0;
    xml2json_buffer_t buffer = { 0 };
    int error = -1;
    if(strncmp("-f", argv[1], 2) == 0) {
      error = xml2json_file(&buffer, argv[2]);
    } else if(strncmp("-u", argv[1], 2) == 0) {
      error = xml2json_link(&buffer, argv[2]);
    } else {
      usage = 1;
    }
    if (error == 0) {
      fprintf(stdout, "%s\n", buffer.data);
      result = EXIT_SUCCESS;
      free(buffer.data);
      buffer.data = NULL;
    }
  }

  if(usage) {
    fprintf(stdout, "%s\n", "Usage: xml2json [-f file | -u URL]");
  }

  return result;
}
