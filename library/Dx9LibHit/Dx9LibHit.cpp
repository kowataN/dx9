/******************************************************************************/
//	�����蔻�胉�C�u����
//		�\�[�X�t�@�C��
/******************************************************************************/


//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibHit.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�����蔻��p�\����
ST_HitData	HIT[HIT_MAX];

//	�ǂݍ��ݗp�\����
ST_HitLoad	HLT[HIT_MAX];

//	�p�X
char	HitFilePath[MAX_PATH];



/******************************************************************************/
//	���O	�F	�q�b�g�f�[�^������
//	����	�F	�q�b�g�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	HitNo		�q�b�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitHit( long HitNo )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	HIT[ HitNo ].Use = OFF;
	HIT[ HitNo ].Flag = 0;
	HIT[ HitNo ].State = 0;

	HIT[ HitNo ].AnmNo = NOT;
	HIT[ HitNo ].FrameNo = NOT;

	for( short i=0; i<2; i++ )
	{
		HIT[ HitNo ].AREA.PosX[ i ] = 0;
		HIT[ HitNo ].AREA.PosY[ i ] = 0;
	}
}

/******************************************************************************/
//	���O	�F	�q�b�g�f�[�^�S������
//	����	�F	�q�b�g�\���̂̑S���������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitHitAll( void )
{
	ZeroMemory( HitFilePath, sizeof(HitFilePath) );

	for( long i=0; i<HIT_MAX; i++ )
	{
		InitHit( i );
	}
}

/******************************************************************************/
//	���O	�F	�q�b�g�t�@�C���ǂݍ���
//	����	�F	�q�b�g�t�@�C���̓ǂݍ��݂��s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]char	*FileName	�q�b�g�t�@�C����
//				[OUT]char	*LoadBuf	�ǂݍ��ݐ�̃o�b�t�@
//				[IN]long	HitNo		�q�b�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	LoadHitData( char *FileName, char *LoadBuf, long HitNo )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	LoadFile( FileName, LoadBuf );
	HLT[ HitNo ].Use = ON;
}

/******************************************************************************/
//	���O	�F	�q�b�g�Z�b�g
//	����	�F	�q�b�g�f�[�^�̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AnmNo		�Z�b�g����A�j���ԍ�
//				[IN]long	HitNo		�����蔻��ԍ�
//				[IN]long	FileNo		�t�@�C���ԍ�
//				[IN]long	HitDataNo	�����蔻��̃f�[�^�ԍ�
//				[IN]UChar	Flag		�t���O�i0�F��~�A1�F���[�v�j
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetHitData(long AnmNo,
				   long HitNo,
				   long FileNo,
				   long HitDataNo,
				   UChar Flag )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	HIT[ HitNo ].Use = ON;
	HIT[ HitNo ].AnmNo = AnmNo;
	HIT[ HitNo ].Flag = Flag;
	HIT[ HitNo ].State = ON;

	for( long i=0; i<2; i++ )
	{
		HIT[ HitNo ].AREA.PosX[ i ] = HLT[ FileNo ].HDT[ HitDataNo ].AREA.PosX[ i ];
		HIT[ HitNo ].AREA.PosY[ i ] = HLT[ FileNo ].HDT[ HitDataNo ].AREA.PosY[ i ];
	}
}

/******************************************************************************/
//	���O	�F	�����蔻��
//	����	�F	���蔻����s��
//	�߂�l	�F	short	���茋��	0(���ڐG)
//									1(�ڐG)
//	����	�F	[IN]long	HitNo1	�����蔻��̈�P
//				[IN]long	HitNo2	�����蔻��̈�Q
//	���l	�F	�Ȃ�
/******************************************************************************/
short	CheackHit( long HitNo1, long HitNo2 )
{
	if( HitNo1 >= HIT_MAX || HitNo2 >= HIT_MAX )
	{
		return 0;
	}

	short	Ret=0,
			x1,x2,x3,x4,y1,y2,y3,y4;

	if( !HIT[ HitNo1 ].Use || !HIT[ HitNo1 ].State
	|| !HIT[ HitNo2 ].Use || !HIT[ HitNo2 ].State )
	{
		return	Ret;
	}

	//	no1�̗̈�
	long PolNo1 = HIT[ HitNo1 ].PolNo;
	x1 = (short)g_LibPol.GetInfo( PolNo1 ).PosX + HIT[ HitNo1 ].AREA.PosX[ 0 ];
	y1 = (short)g_LibPol.GetInfo( PolNo1 ).PosY + HIT[ HitNo1 ].AREA.PosY[ 0 ];
	x2 = (short)g_LibPol.GetInfo( PolNo1 ).PosX + HIT[ HitNo1 ].AREA.PosX[ 1 ];
	y2 = (short)g_LibPol.GetInfo( PolNo1 ).PosY + HIT[ HitNo1 ].AREA.PosY[ 1 ];

	//	no2�̗̈�
	long PolNo2 = HIT[ HitNo2 ].PolNo;
	x3 = (short)g_LibPol.GetInfo( PolNo2 ).PosX + HIT[ HitNo2 ].AREA.PosX[ 0 ];
	y3 = (short)g_LibPol.GetInfo( PolNo2 ).PosY + HIT[ HitNo2 ].AREA.PosY[ 0 ];
	x4 = (short)g_LibPol.GetInfo( PolNo2 ).PosX + HIT[ HitNo2 ].AREA.PosX[ 1 ];
	y4 = (short)g_LibPol.GetInfo( PolNo2 ).PosY + HIT[ HitNo2 ].AREA.PosY[ 1 ];

	//	�ڐG����
	if( ( x1 <= x4 ) && ( x2 >= x3 )
	&& ( y1 <= y4 ) && ( y2 >= y3 ) )
	{
		Ret = 1;
	}

	return	Ret;
}

