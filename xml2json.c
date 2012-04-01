#include "xml2json.h"
#include "xml2json_transformer.h"

#include <string.h>

#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

int buffer_readfile(xml2json_buffer_t *buffer, const char *filename) {
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

int xml2json_buffer(xml2json_buffer_t *output, xml2json_buffer_t buffer) {
	int result = -1;

	if (output != NULL) {
		output->size = 0;
		output->data = NULL;
		xmlDocPtr doc = xmlParseMemory(buffer.data, buffer.size);
		if (doc != NULL) {
			xmlDocPtr stylesheet_doc = xmlParseMemory(xml2json_transformer, strlen(xml2json_transformer));
			if (stylesheet_doc != NULL) {
				const char *tmp_filename_template = "xslt.XXXXXX";
				char *tmp_filename = (char *) calloc(sizeof(char), strlen(tmp_filename_template) + 1);
				snprintf(tmp_filename, strlen(tmp_filename_template) + 1, "%s", tmp_filename_template);
				if (tmp_filename != NULL) {
					int tmp_fd = mkstemp(tmp_filename);
					if (tmp_fd >= 0) {
						FILE *tmp_file = fdopen(tmp_fd, "wb");
						if (tmp_file != NULL) {
							if (xmlDocDump(tmp_file, stylesheet_doc) < 0) {
								xmlFreeDoc(stylesheet_doc);
								stylesheet_doc = NULL;
							}
							if (fclose(tmp_file) != 0) {
								fprintf(stderr, "Unable to close the file: %s\n", tmp_filename);
							}
							tmp_file = NULL;
						}
						if (stylesheet_doc != NULL) {
							xmlFreeDoc(stylesheet_doc);
							stylesheet_doc = NULL;

							xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc(xmlParseFile(tmp_filename));
							if (stylesheet != NULL) {
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

								xsltFreeStylesheet(stylesheet);
								stylesheet = NULL;
							}
						}
						if (remove(tmp_filename) == -1) {
							fprintf(stderr, "Unable to delete the tmp file: %s\n", tmp_filename);
						}
					}
					free(tmp_filename);
					tmp_filename = NULL;
				}
			}

			xmlFreeDoc(doc);
			doc = NULL;
		}
	}

	return result;
}

int xml2json_file(xml2json_buffer_t *output, const char *filename) {
	int result = -1;
	xml2json_buffer_t buffer = { 0 };
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
