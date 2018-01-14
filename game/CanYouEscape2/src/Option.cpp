/******************************************************************************/
//	オプションメイン
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Obj.h"
#include	"./Option.h"

//----------------------------------------------------------------------------//
//	グローバル
//----------------------------------------------------------------------------//
long	OptionFlag;
short	IconPos;

double	IconDisp[][2] =
{
	//	[TIME]、[バズリ範囲]、[ハンディ]
	72,120,		//	0
	72,192,		//	1
	72,264,		//	2

	//	[60]、[120]、[∞]、
	336,112,	//	3
	416,112,	//	4
	512,112,	//	5

	//	[OFF]、[ON]
	336,192,	//	6
	440,192,	//	7

	//	[1P]、[2P]
	192,304,	//	8
	192,336,	//	9

	//	1P:[100]、[75]、[50]、[25]、[10]
	272,304,	//	10
	352,304,	//	11
	416,304,	//	12
	480,304,	//	13
	544,304,	//	14

	//	2P:[100]、[75]、[50]、[25]、[10]
	272,336,	//	15
	352,336,	//	16
	416,336,	//	17
	480,336,	//	18
	544,336,	//	19
};

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	OptionInit( void );	//	オプション初期化
void	OptionMain( void );	//	オプションメイン



/******************************************************************************/
//	オプション初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	OptionInit( void )
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

	/////////////////////////////////////////////////
	//	テクスチャ読み込み
	LoadTexture( TEX_OPTION_BG, "img\\sys\\option_bg.img" );
	LoadTexture( TEX_OPTION_SYS, "img\\sys\\system.img" );

	/////////////////////////////////////////////////
	//	パターンカット
	SetPattern( PAT_OPTION_BG, TEX_OPTION_BG, 0,0, 640,480 );
	SetPattern( PAT_OPTION_ICON, TEX_OPTION_SYS, 136,56, 8,8 );

	/////////////////////////////////////////////////
	//	オブジェクト
	ObjSet( OBJ_OPTION_BG, POL_OPTION_BG, PAT_OPTION_BG, 0,0,0, OBJ_TYPE_BG, OBJ_ATR_NONE );
	ObjSet( OBJ_OPTION_ICON, POL_OPTION_ICON, PAT_OPTION_ICON, IconDisp[0][0],IconDisp[0][1],0, OBJ_TYPE_BG, OBJ_ATR_NONE );

	OptionFlag = 0;
	IconPos = 0;

	/////////////////////////////////////////////////
	//	SEの読み込み
	g_LibMusic.Load( SE_CURSOR, "se\\se_cursor.wav" );
	g_LibMusic.Load( SE_BUT, "se\\se_button00.wav" );
	g_LibMusic.Load( SE_START, "se\\se_button01.wav" );
}

