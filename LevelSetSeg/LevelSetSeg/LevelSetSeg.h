#pragma once
#include "opencv/cv.hpp"

class LevelSetSeg
{
public:
	cv::Mat simpleseg( cv::Mat I, cv::Mat init_mask, size_t max_its,
				   int E, int T, double alpha );
private:
	cv::Mat shiftR( cv::Mat M );
	cv::Mat shiftL( cv::Mat M );
	cv::Mat shiftU( cv::Mat M );
	cv::Mat shiftD( cv::Mat M );
	cv::Mat getCurvature( cv::Mat phi );
};