/******************************************************************************/
//	ゲームメイン
//		ヘッダファイル
/******************************************************************************/

#ifndef	__GameMain_H__
#define	__GameMain_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//
enum
{
	/////////////////////////////////////////////////
	//	ゲームモード
	MAIN_MODE_INIT = 0,
	MAIN_MODE_TITLE_INIT = 10,
	MAIN_MODE_TITLE_MAIN,
	MAIN_MODE_OPTION_INIT = 20,
	MAIN_MODE_OPTION_MAIN,
	MAIN_MODE_CHAR_SELECT_INIT = 30,
	MAIN_MODE_CHAR_SELECT_MAIN,
	MAIN_MODE_GAME_INIT = 40,
	MAIN_MODE_GAME_MAIN,
	MAIN_MODE_END_INIT = 50,
	MAIN_MODE_END_MAIN,

	/////////////////////////////////////////////////
	//	テクスチャ番号
	TEX_GAME_BG	= 0,
	TEX_GAME_SYSTEM,
	TEX_GAME_END_TEXT,
	TEX_GAME_SHOT,
	TEX_GAME_UNIT,
	TEX_GAME_END_ICON,

	/////////////////////////////////////////////////
	//	パターン番号
	PAT_GAME_BG = 0,
	PAT_GAME_TIME = 10,
	PAT_GAME_GAGE_FRM = 25,
	PAT_GAME_GAGE_BAR = 30,
	PAT_GAME_CONTER = 35,
	PAT_GAME_PLAYER_NO = 50,
	PAT_GAME_PAUSE = 53,
	PAT_GAME_ICON,
	PAT_GAME_UNIT = 55,
	PAT_GAME_BUZZ = 65,
	PAT_GAME_TOUCH = 70,
	PAT_GAME_EFFECT = 75,
	PAT_GAME_END_TEXT = 100,
	PAT_GAME_END_ICON = 110,
	PAT_GAME_SHOT = 120,

	/////////////////////////////////////////////////
	//	ポリゴン番号
	POL_GAME_BG = 0,
	POL_GAME_TIME = 10,
	POL_GAME_GAGE_FRM = 20,
	POL_GAME_GAGE_BAR = 25,
	POL_GAME_CONTER = 30,
	POL_GAME_PLAYER_NO = 35,
	POL_GAME_PAUSE = 38,
	POL_GAME_ICON,
	POL_GAME_UNIT = 40,
	POL_GAME_BUZZ = 45,
	POL_GAME_TOUCH = 50,
	POL_GAME_EFFECT = 60,
	POL_GAME_END_TEXT = 80,
	POL_GAME_END_ICON = 90,
	POL_GAME_SHOT = 100,

	/////////////////////////////////////////////////
	//	オブジェクト番号
	OBJ_GAME_BG = 0,
	OBJ_GAME_TIME = 10,
	OBJ_GAME_GAGE_FRM = 20,
	OBJ_GAME_GAGE_BAR = 25,
	OBJ_GAME_PLAYER_NO = 30,
	OBJ_GAME_CONTER = 35,
	OBJ_GAME_PAUSE = 38,
	OBJ_GAME_ICON,
	OBJ_GAME_UNIT = 40,
	OBJ_GAME_BUZZ = 45,
	OBJ_GAME_TOUCH = 50,
	OBJ_GAME_EFFECT = 60,
	OBJ_GAME_END_TEXT = 80,
	OBJ_GAME_END_ICON = 90,
	OBJ_GAME_SHOT = 100,

	/////////////////////////////////////////////////
	//	アニメ番号

	/////////////////////////////////////////////////
	//	タイプ
	OBJ_TYPE_NONE,	//	なし（初期値）
	OBJ_TYPE_BG,	//	背景
	OBJ_TYPE_SYSTEM,//	システム
	OBJ_TYPE_RECT,	//	領域
	OBJ_TYPE_UNIT,	//	ユニット（プレイヤー）
	OBJ_TYPE_SHOT,	//	ショット

	/////////////////////////////////////////////////
	//	ＢＧＭ＆ＳＥ

	//	BGM
	BGM_BATTLE = 0,

	//	SE
	SE_CURSOR,
	SE_START,
	SE_BUT,
	SE_SHOT1,
	SE_SHOT2,
	SE_SHOT3,
	SE_BUZZ,
	SE_RELOAD,
	SE_BAKUHATU,
	SE_STOCK,
	SE_WARNING,
};

/////////////////////////////////////////////////
//	アトリビュート
#define	OBJ_ATR_NONE			(1<<0)	//	なし（初期値）
#define	OBJ_ATR_UNIT1			(1<<1)	//	ユニット１
#define	OBJ_ATR_UNIT2			(1<<2)	//	ユニット２

#define	OBJ_ATR_SHOT_NORMAL		(1<<3)	//	ノーマルショット
#define	OBJ_ATR_SHOT_SPECIAL1	(1<<4)	//	特殊ショット１
#define	OBJ_ATR_SHOT_SPECIAL2	(1<<5)	//	特殊ショット２
#define	OBJ_ATR_SHOT_EXTRA1		(1<<6)	//	EXショット１
#define	OBJ_ATR_SHOT_EXTRA2		(1<<7)	//	EXショット２

#define	OBJ_ATR_SHOT_HOMING1	(1<<8)	//	ホーミング1
#define	OBJ_ATR_SHOT_HOMING2	(1<<9)	//	ホーミング2
#define	OBJ_ATR_SHOT_HOMING3	(1<<10)	//	ホーミング3
#define	OBJ_ATR_SHOT_HOMING4	(1<<11)	//	ホーミング4
#define	OBJ_ATR_SHOT_HOMING5	(1<<12)	//	ホーミング5

#define	OBJ_ATR_SHOT_HOMING		(1<<13)	//	ホーミング
#define	OBJ_ATR_SHOT_SET_UP		(1<<14)	//	設置式
#define	OBJ_ATR_SHOT_SPLINE		(1<<15)	//	スプライン

#define	OBJ_ATR_ROT_NONE		(1<<16)	//	回転なし
#define	OBJ_ATR_ROT_USE			(1<<17)	//	回転する
#define	OBJ_ATR_ROT_CHANGE		(1<<18)	//	角度変更

#define	OBJ_ATR_SHOT_RANK		(OBJ_ATR_SHOT_NORMAL|OBJ_ATR_SHOT_SPECIAL1|OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_EXTRA2)
#define	OBJ_ATR_SHOT_TYPE		(OBJ_ATR_SHOT_HOMING|OBJ_ATR_SHOT_SET_UP|OBJ_ATR_SHOT_SPLINE)
#define	OBJ_ATR_ROT_TYPE		(OBJ_ATR_ROT_NONE|OBJ_ATR_ROT_USE|OBJ_ATR_ROT_CHANGE)

/////////////////////////////////////////////////
//	デバッグスイッチ
//#define		__DEBUG__

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	システム用
typedef	struct
{
	long	main_mode;	//	メインモード

	short	stage_no;	//	ステージ番号
	short	win;		//	勝者番号（0:プレイヤー１、1:プレイヤー２、2:ドロー）

	short	time;		//	戦闘の時間

	long	frame_ct;	//	フレームカウンタ
	u_char	buzz_disp;	//	バズリ表示フラグ
	u_char	pause_disp;	//	ポーズ表示フラグ

	long	life[2];	//	ライフ（初期化用）
}SYSTEM_TBL;

#endif	//	__GameMain_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	システム
extern	SYSTEM_TBL	SYS;

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 );
