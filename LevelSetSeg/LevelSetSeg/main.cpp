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
	Mat img = imread( "tumor.jpg" );//Read first image

	/*Mat bgr[3];   //destination array
	split( img, bgr );//split source  */
	
	cvtColor( img, img, CV_RGB2GRAY );
	Mat init_mask( img.size( ), CV_8UC1 );
	init_mask.setTo( 0 );
	init_mask( Rect( 150, 100, 40, 40 ) ).setTo( 255 );

	
	LevelSetSeg Segmentator;
	//Image, Mask, Num of iteration, Error, (T) Intencity Value, Alpha.
	Mat result = Segmentator.simpleseg( img, init_mask, 1000, 15, 215, 0.5 );

	
	cout << "Time: " << clock( ) / 1000.0 << endl;
	imwrite( "result.jpg", result );
	out << result;
	system( "pause" );
	return 0;
}