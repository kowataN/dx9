/******************************************************************************/
//	カードゲーム
//		ゲームメインソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Main.h"
#include	"./GameMain.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
E_GameMode		g_GameMode = GameModeInit;
long			BlinkState;
CARD_TBL		Next[NEXT_CARD_MAX];
CARD_TBL		Card[CARD_MAX];
CARD_TBL		Fld[FLD_HEIGHT][FLD_WIDTH];
FLD_CLS_TBL		Cls[FLD_HEIGHT*FLD_WIDTH];
FLAG_TBL		FLAG;
long			CardIndex;
long			FrmCt;
long			Ct;
long			FrmMax;
long			Level;
long			BLevel;
long			ClsIndex;
long			ClearState;
long			ClearIndex;
char			AnmBuf[1024*1024];

////////////////////////////////////////
//	カード用当り判定領域
short	HitRect[FLD_HEIGHT*FLD_WIDTH][4]=
{
	72,120,120,180,
	168,120,216,180,
	264,120,312,180,

	72,228,120,288,
	168,228,216,288,
	264,228,312,288,

	72,336,120,396,
	168,336,216,396,
	264,336,312,396,
};

////////////////////////////////////////
//	矢印アニメ表示座標
double	PosArrow[ARROW_MAX][2] =
{
	//	1段目横線
	132,144,
	228,144,
	//	2段目の縦線
	101,192,
	197,192,
	293,192,
	//	3段目の横線
	132,252,
	228,252,
	//	4段目の縦線
	101,300,
	197,300,
	293,300,
	//	5段目の横線
	132,360,
	228,360,
};


//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	GameLoop( void );
void	GameInit( void );
void	GameMain( void );
void	GameEnd( void );
void	CardInit( void );
void	InitFld( void );
void	FldCardDisp( void );
void	FldCardSet( short PosX, short PosY );
void	NextCardSet( void );
void	NextCardDisp( void );
void	MouseChk( void );
void	AddCardIndex( void );
bool	Judgment( short x, short y );
ULong	JudgePos( short x, short y, short Col, short Num );
void	SetClearPos( short PosX, short PosY, ULong State );
bool	FldChk( short PosX, short PosY );
long	ScoreChk( void );
void	LevelChk( void );
long	GetPatNo( CARD_TBL card );
void	FldClear( short x, short y );
void	CardBlink( void );
bool	ChkGameOver( void );
void	DispScore( void );
void	DispAddScore( void );
void	DispArrow( void );
void	CardClear( void );
void	DispDeckNumber( void );


/******************************************************************************/
//	ゲームループ
/******************************************************************************/
void	GameLoop( void )
{
	switch( g_GameMode )
	{
		//	ゲーム初期化
		case	GameModeInit:
			GameInit();
			g_GameMode = GameModeMain;

		//	ゲームメイン
		case	GameModeMain:
			GameMain();
			break;

		//	カード点滅
		case	GameModeBlink:
			CardBlink();
			break;
		
		//	カード消滅
		case	GameModeCardClear:
			CardClear();
			break;

		//	終了
		case	GameModeEnd:
			GameEnd();
			break;
	}

	if( g_GameMode == GameModeMain )
	{
		DispArrow();
	}

#ifdef	__DEBUG__
	if( KEY.Trg[ DIK_SPACE ] )
	{
		g_GameMode = GameModeInit;
	}
	if( KEY.Trg[ DIK_A ] )
	{
		g_GameMode = GameModeEnd;
	}
#endif
}

