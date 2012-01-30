#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stdint.h>

#define PORT "4950"	// the port users will be connecting to

typedef struct {
	int32_t x;
	int32_t y;
} Coor;

int32_t clientInit(char hostname[]);
int32_t clientClose();
int32_t clientSendCoor(Coor *xy, int32_t n);

int32_t serverInit();
int32_t serverClose();
int32_t serverReceive(Coor *xy);

#endif
