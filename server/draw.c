#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <stdint.h>

//static IplImage *img;

int32_t draw(IplImage *img, int32_t x, int32_t y)
{
	/* draw a red circle */
	cvCircle(img,                       /* the dest image */
	cvPoint(x, y), 7,      /* center point and radius */
	cvScalar(0, 0, 255, 0),    /* the color; red */
	1, 8, 0);                  /* thickness, line type, shift */

	cvWaitKey(1);			//wait for 1 ms
	cvShowImage("image", img);	//display image
}

//int32_t main(int argc, char** argv)
//{
	//IplImage *img = cvLoadImage("aston.jpg", CV_LOAD_IMAGE_COLOR);
        /* initialize font and add text */
        /*CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
        cvPutText(img, "Draw with your finger!", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));


	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);	//initialize window
        cvShowImage("image", img);			//show image
	int32_t i;	

	for(i=0; i<100; i++)	//call draw function 100 times
	{
		draw(img, i*10, i*10);
	}    

	cvWaitKey(0);		//wait until window closed
        cvReleaseImage( &img );
	cvDestroyWindow("image");
        cvReleaseImage( &img );
     
        return 0;
}*/
