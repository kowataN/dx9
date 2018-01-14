/******************************************************************************/
//	カードゲーム
//		ゲームメインヘッダファイル
/******************************************************************************/

#ifndef	__GameMain_H__
#define	__GameMain_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	ゲームモードフラグ
enum	E_GameMode
{
	GameModeInit,
	GameModeMain,
	GameModeBlink,
	GameModeCardClear,
	GameModeEnd,
};
////////////////////////////////////////
//	テクスチャ番号
enum
{
	TEX_BG = 0,
	TEX_CARD,
	TEX_SYSTEM,
	TEX_ANM,
};

////////////////////////////////////////
//	パターン番号
#define	PAT_BG			0
#define	PAT_DECK		(PAT_BG+1)
#define	PAT_CARD_RED	10
#define	PAT_CARD_GREEN	20
#define	PAT_CARD_BLUE	30
#define	PAT_CARD_SKY	40
#define	PAT_CARD_BLACK	50
#define	PAT_CARD_YELLOW	60
#define	PAT_NEXT1		70
#define	PAT_NEXT2		(PAT_NEXT1+1)
#define	PAT_NEXT3		(PAT_NEXT2+1)
#define	PAT_ARROW		100
#define	PAT_NUMBER0		150
#define	PAT_NUMBER1		(PAT_NUMBER0+1)
#define	PAT_NUMBER2		(PAT_NUMBER1+1)
#define	PAT_NUMBER3		(PAT_NUMBER2+1)
#define	PAT_NUMBER4		(PAT_NUMBER3+1)
#define	PAT_NUMBER5		(PAT_NUMBER4+1)
#define	PAT_NUMBER6		(PAT_NUMBER5+1)
#define	PAT_NUMBER7		(PAT_NUMBER6+1)
#define	PAT_NUMBER8		(PAT_NUMBER7+1)
#define	PAT_NUMBER9		(PAT_NUMBER8+1)
#define	PAT_NUMBER_NONE	(PAT_NUMBER9+1)
#define	PAT_PLUS		(PAT_NUMBER_NONE+1)
#define	PAT_CHAIN		200
#define	PAT_END			210

////////////////////////////////////////
//	ポリゴン番号
#define	POL_BG			0
#define	POL_DECK		(POL_BG+1)
#define	POL_NEXT1		10
#define	POL_NEXT2		(POL_NEXT1+1)
#define	POL_NEXT3		(POL_NEXT2+1)
#define	POL_CARD		100
#define	POL_ARROW		200
#define	POL_BLINK		220
#define	POL_CLEAR		225
#define	POL_NUMBER		230
#define	POL_CHAIN		250
#define	POL_PLUS		260
#define	POL_ADD1		(POL_PLUS+1)
#define	POL_ADD2		(POL_ADD1+1)
#define	POL_ADD3		(POL_ADD2+1)
#define	POL_DECKNUM1	270
#define	POL_DECKNUM2	(POL_DECKNUM1+1)
#define	POL_END			280

////////////////////////////////////////
//	フラグ用
#define	CARD_NONE	(0)		//	何もなし
#define	CARD_MARK	(1<<0)	//	マーク
#define	CARD_NUM	(1<<1)	//	番号

////////////////////////////////////////
//	フィールドサイズ
#define	FLD_WIDTH	3
#define	FLD_HEIGHT	3

////////////////////////////////////////
//	色の最大数
enum
{
	CARD_COLOR_RED,
	CARD_COLOR_GREEN,
	CARD_COLOR_BLUE,
	CARD_COLOR_SKY,
	CARD_COLOR_BLACK,
	CARD_COLOR_YELLOW,
	CARD_COLOR_MAX,
};

#define	CARD_W_SIZE		48
#define	CARD_H_SIZE		60
#define	NUM_W_SIZE		16
#define	NUM_H_SIZE		24
#define	CHAIN_W_SIZE	80
#define	CHAIN_H_SIZE	24
#define	END_W_SIZE		288
#define	END_H_SIZE		40

#define	DECK_X_POS		444
#define	DECK_Y_POS		336
#define	BLINK_X_POS		72
#define	BLINK_Y_POS		120
#define	SCORE_X_POS		432
#define	SCORE_Y_POS		134
#define	PLUS_X_POS		528
#define	PLUS_Y_POS		102
#define	ADD1_X_POS		544
#define	ADD2_X_POS		560
#define	ADD3_X_POS		576
#define	DECKNUM1_X_POS	560
#define	DECKNUM1_Y_POS	360
#define	DECKNUM2_X_POS	576
#define	DECKNUM2_Y_POS	360
#define	NEXT1_X_POS		348
#define	NEXT1_Y_POS		336
#define	NEXT2_X_POS		384
#define	NEXT2_Y_POS		336
#define	NEXT3_X_POS		420
#define	NEXT3_Y_POS		336

#define	ADD_Y_POS		4
#define	ADD_COLOR		24
#define	ADD_COLOR2		46

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	ゲーム管理構造体
typedef	struct
{
	long		Score;			//	スコア
	long		ScoreAdd;		//	スコア加算値
	long		Level;			//	ゲームレベル
	long		CardMax;		//	山札最大数
	long		UseCardNumber;	//	使用カード番号
	long		UseSameCard;	//	使用同名カード数
}GAME_MNG_TBL;

////////////////////////////////////////
//	矢印管理用構造体
typedef	struct
{
	UChar		Use;			//	使用フラグ
	long		PolNo;			//	ポリゴン番号
}ARROW_TBL;

////////////////////////////////////////
//	カード用構造体
typedef	struct
{
	UChar		Use;			//	使用フラグ
	short		No;				//	番号(0〜8)
	short		Color;			//	色(0〜5)
	ARROW_TBL	Arrow[4];		//	矢印
}CARD_TBL;

////////////////////////////////////////
//	フラグ用構造体
typedef	struct
{
	short		Trg;			//	トリガフラグ
	UChar		Set;			//	設置フラグ
	UChar		Judge;			//	判定フラグ
}FLAG_TBL;

////////////////////////////////////////
//	フィールドカード消去用構造体
typedef	struct
{
	short		PosX;			//	X座標
	short		PosY;			//	Y座標
	ULong		State;			//	状態
}FLD_CLS_TBL;

#endif	// __GameMain_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	GAME_MNG_TBL	g_GameMng;

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	GameLoop( void );
