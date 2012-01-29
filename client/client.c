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

#define SERVERPORT "4950"	// the port users will be connecting to

#define NUM_COOR 5

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

	if ((rv = getaddrinfo(hostname, SERVERPORT, &hints, &servinfo)) != 0) {
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
		perror("talker: sendto");
		return 1;
	}
	return 0;
}
/*
int32_t main(int32_t argc, char *argv[])
{
	hostname = argv[1];
	int32_t err=0;
	Coor xy[NUM_COOR];
	xy[0].x = 1;
	xy[0].y = 8;
	xy[1].x = 2;
	xy[1].y = 2;
	xy[2].x = 3;
	xy[2].y = 3;
	xy[3].x = 4;
	xy[3].y = 4;
	xy[4].x = 13;
	xy[4].y = 11;

	if((err = clientInit(hostname))) {
		exit(err);
	}

	while(!clientSendCoor(xy, NUM_COOR))
	{
		sleep(2);
	}

	if((err = clientClose())) {
		exit(err);
	}

	exit(err);
}
*/
