#include "route.h"
#include "server.h"
#include "routes.h"
#include "string.h"
#include <stdio.h>


void add_route(route* available_routes, int* route_count, const char* path, const char* method, void (*handler)(http_request* req, char* response, int* response_len)) {
	available_routes[*route_count].path = path;
	available_routes[*route_count].method = method;
	available_routes[*route_count].handler = handler;
	(*route_count)++;
}

const route* match_route(const route* available_routes, const int* route_count, const char* path, const char* method) {
	for (int i = 0; i < *route_count; i++) {
		printf("route path: %s\n", available_routes[i].path);
		printf("input path: %s\n", path);
		printf("route path: %s\n", available_routes[i].method);
		printf("input path: %s\n", method);
		printf("comparison: %b\n", strcmp(available_routes[i].path, path));
		printf("comparison: %b\n", strcmp(available_routes[i].method, method));
		if (strcmp(available_routes[i].path, path) == 0 && strcmp(available_routes[i].method, method) == 0) {
			return &available_routes[i];
		}
	}
	return NULL;
}

void register_routes(route* available_routes, int* route_count) {
	add_route(available_routes, route_count, "/", "GET", handle_home);
	add_route(available_routes, route_count, "/api", "GET", handle_api);
}
