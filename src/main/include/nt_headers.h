#include <iostream>

// Server headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFLEN 512 // Max length of buffer
#define PORT 8080

#define KILL(s) perror(s); exit(1)