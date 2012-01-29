#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// openCV includes
#include "cv.h"
#include "highgui.h"

// marker AR includes
#include "aruco.h"
#include "cvdrawingutils.h"

using namespace cv;
using namespace aruco;

int main(int argc, char *argv[])
{
	// video in
	VideoCapture vc;
	Mat frame;

	// marker detector
	MarkerDetector detector;
	vector<Marker> markers;

	cv::namedWindow("Video In", CV_WINDOW_AUTOSIZE);
	vc.open(0);

	while(vc.grab()) {
		vc.retrieve(frame);

		// detect markers
		detector.detect(frame, markers);
		for (unsigned int i=0; i<markers.size(); i++) {
			cv::line(frame, markers[i][0], markers[i][1], Scalar(0,0,255), 2, CV_AA);                             
			cv::line(frame, markers[i][1], markers[i][2], Scalar(0,255,0), 2, CV_AA);                             
			cv::line(frame, markers[i][2], markers[i][3], Scalar(255,0,0), 2, CV_AA);                             
			cv::line(frame, markers[i][3], markers[i][0], Scalar(255,0,255), 2, CV_AA);                             
		}

		cv::imshow("Video In", frame);
	}
}

