#ifndef ROUTE_H
#define ROUTE_H

struct http_request;

typedef struct route {
	const char* path;
	const char* method;
	void (*handler)(struct http_request* req, char* response, int* response_len);
} route;

void add_route(route* available_routes, int* route_count, const char* path, const char* method, void (*handler)(struct http_request* req, char* response, int* response_len));
const route* match_route(const route* available_routes, const int* route_count, const char* path, const char* method);
void register_routes(route* available_routes, int* route_count);

#endif
