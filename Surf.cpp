// Surf.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void PyrMat(Mat& Mat);

int _tmain(int argc, _TCHAR* argv[])
{
	//Initial detecting unity
	SurfFeatureDetector detector(400);
	vector<KeyPoint> lastKeyPoint;
	vector<KeyPoint> currentKeyPoint;
	//Read Video
	VideoCapture capture("..\\testVideo.mp4");
	//Create the Mat for storing current frame and last frame
	Mat frame;
	Mat lastFrame;
	if (!capture.isOpened())
	{
		cout<<"Video open failed!"<<endl;
		return 0;
	}
	namedWindow("Window");
	while(1)
	{
		//Compute the key points in each frame--------

		//Get Start Time to evaluate time period of the algorithm
		double t = (double)getTickCount();

		//Read a frame from video capture
		//And move it to previous frame
		lastFrame = null;
		lastFrame = frame.clone();
		capture>>frame;
		if(lastFrame.empty())
		{
			//Skip first frame;
			continue;
		}

		//进行特征点提取
		detector.detect(lastFrame,lastKeyPoint);
		detector.detect(frame,currentKeyPoint);
		//生成描述符
		SurfDescriptorExtractor extractor;
		Mat descriptor1;
		Mat descriptor2;
		extractor.compute(frame,lastKeyPoint,descriptor1);
		extractor.compute(frame,currentKeyPoint,descriptor2);

		//特征匹配
		FlannBasedMatcher matcher;
		vector<DMatch> matches;
		matcher.match( descriptor1, descriptor2, matches );
		//-- Quick calculation of max and min distances between keypoints
		double min_dist = 100;
		double max_dist = 0;
		for( int i = 0; i < descriptor1.rows; i++ )
		{ 
			double dist = matches[i].distance;
			if( dist < min_dist ) 
				min_dist = dist;
			if( dist > max_dist ) 
				max_dist = dist;
		}

		//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
		//-- PS.- radiusMatch can also be used here.
		vector< DMatch > good_matches;

		for( int i = 0; i < descriptor1.rows; i++ )
		{ 
			if( matches[i].distance < 2*min_dist )
			{
				good_matches.push_back( matches[i]); 
			}
		}

		//Compute the Total Time
		t = ((double)getTickCount()-t)/getTickFrequency();
		cout<<"time is:"<<t<<endl;
		//-- Draw only "good" matches
		Mat img_matches;
		drawMatches( frame, keyPoint, frame, keyPoint2,
			good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
		if (frame.empty())
		{
			cout << " < < <  Video End!  > > > "<<endl;
			break;
		}
		imshow("Window",frame);
		waitKey(10);
	}
	return 0;
	////画出KeyPoint
	//drawKeypoints(img,keyPoint,img);
	//drawKeypoints(smallimg,keyPoint2,smallimg);
	waitKey();
	return 0;
}

void PyrMat(Mat& matrix)
{
}