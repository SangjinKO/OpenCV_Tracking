// ConsoleApplication5.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

// Standard include files
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{	
	//Personal Practice
	Mat srcImage = imread("lena.jpg", IMREAD_GRAYSCALE);
	if (srcImage.empty())
	{
		cout <<"no file" <<endl;
		return -1;
	}

	Point pt1(10, 10), pt2(400, 400);
	LineIterator it(srcImage, pt1, pt2, 8);

	vector<uchar> buffer(it.count);
	for (int i = 0; i < it.count ; i++, ++it)
	{
		buffer[i] = **it;
	}

	//cout << (Mat)buffer << endl;
	line(srcImage, pt1, pt2, Scalar(255), 2);

	Mat dstImage(512, 512, CV_8UC3, Scalar(255, 255, 255));
	pt1 = Point (0, dstImage.cols - buffer[0]);
	for (int i = 1; i < buffer.size(); i++, ++it)
	{
		pt2 = Point(i, dstImage.cols - buffer[i]);
		line(dstImage, pt1, pt2, Scalar(255), 2);
		pt1 = pt2;
	}

	imshow("srcImage", srcImage);
	imshow("dstImage", dstImage);

	waitKey();
	// Object Tracking (2017.09)
	
	if (argc<2) {
		cout <<
			" Usage: tracker <video_name>\n"
			" examples:\n"
			" example_tracking_kcf Bolt/img/%04d.jpg\n"
			" example_tracking_kcf faceocc2.webm\n"
			<< endl;
		return 0;
	}
	

	// declares all required variables
	Rect2d roi;
	Mat frame;
	// create a tracker object
	Ptr<TrackerKCF> tracker = TrackerKCF::create();
	// set input video
	
	//***std::string video = argv[1];
	
	VideoCapture cap("TEST3.MP4");

	// get bounding box
	cap >> frame;
	roi = selectROI("tracker", frame);
	//quit if ROI was not selected
	if (roi.width == 0 || roi.height == 0)
		return 0;
	// initialize the tracker
	tracker->init(frame, roi);
	// perform the tracking process
	printf("Start the tracking process, press ESC to quit.\n");
	for (;; ) {
		// get frame from the video
		cap >> frame;
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;
		// update the tracking result
		tracker->update(frame, roi);
		// draw the tracked object
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);
		// show image with the tracked object
		imshow("tracker", frame);
		//quit on ESC button
		if (waitKey(1) == 27)break;
	}

	
	return 0;

}

