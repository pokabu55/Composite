#include "../include/ImageCV2.hpp"

CImageCV2::CImageCV2(void)
{
}

CImageCV2::CImageCV2( const CImageCV2 &res )
{
	//�R�s�[�R���X�g���N�^
	this->m_Img = res.m_Img.clone();
}

CImageCV2& CImageCV2::operator=( const CImageCV2 &res )
{
	this->m_Img = res.m_Img.clone();
	
	return *this;
}

int CImageCV2::GetDepth()
{
	/*
	#define CV_8U   0     //8�r�b�g�A�����Ȃ��f�[�^
	#define CV_8S   1     //8�r�b�g�A�����t���f�[�^
	#define CV_16U  2     //16�r�b�g�A�����Ȃ��f�[�^
	#define CV_16S  3     //16�r�b�g�A�����t���f�[�^
	#define CV_32S  4     //32�r�b�g�A�����t���f�[�^
	#define CV_32F  5     //32�r�b�g�A�P���x�������������_�f�[�^
	#define CV_64F  6     //64�r�b�g�A�{���x�������������_�f�[�^
	#define CV_USRTYPE1 7
	*/
	//MyTrace( "�߂�l������Ȃ̂ŁA�v�m�F�B�֐����ɐ����L��B\n" );
	return m_Img.depth(); 
}

bool CImageCV2::Load8( string fname )
{
	//�����I�ɃO���C�X�P�[���œǂݍ���
	int flags = 0;

	m_Img = imread( fname, flags );

	if( m_Img.empty() )
	{
		return false;
	}

	return true;
}

bool CImageCV2::Load24( string fname )
{
	//�����I��3ch�J���[�œǂݍ���
	int flags = 1;

	m_Img = imread( fname, flags );
	
	if( m_Img.empty() )
	{
		return false;
	}

	return true;
}

bool CImageCV2::LoadTif8( string fname )
{
	//��U�ATif16bit�œǂݍ���ŁA���̌��8bit

	//�摜�����̂܂ܓǂݍ��ށD�������C�A���t�@�`�����l���͖��������D
	int flags = -1;
	Mat buf = imread( fname, flags );

	if( buf.empty() )
	{
		return false;
	}

	//8bit��
	this->Create8( buf.cols, buf.rows );

	double oldMin, oldMax;
	Point minLoc, maxLoc;
	minMaxLoc( buf, &oldMin, &oldMax, &minLoc, &maxLoc );

	//�m���}���C�Y
	int x, y, newBrit;
	double b, c;
	int W = buf.cols;
	int H = buf.rows;
	double newMin = 0., newMax = 255.;

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			b = (double)( buf.at<unsigned short int>(y,x) );
			c = (b-oldMin) * (newMax - newMin) / (oldMax-oldMin) + newMin;

			newBrit = (int)(c+0.5);
			if (newBrit < 0) newBrit = 0;
			else if( newBrit > 255) newBrit = 255;

			this->SetImg( x, y, newBrit );
		}
	}

	return true;
}

bool CImageCV2::LoadTif24( string fname )
{
	//��U�ATif16bit�œǂݍ���ŁA���̌��3ch,24bit

	//�摜�����̂܂ܓǂݍ��ށD�������C�A���t�@�`�����l���͖��������D
	int flags = -1;
	Mat buf = imread( fname, flags );

	if( buf.empty() )
	{
		return false;
	}

	//8bit��
	this->Create24( buf.cols, buf.rows );

	double oldMin, oldMax;
	Point minLoc, maxLoc;
	minMaxLoc( buf, &oldMin, &oldMax, &minLoc, &maxLoc );

	//�m���}���C�Y
	int x, y, newBrit;
	double b, c;
	int W = buf.cols;
	int H = buf.rows;
	double newMin = 0., newMax = 255.;

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			b = (double)( buf.at<unsigned short int>(y,x) );
			c = (b-oldMin) * (newMax - newMin) / (oldMax-oldMin) + newMin;

			newBrit = (int)(c+0.5);
			if (newBrit < 0) newBrit = 0;
			else if( newBrit > 255) newBrit = 255;

			this->SetImg( x, y, newBrit, newBrit, newBrit );
		}
	}

	return true;
}

bool CImageCV2::LoadTif16( string fname )
{
	//Tif16bit�œǂݍ���

	//�摜�����̂܂ܓǂݍ��ށD�������C�A���t�@�`�����l���͖��������D
	int flags = -1;
	m_Img = imread( fname, flags );

	if( m_Img.empty() ) return false;
	if( this->GetChNum() != 1 && this->GetDepth() != 2) return false;

	return true;
}

bool CImageCV2::ResizeV2( CImageCV2 *Dst, int TargetW, int TargetH )
{
	resize( this->m_Img, Dst->m_Img, Size(TargetW, TargetH), 0, 0, CV_INTER_AREA );

	return true;
}

bool CImageCV2::ResizeV2( CImageCV2 *Dst, float Ratio )
{
	int TargetW = (int)( this->GetImgW() * Ratio );
	int TargetH = (int)( this->GetImgH() * Ratio );

	resize( this->m_Img, Dst->m_Img, Size(TargetW, TargetH), 0, 0, CV_INTER_AREA );

	return true;
}

bool CImageCV2::ConvertRgb2Gray( CImageCV2 *Dst )
{
	cvtColor( this->m_Img, Dst->m_Img, CV_BGR2GRAY );

	return true;
}

bool CImageCV2::ConvertGray2Rgb( CImageCV2 *Dst )
{
	int depth = this->GetDepth();
	bool cnvt8 = false;

	switch (depth) {
	case 0:
		break;
	case 2:
		cnvt8 = true;
		break;
	default:
		return false;
	}

	if (cnvt8) {
		CImageCV2 tmp;
		this->Convert16to8( &tmp, true );
		cvtColor( tmp.m_Img, Dst->m_Img, CV_GRAY2BGR );
	}
	else {
		cvtColor( this->m_Img, Dst->m_Img, CV_GRAY2BGR );
	}

	return true;
}

void CImageCV2::Show( string ShowName )
{
	if( ShowName == "" )
	{
		ShowName = "test";
	}
	
	namedWindow( ShowName, CV_WINDOW_AUTOSIZE|CV_WINDOW_KEEPRATIO);
	imshow( ShowName, this->m_Img );
	waitKey( 0 );
	destroyWindow( ShowName );
}

bool CImageCV2::Save( string fname )
{
	imwrite( fname, this->m_Img );

	return true;
}

bool CImageCV2::SaveTxt( string fname )
{
	ofstream ofs( fname );
	int x,y;
	int W = this->GetImgW();
	int H = this->GetImgH();
	
	for (y=0; y<H; y++) {
		for( x=0; x<W; x++) {

			ofs << (int)this->GetImg(x,y) << endl;
		}
	}

	return true;
}

bool CImageCV2::SaveTxt( string fname, int no, int digit, string Ext )
{
	//�t�@�C�����̐���
	{
		string strNo;
		stringstream ss;
		ss << no;
		ss >> strNo;

		int DigCnt = 0;

		do
		{
			no /= 10;
			DigCnt++;

		}while( no > 0 );

		string ff;

		if( DigCnt < digit )
		{
			for( int i = 0; i < (digit - DigCnt ); i++ )
			{
				ff += "0";
			}
		}

		ff += strNo;

		fname += ff;
		fname += Ext;
	}
	
	ofstream ofs( fname );
	int x,y;
	int W = this->GetImgW();
	int H = this->GetImgH();
	
	for (y=0; y<H; y++) {
		for( x=0; x<W; x++) {

			ofs << (int)this->GetImg(x,y) << endl;
		}
	}

	return true;
}

bool CImageCV2::Save( 
	string	fname,	//�ۑ��t�@�C����
	int		ImgNo,	//�ʂ��ԍ�
	int		digit,	//��
	string	Ext,	//�g���q
	bool	Normalize
){
	//�t�@�C�����̐���
	{
		string strNo;
		stringstream ss;
		ss << ImgNo;
		ss >> strNo;

		int DigCnt = 0;

		do
		{
			ImgNo /= 10;
			DigCnt++;

		}while( ImgNo > 0 );

		string ff;

		if( DigCnt < digit )
		{
			for( int i = 0; i < (digit - DigCnt ); i++ )
			{
				ff += "0";
			}
		}

		ff += strNo;

		fname += ff;
		fname += Ext;
	}

	if( m_Img.empty() ) return false;

	/*int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );*/

	if( Normalize )
	{
		int Min = 255;
		int Max = 0;
		int brit;
		int H = GetImgH();
		int W = GetImgW();

		for( int y = 0; y < H; y++ )
		{
			for( int x = 0; x < W; x++ )
			{
				brit = GetImg(x,y);
				if( brit == 0 ) continue;

				if( Min > brit ) Min = brit;
				if( Max < brit ) Max = brit;
			}
		}

		//�ŏ��l���O�A�ő�l���Q�T�T�ɕϊ�
		double coef = 255. / (double)(Max-Min);
		
		for( int y = 0; y < H; y++ )
		{
			for( int x = 0; x < W; x++ )
			{
				brit = (int)( coef * (GetImg(x,y) - Min ) + 0.5 );
				
				if( brit > 255 ) brit = 255;
				else if( brit < 0 ) brit = 0;

				SetImg(x,y,brit);
			}
		}
	}

	//cvSaveImage( dst, m_Img );
	imwrite( fname, this->m_Img );

	return true;
}

bool CImageCV2::Normalization()
{
	int W = GetImgW();
	int H = GetImgH();

	int x, y;
	int brit;
	int Max = -1, Min = -1;

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			brit = GetImg( x, y );

			if( Min < 0 )
			{
				Min = Max = brit;
			}
			else
			{
				if( Min > brit ) Min = brit;
				if( Max < brit ) Max = brit;
			}
		}
	}

	vector<int> LUT;
	LUT.resize( 256, 0 );

	for( int i = 0; i < 256; i++ )
	{
		double k = (double)(i-Min) * 255. / (double)(Max-Min);

		LUT[i] = (int)(k+0.5);
	}
	
	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			brit = LUT[ GetImg( x, y ) ];
			SetImg( x, y, brit );
		}
	}
	return true;
}

bool CImageCV2::Normalization( double targetAvg, double targetSig )
{
	int W = GetImgW();
	int H = GetImgH();

	int x, y;
	int b;
	double c;
	
	double oldAvg, oldSig;
	this->GetAvg( oldAvg, oldSig, true );

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			b = GetImg(x,y);
			c = (b-oldAvg) / oldSig * targetSig + targetAvg;

			b = (int)(c+0.5);

			if (b<0) b = 0;
			if (b> 255) b = 255;
			
			this->SetImg(x,y,b);
		}
	}

	return true;
}

bool CImageCV2::Normalization16( double targetAvg, double targetSig, bool NonZero )
{
	int W = GetImgW();
	int H = GetImgH();

	int x, y;
	int b;
	double c;
	
	double oldAvg, oldSig;
	this->GetAvg( oldAvg, oldSig, NonZero );

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			b = this->GetImg(x,y);

			if (b==0 && NonZero) {
				c = 0.0;
			}
			else {
				c = (b-oldAvg) / oldSig * targetSig + targetAvg;
			}

			b = (int)(c+0.5);

			//if (b<0) b = 0;
			//if (b> 255) b = 255;
			
			this->SetImg(x,y,b);
		}
	}

	return true;
}

