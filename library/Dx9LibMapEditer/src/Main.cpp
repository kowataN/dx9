/******************************************************************************/
//	�}�b�v�G�f�B�^�[
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibPad.h"

#include	"MainD.h"

//----------------------------------------------------------------------------//
//	�O���[�o��
//----------------------------------------------------------------------------//
short		MainMode = OFF;
char		BmpFilePath[MAX_PATH];	//	BMP�t�@�C���̃p�X
char		MapFilePath[MAX_PATH];	//	MAP�t�@�C���̃p�X
char		BmpFileName[MAX_PATH];	//	BMP�t�@�C���̖��O
char		MapFileName[MAX_PATH];	//	MAP�t�@�C���̖��O
short		DispX,DispY;			//	MAP�̕\�����W
FLAG_TBL	FLAG;					//	�t���O�Ǘ��\����
MAP_TBL		MAP;					//	MAP�\����
MAP_TBL		S_MAP;					//	�T�[�`�p

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
void	MainInit( void );						//	���C��������
void	EditerMain( void );						//	�G�f�B�^�[���C��
void	MenuSelect( void );						//	���j���[�Z���N�g
short	MouseSel( void );						//	�}�E�X�Z���N�g
void	Clip( void );							//	�N���b�s���O
void	MapSave( void );						//	�}�b�v�f�[�^�̃Z�[�u
void	MapLoad( void );						//	�}�b�v�f�[�^�̃��[�h
void	BmpLoad( void );						//	BMP�t�@�C���̃��[�h
void	WidthUp( void );						//	�v��
void	WidthDown( void );						//	�v��
void	HeightUp( void );						//	�g��
void	HeightDown( void );						//	�g��
void	Layer1Switch( void );					//	���C���[�P�X�C�b�`
void	Layer2Switch( void );					//	���C���[�Q�X�C�b�`
void	Layer3Switch( void );					//	���C���[�R�X�C�b�`
void	HitSwitch( void );						//	�����蔻��X�C�b�`
void	PageLeft( void );						//	�y�[�W��
void	PageRight( void );						//	�y�[�W�E
void	MenuHitChk( void );						//	���j���[�Ƃ̓����蔻��
void	ChipSel( void );						//	�`�b�v�̑I��
void	ChipMapSet( UChar Flag );				//	�`�b�v���}�b�v�ɃZ�b�g
long	MapSet( long Map, long Layer );			//	�}�b�v�Z�b�g
long	ChipGet( long MapDat, long Layer );		//	�`�b�v�ԍ��擾
long	MapHitChkSet( long Map );				//	�}�b�v�����蔻��Z�b�g
long	MapHitChkGet( long Map );				//	�}�b�v�����蔻��擾
void	MapInit(void );							//	�}�b�v������
void	MapDisp( void );						//	�}�b�v�\��
void	MapOff( void );							//	�}�b�v��\��
long	LayerDisp( long PolNo, long Layer );	//	���C���[�\��
void	FontDisp( void );						//	�����\��
void	GridInit( void );						//	�O���b�h������
void	GridDisp( void );						//	�O���b�h�\��
void	BrushCheck( void );						//	�u���V�Ď�
void	BrushPaint( void );						//	�h��Ԃ�
void	BrushPaintSearch( long x, long y );		//	�h��Ԃ��ӏ����o
void	GetKeyButton( void );					//	�L�[�̎擾
void	ChipToCursorPos( long Chip );			//	�`�b�v�ԍ�����J�[�\�����W�擾
void	MenuDispCheck( void );					//	���j���[�\���Ď�
void	*Tbl[]=
{
	MapSave,
	MapLoad,
	BmpLoad,
	WidthUp,
	WidthDown,
	HeightUp,
	HeightDown,
	Layer1Switch,
	Layer2Switch,
	Layer3Switch,
	HitSwitch,
	PageLeft,
	PageRight,
};

void	DebugMsInit( void );	//	�f�o�b�O�p�����̏�����
void	DebugMsDisp( void );	//	�f�o�b�O�p�����̕\��



/******************************************************************************/
//	���C��
/******************************************************************************/
void	MainLoop( void )
{
	switch( MainMode )
	{
		/////////////////////////////////////
		//	���C��������
		case	OFF:
			MainInit();
			MainMode = ON;

		/////////////////////////////////////
		//	�G�f�B�^�[���C��
		case	ON:
			EditerMain();
			break;
	}
}