/******************************************************************************/
//	オプションメイン
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	OptionMain( void )
{
	switch( OptionFlag )
	{
		/////////////////////////////////////////////////
		//	[TIME]、[バズリ範囲]の選択
		case	0:
			if( PAD[0].Trg[ PAD_UP ] )
			{
				IconPos --;
			}
			else if( PAD[0].Trg[ PAD_DOWN ] )
			{
				IconPos ++;
			}

			if( IconPos < 0 )
			{
				IconPos = 2;
			}
			else if( IconPos > 2 )
			{
				IconPos = 0;
			}

			OBJ[ OBJ_OPTION_ICON ].px = IconDisp[IconPos][0];
			OBJ[ OBJ_OPTION_ICON ].py = IconDisp[IconPos][1];

			if( PAD[0].Trg[ PAD_BUT1 ] || PAD[1].Trg[ PAD_BUT1 ] )
			{
				OptionFlag = 1 + IconPos;

				if( OptionFlag == 1 )
				{
					/////////////////////////////////////////////////
					//	TIME
					if( SYS.time == 60 )
					{
						OBJ[ OBJ_OPTION_ICON ].px = IconDisp[3][0];
						OBJ[ OBJ_OPTION_ICON ].py = IconDisp[3][1];
						IconPos = 3;
					}
					else if( SYS.time == 120 )
					{
						OBJ[ OBJ_OPTION_ICON ].px = IconDisp[4][0];
						OBJ[ OBJ_OPTION_ICON ].py = IconDisp[4][1];
						IconPos = 4;
					}
					else if( SYS.time == -2 )
					{
						OBJ[ OBJ_OPTION_ICON ].px = IconDisp[5][0];
						OBJ[ OBJ_OPTION_ICON ].py = IconDisp[5][1];
						IconPos = 5;
					}
				}
				else if( OptionFlag == 2 )
				{
					/////////////////////////////////////////////////
					//	バズリ範囲
					if( SYS.buzz_disp == OFF )
					{
						OBJ[ OBJ_OPTION_ICON ].px = IconDisp[6][0];
						OBJ[ OBJ_OPTION_ICON ].py = IconDisp[6][1];
						IconPos = 6;
					}
					else
					{
						OBJ[ OBJ_OPTION_ICON ].px = IconDisp[7][0];
						OBJ[ OBJ_OPTION_ICON ].py = IconDisp[7][1];
						IconPos = 7;
					}
				}
				else if( OptionFlag == 3 )
				{
					/////////////////////////////////////////////////
					//	ハンディ
					OBJ[ OBJ_OPTION_ICON ].px = IconDisp[8][0];
					OBJ[ OBJ_OPTION_ICON ].py = IconDisp[8][1];
					IconPos = 8;
				}
			}

			break;

		/////////////////////////////////////////////////
		//	TIME
		case	1:
			if( PAD[0].Trg[ PAD_LEFT ] )
			{
				IconPos --;
			}
			else if( PAD[0].Trg[ PAD_RIGHT ] )
			{
				IconPos ++;
			}

			if( IconPos < 3 )
			{
				IconPos = 5;
			}
			else if( IconPos > 5 )
			{
				IconPos = 3;
			}

			OBJ[ OBJ_OPTION_ICON ].px = IconDisp[IconPos][0];
			OBJ[ OBJ_OPTION_ICON ].py = IconDisp[IconPos][1];

			if( PAD[0].Trg[ PAD_BUT1 ] || PAD[1].Trg[ PAD_BUT1 ] )
			{
				if( IconPos == 3 )
				{
					SYS.time = 60;
				}
				else if( IconPos == 4 )
				{
					SYS.time = 120;
				}
				else if( IconPos == 5 )
				{
					SYS.time = -2;
				}

				OptionFlag = 0;
				IconPos = 0;
			}
			break;

		/////////////////////////////////////////////////
		//	バズリ範囲
		case	2:
			if( PAD[0].Trg[ PAD_LEFT ] )
			{
				IconPos --;
			}
			if( PAD[0].Trg[ PAD_RIGHT ] )
			{
				IconPos ++;
			}
			if( IconPos < 6 )
			{
				IconPos = 7;
			}
			if( IconPos > 7 )
			{
				IconPos = 6;
			}

			OBJ[ OBJ_OPTION_ICON ].px = IconDisp[IconPos][0];
			OBJ[ OBJ_OPTION_ICON ].py = IconDisp[IconPos][1];

			if( PAD[0].Trg[ PAD_BUT1 ] || PAD[1].Trg[ PAD_BUT1 ] )
			{
				if( IconPos == 6 )
				{
					SYS.buzz_disp= OFF;
				}
				else if( IconPos == 7 )
				{
					SYS.buzz_disp= ON;
				}

				OptionFlag = 0;
				IconPos = 1;
			}
			break;

		/////////////////////////////////////////////////
		//	ハンディ
		case	3:
			if( PAD[0].Trg[ PAD_UP ] )
			{
				IconPos --;
			}
			else if( PAD[0].Trg[ PAD_DOWN ] )
			{
				IconPos ++;
			}

			if( IconPos < 8 )
			{
				IconPos = 9;
			}
			else if( IconPos > 9 )
			{
				IconPos = 8;
			}

			OBJ[ OBJ_OPTION_ICON ].px = IconDisp[IconPos][0];
			OBJ[ OBJ_OPTION_ICON ].py = IconDisp[IconPos][1];

			if( PAD[0].Trg[ PAD_BUT1 ] || PAD[1].Trg[ PAD_BUT1 ] )
			{
				if( IconPos == 8 )
				{
					if( SYS.life[0] == 1000 )
					{
						IconPos = 10;
					}
					else if( SYS.life[0] == 750 )
					{
						IconPos = 11;
					}
					else if( SYS.life[0] == 500 )
					{
						IconPos = 12;
					}
					else if( SYS.life[0] == 250 )
					{
						IconPos = 13;
					}
					else if( SYS.life[0] == 100 )
					{
						IconPos = 14;
					}

					OptionFlag = 4;
				}
				else if( IconPos == 9 )
				{
					if( SYS.life[1] == 1000 )
					{
						IconPos = 15;
					}
					else if( SYS.life[1] == 750 )
					{
						IconPos = 16;
					}
					else if( SYS.life[1] == 500 )
					{
						IconPos = 17;
					}
					else if( SYS.life[1] == 250 )
					{
						IconPos = 18;
					}
					else if( SYS.life[1] == 100 )
					{
						IconPos = 19;
					}

					OptionFlag = 5;
				}
			}

			if( PAD[0].Trg[ PAD_BUT2 ] )
			{
				OptionFlag = 0;
				IconPos = 2;
			}
			break;

		/////////////////////////////////////////////////
		//	1Pライフ
		case	4:
			if( PAD[0].Trg[ PAD_LEFT ] )
			{
				IconPos --;
			}
			else if( PAD[0].Trg[ PAD_RIGHT ] )
			{
				IconPos ++;
			}

			if( IconPos < 10 )
			{
				IconPos = 14;
			}
			else if( IconPos > 14 )
			{
				IconPos = 10;
			}

			OBJ[ OBJ_OPTION_ICON ].px = IconDisp[IconPos][0];
			OBJ[ OBJ_OPTION_ICON ].py = IconDisp[IconPos][1];

			if( PAD[0].Trg[ PAD_BUT1 ] || PAD[1].Trg[ PAD_BUT1 ] )
			{
				if( IconPos == 10 )
				{
					SYS.life[0] = 1000;
				}
				else if( IconPos == 11 )
				{
					SYS.life[0] = 750;
				}
				else if( IconPos == 12 )
				{
					SYS.life[0] = 500;
				}
				else if( IconPos == 13 )
				{
					SYS.life[0] = 250;
				}
				else if( IconPos == 14 )
				{
					SYS.life[0] = 100;
				}

				OptionFlag = 3;
				IconPos = 8;
			}
			break;

		/////////////////////////////////////////////////
		//	2Pライフ
		case	5:
			if( PAD[0].Trg[ PAD_LEFT ] )
			{
				IconPos --;
			}
			else if( PAD[0].Trg[ PAD_RIGHT ] )
			{
				IconPos ++;
			}

			if( IconPos < 15 )
			{
				IconPos = 19;
			}
			else if( IconPos > 19 )
			{
				IconPos = 15;
			}

			OBJ[ OBJ_OPTION_ICON ].px = IconDisp[IconPos][0];
			OBJ[ OBJ_OPTION_ICON ].py = IconDisp[IconPos][1];

			if( PAD[0].Trg[ PAD_BUT1 ] || PAD[1].Trg[ PAD_BUT1 ] )
			{
				if( IconPos == 15 )
				{
					SYS.life[1] = 1000;
				}
				else if( IconPos == 16 )
				{
					SYS.life[1] = 750;
				}
				else if( IconPos == 17 )
				{
					SYS.life[1] = 500;
				}
				else if( IconPos == 18 )
				{
					SYS.life[1] = 250;
				}
				else if( IconPos == 19 )
				{
					SYS.life[1] = 100;
				}

				OptionFlag = 3;
				IconPos = 9;
			}
			break;
	}

	if( PAD[0].Trg[ PAD_START ] )
	{
		SYS.main_mode = MAIN_MODE_TITLE_INIT;
	}
}
