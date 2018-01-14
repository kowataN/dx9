/******************************************************************************/
//	当たり判定ライブラリ
//		ヘッダファイル
/******************************************************************************/

#ifndef	__Dx9LibHit_H__
#define	__Dx9LibHit_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

//	当たり判定最大数
#define	HIT_MAX		1024

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	当たり判定領域用構造体
typedef	struct
{
	short			PosX[2];		//	中心からの左端、右端の距離
	short			PosY[2];		//	中心からの上端、下端の距離
}ST_HitArea;

////////////////////////////////////////
//	当たり判定用構造体
typedef	struct
{
	UChar			Use;			//	使用フラグ
	UChar			Flag;			//	フラグ（0:停止、1:ループ）
	UChar			State;			//	状態（0:停止、1:再生中）

	long			PolNo;			//	ポリゴン番号
	long			AnmNo;			//	アニメ番号
	long			FrameNo;		//	フレーム番号

	ST_HitArea		AREA;			//	当たり判定領域
}ST_HitData;

////////////////////////////////////////
//	当たり判定読み込み用構造体
typedef	struct
{
	UChar			Use;			//	使用フラグ
	ST_HitData		HDT[HIT_MAX];	//	読み込み用構造体
}ST_HitLoad;

#endif	//	__Dx9LibHit_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	char		HitFilePath[MAX_PATH];	//	パス
extern	ST_HitData	HIT[HIT_MAX];
extern	ST_HitLoad	HLT[HIT_MAX];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	InitHit( long HitNo );
extern	void	InitHitAll( void );
extern	void	LoadHitData( char *FileName, char *LoadBuf, long HitNo );
extern	void	SetHitData( long AnmNo, long HitNo, long FileNo, long HitDataNo, UChar Flag );
extern	short	CheackHit( long HitNo1, long HitNo2 );
extern	void	DispHitAll( long PolNo );
extern	void	DispHit( long HitNo, long DispPolNo );
extern	void	DispHitData( long HitNo, long TextNo, double PosX, double PosY );
