/******************************************************************************/
//	DirectX9ライブラリ
//		スプライト
/******************************************************************************/

#ifndef	__Dx9LibSprite_H__
#define	__Dx9LibSprite_H__

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	スプライト用構造体
typedef	struct
{
	UChar	Use;						//	使用フラグ
	UChar	Disp;						//	表示フラグ
	long	PatNo;						//	パターン番号
	long	Width,Height;				//	サイズ
	long	CenterX,CenterY;			//	中心座標
	UChar	A;							//	アルファ
	UChar	R,G,B;						//	各色成分
	double	PosX,PosY,PosZ;				//	表示座標
	double	MoveX,MoveY;				//	移動量
	double	PosXBk,PosYBk;				//	表示座標のバックアップ
	double	ScaleX,ScaleY;				//	スケール
	double	Rot;						//	角度
	UShort	Atr;						//	アトリビュート
}ST_SpriteInfo;

#endif	//	__Dx9LibSprite_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	ST_SpriteInfo		SP[SP_MAX];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	CreateSpriteDevice( void );
extern	void	InitSpriteAll( void );
extern	void	InitSprite( long SpriteNo );
extern	void	DispSpriteAll( void );
extern	void	DispSprite( long SpriteNo );
extern	void	SetSprite( long SpriteNo, long PatNo, double PosX, double PosY, double PosZ, UShort Atr );
extern	void	SetSpriteCenter( long SpriteNo, long CenterX, long CenterY );
extern	void	SetSpriteColor( long SpriteNo, UChar A, UChar R, UChar G, UChar B );
extern	void	SetSpriteScale( long SpriteNo, double Scale );
extern	void	SetSpritePos( long SpriteNo, double PosX, double PosY );
extern	long	GetSpriteNo( void );
extern	void	DispChangeSprite( UChar Flag );
extern	void	ReleaseSprite( void );
