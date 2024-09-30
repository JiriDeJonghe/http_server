#include "routes.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

void handle_home(http_request* req, char* response, int* response_len) {
	const char* body = "Welcome to the home page";
	*response_len = sprintf(response,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: %d\r\n\r\n" 
		"%s",
		(int)strlen(body), body);
}

void handle_api(http_request* req, char* response, int* response_len) {
	const char* body = "{\"message\": \"This is the API endpoint\"}";
	*response_len = sprintf(response,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Content-Length: %d\r\n\r\n" 
		"%s",
		(int)strlen(body), body);
}
