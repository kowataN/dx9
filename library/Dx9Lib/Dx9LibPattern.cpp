/******************************************************************************/
//	DirectX9ライブラリ
//		パターン
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
ST_PatternInfo	PAT[PAT_MAX];



/******************************************************************************/
//	名前	：	パターン初期化
//	説明	：	パターン構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PatNo	パターン番号
//	備考	：	なし
/******************************************************************************/
void	InitPattern( long PatNo )
{
	if( PatNo >= PAT_MAX )
	{
		return;
	}

	//	使用フラグOFF
	PAT[ PatNo ].Use		= OFF;

	//	テクスチャ番号
	PAT[ PatNo ].TextureNo	= 0;

	//	切り取り開始座標
	PAT[ PatNo ].PosX		= 0;
	PAT[ PatNo ].PosY		= 0;

	//	サイズ
	PAT[ PatNo ].Width		= 0;
	PAT[ PatNo ].Height		= 0;
}

/******************************************************************************/
//	名前	：	パターン全初期化
//	説明	：	パターン構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	InitPatternAll( void )
{
	for( long i=0; i<PAT_MAX; i++ )
	{
		InitPattern( i );
	}
}

/******************************************************************************/
//	名前	：	パターン設定
//	説明	：	パターンの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PatNo		パターン番号
//				[IN]long	TexureNo	テクスチャ番号
//				[IN]long	PosX		切り取り開始座標Ｘ
//				[IN]long	PosY		切り取り開示座標Ｙ
//				[IN]long	Width		横サイズ
//				[IN]long	Height		縦サイズ
//	備考	：	なし
/******************************************************************************/
void	SetPattern(long PatNo,
				   long TexureNo,
				   long PosX, long PosY,
				   long Width, long Height )
{
	if( PatNo >= PAT_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	各メンバ初期化
	PAT[ PatNo ].Use		= ON;
	PAT[ PatNo ].TextureNo	= TexureNo;

	PAT[ PatNo ].PosX		= PosX;
	PAT[ PatNo ].PosY		= PosY;
	PAT[ PatNo ].Width		= Width;
	PAT[ PatNo ].Height		= Height;
}

/******************************************************************************/
//	名前	：	パターンデータコピー
//	説明	：	パターンデータのコピーを行う
//	戻り値	：	なし
//	引数	：	[IN]long	DstPatNo	コピー先パターン番号
//				[IN]long	SrcPatNo	コピー元パターン番号
//	備考	：	なし
/******************************************************************************/
void	CopyPatternData( long DstPatNo, long SrcPatNo )
{
	PAT[ DstPatNo ] = PAT[ SrcPatNo ];
}

/******************************************************************************/
//	名前	：	空きパターン番号取得
//	説明	：	空きパターン番号を取得する
//	戻り値	：	long	空き番号
//						-1で空き番号なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
long	GetPatternNo( void )
{
	for( long i=0; i<PAT_MAX; i++ )
	{
		if( !PAT[i].Use )
		{
			return i;
		}
	}
	return NOT;
}
