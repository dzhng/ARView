
CC= g++

NAME= server
SRC= main.cpp server.c draw.c

LIBS= -L../libs/aruco/build/src 
CFLAGS= -O2 -I../include -Wall 


all:
	$(CC) $(CFLAGS) $(SRC) $(LIBS) `pkg-config opencv --cflags --libs` -o $(NAME)

clean:
	rm $(NAME)
