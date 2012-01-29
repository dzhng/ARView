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

// network includes
#include "network.h"

using namespace cv;
using namespace aruco;

int main(int argc, char *argv[])
{
	Coor xy[4];

	// check arguments
	if(argc < 2) {
		printf("Make sure to input hostname!\n");
		exit(EXIT_FAILURE);
	}
	
	// video in
	VideoCapture vc;
	Mat frame;

	// marker detector
	MarkerDetector detector;
	vector<Marker> markers;

	// initialize the network
	if(clientInit(argv[1])){
		perror("Client init error");
	}

	cv::namedWindow("Video In", CV_WINDOW_AUTOSIZE);
	vc.open(0);

	while(vc.grab()) {
		vc.retrieve(frame);

		// detect markers
		detector.detect(frame, markers);
		for (unsigned int i=0; i<markers.size(); i++) {
			// only work for id of 256
			if(markers[i].id == 256) {
				cv::line(frame, markers[i][0], markers[i][1], Scalar(0,0,255), 2, CV_AA);
				cv::line(frame, markers[i][1], markers[i][2], Scalar(0,255,0), 2, CV_AA);
				cv::line(frame, markers[i][2], markers[i][3], Scalar(255,0,0), 2, CV_AA);
				cv::line(frame, markers[i][3], markers[i][0], Scalar(255,0,255), 2, CV_AA);

				// make coordinates
				xy[0].x = (int)floor(markers[i][0].x);
				xy[0].y = (int)floor(markers[i][0].y);

				xy[1].x = (int)floor(markers[i][1].x);
				xy[1].y = (int)floor(markers[i][1].y);

				xy[2].x = (int)floor(markers[i][2].x);
				xy[2].y = (int)floor(markers[i][2].y);

				xy[3].x = (int)floor(markers[i][3].x);
				xy[3].y = (int)floor(markers[i][3].y);

				// send coordinates
				clientSendCoor(xy, 4);
			}
		}
		cv::imshow("Video In", frame);
	}

	clientClose();
}