bool CImageCV2::Normalize32Fto8U( float OldMin, float OldMax, float NewMin, float NewMax, float Irregular )
{
	int W = GetImgW();
	int H = GetImgH();

	int x, y;
	float b, c;
	
	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			b = m_Img.at<float>(y,x);

			if( b <= Irregular )
			{
				c = 0.0f;
			}
			else
			{
				c = (b-OldMin) * (NewMax - NewMin) / (OldMax-OldMin) + NewMin;
			}

			m_Img.at<float>(y,x) = c;
		}
	}

	return true;
}

int	CImageCV2::GetImg( int x, int y )
{
	if( this->GetChNum() > 1 ) return 0;

	int Depth = this->GetDepth();
	/*
	#define CV_8U   0     //8�r�b�g�A�����Ȃ��f�[�^
	#define CV_8S   1     //8�r�b�g�A�����t���f�[�^
	#define CV_16U  2     //16�r�b�g�A�����Ȃ��f�[�^
	#define CV_16S  3     //16�r�b�g�A�����t���f�[�^
	#define CV_32S  4     //32�r�b�g�A�����t���f�[�^
	#define CV_32F  5     //32�r�b�g�A�P���x�������������_�f�[�^
	#define CV_64F  6     //64�r�b�g�A�{���x�������������_�f�[�^*/

	int ret = 0;

	switch( Depth )
	{
	case 0:
		ret = this->m_Img.at<unsigned char>(y,x);
		break;
	
	case 1:
		ret = this->m_Img.at<signed char>(y,x);
		break;

	case 2:
		ret = this->m_Img.at<unsigned short>(y,x);
		break;
	
	case 3:
		ret = this->m_Img.at<short>(y,x);
		break;
	
	case 4:
		ret = this->m_Img.at<int>(y,x);
		break;
	
	/*case 5:
		ret = this->m_Img.at<float>(y,x);
		break;
	
	case 6:
		ret = this->m_Img.at<double>(y,x);
		break;*/
	
	default:
		cout << "GetImg error.\n" << endl;
		break;
	};

	//int ret = this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() ];
	//int ret = this->m_Img.at<unsigned short>(y,x);
	return ret;
}

double CImageCV2::GetImgD( int x, int y )
{
	if( this->GetChNum() > 1 ) return 0;

	int Depth = this->GetDepth();
	/*
	#define CV_8U   0     //8�r�b�g�A�����Ȃ��f�[�^
	#define CV_8S   1     //8�r�b�g�A�����t���f�[�^
	#define CV_16U  2     //16�r�b�g�A�����Ȃ��f�[�^
	#define CV_16S  3     //16�r�b�g�A�����t���f�[�^
	#define CV_32S  4     //32�r�b�g�A�����t���f�[�^
	#define CV_32F  5     //32�r�b�g�A�P���x�������������_�f�[�^
	#define CV_64F  6     //64�r�b�g�A�{���x�������������_�f�[�^*/

	double ret = 0.;

	switch( Depth )
	{
	case 0:
		ret = (double)this->m_Img.at<unsigned char>(y,x);
		break;
	
	case 1:
		ret = (double)this->m_Img.at<signed char>(y,x);
		break;

	case 2:
		ret = (double)this->m_Img.at<unsigned short>(y,x);
		break;
	
	case 3:
		ret = (double)this->m_Img.at<short>(y,x);
		break;
	
	case 4:
		ret = (double)this->m_Img.at<int>(y,x);
		break;
	
	case 5:
		ret = (double)this->m_Img.at<float>(y,x);
		break;
	
	case 6:
		ret = (double)this->m_Img.at<double>(y,x);
		break;
	
	default:
		cout << "GetImgD error.\n" << endl;
		break;
	};

	//int ret = this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() ];
	//int ret = this->m_Img.at<unsigned short>(y,x);
	return ret;
}

bool CImageCV2::GetImg24( int x, int y, int *R, int *G, int *B )
{
	if( this->GetChNum() != 3 ) return 0;
		
	*B = this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() +0 ];
	*G = this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() +1 ];
	*R = this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() +2 ];

	return true;
}

void CImageCV2::SetImg( int x, int y, int brit )
{
	if( this->GetChNum() > 1 ) return;
	
	//this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() ] = brit;
	
	int Depth = this->GetDepth();
	/*
	#define CV_8U   0     //8�r�b�g�A�����Ȃ��f�[�^
	#define CV_8S   1     //8�r�b�g�A�����t���f�[�^
	#define CV_16U  2     //16�r�b�g�A�����Ȃ��f�[�^
	#define CV_16S  3     //16�r�b�g�A�����t���f�[�^
	#define CV_32S  4     //32�r�b�g�A�����t���f�[�^
	#define CV_32F  5     //32�r�b�g�A�P���x�������������_�f�[�^
	#define CV_64F  6     //64�r�b�g�A�{���x�������������_�f�[�^*/
	if( Depth >= 4 ) return;
	
	switch( Depth )
	{
	case 0:
		this->m_Img.at<unsigned char>(y,x) = (unsigned char)brit;
		break;
	
	case 1:
		this->m_Img.at<signed char>(y,x) = (signed char)brit;
		break;

	case 2:
		this->m_Img.at<unsigned short>(y,x) = (unsigned short)brit;
		break;
	
	case 3:
		this->m_Img.at<short>(y,x) = (short)brit;
		break;
	
	case 4:
		this->m_Img.at<int>(y,x) = brit;
		break;
	
	default:
		cout << "SetImg error.\n" << endl;
		break;
	};
}

void CImageCV2::SetImgD( int x, int y, double brit )
{
	if( this->GetChNum() > 1 ) return;
	
	//this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() ] = brit;
	
	int Depth = this->GetDepth();
	/*
	#define CV_8U   0     //8�r�b�g�A�����Ȃ��f�[�^
	#define CV_8S   1     //8�r�b�g�A�����t���f�[�^
	#define CV_16U  2     //16�r�b�g�A�����Ȃ��f�[�^
	#define CV_16S  3     //16�r�b�g�A�����t���f�[�^
	#define CV_32S  4     //32�r�b�g�A�����t���f�[�^
	#define CV_32F  5     //32�r�b�g�A�P���x�������������_�f�[�^
	#define CV_64F  6     //64�r�b�g�A�{���x�������������_�f�[�^*/
	if( Depth > 6 ) return;
	
	switch( Depth )
	{
	case 0:
		this->m_Img.at<unsigned char>(y,x) = (unsigned char)brit;
		break;
	
	case 1:
		this->m_Img.at<signed char>(y,x) = (signed char)brit;
		break;

	case 2:
		this->m_Img.at<unsigned short>(y,x) = (unsigned short)brit;
		break;
	
	case 3:
		this->m_Img.at<short>(y,x) = (short)brit;
		break;
	
	case 4:
		this->m_Img.at<int>(y,x) = (int)brit;
		break;
		
	case 5:
		this->m_Img.at<float>(y,x) = (float)brit;
		break;
		
	case 6:
		this->m_Img.at<double>(y,x) = brit;
		break;
	
	default:
		cout << "SetImgD error.\n" << endl;
		break;
	};
}

void CImageCV2::SetImg( int x, int y, int R, int G, int B )
{
	if( this->GetChNum() == 1 ) return;

	this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() +0 ] = B;
	this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() +1 ] = G;
	this->m_Img.data[ y * this->m_Img.step + x * this->m_Img.elemSize() +2 ] = R;
}

bool CImageCV2::Create8( int Width, int Height )
{
	m_Img = Mat::zeros( Height, Width, CV_8UC1 );

	return true;
}

bool CImageCV2::Create16( int Width, int Height )
{
	m_Img = Mat::zeros( Height, Width, CV_16UC1 );

	return true;
}

bool CImageCV2::Create24( int Width, int Height )
{
	m_Img = Mat::zeros( Height, Width, CV_8UC3 );

	return true;
}

bool CImageCV2::Create32F( int Width, int Height )
{
	// float
	m_Img = Mat::zeros( Height, Width, CV_32F );

	return true;
}

bool CImageCV2::Create64F( int Width, int Height )
{
	// double
	m_Img = Mat::zeros( Height, Width, CV_64F );

	return true;
}

bool CImageCV2::CreateClone( CImageCV2 *Src )
{
	this->m_Img = Src->m_Img.clone();

	return true;
}


bool CImageCV2::Convert16to8( CImageCV2 *Dst, bool NonZero )
{
	if( this->GetDepth() != 2 || this->GetChNum() != 1 ) return false;

	int W = this->GetImgW();
	int H = this->GetImgH();

	if( !Dst->Create8( W, H ) ) return false;

	//�ő�l�ƍŏ��l�����߂�
	double Min, Max;
	Point MinPos, MaxPos;
	minMaxLoc( this->m_Img, &Min, &Max, &MinPos, &MaxPos );
	
	if (NonZero) {
		//�[�����������ŏ��l

		int brit;
		int iMin = -1;
		int x,y;
		for (y=0;y<H;y++) {
			for (x=0;x<W;x++) {

				brit = this->GetImg(x,y);
				if (brit==0) continue;

				if (iMin<0) {
					iMin = brit;
				} else {
					if (iMin > brit) {
						iMin = brit;
					}
				}
			}
		}

		Min = (double)iMin;
	}


	int x, y;
	int buf;
	double a;
	
	if (Max==Min) {
		
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				Dst->SetImg( x, y, 0 );
			}
		}
	}
	else {
		a = 255.0 / (Max-Min);
		
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				buf = (int)( a * ( this->GetImg(x,y) - Min ) + 0.5 );

				if (buf>255) buf = 255;
				else if(buf<0) buf = 0;	//NonZero�Ή��̂��ߕK�v�ƂȂ���

				Dst->SetImg( x, y, buf );
			}
		}
	}

	return true;
}

bool CImageCV2::Convert16to8( CImageCV2 *Dst, int oldMin, int oldMax )
{
	if( this->GetDepth() != 2 || this->GetChNum() != 1 ) return false;

	int W = this->GetImgW();
	int H = this->GetImgH();

	if( !Dst->Create8( W, H ) ) return false;

	//�ő�l�ƍŏ��l�����߂�
	double Min = (double)oldMin;
	double Max = (double)oldMax;
	
	int x, y;
	int buf;
	
	double a = 255.0 / (Max-Min);

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			buf = (int)( a * ( this->GetImg(x,y) - Min ) + 0.5 );

			if (buf>255) buf = 255;
			else if(buf<0) buf = 0;	//NonZero�Ή��̂��ߕK�v�ƂȂ���

			Dst->SetImg( x, y, buf );
		}
	}

	return true;
}


bool CImageCV2::Convert16toRGB( CImageCV2 *Dst, bool NonZero )
{
	if( this->GetDepth() != 2 || this->GetChNum() != 1 ) return false;

	int W = this->GetImgW();
	int H = this->GetImgH();

	//�܂��́A8�r�b�g��
	CImageCV2 buf;
	if( !buf.Create8( W, H ) ) return false;

	//�ő�l�ƍŏ��l�����߂�
	double Min, Max;
	Point MinPos, MaxPos;
	minMaxLoc( this->m_Img, &Min, &Max, &MinPos, &MaxPos );
	
	if (NonZero) {
		//�[�����������ŏ��l

		int brit;
		int iMin = -1;
		int x,y;
		for (y=0;y<H;y++) {
			for (x=0;x<W;x++) {

				brit = this->GetImg(x,y);
				if (brit==0) continue;

				if (iMin<0) {
					iMin = brit;
				} else {
					if (iMin > brit) {
						iMin = brit;
					}
				}
			}
		}

		Min = (double)iMin;
	}


	int x, y;
	int b;
	
	double a = 255.0 / (Max-Min);

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			b = (int)( a * ( this->GetImg(x,y) - Min ) + 0.5 );

			if (b>255)   b = 255;
			else if(b<0) b =   0;	//NonZero�Ή��̂��ߕK�v�ƂȂ���

			buf.SetImg( x, y, b );
		}
	}

	//24bit�ɃR���o�[�g
	if (!buf.ConvertGray2Rgb( Dst )) return false;

	return true;
}

