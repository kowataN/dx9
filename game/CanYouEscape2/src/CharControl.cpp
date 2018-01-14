/******************************************************************************/
//	キャラクター制御
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibANm.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Obj.h"
#include	"./CharControl.h"
#include	"./Shot.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	キャラクターパターン
UNIT_TBL	UNIT[2];

UNIT_TBL	UNIT_INIT[4] =
{
	//	obj, サイズ,      中心座標, HIT, バズリ, 接触, スピード, ショット番号
	{	NOT, 64,0,48,40,  24,17,    2,   30,     64,   4,        0,0,0,0,0 },	//	ユニット0
	{	NOT, 56,48,40,24, 20,11,    2,   30,     64,   5,        1,1,1,1,1 },	//	ユニット1
	{	NOT, 0,0,64,48,   32,29,    2,   30,     64,   4,        2,2,2,2,2 },	//	ユニット2
	{	NOT, 0,48,56,32,  28,15,    2,   30,     64,   3,        3,3,3,3,3 },	//	ユニット3

};

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
//	キャラクター初期化
void	CharInit( short char_no, short unit_no );
//	キャラクターメインループ
void	CharMainLoop( void );
//	キャラクター移動処理
void	CharMove( void );
//	キャラクターライフチェック
void	CharLifeChk( void );
//	キャラクター同士の当たり判定
void	CharHitChk( void );
//	向き判定
void	CharTurn( OBJ_TBL *obj1, OBJ_TBL *obj2 );
//	ショット
void	ChrShotLoop( void );
void	ChrShotMain( short no );



/******************************************************************************/
//	キャラクター初期化
//		戻り値：
//				なし
//		引数：
//				char_no		キャラクター番号
//				unit_no		ユニット番号
/******************************************************************************/
void	CharInit( short char_no, short unit_no )
{
	//	番号（オブジェクト、パターン）
	long obj = UNIT[ char_no ].obj_no = OBJ_GAME_UNIT + char_no;
	UNIT[ char_no ].pat = UNIT_INIT[ unit_no ].pat;

	//	中心座標
	UNIT[ char_no ].cx = UNIT_INIT[ unit_no ].cx;
	UNIT[ char_no ].cy = UNIT_INIT[ unit_no ].cy;

	//	その他
	UNIT[ char_no ].hit_rect = UNIT_INIT[ unit_no ].hit_rect;
	UNIT[ char_no ].buzz_rect = UNIT_INIT[ unit_no ].buzz_rect;
	UNIT[ char_no ].touch = UNIT_INIT[ unit_no ].touch;
	UNIT[ char_no ].speed = UNIT_INIT[ unit_no ].speed;

	UNIT[ char_no ].life = SYS.life[ char_no ];

	UNIT[ char_no ].gage = 100;

	for( long i=0; i<4; i++ )
	{
		UNIT[ char_no ].shot[ i ] = UNIT_INIT[ unit_no ].shot[ i ];
	}

	OBJ[ obj ].type = OBJ_TYPE_UNIT;

	if( char_no == 0 )
	{
		OBJ[ obj ].atr |= OBJ_ATR_UNIT1;
	}
	else
	{
		OBJ[ obj ].atr |= OBJ_ATR_UNIT2;
	}

	OBJ[ obj ].hit_rect = UNIT_INIT[ unit_no ].hit_rect;
	OBJ[ obj ].buzz_rect = UNIT_INIT[ unit_no ].buzz_rect;
}

/******************************************************************************/
//	キャラクターメインループ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharMainLoop( void )
{
	//	向きチェック
	CharTurn( &OBJ[OBJ_GAME_UNIT+0], &OBJ[OBJ_GAME_UNIT+1] );

	//	キャラ移動
	CharMove();

	//	ライフチェック
	CharLifeChk();

	//	キャラ同士のあたり判定
	CharHitChk();

	//	ショット
	ChrShotLoop();
}

