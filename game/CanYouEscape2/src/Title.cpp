/******************************************************************************/
//	タイトルメイン
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
#include	"./Obj.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	タイトルモードフラグ
long	TitleFlag,TitleEndFlag=0;
long	TitleAlpha,TitleAdd;

/////////////////////////////////////////////////
//	画像ファイル名
char	FileName[][128]=
{
	"img\\sys\\title_bg.img",
	"img\\sys\\sys_title.img",
};

/////////////////////////////////////////////////
//	パターンサイズ
short	PatDat[][4] =
{
	0,0,253,44,		//	[CAN YOU]
	0,44,244,42,	//	[ESCAPE2]
	0,86,218,27,	//	[PUSH START]
	0,120,200,24,	//	[GAME START]
	0,144,120,24,	//	[OPTION]
};

/////////////////////////////////////////////////
//	表示座標
double	PosTbl[][2]=
{
	110,110,	//	[CAN YOU]
	313,195,	//	[ESCAPE2]
	320,380,	//	[PUSH START]
	320,380,	//	[GAME START]
	320,422,	//	[OPTION]
};

double	py1,py2,ph1,ph2;
long	AnimeFlag = 0;

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	TitleInit( void );	//	タイトル初期化
short	TitleMain( void );	//	タイトルメイン



/******************************************************************************/
//	タイトル初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	TitleInit( void )
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

	/////////////////////////////////////////////////
	//	画像読み込み
	LoadTexture( TEX_TITLE_BG, FileName[0] );
	LoadTexture( TEX_TITLE_SYS, FileName[1] );

	/////////////////////////////////////////////////
	//	パターンカット
	SetPattern( PAT_TITLE_BG, TEX_TITLE_BG, 320,240, 0,0 );
	SetPattern( PAT_TITLE_TEXT1, TEX_TITLE_SYS, PatDat[0][0],PatDat[0][1]+PatDat[0][3], PatDat[0][2],0 );
	SetPattern( PAT_TITLE_TEXT2, TEX_TITLE_SYS, PatDat[1][0],PatDat[1][1]+PatDat[1][3], PatDat[1][2],0 );
	SetPattern( PAT_TITLE_TEXT3, TEX_TITLE_SYS, PatDat[2][0],PatDat[2][1], PatDat[2][2],PatDat[2][3] );
	SetPattern( PAT_TITLE_TEXT4, TEX_TITLE_SYS, PatDat[3][0],PatDat[3][1], PatDat[3][2],PatDat[3][3] );
	SetPattern( PAT_TITLE_TEXT5, TEX_TITLE_SYS, PatDat[4][0],PatDat[4][1], PatDat[4][2],PatDat[4][3] );

	/////////////////////////////////////////////////
	//	オブジェクト
	ObjSet( OBJ_TITLE_BG,    POL_TITLE_BG,    PAT_TITLE_BG,    320,240,1.0, OBJ_TYPE_BG, OBJ_ATR_NONE );
	ObjSet( OBJ_TITLE_TEXT1, POL_TITLE_TEXT1, PAT_TITLE_TEXT1, PosTbl[0][0],PosTbl[0][1]+PatDat[0][3],1.0, OBJ_TYPE_BG, OBJ_ATR_NONE );
	ObjSet( OBJ_TITLE_TEXT2, POL_TITLE_TEXT2, PAT_TITLE_TEXT2, PosTbl[1][0],PosTbl[1][1]+PatDat[1][3],1.0, OBJ_TYPE_BG, OBJ_ATR_NONE );
	ObjSet( OBJ_TITLE_TEXT3, POL_TITLE_TEXT3, PAT_TITLE_TEXT3, PosTbl[2][0],PosTbl[2][1],1.0, OBJ_TYPE_BG, OBJ_ATR_NONE );
	ObjSet( OBJ_TITLE_TEXT4, POL_TITLE_TEXT4, PAT_TITLE_TEXT4, PosTbl[3][0],PosTbl[3][1],1.0, OBJ_TYPE_BG, OBJ_ATR_NONE );
	ObjSet( OBJ_TITLE_TEXT5, POL_TITLE_TEXT5, PAT_TITLE_TEXT5, PosTbl[4][0],PosTbl[4][1],1.0, OBJ_TYPE_BG, OBJ_ATR_NONE );

	ObjCenterSet( OBJ_TITLE_TEXT3, 109,14 );
	ObjCenterSet( OBJ_TITLE_TEXT4, 100,12 );
	ObjCenterSet( OBJ_TITLE_TEXT5, 60,12 );

	OBJ[ OBJ_TITLE_TEXT4 ].disp = OFF;
	OBJ[ OBJ_TITLE_TEXT5 ].disp = OFF;

	PAD[0].Use = ON;
	PAD[1].Use = ON;

	if( TitleEndFlag )
	{
		SetPattern( PAT_TITLE_BG, TEX_TITLE_BG, 0,0, 640,480 );
		SetPattern( PAT_TITLE_TEXT1, TEX_TITLE_SYS, PatDat[0][0],PatDat[0][1], PatDat[0][2],PatDat[0][3] );
		SetPattern( PAT_TITLE_TEXT2, TEX_TITLE_SYS, PatDat[1][0],PatDat[1][1], PatDat[1][2],PatDat[1][3] );
		ObjPosSet( OBJ_TITLE_BG, 0,0,1.0 );
		OBJ[ OBJ_TITLE_TEXT1 ].disp = ON;
		OBJ[ OBJ_TITLE_TEXT2 ].disp = ON;
		TitleFlag = 2;
	}
	else
	{
		TitleFlag = 0;
	}

	TitleAlpha = OFF;
	TitleAdd = 0;

	/////////////////////////////////////////////////
	//	SEの読み込み
	g_LibMusic.Load( SE_CURSOR, "se\\se_cursor.wav" );
	g_LibMusic.Load( SE_BUT, "se\\se_button00.wav" );
	g_LibMusic.Load( SE_START, "se\\se_button01.wav" );
}

