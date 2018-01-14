/******************************************************************************/
//	�Q�[�����C��
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Title.h"
#include	"./Option.h"
#include	"./Obj.h"
#include	"./CharControl.h"
#include	"./Shot.h"
#include	"./Battle.h"
#include	"./End.h"
#include	"./CharSelect.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�V�X�e��
SYSTEM_TBL	SYS =
{
	//	�Q�[�����[�h�t���O
	MAIN_MODE_INIT,
//	MAIN_MODE_TITLE_INIT,
//	MAIN_MODE_OPTION_INIT,
//	MAIN_MODE_CHAR_SELECT_INIT,
//	MAIN_MODE_GAME_INIT,
//	MAIN_MODE_END_INIT,

	0,		//	�X�e�[�W�ԍ�
	NOT,	//	����
//	0,	//	����

	60,	//	TIME(-2:������)

	0,		//	�t���[���J�E���^
	OFF,	//	BUZZ�t���O
	OFF,	//	PAUSE�t���O

	1000,1000,	//	���C�t
//	0,0,	//	���C�t
};

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
void	MainInit( void );	//	���C��������
void	GameInit( void );	//	�Q�[��������
void	GameMain( void );	//	�Q�[�����C��
long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 );

//	�f�o�b�O
#ifdef	__DEBUG__
	void	DebugUnitChagne( void );
#endif



/******************************************************************************/
//	���C�����[�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	MainLoop( void )
{
	switch( SYS.main_mode )
	{
		/////////////////////////////////////////////////
		//	�@��������
		case	MAIN_MODE_INIT:
			MainInit();
			SYS.main_mode = MAIN_MODE_TITLE_INIT;

		/////////////////////////////////////////////////
		//	�^�C�g��������
		case	MAIN_MODE_TITLE_INIT:
			TitleInit();
			SYS.main_mode = MAIN_MODE_TITLE_MAIN;

		/////////////////////////////////////////////////
		//	�^�C�g�����C��
		case	MAIN_MODE_TITLE_MAIN:
			TitleMain();
			break;

		/////////////////////////////////////////////////
		//	�I�v�V����������
		case	MAIN_MODE_OPTION_INIT:
			OptionInit();
			SYS.main_mode = MAIN_MODE_OPTION_MAIN;

		/////////////////////////////////////////////////
		//	�I�v�V�������C��
		case	MAIN_MODE_OPTION_MAIN:
			OptionMain();
			break;

		/////////////////////////////////////////////////
		//	�L�����N�^�[�Z���N�g������
		case	MAIN_MODE_CHAR_SELECT_INIT:
			CharSelectInit();
			SYS.main_mode = MAIN_MODE_CHAR_SELECT_MAIN;

		/////////////////////////////////////////////////
		//	�L�����N�^�[�Z���N�g���C��
		case	MAIN_MODE_CHAR_SELECT_MAIN:
			CharSelectMain();
			break;

		/////////////////////////////////////////////////
		//	�Q�[��������
		case	MAIN_MODE_GAME_INIT:
			GameInit();
			SYS.main_mode = MAIN_MODE_GAME_MAIN;

		/////////////////////////////////////////////////
		//	�Q�[�����C��
		case	MAIN_MODE_GAME_MAIN:
			GameMain();
			break;

		/////////////////////////////////////////////////
		//	�I��������
		case	MAIN_MODE_END_INIT:
			EndInit();
			SYS.main_mode = MAIN_MODE_END_MAIN;

		/////////////////////////////////////////////////
		//	�I�����C��
		case	MAIN_MODE_END_MAIN:
			EndMain();
			break;
	}

	ObjMove();
	ObjDispAll();
	LoopAnm();
	LoopPad();
}

/******************************************************************************/
//	���C��������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	MainInit( void )
{
	/////////////////////////////////////////////////
	//	���C�u����������
	InitTextureAll();
	InitPatternAll();
	InitPolygonAll();
	g_LibText.InitAll();
	g_LibMusic.InitAll();
	g_LibFade.Init();

	InitAnmAll();
	InitPadAll();
}

