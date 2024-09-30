#ifndef ROUTES_H
#define ROUTES_H

#include "server.h"

void handle_home(http_request* req, char* response, int* response_len);
void handle_api(http_request* req, char* response, int* response_len);

#endif
