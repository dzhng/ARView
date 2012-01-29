#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// openCV includes
#include "cv.h"
#include "highgui.h"

// blob extraciton
#include "Blob.h"
#include "BlobResult.h"

IplImage* getThresholdedImage(IplImage* img);
IplImage* getBlobs(IplImage* img);

int main(int argc, char *argv[])
{
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

// Threshold the image by its color
IplImage* getThresholdedImage(IplImage* img)
{
	// new image object to store the thresholded image
	IplImage* imgThreshed = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	// perform thresholding on input image and store in imgThreshed
	cvInRangeS(img, cvScalar(0, 70, 0, 0), cvScalar(90, 150, 40, 0), imgThreshed);

	// return thresholded image
	return imgThreshed;
}

// get blobs and filter them using its area
IplImage* getBlobs(IplImage* img)
{
	CBlobResult blobs;
	CBlobGetCompactness comp;
	int i, tot;
	CBlob nthBlob;
	
	// find white blobs in thresholded image
	//printf("%x ", *((char*)img->imageData));
	blobs = CBlobResult(img, NULL, 0, false);
	// exclude the huge blobs - won't be helpful anyways
	blobs.Filter(blobs, B_EXCLUDE, CBlobGetArea(), B_GREATER, 50000);
	// exclude small uncessary blobs
	blobs.Filter(blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 150);
	// exclude non-circle blobs
	blobs.Filter(blobs, B_EXCLUDE, CBlobGetCompactness(), B_GREATER, 5);

	// new image object to store the output image
	IplImage* displayedImage = cvCreateImage(cvGetSize(img), 8, 1);
	
	printf("%d blobs found\n", blobs.GetNumBlobs());
	// filter out the top 4 biggest blob and display
	if((tot = blobs.GetNumBlobs()) > 4) {
		tot = 4;
	}
	comp = CBlobGetCompactness();
	for (i = 0; i < tot; i++ )
	{
		blobs.GetNthBlob((funcio_calculBlob*)(&CBlobGetArea()), i, nthBlob);
		nthBlob.FillBlob(displayedImage, CV_RGB(255,255,255));
	}

	return displayedImage;
}

