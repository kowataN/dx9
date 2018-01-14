/******************************************************************************/
//	メインソース
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
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
//	グローバル変数
//----------------------------------------------------------------------------//
//	システム
SYSTEM_TBL	SYS =
{
	//	ゲームモードフラグ
	MAIN_MODE_INIT,

	0,		//	ステージ番号

	60,	//	TIME(-2:無制限)

	0,		//	フレームカウンタ
	OFF,	//	PAUSEフラグ

	1000	//	ライフ
};

CEnemyList	Enemy;
long	Time = 0;

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	MainInit( void );					//	メイン初期化
void	GameMain( void );					//	ゲームメイン
long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 );



/******************************************************************************/
//	メインルーチン
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	MainLoop( void )
{
#ifdef	_DEBUG
	//	メモリリーク検出設定
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
//	起動初期化
//		戻り値：
//				なし
//		引数：
//				なし
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

	//	ショト
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
//	ゲームメインルーチン
//		戻り値：
//				なし
//		引数：
//				なし
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
	//sprintf_s( TXT[0].Str, "経過時間:%d", Time/60 );
	//g_LibText.Set( 1, 0,FONT_SIZE, 255,255,0,0, "" );
	//sprintf_s( TXT[1].Str, "ライフ:%d", UNIT.Life );
}

/******************************************************************************/
//	当たり判定
//		戻り値：
//				０：未接触、１：接触
//		引数：
//				x1,y1	座標１
//				r1		直径１
//				x2,y2	座標２
//				r2		直径２
/******************************************************************************/
long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 )
{
	double	w,h,len,r;

	//	横
	if( x1 > x2 )
	{
		w = x1 - x2;
	}
	else
	{
		w = x2 - x1;
	}

	//	縦
	if( y1 > y2 )
	{
		h = y1 - y2;
	}
	else
	{
		h = y2 - y1;
	}

	//	距離
	len = sqrt( ( w * w ) + ( h * h ) );

	//	半径
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