/******************************************************************************/
//	ゲーム初期化
/******************************************************************************/
void	GameInit( void )
{
	/////////////////////////////////////////////////
	//	画像読み込み

	//	背景
	LoadTexture( TEX_BG, "img\\bg.bmp" );
	//	カード
	LoadTexture( TEX_CARD, "img\\card.bmp" );
	//	システム
	LoadTexture( TEX_SYSTEM, "img\\system.bmp" );

	/////////////////////////////////////////////////
	//	パターン

	//	背景
	SetPattern( PAT_BG, TEX_BG, 0,0, 640,480 );

	//	カード
	for( int i=0; i<DEF_USE_CARD_NUMBER; i++ )
	{
		//	赤
		SetPattern( PAT_CARD_RED + i, TEX_CARD, CARD_W_SIZE*i, 0, CARD_W_SIZE,CARD_H_SIZE );
		//	緑
		SetPattern( PAT_CARD_GREEN + i, TEX_CARD, CARD_W_SIZE*i, 60, CARD_W_SIZE,CARD_H_SIZE );
		//	青
		SetPattern( PAT_CARD_BLUE + i, TEX_CARD, CARD_W_SIZE*i, 120, CARD_W_SIZE,CARD_H_SIZE );
		//	空
		SetPattern( PAT_CARD_SKY + i, TEX_CARD, CARD_W_SIZE*i, 180, CARD_W_SIZE,CARD_H_SIZE );
		//	黒
		SetPattern( PAT_CARD_BLACK + i, TEX_CARD, CARD_W_SIZE*i, 240, CARD_W_SIZE,CARD_H_SIZE );
		//	黄
		SetPattern( PAT_CARD_YELLOW + i, TEX_CARD, CARD_W_SIZE*i, 300, CARD_W_SIZE,CARD_H_SIZE );
	}

	//	カード裏
	SetPattern( PAT_DECK, TEX_CARD, 0,360, CARD_W_SIZE,CARD_H_SIZE );

	//	スコア
	for( int i=0; i<11; i++ )
	{
		SetPattern( PAT_NUMBER0 + i, TEX_SYSTEM, 24+(NUM_W_SIZE*i), 96, NUM_W_SIZE,NUM_H_SIZE );
	}

	//	チェイン
	for( int i=0; i<4; i++ )
	{
		SetPattern( PAT_CHAIN+i, TEX_SYSTEM, 24, (i*CHAIN_H_SIZE), CHAIN_W_SIZE,CHAIN_H_SIZE);
	}

	//	加算値用
	SetPattern( PAT_PLUS, TEX_SYSTEM, 184,96, NUM_W_SIZE,NUM_H_SIZE );

	//	終了文字
	SetPattern( PAT_END, TEX_SYSTEM, 0,136, END_W_SIZE,END_H_SIZE );

	/////////////////////////////////////////////////
	//	アニメデータ読み込み
	LoadAnm( 0, "img\\system.anm", AnmBuf, TEX_ANM );
	SetAnm( 0, 0, 0, PAT_ARROW, AnmBuf );
	for( int i=0; i<ARROW_MAX; i++ )
	{
		SetAnmPolygon( POL_ARROW + i, 0, PosArrow[i][0], PosArrow[i][1], 0.2, ANM_LOOP );
		g_LibPol.DispChange( POL_ARROW + i, OFF );
	}

	g_LibPol.SetRotZ( POL_ARROW+2, 90 );
	g_LibPol.SetRotZ( POL_ARROW+3, 90 );
	g_LibPol.SetRotZ( POL_ARROW+4, 90 );
	g_LibPol.SetRotZ( POL_ARROW+7, 90 );
	g_LibPol.SetRotZ( POL_ARROW+8, 90 );
	g_LibPol.SetRotZ( POL_ARROW+9, 90 );

	/////////////////////////////////////////////////
	//	乱数初期化
	srand( (unsigned)time(NULL) );

	//	カ−ドの初期化
	CardInit();

	/////////////////////////////////////////////////
	//	ポリゴン表示

	//	背景
	g_LibPol.SetPat( POL_BG, PAT_BG, 0,0,0.0, ATR_NONE );

	//	山札
	g_LibPol.SetPat( POL_DECK, PAT_DECK, DECK_X_POS,DECK_Y_POS,0.1, ATR_NONE );
	//	ネクストカード
	NextCardDisp();

	//	点滅用
	g_LibPol.Set( POL_BLINK, CARD_W_SIZE,CARD_H_SIZE, BLINK_X_POS,BLINK_Y_POS,1.0, ATR_NONE );
	g_LibPol.DispChange( POL_BLINK, OFF );

	//	クリア用
	for( int i=0; i<5; i++ )
	{
		g_LibPol.Set( POL_CLEAR + i, CARD_W_SIZE,CARD_H_SIZE, BLINK_X_POS,BLINK_Y_POS,1.0, ATR_NONE );
		g_LibPol.SetCenter( POL_CLEAR + i, (CARD_W_SIZE/2),(CARD_H_SIZE/2) );
		g_LibPol.DispChange( POL_CLEAR + i, OFF );
	}

	//	スコア
	for( int i=0; i<10; i++ )
	{
		g_LibPol.SetPat( POL_NUMBER+i, PAT_NUMBER_NONE, SCORE_X_POS+(i*16),SCORE_Y_POS,0.0, ATR_NONE );
	}

	//	チェイン
	g_LibPol.SetPat( POL_CHAIN, PAT_CHAIN, HitRect[0][0]+24, HitRect[0][1], 0.0, ATR_NONE );
	g_LibPol.DispChange( POL_CHAIN, OFF );

	//	加算値
	g_LibPol.SetPat( POL_PLUS, PAT_PLUS,    PLUS_X_POS,PLUS_Y_POS,0.0, ATR_NONE );
	g_LibPol.SetPat( POL_ADD1, PAT_NUMBER7, ADD1_X_POS,PLUS_Y_POS,0.0, ATR_NONE );
	g_LibPol.SetPat( POL_ADD2, PAT_NUMBER8, ADD2_X_POS,PLUS_Y_POS,0.0, ATR_NONE );
	g_LibPol.SetPat( POL_ADD3, PAT_NUMBER9, ADD3_X_POS,PLUS_Y_POS,0.0, ATR_NONE );
	g_LibPol.DispChange( POL_PLUS, OFF );
	g_LibPol.DispChange( POL_ADD1, OFF );
	g_LibPol.DispChange( POL_ADD2, OFF );
	g_LibPol.DispChange( POL_ADD3, OFF );

	//	山札の残り枚数
	g_LibPol.SetPat( POL_DECKNUM1, PAT_NUMBER0, DECKNUM1_X_POS,DECKNUM1_Y_POS,0.0, ATR_NONE );
	g_LibPol.SetPat( POL_DECKNUM2, PAT_NUMBER0, DECKNUM2_X_POS,DECKNUM2_Y_POS,0.0, ATR_NONE );

	//	終了文字
	g_LibPol.SetPat( POL_END, PAT_END, 320,240,1.0, ATR_NONE );
	g_LibPol.SetCenter( POL_END, 144, 20 );
	g_LibPol.DispChange( POL_END, OFF );
	g_LibPol.Set( POL_END+1, 640,480, 0,0,0.9, ATR_NONE );
	g_LibPol.SetColor( POL_END+1, 127, 0,0,0 );
	g_LibPol.DispChange( POL_END+1, OFF );

	/////////////////////////////////////////////////
	//	フィールド初期化
	InitFld();

	CardIndex = 0;
	BlinkState = 0;
	ClearState = 0;
	ClearIndex = 0;
	FrmCt = 0;
	Ct = 0;
	FrmMax = 15;
	Level = BLevel = 5;
	ClsIndex = 0;
	FLAG.Trg = NOT;
	FLAG.Set = OFF;
	FLAG.Judge = OFF;
	g_GameMng.Score  = 0;
}