bool CImageCV2::Convert8( vector<vector<double> > &src )
{
	int W = (int)src[0].size();
	int H = (int)src.size();

	if( !this->Create8( W, H ) ) return false;

	//�ő�l�ƍŏ��l�����߂�
	double Min, Max;
	bool init = true;	
	int x, y;
	int buf;
	
	for (y=0; y<H; y++) {
		for( x=0; x<W; x++) {
			if (init) {
				init = false;
				Min = Max = src[y][x];
			}
			else {
				if (Min>src[y][x]) Min = src[y][x];
				if (Max<src[y][x]) Max = src[y][x];
			}
		}
	}

	double a = 255.0 / (Max-Min);

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			buf = (int)( a * ( src[y][x] - Min ) + 0.5 );

			if (buf>255) buf = 255;
			else if(buf<0) buf = 0;	//NonZero�Ή��̂��ߕK�v�ƂȂ���

			this->SetImg( x, y, buf );
		}
	}

	return true;
}

bool CImageCV2::CutSquare( CImageCV2 *Dst, int cx, int cy, int CutR )
{
	int d = this->GetDepth();
	int c = this->GetChNum();

	if( this->GetDepth() != 0 || this->GetChNum() != 1 ) return false;

	int CutW = CutR*2+1;

	if( !Dst->Create8(CutW,CutW) ) return false;

	int x,y;
	int x2, y2;
	int W = this->GetImgW();
	int H = this->GetImgH();

	for( y2 = 0, y = cy-CutR; y <= cy+CutR; y++, y2++ )
	{
		if( y < 0 || y >= H ) continue;

		for( x2 = 0, x = cx-CutR; x <= cx+CutR; x++, x2++ )
		{
			if( x < 0 || x >= W ) continue;

			int value = this->GetImg(x,y);
			Dst->SetImg( x2, y2, value );
		}
	}

	return true;
}

bool CImageCV2::Cut( CImageCV2 *Dst, int sx, int sy, int CutW, int CutH )
{
	//if( this->GetDepth() != 0 || this->GetChNum() != 1 ) return false;
	if( this->GetChNum() != 1 ) return false;

	if (this->GetDepth() == 2) return Cut16(Dst, sx, sy, CutW, CutH);
	
	//Dst��this�������ꍇ�́A�������Ȏ��Ɂc
	//if( !Dst->Create8(CutW,CutH) ) return false;
	CImageCV2 buf;
	if (!buf.Create8(CutW, CutH) ) return false;

	int x,y;
	int x2, y2;
	int W = this->GetImgW();
	int H = this->GetImgH();

	for( y2 = 0, y = sy; y < sy+CutH; y++, y2++ )
	{
		if( y < 0 || y >= H ) continue;

		for( x2 = 0, x = sx; x < sx+CutW; x++, x2++ )
		{
			if( x < 0 || x >= W ) continue;

			int value = this->GetImg(x,y);
			//Dst->SetImg( x2, y2, value );
			buf.SetImg( x2, y2, value );
		}
	}
	
	if (!Dst->CreateClone( &buf )) return false;

	return true;
}

bool CImageCV2::Cut24( CImageCV2 *Dst, int sx, int sy, int CutW, int CutH )
{
	//if( this->GetDepth() != 0 || this->GetChNum() != 1 ) return false;
	if( this->GetChNum() != 3 ) return false;

	if (this->GetDepth() == 2) return false;//Cut16(Dst, sx, sy, CutW, CutH);
	
	//Dst��this�������ꍇ�́A�������Ȏ��Ɂc
	//if( !Dst->Create8(CutW,CutH) ) return false;
	CImageCV2 buf;
	if (!buf.Create24(CutW, CutH) ) return false;

	int x,y;
	int x2, y2;
	int W = this->GetImgW();
	int H = this->GetImgH();
	int R,G,B;

	for( y2 = 0, y = sy; y < sy+CutH; y++, y2++ )
	{
		if( y < 0 || y >= H ) continue;

		for( x2 = 0, x = sx; x < sx+CutW; x++, x2++ )
		{
			if( x < 0 || x >= W ) continue;

			this->GetImg24(x,y,&R,&G,&B);
			buf.SetImg( x2, y2, R, G, B );
		}
	}
	
	if (!Dst->CreateClone( &buf )) return false;

	return true;
}

bool CImageCV2::Cut16( CImageCV2 *Dst, int sx, int sy, int CutW, int CutH )
{
	if (this->GetChNum() != 1) return false;
	if (this->GetDepth() != 2) return false;	

	//Dst��this�������ꍇ�́A�������Ȏ��Ɂc
	//if( !Dst->Create16(CutW,CutH) ) return false;
	CImageCV2 buf;
	if (!buf.Create16(CutW, CutH) ) return false;

	int x,y;
	int x2, y2;
	int W = this->GetImgW();
	int H = this->GetImgH();

	for( y2 = 0, y = sy; y < sy+CutH; y++, y2++ )
	{
		if( y < 0 || y >= H ) continue;

		for( x2 = 0, x = sx; x < sx+CutW; x++, x2++ )
		{
			if( x < 0 || x >= W ) continue;

			int value = this->GetImg(x,y);
			//Dst->SetImg( x2, y2, value );
			buf.SetImg( x2, y2, value );
		}
	}

	if (!Dst->CreateClone( &buf )) return false;

	return true;
}

bool CImageCV2::placeCenter( CImageCV2 &src )
{
	//this��src���ڂ��܂�

	if (src.GetImgW() == this->GetImgW() && src.GetImgH() == this->GetImgH()) {
		this->CreateClone( &src );

	}
	else if (src.GetImgW() < this->GetImgW() && src.GetImgH() < this->GetImgH()) {
		
		int sx = (this->GetImgW()-src.GetImgW())/2;
		int sy = (this->GetImgH()-src.GetImgH())/2;
		if (!this->SetPartialImg( &src, sx, sy )) return false;
	}
	
	else if (src.GetImgW() > this->GetImgW() && src.GetImgH() > this->GetImgH()) {
		//src�J�b�g���Ē����ɍڂ��܂�
		int sx = (src.GetImgW() - this->GetImgW())/2;
		int sy = (src.GetImgH() - this->GetImgH())/2;
		int CutW = this->GetImgW();
		int CutH = this->GetImgH();
		if (!src.Cut( this, sx, sy, CutW, CutH )) return false;
	}
	
	else {
		cout << "�����Ή��Ƃ��܂��B�o���Ȃ��Ȃ����ǁc" << endl;
		return false;
	}

	return true;
}

bool CImageCV2::SetPartialImg( CImageCV2 *Partial, int sx, int sy )
{
	int W = this->GetImgW();
	int H = this->GetImgH();

	int x, y;
	int px, py;

	for( py = 0, y = sy; y < sy + Partial->GetImgH(); y++, py++ )
	{
		if( y < 0 || y >= H ) continue;

		for( px = 0, x = sx; x < sx + Partial->GetImgW(); x++, px++ )
		{
			if( x < 0 || x >= W ) continue;
			
			this->SetImg( x,y, Partial->GetImg(px,py) );
		}
	}

	return true;
}

bool CImageCV2::SetPartialImg24( CImageCV2 *Partial, int sx, int sy, bool ThroughZero )
{
	int W = this->GetImgW();
	int H = this->GetImgH();
	
	if( this->GetChNum() != 3 || this->GetDepth() != 0 ) return false;

	int x, y;
	int px, py;

	for( py = 0, y = sy; y < sy + Partial->GetImgH(); y++, py++ )
	{
		if( y < 0 || y >= H ) continue;

		for( px = 0, x = sx; x < sx + Partial->GetImgW(); x++, px++ )
		{
			if( x < 0 || x >= W ) continue;

			int R,G,B;
			Partial->GetImg24(px,py,&R,&G,&B);

			if( R+B+G == 0 ) continue;

			this->SetImg( x,y,R,G,B );
		}
	}

	return true;
}

bool CImageCV2::SetPartialImgV2( CImageCV2 *Partial, int sx, int sy, int Ry1, int Ry2 )
{
	int W = this->GetImgW();
	int H = this->GetImgH();

	int x, y;
	int px, py;

	for( py = 0, y = sy; y < sy + Partial->GetImgH(); y++, py++ )
	{
		if( y < 0 || y >= H ) continue;

		if( py < Ry1 || Ry2 < py ) continue;

		for( px = 0, x = sx; x < sx + Partial->GetImgW(); x++, px++ )
		{
			if( x < 0 || x >= W ) continue;

			this->SetImg( x,y, Partial->GetImg(px,py) );
		}
	}

	return true;
}

bool CImageCV2::PasteTriangle( CImageCV2 &Src, Point2f *TriPt )
{
	//Src����O�p�`���J�b�g���āAThis.m_Img�ɓ\��t����

	//BoundingBox
	int MinX, MinY, MaxX, MaxY;

	MinX = MaxX = TriPt[0].x;
	MinY = MaxY = TriPt[0].y;

	for (int i=0; i<3; i++) {

		if (MinX>TriPt[i].x) MinX = TriPt[i].x;
		if (MaxX<TriPt[i].x) MaxX = TriPt[i].x;
		
		if (MinY>TriPt[i].y) MinY = TriPt[i].y;
		if (MaxY<TriPt[i].y) MaxY = TriPt[i].y;
	}
	
	if (MinX<0) MinX = 0;
	if (MaxX>=GetImgW()) MaxX = GetImgW()-1;
	
	if (MinY<0) MinY = 0;
	if (MaxY>=GetImgH()) MaxY = GetImgH()-1;

	int x, y;

	for (y=MinY; y<=MaxY; y++) {
		for (x=MinX; x<=MaxX; x++) {

			if (InTriangle(x,y, TriPt)) {
				SetImg(x,y,Src.GetImg(x,y));
			}
		}
	}

	return true;
}

bool CImageCV2::paintTriangle( vector<Point2f> &TriPt, int R, int G, int B )
{
	//This.m_Img�Ɏw��̎O�p�`�ɐF��h��
	
	//BoundingBox
	int MinX, MinY, MaxX, MaxY;

	MinX = MaxX = TriPt[0].x;
	MinY = MaxY = TriPt[0].y;

	for (int i=0; i<(int)TriPt.size(); i++) {

		if (MinX>TriPt[i].x) MinX = TriPt[i].x;
		if (MaxX<TriPt[i].x) MaxX = TriPt[i].x;
		
		if (MinY>TriPt[i].y) MinY = TriPt[i].y;
		if (MaxY<TriPt[i].y) MaxY = TriPt[i].y;
	}
	
	if (MinX<0) MinX = 0;
	if (MaxX>=GetImgW()) MaxX = GetImgW()-1;
	
	if (MinY<0) MinY = 0;
	if (MaxY>=GetImgH()) MaxY = GetImgH()-1;

	int x, y;

	for (y=MinY; y<=MaxY; y++) {
		for (x=MinX; x<=MaxX; x++) {

			if (InTriangle(x,y, TriPt)) {
				this->SetImg(x,y,R,G,B);
			}
		}
	}

	return true;
}

