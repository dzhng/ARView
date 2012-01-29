/*
** a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>

#include "network.h"

#define MAXBUFLEN 100

static int sockfd;
static struct addrinfo hints, *servinfo, *p;
static int rv;
static int numbytes;
static struct sockaddr_storage their_addr;
static char buf[MAXBUFLEN];
static socklen_t addr_len;
static char s[INET6_ADDRSTRLEN];

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int32_t serverInit()
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	return 0;
}

int32_t serverClose()
{
	close(sockfd);
	return 0;
}

int32_t serverReceive()
{
	Coor* pair;
	numbytes=0;
	addr_len=0;
	
	addr_len = sizeof(their_addr);
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		return 1;
	}

	printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof(s)));
	printf("listener: packet is %d bytes long\n", numbytes);

	pair = (Coor*)buf;

	//iterate through all the points, divide by 8 = 4 byte ints * 2 ints per Coor
	int32_t i;
	for(i=0; i<(numbytes/sizeof(Coor)); i++)
	{
		printf("point %d: x=%d y=%d\n", (i+1), 
				pair[i].x, pair[i].y);
	}
	return 0;
}