/******************************************************************************/
//	�Q�[��������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	GameInit( void )
{
	/////////////////////////////////////////////////
	//	���C�u����������
	InitTextureAll();
	InitPatternAll();
	InitPolygonAll();
	g_LibText.InitAll();
	g_LibMusic.InitAll();
	g_LibFade.Init();

	InitAnmAll();
	InitPadAll();

	ObjInitAll();

	PAD[0].Use = ON;
	PAD[1].Use = ON;
	PAD[0].JoyUse = ON;

	/////////////////////////////////////////////////
	//	�L�����̏�����
	CharInit( 0, SelectFlag[0] );
	CharInit( 1, SelectFlag[1] );

	/////////////////////////////////////////////////
	//	�e�N�X�`���ǂݍ���
	LoadTexture( TEX_GAME_UNIT, "img\\char\\chr_samp.img" );
	LoadTexture( TEX_GAME_SHOT, "img\\battle\\shot.img" );

	/////////////////////////////////////////////////
	//	�p�^�[��
	short	rect[2][4];
	for( int i=0; i<2; i++ )
	{
		rect[i][0] = (short)UNIT[i].pat.left;
		rect[i][1] = (short)UNIT[i].pat.top;
		rect[i][2] = (short)UNIT[i].pat.right;
		rect[i][3] = (short)UNIT[i].pat.bottom;
	}

	//	���j�b�g
	SetPattern( PAT_GAME_UNIT+0, TEX_GAME_UNIT, rect[0][0],rect[0][1], rect[0][2],rect[0][3] );
	SetPattern( PAT_GAME_UNIT+1, TEX_GAME_UNIT, rect[1][0],rect[1][1], rect[1][2],rect[1][3] );

	SetPattern( PAT_GAME_TOUCH, TEX_GAME_SHOT, 32,0,64,64 );	//	�ڐG�͈�
	SetPattern( PAT_GAME_BUZZ,  TEX_GAME_SHOT, 0,32, 29,29 );	//	�o�Y���͈�

	//	�V���g
	SetPattern( PAT_GAME_SHOT+0, TEX_GAME_SHOT, 0,0, 16,32 );
	SetPattern( PAT_GAME_SHOT+1, TEX_GAME_SHOT, 16,0, 16,32 );

	SetPattern( PAT_GAME_SHOT+2, TEX_GAME_SHOT, 48,64, 16,16 );
	SetPattern( PAT_GAME_SHOT+3, TEX_GAME_SHOT, 64,64, 16,16 );
	SetPattern( PAT_GAME_SHOT+4, TEX_GAME_SHOT, 80,64, 16,16 );

	SetPattern( PAT_GAME_SHOT+5, TEX_GAME_SHOT, 0,64, 16,16 );

	/////////////////////////////////////////////////
	//	�I�u�W�F�N�g
	ObjSet( OBJ_GAME_UNIT+0,  POL_GAME_UNIT+0,  PAT_GAME_UNIT+0, 160,240,0.5, OBJ_TYPE_UNIT, OBJ_ATR_NONE );
	ObjSet( OBJ_GAME_UNIT+1,  POL_GAME_UNIT+1,  PAT_GAME_UNIT+1, 480,240,0.5, OBJ_TYPE_UNIT, OBJ_ATR_NONE );
	ObjCenterSet( OBJ_GAME_UNIT+0, UNIT[0].cx,UNIT[0].cy );
	ObjCenterSet( OBJ_GAME_UNIT+1, UNIT[1].cx,UNIT[1].cy );

	ObjSet( OBJ_GAME_TOUCH+0, POL_GAME_TOUCH+0, PAT_GAME_TOUCH,  160,240,0.8, OBJ_TYPE_RECT, OBJ_ATR_NONE );
	ObjSet( OBJ_GAME_TOUCH+1, POL_GAME_TOUCH+1, PAT_GAME_TOUCH,  480,240,0.8, OBJ_TYPE_RECT, OBJ_ATR_NONE );
	ObjCenterSet( OBJ_GAME_TOUCH+0,  31, 31 );
	ObjCenterSet( OBJ_GAME_TOUCH+1,  31, 31 );

	ObjSet( OBJ_GAME_BUZZ+0, POL_GAME_BUZZ+0, PAT_GAME_BUZZ,   160,240,1.0, OBJ_TYPE_RECT, OBJ_ATR_NONE );
	ObjSet( OBJ_GAME_BUZZ+1, POL_GAME_BUZZ+1, PAT_GAME_BUZZ,   480,240,1.0, OBJ_TYPE_RECT, OBJ_ATR_NONE );
	ObjCenterSet( OBJ_GAME_BUZZ+0, 15, 15 );
	ObjCenterSet( OBJ_GAME_BUZZ+1, 15, 15 );
	SetPolygonColor( POL_GAME_BUZZ+0, 128,255,0,0 );
	SetPolygonColor( POL_GAME_BUZZ+1, 128,0,0,255 );
	OBJ[ OBJ_GAME_BUZZ+0 ].disp = SYS.buzz_disp;
	OBJ[ OBJ_GAME_BUZZ+1 ].disp = SYS.buzz_disp;

	/////////////////////////////////////////////////
	//	SE�̓ǂݍ���
	g_LibMusic.Load( SE_CURSOR, "se\\se_cursor.wav" );
	g_LibMusic.Load( SE_BUT, "se\\se_button00.wav" );
	g_LibMusic.Load( SE_START, "se\\se_button01.wav" );
	g_LibMusic.Load( SE_RELOAD, "se\\se_reload.wav" );

	/////////////////////////////////////////////////
	//	���C�t
	g_LibText.Set( 10, 0,0, 255,255,0,0, "" );
	g_LibText.Set( 11, 0,16, 255,255,0,0, "" );
	TXT[10].Disp = OFF;
	TXT[11].Disp = OFF;
	BattleInit();
}

