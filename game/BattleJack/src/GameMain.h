/******************************************************************************/
//	バトルジャック
//		ゲームメインヘッダファイル
/******************************************************************************/

#ifndef	__GameMain_H__
#define	__GameMain_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	メインモードフラグ
enum
{
	MainModeInit,
	MainModeMain,
	MainModeEnd,
};

////////////////////////////////////////
//	ゲームモードフラグ
enum
{
	GameModeInit,
	GameModeStart,
	GameModeCardDraw,
	GameModeSelect,
	GameModeCpuSelect,
	GameModeFldCardSet,
	GameModeDispSum,
	GameModeEnd,
};

//	テクスチャ番号
#define	TEX_GAME_BG				0
#define	TEX_GAME_SYSTEM			1
#define	TEX_GAME_CARD			2

//	パターン番号
#define	PAT_GAME_BG				0
#define	PAT_GAME_DECK_PLAYER	5
#define	PAT_GAME_DECK_CPU		(PAT_GAME_DECK_PLAYER+1)
#define	PAT_GAME_NUMBER0		10
#define	PAT_GAME_NUMBER1		(PAT_GAME_NUMBER0+1)
#define	PAT_GAME_NUMBER2		(PAT_GAME_NUMBER1+1)
#define	PAT_GAME_NUMBER3		(PAT_GAME_NUMBER2+1)
#define	PAT_GAME_NUMBER4		(PAT_GAME_NUMBER3+1)
#define	PAT_GAME_NUMBER5		(PAT_GAME_NUMBER4+1)
#define	PAT_GAME_NUMBER6		(PAT_GAME_NUMBER5+1)
#define	PAT_GAME_NUMBER7		(PAT_GAME_NUMBER6+1)
#define	PAT_GAME_NUMBER8		(PAT_GAME_NUMBER7+1)
#define	PAT_GAME_NUMBER9		(PAT_GAME_NUMBER8+1)
#define	PAT_GAME_PLUS			(PAT_GAME_NUMBER9+1)
#define	PAT_GAME_MINUS			(PAT_GAME_PLUS+1)
#define	PAT_GAME_NUMBER_NONE	(PAT_GAME_MINUS+1)
#define	PAT_GAME_HEART			30
#define	PAT_GAME_DIAMOND		50
#define	PAT_GAME_CLUB			70
#define	PAT_GAME_SPADE			90
#define	PAT_GAME_BUT_OK			110
#define	PAT_GAME_BUT_CANCEL		(PAT_GAME_BUT_OK+1)
#define	PAT_GAME_WIN			120
#define	PAT_GAME_LOSE			(PAT_GAME_WIN+1)
#define	PAT_GAME_DRAW			(PAT_GAME_LOSE+1)
#define	PAT_GAME_JUDGE_FLD		(PAT_GAME_DRAW+1)
#define	PAT_GAME_COUNT_STR		130
#define	PAT_GAME_END_STR		135

//	ポリゴン番号
#define	POL_GAME_BG				0
#define	POL_GAME_DECK_PLAYER	5
#define	POL_GAME_DECK_CPU		(POL_GAME_DECK_PLAYER+1)
#define	POL_GAME_POINT_PLAYER	10
#define	POL_GAME_POINT_CPU		20
#define	POL_GAME_HAND_PLAYER	40
#define	POL_GAME_HAND_CPU		45
#define	POL_GAME_BUT_OK			60
#define	POL_GAME_BUT_CANCEL		(POL_GAME_BUT_OK+1)
#define	POL_GAME_BUT_FLD		(POL_GAME_BUT_CANCEL+1)
#define	POL_GAME_CALC_PLAYER	70
#define	POL_GAME_CALC_CPU		75
#define	POL_GAME_JUDGE			80
#define	POL_GAME_JUDGE_FLD		(POL_GAME_JUDGE+1)
#define	POL_GAME_PLAYER_OPT		100
#define	POL_GAME_PLAYER_ADD1	(POL_GAME_PLAYER_OPT+1)
#define	POL_GAME_PLAYER_ADD2	(POL_GAME_PLAYER_ADD1+1)
#define	POL_GAME_PLAYER_ADD3	(POL_GAME_PLAYER_ADD2+1)
#define	POL_GAME_CPU_OPT		105
#define	POL_GAME_CPU_ADD1		(POL_GAME_CPU_OPT+1)
#define	POL_GAME_CPU_ADD2		(POL_GAME_CPU_ADD1+1)
#define	POL_GAME_CPU_ADD3		(POL_GAME_CPU_ADD2+1)
#define	POL_GAME_COUNT1			110
#define	POL_GAME_COUNT2			(POL_GAME_COUNT1+1)
#define	POL_GAME_COUNT_STR		(POL_GAME_COUNT2+1)
#define	POL_GAME_COUNT_FLD		(POL_GAME_COUNT_STR+1)
#define	POL_GAME_END_STR		115

