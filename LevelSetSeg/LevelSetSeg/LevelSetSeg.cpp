// LevelSetSeg.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "LevelSetSeg.h"

using namespace cv;
using namespace std;

LevelSetSeg::LevelSetSeg( size_t max_its, int E, int T, double alpha )
{
	mT = T;
	mE = E;
	mAlpha = alpha;
	mMax_its = max_its;
}

LevelSetSeg::~LevelSetSeg( )
{
	
}

Mat LevelSetSeg::simpleseg( const Mat I, const Mat init_mask )
{
	
	Mat tempBW1, tempBW2;
	distanceTransform( init_mask, tempBW1, CV_DIST_L2, 3 );
	distanceTransform( 255 - init_mask, tempBW2, CV_DIST_L2, 3 );
	Mat phi = -( tempBW1 - tempBW2 );

	for ( int i = 1; i < mMax_its + 1; ++i )
	{
		cout << "Iteration num: " << i << endl;
		Mat D, K, F;
		D = mE - abs( I - mT);
		D.convertTo( D, CV_32FC1 );
		K = getCurvature( phi );

		F = -mAlpha * D + ( 1 - mAlpha ) * K;

		Mat dxplus, dyplus, dxminus, dyminus, gradphimax_x, gradphimin_x,
			gradphimax_y, gradphimin_y, gradphimax, gradphimin, gradphi;
		Mat Fminus, Fplus;
		dxplus = shiftR( phi ) - phi;
		dyplus = shiftU( phi ) - phi;
		dxminus = phi - shiftL( phi );
		dyminus = phi - shiftD( phi );

		sqrt( max( dxplus, 0 ).mul( max( dxplus, 0 ) ) + 
			  max( -dxminus, 0 ).mul( max( -dxminus, 0 ) ), gradphimax_x );
		sqrt( min( dxplus, 0 ).mul( min( dxplus, 0 ) ) + 
			  min( -dxminus, 0 ).mul( min( -dxminus, 0 ) ), gradphimin_x );
		sqrt( max( dyplus, 0 ).mul( max( dyplus, 0 ) ) + 
			  max( -dyminus, 0 ).mul( max( -dyminus, 0 ) ), gradphimax_y );
		sqrt( min( dyplus, 0 ).mul( min( dyplus, 0 ) ) + 
			  min( -dyminus, 0 ).mul( min( -dyminus, 0 ) ), gradphimin_y );
		sqrt( gradphimax_x.mul( gradphimax_x ) + gradphimax_y.mul( gradphimax_y ), 
			  gradphimax );
		sqrt( gradphimin_x.mul( gradphimin_x ) + gradphimin_y.mul( gradphimin_y ), 
			  gradphimin );

		Fplus = ( F > 0 ); Fplus.convertTo( Fplus, CV_32FC1 );
		Fminus = ( F < 0 ); Fminus.convertTo( Fminus, CV_32FC1 );
		gradphi = Fplus.mul( gradphimax ) + Fminus.mul( gradphimin );

		double min, max;
		cv::minMaxLoc( abs( F.mul( gradphi ) ), &min, &max );
		double dt = 0.5 / max;
		phi = phi + dt * ( F.mul( gradphi ) );

		if ( i % 10 == 0 )
		{
			distanceTransform( ( phi < 0 ), tempBW1, CV_DIST_L2, 3 );
			distanceTransform( ( phi > 0 ), tempBW2, CV_DIST_L2, 3 );
			phi = tempBW2 - tempBW1;
		}
		Mat seg = ( phi < 0 );
		
		Laplacian( seg, seg, CV_8UC1, 3, 1, 0, BORDER_DEFAULT );
		namedWindow( "Segmentation process", WINDOW_NORMAL );
		imshow( "Segmentation process", seg + I );
		if ( waitKey( 10 ) == 27 )
		{
			cout << "Esc key is pressed by the user. Stopping the process" << endl;
			break;
		}
	}
	return ( phi < 0 );
}

Mat LevelSetSeg::shiftR( const Mat M )
{
	Mat result( M.size( ), CV_32FC1 );

	M( Rect( 0, 0, 1, M.rows ) ).copyTo( result( Rect( 0, 0, 1, M.rows ) ) );
	M( Rect( 0, 0, M.cols - 1, M.rows ) ).copyTo( result( Rect( 1, 0, M.cols - 1, M.rows ) ) );

	return result;
}

Mat LevelSetSeg::shiftL( const Mat M )
{
	Mat result( M.size( ), CV_32FC1 );

	M( Rect( M.cols - 1, 0, 1, M.rows ) ).copyTo( result( Rect( M.cols - 1, 0, 1, M.rows ) ) );
	M( Rect( 1, 0, M.cols - 1, M.rows ) ).copyTo( result( Rect( 0, 0, M.cols - 1, M.rows ) ) );

	return result;
}

Mat LevelSetSeg::shiftU( const Mat M )
{
	Mat result( M.size( ), CV_32FC1 );
	M.copyTo( result );
	result = shiftL( result.t( ) ).t( );

	return result;
}

Mat LevelSetSeg::shiftD( const Mat M )
{
	Mat result( M.size( ), CV_32FC1 );
	M.copyTo( result );
	result = shiftR( result.t( ) ).t( );

	return result;
}

Mat LevelSetSeg::getCurvature( const Mat phi )
{
	Mat dx, dy, dxplus, dyplus, dxminus, dyminus, dxplusy, dyplusx,
		dxminusy, dyminusx, nplusx, nplusy, nminusx, nminusy, temp;

	dx = ( shiftR( phi ) - shiftL( phi ) ) / 2;
	dy = ( shiftU( phi ) - shiftD( phi ) ) / 2;
	dxplus = shiftR( phi ) - phi;
	dyplus = shiftU( phi ) - phi;
	dxminus = phi - shiftL( phi );
	dyminus = phi - shiftD( phi );
	dxplusy = ( shiftU( shiftR( phi ) ) - shiftU( shiftL( phi ) ) ) / 2;
	dyplusx = ( shiftR( shiftU( phi ) ) - shiftR( shiftD( phi ) ) ) / 2;
	dxminusy = ( shiftD( shiftR( phi ) ) - shiftD( shiftL( phi ) ) ) / 2;
	dyminusx = ( shiftL( shiftU( phi ) ) - shiftL( shiftD( phi ) ) ) / 2;

	sqrt( dxplus.mul( dxplus ) +
		( ( dyplusx + dy ) / 2 ).mul( ( ( dyplusx + dy ) / 2 ) ), temp );
	nplusx = dxplus / temp;

	sqrt( dyplus.mul( dyplus ) +
		( ( dxplusy + dx ) / 2 ).mul( ( ( dxplusy + dx ) / 2 ) ), temp );
	nplusy = dyplus / temp;

	sqrt( dxminus.mul( dxminus ) +
		( ( dyminusx + dy ) / 2 ).mul( ( ( dyminusx + dy ) / 2 ) ), temp );
	nminusx = dxminus / temp;

	sqrt( dyminus.mul( dyminus ) +
		( ( dxminusy + dx ) / 2 ).mul( ( ( dxminusy + dx ) / 2 ) ), temp );
	nminusy = dyminus / temp;

	return ( ( nplusx - nminusx ) + ( nplusy - nminusy ) / 2 );
}