bool CImageCV2::InTriangle( int x, int y, Point2f *TriPt )
{
	float vx0, vy0, vx1, vy1, z;

	vx0 = TriPt[1].x - TriPt[0].x;
	vy0 = TriPt[1].y - TriPt[0].y;
	vx1 = (float)x   - TriPt[1].x;
	vy1 = (float)y   - TriPt[1].y;

	//�O�ς̂�������
	z = vx0 * vy1 - vx1 * vy0;
	if (z < 0) return false;

	vx0 = TriPt[2].x - TriPt[1].x;
	vy0 = TriPt[2].y - TriPt[1].y;
	vx1 = (float)x   - TriPt[2].x;
	vy1 = (float)y   - TriPt[2].y;

	//�O�ς̂�������
	z = vx0 * vy1 - vx1 * vy0;
	if (z < 0) return false;

	
	vx0 = TriPt[0].x - TriPt[2].x;
	vy0 = TriPt[0].y - TriPt[2].y;
	vx1 = (float)x   - TriPt[0].x;
	vy1 = (float)y   - TriPt[0].y;

	//�O�ς̂�������
	z = vx0 * vy1 - vx1 * vy0;
	if (z < 0) return false;

	return true;
}

bool CImageCV2::InTriangle( int x, int y, vector<Point2f> &TriPt )
{
	float vx0, vy0, vx1, vy1, z;

	vx0 = TriPt[1].x - TriPt[0].x;
	vy0 = TriPt[1].y - TriPt[0].y;
	vx1 = (float)x   - TriPt[1].x;
	vy1 = (float)y   - TriPt[1].y;

	//�O�ς̂�������
	z = vx0 * vy1 - vx1 * vy0;
	if (z < 0) return false;

	vx0 = TriPt[2].x - TriPt[1].x;
	vy0 = TriPt[2].y - TriPt[1].y;
	vx1 = (float)x   - TriPt[2].x;
	vy1 = (float)y   - TriPt[2].y;

	//�O�ς̂�������
	z = vx0 * vy1 - vx1 * vy0;
	if (z < 0) return false;

	
	vx0 = TriPt[0].x - TriPt[2].x;
	vy0 = TriPt[0].y - TriPt[2].y;
	vx1 = (float)x   - TriPt[0].x;
	vy1 = (float)y   - TriPt[0].y;

	//�O�ς̂�������
	z = vx0 * vy1 - vx1 * vy0;
	if (z < 0) return false;

	return true;
}

bool CImageCV2::GetAvg( double &Avg, double &sig, bool NonZero )
{
	int x,y;
	int brit, cnt=0;
	double sm = 0., sq = 0.;

	if (NonZero) {

		for (y=0; y<GetImgH(); y++) {
			for (x=0; x<GetImgW(); x++) {

				brit = GetImg(x,y);
				if (brit <= 0) continue;

				sm += (double)brit;
				sq += (double)(brit*brit);
				cnt++;
			}
		}

	} else {
		
		for (y=0; y<GetImgH(); y++) {
			for (x=0; x<GetImgW(); x++) {

				brit = GetImg(x,y);

				sm += (double)brit;
				sq += (double)(brit*brit);
				cnt++;
			}
		}

	}

	if (cnt<=0) {
		Avg = sig = 0.0;	
		return false;
	}

	Avg = sm / (double)cnt;
	sig = sqrt( sq / (double)cnt - Avg*Avg);

	cout << "Avg = " << Avg << ", Sig = " << sig << endl;

	return true;
}

bool CImageCV2::GetAvgMinMax( double &Avg, double &sig, int &min, int &max, bool NonZero )
{
	int x,y;
	int brit, cnt=0;
	double sm = 0., sq = 0.;
	min = 0;
	max = 0;
	bool init = true;

	if (NonZero) {

		for (y=0; y<GetImgH(); y++) {
			for (x=0; x<GetImgW(); x++) {

				brit = GetImg(x,y);
				if (brit <= 0) continue;

				if (init) {
					init = false;
					min = max = brit;
				}
				else {
					if (min>brit) min = brit;
					else if (max<brit) max = brit;
				}

				sm += (double)brit;
				sq += (double)(brit*brit);
				cnt++;
			}
		}

	} else {
		
		for (y=0; y<GetImgH(); y++) {
			for (x=0; x<GetImgW(); x++) {

				brit = GetImg(x,y);
				
				if (init) {
					init = false;
					min = max = brit;
				}
				else {
					if (min>brit) min = brit;
					else if (max<brit) max = brit;
				}

				sm += (double)brit;
				sq += (double)(brit*brit);
				cnt++;
			}
		}

	}

	if (cnt<=0) return false;

	Avg = sm / (double)cnt;
	sig = sqrt( sq / (double)cnt - Avg*Avg);

	return true;
}

bool CImageCV2::AdjustBrit( double avg0, double sig0, double avgT, double sigT )
{
	int x,y;
	int brit;

	for (y=0; y<GetImgH(); y++) {
		for (x=0; x<GetImgW(); x++) {

			brit = GetImg(x,y);			
			if (brit <= 0) continue;

			double bb = sigT / sig0 * ( brit - avg0 ) + avgT;

			brit = (int)( bb+0.5);

			if (brit<=0) brit = 0;
			else if (brit>255) brit = 255;
		
			SetImg(x,y,brit);
		}
	}

	return true;
}

bool CImageCV2::LoadRaw( string fileName, int ImgW, int ImgH, bool isGray )
{
	ifstream ifs(fileName, ios::binary);
  
    if(ifs.fail()){
        cout << "File NOT FOUND!!" << endl;
        return false;
    }
  
    streamsize size = ifs.seekg(0, ios::end).tellg();
    ifs.seekg(0, ios::beg);
  
	int imgType = -1;
	
    if(size == ImgW * ImgH +1) imgType = 1;
	else if (size == ImgW * ImgH) imgType = 0;

    //if(size != ImgW * ImgH +1){
	if (imgType < 0) {
        cout << "Picture size is Inaccurate!!" << endl;
        return false;
    }
  
	CImageCV2 buf;
	buf.Create8( ImgW, ImgH );

	ifs.read(reinterpret_cast<char*>(buf.m_Img.data), ImgW * ImgH); //�摜����C�ɓǂݍ���
    ifs.close();

	if (isGray) {
		this->CreateClone( &buf );
	}
	else {
		buf.ConvertGray2Rgb( this );
	}

	return true;
}

bool CImageCV2::LoadCsv( string fileName, int ImgW, int ImgH, bool isGray )
{
	ifstream ifs(fileName, ios::binary);
  
    if(ifs.fail()){
        cout << "File NOT FOUND!!" << endl;
        return false;
	}

	std::string str;
	int brit;
	int pos = 0;
	int x, y;

	if (isGray)
		this->Create8(ImgW, ImgH);
	else
		this->Create24(ImgW, ImgH);

	while(getline(ifs,str))
	{
		std::string tmp;
		std::istringstream stream(str);
		while ( getline(stream,tmp,','))
		{
			//std::cout<< tmp << std::endl;

			std::stringstream ss;
			ss << tmp;
			ss >> brit;

			x = pos % ImgW;
			y = pos / ImgW;

			if (isGray)
				this->SetImg(x,y,brit);
			else
				this->SetImg(x,y,brit,brit,brit);

			pos++;
		}
	}

	return true;
}

bool CImageCV2::transPolar( cv::Point2i center, double res, CImageCV2 &dst, int rad )
{
	//int rad = 200;

	int iW = this->GetImgW();
	int iH = this->GetImgH();

	int x, y;
	int W = 360;
	int H = rad;

	if (!dst.Create8(W,H)) return false;

	double r, t;
	double pi = acos( -1.0 );
	double dW = (double)W;

	double dx, dy;
	double N2 = dW * 0.5;
	
	//���S���W
	int px = center.x;
	int py = center.y;
	int ix, iy;

	for( y = 0; y < H; y++ )
	{
		for( x = -180; x < 180; x++ )
		{
			
			t = (double)x / 180.0 * pi;

			dx = y * cos( t );
			dy = y * sin( t );
						
			//���S�ɂ��������W�𑫂�
			dx += px;
			dy += py;

			if( dx > 0 )	ix = (int)dx;
			else			ix = (int)(dx-1.0);
			
			if( dy > 0 )	iy = (int)dy;
			else			iy = (int)(dy-1.0);

			if( ix >= 0 && ix <= iW-2 && iy >= 0 && iy <= iH-2 )
			{
				double sx = dx - ix;
				double sy = dy - iy;

				/*int add = ix + iy * W;
				double d1 = Amp->Img[ add ];
				double d2 = Amp->Img[ add + 1 ];
				double d3 = Amp->Img[ add + W ];
				double d4 = Amp->Img[ add + W + 1 ];*/

				double d1 = this->GetImg(ix,  iy  );
				double d2 = this->GetImg(ix+1,iy  );
				double d3 = this->GetImg(ix,  iy+1);
				double d4 = this->GetImg(ix+1,iy+1);

				//���`��Ԃł��B
				//PImg[ t + r*pt ] = ( (1.0-sy)*( (1.0-sx)*d1 + sx*d2 ) + sy*( (1.0-sx)*d3 + sx*d4 ) );
				//dst->Img[ x + y*W ] = d1 + sy*(d3-d1) + sx*( (d2-d1) + sy*(d1-d2-d3+d4) );
				int brit =  (int)(d1 + sy*(d3-d1) + sx*( (d2-d1) + sy*(d1-d2-d3+d4) ) + 0.5);
				dst.SetImg(x+180,y,brit);
			}
			else
			{
				//�O�������獕
				//Dst->Img[ x + y*W ] = 0.0;
				dst.SetImg(x+180,y,0);
			}
		}
	}

	return true;
}

bool CImageCV2::transPolar2( cv::Point2i center, double res, CImageCV2 &dst, int rad )
{
	int iW = this->GetImgW();
	int iH = this->GetImgH();
	
	int x, y;
	int W = 360;
	int H = rad;

	//�p�x���W�̃����W �O�`
	W = (int)(360. / res + 0.5);

	if (!dst.Create8(W,H)) return false;

	double r, t;
	double pi = acos( -1.0 );
	double dW = (double)W;

	double dx, dy;
	double N2 = dW * 0.5;
	
	//���S���W
	int px = center.x;
	int py = center.y;
	int ix, iy;
	double initAngle = -180.0;
	double angle;

	for( y = 0; y < H; y++ )
	{
		angle = initAngle;
		
		//for( x = -180; x < 180; x++ )
		for( x = -W/2; x < W/2; x++ )
		{
		//for( x = 0; x < W; x++, angle+res)
		//{
		//	t = (double)x / 180.0 * pi;

			t = (x * res) / 180.0 * pi;

			//t = (double)angle / 180.0 * pi;

			dx = y * cos( t );
			dy = y * sin( t );
						
			//���S�ɂ��������W�𑫂�
			dx += px;
			dy += py;

			if( dx > 0 )	ix = (int)dx;
			else			ix = (int)(dx-1.0);
			
			if( dy > 0 )	iy = (int)dy;
			else			iy = (int)(dy-1.0);

			if( ix >= 0 && ix <= iW-2 && iy >= 0 && iy <= iH-2 )
			{
				double sx = dx - ix;
				double sy = dy - iy;
				
				double d1 = this->GetImg(ix,  iy  );
				double d2 = this->GetImg(ix+1,iy  );
				double d3 = this->GetImg(ix,  iy+1);
				double d4 = this->GetImg(ix+1,iy+1);

				//���`��Ԃł��B
				int brit =  (int)(d1 + sy*(d3-d1) + sx*( (d2-d1) + sy*(d1-d2-d3+d4) ) + 0.5);
				dst.SetImg(x+W/2,y,brit);
			}
			else
			{
				//�O�������獕
				dst.SetImg(x+W/2,y,0);
			}
		}
	}

	return true;
}

