/******************************************************************************/
//	バトルメイン
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/DX9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Obj.h"
#include	"./Battle.h"
#include	"./CharControl.h"

//----------------------------------------------------------------------------//
//	グローバル
//----------------------------------------------------------------------------//
long	Time[3];
short	BattleCt;
short	IconPosition;
char	AnmBuf[4][1024*1024];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	BattleInit( void );
void	BattleMain( void );

void	GageMain( void );
void	TimeMain( void );
void	BuzzRectDisp ( void );
void	PauseDisp( void );



/******************************************************************************/
//	バトル初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	BattleInit( void )
{
	/////////////////////////////////////////////////
	//	テクスチャ読み込み
	LoadTexture( TEX_GAME_BG, "img\\battle\\samp_bg.img" );
	LoadTexture( TEX_GAME_SYSTEM, "img\\sys\\system.img" );

	/////////////////////////////////////////////////
	//	パターン
	SetPattern( PAT_GAME_BG, TEX_GAME_BG,   0,0, 640,480 );
	SetPattern( PAT_GAME_GAGE_FRM, TEX_GAME_SYSTEM, 0,48, 56,24 );
	SetPattern( PAT_GAME_GAGE_FRM+1, TEX_GAME_SYSTEM, 56,48, 56,24 );
	SetPattern( PAT_GAME_PLAYER_NO, TEX_GAME_SYSTEM, 120,56, 8,16 );
	SetPattern( PAT_GAME_PLAYER_NO+1, TEX_GAME_SYSTEM, 128,56, 8,16 );
	SetPattern( PAT_GAME_GAGE_BAR+0, TEX_GAME_SYSTEM, 120,48, 50, 8 );
	SetPattern( PAT_GAME_GAGE_BAR+1, TEX_GAME_SYSTEM, 120,48, 50, 8 );
	SetPattern( PAT_GAME_PAUSE, TEX_GAME_SYSTEM, 0,80, 192,144 );
	SetPattern( PAT_GAME_ICON, TEX_GAME_SYSTEM, 136,56, 8,8 );

	for( int i=0; i<10; i++ )
	{
		//	0〜9
		SetPattern( PAT_GAME_CONTER + i, TEX_GAME_SYSTEM, i*24,0, 24,24 );
	}
	SetPattern( PAT_GAME_CONTER+10, TEX_GAME_SYSTEM, 240,0, 40,24 );

	/////////////////////////////////////////////////
	//	オブジェクト
	ObjSet( OBJ_GAME_BG, POL_GAME_BG, PAT_GAME_BG, 0,0,0.0, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_GAGE_FRM+0, POL_GAME_GAGE_FRM+0, PAT_GAME_GAGE_FRM+0, 10,451,0.2, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_GAGE_FRM+1, POL_GAME_GAGE_FRM+1, PAT_GAME_GAGE_FRM+1, 574,451,0.2, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_GAGE_BAR+0, POL_GAME_GAGE_BAR+0, PAT_GAME_GAGE_BAR+0, 10+3,451+8,0.1, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_GAGE_BAR+1, POL_GAME_GAGE_BAR+1, PAT_GAME_GAGE_BAR+1, 574+3,451+8,0.1, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_PLAYER_NO+0, POL_GAME_PLAYER_NO+0, PAT_GAME_PLAYER_NO+0, 10+8,443,0.2, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_PLAYER_NO+1, POL_GAME_PLAYER_NO+1, PAT_GAME_PLAYER_NO+1, 574+8,443,0.2, OBJ_TYPE_BG, ATR_NONE );

	ObjSet( OBJ_GAME_CONTER+0, POL_GAME_CONTER+0, PAT_GAME_CONTER, 320-24,10,0.0, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_CONTER+1, POL_GAME_CONTER+1, PAT_GAME_CONTER, 320-12,10,0.0, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_CONTER+2, POL_GAME_CONTER+2, PAT_GAME_CONTER, 320,10,0.1, OBJ_TYPE_BG, ATR_NONE );

	ObjSet( OBJ_GAME_PAUSE, POL_GAME_PAUSE, PAT_GAME_PAUSE, 234, 158, 1.0, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_GAME_ICON, POL_GAME_ICON, PAT_GAME_ICON, 254, 210,1.0, OBJ_TYPE_BG, ATR_NONE );

	ObjScaleSet( OBJ_GAME_BG, 2);

	/////////////////////////////////////////////////
	//	テキスト

	//	ライフ
	g_LibText.Set( 0, 10+24, 443, 255,255,255,225, "" );
	g_LibText.Set( 1, 574+24, 443, 255,255,255,225, "" );
	//	ゲージ
	g_LibText.Set( 5, 10, 451+8, 255,255,255,225, "" );
	g_LibText.Set( 6, 574, 451+8, 255,255,255,225, "" );

	BattleCt = SYS.time;
	SYS.win = NOT;
	IconPosition = OFF;
	SYS.pause_disp = OFF;

	g_LibText.Set( 100, 0,0, 255,255,0,0, "" );
	TXT[100].Disp = OFF;

	g_LibMusic.Load( BGM_BATTLE, "bgm\\bgm_battle.wav" );
	g_LibMusic.Play( BGM_BATTLE, LOOP );
}