/******************************************************************************/
//	ゲームメイン
/******************************************************************************/
void	GameMain( void )
{
	MouseChk();
	FldCardDisp();
	LevelChk();
	DispScore();
	DispDeckNumber();

	if( CARD_MAX - CardIndex == 1 )
	{
		//	ゲーム終了
		g_GameMode = GameModeEnd;
	}

	Ct ++;
	if( Ct >= 60 )
	{
		Ct = 0;

		FrmCt ++;
		if( FrmCt >= FrmMax )
		{
			FrmCt = 0;

			Level --;
			if( Level <= 1 )
				Level = 1;
		}
	}
}

/******************************************************************************/
//	ゲーム終了処理
/******************************************************************************/
void	GameEnd( void )
{
	//	一旦非表示
	for( int i=0; i<12; i++ )
	{
		g_LibPol.DispChange( POL_ARROW + i, OFF );
	}

	g_LibPol.DispChange( POL_END, ON );
	g_LibPol.DispChange( POL_END+1, ON );
}

/******************************************************************************/
//	山札初期化
/******************************************************************************/
void	CardInit( void )
{
	short	i,j,num,flag,col;
	CARD_TBL	buf[CARD_MAX];

	memset( &buf, 0x00, sizeof(buf) );

	i = j = 0;

	//	カードをランダムで初期化
	for( i=0; i<CARD_MAX; i++ )
	{
		flag = 0;
		num = rand() % DEF_USE_CARD_NUMBER;
		col = rand() % CARD_COLOR_MAX;

		//	同じカードが使われているか検索
		for( j=0; j<CARD_MAX; j++ )
		{
			if( buf[ j ].Use == OFF )
			{
				continue;
			}

			if( ( num == buf[ j ].No )
			&& ( col == buf[ j ].Color ) )
			{
				flag ++;
			}
		}

		if( flag > DEF_USE_SAME_NAME )
		{
			i --;
			continue;
		}

		Card[ i ].Use = ON;
		buf[ i ].Use = ON;
		buf[ i ].No = Card[ i ].No = num;
		buf[ i ].Color = Card[ i ].Color = col;
	}

	for( i=0; i<NEXT_CARD_MAX; i++ )
	{
		Next[ i ].Use = ON;
		Next[ i ].No = Card[ i ].No;
		Next[ i ].Color = Card[ i ].Color;
	}
#ifdef	__DEBUG__
	//char	ColTbl[CARD_COLOR_MAX][3]=
	//{
	//	"赤","緑","青","空","黒","黄"
	//};
	//for( int No=0; No<CARD_MAX; No++ )
	//{
	//	g_LibText.Set( 1+No, 0,FONT_SIZE*No, 255,255,0,0, "" );
	//	sprintf_s( TXT[1+No].Str, MAX_PATH, "%2d %s%d\n", No, ColTbl[Card[No].Color], Card[No].No+1 );
	//	OutputDebugString(TXT[1+No].Str);
	//}
#endif
}