void CImageCV2::clear()
{
	int w = this->GetImgW();
	int h = this->GetImgH();
	int Depth = this->GetDepth();

	if( this->GetChNum() == 3 ) {

		if (Depth==0) {
			m_Img = Mat::zeros( h, w, CV_8UC3 );
		}
		else {
			cout << "�ł��Ȃ���" << endl;
			return;
		}
	}
	else if (this->GetChNum() == 1) {

		/*
		#define CV_8U   0     //8�r�b�g�A�����Ȃ��f�[�^
		#define CV_8S   1     //8�r�b�g�A�����t���f�[�^
		#define CV_16U  2     //16�r�b�g�A�����Ȃ��f�[�^
		#define CV_16S  3     //16�r�b�g�A�����t���f�[�^
		#define CV_32S  4     //32�r�b�g�A�����t���f�[�^
		#define CV_32F  5     //32�r�b�g�A�P���x�������������_�f�[�^
		#define CV_64F  6     //64�r�b�g�A�{���x�������������_�f�[�^*/
		if( Depth > 6 ) return;

		switch( Depth )
		{
		case 0:
			m_Img = Mat::zeros( h, w, CV_8UC1 );
			break;

		case 1:
			m_Img = Mat::zeros( h, w, CV_8SC1 );
			break;

		case 2:
			m_Img = Mat::zeros( h, w, CV_16UC1 );
			break;

		case 3:
			m_Img = Mat::zeros( h, w, CV_16SC1 );
			break;

		case 4:
			m_Img = Mat::zeros( h, w, CV_32SC1 );
			break;

		case 5:
			m_Img = Mat::zeros( h, w, CV_32FC1 );
			break;

		case 6:
			m_Img = Mat::zeros( h, w, CV_64FC1 );
			break;

		default:
			cout << "�ł��Ȃ���" << endl;
			break;
		};
	}
	else {
		cout << "�ł��Ȃ���" << endl;
		return;
	}
}

#if 0

CImageCV2::CImageCV2( const CImageCV2 &res )
{
	//�R�s�[�R���X�g���N�^
	
	m_Img = NULL;
	m_iW = res.m_iW;
	m_iH = res.m_iH;
	m_iCh = res.m_iCh;
	m_iDepth = res.m_iDepth;

	m_bOpenShowWindow = false;

	if( res.m_Img != NULL )
	{
		m_Img = cvCloneImage( res.m_Img );	
	}
	else
	{
		m_Img = NULL;
	}
}

CImageCV2& CImageCV2::operator=( const CImageCV2 &res )
{
	//@@
	ReleaseMem();

	m_Img = NULL;
	m_iW = res.m_iW;
	m_iH = res.m_iH;
	m_iCh = res.m_iCh;
	m_iDepth = res.m_iDepth;
	
	m_bOpenShowWindow = false;

	if( res.m_Img != NULL )
	{
		m_Img = cvCloneImage( res.m_Img );	
	}
	else
	{
		m_Img = NULL;
	}

	return *this;
}

CImageCV2::~CImageCV2(void)
{
	ReleaseMem();

	if( m_bOpenShowWindow )
	{
		CloseShowWindow();
	}
}

void CImageCV2::ReleaseMem()
{
	if( m_Img != NULL )
	{
		cvReleaseImage( &m_Img );
	}
}

bool CImageCV2::Load8( 
	string fname,	//�t�@�C����
	bool typeCheck	//��ʓI��jpg�ABMP��������s�v�����A���i�łƂ��Ȃ�������������������
){
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	if( typeCheck )
	{
		// load for 16bit
		//m_Img = cvLoadImage( dst, CV_LOAD_IMAGE_ANYDEPTH );
		m_Img = cvLoadImage( dst, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH );
		//m_Img = cvLoadImage( dst, CV_LOAD_IMAGE_COLOR );

		if( m_Img == NULL )
		{
			//retry for 8bit
			//m_Img = cvLoadImage( dst, CV_LOAD_IMAGE_UNCHANGED  );

			//m_Img = IplImage.fromfile.FromFile("lenna.png");

			Mat image;
			image = imread( dst, CV_LOAD_IMAGE_COLOR);   // Read the file

			if( m_Img == NULL )
			{
				delete [] dst;
				return false;
			}
		}

		if( m_Img->depth == 16 && m_Img->nChannels == 1 )
		{
			//�m���}���C�Y����
			//cvReleaseImage( &m_Img );

			CImageCV2 buf;
			if( !buf.LoadTif16( fname ) )
			{
				delete [] dst;
				return false;
			}

			if( !buf.Convert8b( this ) )
			{
				delete [] dst;
				return false;
			}

			delete [] dst;
			return true;
		}

		//��U�J��
		cvReleaseImage( &m_Img );
	}

	//�O���[�Ŏ�蒼��
	m_Img = cvLoadImage( dst, CV_LOAD_IMAGE_GRAYSCALE );
	if( m_Img == NULL )
	{
		delete [] dst;
		return false;
	}

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;

	delete [] dst;

	return true;
}

bool CImageCV2::Load24( string fname )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );
	
	m_Img = cvLoadImage( dst, CV_LOAD_IMAGE_COLOR );

	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;
	
	delete [] dst;


	return true;
}

bool CImageCV2::LoadGch( string fname )
{
	//Green�`�����l����8bit�Ŏ�o��

	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	//�Ƃ肠�����J���[�摜���J��
	CImageCV2 buf;
	if( !buf.Load24( fname ) ) return false;

	//8bit�̃�����
	m_Img = cvCreateImage( cvSize(buf.m_iW, buf.m_iH), IPL_DEPTH_8U, 1 );
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;
	
	int x, y, R, G, B;

	// zero clear
	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			buf.GetImg( x, y, &R, &G, &B );

			SetImg( x, y, G );
		}
	}

	return true;
}

bool CImageCV2::LoadRch( string fname )
{
	//Red�`�����l����8bit�Ŏ�o��

	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	//�Ƃ肠�����J���[�摜���J��
	CImageCV2 buf;
	if( !buf.Load24( fname ) ) return false;

	//8bit�̃�����
	m_Img = cvCreateImage( cvSize(buf.m_iW, buf.m_iH), IPL_DEPTH_8U, 1 );
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;
	
	int x, y, R, G, B;

	// zero clear
	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			buf.GetImg( x, y, &R, &G, &B );

			SetImg( x, y, R );
		}
	}

	return true;
}

bool CImageCV2::LoadBch( string fname )
{
	//Blue�`�����l����8bit�Ŏ�o��

	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	//�Ƃ肠�����J���[�摜���J��
	CImageCV2 buf;
	if( !buf.Load24( fname ) ) return false;

	//8bit�̃�����
	m_Img = cvCreateImage( cvSize(buf.m_iW, buf.m_iH), IPL_DEPTH_8U, 1 );
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;
	
	int x, y, R, G, B;

	// zero clear
	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			buf.GetImg( x, y, &R, &G, &B );

			SetImg( x, y, B );
		}
	}

	return true;
}

bool CImageCV2::LoadTif16( string fname )
{
	//for 16bit Tif Image

	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	m_Img = cvLoadImage( dst, CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_ANYCOLOR );
	delete [] dst;	

	if( m_Img == NULL ) return false;
	if( m_Img->nChannels != 1 && m_Img->depth != 16 ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;

	return true;
}

//BYTE CImageCV2::GetImg( int x, int y )
int CImageCV2::GetImg( int x, int y )
{
	if( this->m_iDepth == 16 )
	{
		//TRACE("GetImg16���g��!!\n");
		return GetImg16( x, y );
		//return 0;
	}

	/*if( x < 0 || x >= m_iW ||
		y < 0 || y >= m_iH )
	{
		return 0;
	}*/

	return m_Img->imageData[ x + y * m_Img->widthStep ];
}

//void CImageCV2::SetImg( int x, int y, BYTE value )
void CImageCV2::SetImg( int x, int y, int value )
{
	if( this->m_iDepth == 16 )
	{
		//TRACE("SetImg16���g��!!\n");
		SetImg16( x, y, value );
		return;
	}

	m_Img->imageData[ x + y * m_Img->widthStep ] = (BYTE)value;
}

WORD CImageCV2::GetImg16( int x, int y )
{
	//int Step = m_Img->widthStep / 2;
	WORD *pBuf = (WORD*)m_Img->imageData;
	return pBuf[ x + y * (m_Img->widthStep / 2) ];
}

void CImageCV2::SetImg16( int x, int y, WORD value )
{
	//�������A�N�Z�X�̂��߂ɃL���X�g
	WORD *pBuf = (WORD*)m_Img->imageData;
	pBuf[ x + y * (m_Img->widthStep / 2) ] = value;
}

bool CImageCV2::Convert8b1c( bool Blending )
{
	if( m_iDepth != 8 ) return false;
	if( m_iDepth == 8 && m_iCh == 1 ) return true;

	//����̂Wb�Rc��buf��
	CImageCV2 buf;
	if( !buf.CreateClone( this ) ) return false;

	//�V����������
	if( !Create8( m_iW, m_iH ) ) return false;

	int x, y;
	int R, G, B;

	if( Blending )
	{
		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				buf.GetImgGray( x, y, &G );
				this->SetImg( x, y, G );
			}
		}
	}
	else
	{
		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				buf.GetImg( x, y, &R, &G, &B );
				this->SetImg( x, y, R );
			}
		}
	}

	return true;
}

bool CImageCV2::Convert24to8( CImageCV2 *Dst, bool BlendingRGB )
{
	if( m_iDepth != 8 || m_iCh != 3 ) return false;

	//����̂Wb�Rc��buf��
	if( !Dst->Create8( m_iW, m_iH ) ) return false;

	int x, y;
	int R, G, B;

	if( BlendingRGB )
	{
		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				GetImgGray( x, y, &G );
				Dst->SetImg( x, y, G );
			}
		}
	}
	else
	{
		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				//�u�����h���Ȃ��ꍇ�́AR�`�����l�����Ԃ�����
				GetImg( x, y, &R, &G, &B );
				Dst->SetImg( x, y, R );
			}
		}
	}

	return true;
}

bool CImageCV2::Convert8b( CImageCV2 *Dst, int TargetMin, int TargetMax )
{
	if( this->m_iDepth == 16 && this->m_iCh == 1 )
	{
		return Convert16to8( Dst, TargetMin, TargetMax );
	}
	else if( this->m_iDepth == 8 && this->m_iCh == 3 )
	{
		TRACE("�ŏ��A�ő�l���Z�b�g���Ȃ���\n");
	}

	return false;

}

bool CImageCV2::Convert16to8( CImageCV2 *Dst, int TargetMin, int TargetMax )
{
	if( this->m_iDepth != 16 || this->m_iCh != 1 ) return false;

	if( !Dst->Create8( m_iW, m_iH ) ) return false;

	int x, y;
	int buf;
	
	double a = 255.0 / (double)( TargetMax - TargetMin );

	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			buf = (int)( a * ( GetImg16(x,y) - TargetMin ) + 0.5 );

			if( buf > 255 ) buf = 255;

			Dst->SetImg( x, y, buf );
		}
	}

	return true;
}

bool CImageCV2::Convert8b( CImageCV2 *Dst )
{
	if( this->m_iDepth == 16 && this->m_iCh == 1 )
	{
		return Convert16to8( Dst );
	}
	else if( this->m_iDepth == 8 && this->m_iCh == 3 )
	{
		return Convert24to8( Dst );
	}

	return false;
}

