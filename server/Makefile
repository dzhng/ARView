
CC= g++

NAME= server
SRC= server.c #processimage.cpp

LIBS= -L../libs/aruco/build/src 
CFLAGS= -O2 -I../include -Wall 


all:
	$(CC) $(CFLAGS) $(SRC) $(LIBS) `pkg-config opencv --cflags --libs` -o $(NAME)

clean:
	rm $(NAME)
