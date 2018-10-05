#pragma once
#include "opencv/cv.hpp"
#include <stdint.h>

class LevelSetSeg
{
public:
	LevelSetSeg( size_t max_its, int E, int T, double alpha );
	~LevelSetSeg( );

	cv::Mat simpleseg( cv::Mat I, cv::Mat init_mask );

	double mAlpha;
	int mT;
	int mE;
	size_t mMax_its;
private:
	cv::Mat shiftR( cv::Mat M );
	cv::Mat shiftL( cv::Mat M );
	cv::Mat shiftU( cv::Mat M );
	cv::Mat shiftD( cv::Mat M );
	cv::Mat getCurvature( cv::Mat phi );
};