bool CImageCV2::Convert16to8( CImageCV2 *Dst )
{
	if( this->m_iDepth != 16 || this->m_iCh != 1 ) return false;

	if( !Dst->Create8( m_iW, m_iH ) ) return false;

	WORD *pBuf = (WORD*)m_Img->imageData;
	int x, y;
	int buf;
	
	int Min = -1, Max = -1;
	int brit;

	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			brit = GetImg16( x, y );
			if( brit <= 0 ) continue;
			
			if( Min < 0 )
			{
				Min = Max = brit;
			}
			else
			{
				if( Min > brit ) Min = brit;
				if( Max < brit ) Max = brit;
			}
		}
	}

	double a = 255.0 / (double)( Max - Min );

	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			brit = GetImg16(x,y);
			buf = (int)( a * ( brit - Min ) + 0.5 );

			if( buf > 255 ) buf = 255;

			Dst->SetImg( x, y, buf );
		}
	}

	return true;
}

bool CImageCV2::Convert24( CImageCV2 *Dst )
{
	if( !Dst->Create24( m_iW, m_iH ) ) return false;

	if( m_iDepth == 16 )
	{
		WORD *pBuf = (WORD*)m_Img->imageData;
		int x, y;
		int buf;

		int Min = -1, Max = -1;
		int brit;

		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				brit = GetImg16( x, y );
				if( brit <= 0 ) continue;

				if( Min < 0 )
				{
					Min = Max = brit;
				}
				else
				{
					if( Min > brit ) Min = brit;
					if( Max < brit ) Max = brit;
				}
			}
		}

		double a = 255.0 / (double)( Max - Min );

		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				brit = GetImg16(x,y);
				buf = (int)( a * ( brit - Min ) + 0.5 );

				if( buf > 255 ) buf = 255;

				Dst->SetImg( x, y, buf, buf, buf );
			}
		}
	}
	else if( m_iDepth == 8 )
	{
		int brit;
		int x, y;

		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				brit = GetImg(x,y);

				Dst->SetImg( x, y, brit, brit, brit );
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CImageCV2::Create8( int W, int H )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	m_Img = cvCreateImage( cvSize(W, H), IPL_DEPTH_8U, 1 );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;
	
	// zero clear
	for( int y = 0; y < m_iH; y++ )
	{
		for( int x = 0; x < m_iW; x++ )
		{
			SetImg( x, y, 0 );
		}
	}

	return true;
}

bool CImageCV2::Create16( int W, int H )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	m_Img = cvCreateImage( cvSize(W, H), IPL_DEPTH_16U, 1 );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;
	
	// zero clear
	for( int y = 0; y < m_iH; y++ )
	{
		for( int x = 0; x < m_iW; x++ )
		{
			SetImg( x, y, 0 );
		}
	}

	return true;
}

bool CImageCV2::Create24( int W, int H )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	m_Img = cvCreateImage( cvSize(W, H), IPL_DEPTH_8U, 3 );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;

	return true;
}

bool CImageCV2::Create16( int W, int H )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	m_Img = cvCreateImage( cvSize(W, H), IPL_DEPTH_16U, 1 );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;
	
	// zero clear
	for( int y = 0; y < m_iH; y++ )
	{
		for( int x = 0; x < m_iW; x++ )
		{
			SetImg16( x, y, 0 );
			//SetImg( x, y, 0 );
		}
	}

	return true;
}

bool CImageCV2::CreateClone( CImageCV2 *Src )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	m_Img = (IplImage *)cvClone( Src->m_Img );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;

	return true;
}

bool CImageCV2::CreateClone( CbImage *Src )
{
	if( !Create8( Src->W, Src->H ) ) return false;

	int x, y;

	for( y = 0; y < Src->H; y++ )
	{
		for( x = 0; x < Src->W; x++ )
		{
			SetImg( x, y, Src->GetValue(x,y) );
		}
	}

	return true;
}

bool CImageCV2::CreateClone( CwImage *Src )
{
	if( !Create16( Src->W, Src->H ) ) return false;

	int x, y;

	for( y = 0; y < Src->H; y++ )
	{
		for( x = 0; x < Src->W; x++ )
		{
			SetImg16( x, y, Src->GetValue(x,y) );
		}
	}

	return true;
}

void CImageCV2::Show( char *str )
{
	if( str == "" )
	{
		str = "Image";
	}
	
	m_chrWindowName = str;

	cvNamedWindow( str, CV_WINDOW_AUTOSIZE ); 
	cvShowImage( str, m_Img ); 
	m_bOpenShowWindow = true;

	cvWaitKey( 0 ); 
	cvDestroyWindow( str );
	m_bOpenShowWindow = false;
}

void CImageCV2::CloseShowWindow()
{
	if( !m_bOpenShowWindow ) return;
	
	cvDestroyWindow( m_chrWindowName );
	m_bOpenShowWindow = false;
	m_chrWindowName = "";
}

void CImageCV2::Show( string ShowName, string FolderPath )
{
	if( ShowName == "" )
	{
		ShowName = "Image";
	}

	int len = ShowName.length();
	char* fname = new char[len+1];
	memcpy(fname, ShowName.c_str(), len+1);
	
	m_chrWindowName = fname;

	cvNamedWindow( fname, CV_WINDOW_AUTOSIZE ); 
	cvShowImage( fname, m_Img );
	m_bOpenShowWindow = true;
	
	if( cvWaitKey( 0 ) == 's' )
	{
		string SaveName = FolderPath + "\\" + ShowName + ".png";
	
		if( FolderPath == "" )
		{
			SaveName = ShowName + ".png";
		}

		int len2 = SaveName.length();
		char* fname2 = new char[len2+1];
		memcpy( fname2, SaveName.c_str(), len2+1);

		//�ۑ�����
		cvSaveImage( fname2, m_Img );

		delete fname2;
	}

	cvDestroyWindow( fname );
	m_bOpenShowWindow = false;

	delete fname;
}

void CImageCV2::Show( string ShowName, IplImage *Src )
{
	if( ShowName == "" )
	{
		ShowName = "Image";
	}
	
	int len = ShowName.length();
	char* fname = new char[len+1];
	memcpy(fname, ShowName.c_str(), len+1);
	
	m_chrWindowName = fname;

	cvNamedWindow( fname, CV_WINDOW_AUTOSIZE ); 
	cvShowImage( fname, Src );
	m_bOpenShowWindow = true;

	cvWaitKey( 0 ); 
	cvDestroyWindow( fname );
	m_bOpenShowWindow = false;
	
	delete fname;
}

void CImageCV2::SetValueC( int x, int y, BYTE R, BYTE G, BYTE B )
{
	int step = m_Img->widthStep;

	m_Img->imageData[ x*m_iCh + y * step + 0 ] = B;
	m_Img->imageData[ x*m_iCh + y * step + 1 ] = G;
	m_Img->imageData[ x*m_iCh + y * step + 2 ] = R;
}

bool CImageCV2::Save( string fname )
{
	if( m_Img == NULL ) return false;

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	cvSaveImage( dst, m_Img );

	delete [] dst;

	return true;
}

bool CImageCV2::Save( 
	string	fname,	//�ۑ��t�@�C����
	int		ImgNo,	//�ʂ��ԍ�
	int		digit,	//��
	string	Ext,	//�g���q
	bool	Normalize
){
	//�t�@�C�����̐���
	{
		string strNo;
		stringstream ss;
		ss << ImgNo;
		ss >> strNo;

		int DigCnt = 0;

		do
		{
			ImgNo /= 10;
			DigCnt++;

		}while( ImgNo > 0 );

		string ff;

		if( DigCnt < digit )
		{
			for( int i = 0; i < (digit - DigCnt ); i++ )
			{
				ff += "0";
			}
		}

		ff += strNo;

		fname += ff;
		fname += Ext;
	}

	if( m_Img == NULL ) return false;

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	if( Normalize )
	{
		int Min = 255;
		int Max = 0;
		int brit;

		for( int y = 0; y < m_iH; y++ )
		{
			for( int x = 0; x < m_iW; x++ )
			{
				brit = GetImg(x,y);
				if( brit == 0 ) continue;

				if( Min > brit ) Min = brit;
				if( Max < brit ) Max = brit;
			}
		}

		//�ŏ��l���O�A�ő�l���Q�T�T�ɕϊ�
		double coef = 255. / (double)(Max-Min);
		
		for( int y = 0; y < m_iH; y++ )
		{
			for( int x = 0; x < m_iW; x++ )
			{
				brit = (int)( coef * (GetImg(x,y) - Min ) + 0.5 );
				
				if( brit > 255 ) brit = 255;
				else if( brit < 0 ) brit = 0;

				SetImg(x,y,brit);
			}
		}
	}

	cvSaveImage( dst, m_Img );

	delete [] dst;

	return true;
}

bool CImageCV2::Save( string fname, int sx, int sy, int CutW, int CutH )
{
	if( m_Img == NULL ) return false;

	if( m_iCh == 3 && m_Img->depth == 8 )
	{
		return Save8b3c( fname, sx, sy, CutW, CutH );
	}
	
	if( m_iCh == 1 && m_Img->depth == 16 )
	{
		return Save16to8b( fname, sx, sy, CutW, CutH );
	}

	if( m_iCh != 1 || m_Img->depth != 8 ) return false;

	CImageCV2 Cut;
	if( !Cut.Create8( CutW, CutH ) ) return false;

	int x, y, cx, cy;

	for( y = sy, cy = 0; y < sy+CutH; y++, cy++ )
	{
		for( x = sx, cx = 0; x < sx+CutW; x++, cx++ )
		{
			Cut.SetImg( cx, cy, GetImg( x, y ) );
		}
	}

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	cvSaveImage( dst, Cut.m_Img );

	delete [] dst;

	return true;
}

bool CImageCV2::Save8b3c( string fname, int sx, int sy, int CutW, int CutH )
{
	if( m_Img == NULL ) return false;

	if( m_iCh != 3 && m_Img->depth != 8 )
	{
		return false;
	}

	CImageCV2 Cut;
	if( !Cut.Create24( CutW, CutH ) ) return false;

	int x, y, cx, cy;
	int R, G, B;

	for( y = sy, cy = 0; y < sy+CutH; y++, cy++ )
	{
		for( x = sx, cx = 0; x < sx+CutW; x++, cx++ )
		{
			GetImg( x, y, &R, &G, &B );
			Cut.SetValueC( cx, cy, R, G, B );
		}
	}

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	cvSaveImage( dst, Cut.m_Img );

	delete [] dst;

	return true;
}

bool CImageCV2::Save16to8b( string fname, bool Norm )
{
	//16bit��8bit�ɕϊ����ĕۑ����܂�

	if( m_Img == NULL ) return false;

	if( m_iCh != 1 && m_Img->depth != 16 )
	{
		return false;
	}
	
	CImageCV2 buf;
	if( !buf.Create8( m_iW, m_iH ) ) return false;

	int x, y;
	int brit;
	int Min = -1, Max = -1;

	if( Norm )
	{
		for( y = 0; y < m_iH; y++ )
		{
			for( x = 0; x < m_iW; x++ )
			{
				brit = GetImg16(x,y);

				if( brit == 0 ) continue;

				if( Min < 0 )
				{
					Min = Max =brit;
				}
				else
				{
					if( Min > brit ) Min = brit;
					if( Max < brit ) Max = brit;
				}
			}
		}
		
		if( Min < 0 ) return false;

		//�ŏ��l���O�A�ő�l���Q�T�T�ɕϊ�
		double coef = 255. / (double)(Max-Min);
		
		for( int y = 0; y < m_iH; y++ )
		{
			for( int x = 0; x < m_iW; x++ )
			{
				brit = (int)( coef * (GetImg16(x,y) - Min ) + 0.5 );
				
				if( brit > 255 ) brit = 255;
				else if( brit < 0 ) brit = 0;

				//SetImg16(x,y,brit);
				buf.SetImg( x, y, brit );
			}
		}
	}
	else
	{
		for( int y = 0; y < m_iH; y++ )
		{
			for( int x = 0; x < m_iW; x++ )
			{
				brit = GetImg16(x,y);
				
				if( brit > 255 ) brit = 255;
				else if( brit < 0 ) brit = 0;

				//SetImg16(x,y,brit);
				buf.SetImg( x, y, brit );
			}
		}

	}

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	cvSaveImage( dst, m_Img );

	delete [] dst;

	return true;
}

