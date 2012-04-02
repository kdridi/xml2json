//============================================================================
// Name        : api.c
// Author      : Karim DRIDI
// Version     : 0.1
// Copyright   : Private
// Description : XML2JSON Utility
//============================================================================

#define xml_doc_parse_filename(_doc, _filename, _block) {													\
	xmlDocPtr _doc = xmlParseFile((_filename));																\
	if(_doc != NULL) {																						\
		_block																								\
		if(_doc != NULL) {																					\
			xmlFreeDoc(_doc);																				\
			_doc = NULL;																					\
		}																									\
	}																										\
}

#define xml_doc_parse_memory(_doc, _data, _size, _block) {													\
	xmlDocPtr _doc = xmlParseMemory((_data), (_size));														\
	if(_doc != NULL) {																						\
		_block																								\
		if(_doc != NULL) {																					\
			xmlFreeDoc(_doc);																				\
			_doc = NULL;																					\
		}																									\
	}																										\
}

#define xml_doc_parse_json_buffer(_doc, _buffer, _block) {													\
	xml_doc_parse_memory(_doc, ((_buffer).data), ((_buffer).size), _block);									\
}

#define xml_doc_parse_string(_doc, _string, _block) {														\
	xml_doc_parse_memory(_doc, (_string), strlen(_string), _block);											\
}

#define xsl_doc_parse_filename(_result, _filename, _block) {												\
	xml_doc_parse_filename(_result##_xml, (_filename), {													\
		xsltStylesheetPtr _result = xsltParseStylesheetDoc(_result##_xml);									\
		if(_result != NULL) {																				\
			_block																							\
			if(_result != NULL) {																			\
				xsltFreeStylesheet(_result);																\
				_result = NULL;																				\
				_result##_xml = NULL;																		\
			}																								\
		}																									\
	});																										\
}

#define file_open_path(_result, _path, _mode, _block) {														\
	FILE *_result = fopen((_path), (_mode));																\
	if(_result != NULL) {																					\
		_block																								\
		if(_result != NULL) {																				\
			if(fclose(_result) != 0) {																		\
				fprintf(stderr, "Unable to close the file: %s\n", (_path));									\
			}																								\
			_result = NULL;																					\
		}																									\
}

