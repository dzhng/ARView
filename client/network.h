#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stdint.h>

typedef struct {
	int32_t x;
	int32_t y;
} Coor;

int32_t clientInit(char hostname[]);
int32_t clientClose();
int32_t clientSendCoor(Coor *xy, int32_t n);

#endif
