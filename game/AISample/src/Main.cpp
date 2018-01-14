/******************************************************************************/
//	���C���\�[�X
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#ifdef	_DEBUG
#define	_CRTDBG_MAP_ALLOC
#endif

#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"
#include	"./ScriptEngine.h"
#include	"./Main.h"
#include	"./CharControl.h"
#include	"./EnemyList.h"
#include	"./ObjectMng.h"
#include	"./ShotMng.h"

#ifdef	_DEBUG
#include	<crtdbg.h>
#define	new ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif	//	_DEBUG

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
//	�V�X�e��
SYSTEM_TBL	SYS =
{
	//	�Q�[�����[�h�t���O
	MAIN_MODE_INIT,

	0,		//	�X�e�[�W�ԍ�

	60,	//	TIME(-2:������)

	0,		//	�t���[���J�E���^
	OFF,	//	PAUSE�t���O

	1000	//	���C�t
};

CEnemyList	Enemy;
long	Time = 0;

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
void	MainInit( void );					//	���C��������
void	GameMain( void );					//	�Q�[�����C��
long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 );



/******************************************************************************/
//	���C�����[�`��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	MainLoop( void )
{
#ifdef	_DEBUG
	//	���������[�N���o�ݒ�
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	switch( SYS.MainMode )
	{
		case	0:
			MainInit();
			SYS.MainMode = 1;

		case	1:
			GameMain();
			break;
	}
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif	//	_DEBUG
}

/******************************************************************************/
//	�N��������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	MainInit( void )
{
	CObjectMng	&Obj = CObjectMng::GetInstance();
	Dx9LibInit();
	InitPadAll();
	PAD[0].Use = ON;

	srand( (unsigned)time(NULL) );

	Obj.Clear();

	LoadTexture( TEX_GAME_SHOT, "data\\shot.bmp" );

	//	�V���g
	SetPattern( PAT_GAME_SHOT+0, TEX_GAME_SHOT, 0,0, 16,32 );
	SetPattern( PAT_GAME_SHOT+1, TEX_GAME_SHOT, 16,0, 16,32 );
	SetPattern( PAT_GAME_SHOT+2, TEX_GAME_SHOT, 48,64, 16,16 );
	SetPattern( PAT_GAME_SHOT+3, TEX_GAME_SHOT, 64,64, 16,16 );
	SetPattern( PAT_GAME_SHOT+4, TEX_GAME_SHOT, 80,64, 16,16 );
	SetPattern( PAT_GAME_SHOT+5, TEX_GAME_SHOT, 0,64, 16,16 );

	LoadTexture( TEX_GAME_UNIT, "Data\\CharSamp.bmp" );
	SetPattern( PAT_GAME_UNIT,TEX_GAME_UNIT, 64, 0, 48, 48 );

	Obj.Set( OBJ_GAME_BG, 440,440, 320,240,0.1, g_ObjType.BG, OBJ_ATR_NONE );
	Obj.SetCenter( OBJ_GAME_BG, 220,220 );
	//Obj.Set( OBJ_GAME_UNIT, PAT_GAME_UNIT, 320,400,0.5, g_ObjType.Player, OBJ_ATR_UNIT );
	//Obj.SetCenter( OBJ_GAME_UNIT, 24,24 );

	//CharInit();
	Enemy.Init( 0, OBJ_GAME_UNIT, OBJ_GAME_ENEMY, 320,400 );
	Enemy.Init( 1, OBJ_GAME_ENEMY, OBJ_GAME_UNIT, 320,200 );

	Time = 0;
}

/******************************************************************************/
//	�Q�[�����C�����[�`��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	GameMain( void )
{
	//if( UNIT.Life <= 0 )
	//{
	//	g_LibText.Set( 1, 320,0, 255,255,255,255, "GameOver" );
	//	if( KEY.Trg[ DIK_SPACE ] )
	//	{
	//		SYS.MainMode = 0;
	//	}

	//	return;
	//}

	//CharMainLoop();
	Enemy.MainLoop();

	ShotLoop();
	LoopPad();
	CObjectMng::GetInstance().Loop();

	//Time ++;
	//if( Time == 5*60 )
	//{
	//	Enemy.Init( 1, OBJ_GAME_ENEMY + 1 );
	//}
	//if( Time == 10*60 )
	//{
	//	Enemy.Init( 2, OBJ_GAME_ENEMY + 2 );
	//}
	//if( Time == 15*60 )
	//{
	//	Enemy.Init( 3, OBJ_GAME_ENEMY + 3 );
	//}

	//g_LibText.Set( 0, 0,0, 255,255,0,0, "" );
	//sprintf_s( TXT[0].Str, "�o�ߎ���:%d", Time/60 );
	//g_LibText.Set( 1, 0,FONT_SIZE, 255,255,0,0, "" );
	//sprintf_s( TXT[1].Str, "���C�t:%d", UNIT.Life );
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
