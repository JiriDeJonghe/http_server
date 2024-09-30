#include "server.h"
#include "utils.h"
#include "route.h"
#include "routes.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ROUTES 50

http_request parse_http_request(char *buffer) {
	char method[10], uri[100], version[10];
	sscanf(buffer, "%s %s %s", method, uri, version);

	http_request req;
	strcpy(req.method, (char *)method);
	strcpy(req.uri, (char *)uri);
	strcpy(req.version, (char *)version);
	
	return req;
}

// This handles the interaction with the client. This is used in the following listen step that should be checked out first if implementing from scratch.
void *handle_client(void *arguments) {
	arg_struct *args = (struct arg_struct *)arguments;
	int sock = *(int*)args->client_fd;
	char buffer[1024] = {0};
	char response[4096];
	int response_len = 0;
	
	ssize_t valread = read(sock, buffer, 1024-1);
	if (valread < 0) {
		perror("read failed");
	} else {
		printf("Received message\n");
		http_request req = parse_http_request(buffer);

		const route* matched_route = match_route(args->available_routes, args->route_count, req.uri, req.method);
		if (matched_route) {
			matched_route->handler(&req, response, &response_len); // Access member of a struct trough a ponter
		} else {
			strcpy(response, "HTTP/1.1 404 Not Found\r\n");
			response_len = strlen(response);
		}

		if (send(sock, response, response_len, 0) < 0) {
			perror("send failed");
		} else {
			printf("message sent\n");
			fflush(stdout);
		}
	}
	close(sock);
	free(args->client_fd);
	return NULL;
}


int main() {
	// Set Up Routes
	route available_routes[MAX_ROUTES];
	int route_count = 0;
	register_routes(available_routes, &route_count);
	// Create server socket that will later be used to connect to the clients.
	int domain = AF_INET; // value for IPv4 connections only
	int type = SOCK_STREAM; // value for TCP
	int protocol = 0; // value for IP
	int sockfd = socket(domain, type, protocol);
	if (sockfd < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Bind the socket to a specific address to listen on. This address will later be used by client's to connect to this server.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY; // constant that tells system to bind the socket to all avialable network interfaces
	server_address.sin_port = htons(1234); // different computer architectures store values in different orders (big or little-endian). Network standard is big-endian. htons (host-to-network-short) does the conversion if necesary
	if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
			// explicit casting of the struct to make sure the compiler knows it's okay
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Make the socket listen by marking it as passive, which will allow it to accept incoming connection requests
	int backlog = 4; // backlog affects how mamy incoming connections can queue up. Most sytems use 128. Let's take 4 for a simple server
	if (listen(sockfd, backlog) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	// Implement Accept functionality: it takes the first connection request from the backlog queue. It creates a new connected socket, and returns a new file descriptor referring to that socket.
	while(1) {
		struct sockaddr_in client_address;
		socklen_t addrlen = sizeof(client_address);
		int *client_fd = malloc(sizeof(int));

		*client_fd = accept(sockfd, (struct sockaddr *)&client_address, &addrlen); // accept automatically extracts the first connection request from the queue and returns a new socket descriptor specifically for this connection. The address of this connection is stored in &address, which is why we can use this as input
		//
		if (client_fd < 0) {
			perror("accept failed");
			continue;
		}
		
		// For a multithreaded approach we use the library pthread. We create a thread for each connection request. After handle_client has been completed, we close the thread.
		pthread_t thread_id;
		struct arg_struct args;
		args.client_fd = client_fd;
		args.available_routes = available_routes;
		args.route_count = &route_count;
		if (pthread_create(&thread_id, NULL, handle_client, (void *)&args) < 0){
			perror("could not create thread");
			free(client_fd);
			continue;
		}
	}
}