/******************************************************************************/
//	フィールド初期化
/******************************************************************************/
void	InitFld( void )
{
	memset( &Fld, 0x00, sizeof(Fld) );

	for( int i=0; i<FLD_HEIGHT; i++ )
	{
		for( int j=0; j<FLD_WIDTH; j++ )
		{
			Fld[i][j].Use = OFF;
			Fld[i][j].No = NOT;
			Fld[i][j].Color = NOT;
		}
	}

	/////////////////////////////////////////////////
	//	矢印設定

	//	上段：左
	Fld[0][0].Arrow[0].Use = OFF;
	Fld[0][0].Arrow[0].PolNo = 0;
	Fld[0][0].Arrow[1].Use = ON;
	Fld[0][0].Arrow[1].PolNo = POL_ARROW+2;
	Fld[0][0].Arrow[2].Use = OFF;
	Fld[0][0].Arrow[2].PolNo = 0;
	Fld[0][0].Arrow[3].Use = ON;
	Fld[0][0].Arrow[3].PolNo = POL_ARROW+0;
	//	上段：真ん中
	Fld[0][1].Arrow[0].Use = OFF;
	Fld[0][1].Arrow[0].PolNo = 0;
	Fld[0][1].Arrow[1].Use = ON;
	Fld[0][1].Arrow[1].PolNo = POL_ARROW+3;
	Fld[0][1].Arrow[2].Use = ON;
	Fld[0][1].Arrow[2].PolNo = POL_ARROW+0;
	Fld[0][1].Arrow[3].Use = ON;
	Fld[0][1].Arrow[3].PolNo = POL_ARROW+1;
	//	上段：右
	Fld[0][2].Arrow[0].Use = OFF;
	Fld[0][2].Arrow[0].PolNo = 0;
	Fld[0][2].Arrow[1].Use = ON;
	Fld[0][2].Arrow[1].PolNo = POL_ARROW+4;
	Fld[0][2].Arrow[2].Use = ON;
	Fld[0][2].Arrow[2].PolNo = POL_ARROW+1;
	Fld[0][2].Arrow[3].Use = OFF;
	Fld[0][2].Arrow[3].PolNo = 0;

	//	中段：左
	Fld[1][0].Arrow[0].Use = ON;
	Fld[1][0].Arrow[0].PolNo = POL_ARROW+2;
	Fld[1][0].Arrow[1].Use = ON;
	Fld[1][0].Arrow[1].PolNo = POL_ARROW+7;
	Fld[1][0].Arrow[2].Use = OFF;
	Fld[1][0].Arrow[2].PolNo = 0;
	Fld[1][0].Arrow[3].Use = ON;
	Fld[1][0].Arrow[3].PolNo = POL_ARROW+5;
	//	中段：真ん中
	Fld[1][1].Arrow[0].Use = ON;
	Fld[1][1].Arrow[0].PolNo = POL_ARROW+3;
	Fld[1][1].Arrow[1].Use = ON;
	Fld[1][1].Arrow[1].PolNo = POL_ARROW+8;
	Fld[1][1].Arrow[2].Use = ON;
	Fld[1][1].Arrow[2].PolNo = POL_ARROW+5;
	Fld[1][1].Arrow[3].Use = ON;
	Fld[1][1].Arrow[3].PolNo = POL_ARROW+6;
	//	中段：右
	Fld[1][2].Arrow[0].Use = ON;
	Fld[1][2].Arrow[0].PolNo = POL_ARROW+4;
	Fld[1][2].Arrow[1].Use = ON;
	Fld[1][2].Arrow[1].PolNo = POL_ARROW+9;
	Fld[1][2].Arrow[2].Use = ON;
	Fld[1][2].Arrow[2].PolNo = POL_ARROW+6;
	Fld[1][2].Arrow[3].Use = OFF;
	Fld[1][2].Arrow[3].PolNo = 0;

	//	下段：左
	Fld[2][0].Arrow[0].Use = ON;
	Fld[2][0].Arrow[0].PolNo = POL_ARROW+7;
	Fld[2][0].Arrow[1].Use = OFF;
	Fld[2][0].Arrow[1].PolNo = 0;
	Fld[2][0].Arrow[2].Use = OFF;
	Fld[2][0].Arrow[2].PolNo = 0;
	Fld[2][0].Arrow[3].Use = ON;
	Fld[2][0].Arrow[3].PolNo = POL_ARROW+10;
	//	下段：真ん中
	Fld[2][1].Arrow[0].Use = ON;
	Fld[2][1].Arrow[0].PolNo = POL_ARROW+8;
	Fld[2][1].Arrow[1].Use = OFF;
	Fld[2][1].Arrow[1].PolNo = 0;
	Fld[2][1].Arrow[2].Use = ON;
	Fld[2][1].Arrow[2].PolNo = POL_ARROW+10;
	Fld[2][1].Arrow[3].Use = ON;
	Fld[2][1].Arrow[3].PolNo = POL_ARROW+11;
	//	下段：右
	Fld[2][2].Arrow[0].Use = ON;
	Fld[2][2].Arrow[0].PolNo = POL_ARROW+9;
	Fld[2][2].Arrow[1].Use = OFF;
	Fld[2][2].Arrow[1].PolNo = 0;
	Fld[2][2].Arrow[2].Use = ON;
	Fld[2][2].Arrow[2].PolNo = POL_ARROW+11;
	Fld[2][2].Arrow[3].Use = OFF;
	Fld[2][2].Arrow[3].PolNo = 0;
}

/******************************************************************************/
//	フィールドカードの表示
/******************************************************************************/
void	FldCardDisp( void )
{
	long	PolNo = POL_CARD;
	for( int h=0; h<FLD_HEIGHT; h++ )
	{
		for( int w=0; w<FLD_WIDTH; w++ )
		{
			g_LibPol.SetPat( PolNo, GetPatNo( Fld[h][w] ),
				72+(w*96), 120+(h*108), 0.5, ATR_NONE );

			PolNo ++;
		}
	}
}

/******************************************************************************/
//	フィールドにカードをセット
/******************************************************************************/
void	FldCardSet( short PosX, short PosY )
{
	if( Fld[ PosY ][ PosX ].No != NOT )
	{
		return;
	}

	Fld[ PosY ][ PosX ].Use = ON;
	Fld[ PosY ][ PosX ].No = Next[0].No;
	Fld[ PosY ][ PosX ].Color = Next[0].Color;

	FLAG.Set = ON;
	FLAG.Judge = ON;

	if( Judgment( PosX, PosY ) )
	{
		//	TODO ここで、カード消滅アニメとか
		g_GameMode = GameModeCardClear;
		ClearState = 0;
	}
}