/******************************************************************************/
//	���C��������
/******************************************************************************/
void	MainInit( void )
{
	/////////////////////////////////////
	//	���C�u����������
	InitTextureAll();
	InitPatternAll();
	InitPolygonAll();
	g_LibText.InitAll();
	g_LibFade.Init();
	PAD[0].Use = ON;

	/////////////////////////////////////
	//	�e�N�X�`���[�ǂݍ���
	LoadTexture( TEX_SYSTEM, "dat\\system000.bmp" );

	/////////////////////////////////////
	//	�p�^�[��
	short	pat_no[6] =
	{
		PAT_MENU,	//	0:���j���[
		PAT_CURSOR,	//	1:�J�[�\��
		PAT_FRM,	//	2:�X�C�b�`�g
		PAT_MARU,	//	3:��
		PAT_BATU,	//	4:�~
		PAT_BRUSH,	//	5:�u���V
	};
	for( long i=0; i<6; i++ )
	{
		SetPattern( pat_no[i], TEX_SYSTEM,
			PatDat[i][0], PatDat[i][1],
			PatDat[i][2], PatDat[i][3] );
	}

	/////////////////////////////////////
	//	�|���S��
	short	pol_no[4][2] =
	{
		POL_MENU,	PAT_MENU,		//	���j���[
		POL_CURSOR,	PAT_CURSOR,		//	�J�[�\��
		POL_FRM,	PAT_FRM,		//	�X�C�b�`�g
		POL_SEL,	PAT_CURSOR,		//	�J�[�\���i���j���[�j
	};

	for( long i=0; i<4; i++ )
	{
		SetPolygonPat( pol_no[i][0], pol_no[i][1],
			PolDisp[i][0], PolDisp[i][1], PolDisp[i][2], ATR_NONE );
	}

	SetPolygonPat( POL_BRUSH, PAT_BRUSH, MOUSE.PosX,MOUSE.PosY,1.0, ATR_NONE );

	/////////////////////////////////////
	//	�e�L�X�g
	g_LibText.Set( TXT_MOUSE, Font[0][0], Font[0][1], 255, 255, 255, 255, "" );
	g_LibText.Set( TXT_FILE_NAME, Font[1][0], Font[1][1], 255, 0, 0, 0, "" );
	g_LibText.Set( TXT_WIDTH, Font[2][0], Font[2][1], 255, 0, 0, 0, "" );
	g_LibText.Set( TXT_HEIGHT, Font[3][0], Font[3][1], 255, 0, 0, 0, "" );

	ZeroMemory( BmpFilePath, MAX_PATH );
	ZeroMemory( MapFilePath, MAX_PATH );
	ZeroMemory( BmpFileName, MAX_PATH );
	ZeroMemory( MapFileName, MAX_PATH );

	//	�����}�b�v�T�C�Y������
	MAP.Use = OFF;
	MAP.Width = 40;
	MAP.Height = 30;

	//	�w�i
	SetPolygon( POL_BG,
		MAP.Width*CHIP_SIZE, MAP.Height*CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
	SetPolygonColor( POL_BG, 255, 100, 100, 100 );

	//	�t���O������
	FLAG.HitMenu = OFF;
	FLAG.HitChipWin = OFF;
	FLAG.Page = 0;
	FLAG.Chip = 0;
	FLAG.ChipBk = 0;
	FLAG.DispMap = OFF;
	FLAG.LoadBmp = OFF;
	FLAG.Layer = 0;
	FLAG.Brush = OFF;

	//	�}�b�v�\���ʒu������
	DispX = DispY = 0;

	/////////////////////////////////////
	//	�}�b�v������
	MapInit();

	/////////////////////////////////////
	//	�O���b�h������
	GridInit();

	DebugMsInit();
}

/******************************************************************************/
//	�G�f�B�^�[���C������
/******************************************************************************/
void	EditerMain( void )
{
	long	mx = MOUSE.PosX / CHIP_SIZE;
	long	my = MOUSE.PosY / CHIP_SIZE;

	/////////////////////////////////////
	//	���j���[�Ƃ̓����蔻��
	MenuHitChk();

	/////////////////////////////////////
	//	�}�E�X����
	if( FLAG.HitMenu )
	{
		MenuSelect();
	}
	else
	{
		//	���蔻��
		if( FLAG.Layer == 3 )
		{
			if( MOUSE.Trg[ MOUSE_LEFT ] )
			{
				if( FLAG.Brush == OFF )
				{
					ChipMapSet( 0 );
				}
				else if( FLAG.Brush == ON )
				{
					BrushPaint();
				}

				FLAG.DispMap = ON;
			}
		}
		else
		{
			if( MOUSE.Trg[ MOUSE_LEFT ] || MOUSE.Push[ MOUSE_LEFT ] )
			{
				if( FLAG.Brush == OFF )
				{
					ChipMapSet( 0 );
				}
				else if( FLAG.Brush == ON )
				{
					BrushPaint();
				}

				FLAG.DispMap = ON;
			}
			else if( MOUSE.Trg[ MOUSE_RIGHT ] || MOUSE.Push[ MOUSE_RIGHT ] )
			{
				ChipMapSet( 1 );
				FLAG.DispMap = ON;
			}
		}
	}

	/////////////////////////////////////
	//	�`�b�v�I��
	ChipSel();

	/////////////////////////////////////
	//	�N���b�s���O
	POL[ POL_CURSOR ].PosX = mx * CHIP_SIZE;
	POL[ POL_CURSOR ].PosY = my * CHIP_SIZE;
	Clip();

	/////////////////////////////////////
	//	�}�b�v�\��
	if( FLAG.DispMap )
	{
		MapDisp();
	}

	/////////////////////////////////////
	//	�t�H���g�\��
	FontDisp();

	/////////////////////////////////////
	//	�O���b�h�\��
	GridDisp();

	/////////////////////////////////////
	//	�L�[�{�[�h����
	GetKeyButton();

	/////////////////////////////////////
	//	�u���V�̊Ď�
	BrushCheck();

	PAT[ PAT_BMP ].PosX = ChipPatDat[ FLAG.Page ][0];
	PAT[ PAT_BMP ].PosY = ChipPatDat[ FLAG.Page ][1];

	DebugMsDisp();
}

/******************************************************************************/
//	���j���[�Z���N�g
/******************************************************************************/
void	MenuSelect( void )
{
	if( MOUSE.Trg[ MOUSE_LEFT ]
	|| ( ( MOUSE.Count[ MOUSE_LEFT ] >= KEY_REPEAT_START )
	&& ( MOUSE.Count[ MOUSE_LEFT ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		short ret = MouseSel();
		if( ret >= 0 )
		{
			void(*func)(void);
			func = ( void(*)(void) )Tbl[ ret ];
			(*func)();
		}
	}
}

/******************************************************************************/
//	�}�E�X�̑I��
/******************************************************************************/
short	MouseSel( void )
{
	short	ret = -1;
	for( short i=0; i<DAT_HIT_MAX; i++ )
	{
		short	x1 = DatHit[ i ][ 0 ];
		short	x2 = x1 + DatHit[ i ][ 2 ];
		short	y1 = DatHit[ i ][ 1 ];
		short	y2 = y1 + DatHit[ i ][ 3 ];

		if( ( MOUSE.PosX >= x1 ) && ( MOUSE.PosX <= x2 )
		&& ( MOUSE.PosY >= y1 ) && ( MOUSE.PosY <= y2 ) )
		{
			ret = i;
			break;
		}
	}

	return	ret;
}

/******************************************************************************/
//	�N���b�s���O
/******************************************************************************/
void	Clip( void )
{
	short	PolNo = POL_CURSOR;

	//	���[
	if( POL[ PolNo ].PosX < POL[ POL_BG ].PosX )
	{
		POL[ PolNo ].PosX = POL[ POL_BG ].PosX;
	}

	//	�E�[
	if( POL[ PolNo ].PosX >= POL[ POL_BG ].PosX + MAP.Width * CHIP_SIZE )
	{
		POL[ PolNo ].PosX = POL[ POL_BG ].PosX + MAP.Width * CHIP_SIZE - CHIP_SIZE;
	}

	//	��[
	if( POL[ PolNo ].PosY < POL[ POL_BG ].PosY )
	{
		POL[ PolNo ].PosY = POL[ POL_BG ].PosY;
	}

	//	���[
	if( POL[ PolNo ].PosY >= POL[ POL_BG ].PosY + MAP.Height * CHIP_SIZE )
	{
		POL[ PolNo ].PosY = POL[ POL_BG ].PosY + MAP.Height * CHIP_SIZE - CHIP_SIZE;
	}
}

/******************************************************************************/
//	�}�b�v�f�[�^�ۑ�
/******************************************************************************/
void	MapSave( void )
{
	long	Chip[256]={NOT};
	long	FileSize = 0;

	ShowCursor( TRUE );
	//	�t�@�C���_�C�A���O�̕\��
	if( SaveFileDialog( Dialog[0], Dialog[2], Dialog[5], MapFilePath ) == 0 )
	{
		ShowCursor( FALSE );
		return;
	}
	ShowCursor( FALSE );

	/////////////////////////////////////
	//	����

	//	bmp�t�@�C���̒���
	long BmpFileLen = (long)strlen( BmpFileName );

	/////////////////////////////////////
	//	�w�b�_��
	ZeroMemory( &g_FreeWork, sizeof(g_FreeWork) );
	char *pt = (char *)g_FreeWork;
	//	�t�@�C���̎��
	*pt = 'M';
	pt ++;
	*pt = 'A';
	pt ++;
	*pt = 'P';
	pt ++;
	FileSize += 3;

	//	bmp�t�@�C���̒���
	SetLong( BmpFileLen, pt );
	pt += 4;
	FileSize += 4;

	//	bmp�t�@�C����
	for( long i=0; i<BmpFileLen; i++ )
	{
		*pt = BmpFileName[ i ];
		pt ++;
		FileSize ++;
	}

	//	���`�b�v���擾
	long	ChipMax = 0;
	for( long h=0; h<MAP.Height; h++ )
	{
		for( long w=0; w<MAP.Width; w++ )
		{
			for( long Layer=0; Layer<LAYER_HIT; Layer++ )
			{
				//	�`�b�v�ԍ��擾
				long	ChipNo = MAP.Layer[Layer].Chip[h][w];
				if( Layer >= 1 && ChipNo == 0 )
				{
					continue;
				}

				long	UseFlag = 1;
				for( long Index=0; Index<ChipMax; Index++ )
				{
					if( Chip[ Index ] == ChipNo )
					{
						UseFlag = 0;
					}
				}

				if( UseFlag )
				{
					Chip[ ChipMax ] = ChipNo;
					ChipMax ++;
				}
			}
		}
	}

	//	�}�b�v�T�C�Y
	SetLong( MAP.Height, pt );
	pt += 2;
	FileSize += 2;
	SetLong( MAP.Width, pt );
	pt += 2;
	FileSize += 2;

	//	���`�b�v��
	SetLong( ChipMax, pt );
	pt += 4;
	FileSize += 4;

	/////////////////////////////////////
	//	���f�[�^

	//	�}�b�v�f�[�^
	for( long h=0; h<MAP.Height; h++ )
	{
		TRACE("\n");
		for( long w=0; w<MAP.Width; w++ )
		{
			long	Map = 0;
			for( long Layer=0; Layer<LAYER_MAX; Layer++)
			{
				Map |= MapSet( MAP.Layer[Layer].Chip[h][w], Layer );
			}
			Map |= MapHitChkSet( MAP.Layer[LAYER_HIT].Chip[h][w] );

			TRACE1("%d ",Map);
			SetLong( Map, pt );
			pt += 4;
			FileSize += 4;
		}
	}

	//	�����o��
	SaveFile( MapFilePath, g_FreeWork, FileSize );
}

/******************************************************************************/
//	�}�b�v�f�[�^�̓ǂݍ���
/******************************************************************************/
void	MapLoad( void )
{
	//	�}�b�v�����Ɏg�p�����ǂ���
	if( MAP.Use )
	{

	}

	ShowCursor( TRUE );
	//	�t�@�C���_�C�A���O�̕\��
	if( LoadFileDialog( Dialog[0], Dialog[2], Dialog[4], MapFilePath ) == 0 )
	{
		ShowCursor( FALSE );
		return;
	}
	ShowCursor( FALSE );

	ZeroMemory( &g_FreeWork, sizeof(g_FreeWork) );
	long size = LoadFile( MapFilePath, g_FreeWork );

	/////////////////////////////////////
	//	�w�b�_��

	char *pt = ( char * )g_FreeWork;

	//	�t�@�C���̎��
	if( ( pt[0] != 'M' )
	&& ( pt[1] != 'A' )
	&& ( pt[2] != 'P' ) )
	{
		return;
	}
	pt += 3;

	//	�t�@�C���̒����擾
	long BmpFileLen = GetLong( pt );
	pt += 4;

	//	bmp�t�@�C����
	for( long i=0; i<BmpFileLen; i++ )
	{
		BmpFileName[ i ] = *pt;
		pt ++;
	}

	//	�}�b�v�T�C�Y
	MAP.Height = GetShort( pt );
	pt += 2;
	MAP.Width = GetShort( pt );
	pt += 2;

	//	���p�^�[����
	pt += 4;

	/////////////////////////////////////
	//	���f�[�^

	//	�}�b�v�f�[�^
	for( long h=0; h<MAP.Height; h++ )
	{
		TRACE("\n");
		for( long w=0; w<MAP.Width; w++ )
		{
			long Chip = GetLong( pt );
			pt += 4;

			TRACE1("%d ",Chip);
			if( Chip < 0 )
			{
				continue;
			}

			for( long Layer=0; Layer<LAYER_MAX; Layer++ )
			{
				MAP.Layer[Layer].Chip[h][w] = ChipGet( Chip, Layer );
			}
			MAP.Layer[LAYER_HIT].Chip[h][w] = MapHitChkGet(Chip);
		}
	}
	TRACE("\n");

	/////////////////////////////////////
	//	�J�����g�f�B���N�g���̎擾
	char FilePath[MAX_PATH];

	GetCurrentDirectory( MAX_PATH, MapFilePath );
	strcat_s( MapFilePath, MAX_PATH, "\\" );
	strcat_s( MapFilePath, MAX_PATH, BmpFileName );
	PathToDir( MapFilePath, FilePath );
	SetCurrentDirectory( FilePath );
	strcat_s( FilePath, MAX_PATH, "\\" );
	strcat_s( FilePath, MAX_PATH, BmpFileName );

	/////////////////////////////////////
	//	bmp�t�@�C���ǂݍ���
	LoadTexture( TEX_BMP, FilePath );
	SetPattern( PAT_BMP, TEX_BMP, 0, 0, 96, 192 );
	SetPolygonPat( POL_BMP, PAT_BMP, CHIP_SIZE, 164, 0.7, ATR_NONE );

	PathToName( BmpFileName, BmpFilePath );
	FLAG.LoadBmp = ON;
	FLAG.DispMap = ON;
}

/******************************************************************************/
//	bmp�ǂݍ���
/******************************************************************************/
void	BmpLoad( void )
{
	ShowCursor( TRUE );
	if( LoadFileDialog( Dialog[1], Dialog[3], Dialog[4], BmpFilePath ) == 0 )
	{
		ShowCursor( FALSE );
		return;
	}
	ShowCursor( FALSE );

	LoadTexture( TEX_BMP, BmpFilePath );
	SetPattern( PAT_BMP, TEX_BMP, 0, 0, 96, 192 );
	SetPolygonPat( POL_BMP, PAT_BMP, CHIP_SIZE, 164, 0.7, ATR_NONE );

	FLAG.LoadBmp = ON;

	PathToName( BmpFilePath, BmpFileName );

	MapInit();
	MapOff();
}

/******************************************************************************/
//	����
/******************************************************************************/
void	WidthUp( void )
{
	MAP.Width ++;
	MaxMin( MAP.Width, MAP_W_MAX, 0 );

	//	�w�i
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	����
/******************************************************************************/
void	WidthDown( void )
{
	MAP.Width --;
	MaxMin( MAP.Width, MAP_W_MAX, 0 );

	//	�w�i
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	������
/******************************************************************************/
void	HeightUp( void )
{
	MAP.Height ++;
	MaxMin( MAP.Height, MAP_H_MAX, 0 );

	//	�w�i
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	������
/******************************************************************************/
void	HeightDown( void )
{
	MAP.Height --;
	MaxMin( MAP.Height, MAP_H_MAX, 0 );

	//	�w�i
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	���C���[�P�X�C�b�`
/******************************************************************************/
void	Layer1Switch( void )
{
	FLAG.Layer = 0;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 20, 368 );
}

/******************************************************************************/
//	���C���[�Q�X�C�b�`
/******************************************************************************/
void	Layer2Switch( void )
{
	FLAG.Layer = 1;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 68, 368 );
}

/******************************************************************************/
//	���C���[�R�X�C�b�`
/******************************************************************************/
void	Layer3Switch( void )
{
	FLAG.Layer = 2;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 20, 388 );
}

/******************************************************************************/
//	�����蔻��
/******************************************************************************/
void	HitSwitch( void )
{
	FLAG.Layer = 3;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 68, 388 );
}

/******************************************************************************/
//	���{�^��
/******************************************************************************/
void	PageLeft( void )
{
	FLAG.Page --;
	MaxMin( FLAG.Page, PAGE_MAX, 0 );

	PAT[ PAT_BMP ].PosX = ChipPatDat[ FLAG.Page ][0];
	PAT[ PAT_BMP ].PosY = ChipPatDat[ FLAG.Page ][1];
}

/******************************************************************************/
//	�E�{�^��
/******************************************************************************/
void	PageRight( void )
{
	FLAG.Page ++;
	MaxMin( FLAG.Page, PAGE_MAX, 0 );

	PAT[ PAT_BMP ].PosX = ChipPatDat[ FLAG.Page ][0];
	PAT[ PAT_BMP ].PosY = ChipPatDat[ FLAG.Page ][1];
}

/******************************************************************************/
//	���j���[�Ƃ̓����蔻��
/******************************************************************************/
void	MenuHitChk( void )
{
	short	x1 = PatDat[0][0];
	short	y1 = PatDat[0][1];
	short	x2 = PatDat[0][2];
	short	y2 = PatDat[0][3];

	if( ( MOUSE.PosX >= x1 ) && ( MOUSE.PosX <= x2 )
	&& ( MOUSE.PosY >= y1 ) && ( MOUSE.PosY <= y2 ) )
	{
		FLAG.HitMenu = ON;
	}
	else
	{
		FLAG.HitMenu = OFF;
	}
}

/******************************************************************************/
//	�`�b�v�I��
/******************************************************************************/
void	ChipSel( void )
{
	long	x1 = CHIP_WINDOW_X;
	long	y1 = CHIP_WINDOW_Y;
	long	x2 = CHIP_WINDOW_W;
	long	y2 = CHIP_WINDOW_H;

	if( ( MOUSE.PosX > x1 ) && ( MOUSE.PosX < x2 )
	&& ( MOUSE.PosY > y1 ) && ( MOUSE.PosY < y2 ) )
	{
		FLAG.HitChipWin = ON;
		if( MOUSE.Trg[ MOUSE_LEFT ] )
		{
			long x = MOUSE.PosX - CHIP_WINDOW_X;
			long y = MOUSE.PosY - CHIP_WINDOW_Y;
			x /= CHIP_SIZE;	
			y /= CHIP_SIZE;

			FLAG.Chip = ( (y%CHIP_H_NUM_MAX) * CHIP_W_NUM_MAX ) + x + ( FLAG.Page * CHIP_NUM_MAX );

			POL[ POL_SEL ].PosX = CHIP_SIZE * x + CHIP_WINDOW_X;
			POL[ POL_SEL ].PosY = CHIP_SIZE * y + CHIP_WINDOW_Y;
		}
	}
	else
	{

		FLAG.HitChipWin = OFF;
	}
}

/******************************************************************************/
//	�`�b�v���}�b�v�ɓo�^
/******************************************************************************/
void	ChipMapSet( UChar Flag )
{
	long	PosX = MOUSE.PosX / CHIP_SIZE - 8 - DispX;
	long	PosY = MOUSE.PosY / CHIP_SIZE - DispY;

	if( FLAG.Layer == LAYER_HIT )
	{
		//	�����蔻��
		if( MapHitChkGet( MAP.Layer[ FLAG.Layer ].Chip[PosY][PosX] ) == 0 )
		{
			MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ] = ON;
		}
		else
		{
			MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ] = OFF;
		}
	}
	else
	{
		if( Flag == 0 )
		{
			//	���N���b�N
			MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ] = FLAG.Chip;
		}
		else
		{
			//	�E�N���b�N
			FLAG.Chip = MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ];

			//	�`�b�v�ԍ�����J�[�\�����W�擾
			ChipToCursorPos( FLAG.Chip );
		}
	}
}

/******************************************************************************/
//	�}�b�v�Z�b�g
/******************************************************************************/
long	MapSet( long Map, long Layer )
{
	if( Layer == 0 )
	{
		//	���C���[0
		return (Map<<LAYER(Layer));
	}
	else
	{
		if( Map == NOT )
		{
			return	0;
		}
		else
		{
			return (Map<<LAYER(Layer));
		}
	}
}

/******************************************************************************/
//	�`�b�v�ԍ��擾
/******************************************************************************/
long	ChipGet( long MapDat, long Layer )
{
	if( MapDat < 0 )
	{
		return 0;
	}
	else
	{
		long ChipNo = ((MapDat&(0x3FF<<LAYER(Layer)))>>LAYER(Layer));
		return ChipNo;
	}
}

/******************************************************************************/
//	�}�b�v�̓����蔻��t���O�Z�b�g
/******************************************************************************/
long	MapHitChkSet( long Map )
{
	return (Map<<MAP_INDEX_HIT);
}

/******************************************************************************/
//	�}�b�v�̓����蔻��t���O�擾
/******************************************************************************/
long	MapHitChkGet( long Map )
{
	return ((Map&(0x3FF<<MAP_INDEX_HIT))>>MAP_INDEX_HIT);
}

/******************************************************************************/
//	�}�b�v�̏�����
/******************************************************************************/
void	MapInit(void )
{
	for( long h=0; h<MAP_H_MAX; h++ )
	{
		for( long w=0; w<MAP_W_MAX; w++ )
		{
			for( long Layer=0; Layer<LAYER_MAX; Layer++ )
			{
				if( Layer == LAYER_HIT )
				{
					MAP.Layer[Layer].Chip[h][w] = OFF;
				}
				else
				{
					MAP.Layer[Layer].Chip[h][w] = NOT;
				}
			}
		}
	}

	MAP.Use = OFF;
}

/******************************************************************************/
//	�}�b�v�\��
/******************************************************************************/
void	MapDisp( void )
{
	FLAG.DispMap = OFF;
	if(	!FLAG.LoadBmp )
	{
		return;
	}

	long PolNo = 0;
	for( int i=0; i<LAYER_HIT; i++ )
	{
		PolNo = LayerDisp( PolNo, i );
	}
}

/******************************************************************************/
//	�}�b�v��\��
/******************************************************************************/
void	MapOff( void )
{
	long Size = MAP.Height * MAP.Width;
	for( long i=0; i<Size; i++ )
	{
		POL[ POL_MAP + i ].Disp = OFF;
	}
}

/******************************************************************************/
//	���C���[�\��
//		�߂�l�F
//				�g�����|���S����
//		�����F
//				PolNo		�g���擪�̃|���S���ԍ�
//				Layer		�\�����郌�C���[
/******************************************************************************/
long	LayerDisp( long PolNo, long Layer )
{
	for( long h=0; h<MAP.Height; h++ )
	{
		for( long w=0; w<MAP.Width; w++ )
		{
			if( MAP.Layer[Layer].Chip[h][w] == NOT )
			{
				continue;
			}
			if( MAP.Layer[Layer].Chip[h][w] == 0 && Layer >= LAYER_2 )
			{
				continue;
			}

			//	�p�^�[��
			//long PatNo = ChipGet( MAP.Layer[Layer].Chip[h][w], Layer );
			long PatNo = MAP.Layer[Layer].Chip[h][w];
			if( PatNo != NOT )
			{
				long x = ( PatNo / CHIP_NUM_MAX ) / 2 * BMP_PAT_SIZE_W;
				long AddX = PatNo - ( (PatNo / CHIP_NUM_MAX) * CHIP_NUM_MAX );
				AddX %= CHIP_W_NUM_MAX;
				AddX *= CHIP_SIZE;
				x += AddX;
				long IndexH = ( PatNo / CHIP_NUM_MAX ) * CHIP_NUM_MAX;
				long ChipIndex = PatNo - IndexH;
				ChipIndex /= CHIP_W_NUM_MAX;
				ChipIndex *= CHIP_SIZE;
				long AddY = ( ( PatNo / CHIP_NUM_MAX ) % 2 ) * BMP_PAT_SIZE_H;
				long y = ChipIndex + AddY;

				if( !PAT[PAT_MAP+PatNo].Use )
				{
					SetPattern( PAT_MAP+PatNo, TEX_BMP, x, y, CHIP_SIZE, CHIP_SIZE );
				}

				SetPolygonPat( POL_MAP+PolNo, PAT_MAP+PatNo,
					POL[POL_BG].PosX + ( CHIP_SIZE * w ),
					POL[POL_BG].PosY + ( CHIP_SIZE * h ),
					0.1, ATR_NONE );
			}
			else
			{
				InitPolygon( POL_MAP + PolNo );
			}

			//	�A���t�@
			if( FLAG.Layer == Layer )
			{
				POL[ POL_MAP + PolNo ].A = 255;
			}
			else
			{
				POL[ POL_MAP + PolNo ].A = 255 / 3;
			}

			PolNo ++;
		}
	}

	return PolNo;
}

/******************************************************************************/
//	�t�H���g�\��
/******************************************************************************/
void	FontDisp( void )
{
	long mx = MOUSE.PosX / CHIP_SIZE - 8 - DispX;
	long my = MOUSE.PosY / CHIP_SIZE - DispY;

	if( FLAG.HitMenu )
	{
		sprintf_s( TXT[ TXT_MOUSE ].Str, MAX_PATH, "" );
	}
	else
	{
		sprintf_s( TXT[ TXT_MOUSE ].Str, MAX_PATH, "X:%3d,Y%3d", mx,my );
	}

	sprintf_s( TXT[ TXT_FILE_NAME ].Str, MAX_PATH, "%s", BmpFileName );
	TXT[ TXT_FILE_NAME ].PosX = 12;
	TXT[ TXT_FILE_NAME ].PosY = 80;
	sprintf_s( TXT[ TXT_WIDTH ].Str, MAX_PATH, "%3d", MAP.Width );
	sprintf_s( TXT[ TXT_HEIGHT ].Str, MAX_PATH, "%3d", MAP.Height );
}

/******************************************************************************/
//	�O���b�h������
/******************************************************************************/
void	GridInit( void )
{
	FLAG.DispGrid = ON;

	long	w = 480 / CHIP_SIZE;
	long	h = 640 / CHIP_SIZE;

	for( int i=0; i<w; i++ )
	{
		SetPolygon( POL_GRID+i, 640, 1, 0, i*CHIP_SIZE, 0.3, ATR_NONE );	//	����
	}
	for( int i=0; i<h; i++ )
	{
		SetPolygon( POL_GRID+30+i, 1, 480, i*CHIP_SIZE, 0, 0.3, ATR_NONE );	//	�c��
	}
	for( int i=0; i<w+h; i++ )
	{
		SetPolygonColor( POL_GRID+i, 255, 0, 0, 0 );
	}

	GridDisp();
}

/******************************************************************************/
//	�O���b�h�\��
/******************************************************************************/
void	GridDisp( void )
{
	long	w = 480 / CHIP_SIZE;
	long	h = 640 / CHIP_SIZE;
	for( int i=0; i<w+h; i++ )
	{
		POL[POL_GRID+i].Disp = FLAG.DispGrid;
	}
}

/******************************************************************************/
//	�u���V�Ď�
/******************************************************************************/
void	BrushCheck( void )
{
	POL[ POL_BRUSH ].PosX = MOUSE.PosX;

	if( FLAG.Brush == OFF )
	{
		//	�m�[�}��
		POL[ POL_BRUSH ].PosY = MOUSE.PosY;
		SetPattern( PAT_BRUSH, TEX_SYSTEM, 176,0, 16,16 );
	}
	else
	{
		//	�u���V
		POL[ POL_BRUSH ].PosY = MOUSE.PosY - 12;
		SetPattern( PAT_BRUSH, TEX_SYSTEM, 160,0, 16,16 );
	}
}

/******************************************************************************/
//	�h��Ԃ�
/******************************************************************************/
void	BrushPaint( void )
{
	long	x = MOUSE.PosX / CHIP_SIZE - 8 - DispX;
	long	y = MOUSE.PosY / CHIP_SIZE - DispY;

	for( long Layer=0; Layer<LAYER_MAX; Layer++ )
	{
		for( long h=0; h<MAP_H_MAX; h++ )
		{
			for( long w=0; w<MAP_W_MAX; w++ )
			{
				S_MAP.Layer[Layer].Chip[h][w] = OFF;
			}
		}
	}

	//	�`�b�v�̃o�b�N�A�b�v
	FLAG.ChipBk = MAP.Layer[ FLAG.Layer ].Chip[y][x];
	ChipMapSet( 0 );

	S_MAP.Layer[ FLAG.Layer ].Chip[y][x] = ON;

	//	����
	BrushPaintSearch( x, y );
}

/******************************************************************************/
//	�h��Ԃ��ӏ�����
//		�߂�l�F
//				�O�F�����I���A�P�F������
/******************************************************************************/
void	BrushPaintSearch( long x, long y )
{
	//////////////////////////////////
	//	�ċN�֐��Ō���

	if( y-1 >= 0 )
	{
		//	��𒲂ׂ�
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] = FLAG.Chip;
			BrushPaintSearch( x, y-1 );
		}
	}

	if( y+1 < MAP.Height )
	{
		//	���𒲂ׂ�
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] = FLAG.Chip;
			BrushPaintSearch( x, y+1 );
		}
	}

	if( x-1 >= 0 )
	{
		//	���𒲂ׂ�
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] = FLAG.Chip;
			BrushPaintSearch( x-1, y );
		}
	}

	if( x+1 < MAP.Width )
	{
		//	�E�𒲂ׂ�
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] = FLAG.Chip;
			BrushPaintSearch( x+1, y );
		}
	}
}

