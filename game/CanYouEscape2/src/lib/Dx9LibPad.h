/******************************************************************************/
//	パッドライブラリ
//		ヘッダファイル
/******************************************************************************/


#ifndef	__Dx9LibPad_H__
#define	__Dx9LibPad_H__


//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//

//	パッド最大数
#define	PAD_MAX		2

//	キーリピート
#define	KEY_REPEAT_START	20
#define	KEY_REPEAT_TIME		15

//	ボタン番号
enum
{
	PAD_UP = 0,
	PAD_DOWN,
	PAD_LEFT,
	PAD_RIGHT,

	PAD_BUT1,
	PAD_BUT2,
	PAD_BUT3,
	PAD_BUT4,
	PAD_BUT5,
	PAD_START,
	PAD_BUTTON_MAX
};

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	パッド構造体
typedef	struct
{
	UChar	Use;	//	使用フラグ
	UChar	JoyUse;	//	ジョイスティック使用フラグ

	UChar	Trg[PAD_BUTTON_MAX];	//	トリガ
	UChar	TrgBk[PAD_BUTTON_MAX];	//	トリガ（バックアップ）
	UChar	Push[PAD_BUTTON_MAX];	//	プッシュ
	UChar	PushBk[PAD_BUTTON_MAX];	//	プッシュ（バックアップ）
	UChar	Rpt[PAD_BUTTON_MAX];	//	リピート
	long	Count[PAD_BUTTON_MAX];	//	カウンタ

	short	KeyNo[PAD_BUTTON_MAX];	//	割り当てられているキーボード番号
	short	JoyNo[PAD_BUTTON_MAX];	//	割り当てられているジョイスティックのキー番号
}PAD_TBL;

#endif	//	__Dx9LibPad_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	short	SetKeyDat[PAD_MAX][PAD_BUTTON_MAX];
extern	short	SetJoyDat[PAD_MAX][PAD_BUTTON_MAX];
extern	PAD_TBL	PAD[PAD_MAX];

//----------------------------------------------------------------------------//
//	外部宣言
//----------------------------------------------------------------------------//
extern	void	InitPadAll( void );
extern	void	InitPad( long PadNo );
extern	void	LoopPad( void );
extern	void	LoadPadData( char* FileName );