/******************************************************************************/
//	ネクストカードセット
/******************************************************************************/
void	NextCardSet( void )
{
	Next[ 0 ].No = Card[ CardIndex ].No;
	Next[ 0 ].Color = Card[ CardIndex ].Color;

	if( CardIndex+1 == CARD_MAX )
	{
		Next [ 0 ].Use = OFF;
	}

	if( CardIndex+1 < CARD_MAX )
	{
		Next[ 1 ].No = Card[ CardIndex+1 ].No;
		Next[ 1 ].Color = Card[ CardIndex+1 ].Color;
	}
	else
	{
		Next[ 1 ].Use = OFF;
	}

	if( CardIndex+2 < CARD_MAX )
	{
		Next[ 2 ].No = Card[ CardIndex+2 ].No;
		Next[ 2 ].Color = Card[ CardIndex+2 ].Color;
	}
	else
	{
		Next[ 2 ].Use = OFF;
	}
}

/******************************************************************************/
//	ネクストカード表示
/******************************************************************************/
void	NextCardDisp( void )
{
	g_LibPol.SetPat( POL_NEXT1,GetPatNo( Next[0] ), NEXT1_X_POS,NEXT1_Y_POS,0.4, ATR_NONE );
	g_LibPol.SetPat( POL_NEXT2,GetPatNo( Next[1] ), NEXT2_X_POS,NEXT2_Y_POS,0.3, ATR_NONE );
	g_LibPol.SetPat( POL_NEXT3,GetPatNo( Next[2] ), NEXT3_X_POS,NEXT3_Y_POS,0.2, ATR_NONE );

	if( Next[0].Use == OFF )
	{
		g_LibPol.DispChange( POL_NEXT1, OFF );
	}
	if( Next[1].Use == OFF )
	{
		g_LibPol.DispChange( POL_NEXT2, OFF );
	}
	if( Next[2].Use == OFF )
	{
		g_LibPol.DispChange( POL_NEXT3, OFF );
	}
}

/******************************************************************************/
//	マウスのクリックチェック
/******************************************************************************/
void	MouseChk( void )
{
	long	x1,y1,x2,y2,py,px;
	FLAG.Trg = NOT;

	if( FLAG.Set && FLAG.Judge )
	{
		return;
	}

	if( MOUSE.Trg[ MOUSE_LEFT ] )
	{
		px = MOUSE.PosX;
		py = MOUSE.PosY;

		for( int i=0; i<FLD_HEIGHT*FLD_WIDTH; i++ )
		{
			x1 = HitRect[ i ][ 0 ];
			y1 = HitRect[ i ][ 1 ];
			x2 = HitRect[ i ][ 2 ];
			y2 = HitRect[ i ][ 3 ];

			if( (px > x1) && (px < x2)
			&& (py > y1) && (py < y2) )
			{
				FLAG.Trg = i;
				break;
			}
			else
			{
				FLAG.Trg = NOT;
			}
		}
	}

	if( FLAG.Trg != NOT )
	{
		short x = FLAG.Trg % 3;
		short y = FLAG.Trg / 3;

		//	カードが置けるか判定
		if( FldChk( x, y ) )
		{
			//	インデックス加算
			AddCardIndex();

			FldCardSet(x,y);
			NextCardSet();
			NextCardDisp();
			//DispArrow();
		}
		else
		{
			//	ゲームオーバー判定
			if( ChkGameOver() )
			{
				//	ゲームオーバー
			}
			else
			{
				//	カード点滅
				g_GameMode = GameModeBlink;
				BlinkState = 0;
			}
		}
	}
}

/******************************************************************************/
//	カードインデックス加算
/******************************************************************************/
void	AddCardIndex( void )
{
	CardIndex ++;
	if( CardIndex >= CARD_MAX )
	{
		CardIndex = CARD_MAX - 1;
	}
}

/******************************************************************************/
//	判定
/******************************************************************************/
bool	Judgment( short x, short y )
{
	bool bRet = false;
	UShort	buf[FLD_HEIGHT][FLD_WIDTH] = {0},
			num,col;
	ULong	flag[4] = {CARD_NONE};
	FLD_CLS_TBL Buf[4];

	if( !FLAG.Judge )
	{
		return bRet;
	}

	//	基準情報
	col = Fld[y][x].Color;
	num = Fld[y][x].No;
	memset( &Cls, 0x00, sizeof(Cls) );
	memset( &Buf, 0x00, sizeof(Buf) );
	ClsIndex = 0;

	Buf[0].PosX = x;
	Buf[0].PosY = y-1;
	Buf[1].PosX = x;
	Buf[1].PosY = y+1;
	Buf[2].PosX = x-1;
	Buf[2].PosY = y;
	Buf[3].PosX = x+1;
	Buf[3].PosY = y;
	for( int i=0; i<4; i++ )
	{
		Buf[i].State = CARD_NONE;
	}

	////////////////////////////////////
	//	上判定
	if( !( y-1 < 0 ) )
	{
		Buf[0].State = JudgePos( x, y-1, col, num );
	}

	////////////////////////////////////
	//	下判定
	if( !( y+1 >= 3 ) )
	{
		Buf[1].State = JudgePos( x, y+1, col, num );
	}

	////////////////////////////////////
	//	左判定
	if( !( x-1 < 0 ) )
	{
		Buf[2].State = JudgePos(x-1, y, col, num );
	}

	////////////////////////////////////
	//	右判定
	if( !( x+1 >= 3 ) )
	{
		Buf[3].State = JudgePos(x+1, y, col, num );
	}

	////////////////////////////////////
	//	判定の結果
	for( int i=0; i<4; i++ )
	{
		if( Buf[i].State != CARD_NONE )
		{
			SetClearPos( Buf[i].PosX, Buf[i].PosY, Buf[i].State );
			bRet = true;
		}
	}

	FLAG.Judge = OFF;

	return bRet;
}

