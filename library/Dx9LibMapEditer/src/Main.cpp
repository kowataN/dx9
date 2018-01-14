/******************************************************************************/
//	マップエディター
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibPad.h"

#include	"MainD.h"

//----------------------------------------------------------------------------//
//	グローバル
//----------------------------------------------------------------------------//
short		MainMode = OFF;
char		BmpFilePath[MAX_PATH];	//	BMPファイルのパス
char		MapFilePath[MAX_PATH];	//	MAPファイルのパス
char		BmpFileName[MAX_PATH];	//	BMPファイルの名前
char		MapFileName[MAX_PATH];	//	MAPファイルの名前
short		DispX,DispY;			//	MAPの表示座標
FLAG_TBL	FLAG;					//	フラグ管理構造体
MAP_TBL		MAP;					//	MAP構造体
MAP_TBL		S_MAP;					//	サーチ用

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	MainInit( void );						//	メイン初期化
void	EditerMain( void );						//	エディターメイン
void	MenuSelect( void );						//	メニューセレクト
short	MouseSel( void );						//	マウスセレクト
void	Clip( void );							//	クリッピング
void	MapSave( void );						//	マップデータのセーブ
void	MapLoad( void );						//	マップデータのロード
void	BmpLoad( void );						//	BMPファイルのロード
void	WidthUp( void );						//	Ｗ▲
void	WidthDown( void );						//	Ｗ▼
void	HeightUp( void );						//	Ｈ▲
void	HeightDown( void );						//	Ｈ▼
void	Layer1Switch( void );					//	レイヤー１スイッチ
void	Layer2Switch( void );					//	レイヤー２スイッチ
void	Layer3Switch( void );					//	レイヤー３スイッチ
void	HitSwitch( void );						//	当たり判定スイッチ
void	PageLeft( void );						//	ページ左
void	PageRight( void );						//	ページ右
void	MenuHitChk( void );						//	メニューとの当たり判定
void	ChipSel( void );						//	チップの選択
void	ChipMapSet( UChar Flag );				//	チップをマップにセット
long	MapSet( long Map, long Layer );			//	マップセット
long	ChipGet( long MapDat, long Layer );		//	チップ番号取得
long	MapHitChkSet( long Map );				//	マップ当たり判定セット
long	MapHitChkGet( long Map );				//	マップあたり判定取得
void	MapInit(void );							//	マップ初期化
void	MapDisp( void );						//	マップ表示
void	MapOff( void );							//	マップ非表示
long	LayerDisp( long PolNo, long Layer );	//	レイヤー表示
void	FontDisp( void );						//	文字表示
void	GridInit( void );						//	グリッド初期化
void	GridDisp( void );						//	グリッド表示
void	BrushCheck( void );						//	ブラシ監視
void	BrushPaint( void );						//	塗りつぶし
void	BrushPaintSearch( long x, long y );		//	塗りつぶし箇所検出
void	GetKeyButton( void );					//	キーの取得
void	ChipToCursorPos( long Chip );			//	チップ番号からカーソル座標取得
void	MenuDispCheck( void );					//	メニュー表示監視
void	*Tbl[]=
{
	MapSave,
	MapLoad,
	BmpLoad,
	WidthUp,
	WidthDown,
	HeightUp,
	HeightDown,
	Layer1Switch,
	Layer2Switch,
	Layer3Switch,
	HitSwitch,
	PageLeft,
	PageRight,
};

void	DebugMsInit( void );	//	デバッグ用文字の初期化
void	DebugMsDisp( void );	//	デバッグ用文字の表示



/******************************************************************************/
//	メイン
/******************************************************************************/
void	MainLoop( void )
{
	switch( MainMode )
	{
		/////////////////////////////////////
		//	メイン初期化
		case	OFF:
			MainInit();
			MainMode = ON;

		/////////////////////////////////////
		//	エディターメイン
		case	ON:
			EditerMain();
			break;
	}
}