/******************************************************************************/
//	キャラクター移動処理
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharMove( void )
{
	if( SYS.pause_disp )
	{
		return;
	}

	for( long i=0; i<2; i++ )
	{
		if( PAD[ i ].Use != ON )
		{
			continue;
		}

		/////////////////////////////////////////////////
		//	上
		if( PAD[ i ].Push[ PAD_UP ] )
		{
			/////////////////////////////////////////////////
			//	左上
			if( PAD[ i ].Push[ PAD_LEFT ] )
			{
				OBJ[ OBJ_GAME_UNIT + i ].mx = -UNIT[ i ].speed * 0.701;
				OBJ[ OBJ_GAME_UNIT + i ].my = -UNIT[ i ].speed * 0.701;
			}
			else
			{
				if( PAD[ i ].Push[ PAD_RIGHT ] )
				{
					/////////////////////////////////////////////////
					//	右上
					OBJ[ OBJ_GAME_UNIT + i ].mx = UNIT[ i ].speed * 0.701;
					OBJ[ OBJ_GAME_UNIT + i ].my = -UNIT[ i ].speed * 0.701;
				}
				else
				{
					OBJ[ OBJ_GAME_UNIT + i ].mx = 0;
					OBJ[ OBJ_GAME_UNIT + i ].my = -UNIT[ i ].speed;
				}
			}
		}
		else
		{
			/////////////////////////////////////////////////
			//	下
			if( PAD[ i ].Push[ PAD_DOWN ] )
			{
				/////////////////////////////////////////////////
				//	左下
				if( PAD[ i ].Push[ PAD_LEFT ] )
				{
					OBJ[ OBJ_GAME_UNIT + i ].mx = -UNIT[ i ].speed * 0.701;
					OBJ[ OBJ_GAME_UNIT + i ].my = UNIT[ i ].speed * 0.701;
				}
				else
				{
					if( PAD[ i ].Push[ PAD_RIGHT ] )
					{
						/////////////////////////////////////////////////
						//	右下
						OBJ[ OBJ_GAME_UNIT + i ].mx = UNIT[ i ].speed * 0.701;
						OBJ[ OBJ_GAME_UNIT + i ].my = UNIT[ i ].speed * 0.701;
					}
					else
					{
						OBJ[ OBJ_GAME_UNIT + i ].mx = 0;
						OBJ[ OBJ_GAME_UNIT + i ].my = UNIT[ i ].speed;
					}
				}
			}
			else
			{
				/////////////////////////////////////////////////
				//	左
				if( PAD[ i ].Push[ PAD_LEFT ] )
				{
					OBJ[ OBJ_GAME_UNIT + i ].mx = -UNIT[ i ].speed;
					OBJ[ OBJ_GAME_UNIT + i ].my = 0;
				}
				else
				{
					/////////////////////////////////////////////////
					//	右
					if( PAD[ i ].Push[ PAD_RIGHT ] )
					{
						OBJ[ OBJ_GAME_UNIT + i ].mx = UNIT[ i ].speed;
						OBJ[ OBJ_GAME_UNIT + i ].my = 0;
					}
					else
					{
						OBJ[ OBJ_GAME_UNIT + i ].mx = 0;
						OBJ[ OBJ_GAME_UNIT + i ].my = 0;
					}
				}
			}
		}
	}
}

/******************************************************************************/
//	キャラクターライフチェック
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharLifeChk( void )
{
	short	i;
	long	obj,ct;

	for( i=0; i<2; i++ )
	{
		if( UNIT[i].life <= 0 )
		{
//	デバッグ
#ifdef	__DEBUG__
			UNIT[i].life = 1000;
			break;
#endif
			obj = UNIT[i].obj_no;
			OBJ[obj].dead = ON;
			SYS.win = 1 - i;
		}
		else if( UNIT[i].life >= 1000 )
		{
			UNIT[i].life = 1000;
		}
	}

	ct = 0;
	for( i=0; i<2; i++ )
	{
		obj = UNIT[i].obj_no;
		if( OBJ[ obj ].dead == ON )
		{
			ct ++;
		}
	}

	if( ct == 2 )
	{
		SYS.win = 2;
	}
}


/******************************************************************************/
//	キャラクター同士の当たり判定
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharHitChk( void )
{
	short	i;
	double	x1,y1,x2,y2,w,h,len;

	x1 = OBJ[ OBJ_GAME_UNIT+0 ].px;
	y1 = OBJ[ OBJ_GAME_UNIT+0 ].py;
	x2 = OBJ[ OBJ_GAME_UNIT+1 ].px;
	y2 = OBJ[ OBJ_GAME_UNIT+1 ].py;

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
	len = ( w * w ) + ( h * h );

	if( len < (64*64) )
	{
		for( i=0; i<2; i++ )
		{
			OBJ[ OBJ_GAME_TOUCH + i ].px = OBJ[ OBJ_GAME_UNIT + i ].px;
			OBJ[ OBJ_GAME_TOUCH + i ].py = OBJ[ OBJ_GAME_UNIT + i ].py;
			OBJ[ OBJ_GAME_TOUCH + i ].disp = ON;
		}

		if( OBJ[ OBJ_GAME_UNIT ].px <= OBJ[ OBJ_GAME_UNIT+1 ].px )
		{
			OBJ[ OBJ_GAME_UNIT ].mx -= UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].mx += UNIT[ 1 ].speed;
		}
		else
		{
			OBJ[ OBJ_GAME_UNIT ].mx += UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].mx -= UNIT[ 1 ].speed;
		}

		if( OBJ[ OBJ_GAME_UNIT ].py <= OBJ[ OBJ_GAME_UNIT+1 ].py )
		{
			OBJ[ OBJ_GAME_UNIT ].my -= UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].my += UNIT[ 1 ].speed;
		}
		else
		{
			OBJ[ OBJ_GAME_UNIT ].my += UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].my -= UNIT[ 1 ].speed;
		}
	}
	else
	{
		OBJ[ OBJ_GAME_TOUCH+0 ].disp = OFF;
		OBJ[ OBJ_GAME_TOUCH+1 ].disp = OFF;
	}
}

