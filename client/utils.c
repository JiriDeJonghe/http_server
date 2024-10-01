#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_INPUT_LEN 256

void read_input(const char* prompt, char* buffer, size_t buffer_size) {
	printf("%s", prompt);
	if (fgets(buffer, buffer_size, stdin) != NULL) {
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') {
			buffer[len-1] = '\0';
		}
	}
}

char* get_method(void) {
	char* method = malloc(MAX_INPUT_LEN);
	read_input("Enter method (GET, POST, etc.): ", method, sizeof(method));
	return method;
}

char* get_path(void) {
	char* path = malloc(MAX_INPUT_LEN);
	read_input("Enter path: ", path, sizeof(path));
	return path;
}
