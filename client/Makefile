
CC= g++

NAME= client
SRC= processimage.cpp

LIBS= -L../libs/aruco/build/src -laruco
CFLAGS= -O2 -I../libs/aruco/src -Wall 


all:
	$(CC) $(CFLAGS) $(SRC) $(LIBS) `pkg-config opencv --cflags --libs` -o $(NAME)

clean:
	rm $(NAME)
