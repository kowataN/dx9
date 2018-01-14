/******************************************************************************/
//	DirectX9ライブラリ
//		フェード
/******************************************************************************/

#ifndef	__Dx9LibFade_H__
#define	__Dx9LibFade_H__

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	フェード用構造体
typedef	struct
{
	UChar	Use;						//	使用フラグ
	UChar	Disp;						//	表示フラグ
	UChar	Init;						//	初期化フラグ
	UChar	State;						//	状態（0:終了、1:フェード中）
	long	Time;						//	所要時間
	long	Count;						//	カウンタ
	double	A,R,G,B;					//	カレントの色
	double	StartA;						//	開始色(A)
	double	StartR,StartG,StartB;		//	開始色(各色成分)
	double	TargetA;					//	目標色(A)
	double	TargetR,TargetG,TargetB;	//	目標色(各色成分)
}ST_FadeInfo;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CDx9LibFade
{
private:
	ST_FadeInfo	m_Fade;

public:
	void	Init( void );				//	フェード初期化
	void	Set( long Time, double a1, double r1, double g1, double b1, double a2, double r2, double g2, double b2 );
	void	Loop( void );				//	フェードループ
	void	BlackOut( long Time );		//	ブラックアウト
	void	BlackIn( long Time );		//	ブラックイン
	void	WhiteOut( long Time );		//	ホワイトアウト
	void	WhiteIn( long Time );		//	ホワイトイン
	short	GetState( void );			//	状態取得
	long	GetProgress( void );		//	進行状況取得
private:
	void	Draw( void );				//	描画
};

#endif	//	__Dx9LibFade_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	CDx9LibFade	g_LibFade;