/******************************************************************************/
//	バトルメイン
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	BattleMain( void )
{
//	sprintf( TXT[ 100 ].str, "0:%d  1:%d  2:%d  3:%d  4:%d", OBJ[OBJ_GAME_SHOT+0].disp,OBJ[OBJ_GAME_SHOT+1].disp,OBJ[OBJ_GAME_SHOT+2].disp,OBJ[OBJ_GAME_SHOT+3].disp,OBJ[OBJ_GAME_SHOT+4] );
	sprintf_s( TXT[0].Str, MAX_PATH, "%d", UNIT[0].life );
	sprintf_s( TXT[1].Str, MAX_PATH, "%d", UNIT[1].life );

	GageMain();
	TimeMain();
	BuzzRectDisp();
	PauseDisp();

//	デバッグ
#ifdef	__DEBUG__
	UNIT[0].gage = UNIT[1].gage = 300;
#endif
}

/******************************************************************************/
//	ゲージ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	GageMain( void )
{
	for( long i=0; i<2; i++ )
	{
		if( UNIT[ i ].gage > 300 )
		{
			UNIT[ i ].gage = 300;
		}
		else if( UNIT[ i ].gage < 0 )
		{
			UNIT[ i ].gage = 0;
		}
		PAT[ PAT_GAME_GAGE_BAR + i ].Width = short( UNIT[ i ].gage / 6 );
	}
}

/******************************************************************************/
//	タイム管理
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	TimeMain( void )
{
	short	time,max;

	if( BattleCt == -2 )
	{
		OBJ[ OBJ_GAME_CONTER+0 ].pat_no = PAT_GAME_CONTER+10;
		OBJ[ OBJ_GAME_CONTER+1 ].disp = OFF;
		OBJ[ OBJ_GAME_CONTER+2 ].disp = OFF;
	}
	else
	{
		time = BattleCt;
		max = 100;
		for( int i=0; i<3; i++ )
		{
			Time[ i ] = time / max;
			time %= max;
			max /= 10;
		}

		OBJ[ OBJ_GAME_CONTER+0 ].pat_no = PAT_GAME_CONTER+Time[0];
		OBJ[ OBJ_GAME_CONTER+1 ].pat_no = PAT_GAME_CONTER+Time[1];
		OBJ[ OBJ_GAME_CONTER+2 ].pat_no = PAT_GAME_CONTER+Time[2];
	}

	if( SYS.frame_ct >= 90 )
	{
		if( BattleCt == -1 )
		{
			BattleCt = 0;
			SYS.main_mode = MAIN_MODE_END_INIT;

			if( UNIT[0].life > UNIT[1].life )		SYS.win = 0;
			else if( UNIT[1].life > UNIT[0].life )	SYS.win = 1;
			else if( UNIT[0].life == UNIT[1].life ) SYS.win = 2;
		}
		else if( BattleCt == -2 )
		{
		}
		else
		{
			BattleCt --;
		}
		SYS.frame_ct = 0;
	}
	else
	{
		SYS.frame_ct ++;
	}
}

/******************************************************************************/
//	バズリ領域表示
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	BuzzRectDisp ( void )
{
	for( long i=0; i<2; i++ )
	{
		OBJ[ OBJ_GAME_BUZZ+i ].px = OBJ[ OBJ_GAME_UNIT+i ].px;
		OBJ[ OBJ_GAME_BUZZ+i ].py = OBJ[ OBJ_GAME_UNIT+i ].py;
		OBJ[ OBJ_GAME_BUZZ+i ].disp = SYS.buzz_disp;
	}

#ifdef	__DEBUG__
	if( KEY.trg[ DIK_F1 ] )
		SYS.buzz_disp ^= 1;
#endif
}

/******************************************************************************/
//	ポーズ画面表示
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	PauseDisp( void )
{
	double	pos[3] =
	{
		210,
		242,
		274,
	};

	OBJ[ OBJ_GAME_PAUSE ].disp = ON;
	OBJ[ OBJ_GAME_ICON ].disp = ON;

	for( long i=0 ;i<2; i++ )
	{
		if( PAD[i].Trg[ PAD_UP ] )
		{
			IconPosition --;
		}
		else if( PAD[i].Trg[ PAD_DOWN ] )
		{
			IconPosition ++;
		}

		if( PAD[ i ].Trg[ PAD_START ] )
		{
			if( SYS.pause_disp == 0 )
			{
				SYS.pause_disp = ON;
				break;
			}

			switch( IconPosition )
			{
				case	0:
					SYS.pause_disp = OFF;
					break;
				case	1:
					SYS.main_mode = MAIN_MODE_OPTION_INIT;
					break;
				case	2:
					SYS.main_mode = MAIN_MODE_TITLE_INIT;
					break;
			}
		}
	}

	if( !SYS.pause_disp )
	{
		OBJ[ OBJ_GAME_PAUSE ].disp = OFF;
		OBJ[ OBJ_GAME_ICON ].disp = OFF;
		return;
	}

	if( IconPosition < 0 )
	{
		IconPosition = 2;
	}
	else if( IconPosition> 2 )
	{
		IconPosition = 0;
	}

	OBJ[ OBJ_GAME_ICON ].py = pos[ IconPosition ];
}
