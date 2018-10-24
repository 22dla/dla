#pragma once
#include "opencv/cv.hpp"
#include <stdint.h>

class LevelSetSeg
{
public:
	LevelSetSeg( size_t max_its, int E, int T, double alpha );
	~LevelSetSeg( );

	cv::Mat simpleseg( const cv::Mat I, const cv::Mat init_mask );

	double mAlpha;
	int mT;
	int mE;
	size_t mMax_its;
private:
	cv::Mat shiftR( const cv::Mat M );
	cv::Mat shiftL( const cv::Mat M );
	cv::Mat shiftU( const cv::Mat M );
	cv::Mat shiftD( const cv::Mat M );
	cv::Mat getCurvature( const cv::Mat phi );
};