#define xsl_doc_parse_string(_result, _string, _block) {													\
	xml_doc_parse_string(_result##_xml, (_string), {														\
		const char *_result##_filename_suffix = ".XXXXXX";													\
		const char *_result##_filename_template = #_result;													\
		size_t _result##_filename_length = 1;																\
		_result##_filename_length += strlen(_result##_filename_template);									\
		_result##_filename_length += strlen(_result##_filename_suffix);										\
		char *_result##_filename = (char *) calloc(sizeof(char), _result##_filename_length);				\
		if(_result##_filename != NULL) {																	\
			memset(_result##_filename, 0, _result##_filename_length);										\
			sprintf(_result##_filename, "%s%s", _result##_filename_template, _result##_filename_suffix);	\
			int _result##_fd = mkstemp(_result##_filename);													\
			if(_result##_fd  > 0) {																			\
				FILE *_result##_file = fdopen(_result##_fd, "wb");											\
				if (_result##_file != NULL) {																\
					if (xmlDocDump(_result##_file, _result##_xml) < 0) {									\
						xmlFreeDoc(_result##_xml);															\
						_result##_xml = NULL;																\
					}																						\
					if (fclose(_result##_file) != 0) {														\
						fprintf(stderr, "Unable to close the file: %s\n", _result##_filename);				\
					}																						\
					_result##_file = NULL;																	\
				}																							\
				if (_result##_xml != NULL) {																\
					xsl_doc_parse_filename(_result, _result##_filename, _block);							\
				}																							\
				if (remove(_result##_filename) == -1) {														\
					fprintf(stderr, "Unable to delete the tmp file: %s\n", _result##_filename);				\
				}																							\
			}																								\
			free(_result##_filename);																		\
			_result##_filename = NULL;																		\
		}																									\
	});																										\
}

#include "api.h"
#include "api_xml2json.h"

#include <string.h>

#include <curl/curl.h>

#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

static size_t buffer_readlink_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t result = size * nmemb;
	json_buffer_t *buffer = (json_buffer_t *) userp;

	buffer->data = realloc(buffer->data, buffer->size + result + 1);
	if (buffer->data == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		exit(EXIT_FAILURE);
	}

	memcpy(&(buffer->data[buffer->size]), contents, result);
	buffer->size += result;
	buffer->data[buffer->size] = 0;

	return result;
}

static int buffer_readlink(json_buffer_t *buffer, const char *link) {
	int result = -1;

	if (buffer != NULL) {
		buffer->size = 0;
		buffer->data = (char *) calloc(sizeof(char), 1);
		if (buffer->data != NULL) {
			curl_global_init(CURL_GLOBAL_ALL);
			CURL *curl_handle = curl_easy_init();
			if (curl_handle != NULL) {
				curl_easy_setopt(curl_handle, CURLOPT_URL, link);
				curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, buffer_readlink_callback);
				curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)buffer);
				curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/534.55.3 (KHTML, like Gecko) Version/5.1.5 Safari/534.55.3");
				curl_easy_perform(curl_handle);
				curl_easy_cleanup(curl_handle);
				result = 0;
			}
			curl_global_cleanup();
		}
	}

	return result;
}

int xml2json_link(json_buffer_t *output, const char *link) {
	int result = -1;
	json_buffer_t buffer = { 0 };
	if (buffer_readlink(&buffer, link) == 0) {
		result = xml2json_buffer(output, buffer);
	}
	if (buffer.data != NULL) {
		free(buffer.data);
		buffer.data = NULL;
		buffer.size = 0;
	}
	return result;
}

static int buffer_readfile(json_buffer_t *buffer, const char *filename) {
	int result = -1;

	if (buffer != NULL) {
		buffer->size = 0;
		buffer->data = NULL;
		FILE *file = fopen(filename, "rb");
		if (file != NULL) {
			if (fseek(file, 0L, SEEK_END) == 0) {
				buffer->size = ftell(file);
				if (buffer->size >= 0) {
					buffer->data = (char *) calloc(sizeof(char), buffer->size);
					if (buffer->data != NULL) {
						if (fseek(file, 0, SEEK_SET) == 0) {
							if (buffer->size == fread(buffer->data, sizeof(char), buffer->size, file)) {
								result = 0;
							}
						}
					}
				}
			}
			if (fclose(file) != 0) {
				fprintf(stderr, "Unable to close the file: %s\n", filename);
			}
			file = NULL;
		}
	}

	return result;
}

int xml2json_buffer(json_buffer_t *output, json_buffer_t buffer) {
	int result = -1;

	if (output != NULL) {
		output->size = 0;
		output->data = NULL;
		xml_doc_parse_json_buffer(doc, buffer, {
					xsl_doc_parse_string(stylesheet, api_xml2json, {
								const char *params = NULL;
								xmlDocPtr result_doc = xsltApplyStylesheet(stylesheet, doc, &params);
								if (result_doc != NULL) {
									int result_length = -1;
									xmlChar *result_buffer = NULL;
									result = xsltSaveResultToString(&result_buffer, &result_length, result_doc, stylesheet);
									output->size = result_length;
									output->data = (char *) result_buffer;
									xmlFreeDoc(result_doc);
									result_doc = NULL;
								}

							});
				});
	}

	return result;
}

int xml2json_file(json_buffer_t *output, const char *filename) {
	int result = -1;
	json_buffer_t buffer = { 0 };
	if (buffer_readfile(&buffer, filename) == 0) {
		result = xml2json_buffer(output, buffer);
	}
	if (buffer.data != NULL) {
		free(buffer.data);
		buffer.data = NULL;
		buffer.size = 0;
	}
	return result;
}

#include <json.h>

int opml2json_buffer(json_buffer_t *output, json_buffer_t buffer) {
	int result = -1;
	if (output != NULL) {
		output->size = 0;
		output->data = NULL;

		json_buffer_t xml_buffer = { 0 };
		if (xml2json_buffer(&xml_buffer, buffer) == 0) {
			struct json_object *json = NULL;
			struct json_tokener* tok = json_tokener_new();
			if (tok != NULL) {
				json = json_tokener_parse_ex(tok, xml_buffer.data, xml_buffer.size);
				json_tokener_free(tok);
				if (xml_buffer.data != NULL) {
					free(xml_buffer.data);
					xml_buffer.data = NULL;
				}
			}
			if (json != NULL) {
				const char *string = json_object_to_json_string(json);
				output->size = strlen(string);
				output->data = (char *) calloc(sizeof(char), output->size);
				if (output->data != NULL) {
					memcpy(output->data, string, output->size);
					result = 0;
				}
				json_object_put(json);
			}
		}
	}
	return result;

}
int opml2json_file(json_buffer_t *output, const char *filename) {
	int result = -1;

	if (output != NULL) {
		output->size = 0;
		output->data = NULL;
		json_buffer_t buffer = { 0 };
		if (buffer_readfile(&buffer, filename) == 0) {
			result = opml2json_buffer(output, buffer);
		}
		if (buffer.data != NULL) {
			free(buffer.data);
			buffer.data = NULL;
			buffer.size = 0;
		}
	}

	return result;
}
int opml2json_link(json_buffer_t *output, const char *link) {
	int result = -1;
	if (output != NULL) {
		output->size = 0;
		output->data = NULL;
		json_buffer_t buffer = { 0 };
		if (buffer_readlink(&buffer, link) == 0) {
			result = opml2json_buffer(output, buffer);
		}
		if (buffer.data != NULL) {
			free(buffer.data);
			buffer.data = NULL;
			buffer.size = 0;
		}
	}
	return result;
}

#undef xml_doc_parse_memory
