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
	ofstream out( "out.txt" );
	Mat img = imread( "face.jpg", CV_LOAD_IMAGE_GRAYSCALE );//Read image
	
	Mat init_mask( img.size( ), CV_8UC1 );
	init_mask.setTo( 0 );
	init_mask( Rect( 10, 10, 40, 40 ) ).setTo( 255 );

	
	LevelSetSeg Segmentator( 10000, 30, 255, 0.1 );
	Mat result = Segmentator.simpleseg( img, init_mask );
		
	cout << "Time: " << clock( ) / 1000.0 << endl;

	imwrite( "result.jpg", result );
	out << result;
	system( "pause" );
	return 0;
}