/******************************************************************************/
//	座標判定
/******************************************************************************/
ULong	JudgePos( short x, short y, short Col, short Num )
{
	ULong Ret = CARD_NONE;

	//	番号が同じ
	if( Num == (Fld[y][x].No) )
	{
		Ret |= CARD_NUM;
	}

	//	色が同じ
	if( Col == (Fld[y][x].Color) )
	{
		Ret |= CARD_MARK;
	}

	return Ret;
}

/******************************************************************************/
//	消去対象設定
/******************************************************************************/
void	SetClearPos( short PosX, short PosY, ULong State )
{
	Cls[ClsIndex].PosX = PosX;
	Cls[ClsIndex].PosY = PosY;
	Cls[ClsIndex].State = State;
	ClsIndex ++;
}

/******************************************************************************/
//	カードが置けるか判定
/******************************************************************************/
bool	FldChk( short PosX, short PosY )
{
	bool bRet = false;

	if( Fld[PosY][PosX].Use == OFF )
	{
		bRet = true;
	}

	return bRet;
}

/******************************************************************************/
//	スコア判定
/******************************************************************************/
long	ScoreChk( void )
{
	return	0;
}

/******************************************************************************/
/******************************************************************************/
void	LevelChk( void )
{
	if( Level != BLevel )
	{
		FrmMax -= 5 - Level;
	}

	if( Level == 5 )
	{
		FrmMax = 15;
	}

	BLevel = Level;
}

/******************************************************************************/
//	パターン番号取得
//		戻り値：
//				パターン番号
//		引数：
//				なし
/******************************************************************************/
long	GetPatNo( CARD_TBL card )
{
	long PatNo = ( card.Color * 10 ) + PAT_CARD_RED + card.No;
	return PatNo;
}

/******************************************************************************/
//	フィールド初期化
/******************************************************************************/
void	FldClear( short x, short y )
{
	Fld[y][x].Use = OFF;
	Fld[y][x].Color = NOT;
	Fld[y][x].No = NOT;
}

/******************************************************************************/
//	カード点滅
/******************************************************************************/
void	CardBlink( void )
{
	switch( BlinkState )
	{
		//	まずは初期化
		case 0:
			g_LibPol.SetPos( POL_BLINK, HitRect[FLAG.Trg][0], HitRect[FLAG.Trg][1] );
			g_LibPol.SetColor( POL_BLINK, 128,127,127,127 );
			BlinkState = 1;
			g_LibPol.DispChange( POL_BLINK, ON );
			break;

		//	徐々に白く
		case 1:
			g_LibPol.AddColor( POL_BLINK, 0,ADD_COLOR,ADD_COLOR,ADD_COLOR );
			if( g_LibPol.GetInfo(POL_BLINK).R >= 247 )
			{
				BlinkState = 2;
			}
			break;

		//	一旦元の色に戻す
		case 2:
			g_LibPol.AddColor( POL_BLINK, 0,-ADD_COLOR,-ADD_COLOR,-ADD_COLOR );
			if( g_LibPol.GetInfo(POL_BLINK).R <= 127 )
			{
				BlinkState = 3;
			}
			break;

		//	徐々に白く
		case 3:
			g_LibPol.AddColor( POL_BLINK, 0,ADD_COLOR,ADD_COLOR,ADD_COLOR );
			if( g_LibPol.GetInfo(POL_BLINK).R >= 247 )
			{
				BlinkState = 4;
			}
			break;

		//	一旦元の色に戻す
		case 4:
			g_LibPol.AddColor( POL_BLINK, 0,-ADD_COLOR,-ADD_COLOR,-ADD_COLOR );
			if( g_LibPol.GetInfo(POL_BLINK).R <= 127 )
			{
				BlinkState = 5;
			}
			break;

		case 5:
			g_GameMode = GameModeMain;
			BlinkState = 0;
			g_LibPol.DispChange( POL_BLINK, OFF );
			break;
	}
}

/******************************************************************************/
//	ゲームオーバー判定
/******************************************************************************/
bool	ChkGameOver( void )
{
	bool bRet = false;


	return bRet;
}

/******************************************************************************/
//	スコア表示
/******************************************************************************/
void	DispScore( void )
{
	long	cScore[10] = {0};
	long	Max = 1000000000;
	long	scr = g_GameMng.Score;

	for( int i=0; i<10; i++ )
	{
		cScore[i] = scr / Max;
		scr %= Max;
		Max /=10;
	}

	for( int i=0; i<10; i++ )
	{
		g_LibPol.DispChange( POL_NUMBER + i, ON );
		g_LibPol.SetPatNo( POL_NUMBER + i, PAT_NUMBER0 + cScore[i] );
	}

	//	表示設定
	if( cScore[0] == 0 )
	{
		g_LibPol.DispChange( POL_NUMBER, OFF );
	}
	for( int i=1; i<9; i++ )
	{
		if( cScore[i] == 0 )
		{
			if( cScore[i-1] == 0 && g_LibPol.GetInfo( POL_NUMBER + i-1 ).Disp == OFF )
			{
				g_LibPol.DispChange( POL_NUMBER + i, OFF );
			}
		}
	}
}

