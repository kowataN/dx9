/******************************************************************************/
//	DirectX9ライブラリ
//		テキスト
/******************************************************************************/

#ifndef	__Dx9LibText_H__
#define	__Dx9LibText_H__

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	<string>
#include	<vector>
using namespace std;

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

//	テキストアトリビュート
#define	ATR_ONCE	1	//	１文字づつ表示
//#define	ATR_ITALI	1	//	イタリック
//#define	ATR_UNDAR	2	//	アンダーライン
//#define	ATR_DENY	4	//	打ち消し線

//	カラー設定
#define	COLOR_RED	D3DCOLOR_ARGB( 255,255,0,0)
#define	COLOR_GREEN	D3DCOLOR_ARGB( 255,0,255,0)
#define	COLOR_BLUE	D3DCOLOR_ARGB( 255,0,0,255)

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//
typedef	struct
{
	UChar	Disp;						//	表示フラグ
	UChar	Use;						//	使用フラグ
	double	PosX,PosY;					//	表示座標
	double	CenterX,CenterY;			//	中心座標
	long	Width,Height;				//	表示サイズ
	long	Time;						//	表示時間
	UChar	A,R,G,B;					//	各色成分
	char	Str[MAX_PATH];				//	文字列
	long	Step;						//	現在の表示文字数
	long	Speed;						//	表示速度
	long	Count;						//	カウンタ
	UChar	State;						//	表示状態(0:表示中、1:表示完了)
	UChar	Atr;						//	アトリビュート
}ST_TextInfo;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CDx9LibText
{
private:
	LPD3DXFONT		m_lpFont;		//	テキストデバイス
	LPD3DXSPRITE	m_lpTxtSprite;	//	スプライトオブジェクト

public:
	CDx9LibText();
	~CDx9LibText();

	void	InitDevice( void );
	void	InitAll( void );
	void	Init( long TextNo );
	void	DispAll( void );
	void	Set( long TextNo, double PosX, double PosY, UChar A, UChar R, UChar G, UChar B, char *Str, UChar Atr=ATR_NONE, long Speed=1 );
	void	ChangeDisp( UChar Flag );
	void	ChangeDisp( long TextNo, UChar Flag );
	void	SetColor( long TextNo, UChar A, UChar R, UChar G, UChar B );
	void	SetPos( long TextNo, double PosX, double PosY );
	void	SetCenter( long TextNo );
	void	SetRect( long TextNo, long Width, long Height );
	void	CopyData( long TextNo, long CopyNo );
	void	SetTimer( long TextNo, long Time );
	void	Release();
private:
	void	Disp( long TextNo );
};

#endif	//	__Dx9LibText_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	ST_TextInfo		TXT[TXT_MAX];
extern	CDx9LibText	g_LibText;
