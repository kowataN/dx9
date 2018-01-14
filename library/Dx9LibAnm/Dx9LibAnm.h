/******************************************************************************/
//	アニメライブラリ
//		ヘッダファイル
/******************************************************************************/

#ifndef	__Dx9LibAnm_H__
#define	__Dx9LibAnm_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

//	アニメ最大数
#define	ANM_MAX		100

//	フラグ
enum
{
	ANM_LOOP = 0,	//	ループ
	ANM_STOP,		//	停止
	ANM_END,		//	終了
};

//	各インデックスサイズ
#define	PAT_INDEX_SIZE		(2+2+2+2)
#define	ANM_INDEX_SIZE		(4+8+8+8+8+8+4+8+8+8+8+2)

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	アニメテクスチャデータ
typedef	struct
{
	long	TextureNo;					//	テクスチャ番号
	char	FileName[MAX_PATH];			//	ファイル名
}ST_AnmTexture;

////////////////////////////////////////
//	アニメパターンデータ用構造体
typedef	struct
{
	long	TextureNo;					//	テクスチャ番号
	short	PosX,PosY;					//	切り取り開始座標
	short	Width,Height;				//	サイズ
}ST_AnmPattern;

////////////////////////////////////////
//	アニメーション用フレーム構造体
typedef	struct
{
	long	PatNo;						//	パターン番号
	double	CorrectX;					//	補正座標Ｘ
	double	CorrectY;					//	補正座標Ｙ
	double	ScaleX;						//	拡大率Ｘ
	double	ScaleY;						//	拡大率Ｙ
	double	Rot;						//	回転角度
	long	Time;						//	表示フレーム数
	double	A;							//	不透明度
	double	R,G,B;						//	各色成分
	short	Rev;						//	反転フラグ
}ST_AnmFrame;

////////////////////////////////////////
//	アニメーション用構造体
typedef	struct
{
	UChar			Use;				//	使用フラグ
	UChar			Flag;				//	フラグ（0:ループ、1:停止、2:終了）
	UChar			State;				//	状態（0:停止、1:再生中）

	long			FileNo;				//	読み込んだファイル番号
	long			FrameMax;			//	フレームの最大数
	long			FrmNo;				//	フレーム番号
	long			DataNo;				//	アニメデータ番号

	long			PolygonNo;			//	ポリゴン番号
	long			PatNo;				//	パターン番号

	ST_AnmFrame		FRM;				//	フレームデータ(カレントデータ)
	ST_AnmPattern	PAT;				//	パターンデータ(カレントデータ)

	long			TimeCt;				//	タイムカウンタ

	UShort			Atr;				//	アトリビュート
	char			*AnmAddr;			//	アニメデータのアドレス
}ST_AnmData;

#endif	//	__Dx9LibAnm_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	アニメーション用構造体
extern	ST_AnmData	ANM[ANM_MAX];
extern	char		AnmFilePath[MAX_PATH];
extern	char		AnmFileName[MAX_PATH];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	InitAnmAll( void );
extern	void	InitAnm( long AnmNo );
extern	void	LoadAnm( long FileNo, char *FileName, char *Buf, long TextureNo );
extern	void	SetAnm( long AnmNo, long FileNo, long DataNo, long PatNo, char *AnmAddr );
extern	void	SetAnmPolygon( long PolNo, long AnmNo, double PosX, double PosY, double PosZ, UChar Flag );
extern	void	LoopAnm( void );
extern	void	CopyAnmData( long AnmNo, long CopyAnmNo );