/******************************************************************************/
//	向き判定
//		戻り値：
//				なし
//		引数：
//				*obj1	オブジェクトのアドレス（自分）
//				*obj2	オブジェクトのアドレス（相手）
/******************************************************************************/
void	CharTurn( OBJ_TBL *obj1, OBJ_TBL *obj2 )
{
	//	一時格納用
	double	x1,y1,x2,y2;

	x1 = obj1->px;
	y1 = obj1->py;
	x2 = obj2->px;
	y2 = obj2->py;

	obj1->rot_r = GetRot( x1,y1, x2,y2 );
	obj2->rot_r = GetRot( x2,y2, x1,y1 );
	obj1->rot_d = RadToDeg( obj1->rot_r ) + 90;
	obj2->rot_d = RadToDeg( obj2->rot_r ) + 90;

	obj1->rot = obj1->rot_d;
	obj2->rot = obj2->rot_d;
}

/******************************************************************************/
//	ショット
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ChrShotLoop( void )
{
	for( short i=0; i<2; i++ )
	{
		ChrShotMain( i );
	}
}

/******************************************************************************/
//	ショット管理
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
void	ChrShotMain( short no )
{
	short	ret;
	void(*func)(void);

	ret = NOT;

	//	ノーマルショット
	if( PAD[ no ].Trg[ PAD_BUT1 ] )
	{
		ret = 0;
	}
	else if( PAD[ no ].Rpt[ PAD_BUT1 ] )
	{
		ret = 1;
	}

	//	スペシャルショット１
	if( PAD[ no ].Trg[ PAD_BUT2 ] )
	{
		if( UNIT[ no ].gage >= 20 )
		{
			ret = 2;
			UNIT[no].gage -= 40;
			g_LibText.SetColor( 5+no, 255,0,255,255 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-20" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 30 );
		}
		else
		{
			g_LibText.SetColor( 5+no, 255,255,0,0 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 6 );
			g_LibMusic.Play( SE_RELOAD, ONE );
		}
	}
	else if( PAD[ no ].Trg[ PAD_BUT3 ] )
	{
		//	スペシャルショット２
		if( UNIT[ no ].gage >= 60 )
		{
			ret = 3;
			UNIT[no].gage -= 60;
			g_LibText.SetColor( 5+no, 255,0,255,255 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-60" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 30 );
		}
		else
		{
			g_LibText.SetColor( 5+no, 255,255,0,0 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 6 );
			g_LibMusic.Play( SE_RELOAD, ONE );
		}
	}
	else if( PAD[ no ].Trg[ PAD_BUT4 ] )
	{
		//	EXショット１
		if( UNIT[ no ].gage >= 80 )
		{
			ret = 4;
			UNIT[no].gage -= 80;
			g_LibText.SetColor( 5+no, 255,0,255,255 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-80" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 30 );
		}
		else
		{
			g_LibText.SetColor( 5+no, 255,255,0,0 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 6 );
			g_LibMusic.Play( SE_RELOAD, ONE );
		}
	}
	else if( PAD[ no ].Trg[ PAD_BUT5 ] )
	{
		////	EXショット２
		//if( UNIT[ no ].gage >= 160 )
		//{
		//	ret = 5;
		//	UNIT[no].gage -= 160;
		//	g_LibText.SetColor( 5+no, 255,0,255,255 );
		//	sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-160" );
		//	TXT[ 5+no ].Disp = ON;
		//	g_LibText.SetTimer( 5+no, 30 );
		//}
		//else
		//{
		//	g_LibText.SetColor( 5+no, 255,255,0,0 );
		//	sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
		//	TXT[ 5+no ].Disp = ON;
		//	g_LibText.SetTimer( 5+no, 6 );
		//	g_LibMusic.Play( SE_RELOAD, ONE );
		//}
	}

	if( ret < 0 )
	{
		return;
	}

	UNIT[ no ].but_no = ON;
	func = ( void(*)(void) )ShotTbl[ ret ];
	(*func)();
}