/******************************************************************************/
//	�L�[�{�[�h���͏���
/******************************************************************************/
void	GetKeyButton( void )
{
	short	mx,my,x,y;
	u_char	alt,ctrl,shift;

	mx = my = x = y = 0;
	alt = ctrl = shift = OFF;

	//	��
	if( KEY.Trg[ DIK_UP ]
	|| ( ( KEY.Count[ DIK_UP ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_UP ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		my --;
	}

	//	��
	if( KEY.Trg[ DIK_DOWN ]
	|| ( ( KEY.Count[ DIK_DOWN ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_DOWN ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		my ++;
	}

	//	��
	if( KEY.Trg[ DIK_LEFT ]
	|| ( ( KEY.Count[ DIK_LEFT ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_LEFT ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		mx --;
	}

	//	��
	if( KEY.Trg[ DIK_RIGHT ]
	|| ( ( KEY.Count[ DIK_RIGHT ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_RIGHT ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		mx ++;
	}

	if( KEY.Push[ DIK_LALT ] || KEY.Push[ DIK_RALT ] )
	{
		alt = ON;	//	[ALT]�L�[
	}
	if( KEY.Push[ DIK_LCONTROL ] || KEY.Push[ DIK_RCONTROL ] )
	{
		ctrl = ON;	//	[CTRL]�L�[
	}
	if( KEY.Push[ DIK_LSHIFT ] || KEY.Push[ DIK_RSHIFT ] )
	{
		shift = ON;	//	[SHIFT]�L�[
	}

	if( alt )
	{
		/////////////////////////////////////
		//	[ALT]�L�[
	}
	else	if( ctrl )
	{
		/////////////////////////////////////
		//	[CTRL]�L�[
	}
	else	if( shift )
	{
		/////////////////////////////////////
		//	[SHIFT]�L�[
		if( KEY.Trg[ DIK_L ] )
		{
			FLAG.Layer --;
			MaxMin( FLAG.Layer, LAYER_MAX, LAYER_1 ); 
			FLAG.DispMap = ON;
			switch( FLAG.Layer )
			{
				case	LAYER_1:
					SetPolygonPos( POL_FRM, 20, 368 );
					break;
				case	LAYER_2:
					SetPolygonPos( POL_FRM, 68, 368 );
					break;
				case	LAYER_3:
					SetPolygonPos( POL_FRM, 20, 388 );
					break;
				case	LAYER_HIT:
					SetPolygonPos( POL_FRM, 68, 388 );
					break;
			}
		}
	}
	else
	{
		/////////////////////////////////////
		//	���̑�
		DispX += mx;
		DispY += my;

		if( mx != 0 || my != 0 )
		{
			FLAG.DispMap = ON;
		}

		SetPolygon( POL_BG,
			MAP.Width*CHIP_SIZE, MAP.Height*CHIP_SIZE,
			128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );

		if( KEY.Trg[ DIK_G ] )
		{
			FLAG.DispGrid ^= 1;	//	�O���b�h�\���ؑ�
		}
		if( KEY.Trg[ DIK_B ] )
		{
			FLAG.Brush ^= 1;		//	�u���V�ؑ�
		}
		if( KEY.Trg[ DIK_M ] )
		{
			MenuDispCheck();		//	���j���[�\���ؑ�
		}
		if( KEY.Trg[ DIK_L ] )
		{
			FLAG.Layer ++;
			MaxMin( FLAG.Layer, LAYER_MAX, LAYER_1 ); 
			FLAG.DispMap = ON;
			switch( FLAG.Layer )
			{
				case	LAYER_1:
					SetPolygonPos( POL_FRM, 20, 368 );
					break;
				case	LAYER_2:
					SetPolygonPos( POL_FRM, 68, 368 );
					break;
				case	LAYER_3:
					SetPolygonPos( POL_FRM, 20, 388 );
					break;
				case	LAYER_HIT:
					SetPolygonPos( POL_FRM, 68, 388 );
					break;
			}
		}
	}
}

/******************************************************************************/
//	�`�b�v�ԍ�����J�[�\�����W�擾
/******************************************************************************/
void	ChipToCursorPos( long Chip )
{
	double x = ( Chip % CHIP_W_NUM_MAX ) * CHIP_SIZE;
	x += Chip / BMP_PAT_SIZE_H * BMP_PAT_SIZE_W;
	double y = ( ( Chip - (Chip/BMP_PAT_SIZE_H*BMP_PAT_SIZE_H) ) / CHIP_W_NUM_MAX ) * CHIP_SIZE;
	x /= CHIP_SIZE;
	y /= CHIP_SIZE;

	long Page = (short)x / CHIP_W_NUM_MAX;
	FLAG.Page = Page * 2;
	Page *= 6;
	x -= Page;
	x = (short)x;

	Page = (short)y / CHIP_H_NUM_MAX;
	FLAG.Page += Page;
	Page *= CHIP_H_NUM_MAX;
	y -= Page;
	y = (short)y;

	POL[ POL_SEL ].PosX = CHIP_SIZE * x + CHIP_WINDOW_X;
	POL[ POL_SEL ].PosY = CHIP_SIZE * y + CHIP_WINDOW_Y;

	if( Chip < 0 )
	{
		POL[ POL_SEL ].Disp = OFF;
	}
	else
	{
		POL[ POL_SEL ].Disp = ON;
	}
}

/******************************************************************************/
//	���j���[�\���Ď�
/******************************************************************************/
void	MenuDispCheck( void )
{
	//	�t���O���]
	POL[ POL_MENU ].Disp ^= 1;
	POL[ POL_BMP ].Disp ^= 1;
	TXT[ TXT_MOUSE ].Disp ^= 1;
	TXT[ TXT_FILE_NAME ].Disp ^= 1;
	TXT[ TXT_WIDTH ].Disp ^= 1;
	TXT[ TXT_HEIGHT ].Disp ^= 1;
	POL[ POL_FRM ].Disp ^= 1;
	POL[ POL_SEL ].Disp ^= 1;
}

/******************************************************************************/
//	�f�o�b�O���b�Z�[�W������
/******************************************************************************/
void	DebugMsInit( void )
{
#ifdef	__DEBUG__

	for( long i=0; i<10; i++ )
	{
		g_LibText.Set( DB_MS+i, 128, 240+i*FONT_SIZE, 255, 255,0,0, "" );
	}

#endif
}

/******************************************************************************/
//	�f�o�b�O���b�Z�[�W������
/******************************************************************************/
void	DebugMsDisp( void )
{
#ifdef	__DEBUG__

	long	i;
	char	str[128],
			ms[10][128] =
			{
				"���C���[�ԍ��F",
				"�y�[�W�ԍ��F",
				"�`�b�v�ԍ��F",
				"���j���[�����蔻��F",
				"�`�b�v�����蔻��F",
			};

	long	dat[10] =
			{
				FLAG.Layer,
				FLAG.Page,
				FLAG.Chip,
				FLAG.HitMenu,
				FLAG.HitChipWin,
			};

	for( i=0; i<5; i++ )
	{
		sprintf_s( str, 128, "%d", dat[i] );
		strcat_s( ms[i], 128, str );
		sprintf_s( TXT[DB_MS+i].Str, MAX_PATH, ms[i] );
	}

	sprintf_s( TXT[ DB_MS+7 ].Str, MAX_PATH, "�t�@�C�������W:%d,%d", TXT[TXT_FILE_NAME].PosX, TXT[TXT_FILE_NAME].PosY );

#endif
}