bool CImageCV2::Save16to8b( string fname, int sx, int sy, int CutW, int CutH, bool Norm )
{
	if( m_Img == NULL ) return false;

	if( m_iCh != 1 && m_Img->depth != 16 )
	{
		return false;
	}

	CImageCV2 Cut;
	if( !Cut.Create16( CutW, CutH ) ) return false;

	int x, y, cx, cy;
	//int R, G, B;

	for( y = sy, cy = 0; y < sy+CutH; y++, cy++ )
	{
		for( x = sx, cx = 0; x < sx+CutW; x++, cx++ )
		{
			Cut.SetImg16( cx, cy, GetImg16(x,y) );
		}
	}

	bool ret = Cut.Save16to8b( fname, Norm );

	return ret;

	/*int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	cvSaveImage( dst, Cut.m_Img );

	delete [] dst;

	return true;*/
}

bool CImageCV2::Save16Tif( string fname )
{
	//16bit�����̂܂ܕۑ�

	if( m_Img == NULL ) return false;

	if( m_iCh != 1 && m_Img->depth != 16 )
	{
		return false;
	}

	int len = fname.length();
	char* dst = new char[len+1];
	memcpy( dst, fname.c_str(), len+1 );

	cvSaveImage( dst, m_Img );

	delete [] dst;

	return true;
}

bool CImageCV2::Save16Tif( string fname, int sx, int sy, int CutW, int CutH )
{
	if( m_Img == NULL ) return false;

	if( m_iCh != 1 && m_Img->depth != 16 )
	{
		return false;
	}

	CImageCV2 Cut;
	if( !Cut.Create16( CutW, CutH ) ) return false;

	int x, y, cx, cy;
	//int R, G, B;

	for( y = sy, cy = 0; y < sy+CutH; y++, cy++ )
	{
		for( x = sx, cx = 0; x < sx+CutW; x++, cx++ )
		{
			Cut.SetImg16( cx, cy, GetImg16(x,y) );
		}
	}

	bool ret = Cut.Save16Tif( fname );

	return ret;
}

bool CImageCV2::ConvertB( CbImage *Dst )
{
	if( m_iCh == 3 ) return false;

	//memory
	if( !Dst->Alloc( m_iW, m_iH ) ) return false;

	int x, y;
	int W = m_iW;
	int H = m_iH;

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			Dst->SetValue( x, y, GetImg(x,y) );
		}
	}

	return true;
}

bool CImageCV2::ConvertW( CwImage *Dst )
{
	if( m_iCh == 3 ) return false;

	if( m_iDepth == 16 )
	{
		int W = m_iW;
		int H = m_iH;

		//memory
		if( !Dst->Alloc( W, H ) ) return false;

		int x, y;

		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				Dst->SetValue( x, y, GetImg16(x,y) );
			}
		}
	}
	else if( m_iDepth == 8 )
	{
		int W = m_iW;
		int H = m_iH;

		//memory
		if( !Dst->Alloc( W, H ) ) return false;

		int x, y;

		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				Dst->SetValue( x, y, GetImg(x,y) );
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CImageCV2::GetImg( int x, int y, int *R, int *G, int *B )
{
	if( this->m_iCh != 3 ) return false;
	
	int step = m_Img->widthStep;

	*B = m_Img->imageData[ x*m_iCh + y * step + 0 ];
	*G = m_Img->imageData[ x*m_iCh + y * step + 1 ];
	*R = m_Img->imageData[ x*m_iCh + y * step + 2 ];

	return true;
}

bool CImageCV2::GetImgGray( int x, int y, int *Gray )
{
	if( this->m_iCh != 3 ) return false;
	
	int step = m_Img->widthStep;

	int B = m_Img->imageData[ x*m_iCh + y * step + 0 ];
	int G = m_Img->imageData[ x*m_iCh + y * step + 1 ];
	int R = m_Img->imageData[ x*m_iCh + y * step + 2 ];

	*Gray = (int)( 0.299 * R + 0.587 * G + 0.114 * B + 0.5 );

	if( *Gray > 255 )    *Gray = 255;
	else if( *Gray < 0 ) *Gray = 0;

	return true;
}

bool CImageCV2::SetImg( int x, int y, int R, int G, int B )
{
	if( this->m_iCh != 3 ) return false;
	
	int step = m_Img->widthStep;

	m_Img->imageData[ x*m_iCh + y * step + 0 ] = B;
	m_Img->imageData[ x*m_iCh + y * step + 1 ] = G;
	m_Img->imageData[ x*m_iCh + y * step + 2 ] = R;

	return true;
}

bool CImageCV2::Copy( CImageCV2 *Src )
{
	//check size
	if( this->m_iW != Src->m_iW ||
		this->m_iH != Src->m_iH )
	{
		return false;
	}

	int W = m_iW;
	int H = m_iH;
	int x, y;
	int R, G, B;
	int Brit;

	//check depth
	if( this->m_iCh == 3 && Src->m_iCh == 3 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				Src->GetImg( x, y, &R, &G, &B );

				this->SetImg( x, y, R, G, B );
			}
		}
	}
	else if( this->m_iCh == 3 && Src->m_iCh == 1 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				Brit = Src->GetImg( x, y );

				this->SetImg( x, y, Brit, Brit, Brit );
			}
		}
	}
	else if( this->m_iCh == 1 && Src->m_iCh == 3 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				Src->GetImg( x, y, &R, &G, &B );
				
				Brit = (int)(R * 0.3 + G * 0.59 + B * 0.11);

				this->SetImg( x, y, Brit );
			}
		}
	}
	else if( this->m_iCh == 1 && Src->m_iCh == 1 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				Brit = Src->GetImg( x, y );

				this->SetImg( x, y, Brit );
			}
		}
	}
	else 
	{
		return false;
	}

	return true;
}

bool CImageCV2::Cut( CImageCV2 *Src, int sx, int sy, int CutW, int CutH )
{
	if( Src->m_iCh  == 1 && Src->m_iDepth  == 16 &&
		this->m_iCh == 1 && this->m_iDepth == 16 )
	{
		return Cut16( Src, sx, sy, CutW, CutH );
	}

	if( Src->m_iCh != 1 || Src->m_iDepth != 8 ) return false;
	if( !Create8( CutW, CutH ) ) return false;

	//check size
	if( this->m_iW < CutW || 
		this->m_iH < CutH )
	{
		return false;
	}

	int W = m_iW;
	int H = m_iH;
	int x, y;
	int R, G, B;
	int Brit;

	int ex = sx + CutW;
	int ey = sy + CutH;

	if( sx < 0 || sy < 0 ) return false;
	if( ex > Src->m_iW || ey > Src->m_iH ) return false;

	int cx, cy;

	//check depth
	if( this->m_iCh == 3 && Src->m_iCh == 3 )
	{
		for( cy = 0, y = sy; y < ey; y++, cy++ )
		{
			for( cx = 0, x = sx; x < ex; x++, cx++ )
			{
				Src->GetImg( x, y, &R, &G, &B );

				this->SetImg( cx, cy, R, G, B );
			}
		}
	}
	else if( this->m_iCh == 3 && Src->m_iCh == 1 )
	{
		for( cy = 0, y = sy; y < ey; y++, cy++ )
		{
			for( cx = 0, x = sx; x < ex; x++, cx++ )
			{
				Brit = Src->GetImg( x, y );

				this->SetImg( cx, cy, Brit, Brit, Brit );
			}
		}
	}
	else if( this->m_iCh == 1 && Src->m_iCh == 3 )
	{
		for( cy = 0, y = sy; y < ey; y++, cy++ )
		{
			for( cx = 0, x = sx; x < ex; x++, cx++ )
			{
				Src->GetImg( x, y, &R, &G, &B );
				
				Brit = (int)(R * 0.3 + G * 0.59 + B * 0.11);

				this->SetImg( cx, cy, Brit );
			}
		}
	}
	else if( this->m_iCh == 1 && Src->m_iCh == 1 )
	{
		for( cy = 0, y = sy; y < ey; y++, cy++ )
		{
			for( cx = 0, x = sx; x < ex; x++, cx++ )
			{
				Brit = Src->GetImg( x, y );

				this->SetImg( cx, cy, Brit );
			}
		}
	}
	else 
	{
		return false;
	}

	return true;
}

bool CImageCV2::Cut16( CImageCV2 *Src, int sx, int sy, int CutW, int CutH )
{
	if( Src->m_iCh  != 1 || Src->m_iDepth  != 16 ||
		this->m_iCh != 1 || this->m_iDepth != 16 ) return false;
	
	if( !Create16( CutW, CutH ) ) return false;

	//check size
	if( this->m_iW < CutW || 
		this->m_iH < CutH )
	{
		return false;
	}

	int W = m_iW;
	int H = m_iH;
	int x, y;

	int ex = sx + CutW;
	int ey = sy + CutH;

	if( sx < 0 || sy < 0 ) return false;
	if( ex > Src->m_iW || ey > Src->m_iH ) return false;

	int cx, cy, b;

	for( cy = 0, y = sy; y < ey; y++, cy++ )
	{
		for( cx = 0, x = sx; x < ex; x++, cx++ )
		{
			b = Src->GetImg16(x, y);

			this->SetImg16( cx, cy, b );
		}
	}	

	return true;
}

bool CImageCV2::CopyB( CbImage *Src )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	int W = Src->W;
	int H = Src->H;

	m_Img = cvCreateImage( cvSize(W, H), IPL_DEPTH_8U, 1 );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;

	int x, y, Brit;

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			Brit = Src->GetValue( x, y );
			this->SetImg( x, y, Brit );
		}
	}

	return true;
}

bool CImageCV2::Copy16( CwImage *Src )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	int W = Src->W;
	int H = Src->H;

	m_Img = cvCreateImage( cvSize(W, H), IPL_DEPTH_16U, 1 );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;

	int x, y;
	WORD Brit;

	for( y = 0; y < H; y++ )
	{
		for( x = 0; x < W; x++ )
		{
			Brit = Src->GetValue( x, y );
			this->SetImg16( x, y, Brit );
		}
	}

	return true;
}

bool CImageCV2::Copy16to8( CwImage *Src )
{
	if( m_Img != NULL )
	{
		ReleaseMem();
	}

	int W = Src->W;
	int H = Src->H;

	m_Img = cvCreateImage( cvSize(W, H), IPL_DEPTH_8U, 1 );
	
	if( m_Img == NULL ) return false;

	m_iW = m_Img->width;
	m_iH = m_Img->height;
	m_iCh = m_Img->nChannels;
	m_iDepth = m_Img->depth;

	int x, y;
	int brit;
	int Min = -1, Max = -1;

	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			brit = Src->GetValue(x,y);

			if( brit == 0 ) continue;

			if( Min < 0 )
			{
				Min = Max =brit;
			}
			else
			{
				if( Min > brit ) Min = brit;
				if( Max < brit ) Max = brit;
			}
		}
	}

	if( Min < 0 ) return false;

	//�ŏ��l���O�A�ő�l���Q�T�T�ɕϊ�
	double coef = 255. / (double)(Max-Min);

	for( int y = 0; y < m_iH; y++ )
	{
		for( int x = 0; x < m_iW; x++ )
		{
			brit = (int)( coef * ( Src->GetValue(x,y) - Min ) + 0.5 );

			if( brit > 255 ) brit = 255;
			else if( brit < 0 ) brit = 0;

			SetImg( x, y, brit );
		}
	}

	return true;
}

