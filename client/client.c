#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

// socket includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "network.h"

static int32_t sockfd;
static struct addrinfo hints, *servinfo, *p;
static int32_t rv;
static int32_t numbytes;

int32_t clientInit(char hostname[])
{
	printf("hostname: %s\n", hostname);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;  //UDP

	if ((rv = getaddrinfo(hostname, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}
	return 0;
}

int32_t clientClose()
{
	freeaddrinfo(servinfo);

	printf("talker: sent %d bytes to hostname\n", numbytes);
	close(sockfd);
	return 0;
}

//params: array of coordinates (xy) and size of array (n)
int32_t clientSendCoor(Coor *xy, int32_t n)
{
			     //socket, message, length
	if ((numbytes = sendto(sockfd, xy, sizeof(Coor)*n, 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("sender error");
		return 1;
	}
	printf("%d bytes sent\n", sizeof(Coor)*n);
	return 0;
}

