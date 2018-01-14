/******************************************************************************/
//	DirectX9���C�u����
//		�R����
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�ėp���[�N�G���A
char	g_FreeWork[FREE_WORK_SIZE];



/******************************************************************************/
//	���O	�F	�t�@�C�����[�h
//	����	�F	�t�@�C����ǂݍ���
//	�߂�l	�F	long		��������	-1(���s)
//										-1�ȊO(�t�@�C���T�C�Y)
//	����	�F	[IN]char	*FileName	�t�@�C����
//				[OUT]char	*LoadBuf	�ǂݍ��݃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
long	LoadFile( char *FileName, char *LoadBuf )
{
	long	Ret = -1;
	FILE	*fp;
	errno_t	Err;
	int		Index = 0;
	struct	stat St;

	//	�t�@�C�����擾
	Index = stat( FileName, &St );
	if( Index != 0 )
	{
		//	�擾���s
		return	Ret;
	}

	for( int i=0; i<10; i++ )
	{
		//	�t�@�C���I�[�v��
		Err = fopen_s( &fp, FileName, "rb" );
		if( Err == 0 )
		{
			//	�t�@�C���I�[�v���ɐ���
			if( fseek( fp, 0, SEEK_SET ) == 0 )
			{
				//	�t�@�C�����[�h
				if( fread( LoadBuf, St.st_size, 1, fp ) >= 1 )
				{
					Ret = St.st_size;
				}
			}
		}

		//	�t�@�C���N���[�Y
		fclose( fp );
		if( Ret >= 0 )
		{
			break;
		}
	}

	return Ret;
}

/******************************************************************************/
//	���O	�F	�t�@�C���Z�[�u
//	����	�F	�t�@�C����ۑ�����
//	�߂�l	�F	long		��������	1(����)
//										0(���s)
//	����	�F	[IN]char	*FileName	�t�@�C����
//				[IN]char	*SaveBuf	�ǂݍ��݃A�h���X
//				[IN]long	size		�T�C�Y
//	���l	�F	�Ȃ�
/******************************************************************************/
long	SaveFile( char *FileName, char *SaveBuf, long Size )
{
	long	Ret,ct;
	FILE	*fp;
	errno_t	Err;

	Ret = 0;
	for( int i=0; i<10; i++ )
	{
		//	�t�@�C���I�[�v��
		Err = fopen_s( &fp, FileName, "wb" );
		if( Err == 0 )
		{
			fseek( fp, 0, SEEK_SET );

			ct = 0;
			while( 1 )
			{
				if( Size == 0 )
				{
					break;
				}
				
				if( Size < 256 )
				{
					fwrite( &SaveBuf[ct], Size, 1, fp );
					break;
				}

				//	�Z�[�u
				fwrite( &SaveBuf[ct], Size, 1, fp );
				ct += 256;
				Size -= 256;
				fseek( fp, ct, SEEK_SET );
			}

			Ret = 1;
		}

		//	�t�@�C���N���[�Y
		fclose( fp );

		if( Ret == 1 )
		{
			break;
		}
	}

	if( Ret < 0 )
	{
		Ret = 0;
	}

	return Ret;
}

/******************************************************************************/
//	���O	�F	�p�X����t�@�C�����擾
//	����	�F	�p�X����t�@�C�������擾����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]char	*Path		�t���p�X
//				[OUT]char	*FileName	�i�[����t�@�C����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	PathToName( char *Path, char *FileName )
{
	char	*pt = strrchr( Path, 0x5c );
	if( pt == NULL )
	{
		wsprintf( FileName, Path );
	}
	else
	{
		wsprintf( FileName, (pt+1) );
	}
}

/******************************************************************************/
//	���O	�F	�p�X����g���q�擾
//	����	�F	�p�X����g���q���擾����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]char	*Path		�t���p�X
//				[OUT]char	*FileName	�i�[�g���q
//	���l	�F	�Ȃ�
/******************************************************************************/
void	PathToOpt( char *Path, char *FileName )
{
	// ��납��P�ڂ�'.'
	char	*pt = strrchr( Path, '.' );
	if( pt == NULL )
	{
		wsprintf( FileName, "" );
	}
	else
	{
		wsprintf( FileName, (pt+1) );
	}
}

/******************************************************************************/
//	���O	�F	�p�X����f�B���N�g�����o
//	����	�F	�p�X����f�B���N�g���𒊏o����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]char	*Path	�t���p�X
//				[OUT]char	*Dir	�i�[�f�B���N�g��
//	���l	�F	�Ȃ�
/******************************************************************************/
void	PathToDir( char *Path, char *Dir )
{
	char	*pt;
	// ��납��P�ڂ�'\'
	pt = strrchr( Path, 0x5c );
	if( pt == NULL )
	{
		wsprintf( Dir, "" );
	}
	else
	{
		wsprintf( Dir, Path );
		*(Dir + (pt - Path)) = 0;
	}
}

/******************************************************************************/
//	���O	�F	�p�X�ύX
//	����	�F	��Ɨp�p�X��ύX����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]char	*NewPath		�t���p�X
//				[OUT]char	*BackupPath		�ύX�O�̃f�B���N�g���i�[�A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
void	PathChange( char *NewPath, char *BakupPath )
{
	//char	FilePath[MAX_PATH];

	//	�J�����g�f�B���N�g���E�o�b�N�A�b�v
	GetCurrentDirectory( MAX_PATH, BakupPath );
	//	�f�B���N�g�����o
	//PathToDir( NewPath, FilePath );
	//	�f�B���N�g���ύX
	SetCurrentDirectory( NewPath );
}

/******************************************************************************/
//	���O	�F	�Í���
//	����	�F	�Í������s��
//	�߂�l	�F	long	�T�C�Y
//	����	�F	[IN/OUT]char	*FileName	�Í�������t�@�C���̃|�C���^
//	���l	�F	�Ȃ�
/******************************************************************************/
long	Secret( char *FileName )
{
	char	Rnd = (char)137;
	long	Size = LoadFile( FileName, g_FreeWork );
	UChar	*pt = (UChar *)g_FreeWork;

	if( Size > 0 )
	{
		//	�Í���
		for( int i=0; i<Size; i++ )
		{
			Rnd = Rnd * 5 - 3;
			*pt = *pt + Rnd;
			pt++;
		}
	}

	return Size;
}

/******************************************************************************/
//	���O	�F	�Í��t�@�C���̃Z�[�u
//	����	�F	�Í���Ƀt�@�C����ۑ�����
//	�߂�l	�F	�Ȃ�
//	����	�F	[OUT]char	*FileName		�Í��t�@�C��
//				[IN]char	SaveFileName	�ۑ��t�@�C����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SaveSecret( char *FileName, char *SaveFileName )
{
	long Size = Secret( FileName );
	SaveFile( SaveFileName, FileName, Size );
}

/******************************************************************************/
//	���O	�F	�Í��t�@�C���̉��
//	����	�F	�Í������ꂽ�t�@�C������ǂ���
//	�߂�l	�F	long	�ǂݍ��񂾃t�@�C���̃T�C�Y
//	����	�F	[IN]char	*SecretFile	��ǂ���t�@�C��
//				[OUT]char	*FileName	��ǐ�A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
long	AntiSecret( char *SecretFile, char *FileName )
{
	char	Rnd = (char)137;
	long	Size = LoadFile( SecretFile, FileName );
	UChar	*pt = (UChar *)FileName;

	if( Size > 0 )
	{
		//	��ǉ�
		for( int i=0; i<Size; i++ )
		{
			Rnd = Rnd * 5 - 3;
			*pt = *pt - Rnd;
			pt++;
		}
	}

	return	Size;
}

/******************************************************************************/
//	���O	�F	short�^���|�C���^��
//	����	�F	short�^�̃f�[�^��1�o�C�g�Âi�[����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]short		dat		�|�C���^�ɒǉ�����ϐ�
//				[IN/OUT]char	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetShort( short Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<2; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	���O	�F	long�^���|�C���^��
//	����	�F	long�^�̃f�[�^��1�o�C�g�Âi�[����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long		dat		�|�C���^�ɒǉ�����ϐ�
//				[IN/OUT]char	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetLong( long Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<4; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	���O	�F	float�^���|�C���^��
//	����	�F	float�^�̃f�[�^��1�o�C�g�Âi�[����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]float		dat		�|�C���^�ɒǉ�����ϐ�
//				[IN/OUT]char	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetFloat( float Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<4; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	���O	�F	double�^���|�C���^��
//	����	�F	double�^�̃f�[�^��1�o�C�g�Âi�[����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]double		dat		�|�C���^�ɒǉ�����ϐ�
//				[IN/OUT]double	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetDouble( double Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<8; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	���O	�F	short�^�擾
//	����	�F	short�^�̃f�[�^���擾����
//	�߂�l	�F	short	short�^�̃f�[�^
//	����	�F	[IN]char	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
short	GetShort( char *pt )
{
	union
	{
		char	dat[2];
		short	data;
	}SRT;

	for( int i=0; i<2; i++ )
	{
		SRT.dat[ i ] = *( pt + i );
	}
	return	SRT.data;
}

/******************************************************************************/
//	���O	�F	long�^�擾
//	����	�F	long�^�̃f�[�^���擾����
//	�߂�l	�F	long	long�^�̃f�[�^
//	����	�F	[IN]char	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
long	GetLong( char *pt )
{
	union
	{
		char	dat[4];
		long	data;
	}LNG;

	for( int i=0; i<4; i++ )
	{
		LNG.dat[ i ] = *( pt + i );
	}

	return	LNG.data;
}

/******************************************************************************/
//	���O	�F	float�^�擾
//	����	�F	float�^�̃f�[�^���擾����
//	�߂�l	�F	float	float�^�̃f�[�^
//	����	�F	[IN]char	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
float	GetFloat( char *pt )
{
	union
	{
		char	dat[4];
		float	data;
	}FLT;

	for( int i=0; i<4; i++ )
	{
		FLT.dat[ i ] = *( pt + i );
	}

	return	FLT.data;
}

/******************************************************************************/
//	���O	�F	double�^�擾
//	����	�F	double�^�̃f�[�^���擾����
//	�߂�l	�F	double	double�^�̃f�[�^
//	����	�F	[IN]char	*pt		�|�C���^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
double	GetDouble( char *pt )
{
	union
	{
		char	dat[4];
		double	data;
	}DBL;

	for( int i=0; i<8; i++ )
	{
		DBL.dat [ i ] = *( pt + i );
	}

	return	DBL.data;
}

/******************************************************************************/
//	���O	�F	�t�@�C���I�[�v���_�C�A���O�\��
//	����	�F	�t�@�C���I�[�v���_�C�A���O��\������
//	�߂�l	�F	bool	��������	true(����)
//									false(���s)
//	����	�F	[IN]char		*Filter		�t�B���^�[
//				[IN]char		*Ext		�ǂݍ��ފg���q��
//				[IN]char		*Title		�ǂݍ��ގ��̃^�C�g��
//				[IN/OUT]char	*Path		�p�X
//	���l	�F	�Ȃ�
/******************************************************************************/
bool	LoadFileDialog( char *Filter, char *Ext, char *Title, char *Path )
{
	OPENFILENAME	ofn;
	char	FileTitle[64] = { "" };

	memset( &ofn, 0, sizeof( OPENFILENAME ) );

	ofn.lStructSize		= sizeof( OPENFILENAME );
	ofn.hwndOwner		= g_hWnd;

	ofn.lpstrFilter		= Filter;
	ofn.lpstrFile		= Path;

	ofn.nMaxFile		= 0xFFFF;
	ofn.Flags			= OFN_NODEREFERENCELINKS;
	ofn.lpstrDefExt		= Ext;
	ofn.nMaxFileTitle	= 64;
	ofn.lpstrFileTitle	= FileTitle;
	ofn.lpstrTitle		= Title;

	if( GetOpenFileName( &ofn ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************/
//	���O	�F	�t�@�C���Z�[�u�_�C�A���O�\��
//	����	�F	�t�@�C���Z�[�u�_�C�A���O��\������
//	�߂�l	�F	bool	��������	true(����)
//									false(���s)
//	����	�F	[IN]char		*Filter		�t�B���^�[
//				[IN]char		*Ext		�ǂݍ��ފg���q��
//				[IN]char		*Title		�ǂݍ��ގ��̃^�C�g��
//				[IN/OUT]char	*Path		�p�X
//	���l	�F	�Ȃ�
/******************************************************************************/
bool	SaveFileDialog( char *Filter, char *Ext, char *Title, char *Path )
{
	OPENFILENAME	ofn;
	char	FileTitle[64] = { "" };

	memset( &ofn, 0, sizeof( OPENFILENAME ) );

	ofn.lStructSize		= sizeof( OPENFILENAME );
	ofn.hwndOwner		= g_hWnd;

	ofn.lpstrFilter		= Filter;
	ofn.lpstrFile		= Path;

	ofn.nMaxFile		= 0xFFFF;
	ofn.Flags			= OFN_NODEREFERENCELINKS;
	ofn.lpstrDefExt		= Ext;
	ofn.nMaxFileTitle	= 64;
	ofn.lpstrFileTitle	= FileTitle;
	ofn.lpstrTitle		= Title;

	if( GetSaveFileName( &ofn ) )
	{
		return	true;
	}
	else
	{
		return	false;
	}
}

/******************************************************************************/
//	���O	�F	�p�x�擾
//	����	�F	�p�x���v�Z����
//	�߂�l	�F	double		�p�x�i���W�A���j
//	����	�F	[IN]double	x1	���WX1
//				[IN]double	y1	���WY1
//				[IN]double	x2	���WX2
//				[IN]double	y2	���WY2
//	���l	�F	�Ȃ�
/******************************************************************************/
double	GetRot( double x1, double y1, double x2, double y2 )
{
	double w = x2 - x1;
	double h = y2 - y1;
	double rot = atan2( h, w );

	return rot;
}

/******************************************************************************/
//	���O	�F	�����擾
//	����	�F	�������v�Z����
//	�߂�l	�F	double	�Q�_�Ԃ̋���
//	����	�F	[IN]double	x1	���WX1
//				[IN]double	y1	���WY1
//				[IN]double	x2	���WX2
//				[IN]double	y2	���WY2
//	���l	�F	�Ȃ�
/******************************************************************************/
double	GetDistance( double x1, double y1, double x2, double y2 )
{
	double w = x2 - x1;
	double h = y2 - y1;
	double dis = sqrt( (h*h)+(w*w) );

	return dis;
}

/******************************************************************************/
//	���O	�F	�p�x�␳
//	����	�F	�p�x��␳����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN/OUT]double	&Rot	�p�x
//	���l	�F	�p�x��0�`359�ɂ���
/******************************************************************************/
void	CorrectRot( double &Rot )
{
	// �N���b�s���O
	if( Rot >= 360 )
	{
		Rot -= 360;
	}
	else if( Rot < 0 )
	{
		Rot += 360;
	}
}

/******************************************************************************/
//	���O	�F	���W�A�����f�O���[�ɕϊ�
//	����	�F	���W�A�����f�O���[�ɕϊ�����
//	�߂�l	�F	double	�p�x�i�f�O���[�j
//	����	�F	[IN]double	Rot		�p�x�i���W�A���j
//	���l	�F	�Ȃ�
/******************************************************************************/
double	RadToDeg( double Rot )
{
	double	Ret = (Rot * 180.0 ) / PI;
	CorrectRot( Ret );
	return Ret;
}

/******************************************************************************/
//	���O	�F	�f�O���[�����W�A���ɕϊ�
//	����	�F	�f�O���[�����W�A���ɕϊ�����
//	�߂�l	�F	double	�p�x�i���W�A���j
//	����	�F	[IN]double	Rot		�p�x�i�f�O���[�j
//	���l	�F	�Ȃ�
/******************************************************************************/
double	DegToRad( double Rot )
{
	double	Ret = ( Rot * PI ) / 180.0;

	if( Ret > 2 * PI )
	{
		Ret -= 2 * PI;
	}

	if( Ret < 0 )
	{
		Ret += 2 * PI;
	}

	return Ret;
}

/******************************************************************************/
//	���O	�F	�ړ��ʎ擾(X)
//	����	�F	�ړ��ʂ��v�Z����
//	�߂�l	�F	double	�ړ���
//	����	�F	[IN]double	Speed		���x
//				[IN]double	Rot			�p�x(���W�A��)
//	���l	�F	�Ȃ�
/******************************************************************************/
double	GetMoveX( double Speed, double Rot )
{
	double x = Speed * cos( Rot );
	return x;
}

/******************************************************************************/
//	���O	�F	�ړ��ʎ擾(Y)
//	����	�F	�ړ��ʂ��v�Z����
//	�߂�l	�F	double	�ړ���
//	����	�F	[IN]double	Speed		���x
//				[IN]double	Rot			�p�x(���W�A��)
//	���l	�F	�Ȃ�
/******************************************************************************/
double	GetMoveY( double Speed, double Rot )
{
	double y = Speed * sin( Rot );
	return y;
}

/******************************************************************************/
//	���O	�F	�ő�l�ŏ��l����
//	����	�F	�ő�l�ŏ��l�̔�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN/OUT]double	Target	����Ώۃf�[�^
//				[IN]double		Max		�ő�l
//				[IN]double		Min		�ŏ��l
//	���l	�F	�Ȃ�
/******************************************************************************/
void	MaxMin( double& Target, double Max, double Min )
{
	if( Target > Max )
	{
		Target = Max;
	}
	else if( Target < Min )
	{
		Target = Min;
	}
}

/******************************************************************************/
//	���O	�F	�ő�l�ŏ��l����
//	����	�F	�ő�l�ŏ��l�̔�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN/OUT]long	Target	����Ώۃf�[�^
//				[IN]long		Max		�ő�l
//				[IN]long		Min		�ŏ��l
//	���l	�F	�Ȃ�
/******************************************************************************/
void	MaxMin( long& Target, long Max, long Min )
{
	if( Target > Max )
	{
		Target = Max;
	}
	else if( Target < Min )
	{
		Target = Min;
	}
}

/******************************************************************************/
//	���O	�F	16�i�����񐔒l�ϊ�
//	����	�F	16�i�̕�����𐔒l�ɕϊ�����
//	�߂�l	�F	UChar	�ϊ���̐��l
//	����	�F	[IN]char	*Target	16�i������
//	���l	�F	�Ȃ�
/******************************************************************************/
ULong	ToDec( const char *Target )
{
	long	Index = 0;
	ULong	Ret = 0;
	char	cTarget;

	//	�����܂Ń��[�v
	while( Target[Index] != '\0' )
	{
		long	Num = 0;
		cTarget = tolower( Target[Index] );

		//	0�`9����
		if( '0' <= cTarget && cTarget <= '9' )
		{
			//	���l�ɕϊ�
			Num = cTarget - '0';
		}
		else if( 'a' <= cTarget && cTarget <= 'f' )
		{
			//	a�`f
			Num = cTarget - 'a' + 10;
		}

		//	�C���f�b�N�X���Z
		Index ++;

		Ret = Ret * 16 + Num;
	}

	return Ret;
}

/******************************************************************************/
//	���O	�F	�E�G�C�g
//	����	�F	��萔�������~�߂�
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	time	�t���[����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	WaitTime( long time )
{
	clock_t	sTime;
	sTime = clock() + ( (clock_t)time * CLOCKS_PER_SEC ) / 60;
	while( clock() < sTime );
}

/******************************************************************************/
//	���O	�F	�����񕪊�
//	����	�F	������̕������s��
//	�߂�l	�F	list<string>			������̕�����
//	����	�F	[IN]string	Str			�����Ώۂ̕�����
//				[IN]string	delim		��؂蕶��
//	���l	�F	�Ȃ�
/******************************************************************************/
vector<string>	SplitText( string Str, string delim )
{
	vector<string>	Res;
	int	Count;

	while( (Count = (int)Str.find_first_of(delim) ) != Str.npos )
	{
		if( Count > 0 )
		{
			//	�o�b�t�@�Ɋi�[
			Res.push_back( Str.substr( 0, Count ) );
		}
		//	�����ʒu�����炷
		Str = Str.substr( Count + 1 );
	}

	if( Str.length() > 0 )
	{
		Res.push_back(Str);
	}

	return Res;
}