////////////////////////////////////////
//	使用カード数(1～13)
#define	USE_NUMBER		13
//	使用マーク数
#define	USE_MARK		4

////////////////////////////////////////
//	山札最大数
#define	CARD_MAX		(USE_NUMBER*USE_MARK)

enum
{	//	プレイヤー
	GamePlayer =0,
	//	CPU
	GameCpu,
	//	プレイ最大数
	MaxPlayer
};

//	手札最大数
#define	HAND_NUM_MAX	4

////////////////////////////////////////
//	パターンサイズ

//	カードサイズ
#define	CARD_SIZE_W			48
#define	CARD_SIZE_H			56
//	プレイヤー山札サイズ
#define	DECK_PLAYER_PAT_X	0
#define	DECK_PLAYER_PAT_Y	224
#define	DECK_PLAYER_PAT_W	CARD_SIZE_W
#define	DECK_PLAYER_PAT_H	CARD_SIZE_H
//	CPU山札サイズ
#define	DECK_CPU_PAT_X		48
#define	DECK_CPU_PAT_Y		224
#define	DECK_CPU_PAT_W		CARD_SIZE_W
#define	DECK_CPU_PAT_H		CARD_SIZE_H
//	数字サイズ
#define	NUMBER_SIZE_W		16
#define	NUMBER_SIZE_H		24
//	ボタンサイズ
#define	BUT_OK_PAT_X		0
#define	BUT_OK_PAT_Y		64
#define	BUT_OK_PAT_W		88
#define	BUT_OK_PAT_H		40
#define	BUT_CANSEL_PAT_X	88
#define	BUT_CANSEL_PAT_Y	64
#define	BUT_CANSEL_PAT_W	88
#define	BUT_CANSEL_PAT_H	40
#define	BUT_FLD_SIZE_W		208
#define	BUT_FLD_SIZE_H		56
//	勝敗用サイズ
#define	WIN_PAT_X			0
#define	WIN_PAT_Y			104
#define	WIN_PAT_W			120
#define	WIN_PAT_H			32
#define	LOSE_PAT_X			0
#define	LOSE_PAT_Y			136
#define	LOSE_PAT_W			120
#define	LOSE_PAT_H			32
#define	DRAW_PAT_X			0
#define	DRAW_PAT_Y			168
#define	DRAW_PAT_W			120
#define	DRAW_PAT_H			32
#define	WIN_FLD_PAT_X		0
#define	WIN_FLD_PAT_Y		200
#define	WIN_FLD_PAT_W		216
#define	WIN_FLD_PAT_H		32
//	戦目
#define	SENME_PAT_X			192
#define	SENME_PAT_Y			0
#define	SENME_PAT_W			48
#define	SENME_PAT_H			24
//	終了文字
#define	END_STR_PAT_X		0
#define	END_STR_PAT_Y		24
#define	END_STR_PAT_W		288
#define	END_STR_PAT_H		40