bool CImageCV2::Clone( Mat *Src )
{
	int W = Src->size().width;
	int H = Src->size().height;
	int ch = Src->channels();
	
	if( Src->type() == CV_8UC1 )
	{
		if( !Create8( W, H ) ) return false;

		for( int y = 0; y < H; y++ )
		{
			for( int x = 0; x < W; x++ )
			{
				//unsigned char b = (unsigned char)Src->at<unsigned char>(y,x);
				SetImg( x, y, Src->at<unsigned char>(y,x) );
			}
		}
	}
	else if( Src->type() == CV_8UC3 )
	{
		if( !Create24( W, H ) ) return false;

		for( int y = 0; y < H; y++ )
		{
			for( int x = 0; x < W; x++ )
			{
				int B = Src->at<Vec3b>(y,x)[0];
				int G = Src->at<Vec3b>(y,x)[1];
				int R = Src->at<Vec3b>(y,x)[2];

				SetImg( x, y, R, G, B );
			}
		}
	}
	else
	{
		//���x�����
		return false;
	}

	return true;
}

bool CImageCV2::Clone( IplImage *Src )
{
	int W = Src->width;
	int H = Src->height;
	int ch = Src->nChannels;

	//���x�����
	if( ch != 1 ) return false;

	if( !Create8( W, H ) ) return false;

	int step = Src->widthStep;

	for( int y = 0; y < H; y++ )
	{
		for( int x = 0; x < W; x++ )
		{
			//unsigned char b = (unsigned char)Src->at<unsigned char>(y,x);
			SetImg( x, y, Src->imageData[ x + y * step ] );
		}
	}

	return true;
}

bool CImageCV2::CopyFromMat( Mat *Src )
{
	int W = Src->size().width;
	int H = Src->size().height;
	int ch = Src->channels();
	int depth = 8;//Src->depth();

	if( m_Img == NULL )
	{
		//��蒼��
		if( depth == 8 &&  ch == 1 )
		{
			if( !Create8( W, H ) ) return false;
		}
		else if( depth == 8 && ch == 3 )
		{
			if( !Create24( W, H ) ) return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if( W != m_iW || H != m_iH || ch != m_iCh ) return false;
	}
	
	if( depth == 8 &&  ch == 1 ) 
	{
		for( int y = 0; y < H; y++ )
		{
			for( int x = 0; x < W; x++ )
			{
				SetImg( x, y, Src->at<unsigned char>(y,x) );
			}
		}
	}
	else if( depth == 8 && ch == 3 )
	{
		int R, G, B;

		for( int y = 0; y < H; y++ )
		{
			for( int x = 0; x < W; x++ )
			{
				//SetImg( x, y, Src->at<unsigned char>(y,x) );

				B = Src->data[ y * Src->step + x * Src->elemSize() + 0 ];
				G = Src->data[ y * Src->step + x * Src->elemSize() + 1 ];
				R = Src->data[ y * Src->step + x * Src->elemSize() + 2 ];

				SetImg( x, y, R, G, B );
			}
		}
	}


	return true;
}

int CImageCV2::GetMaxBrit()
{
	if( m_iCh == 3 ) return -1;

	int x, y, brit;
	int W = m_iW;
	int H = m_iH;

	int Max = 0;

	if( m_iDepth == 8 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				brit = GetImg(x,y);

				if( Max < brit )
				{
					Max = brit;
				}
			}
		}	

		return Max;
	}
	else if( m_iDepth == 16 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				brit = GetImg16(x,y);

				if( Max < brit )
				{
					Max = brit;
				}
			}
		}		

		return Max;
	}

	return -1;
}

int CImageCV2::GetMinBrit()
{
	if( m_iCh == 3 ) return -1;

	int x, y, brit;
	int W = m_iW;
	int H = m_iH;

	int Min = -1;

	if( m_iDepth == 8 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				brit = GetImg(x,y);

				if( Min < 0 ){
					Min = brit;
				}else 
				{
					if( Min > brit )
					{
						Min = brit;
					}
				}
			}
		}	

		return Min;
	}
	else if( m_iDepth == 16 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				brit = GetImg16(x,y);

				if( Min < 0 ) {
					Min = brit;
				}else 
				{
					if( Min > brit )
					{
						Min = brit;
					}
				}
			}
		}		

		return Min;
	}

	return -1;
}

bool CImageCV2::GetMinMaxBrit( int *Min, int *Max )
{
	if( m_iCh == 3 ) return false;

	int x, y, brit;
	int W = m_iW;
	int H = m_iH;

	*Min = -1;
	*Max = -1;

	if( m_iDepth == 8 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				brit = GetImg(x,y);
				if( brit == 0 ) continue;

				if( *Min < 0 )
				{
					*Min = *Max = brit;
				}
				else
				{
					if( *Min > brit ) *Min = brit;
					if( *Max < brit ) *Max = brit;
				}
			}
		}
	}
	else if( m_iDepth == 16 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				brit = GetImg16(x,y);
				if( brit == 0 ) continue;
				
				if( *Min < 0 )
				{
					*Min = *Max = brit;
				}
				else
				{
					if( *Min > brit ) *Min = brit;
					if( *Max < brit ) *Max = brit;
				}
			}
		}		
	}

	if( *Min < 0 ) return false;
	return true;
}

bool CImageCV2::ReduceSize( CImageCV2 *Dst, int Ratio )
{
	int Rw = m_iW / Ratio;
	int Rh = m_iH / Ratio;

	if( Rw <= 2 || Rh <= 2 ) return false;

	if( m_iDepth != 8 || m_iCh != 1 ) return false;

	if( !Dst->Create8( Rw, Rh ) ) return false;

	cvResize( this->m_Img, Dst->m_Img, CV_INTER_AREA );

	return true;
}

bool CImageCV2::Resize( CImageCV2 *Dst, int TargetSize, bool ShortStd )
{
	int DstW = TargetSize;
	double Ratio = (double)TargetSize / m_iW;
	int DstH = (int)( m_iH * Ratio + 0.5 );

	bool IsSame = false;

	if( ShortStd )
	{
		//�Z�������^�[�Q�b�g�T�C�Y�ɍ��킹��

		if( m_iW > m_iH )
		{
			DstH = TargetSize;
			Ratio = (double)TargetSize / m_iH;
			DstW = (int)( m_iW * Ratio + 0.5 );
				
			if( TargetSize == m_iH )
			{
				IsSame = true;
			}
		}
		else
		{
			if( TargetSize == m_iW )
			{
				IsSame = true;
			}
		}
	}
	else
	{
		//���������^�[�Q�b�g�T�C�Y�ɍ��킹��

		if( m_iW < m_iH )
		{
			DstH = TargetSize;
			Ratio = (double)TargetSize / m_iH;
			DstW = (int)( m_iW * Ratio + 0.5 );
			
			if( TargetSize == m_iH )
			{
				IsSame = true;
			}
		}
		else
		{
			if( TargetSize == m_iW )
			{
				IsSame = true;
			}
		}
	}

	if( IsSame )
	{
		if( !Dst->CreateClone( this ) ) return false;

		return true;
	}

	if( m_iDepth == 8 && m_iCh == 1 )
	{
		if( !Dst->Create8( DstW, DstH ) ) return false;
	}
	else if( m_iDepth == 8 && m_iCh == 3 )
	{
		if( !Dst->Create24( DstW, DstH ) ) return false;
	}
	else 
	{
		return false;
	}

	cvResize( this->m_Img, Dst->m_Img, CV_INTER_AREA );

	return true;
}


void CImageCV2::Clear()
{
	ReleaseMem();
	m_iW = 0;
	m_iH = 0;
	m_iCh = 0;
	m_iDepth = 0;
}

bool CImageCV2::WriteString( string str, int sx, int sy, double Scale )
{ 
	int len = str.length();
	char* dst = new char[len+1];
	memcpy( dst, str.c_str(), len+1 );

	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, Scale, Scale );

	cvPutText( m_Img, dst, cvPoint( sx, sy ), &font, CV_RGB(255,255,255) );

	delete [] dst;

	return true;
}

bool CImageCV2::SetPartial( CImageCV2 *Src, int sx, int sy )
{
	if( this->m_iCh != Src->m_iCh ||
		this->m_iDepth != Src->m_iDepth ) return false;

	int ex = sx + Src->m_iW;
	int ey = sy + Src->m_iH;

	if( ex >= this->m_iW ) ex = this->m_iW-1;
	if( ey >= this->m_iH ) ey = this->m_iH-1;

	int y, x;
	int ix, iy;
	int R, G, B;

	if( this->m_iCh == 3 && this->m_iDepth == 8 )
	{
		for( iy = 0, y = sy; y < ey; y++, iy++ )
		{
			for( ix = 0, x = sx; x < ex; x++, ix++ )
			{
				Src->GetImg( ix, iy, &R, &G, &B );

				this->SetImg( x, y, R, G, B );
			}
		}
	}
	else if( this->m_iCh == 1 && this->m_iDepth == 8 )
	{
		for( iy = 0, y = sy; y < ey; y++, iy++ )
		{
			for( ix = 0, x = sx; x < ex; x++, ix++ )
			{
				B = Src->GetImg(ix, iy);

				this->SetImg( x, y, B );
			}
		}
	}
	else if( this->m_iCh == 1 && this->m_iDepth == 16 )
	{
		for( iy = 0, y = sy; y < ey; y++, iy++ )
		{
			for( ix = 0, x = sx; x < ex; x++, ix++ )
			{
				B = Src->GetImg16( ix, iy );

				this->SetImg16( x, y, B );
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CImageCV2::ExpandHistogram( int NewMin, int NewMax )
{
	int OldMin, OldMax;
	if( !GetMinMaxBrit( &OldMin, &OldMax ) ) return false;

	double den = OldMax - OldMin;

	int x, y, b;

	for( y = 0; y < m_iH; y++ )
	{
		for( x = 0; x < m_iW; x++ )
		{
			b = (int)( 255.0 * GetImg(x,y) / den + 0.5 );
			if( b > 255 ) b = 255;
			else if( b < 0 ) b = 0;

			SetImg( x, y, b );
		}
	}

	return true;
}

bool CImageCV2::CutSquare( CImageCV2 *Dst )
{
	//�����`�ɃJ�b�g���܂�
	//�Z���ӂɍ��킹�܂�

	int side;
	int sx, sy;

	if( m_iH < m_iW )
	{
		side = m_iH;
		sx = (m_iW - m_iH) / 2;
		sy = 0;
	}
	else
	{
		side = m_iW;
		sx = 0;
		sy = (m_iH - m_iW) / 2;
	}

	if( !Dst->Cut( this, sx, sy, side, side ) ) return false;

	return true;
}

bool CImageCV2::SetZero()
{
	int x, y;
	int W = this->m_iW;
	int H = this->m_iH;

	if( this->m_iCh == 3 && this->m_iDepth == 8 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				this->SetImg(x,y,0,0,0);
			}
		}
	}
	else if( this->m_iCh == 1 && this->m_iDepth == 8 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				this->SetImg(x,y,0);
			}
		}
	}
	else if( this->m_iCh == 1 && this->m_iDepth == 16 )
	{
		for( y = 0; y < H; y++ )
		{
			for( x = 0; x < W; x++ )
			{
				this->SetImg16(x,y,0);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

#endif