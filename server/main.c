#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "network.h"

int main(void)
{
	if(serverInit()) {
		perror("server init error");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		serverReceive();
	}

	if(serverClose()) {
		perror("server close error");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

