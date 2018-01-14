/******************************************************************************/
//	ゲームメイン
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
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
//	グローバル変数
//----------------------------------------------------------------------------//

//	システム
SYSTEM_TBL	SYS =
{
	//	ゲームモードフラグ
	MAIN_MODE_INIT,
//	MAIN_MODE_TITLE_INIT,
//	MAIN_MODE_OPTION_INIT,
//	MAIN_MODE_CHAR_SELECT_INIT,
//	MAIN_MODE_GAME_INIT,
//	MAIN_MODE_END_INIT,

	0,		//	ステージ番号
	NOT,	//	勝者
//	0,	//	勝者

	60,	//	TIME(-2:無制限)

	0,		//	フレームカウンタ
	OFF,	//	BUZZフラグ
	OFF,	//	PAUSEフラグ

	1000,1000,	//	ライフ
//	0,0,	//	ライフ
};

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	MainInit( void );	//	メイン初期化
void	GameInit( void );	//	ゲーム初期化
void	GameMain( void );	//	ゲームメイン
long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 );

//	デバッグ
#ifdef	__DEBUG__
	void	DebugUnitChagne( void );
#endif



/******************************************************************************/
//	メインループ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	MainLoop( void )
{
	switch( SYS.main_mode )
	{
		/////////////////////////////////////////////////
		//	機動初期化
		case	MAIN_MODE_INIT:
			MainInit();
			SYS.main_mode = MAIN_MODE_TITLE_INIT;

		/////////////////////////////////////////////////
		//	タイトル初期化
		case	MAIN_MODE_TITLE_INIT:
			TitleInit();
			SYS.main_mode = MAIN_MODE_TITLE_MAIN;

		/////////////////////////////////////////////////
		//	タイトルメイン
		case	MAIN_MODE_TITLE_MAIN:
			TitleMain();
			break;

		/////////////////////////////////////////////////
		//	オプション初期化
		case	MAIN_MODE_OPTION_INIT:
			OptionInit();
			SYS.main_mode = MAIN_MODE_OPTION_MAIN;

		/////////////////////////////////////////////////
		//	オプションメイン
		case	MAIN_MODE_OPTION_MAIN:
			OptionMain();
			break;

		/////////////////////////////////////////////////
		//	キャラクターセレクト初期化
		case	MAIN_MODE_CHAR_SELECT_INIT:
			CharSelectInit();
			SYS.main_mode = MAIN_MODE_CHAR_SELECT_MAIN;

		/////////////////////////////////////////////////
		//	キャラクターセレクトメイン
		case	MAIN_MODE_CHAR_SELECT_MAIN:
			CharSelectMain();
			break;

		/////////////////////////////////////////////////
		//	ゲーム初期化
		case	MAIN_MODE_GAME_INIT:
			GameInit();
			SYS.main_mode = MAIN_MODE_GAME_MAIN;

		/////////////////////////////////////////////////
		//	ゲームメイン
		case	MAIN_MODE_GAME_MAIN:
			GameMain();
			break;

		/////////////////////////////////////////////////
		//	終了初期化
		case	MAIN_MODE_END_INIT:
			EndInit();
			SYS.main_mode = MAIN_MODE_END_MAIN;

		/////////////////////////////////////////////////
		//	終了メイン
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
//	メイン初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	MainInit( void )
{
	/////////////////////////////////////////////////
	//	ライブラリ初期化
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
//	ゲーム初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	GameInit( void )
{
	/////////////////////////////////////////////////
	//	ライブラリ初期化
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
	//	キャラの初期化
	CharInit( 0, SelectFlag[0] );
	CharInit( 1, SelectFlag[1] );

	/////////////////////////////////////////////////
	//	テクスチャ読み込み
	LoadTexture( TEX_GAME_UNIT, "img\\char\\chr_samp.img" );
	LoadTexture( TEX_GAME_SHOT, "img\\battle\\shot.img" );

	/////////////////////////////////////////////////
	//	パターン
	short	rect[2][4];
	for( int i=0; i<2; i++ )
	{
		rect[i][0] = (short)UNIT[i].pat.left;
		rect[i][1] = (short)UNIT[i].pat.top;
		rect[i][2] = (short)UNIT[i].pat.right;
		rect[i][3] = (short)UNIT[i].pat.bottom;
	}

	//	ユニット
	SetPattern( PAT_GAME_UNIT+0, TEX_GAME_UNIT, rect[0][0],rect[0][1], rect[0][2],rect[0][3] );
	SetPattern( PAT_GAME_UNIT+1, TEX_GAME_UNIT, rect[1][0],rect[1][1], rect[1][2],rect[1][3] );

	SetPattern( PAT_GAME_TOUCH, TEX_GAME_SHOT, 32,0,64,64 );	//	接触範囲
	SetPattern( PAT_GAME_BUZZ,  TEX_GAME_SHOT, 0,32, 29,29 );	//	バズリ範囲

	//	ショト
	SetPattern( PAT_GAME_SHOT+0, TEX_GAME_SHOT, 0,0, 16,32 );
	SetPattern( PAT_GAME_SHOT+1, TEX_GAME_SHOT, 16,0, 16,32 );

	SetPattern( PAT_GAME_SHOT+2, TEX_GAME_SHOT, 48,64, 16,16 );
	SetPattern( PAT_GAME_SHOT+3, TEX_GAME_SHOT, 64,64, 16,16 );
	SetPattern( PAT_GAME_SHOT+4, TEX_GAME_SHOT, 80,64, 16,16 );

	SetPattern( PAT_GAME_SHOT+5, TEX_GAME_SHOT, 0,64, 16,16 );

	/////////////////////////////////////////////////
	//	オブジェクト
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
	//	SEの読み込み
	g_LibMusic.Load( SE_CURSOR, "se\\se_cursor.wav" );
	g_LibMusic.Load( SE_BUT, "se\\se_button00.wav" );
	g_LibMusic.Load( SE_START, "se\\se_button01.wav" );
	g_LibMusic.Load( SE_RELOAD, "se\\se_reload.wav" );

	/////////////////////////////////////////////////
	//	ライフ
	g_LibText.Set( 10, 0,0, 255,255,0,0, "" );
	g_LibText.Set( 11, 0,16, 255,255,0,0, "" );
	TXT[10].Disp = OFF;
	TXT[11].Disp = OFF;
	BattleInit();
}

/******************************************************************************/
//	ゲームメイン
//		戻り値：
//				なし
//		引数：
//				なし
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

//	デバッグ
#ifdef	__DEBUG__
	DebugUnitChagne();
#endif
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

//	デバッグ
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
