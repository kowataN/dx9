/******************************************************************************/
//	キャラクターセレクト
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Obj.h"
#include	"./CharSelect.h"

//----------------------------------------------------------------------------//
//	グローバル
//----------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	タイトルモードフラグ
short	CharSelectFlag;
short	Move[2];
short	Center[4][2]=
{
	24,17,
	20,11,
	32,29,
	28,15,
};

short	Pat[4][4]=
{
	64,0,48,40,
	56,48,40,24,
	0,0,64,48,
	0,48,56,32,
};

short	SelectFlag[2],UnitPat[2];

//	ユニット1
char	*Unit1Help={ "一番平均的なユニット。\nそのため、使いやすい。\n初心者向き" };
//	ユニット2
char	*Unit2Help={ "ユニットの中で一番最速を誇るユニ\nット。\n中級者向き" };
//	ユニット3
char	*Unit3Help={ "範囲ショットに優れているユニット。\n初心者向き" };
//	ユニット4
char	*Unit4Help={"全兵器がホーミング性のショット持つ\n特殊なユニット。（最強？）\nその代わり移動速度は遅くなっている\n。\n上級者向き"};

//----------------------------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------------------------//
void	CharSelectInit( void );	//	キャラセレクト初期化
void	CharSelectMain( void );	//	キャラセレクトメイン
void	CharSelectCurSor( void );
void	CharSelectUnitSet( void );
void	CharSelectHelpDisp( void );



/******************************************************************************/
//	キャラセレクト初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharSelectInit( void )
{
	/////////////////////////////////////////////////
	//	ライブラリ初期化
	InitTextureAll();
	InitPatternAll();
	InitPolygonAll();
	g_LibText.InitAll();
	g_LibMusic.InitAll();
	g_LibFade.Init();
	InitPadAll();

	ObjInitAll();

	PAD[0].Use = ON;
	PAD[1].Use = ON;

	/////////////////////////////////////////////////
	//	テクスチャ読み込み
	LoadTexture( TEX_CHAR_SELECT_BG, "img\\sys\\char_sel_bg.img" );
	LoadTexture( TEX_CHAR_SELECT_CHAR, "img\\char\\chr_samp.img" );
	LoadTexture( TEX_CHAR_SELECT_SHOT, "img\\battle\\shot.img" );

	/////////////////////////////////////////////////
	//	パターンカット
	SetPattern( PAT_CHAR_SELECT_BG, TEX_CHAR_SELECT_BG, 0,0, 640,480 );
	SetPattern( PAT_CHAR_SELECT_1P, TEX_CHAR_SELECT_CHAR, 0,0, 0,0 );
	SetPattern( PAT_CHAR_SELECT_2P, TEX_CHAR_SELECT_CHAR, 0,0, 0,0 );

	/////////////////////////////////////////////////
	//	ポリゴンセット
	SetPolygon( POL_CHAR_SELECT_CUR_1P, 64,64, 288,112,1.0, ATR_BOX );
	SetPolygon( POL_CHAR_SELECT_CUR_2P, 64,64, 288,176,1.0, ATR_BOX );
	SetPolygonColor( POL_CHAR_SELECT_CUR_1P, 255,255,0,0 );
	SetPolygonColor( POL_CHAR_SELECT_CUR_2P, 255,0,0,255 );

	/////////////////////////////////////////////////
	//	オブジェクトセット
	ObjSet( OBJ_CHAR_SELECT_BG, POL_CHAR_SELECT_BG, PAT_CHAR_SELECT_BG, 0,0,1.0, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_CHAR_SELECT_1P, POL_CHAR_SELECT_1P, PAT_CHAR_SELECT_1P, 128,344,1.0, OBJ_TYPE_BG, ATR_NONE );
	ObjSet( OBJ_CHAR_SELECT_2P, POL_CHAR_SELECT_2P, PAT_CHAR_SELECT_2P, 512,344,1.0, OBJ_TYPE_BG, ATR_NONE );

	/////////////////////////////////////////////////
	//	SEの読み込み
	g_LibMusic.Load( SE_CURSOR, "se\\se_cursor.wav" );
	g_LibMusic.Load( SE_START, "se\\se_button01.wav" );
	g_LibMusic.Load( SE_BUT, "se\\se_button00.wav" );

	/////////////////////////////////////////////////
	//	グローバル変数初期化
	Move[0] = 0;
	Move[1] = 1;

	SelectFlag[0] = SelectFlag[1] = NOT;

	srand( (unsigned)time(NULL) );

	CharSelectFlag = 0;
	UnitPat[0] = UnitPat[1] = 0;

	g_LibText.Set( 0, 16,112, 255,255,255,255, "" );
	g_LibText.Set( 1, 384,112, 255,255,255,255, "" );
}

