// LevelSetSeg.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include <ctime>
#include "LevelSetSeg.h"

using namespace cv;
using namespace std;

int main( )
{
	ofstream out( "out.csv" );
	Mat img = imread( "tumor.jpg" );//Read image
	
	cvtColor( img, img, CV_BGR2GRAY );
	Mat init_mask( img.size( ), CV_8UC1 );
	init_mask.setTo( 0 );
	init_mask( Rect( 130, 120, 40, 40 ) ).setTo( 255 );

	
	LevelSetSeg Segmentator( 300, 30, 200, 0.1 );
	Mat result = Segmentator.simpleseg( img, init_mask );
		
	cout << "Time: " << clock( ) / 1000.0 << endl;

	imwrite( "result.jpg", result );
	out << result;
	system( "pause" );
	return 0;
}