/******************************************************************************/
//	メイン初期化
/******************************************************************************/
void	MainInit( void )
{
	/////////////////////////////////////
	//	ライブラリ初期化
	InitTextureAll();
	InitPatternAll();
	InitPolygonAll();
	g_LibText.InitAll();
	g_LibFade.Init();
	PAD[0].Use = ON;

	/////////////////////////////////////
	//	テクスチャー読み込み
	LoadTexture( TEX_SYSTEM, "dat\\system000.bmp" );

	/////////////////////////////////////
	//	パターン
	short	pat_no[6] =
	{
		PAT_MENU,	//	0:メニュー
		PAT_CURSOR,	//	1:カーソル
		PAT_FRM,	//	2:スイッチ枠
		PAT_MARU,	//	3:○
		PAT_BATU,	//	4:×
		PAT_BRUSH,	//	5:ブラシ
	};
	for( long i=0; i<6; i++ )
	{
		SetPattern( pat_no[i], TEX_SYSTEM,
			PatDat[i][0], PatDat[i][1],
			PatDat[i][2], PatDat[i][3] );
	}

	/////////////////////////////////////
	//	ポリゴン
	short	pol_no[4][2] =
	{
		POL_MENU,	PAT_MENU,		//	メニュー
		POL_CURSOR,	PAT_CURSOR,		//	カーソル
		POL_FRM,	PAT_FRM,		//	スイッチ枠
		POL_SEL,	PAT_CURSOR,		//	カーソル（メニュー）
	};

	for( long i=0; i<4; i++ )
	{
		SetPolygonPat( pol_no[i][0], pol_no[i][1],
			PolDisp[i][0], PolDisp[i][1], PolDisp[i][2], ATR_NONE );
	}

	SetPolygonPat( POL_BRUSH, PAT_BRUSH, MOUSE.PosX,MOUSE.PosY,1.0, ATR_NONE );

	/////////////////////////////////////
	//	テキスト
	g_LibText.Set( TXT_MOUSE, Font[0][0], Font[0][1], 255, 255, 255, 255, "" );
	g_LibText.Set( TXT_FILE_NAME, Font[1][0], Font[1][1], 255, 0, 0, 0, "" );
	g_LibText.Set( TXT_WIDTH, Font[2][0], Font[2][1], 255, 0, 0, 0, "" );
	g_LibText.Set( TXT_HEIGHT, Font[3][0], Font[3][1], 255, 0, 0, 0, "" );

	ZeroMemory( BmpFilePath, MAX_PATH );
	ZeroMemory( MapFilePath, MAX_PATH );
	ZeroMemory( BmpFileName, MAX_PATH );
	ZeroMemory( MapFileName, MAX_PATH );

	//	初期マップサイズ初期化
	MAP.Use = OFF;
	MAP.Width = 40;
	MAP.Height = 30;

	//	背景
	SetPolygon( POL_BG,
		MAP.Width*CHIP_SIZE, MAP.Height*CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
	SetPolygonColor( POL_BG, 255, 100, 100, 100 );

	//	フラグ初期化
	FLAG.HitMenu = OFF;
	FLAG.HitChipWin = OFF;
	FLAG.Page = 0;
	FLAG.Chip = 0;
	FLAG.ChipBk = 0;
	FLAG.DispMap = OFF;
	FLAG.LoadBmp = OFF;
	FLAG.Layer = 0;
	FLAG.Brush = OFF;

	//	マップ表示位置初期化
	DispX = DispY = 0;

	/////////////////////////////////////
	//	マップ初期化
	MapInit();

	/////////////////////////////////////
	//	グリッド初期化
	GridInit();

	DebugMsInit();
}

/******************************************************************************/
//	エディターメイン処理
/******************************************************************************/
void	EditerMain( void )
{
	long	mx = MOUSE.PosX / CHIP_SIZE;
	long	my = MOUSE.PosY / CHIP_SIZE;

	/////////////////////////////////////
	//	メニューとの当たり判定
	MenuHitChk();

	/////////////////////////////////////
	//	マウス判定
	if( FLAG.HitMenu )
	{
		MenuSelect();
	}
	else
	{
		//	当り判定
		if( FLAG.Layer == 3 )
		{
			if( MOUSE.Trg[ MOUSE_LEFT ] )
			{
				if( FLAG.Brush == OFF )
				{
					ChipMapSet( 0 );
				}
				else if( FLAG.Brush == ON )
				{
					BrushPaint();
				}

				FLAG.DispMap = ON;
			}
		}
		else
		{
			if( MOUSE.Trg[ MOUSE_LEFT ] || MOUSE.Push[ MOUSE_LEFT ] )
			{
				if( FLAG.Brush == OFF )
				{
					ChipMapSet( 0 );
				}
				else if( FLAG.Brush == ON )
				{
					BrushPaint();
				}

				FLAG.DispMap = ON;
			}
			else if( MOUSE.Trg[ MOUSE_RIGHT ] || MOUSE.Push[ MOUSE_RIGHT ] )
			{
				ChipMapSet( 1 );
				FLAG.DispMap = ON;
			}
		}
	}

	/////////////////////////////////////
	//	チップ選択
	ChipSel();

	/////////////////////////////////////
	//	クリッピング
	POL[ POL_CURSOR ].PosX = mx * CHIP_SIZE;
	POL[ POL_CURSOR ].PosY = my * CHIP_SIZE;
	Clip();

	/////////////////////////////////////
	//	マップ表示
	if( FLAG.DispMap )
	{
		MapDisp();
	}

	/////////////////////////////////////
	//	フォント表示
	FontDisp();

	/////////////////////////////////////
	//	グリッド表示
	GridDisp();

	/////////////////////////////////////
	//	キーボード処理
	GetKeyButton();

	/////////////////////////////////////
	//	ブラシの監視
	BrushCheck();

	PAT[ PAT_BMP ].PosX = ChipPatDat[ FLAG.Page ][0];
	PAT[ PAT_BMP ].PosY = ChipPatDat[ FLAG.Page ][1];

	DebugMsDisp();
}

/******************************************************************************/
//	メニューセレクト
/******************************************************************************/
void	MenuSelect( void )
{
	if( MOUSE.Trg[ MOUSE_LEFT ]
	|| ( ( MOUSE.Count[ MOUSE_LEFT ] >= KEY_REPEAT_START )
	&& ( MOUSE.Count[ MOUSE_LEFT ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		short ret = MouseSel();
		if( ret >= 0 )
		{
			void(*func)(void);
			func = ( void(*)(void) )Tbl[ ret ];
			(*func)();
		}
	}
}

/******************************************************************************/
//	マウスの選択
/******************************************************************************/
short	MouseSel( void )
{
	short	ret = -1;
	for( short i=0; i<DAT_HIT_MAX; i++ )
	{
		short	x1 = DatHit[ i ][ 0 ];
		short	x2 = x1 + DatHit[ i ][ 2 ];
		short	y1 = DatHit[ i ][ 1 ];
		short	y2 = y1 + DatHit[ i ][ 3 ];

		if( ( MOUSE.PosX >= x1 ) && ( MOUSE.PosX <= x2 )
		&& ( MOUSE.PosY >= y1 ) && ( MOUSE.PosY <= y2 ) )
		{
			ret = i;
			break;
		}
	}

	return	ret;
}

/******************************************************************************/
//	クリッピング
/******************************************************************************/
void	Clip( void )
{
	short	PolNo = POL_CURSOR;

	//	左端
	if( POL[ PolNo ].PosX < POL[ POL_BG ].PosX )
	{
		POL[ PolNo ].PosX = POL[ POL_BG ].PosX;
	}

	//	右端
	if( POL[ PolNo ].PosX >= POL[ POL_BG ].PosX + MAP.Width * CHIP_SIZE )
	{
		POL[ PolNo ].PosX = POL[ POL_BG ].PosX + MAP.Width * CHIP_SIZE - CHIP_SIZE;
	}

	//	上端
	if( POL[ PolNo ].PosY < POL[ POL_BG ].PosY )
	{
		POL[ PolNo ].PosY = POL[ POL_BG ].PosY;
	}

	//	下端
	if( POL[ PolNo ].PosY >= POL[ POL_BG ].PosY + MAP.Height * CHIP_SIZE )
	{
		POL[ PolNo ].PosY = POL[ POL_BG ].PosY + MAP.Height * CHIP_SIZE - CHIP_SIZE;
	}
}

/******************************************************************************/
//	マップデータ保存
/******************************************************************************/
void	MapSave( void )
{
	long	Chip[256]={NOT};
	long	FileSize = 0;

	ShowCursor( TRUE );
	//	ファイルダイアログの表示
	if( SaveFileDialog( Dialog[0], Dialog[2], Dialog[5], MapFilePath ) == 0 )
	{
		ShowCursor( FALSE );
		return;
	}
	ShowCursor( FALSE );

	/////////////////////////////////////
	//	準備

	//	bmpファイルの長さ
	long BmpFileLen = (long)strlen( BmpFileName );

	/////////////////////////////////////
	//	ヘッダ部
	ZeroMemory( &g_FreeWork, sizeof(g_FreeWork) );
	char *pt = (char *)g_FreeWork;
	//	ファイルの種類
	*pt = 'M';
	pt ++;
	*pt = 'A';
	pt ++;
	*pt = 'P';
	pt ++;
	FileSize += 3;

	//	bmpファイルの長さ
	SetLong( BmpFileLen, pt );
	pt += 4;
	FileSize += 4;

	//	bmpファイル名
	for( long i=0; i<BmpFileLen; i++ )
	{
		*pt = BmpFileName[ i ];
		pt ++;
		FileSize ++;
	}

	//	総チップ数取得
	long	ChipMax = 0;
	for( long h=0; h<MAP.Height; h++ )
	{
		for( long w=0; w<MAP.Width; w++ )
		{
			for( long Layer=0; Layer<LAYER_HIT; Layer++ )
			{
				//	チップ番号取得
				long	ChipNo = MAP.Layer[Layer].Chip[h][w];
				if( Layer >= 1 && ChipNo == 0 )
				{
					continue;
				}

				long	UseFlag = 1;
				for( long Index=0; Index<ChipMax; Index++ )
				{
					if( Chip[ Index ] == ChipNo )
					{
						UseFlag = 0;
					}
				}

				if( UseFlag )
				{
					Chip[ ChipMax ] = ChipNo;
					ChipMax ++;
				}
			}
		}
	}

	//	マップサイズ
	SetLong( MAP.Height, pt );
	pt += 2;
	FileSize += 2;
	SetLong( MAP.Width, pt );
	pt += 2;
	FileSize += 2;

	//	総チップ数
	SetLong( ChipMax, pt );
	pt += 4;
	FileSize += 4;

	/////////////////////////////////////
	//	実データ

	//	マップデータ
	for( long h=0; h<MAP.Height; h++ )
	{
		TRACE("\n");
		for( long w=0; w<MAP.Width; w++ )
		{
			long	Map = 0;
			for( long Layer=0; Layer<LAYER_MAX; Layer++)
			{
				Map |= MapSet( MAP.Layer[Layer].Chip[h][w], Layer );
			}
			Map |= MapHitChkSet( MAP.Layer[LAYER_HIT].Chip[h][w] );

			TRACE1("%d ",Map);
			SetLong( Map, pt );
			pt += 4;
			FileSize += 4;
		}
	}

	//	書き出し
	SaveFile( MapFilePath, g_FreeWork, FileSize );
}

/******************************************************************************/
//	マップデータの読み込み
/******************************************************************************/
void	MapLoad( void )
{
	//	マップが既に使用中かどうか
	if( MAP.Use )
	{

	}

	ShowCursor( TRUE );
	//	ファイルダイアログの表示
	if( LoadFileDialog( Dialog[0], Dialog[2], Dialog[4], MapFilePath ) == 0 )
	{
		ShowCursor( FALSE );
		return;
	}
	ShowCursor( FALSE );

	ZeroMemory( &g_FreeWork, sizeof(g_FreeWork) );
	long size = LoadFile( MapFilePath, g_FreeWork );

	/////////////////////////////////////
	//	ヘッダ部

	char *pt = ( char * )g_FreeWork;

	//	ファイルの種類
	if( ( pt[0] != 'M' )
	&& ( pt[1] != 'A' )
	&& ( pt[2] != 'P' ) )
	{
		return;
	}
	pt += 3;

	//	ファイルの長さ取得
	long BmpFileLen = GetLong( pt );
	pt += 4;

	//	bmpファイル名
	for( long i=0; i<BmpFileLen; i++ )
	{
		BmpFileName[ i ] = *pt;
		pt ++;
	}

	//	マップサイズ
	MAP.Height = GetShort( pt );
	pt += 2;
	MAP.Width = GetShort( pt );
	pt += 2;

	//	総パターン数
	pt += 4;

	/////////////////////////////////////
	//	実データ

	//	マップデータ
	for( long h=0; h<MAP.Height; h++ )
	{
		TRACE("\n");
		for( long w=0; w<MAP.Width; w++ )
		{
			long Chip = GetLong( pt );
			pt += 4;

			TRACE1("%d ",Chip);
			if( Chip < 0 )
			{
				continue;
			}

			for( long Layer=0; Layer<LAYER_MAX; Layer++ )
			{
				MAP.Layer[Layer].Chip[h][w] = ChipGet( Chip, Layer );
			}
			MAP.Layer[LAYER_HIT].Chip[h][w] = MapHitChkGet(Chip);
		}
	}
	TRACE("\n");

	/////////////////////////////////////
	//	カレントディレクトリの取得
	char FilePath[MAX_PATH];

	GetCurrentDirectory( MAX_PATH, MapFilePath );
	strcat_s( MapFilePath, MAX_PATH, "\\" );
	strcat_s( MapFilePath, MAX_PATH, BmpFileName );
	PathToDir( MapFilePath, FilePath );
	SetCurrentDirectory( FilePath );
	strcat_s( FilePath, MAX_PATH, "\\" );
	strcat_s( FilePath, MAX_PATH, BmpFileName );

	/////////////////////////////////////
	//	bmpファイル読み込み
	LoadTexture( TEX_BMP, FilePath );
	SetPattern( PAT_BMP, TEX_BMP, 0, 0, 96, 192 );
	SetPolygonPat( POL_BMP, PAT_BMP, CHIP_SIZE, 164, 0.7, ATR_NONE );

	PathToName( BmpFileName, BmpFilePath );
	FLAG.LoadBmp = ON;
	FLAG.DispMap = ON;
}

/******************************************************************************/
//	bmp読み込み
/******************************************************************************/
void	BmpLoad( void )
{
	ShowCursor( TRUE );
	if( LoadFileDialog( Dialog[1], Dialog[3], Dialog[4], BmpFilePath ) == 0 )
	{
		ShowCursor( FALSE );
		return;
	}
	ShowCursor( FALSE );

	LoadTexture( TEX_BMP, BmpFilePath );
	SetPattern( PAT_BMP, TEX_BMP, 0, 0, 96, 192 );
	SetPolygonPat( POL_BMP, PAT_BMP, CHIP_SIZE, 164, 0.7, ATR_NONE );

	FLAG.LoadBmp = ON;

	PathToName( BmpFilePath, BmpFileName );

	MapInit();
	MapOff();
}

/******************************************************************************/
//	幅▲
/******************************************************************************/
void	WidthUp( void )
{
	MAP.Width ++;
	MaxMin( MAP.Width, MAP_W_MAX, 0 );

	//	背景
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	幅▼
/******************************************************************************/
void	WidthDown( void )
{
	MAP.Width --;
	MaxMin( MAP.Width, MAP_W_MAX, 0 );

	//	背景
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	高さ▲
/******************************************************************************/
void	HeightUp( void )
{
	MAP.Height ++;
	MaxMin( MAP.Height, MAP_H_MAX, 0 );

	//	背景
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	高さ▼
/******************************************************************************/
void	HeightDown( void )
{
	MAP.Height --;
	MaxMin( MAP.Height, MAP_H_MAX, 0 );

	//	背景
	SetPolygon( POL_BG,
		MAP.Width * CHIP_SIZE,
		MAP.Height * CHIP_SIZE,
		128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );
}

/******************************************************************************/
//	レイヤー１スイッチ
/******************************************************************************/
void	Layer1Switch( void )
{
	FLAG.Layer = 0;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 20, 368 );
}

/******************************************************************************/
//	レイヤー２スイッチ
/******************************************************************************/
void	Layer2Switch( void )
{
	FLAG.Layer = 1;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 68, 368 );
}

/******************************************************************************/
//	レイヤー３スイッチ
/******************************************************************************/
void	Layer3Switch( void )
{
	FLAG.Layer = 2;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 20, 388 );
}

/******************************************************************************/
//	当たり判定
/******************************************************************************/
void	HitSwitch( void )
{
	FLAG.Layer = 3;
	FLAG.DispMap = ON;
	SetPolygonPos( POL_FRM, 68, 388 );
}

/******************************************************************************/
//	左ボタン
/******************************************************************************/
void	PageLeft( void )
{
	FLAG.Page --;
	MaxMin( FLAG.Page, PAGE_MAX, 0 );

	PAT[ PAT_BMP ].PosX = ChipPatDat[ FLAG.Page ][0];
	PAT[ PAT_BMP ].PosY = ChipPatDat[ FLAG.Page ][1];
}

/******************************************************************************/
//	右ボタン
/******************************************************************************/
void	PageRight( void )
{
	FLAG.Page ++;
	MaxMin( FLAG.Page, PAGE_MAX, 0 );

	PAT[ PAT_BMP ].PosX = ChipPatDat[ FLAG.Page ][0];
	PAT[ PAT_BMP ].PosY = ChipPatDat[ FLAG.Page ][1];
}

/******************************************************************************/
//	メニューとの当たり判定
/******************************************************************************/
void	MenuHitChk( void )
{
	short	x1 = PatDat[0][0];
	short	y1 = PatDat[0][1];
	short	x2 = PatDat[0][2];
	short	y2 = PatDat[0][3];

	if( ( MOUSE.PosX >= x1 ) && ( MOUSE.PosX <= x2 )
	&& ( MOUSE.PosY >= y1 ) && ( MOUSE.PosY <= y2 ) )
	{
		FLAG.HitMenu = ON;
	}
	else
	{
		FLAG.HitMenu = OFF;
	}
}

/******************************************************************************/
//	チップ選択
/******************************************************************************/
void	ChipSel( void )
{
	long	x1 = CHIP_WINDOW_X;
	long	y1 = CHIP_WINDOW_Y;
	long	x2 = CHIP_WINDOW_W;
	long	y2 = CHIP_WINDOW_H;

	if( ( MOUSE.PosX > x1 ) && ( MOUSE.PosX < x2 )
	&& ( MOUSE.PosY > y1 ) && ( MOUSE.PosY < y2 ) )
	{
		FLAG.HitChipWin = ON;
		if( MOUSE.Trg[ MOUSE_LEFT ] )
		{
			long x = MOUSE.PosX - CHIP_WINDOW_X;
			long y = MOUSE.PosY - CHIP_WINDOW_Y;
			x /= CHIP_SIZE;	
			y /= CHIP_SIZE;

			FLAG.Chip = ( (y%CHIP_H_NUM_MAX) * CHIP_W_NUM_MAX ) + x + ( FLAG.Page * CHIP_NUM_MAX );

			POL[ POL_SEL ].PosX = CHIP_SIZE * x + CHIP_WINDOW_X;
			POL[ POL_SEL ].PosY = CHIP_SIZE * y + CHIP_WINDOW_Y;
		}
	}
	else
	{

		FLAG.HitChipWin = OFF;
	}
}

/******************************************************************************/
//	チップをマップに登録
/******************************************************************************/
void	ChipMapSet( UChar Flag )
{
	long	PosX = MOUSE.PosX / CHIP_SIZE - 8 - DispX;
	long	PosY = MOUSE.PosY / CHIP_SIZE - DispY;

	if( FLAG.Layer == LAYER_HIT )
	{
		//	当たり判定
		if( MapHitChkGet( MAP.Layer[ FLAG.Layer ].Chip[PosY][PosX] ) == 0 )
		{
			MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ] = ON;
		}
		else
		{
			MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ] = OFF;
		}
	}
	else
	{
		if( Flag == 0 )
		{
			//	左クリック
			MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ] = FLAG.Chip;
		}
		else
		{
			//	右クリック
			FLAG.Chip = MAP.Layer[ FLAG.Layer ].Chip[ PosY ][ PosX ];

			//	チップ番号からカーソル座標取得
			ChipToCursorPos( FLAG.Chip );
		}
	}
}

/******************************************************************************/
//	マップセット
/******************************************************************************/
long	MapSet( long Map, long Layer )
{
	if( Layer == 0 )
	{
		//	レイヤー0
		return (Map<<LAYER(Layer));
	}
	else
	{
		if( Map == NOT )
		{
			return	0;
		}
		else
		{
			return (Map<<LAYER(Layer));
		}
	}
}

/******************************************************************************/
//	チップ番号取得
/******************************************************************************/
long	ChipGet( long MapDat, long Layer )
{
	if( MapDat < 0 )
	{
		return 0;
	}
	else
	{
		long ChipNo = ((MapDat&(0x3FF<<LAYER(Layer)))>>LAYER(Layer));
		return ChipNo;
	}
}

/******************************************************************************/
//	マップの当たり判定フラグセット
/******************************************************************************/
long	MapHitChkSet( long Map )
{
	return (Map<<MAP_INDEX_HIT);
}

/******************************************************************************/
//	マップの当たり判定フラグ取得
/******************************************************************************/
long	MapHitChkGet( long Map )
{
	return ((Map&(0x3FF<<MAP_INDEX_HIT))>>MAP_INDEX_HIT);
}

/******************************************************************************/
//	マップの初期化
/******************************************************************************/
void	MapInit(void )
{
	for( long h=0; h<MAP_H_MAX; h++ )
	{
		for( long w=0; w<MAP_W_MAX; w++ )
		{
			for( long Layer=0; Layer<LAYER_MAX; Layer++ )
			{
				if( Layer == LAYER_HIT )
				{
					MAP.Layer[Layer].Chip[h][w] = OFF;
				}
				else
				{
					MAP.Layer[Layer].Chip[h][w] = NOT;
				}
			}
		}
	}

	MAP.Use = OFF;
}

/******************************************************************************/
//	マップ表示
/******************************************************************************/
void	MapDisp( void )
{
	FLAG.DispMap = OFF;
	if(	!FLAG.LoadBmp )
	{
		return;
	}

	long PolNo = 0;
	for( int i=0; i<LAYER_HIT; i++ )
	{
		PolNo = LayerDisp( PolNo, i );
	}
}

/******************************************************************************/
//	マップ非表示
/******************************************************************************/
void	MapOff( void )
{
	long Size = MAP.Height * MAP.Width;
	for( long i=0; i<Size; i++ )
	{
		POL[ POL_MAP + i ].Disp = OFF;
	}
}

/******************************************************************************/
//	レイヤー表示
//		戻り値：
//				使ったポリゴン数
//		引数：
//				PolNo		使う先頭のポリゴン番号
//				Layer		表示するレイヤー
/******************************************************************************/
long	LayerDisp( long PolNo, long Layer )
{
	for( long h=0; h<MAP.Height; h++ )
	{
		for( long w=0; w<MAP.Width; w++ )
		{
			if( MAP.Layer[Layer].Chip[h][w] == NOT )
			{
				continue;
			}
			if( MAP.Layer[Layer].Chip[h][w] == 0 && Layer >= LAYER_2 )
			{
				continue;
			}

			//	パターン
			//long PatNo = ChipGet( MAP.Layer[Layer].Chip[h][w], Layer );
			long PatNo = MAP.Layer[Layer].Chip[h][w];
			if( PatNo != NOT )
			{
				long x = ( PatNo / CHIP_NUM_MAX ) / 2 * BMP_PAT_SIZE_W;
				long AddX = PatNo - ( (PatNo / CHIP_NUM_MAX) * CHIP_NUM_MAX );
				AddX %= CHIP_W_NUM_MAX;
				AddX *= CHIP_SIZE;
				x += AddX;
				long IndexH = ( PatNo / CHIP_NUM_MAX ) * CHIP_NUM_MAX;
				long ChipIndex = PatNo - IndexH;
				ChipIndex /= CHIP_W_NUM_MAX;
				ChipIndex *= CHIP_SIZE;
				long AddY = ( ( PatNo / CHIP_NUM_MAX ) % 2 ) * BMP_PAT_SIZE_H;
				long y = ChipIndex + AddY;

				if( !PAT[PAT_MAP+PatNo].Use )
				{
					SetPattern( PAT_MAP+PatNo, TEX_BMP, x, y, CHIP_SIZE, CHIP_SIZE );
				}

				SetPolygonPat( POL_MAP+PolNo, PAT_MAP+PatNo,
					POL[POL_BG].PosX + ( CHIP_SIZE * w ),
					POL[POL_BG].PosY + ( CHIP_SIZE * h ),
					0.1, ATR_NONE );
			}
			else
			{
				InitPolygon( POL_MAP + PolNo );
			}

			//	アルファ
			if( FLAG.Layer == Layer )
			{
				POL[ POL_MAP + PolNo ].A = 255;
			}
			else
			{
				POL[ POL_MAP + PolNo ].A = 255 / 3;
			}

			PolNo ++;
		}
	}

	return PolNo;
}

/******************************************************************************/
//	フォント表示
/******************************************************************************/
void	FontDisp( void )
{
	long mx = MOUSE.PosX / CHIP_SIZE - 8 - DispX;
	long my = MOUSE.PosY / CHIP_SIZE - DispY;

	if( FLAG.HitMenu )
	{
		sprintf_s( TXT[ TXT_MOUSE ].Str, MAX_PATH, "" );
	}
	else
	{
		sprintf_s( TXT[ TXT_MOUSE ].Str, MAX_PATH, "X:%3d,Y%3d", mx,my );
	}

	sprintf_s( TXT[ TXT_FILE_NAME ].Str, MAX_PATH, "%s", BmpFileName );
	TXT[ TXT_FILE_NAME ].PosX = 12;
	TXT[ TXT_FILE_NAME ].PosY = 80;
	sprintf_s( TXT[ TXT_WIDTH ].Str, MAX_PATH, "%3d", MAP.Width );
	sprintf_s( TXT[ TXT_HEIGHT ].Str, MAX_PATH, "%3d", MAP.Height );
}

/******************************************************************************/
//	グリッド初期化
/******************************************************************************/
void	GridInit( void )
{
	FLAG.DispGrid = ON;

	long	w = 480 / CHIP_SIZE;
	long	h = 640 / CHIP_SIZE;

	for( int i=0; i<w; i++ )
	{
		SetPolygon( POL_GRID+i, 640, 1, 0, i*CHIP_SIZE, 0.3, ATR_NONE );	//	横線
	}
	for( int i=0; i<h; i++ )
	{
		SetPolygon( POL_GRID+30+i, 1, 480, i*CHIP_SIZE, 0, 0.3, ATR_NONE );	//	縦線
	}
	for( int i=0; i<w+h; i++ )
	{
		SetPolygonColor( POL_GRID+i, 255, 0, 0, 0 );
	}

	GridDisp();
}

/******************************************************************************/
//	グリッド表示
/******************************************************************************/
void	GridDisp( void )
{
	long	w = 480 / CHIP_SIZE;
	long	h = 640 / CHIP_SIZE;
	for( int i=0; i<w+h; i++ )
	{
		POL[POL_GRID+i].Disp = FLAG.DispGrid;
	}
}

/******************************************************************************/
//	ブラシ監視
/******************************************************************************/
void	BrushCheck( void )
{
	POL[ POL_BRUSH ].PosX = MOUSE.PosX;

	if( FLAG.Brush == OFF )
	{
		//	ノーマル
		POL[ POL_BRUSH ].PosY = MOUSE.PosY;
		SetPattern( PAT_BRUSH, TEX_SYSTEM, 176,0, 16,16 );
	}
	else
	{
		//	ブラシ
		POL[ POL_BRUSH ].PosY = MOUSE.PosY - 12;
		SetPattern( PAT_BRUSH, TEX_SYSTEM, 160,0, 16,16 );
	}
}

/******************************************************************************/
//	塗りつぶし
/******************************************************************************/
void	BrushPaint( void )
{
	long	x = MOUSE.PosX / CHIP_SIZE - 8 - DispX;
	long	y = MOUSE.PosY / CHIP_SIZE - DispY;

	for( long Layer=0; Layer<LAYER_MAX; Layer++ )
	{
		for( long h=0; h<MAP_H_MAX; h++ )
		{
			for( long w=0; w<MAP_W_MAX; w++ )
			{
				S_MAP.Layer[Layer].Chip[h][w] = OFF;
			}
		}
	}

	//	チップのバックアップ
	FLAG.ChipBk = MAP.Layer[ FLAG.Layer ].Chip[y][x];
	ChipMapSet( 0 );

	S_MAP.Layer[ FLAG.Layer ].Chip[y][x] = ON;

	//	検索
	BrushPaintSearch( x, y );
}

/******************************************************************************/
//	塗りつぶし箇所検索
//		戻り値：
//				０：検索終了、１：検索中
/******************************************************************************/
void	BrushPaintSearch( long x, long y )
{
	//////////////////////////////////
	//	再起関数で検索

	if( y-1 >= 0 )
	{
		//	上を調べる
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y-1 ][ x ] = FLAG.Chip;
			BrushPaintSearch( x, y-1 );
		}
	}

	if( y+1 < MAP.Height )
	{
		//	下を調べる
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y+1 ][ x ] = FLAG.Chip;
			BrushPaintSearch( x, y+1 );
		}
	}

	if( x-1 >= 0 )
	{
		//	左を調べる
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y ][ x-1 ] = FLAG.Chip;
			BrushPaintSearch( x-1, y );
		}
	}

	if( x+1 < MAP.Width )
	{
		//	右を調べる
		if( ( S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] == OFF )
		&& ( MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] == FLAG.ChipBk ) )
		{
			S_MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] = ON;
			MAP.Layer[ FLAG.Layer ].Chip[ y ][ x+1 ] = FLAG.Chip;
			BrushPaintSearch( x+1, y );
		}
	}
}

