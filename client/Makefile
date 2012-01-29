
CC= g++

NAME= client
SRC= processimage.c

prefix=/usr/local
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir_old=${prefix}/include/opencv
includedir_new=${prefix}/include

LIBS= -L${libdir} -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -L../libs/cvblobslib -lblob -L../libs/aruco/build/src -laruco
CFLAGS= -O2 -I${includedir_old} -I${includedir_new} -I../libs/cvblobslib -Wall


all:
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(NAME)

clean:
	rm $(NAME)
