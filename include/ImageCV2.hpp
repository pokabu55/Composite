#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class CImageCV2
{
public:

	CImageCV2(void);
	CImageCV2( const CImageCV2 &res );
	CImageCV2& operator=( const CImageCV2 &res );

	bool	Load8( string fname );
	bool	Load24( string fname );

	//Tif
	bool	LoadTif8( string fname );
	bool	LoadTif16( string fname );
	bool	LoadTif24( string fname );

	//Raw
	bool	LoadRaw( string fileName, int ImgW, int ImgH, bool isGray );
	bool	LoadCsv( string fileName, int ImgW, int ImgH, bool isGray );

	bool	ConvertRgb2Gray( CImageCV2 *Dst );
	bool	ConvertGray2Rgb( CImageCV2 *Dst );
	bool	Convert16to8( CImageCV2 *Dst, bool NonZero = false );
	bool	Convert16toRGB( CImageCV2 *Dst, bool NonZero = false );
	bool	Convert16to8( CImageCV2 *Dst, int oldMin, int oldMax );
	bool	Convert8( vector<vector<double> > &src );

	int		GetImgW(){ return m_Img.cols; };
	int		GetImgH(){ return m_Img.rows; };
	int		GetChNum(){ return m_Img.channels(); };
	int		GetDepth();
	
	void	Show( string ShowName = "" );
		
	bool	Save( string fname );
	bool	Save( string fname, int ImgNo, int digit, string Ext, bool Normalize );
	bool	SaveTxt( string fname );
	bool	SaveTxt( string fname, int no, int digit, string Ext );

	int		GetImg( int x, int y );
	void	SetImg( int x, int y, int brit );
	
	bool	GetImg24( int x, int y, int *R, int *G, int *B );
	void	SetImg( int x, int y, int R, int G, int B );

	double	GetImgD( int x, int y );
	void	SetImgD( int x, int y, double brit );

	bool	Normalization();
	bool	Normalization( double targetAvg, double targetSig );
	bool	Normalization16( double targetAvg, double targetSig, bool NonZero );
	bool	Normalize32Fto8U( float OldMin, float OldMax, float NewMin, float NewMax, float Irregular );

	bool	Create8( int Width, int Height );
	bool	Create16( int Width, int Height );
	bool	Create24( int Width, int Height );
	bool	Create32F( int Width, int Height );
	bool	Create64F( int Width, int Height );
	bool	CreateClone( CImageCV2 *Src );
		
	bool	CutSquare( CImageCV2 *Dst, int cx, int cy, int CutR );
	bool	Cut( CImageCV2 *Dst, int sx, int sy, int CutW, int CutH );
	bool	Cut24( CImageCV2 *Dst, int sx, int sy, int CutW, int CutH );
	bool	Cut16( CImageCV2 *Dst, int sx, int sy, int CutW, int CutH );
	bool	PasteTriangle( CImageCV2 &Src, Point2f *TriPt );
	bool	InTriangle( int x, int y, Point2f *TriPt );
	bool	paintTriangle( vector<Point2f> &TriPt, int R, int G, int B );
	bool	InTriangle( int x, int y, vector<Point2f> &TriPt );
	bool	placeCenter( CImageCV2 &src );

	bool	ResizeV2( CImageCV2 *Dst, int TargetW, int TargetH );
	bool	ResizeV2( CImageCV2 *Dst, float Ratio );
	
	bool	SetPartialImg( CImageCV2 *Partial, int sx, int sy );
	bool	SetPartialImg24( CImageCV2 *Partial, int sx, int sy, bool ThroughZero = false );
	bool	SetPartialImgV2( CImageCV2 *Partial, int sx, int sy, int Ry1, int Ry2 );

	bool	GetAvg( double &Avg, double &sig, bool NonZero );
	bool	GetAvgMinMax( double &Avg, double &sig, int &min, int &max, bool NonZero );
	bool	AdjustBrit( double avg0, double sig0, double avg1, double sig1 );
	
	bool	transPolar( cv::Point2i center, double res, CImageCV2 &dst, int rad = 200 );
	bool	transPolar2( cv::Point2i center, double res, CImageCV2 &dst, int rad = 200 );
	
	void	clear();

/*
	CImageCV2( const CImageCV2 &res );
	virtual ~CImageCV2(void);

	CImageCV2& operator=( const CImageCV2 &res );

	bool	LoadTif16( string fname );
	bool	LoadRch( string fname );
	bool	LoadGch( string fname );
	bool	LoadBch( string fname );

	bool	Create8( int W, int H );
	bool	Create24( int W, int H );
	bool	Create16( int W, int H );
	bool	CreateClone( CImageCV2 *Src );
	bool	CreateClone( CbImage *Src );
	bool	CreateClone( CwImage *Src );
	
	bool	Copy( CImageCV2 *Src );
	bool	CopyB( CbImage *Src );
	bool	Copy16( CwImage *Src );
	bool	Copy16to8( CwImage *Src );

	bool	Cut( CImageCV2 *Src, int sx, int sy, int CutW, int CutH );
	bool	CutSquare( CImageCV2 *Dst );
	bool	Cut16( CImageCV2 *Src, int sx, int sy, int CutW, int CutH );

	bool	Clone( Mat *Src );
	bool	Clone( IplImage *Src );
	bool	CopyFromMat( Mat *Src );

	void	ReleaseMem();

	//BYTE	GetImg( int x, int y );
	int		GetImg( int x, int y );
	bool	GetImg( int x, int y, int *R, int *G, int *B );
	bool	GetImgGray( int x, int y, int *Gray );
	WORD	GetImg16( int x, int y );

	//void	SetImg( int x, int y, BYTE value );
	void	SetImg( int x, int y, int value );
	bool	SetImg( int x, int y, int R, int G, int B );
	void	SetValueC( int x, int y, BYTE R, BYTE G, BYTE B );
	void	SetImg16( int x, int y, WORD value );

	bool	ConvertB( CbImage *Dst );
	bool	ConvertW( CwImage *Dst );
	bool	Convert8b( CImageCV2 *Dst, int TargetMin, int TargetMax );
	bool	Convert8b( CImageCV2 *Dst );
	bool	Convert24( CImageCV2 *Dst );
	bool	Convert8b1c( bool Blending );
	bool	Convert16to8( CImageCV2 *Dst );
	bool	Convert16to8( CImageCV2 *Dst, int TargetMin, int TargetMax );
	bool	Convert24to8( CImageCV2 *Dst, bool BlendingRGB = true );

	void	Show( char *str );
	void	Show( string ShowName, string FolderPath = "" );
	void	Show( string ShowName, IplImage *Src );
	void	CloseShowWindow();

	bool	Save( string fname );
	bool	Save( string fname, int sx, int sy, int CutW, int CutH );
	bool	Save8b3c( string fname, int sx, int sy, int CutW, int CutH );
	bool	Save16to8b( string fname, bool Norm = true );
	bool	Save16to8b( string fname, int sx, int sy, int CutW, int CutH, bool Norm = true );
	bool	Save16Tif( string fname );
	bool	Save16Tif( string fname, int sx, int sy, int CutW, int CutH );
	bool	Save( string fname, int ImgNo, int digit, string Ext, bool Normalize = false );


	bool	ReduceSize( CImageCV2 *Dst, int Ratio );
	bool	Resize( CImageCV2 *Dst, int TargetSize, bool ShortStd = false );*/

/*	int		GetMaxBrit();
	int		GetMinBrit();
	bool	GetMinMaxBrit( int *Min, int *Max );
	bool	ExpandHistogram( int NewMin = 0, int NewMax = 255 );

	bool	WriteString( string str, int sx, int sy, double Scale = 1.0 );
	bool	SetPartial( CImageCV2 *Src, int sx, int sy );
	bool	SetZero();
*/
	Mat		m_Img;
/*	int		m_iW;
	int		m_iH;
	int		m_iCh;
	int		m_iDepth;

	bool	m_bOpenShowWindow;
	char	*m_chrWindowName;*/

};
	