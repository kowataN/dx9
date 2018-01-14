/******************************************************************************/
//	DirectX9ライブラリ
//		パターン
/******************************************************************************/

#ifndef	__Dx9LibPattern_H__
#define	__Dx9LibPattern_H__

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	パターン用構造体
typedef	struct
{
	UChar	Use;			//	パターンの使用フラグ
	long	TextureNo;		//	テクスチャ番号
	long	PosX,PosY;		//	切り取り開始座標
	long	Width,Height;	//	サイズ
}ST_PatternInfo;

#endif	//	__Dx9LibPattern_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	ST_PatternInfo	PAT[PAT_MAX];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	InitPattern( long PatNo );
extern	void	InitPatternAll( void );
extern	void	SetPattern( long PatNo, long TexureNo, long PosX, long PosY, long Width, long Height );
extern	void	CopyPatternData( long DstPatNo, long SrcPatNo );
extern	long	GetPatternNo( void );
