#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include "requests.h"

#define MAX_REQUEST_LEN 102

char* construct_get_request(const char* path, const char* host) {
	char* request = malloc(MAX_REQUEST_LEN);
	snprintf(request, MAX_REQUEST_LEN,
		  "GET %s HTTP/1.1\r\n"
		  "Host: %s\r\n"
		  "Connection: close\r\n"
		  "\r\n",
		  path, host);

	return request;
}

int send_get_request(int socket_fd, char* path, const char* host) {
	char* request = construct_get_request(path, host);
	ssize_t bytes_sent = send(socket_fd, request, strlen(request), 0);

	free(request);
	free(path);

	if (bytes_sent < 0) {
		perror("Failed to send request");
		return -1;
	}
	return 0;
}
