#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "network.h"
#include "display.h"
#include "draw.h"

#include <cv.h>
#include <highgui.h>

using namespace cv;

static Point2f dest[4];	// destination coorinates for transformation matrix
static Point2f src[4];	// source (client) coorinates for transformation matrix

int main(void)
{
	Coor xy[5];
	int n;
	Mat T = Mat(3, 3, CV_32FC1);	// perspective transformation matrix
	double z;

	Coor pt;		// final converted point

	dest[0] = Point2f(SIZEX-MARKER_SIZE, SIZEY-MARKER_SIZE);
	dest[1] = Point2f(SIZEX, SIZEY-MARKER_SIZE);
	dest[3] = Point2f(SIZEX, SIZEY);
	dest[2] = Point2f(SIZEX-MARKER_SIZE, SIZEY);

	if(serverInit()) {
		perror("server init error");
		exit(EXIT_FAILURE);
	}
	
	//IplImage* origImg = cvLoadImage("aston.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage* origImg = cvCreateImage( cvSize(SIZEX,SIZEY), 8, 3);
	cvConvertScaleAbs(origImg, origImg, 0);
	IplImage* resizeImg = cvCreateImage( cvSize(SIZEX,SIZEY), origImg->depth, origImg->nChannels);
	IplImage* displayImg = cvCreateImage( cvSize(SIZEX,SIZEY), origImg->depth, origImg->nChannels);
	IplImage* markupImg = cvCreateImage( cvSize(SIZEX,SIZEY), origImg->depth, origImg->nChannels);
	cvConvertScaleAbs(markupImg, markupImg, 0);
	cvResize(origImg, resizeImg, CV_INTER_LINEAR);

	/* initialize font and add text */
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
	cvPutText(resizeImg, "Draw with your finger!", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));

	//create qrCode image object
	IplImage* qrCode = cvLoadImage("marker256.jpg", CV_LOAD_IMAGE_COLOR);
	//resize qRCode image object
	IplImage* modQR = cvCreateImage( cvSize(MARKER_SIZE,MARKER_SIZE), qrCode->depth, qrCode->nChannels);
	cvResize(qrCode, modQR, CV_INTER_LINEAR);	

	cvSetImageROI(resizeImg, cvRect(resizeImg->width-MARKER_SIZE-20, resizeImg->height-MARKER_SIZE-20, modQR->width, modQR->height));
	cvAddWeighted(resizeImg, 0, modQR, 1, 0.0, resizeImg); 
	cvResetImageROI(resizeImg);

	cvRectangle(resizeImg,                    /* the dest image */
                cvPoint(resizeImg->width-MARKER_SIZE-30, resizeImg->height-MARKER_SIZE-30),        /* top left point */
                cvPoint(resizeImg->width-10, resizeImg->height-10),       /* bottom right point */
                cvScalar(255, 255, 255, 0), /* the color; blue */
                20, 8, 0);

	cvNamedWindow("meow", CV_WINDOW_AUTOSIZE);	//initialize window
	cvShowImage("meow", resizeImg);			//show image

	cvWaitKey(0);

	cvWaitKey(0);
	
	
	while(1)
	{
		if((n = serverReceive(xy)) == 5) {
			src[0] = Point2f((double)(xy[1].x), (double)(xy[1].y));
			src[1] = Point2f((double)(xy[2].x), (double)(xy[2].y));
			src[3] = Point2f((double)(xy[3].x), (double)(xy[3].y));
			src[2] = Point2f((double)(xy[4].x), (double)(xy[4].y));
			T = getPerspectiveTransform(src, dest);

			/*printf("%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n", 
					T.at<double>(0,0), T.at<double>(0,1), T.at<double>(0,2), 
					T.at<double>(1,0), T.at<double>(1,1), T.at<double>(1,2),
					T.at<double>(2,0), T.at<double>(2,1), T.at<double>(2,2));*/

			z = T.at<double>(2,0) * (double)(xy[0].x) + T.at<double>(2,1) * (double)(xy[0].y) + T.at<double>(2,2);
			pt.x = (int)((T.at<double>(0,0) * (double)(xy[0].x) + T.at<double>(0,1) * (double)(xy[0].y) + T.at<double>(0,2))/z);
			pt.y = (int)((T.at<double>(1,0) * (double)(xy[0].x) + T.at<double>(1,1) * (double)(xy[0].y) + T.at<double>(1,2))/z);
			//printf("x:%d, y:%d, z:%f\n", pt.x, pt.y, z);
			
			/* draw a red circle */
			cvCircle(markupImg,						/* the dest image */
				cvPoint(pt.x, pt.y), 7,					/* center point and radius */
				cvScalar(0, 0, 255, 0),				/* the color; red */
				-1);							/* thickness, line type, shift */
			cvCircle(markupImg,						/* the dest image */
				cvPoint(pt.x+rand()%20, pt.y+rand()%20), 7,					/* center point and radius */
				cvScalar(0, 0, 255, 0),				/* the color; red */
				-1);		
			cvCircle(markupImg,						/* the dest image */
				cvPoint(pt.x+rand()%20, pt.y+rand()%20), 7,					/* center point and radius */
				cvScalar(0, 0, 255, 0),				/* the color; red */
				-1);		
			cvCircle(markupImg,						/* the dest image */
				cvPoint(pt.x+rand()%20, pt.y+rand()%20), 7,					/* center point and radius */
				cvScalar(0, 0, 255, 0),				/* the color; red */
				-1);		
		}
		cvConvertScaleAbs(markupImg, markupImg, 0.99);
		cvAdd(resizeImg, markupImg, displayImg); 
		cvShowImage("meow", displayImg);		//display image
		cvWaitKey(1);					//wait for 1 ms
	}

	cvWaitKey(0);		//wait until window closed
	cvDestroyWindow("meow");
	cvReleaseImage( &resizeImg );

	if(serverClose()) {
		perror("server close error");
		exit(EXIT_FAILURE);
	}
	cvDestroyWindow("image");
	cvReleaseImage(&newim);
	exit(EXIT_SUCCESS);
}
