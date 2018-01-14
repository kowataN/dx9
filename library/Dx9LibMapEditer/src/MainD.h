/******************************************************************************/
//	マップエディター
//		ヘッダファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	デファイン
//----------------------------------------------------------------------------//

/////////////////////////////////////
//	番号

//	テクスチャ
#define	TEX_SYSTEM	0		//	システム
#define	TEX_BMP		5		//	bmpファイル

//	パターン
#define	PAT_MENU	0		//	メニュー
#define	PAT_FRM		1		//	フレーム
#define	PAT_CURSOR	5		//	カーソル
#define	PAT_BMP		10		//	bmpファイル
#define	PAT_CHIP	12		//	チップ
#define	PAT_MARU	15		//	○
#define	PAT_BATU	16		//	×
#define	PAT_BRUSH	18		//	ブラシ
#define	PAT_MAP		20		//	マップ

//	ポリゴン
#define	POL_MENU	0		//	メニュー
#define	POL_FRM		1		//	フレーム
#define	POL_CURSOR	5		//	カーソル
#define	POL_SEL		6		//	カーソル（メニュー）
#define	POL_BG		10		//	背景
#define	POL_BMP		15		//	bmpファイル
#define	POL_BRUSH	20		//	ブラシ
#define	POL_GRID	30		//	グリッド
#define	POL_HIT		20		//	○・×
#define	POL_MAP		125		//	マップ

//	テキスト
enum
{
	TXT_MOUSE = 0,
	TXT_FILE_NAME,
	TXT_WIDTH,
	TXT_HEIGHT,
};

/////////////////////////////////////
//	その他

//	チップサイズ
#define	CHIP_SIZE		16
#define	CHIP_W_NUM_MAX	6
#define	CHIP_H_NUM_MAX	12
#define	CHIP_NUM_MAX	(CHIP_W_NUM_MAX*CHIP_H_NUM_MAX)

//	マップの最大数(チップ数)
#define	MAP_W_MAX		300
#define	MAP_H_MAX		300

//	読み込むbmpサイズ
#define	BMP_SIZE		384

//	bmpパターンサイズ
#define	BMP_PAT_SIZE_W	(CHIP_SIZE*CHIP_W_NUM_MAX)	//	96
#define	BMP_PAT_SIZE_H	(CHIP_SIZE*CHIP_H_NUM_MAX)	//	192

//	ページ最大数
#define	PAGE_MAX		8

//	チップウインドウサイズ
#define	CHIP_WINDOW_X	CHIP_SIZE
#define	CHIP_WINDOW_Y	164
#define	CHIP_WINDOW_W	112
#define	CHIP_WINDOW_H	355

#define	LAYER(n)		(n*10)

enum
{
	LAYER_1 = 0,	//	0	レイヤー1
	LAYER_2,		//	1	レイヤー2
	LAYER_3,		//	2	レイヤー3
	LAYER_HIT,		//	3	当り判定
	LAYER_MAX		//	4	最大数
};

#define	MAP_INDEX_LAYER1	0
#define	MAP_INDEX_LAYER2	10
#define	MAP_INDEX_LAYER3	20
#define	MAP_INDEX_HIT		30

//	デバッグスイッチ
//#define	__DEBUG__
#ifdef	__DEBUG__
#define	DB_MS			100
#endif

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	ダイアログ用
char	Dialog[][128]=
{
	"マップファイル(*.map)\0*.map\0\0",
	"画像ファイル(*.bmp *.img)\0*.bmp\0*.img\0",
	"*.map",
	"*.bmp *.img",
	"ロードするファイル名を指定してください。",
	"セーブするファイル名を指定してください",
};

//	パターン座標
short	PatDat[][4] =
{
	0,0,128,416,	//	メニュー１
	128,0,16,16,	//	カーソル
	128,16,40,16,	//	スイッチ枠
	128,32,16,16,	//	○
	144,32,16,16,	//	×
	160,0,16,16,	//	ブラシ
	192,0,128,208,	//	メニュー２
};

//	ポリゴン表示座標
double	PolDisp[][3] =
{
	0,0,0.6,	//	メニュー
	0,0,0.5,	//	カーソル
	20,368,0.8,	//	スイッチ枠
	16,164,0.8,	//	カーソル（メニュー）
};

//	データの当たり判定(MapEdit)
#define	DAT_HIT_MAX	13	//	データの最大数
short	DatHit[DAT_HIT_MAX][4] =
{
	8,32,32,16,		//	save
	48,32,32,16,	//	load
	88,32,32,16,	//	bmp

	100,104,12,8,	//	幅▲
	100,112,12,8,	//	幅▼

	100,124,12,8,	//	高さ▲
	100,132,12,8,	//	高さ▼

	20,368,40,16,	//	レイヤー１スイッチ
	68,368,40,16,	//	レイヤー２スイッチ
	20,388,40,16,	//	レイヤー３スイッチ
	68,388,40,16,	//	当たり判定スイッチ

	12,152,8,8,		//	←
	108,152,8,8,	//	→
};

//	データの当たり判定(MapAnmEdit)
short	AnmHit[][4] =
{
	272,32,32,16,	//	load
	272,128,12,8,	//	アニメ▲
	272,136,12,8,	//	アニメ▼
};

//	フォントの表示座標
#define	FONT_MAX	4	//	フォントの最大数
short	Font[FONT_MAX][2] =
{
	40,8,	//	マウス座標
	12,80,	//	ファイル名
	72,106,	//	高さ
	72,126,	//	幅
};

//	チップパターンデータ
short	ChipPatDat[][2] =
{
	0,0,		//	0
	0,192,		//	1
	96,0,		//	2
	96,192,		//	3
	192,0,		//	4
	192,192,	//	5
	288,0,		//	6
	288,192,	//	7
};

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	フラグ用構造体
typedef	struct
{
	long	Page;		//	ページ
	long	Layer;		//	レイヤー（0:レイヤー1、1:レイヤー2、2:レイヤー3、3:当たり判定）
	long	Chip;		//	チップ番号
	long	ChipBk;		//	チップ番号（バックアップ）
	UChar	HitMenu;	//	メニューたの当たり判定（0:なし、1:あたり）
	UChar	HitChipWin;	//	チップウインドウとの当たり判定（0:なし、1:あたり）
	UChar	DispMap;	//	マップ表示フラグ（0:変化なし、1:変化あり）
	UChar	LoadBmp;	//	画像ファイル読み込みフラグ
	UChar	DispGrid;	//	グリッド表示フラグ（0:非表示、1:表示）
	UChar	Brush;		//	ブラシ（0:ノーマル、1:塗りつぶし）
}FLAG_TBL;

//	マップ構造体
typedef	struct
{
	long	Use;
	long	Width;
	long	Height;	//	マップのサイズ
	struct	LAYER_TBL
	{
		long	Chip[MAP_H_MAX][MAP_W_MAX];	//	パターン番号
	}Layer[LAYER_MAX];
}MAP_TBL;
