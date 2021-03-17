#include <iostream>

// Server headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Threading
#include <thread>

#define SERVER "127.0.0.1"
#define BUFFLEN 512 // Max length of buffer
#define PORT 8080

// Error control
#define KILL(s) perror(s); exit(1)
#define ERROR(s) fprintf(stderr, s); exit(1)