/******************************************************************************/
//	スコア加算値表示
/******************************************************************************/
void	DispAddScore( void )
{
	long	AddScore[3] = {0};
	long	MaxAdd = 100;
	long	add = g_GameMng.ScoreAdd;

	//	加算値を行ったそれぞれの桁に格納
	for( int i=0; i<3; i++ )
	{
		AddScore[i] = add / MaxAdd;
		add %= MaxAdd;
		MaxAdd /= 10;
	}

	//	初期化
	g_LibPol.SetPos( POL_PLUS, PLUS_X_POS,PLUS_Y_POS );
	g_LibPol.SetPos( POL_ADD1, ADD1_X_POS,PLUS_Y_POS );
	g_LibPol.SetPos( POL_ADD2, ADD2_X_POS,PLUS_Y_POS );
	g_LibPol.SetPos( POL_ADD3, ADD3_X_POS,PLUS_Y_POS );
	g_LibPol.SetColor( POL_PLUS, 255,255,255,255 );
	g_LibPol.SetColor( POL_ADD1, 255,255,255,255 );
	g_LibPol.SetColor( POL_ADD2, 255,255,255,255 );
	g_LibPol.SetColor( POL_ADD3, 255,255,255,255 );

	//	一旦表示
	g_LibPol.DispChange( POL_PLUS, ON );
	for( int i=0; i<3; i++ )
	{
		g_LibPol.DispChange( POL_ADD1 + i, ON );
		g_LibPol.SetPatNo( POL_ADD1 + i, PAT_NUMBER0 + AddScore[i] );
	}

	//	表示設定
	if( AddScore[0] == 0 )
	{
		g_LibPol.DispChange( POL_ADD1, OFF );
		if( AddScore[1] == 0 )
		{
			g_LibPol.DispChange( POL_ADD2, OFF );
		}
	}
}

/******************************************************************************/
//	矢印表示
/******************************************************************************/
void	DispArrow( void )
{
	short w,h;

	//	一旦非表示
	for( int i=0; i<12; i++ )
	{
		g_LibPol.DispChange( POL_ARROW + i, OFF );
	}

	//	表示判定
	for( h=0; h<FLD_HEIGHT; h++ )
	{
		for( w=0; w<FLD_WIDTH; w++ )
		{
			//	マークと数字を確認
			if( ( Fld[h][w].No == Card[CardIndex].No )
			|| ( Fld[h][w].Color == Card[CardIndex].Color ) )
			{
				//	上判定
				if( ( Fld[h][w].Arrow[0].Use == ON )
				&& ( Fld[h-1][w].Use == OFF ) )
				{
					g_LibPol.DispChange( Fld[h][w].Arrow[0].PolNo, ON );
				}

				//	下判定
				if( ( Fld[h][w].Arrow[1].Use == ON )
				&& ( Fld[h+1][w].Use == OFF ) )
				{
					g_LibPol.DispChange( Fld[h][w].Arrow[1].PolNo, ON );
				}

				//	左判定
				if( ( Fld[h][w].Arrow[2].Use == ON )
				&& ( Fld[h][w-1].Use == OFF ) )
				{
					g_LibPol.DispChange( Fld[h][w].Arrow[2].PolNo, ON );
				}

				//	右判定
				if( ( Fld[h][w].Arrow[3].Use == ON )
				&& ( Fld[h][w+1].Use == OFF ) )
				{
					g_LibPol.DispChange( Fld[h][w].Arrow[3].PolNo, ON );
				}
			}
		}
	}
}

