#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "requests.h"

int main() {
	// Create client socket that will later be used to connect to the server.
	int domain = AF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;
	int client_fd = socket(domain, type, protocol);
	if (client_fd < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Define the address of the server such that the client can connect to the right address
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(1234);
	int status = connect(client_fd, (struct sockaddr *)&address, sizeof(address));
	if (status < 0) {
		perror("connection failed");
		exit(EXIT_FAILURE);
	}

	// Send a message over the connection and read back the response
	const char* path = "/";
	const char* host = "localhost:1234";
	int result = send_get_request(client_fd, path, host);

	if (result < 0) {
		fprintf(stderr, "Failed to send GET request\n");
	} else {
		printf("Request sent succesffully\n");
		char buffer[1024] = {0};
		ssize_t valread = read(client_fd, buffer, 1024-1);
		printf("%s\n", buffer);
	}

	// Close the connection
	close(client_fd);
}
