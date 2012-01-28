/*
** a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// socket includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// openCV includes
#include "cv.h"
#include "highgui.h"

// blob extraciton
#include "Blob.h"
#include "BlobResult.h"

#define SERVERPORT "4950"	// the port users will be connecting to

IplImage* getThresholdedImage(IplImage* img);
IplImage* getBlobs(IplImage* img);

int main(int argc, char *argv[])
{
	/*int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	if (argc != 3) {
		fprintf(stderr,"usage: client hostname message\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
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

	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	freeaddrinfo(servinfo);

	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
	close(sockfd);

	return 0;*/

	cvNamedWindow("Video In", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Video Thresholded", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Blobs", CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCreateCameraCapture(0) ;
	IplImage* frame;
	IplImage* thres;
	IplImage* bthres;

	while(1) {
		frame = cvQueryFrame(capture);

		// quit on error
		if( !frame ) {
			perror("Error querying frame");
			break;
		}
		cvShowImage("Video In", frame);

		// threshold and display
		thres = getThresholdedImage(frame);
		cvShowImage("Video Thresholded", thres);
		
		// detect blobs
		bthres = getBlobs(thres);
		cvShowImage("Blobs", bthres);
	}
}

IplImage* getThresholdedImage(IplImage* img)
{
	// new image object to store the thresholded image
	IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);

	// perform thresholding on input image and store in imgThreshed
	cvInRangeS(img, cvScalar(0, 70, 0, 0), cvScalar(90, 150, 40, 0), imgThreshed);

	// return thresholded image
	return imgThreshed;
}

IplImage* getBlobs(IplImage* img)
{
	//////////////////////////////////////////////////////////////
	//// get blobs and filter them using its area
	///////////////////////////////////////////////////////////////
	CBlobResult blobs;
	int i;
	CBlob *currentBlob;
	
	// new image object to store the thresholded image
	IplImage* displayedImage = cvCreateImage(cvGetSize(img), 8, 3);

	// find white blobs in thresholded image
	blobs = CBlobResult(img, NULL, 1, true);
	// exclude the ones smaller than param2 value
	blobs.Filter(blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 80);

	printf("%d blobs found\n", blobs.GetNumBlobs());
	for (i = 0; i < blobs.GetNumBlobs(); i++ )
	{
		currentBlob = blobs.GetBlob(i);
		currentBlob->FillBlob(displayedImage, CV_RGB(255,0,0));
	}

	return displayedImage;
}

