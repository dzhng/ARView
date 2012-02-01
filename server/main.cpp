#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "network.h"
#include "display.h"
#include <cv.h>
#include <highgui.h>
#include "draw.h"
//void draw(IplImage *img, int32_t x, int32_t y)


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

	//IplImage* newim=cvCreateImage(cvSize(SIZEY,SIZEX),IPL_DEPTH_32S,4);
	

	IplImage* newim = cvLoadImage("aston.jpg", CV_LOAD_IMAGE_COLOR);
        /* initialize font and add text */
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
        cvPutText(newim, "Draw with your finger!", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));


	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);	//initialize window
        cvShowImage("image", newim);			//show image

	cvWaitKey(0);
	
	
	while(1)
	{
		if((n = serverReceive(xy)) == 5) {
			// debug inputs
			/*xy[0].x = 633;
			xy[0].y = 427;
			xy[1].x = 581;
			xy[1].y = 385;
			xy[2].x = 627;
			xy[2].y = 382;
			xy[3].x = 633;
			xy[3].y = 427;
			xy[4].x = 587;
			xy[4].y = 429;*/
			
			// debug prints
			/*printf("x:%d, y:%d\n", xy[0].x, xy[0].y);
			printf("x:%d, y:%d\n", xy[1].x, xy[1].y);
			printf("x:%d, y:%d\n", xy[2].x, xy[2].y);
			printf("x:%d, y:%d\n", xy[3].x, xy[3].y);
			printf("x:%d, y:%d\n", xy[4].x, xy[4].y);*/

			src[0] = Point2f((double)(xy[1].x), (double)(xy[1].y));
			src[1] = Point2f((double)(xy[2].x), (double)(xy[2].y));
			src[3] = Point2f((double)(xy[3].x), (double)(xy[3].y));
			src[2] = Point2f((double)(xy[4].x), (double)(xy[4].y));
			T = getPerspectiveTransform(src, dest);

			printf("%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n", 
					T.at<double>(0,0), T.at<double>(0,1), T.at<double>(0,2), 
					T.at<double>(1,0), T.at<double>(1,1), T.at<double>(1,2),
					T.at<double>(2,0), T.at<double>(2,1), T.at<double>(2,2));

			z = T.at<double>(2,0) * (double)(xy[0].x) + T.at<double>(2,1) * (double)(xy[0].y) + T.at<double>(2,2);
			pt.x = (int)((T.at<double>(0,0) * (double)(xy[0].x) + T.at<double>(0,1) * (double)(xy[0].y) + T.at<double>(0,2))/z);
			pt.y = (int)((T.at<double>(1,0) * (double)(xy[0].x) + T.at<double>(1,1) * (double)(xy[0].y) + T.at<double>(1,2))/z);
			printf("x:%d, y:%d, z:%f\n", pt.x, pt.y, z);
			
			draw(newim,(int32_t)(pt.x),(int32_t)(pt.y));
		}
	}

	if(serverClose()) {
		perror("server close error");
		exit(EXIT_FAILURE);
	}
	cvDestroyWindow("image");
	cvReleaseImage(&newim);
	exit(EXIT_SUCCESS);
}