////////////////////////////////////////
//	表示座標

//	山札
#define	DECK_PLAYER_POS_X	488
#define	DECK_PLAYER_POS_Y	344
#define	DECK_CPU_POS_X		104
#define	DECK_CPU_POS_Y		80
//	ポイント表示
#define	POINT_PLAYER_POS_X	104
#define	POINT_PLAYER_POS_Y	256
#define	MINUS_PLAYER_POS_X	88
#define	MINUS_PLAYER_POS_Y	256
#define	POINT_CPU_POS_X		576
#define	POINT_CPU_POS_Y		200
#define	MINUS_CPU_POS_X		560
#define	MINUS_CPU_POS_Y		200
//	プレイヤーポイント加算値
#define	OPT_PLAYER_POS_X	88
#define	OPT_PLAYER_POS_Y	224
#define	ADD1_PLAYER_POS_X	104
#define	ADD1_PLAYER_POS_Y	(OPT_PLAYER_POS_Y)
#define	ADD2_PLAYER_POS_X	120
#define	ADD2_PLAYER_POS_Y	(OPT_PLAYER_POS_Y)
#define	ADD3_PLAYER_POS_X	136
#define	ADD3_PLAYER_POS_Y	(OPT_PLAYER_POS_Y)
//	CPUポイント加算値
#define	OPT_CPU_POS_X		560
#define	OPT_CPU_POS_Y		232
#define	ADD1_CPU_POS_X		576
#define	ADD1_CPU_POS_Y		(OPT_CPU_POS_Y)
#define	ADD2_CPU_POS_X		592
#define	ADD2_CPU_POS_Y		(OPT_CPU_POS_Y)
#define	ADD3_CPU_POS_X		608
#define	ADD3_CPU_POS_Y		(OPT_CPU_POS_Y)
//	OK・CENCELダイアログ
#define	BUT_FLD_POS_X		216
#define	BUT_FLD_POS_Y		296
#define	BUT_OK_POS_X		224
#define	BUT_OK_POS_Y		304
#define	BUT_CANCEL_POS_X	328
#define	BUT_CANCEL_POS_Y	(BUT_OK_POS_Y)
//	カード合計値
#define	CALC_PLAYER_POS_X	304
#define	CALC_PLAYER_POS_Y	256
#define	CALC_CPU_POS_X		304
#define	CALC_CPU_POS_Y		200
//	勝敗
#define	WIN_POS_X			256
#define	WIN_POS_Y			224
#define	WIN_FLD_POS_X		208
#define	WIN_FLD_POS_Y		224
//	戦目
#define	COUNT_POS_X			280
#define	COUNT_POS_Y			228

////////////////////////////////////////
//	デバッグフラグ
#ifdef	_DEBUG
#define	__DEBUG__
#endif

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	ゲーム管理構造体
typedef	struct
{
	long		MainMode;				//	メインモード
	long		GameMode;				//	ゲームモード
	long		Point[MaxPlayer];		//	ポイント
	long		PointAdd[MaxPlayer];	//	ポイント加算値
	long		CardMax[MaxPlayer];		//	山札最大数
	UChar		Judge;					//	判定フラグ
	long		GameCount;				//	ゲーム回数
	long		GameModeState;			//	汎用フラグ
}GAME_MNG_TBL;

////////////////////////////////////////
//	山札管理構造体
typedef	struct
{
	long		Mark;					//	マーク
	long		Number;					//	数字
}DECK_TBL;

////////////////////////////////////////
//	キャラクター管理構造体
typedef	struct
{
	long		DeckIndex;
	long		HandNumber;
	long		StateFlag[HAND_NUM_MAX];	//	汎用状態フラグ(Player用)
	long		SelCardCt;
	long		SelectCard[HAND_NUM_MAX];
	double		HandCardRotR[HAND_NUM_MAX];
	DECK_TBL	Deck[CARD_MAX];
}CHARA_TBL;

#endif	// __GameMain_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