/******************************************************************************/
//	���O	�F	�����蔻��\��
//	����	�F	���蔻��̈��\������
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	HitNo		�����蔻��ԍ�
//				[IN]long	DispPolNo	�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	DispHit( long HitNo, long DispPolNo )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	double	px,py;
	short	w,h;

	long PolNo = HIT[ HitNo ].PolNo;

	px = g_LibPol.GetInfo( PolNo ).PosX + HIT[ HitNo ].AREA.PosX[0];
	py = g_LibPol.GetInfo( PolNo ).PosY + HIT[ HitNo ].AREA.PosY[0];

	//	���T�C�Y
	if( HIT[ HitNo ].AREA.PosX[0] > HIT[ HitNo ].AREA.PosX[1] )
	{
		w = HIT[ HitNo ].AREA.PosX[0] - HIT[ HitNo ].AREA.PosX[1];
	}
	else
	{
		w = HIT[ HitNo ].AREA.PosX[1] - HIT[ HitNo ].AREA.PosX[0];
	}

	//	�c�T�C�Y
	if( HIT[ HitNo ].AREA.PosY[0] > HIT[ HitNo ].AREA.PosY[1] )
	{
		h = HIT[ HitNo ].AREA.PosY[0] - HIT[ HitNo ].AREA.PosY[1];
	}
	else
	{
		h = HIT[ HitNo ].AREA.PosY[1] - HIT[ HitNo ].AREA.PosY[0];
	}

	g_LibPol.Set( DispPolNo, w, h, px,py,0.0, ATR_BOX );
	if( !HIT[ HitNo ].State )
	{
		g_LibPol.SetColor( DispPolNo, 128 );
	}
	else
	{
		g_LibPol.SetColor( DispPolNo, 255 );
	}
}

/******************************************************************************/
//	���O	�F	�����蔻��S�\��
//	����	�F	���蔻��̈��S�ĕ\������
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo	�|���S���ԍ��i�Œᐔ�j
//	���l	�F	�Ȃ�
/******************************************************************************/
void	DispHitAll( long PolNo )
{
	for( long i=0; i<HIT_MAX; i++ )
	{
		DispHit( i, PolNo );
		PolNo ++;
	}
}

/******************************************************************************/
//	���O	�F	�����蔻��f�[�^�\��
//	����	�F	���蔻��f�[�^��\������
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	HitNo	�����蔻��ԍ�
//				[IN]long	TextNo	�\���Ɏg���e�L�X�g�ԍ�
//				[IN]double	PosX	�\�����WX
//				[IN]double	PosY	�\�����WY
//	���l	�F	�Ȃ�
/******************************************************************************/
void	DispHitData( long HitNo, long TextNo, double PosX, double PosY )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	short	x1,x2,y1,y2,w,h;

	long PolNo = HIT[ HitNo ].PolNo;

	x1 = (short)g_LibPol.GetInfo( PolNo ).PosX + HIT[ HitNo ].AREA.PosX[0];
	y1 = (short)g_LibPol.GetInfo( PolNo ).PosY + HIT[ HitNo ].AREA.PosY[0];
	x2 = (short)g_LibPol.GetInfo( PolNo ).PosX + HIT[ HitNo ].AREA.PosX[1];
	y2 = (short)g_LibPol.GetInfo( PolNo ).PosY + HIT[ HitNo ].AREA.PosY[1];

	w = HIT[ HitNo ].AREA.PosX[1] - HIT[ HitNo ].AREA.PosX[0];
	h = HIT[ HitNo ].AREA.PosY[1] - HIT[ HitNo ].AREA.PosY[0];

	if( TXT[ TextNo ].Use == OFF )
	{
		g_LibText.Set( TextNo, PosX,PosY, 255,255,0,0, "" );
	}

	sprintf_s( TXT[ TextNo ].Str, MAX_PATH,
		"����_(%d,%d)�@�E���_(%d,%d)�@�T�C�Y(%d,%d)", x1,y1, x2,y2, w,h );
}