/******************************************************************************/
//	カード消去処理
/******************************************************************************/
void	CardClear( void )
{
	long PosX,PosY,Pos;

	switch( ClearState )
	{
		//	初期化
		case	0:
			PosX =  Cls[ ClearIndex ].PosX;
			PosY =  Cls[ ClearIndex ].PosY;
			Pos = PosY * FLD_HEIGHT + PosX;

			g_LibPol.SetPatNo( POL_CHAIN, PAT_CHAIN + ClearIndex );
			g_LibPol.SetPos( POL_CLEAR, HitRect[FLAG.Trg][0]+24, HitRect[FLAG.Trg][1]+30 );
			g_LibPol.SetPos( POL_CLEAR+1, HitRect[ Pos ][0]+24, HitRect[ Pos ][1]+30 );
			g_LibPol.SetPos( POL_CHAIN, HitRect[FLAG.Trg][0]+32, HitRect[FLAG.Trg][1]-36 );
			g_LibPol.SetColor( POL_CLEAR, 128,127,127,127 );
			g_LibPol.SetColor( POL_CLEAR+1, 128,127,127,127 );
			g_LibPol.SetColor( POL_CHAIN, 128,127,127,127 );
			g_LibPol.SetScale( POL_CLEAR, 1 );
			g_LibPol.SetScale( POL_CLEAR+1,1 );

			ClearState = 1;
			g_LibPol.DispChange( POL_CLEAR, ON );
			g_LibPol.DispChange( POL_CLEAR+1, ON );
			g_LibPol.DispChange( POL_CHAIN, ON );
			break;

		//	徐々に白く、大きく
		case	1:
			g_LibPol.SetColor( POL_CLEAR,
				g_LibPol.GetInfo(POL_CLEAR).A,
				g_LibPol.GetInfo(POL_CLEAR).R + ADD_COLOR,
				g_LibPol.GetInfo(POL_CLEAR).G + ADD_COLOR,
				g_LibPol.GetInfo(POL_CLEAR).B + ADD_COLOR );
			g_LibPol.SetColor( POL_CLEAR+1,
				g_LibPol.GetInfo(POL_CLEAR+1).A,
				g_LibPol.GetInfo(POL_CLEAR+1).R + ADD_COLOR,
				g_LibPol.GetInfo(POL_CLEAR+1).G + ADD_COLOR,
				g_LibPol.GetInfo(POL_CLEAR+1).B + ADD_COLOR );
			g_LibPol.SetColor( POL_CHAIN,
				g_LibPol.GetInfo(POL_CHAIN).A,
				g_LibPol.GetInfo(POL_CHAIN).R + ADD_COLOR,
				g_LibPol.GetInfo(POL_CHAIN).G + ADD_COLOR,
				g_LibPol.GetInfo(POL_CHAIN).B + ADD_COLOR );

			g_LibPol.SetScale( POL_CLEAR, g_LibPol.GetInfo(POL_CLEAR).ScaleX+0.05 );
			g_LibPol.SetScale( POL_CLEAR+1, g_LibPol.GetInfo(POL_CLEAR+1).ScaleX+0.05 );

			if( g_LibPol.GetInfo(POL_CLEAR).R >= 247 )
			{
				ClearState = 2;
			}
			break;

		//	非表示
		case	2:
			g_LibPol.DispChange( POL_CLEAR, OFF );
			g_LibPol.DispChange( POL_CLEAR+1, OFF );
			ClearIndex ++;
			if( ClearIndex >= ClsIndex )
			{
				ClearState = 4;
			}
			else
			{
				ClearState = 3;
			}
			break;

		//	ウエイト
		case	3:
			ClearState = 0;
			g_GameMng.ScoreAdd = (ClearIndex*ClearIndex) * 20;
			DispAddScore();
			WaitTime( 15 );
			g_LibPol.DispChange( POL_CHAIN, OFF );
			break;

		//	カード消去
		case	4:
			//	TODO とりあえず、直接加算
			g_GameMng.ScoreAdd = (ClearIndex*ClearIndex) * 20;
			DispAddScore();
			WaitTime( 15 );
			g_LibPol.DispChange( POL_CHAIN, OFF );
			for( int i=0; i<ClsIndex; i ++ )
			{
				FldClear( Cls[i].PosX, Cls[i].PosY );
			}
			ClearState = 5;
			break;

		//	ウエイト
		case	5:
			WaitTime( 10 );
			ClearState = 6;

		//	スコア加算
		case	6:
			//	加算値を下に移動
			g_LibPol.AddPosY(POL_PLUS, ADD_Y_POS );
			g_LibPol.AddPosY(POL_ADD1, ADD_Y_POS );
			g_LibPol.AddPosY(POL_ADD2, ADD_Y_POS );
			g_LibPol.AddPosY(POL_ADD3, ADD_Y_POS );

			g_LibPol.AddColor(POL_PLUS, -ADD_COLOR2,0,0,0 );
			g_LibPol.AddColor(POL_ADD1, -ADD_COLOR2,0,0,0 );
			g_LibPol.AddColor(POL_ADD2, -ADD_COLOR2,0,0,0 );
			g_LibPol.AddColor(POL_ADD3, -ADD_COLOR2,0,0,0 );
			if( g_LibPol.GetInfo(POL_PLUS).PosY >= 130 )
			{
				ClearState = 7;
			}
			break;

		//	終了
		case	7:
			//	加算値非表示
			g_LibPol.DispChange( POL_PLUS, OFF );
			g_LibPol.DispChange( POL_ADD1, OFF );
			g_LibPol.DispChange( POL_ADD2, OFF );
			g_LibPol.DispChange( POL_ADD3, OFF );
			g_GameMode = GameModeMain;
			ClearIndex = 0;
			ClearState = 0;
			g_GameMng.Score += g_GameMng.ScoreAdd;
			g_GameMng.ScoreAdd = 0;
			break;
	}
}

/******************************************************************************/
//	山札残り枚数表示
/******************************************************************************/
void	DispDeckNumber( void )
{
	long	Number[2] = {0};
	long	MaxNum = 10;
	long	Num = CARD_MAX - (CardIndex+1);

	//	桁ごとに格納
	for( int i=0; i<2; i++ )
	{
		Number[i] = Num / MaxNum;
		Num %= MaxNum;
		MaxNum /= 10;
	}

	//	表示
	for( int i=0; i<2; i++ )
	{
		g_LibPol.SetPatNo( POL_DECKNUM1 + i, PAT_NUMBER0 + Number[i] );
	}
}