/******************************************************************************/
//	キャラセレクトメイン
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharSelectMain( void )
{
	switch( CharSelectFlag )
	{
		case	0:
			//	カーソルの移動
			CharSelectCurSor();
			//	ユニット表示
			CharSelectUnitSet();
			//	説明文表示
			CharSelectHelpDisp();
			break;

		case	1:
			WaitTime( 60 );
			SYS.main_mode = MAIN_MODE_GAME_INIT;
			break;
	}
}

/******************************************************************************/
//	カーソル移動
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharSelectCurSor( void )
{
	double	cursor[5]={ 112,176,240,304,368};

	for( short i=0; i<2; i++ )
	{
		if( SelectFlag[i] == NOT )
		{
			//	カ−ソルの移動
			if( PAD[i].Trg[ PAD_UP ] )
			{
				Move[i] --;
				g_LibMusic.Play( SE_CURSOR, ONE );
			}
			else if( PAD[i].Trg[ PAD_DOWN ] )
			{
				Move[i] ++;
				g_LibMusic.Play( SE_CURSOR, ONE );
			}

			//	クリッピング
			if( Move[i] < 0 )
			{
				Move[i] = 4;
			}
			else if( Move[i] > 4 )
			{
				Move[i] = 0;
			}

			if( Move[i] == 4)
			{
				UnitPat[i] = rand() % 4;
			}
			else
			{
				UnitPat[i] = Move[i];
			}
		}

		if( SelectFlag[i] != NOT )
		{
			continue;
		}

		//	ボタン監視
		if( PAD[i].Trg[ PAD_BUT1 ] )
		{
			SelectFlag[i] = Move[i];
			g_LibMusic.Play( SE_START, ONE );
		}
		else if( PAD[i].Trg[ PAD_BUT2 ] )
		{
			SelectFlag[i] = NOT;
			g_LibMusic.Play( SE_BUT, ONE );
		}

		POL[ POL_CHAR_SELECT_CUR_1P+i ].PosY = cursor[ Move[i] ];
	}

	//	カーソルのサイズ
	if( Move[0] == Move[1] )
	{
		POL[ POL_CHAR_SELECT_CUR_1P ].Width = 32;
		POL[ POL_CHAR_SELECT_CUR_2P ].Width = 32;
		POL[ POL_CHAR_SELECT_CUR_2P ].PosX = 288 + 32;
	}
	else
	{
		POL[ POL_CHAR_SELECT_CUR_1P ].Width = 64;
		POL[ POL_CHAR_SELECT_CUR_2P ].Width = 64;
		POL[ POL_CHAR_SELECT_CUR_2P ].PosX = 288;
	}


	if( ( SelectFlag[0] != NOT )
	&& ( SelectFlag[1] != NOT ) )
	{
		CharSelectFlag = 1;
		SelectFlag[0] = UnitPat[0];
		SelectFlag[1] = UnitPat[1];
	}
}

/******************************************************************************/
//	ユニット表示
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharSelectUnitSet( void )
{
	for( short i=0; i<2; i++ )
	{
		short no = UnitPat[ i ];

		PAT[ PAT_CHAR_SELECT_1P+i ].PosX = Pat[ no ][0];
		PAT[ PAT_CHAR_SELECT_1P+i ].PosY = Pat[ no ][1];
		PAT[ PAT_CHAR_SELECT_1P+i ].Width = Pat[ no ][2];
		PAT[ PAT_CHAR_SELECT_1P+i ].Height = Pat[ no ][3];

		ObjCenterSet( OBJ_CHAR_SELECT_1P+i, Center[ no ][0], Center[ no ][1] );
	}
}

/******************************************************************************/
//	ユニットの説明文表示
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CharSelectHelpDisp( void )
{
	char	*str;

	for( short i=0; i<2; i++ )
	{
		if( UnitPat[i] == 0 )
		{
			str = Unit1Help;
		}
		else if( UnitPat[i] == 1 )
		{
			str = Unit2Help;
		}
		else if( UnitPat[i] == 2 )
		{
			str = Unit3Help;
		}
		else if( UnitPat[i] == 3 )
		{
			str = Unit4Help;
		}

		sprintf_s( TXT[i].Str, MAX_PATH, str );
	}
}
