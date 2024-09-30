#ifndef REQUESTS_H
#define REQUESTS_H

char* construct_get_request(const char* path, const char* host);
int send_get_request(int socket_fd, const char* path, const char* host);

#endif
