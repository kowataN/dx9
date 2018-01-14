/******************************************************************************/
//	DirectX9ライブラリ
//		ライン
/******************************************************************************/

#ifndef	__Dx9LibLine_H__
#define	__Dx9LibLine_H__

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	ライン構造体
typedef struct
{
	UChar	Use;						//	使用フラグ
	UChar	Disp;						//	表示フラグ
	double	StartX;						//	開始点X
	double	StartY;						//	開始点Y
	double	EndX;						//	終点X
	double	EndY;						//	終点Y
	UChar	A;							//	アルファ
	UChar	R,G,B;						//	各色成分
	UShort	Atr;						//	アトリビュート
}ST_LineInfo;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class CDx9LibLine
{
protected:
	LPD3DXLINE		m_lpLine;

public:
	CDx9LibLine();
	~CDx9LibLine();
	void	CreateDevice( void );
	void	InitAll( void );
	void	Init( long No );
	void	DispAll( void );
	void	DrawLine( long No );
	void	DrawBox( long No );
	void	Set( long No, double StartX, double StartY, double EndX, double EndY, UShort Atr );
	void	SetColor( long No, UChar A, UChar R, UChar G, UChar B );
	short	GetNo( void );
	void	DispChange( UChar Flag );
	void	ReleaseDevice( void );
};

#endif	//	__Dx9LibLine_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	ST_LineInfo	LINE[LINE_MAX];
extern	CDx9LibLine	g_LibLine;