/******************************************************************************/
//	タイトルメイン
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
short	TitleMain( void )
{
	short	ret = 0;

	switch( TitleFlag )
	{
		/////////////////////////////////////////////////
		//	背景出現
		case	0:
			PAT[ PAT_TITLE_BG ].PosX -= WIDTH / TITLE_TIME / 2;
			PAT[ PAT_TITLE_BG ].Width += WIDTH / TITLE_TIME;
			OBJ[ OBJ_TITLE_BG ].px -= WIDTH / TITLE_TIME / 2;
			PAT[ PAT_TITLE_BG ].PosY -= HEIGHT / TITLE_TIME / 2;
			PAT[ PAT_TITLE_BG ].Height += HEIGHT / TITLE_TIME;
			OBJ[ OBJ_TITLE_BG ].py -= HEIGHT / TITLE_TIME / 2;

			if( ( OBJ[ OBJ_TITLE_BG ].px <= 0 )
			|| ( OBJ[ OBJ_TITLE_BG ].py <= 0 ) )
			{
				PAT[ PAT_TITLE_BG ].PosX = 0;
				PAT[ PAT_TITLE_BG ].Width = WIDTH;
				OBJ[ OBJ_TITLE_BG ].px = 0;
				PAT[ PAT_TITLE_BG ].PosY = 0;
				PAT[ PAT_TITLE_BG ].Height = HEIGHT;
				OBJ[ OBJ_TITLE_BG ].py = 0;
				TitleFlag = 1;

				py1 = PAT[ PAT_TITLE_TEXT1 ].PosY;
				ph1 = PAT[ PAT_TITLE_TEXT1 ].Height;
				py2 = PAT[ PAT_TITLE_TEXT2 ].PosY;
				ph2 = PAT[ PAT_TITLE_TEXT2 ].Height;
			}

			break;

		/////////////////////////////////////////////////
		//	テキスト出現
		case	1:
			py1 -= (double)PatDat[0][3] / TITLE_TIME;
			ph1 += (double)PatDat[0][3] / TITLE_TIME;
			PAT[ PAT_TITLE_TEXT1 ].PosY = (short)py1;
			PAT[ PAT_TITLE_TEXT1 ].Height = (short)ph1;
			OBJ[ OBJ_TITLE_TEXT1 ].py = PosTbl[0][1] - PatDat[0][3] / TITLE_TIME;

			py2 -= (double)PatDat[1][3] / TITLE_TIME;
			ph2 += (double)PatDat[1][3] / TITLE_TIME;
			PAT[ PAT_TITLE_TEXT2 ].PosY = (short)py2;
			PAT[ PAT_TITLE_TEXT2 ].Height = (short)ph2;
			OBJ[ OBJ_TITLE_TEXT2 ].py = PosTbl[1][1] - PatDat[1][3] / TITLE_TIME;

			if( PAT[ PAT_TITLE_TEXT1 ].Height >= PatDat[0][3] )
			{
				SetPattern( PAT_TITLE_TEXT1, TEX_TITLE_SYS, PatDat[0][0],PatDat[0][1], PatDat[0][2],PatDat[0][3] );
				SetPattern( PAT_TITLE_TEXT2, TEX_TITLE_SYS, PatDat[1][0],PatDat[1][1], PatDat[1][2],PatDat[1][3] );
				TitleFlag = 2;
			}

			break;

		/////////////////////////////////////////////////
		//	[PUSH START]の点滅
		case	2:
			if( AnimeFlag == 0 )
			{
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].A -= 5;
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].R -= 5;
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].G -= 5;
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].B -= 5;

				if( POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].R <= 10 )
					AnimeFlag = 1;
			}
			else
			{
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].A += 5;
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].R += 5;
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].G += 5;
				POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].B += 5;

				if( POL[ OBJ[ OBJ_TITLE_TEXT3 ].pol_no ].R >= 250 )
					AnimeFlag = 0;
			}

			if( PAD[0].Trg[ PAD_START ] || PAD[1].Trg[ PAD_START ] )
			{
				g_LibMusic.Play( SE_START, ONE );
				OBJ[ OBJ_TITLE_TEXT3 ].disp = OFF;
				OBJ[ OBJ_TITLE_TEXT4 ].disp = ON;
				OBJ[ OBJ_TITLE_TEXT5 ].disp = ON;
				TitleFlag = 3;

				WaitTime( 30 );
			}

			break;

		/////////////////////////////////////////////////
		//	カーソル選択（選択中は点滅）
		case	3:
			if( ( POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].A <= 0 )
			|| ( POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].A >= 255 ) )
			{
				TitleAlpha ^= 1;
			}

			if( TitleAlpha )
			{
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].A += 5;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].R += 5;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].G += 5;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].B += 5;
			}
			else
			{
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].A -= 5;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].R -= 5;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].G -= 5;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].B -= 5;
			}

			if( ( PAD[0].Trg[ PAD_UP ]  ) || ( PAD[0].Trg[ PAD_DOWN ] )
			|| ( PAD[1].Trg[ PAD_UP ]  ) || ( PAD[1].Trg[ PAD_DOWN ] ) )
			{
				g_LibMusic.Play( SE_CURSOR, ONE );
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].A = 255;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].R = 255;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].G = 255;
				POL[ OBJ[ OBJ_TITLE_TEXT4 + TitleAdd ].pol_no ].B = 255;
				TitleAdd ^= 1;
			}

			if( PAD[0].Trg[ PAD_START ] || PAD[0].Trg[ PAD_BUT1 ]
			|| PAD[0].Trg[ PAD_START ] || PAD[0].Trg[ PAD_BUT1 ] )
			{
				TitleFlag = 4 + (2 * TitleAdd );
				g_LibMusic.Play( SE_START, ONE );
			}
			break;

		/////////////////////////////////////////////////
		//	ゲームスタート
		case	4:
			WaitTime( 60 );
			TitleFlag = 5;
		case	5:
			TitleEndFlag = ON;
			SYS.main_mode = MAIN_MODE_CHAR_SELECT_INIT;
			break;

		/////////////////////////////////////////////////
		//	オプション
		case	6:
			WaitTime( 60 );
			TitleFlag = 7;
		case	7:
			TitleEndFlag = ON;
			SYS.main_mode = MAIN_MODE_OPTION_INIT;
			break;
	}

	return	ret;
}