/******************************************************************************/
//	�Q�[�����C��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	GameMain( void )
{
	CharMainLoop();
	BattleMain();
	ShotLoop();

	if( SYS.win != NOT )
	{
		SYS.main_mode = MAIN_MODE_END_INIT;
	}

	if( KEY.Trg[DIK_G] )
	{
		UNIT[0].gage = 300;
		UNIT[1].gage = 300;
	}

//	�f�o�b�O
#ifdef	__DEBUG__
	DebugUnitChagne();
#endif
}

/******************************************************************************/
//	�����蔻��
//		�߂�l�F
//				�O�F���ڐG�A�P�F�ڐG
//		�����F
//				x1,y1	���W�P
//				r1		���a�P
//				x2,y2	���W�Q
//				r2		���a�Q
/******************************************************************************/
long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 )
{
	double	w,h,len,r;

	//	��
	if( x1 > x2 )
	{
		w = x1 - x2;
	}
	else
	{
		w = x2 - x1;
	}

	//	�c
	if( y1 > y2 )
	{
		h = y1 - y2;
	}
	else
	{
		h = y2 - y1;
	}

	//	����
	len = sqrt( ( w * w ) + ( h * h ) );

	//	���a
	r = (r1/2) * (r2/2);


	if( len < r )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//	�f�o�b�O
#ifdef	__DEBUG__
void	DebugUnitChagne( void )
{
	if( KEY.trg[ DIK_1 ] || KEY.trg[ DIK_2 ] || KEY.trg[ DIK_3 ] || KEY.trg[ DIK_4 ] )
	{
		if( KEY.Trg[ DIK_1 ] )	CharInit( 0, 0 );
		if( KEY.Trg[ DIK_2 ] )	CharInit( 0, 1 );
		if( KEY.Trg[ DIK_3 ] )	CharInit( 0, 2 );
		if( KEY.Trg[ DIK_4 ] )	CharInit( 0, 3 );

		PAT[ OBJ[ UNIT[0].obj_no ].pat_no ].x = (short)UNIT[0].pat.left;
		PAT[ OBJ[ UNIT[0].obj_no ].pat_no ].y = (short)UNIT[0].pat.top;
		PAT[ OBJ[ UNIT[0].obj_no ].pat_no ].w = (short)UNIT[0].pat.right;
		PAT[ OBJ[ UNIT[0].obj_no ].pat_no ].h = (short)UNIT[0].pat.bottom;
		ObjCenterSet( UNIT[0].obj_no, UNIT[0].cx,UNIT[0].cy );
	}


	if( KEY.trg[ DIK_F5 ] || KEY.trg[ DIK_F6 ] || KEY.trg[ DIK_F7 ] || KEY.trg[ DIK_F8 ] )
	{
		if( KEY.Trg[ DIK_F5 ] )	CharInit( 1, 0 );
		if( KEY.Trg[ DIK_F6 ] )	CharInit( 1, 1 );
		if( KEY.Trg[ DIK_F7 ] )	CharInit( 1, 2 );
		if( KEY.Trg[ DIK_F8 ] )	CharInit( 1, 3 );

		PAT[ OBJ[ UNIT[1].obj_no ].pat_no ].x = (short)UNIT[1].pat.left;
		PAT[ OBJ[ UNIT[1].obj_no ].pat_no ].y = (short)UNIT[1].pat.top;
		PAT[ OBJ[ UNIT[1].obj_no ].pat_no ].w = (short)UNIT[1].pat.right;
		PAT[ OBJ[ UNIT[1].obj_no ].pat_no ].h = (short)UNIT[1].pat.bottom;
		ObjCenterSet( UNIT[1].obj_no, UNIT[1].cx,UNIT[1].cy );
	}
}
#endif
