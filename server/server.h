#ifndef SERVER_H
#define SERVER_H

struct route;

typedef struct http_request {
	char method[10];
	char uri[100];
	char version[10];
} http_request;

typedef struct arg_struct {
	int* client_fd;
	struct route* available_routes;
	int* route_count;
} arg_struct;

void *handle_client(void *arguments);


#endif