/******************************************************************************/
//	キーボード入力処理
/******************************************************************************/
void	GetKeyButton( void )
{
	short	mx,my,x,y;
	u_char	alt,ctrl,shift;

	mx = my = x = y = 0;
	alt = ctrl = shift = OFF;

	//	↑
	if( KEY.Trg[ DIK_UP ]
	|| ( ( KEY.Count[ DIK_UP ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_UP ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		my --;
	}

	//	↓
	if( KEY.Trg[ DIK_DOWN ]
	|| ( ( KEY.Count[ DIK_DOWN ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_DOWN ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		my ++;
	}

	//	←
	if( KEY.Trg[ DIK_LEFT ]
	|| ( ( KEY.Count[ DIK_LEFT ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_LEFT ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		mx --;
	}

	//	→
	if( KEY.Trg[ DIK_RIGHT ]
	|| ( ( KEY.Count[ DIK_RIGHT ] >= KEY_REPEAT_START )
	&& ( KEY.Count[ DIK_RIGHT ] % KEY_REPEAT_TIME == 0 ) ) )
	{
		mx ++;
	}

	if( KEY.Push[ DIK_LALT ] || KEY.Push[ DIK_RALT ] )
	{
		alt = ON;	//	[ALT]キー
	}
	if( KEY.Push[ DIK_LCONTROL ] || KEY.Push[ DIK_RCONTROL ] )
	{
		ctrl = ON;	//	[CTRL]キー
	}
	if( KEY.Push[ DIK_LSHIFT ] || KEY.Push[ DIK_RSHIFT ] )
	{
		shift = ON;	//	[SHIFT]キー
	}

	if( alt )
	{
		/////////////////////////////////////
		//	[ALT]キー
	}
	else	if( ctrl )
	{
		/////////////////////////////////////
		//	[CTRL]キー
	}
	else	if( shift )
	{
		/////////////////////////////////////
		//	[SHIFT]キー
		if( KEY.Trg[ DIK_L ] )
		{
			FLAG.Layer --;
			MaxMin( FLAG.Layer, LAYER_MAX, LAYER_1 ); 
			FLAG.DispMap = ON;
			switch( FLAG.Layer )
			{
				case	LAYER_1:
					SetPolygonPos( POL_FRM, 20, 368 );
					break;
				case	LAYER_2:
					SetPolygonPos( POL_FRM, 68, 368 );
					break;
				case	LAYER_3:
					SetPolygonPos( POL_FRM, 20, 388 );
					break;
				case	LAYER_HIT:
					SetPolygonPos( POL_FRM, 68, 388 );
					break;
			}
		}
	}
	else
	{
		/////////////////////////////////////
		//	その他
		DispX += mx;
		DispY += my;

		if( mx != 0 || my != 0 )
		{
			FLAG.DispMap = ON;
		}

		SetPolygon( POL_BG,
			MAP.Width*CHIP_SIZE, MAP.Height*CHIP_SIZE,
			128+(DispX*CHIP_SIZE), DispY*CHIP_SIZE, 0.0, ATR_NONE );

		if( KEY.Trg[ DIK_G ] )
		{
			FLAG.DispGrid ^= 1;	//	グリッド表示切替
		}
		if( KEY.Trg[ DIK_B ] )
		{
			FLAG.Brush ^= 1;		//	ブラシ切替
		}
		if( KEY.Trg[ DIK_M ] )
		{
			MenuDispCheck();		//	メニュー表示切替
		}
		if( KEY.Trg[ DIK_L ] )
		{
			FLAG.Layer ++;
			MaxMin( FLAG.Layer, LAYER_MAX, LAYER_1 ); 
			FLAG.DispMap = ON;
			switch( FLAG.Layer )
			{
				case	LAYER_1:
					SetPolygonPos( POL_FRM, 20, 368 );
					break;
				case	LAYER_2:
					SetPolygonPos( POL_FRM, 68, 368 );
					break;
				case	LAYER_3:
					SetPolygonPos( POL_FRM, 20, 388 );
					break;
				case	LAYER_HIT:
					SetPolygonPos( POL_FRM, 68, 388 );
					break;
			}
		}
	}
}

/******************************************************************************/
//	チップ番号からカーソル座標取得
/******************************************************************************/
void	ChipToCursorPos( long Chip )
{
	double x = ( Chip % CHIP_W_NUM_MAX ) * CHIP_SIZE;
	x += Chip / BMP_PAT_SIZE_H * BMP_PAT_SIZE_W;
	double y = ( ( Chip - (Chip/BMP_PAT_SIZE_H*BMP_PAT_SIZE_H) ) / CHIP_W_NUM_MAX ) * CHIP_SIZE;
	x /= CHIP_SIZE;
	y /= CHIP_SIZE;

	long Page = (short)x / CHIP_W_NUM_MAX;
	FLAG.Page = Page * 2;
	Page *= 6;
	x -= Page;
	x = (short)x;

	Page = (short)y / CHIP_H_NUM_MAX;
	FLAG.Page += Page;
	Page *= CHIP_H_NUM_MAX;
	y -= Page;
	y = (short)y;

	POL[ POL_SEL ].PosX = CHIP_SIZE * x + CHIP_WINDOW_X;
	POL[ POL_SEL ].PosY = CHIP_SIZE * y + CHIP_WINDOW_Y;

	if( Chip < 0 )
	{
		POL[ POL_SEL ].Disp = OFF;
	}
	else
	{
		POL[ POL_SEL ].Disp = ON;
	}
}

/******************************************************************************/
//	メニュー表示監視
/******************************************************************************/
void	MenuDispCheck( void )
{
	//	フラグ反転
	POL[ POL_MENU ].Disp ^= 1;
	POL[ POL_BMP ].Disp ^= 1;
	TXT[ TXT_MOUSE ].Disp ^= 1;
	TXT[ TXT_FILE_NAME ].Disp ^= 1;
	TXT[ TXT_WIDTH ].Disp ^= 1;
	TXT[ TXT_HEIGHT ].Disp ^= 1;
	POL[ POL_FRM ].Disp ^= 1;
	POL[ POL_SEL ].Disp ^= 1;
}

/******************************************************************************/
//	デバッグメッセージ初期化
/******************************************************************************/
void	DebugMsInit( void )
{
#ifdef	__DEBUG__

	for( long i=0; i<10; i++ )
	{
		g_LibText.Set( DB_MS+i, 128, 240+i*FONT_SIZE, 255, 255,0,0, "" );
	}

#endif
}

/******************************************************************************/
//	デバッグメッセージ初期化
/******************************************************************************/
void	DebugMsDisp( void )
{
#ifdef	__DEBUG__

	long	i;
	char	str[128],
			ms[10][128] =
			{
				"レイヤー番号：",
				"ページ番号：",
				"チップ番号：",
				"メニュー当たり判定：",
				"チップ当たり判定：",
			};

	long	dat[10] =
			{
				FLAG.Layer,
				FLAG.Page,
				FLAG.Chip,
				FLAG.HitMenu,
				FLAG.HitChipWin,
			};

	for( i=0; i<5; i++ )
	{
		sprintf_s( str, 128, "%d", dat[i] );
		strcat_s( ms[i], 128, str );
		sprintf_s( TXT[DB_MS+i].Str, MAX_PATH, ms[i] );
	}

	sprintf_s( TXT[ DB_MS+7 ].Str, MAX_PATH, "ファイル名座標:%d,%d", TXT[TXT_FILE_NAME].PosX, TXT[TXT_FILE_NAME].PosY );

#endif
}
