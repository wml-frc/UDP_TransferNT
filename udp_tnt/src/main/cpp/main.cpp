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

template<typename T>
void kill(T s) {
	perror(s);
	exit(1);
}

int main() {
	struct sockaddr_in si_me, si_other;

	int s, i, recv_len;
	socklen_t slen = sizeof(si_other);

	char buffer[BUFFLEN];

	// create UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		kill("socket");
	}

	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));

	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind socket to port
	if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1) {
		kill("bind");
	}

	// keep listening for data
	while (true) {
		printf("Waiting for data...\n");
		fflush(stdout);

		// try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buffer, BUFFLEN, 0, (struct sockaddr *)&si_other, &slen)) == -1) {
			kill("recvfrom()");
		}

		// Print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n", buffer);

		// now reply the client with same data
		if (sendto(s, buffer, recv_len, 0, (struct sockaddr *)&si_other, slen) == -1) {
			kill("sendto()");
		}

		memset(buffer, 0, sizeof(buffer));
	}

	close(s);

	return 0;
}