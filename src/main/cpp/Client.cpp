#include "Client.h"

int Client::client_test() {
	struct sockaddr_in si_other;
	int s, i;
	socklen_t slen = sizeof(si_other);
	char buffer[BUFFSIZE];
	char message[BUFFSIZE];

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		KILL("socket");
	} 

	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);

	if (inet_aton(SERVER_IP, &si_other.sin_addr) == 0) {
		ERROR("inet_aton() failed\n");
	}

	while(true) {
		printf("Enter message: ");
		std::cin >> message;

		// send message
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *)&si_other, slen) == -1) {
			KILL("sendto()");
		}

		// receive a reply and print it
		// clear the buffer by filling null
		memset(buffer, '\0', BUFFSIZE);
		if (recvfrom(s, buffer, BUFFSIZE, 0, (struct sockaddr *)&si_other, &slen) == -1) {
			KILL("recvfrom()");
		}

		std::cout << buffer << std::endl;
	}

	close(s);
	return 0;
}