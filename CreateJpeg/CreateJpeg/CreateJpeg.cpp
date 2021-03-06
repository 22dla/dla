// CreateJpeg.cpp: определяет точку входа для консольного приложения.
//

#include "opencv2/opencv.hpp"
#include <ctime>

using namespace cv;
using namespace std;

int main()
{
	clock_t start = clock( );
	Mat image = imread( "jur_07__rec1215.bmp", CV_LOAD_IMAGE_GRAYSCALE );

	int scale = 10;
	resize( image, image, image.size( ) / scale, 0, 0, INTER_AREA );

	vector<int>parameters( 2 );
	parameters[0] = CV_IMWRITE_JPEG_QUALITY;
	parameters[1] = 100;

	resize( image, image, image.size( ) * scale, 0, 0, INTER_CUBIC );
	imwrite( "result.jpg", image, parameters );

	cout << (clock( ) - start) / 1000.0 << " sec" << endl;
	system( "pause" );
    return 0;
}

