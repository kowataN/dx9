/******************************************************************************/
//	DirectX9ライブラリ
//		テクスチャ
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
ST_TextureInfo		TEX[TEX_MAX];	//	テクスチャ



/******************************************************************************/
//	名前	：	テクスチャ初期化
//	説明	：	テクスチャ構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	TexNo		テクスチャ番号
//	備考	：	なし
/******************************************************************************/
void	InitTexture( long TexNo )
{
	if( TexNo >= TEX_MAX )
	{
		return;
	}

	TEX[ TexNo ].Use = OFF;

	TEX[ TexNo ].Width = 0;
	TEX[ TexNo ].Height = 0;

	TEX[ TexNo ].CenterX = 0;
	TEX[ TexNo ].CenterY = 0;

	TEX[ TexNo ].lpTex = NULL;
}

/******************************************************************************/
//	名前	：	テクスチャ全初期化
//	説明	：	テクスチャ構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	InitTextureAll( void )
{
	for( long i=0; i<TEX_MAX; i++ )
	{
		InitTexture( i );
	}
}

/******************************************************************************/
//	名前	：	テクスチャ座標取得
//	説明	：	テクスチャ座標の取得を行う
//	戻り値	：	long		テクスチャ座標
//	引数	：	[IN]long	TexSize		テクスチャのサイズ
//	備考	：	なし
/******************************************************************************/
long	GetTexPos( long TexSize )
{
	long	size[2],
			ret=256;

	size[0] = TexSize / 256;
	size[1] = TexSize % 256;

	switch( size[0] )
	{
		case	1:
			if( size[1] == 0 )
			{
				ret = 256;
			}
			else
			{
				ret = 512;
			}
			break;

		case	2:
			if( size[1] == 0 )
			{
				ret = 512;
			}
			else
			{
				ret = 1024;
			}
			break;

		case	4:
			if( size[1] == 0 )
			{
				ret = 1024;
			}
			else
			{
				ret = 2048;
			}
			break;
	}

	return	ret;
}

/******************************************************************************/
//	名前	：	テクスチャロード
//	説明	：	画像ファイルの読み込みを行う
//	戻り値	：	bool		処理結果	true(成功)
//										false(失敗)
//	引数	：	[IN]long	TexNo		テクスチャ番号
//				[IN]char	*FileName	ファイル名
//	備考	：	なし
/******************************************************************************/
bool	LoadTexture( long TexNo, char *FileName )
{
	char	str[4] = { NULL };
	long	size = 0;

	if( TexNo >= TEX_MAX )
	{
		return false;
	}

	//	使用フラグ
	TEX[ TexNo ].Use = ON;

	PathToOpt( FileName, str );

	char *string;
	errno_t Err = _strlwr_s( string = _strdup(str), strlen(str)+1 );
	if( Err != 0 )
	{
		SAFE_DELETE(string);
		return false;
	}

	//	カレントディレクトリ・バックアップ
	char	name[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, name );

	//	暗号ファイルかどうか識別
	if( strcmp( "img", string ) == 0 )
	{
		size = AntiSecret( FileName, g_FreeWork );
	}
	else
	{
		size = LoadFile( FileName, g_FreeWork );
	}

	if( size < 0 )
	{
		//	読み込み失敗
		SAFE_DELETE(string);
		return false;
	}

	BITMAPINFOHEADER	*bmp_info;
	bmp_info = (BITMAPINFOHEADER *)&g_FreeWork[sizeof(BITMAPFILEHEADER)];

	//	メモリーからテクスチャ読み込み
	D3DXCreateTextureFromFileInMemoryEx(
		g_lpD3DDev,			//	DirectX9デバイス
		g_FreeWork,			//	メモリ内のポインタ
		size,				//	メモリ内のファイルサイズ
		D3DX_DEFAULT,		//	幅
		D3DX_DEFAULT,		//	高さ
		//D3DX_DEFAULT,		//	ミップマップレベル数
		1,					//	ミップマップレベル数
		0,					//	用法
		D3DFMT_A8R8G8B8,	//	ピクセルフォーマット
		D3DPOOL_MANAGED,	//	テクスチャ配置先
		D3DX_FILTER_NONE,	//	フィルタリング
		D3DX_FILTER_NONE,	//	フィルタリング（ミップマップ）
		TRANS,				//	透明色
		NULL,				//	イメージ情報
		NULL,				//	パレット
		&TEX[ TexNo ].lpTex	//	テクスチャオブジェクト
		);

	D3DSURFACE_DESC	Desc;
	TEX[ TexNo ].lpTex->GetLevelDesc( 0, &Desc );

	//D3DXIMAGE_INFO	Info;
	//D3DXGetImageInfoFromFile(FileName, &Info );

		//	サイズ取得
	//TEX[ TexNo ].Width = GetTexPos( Info->biWidth );
	//TEX[ TexNo ].Height = GetTexPos( bmp_info->biHeight );
	TEX[ TexNo ].Width = Desc.Width;
	TEX[ TexNo ].Height = Desc.Height;

	SAFE_DELETE(string);
	return	true;
}

/******************************************************************************/
//	名前	：	オブジェクト解放
//	説明	：	オブジェクトの解放を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	；	なし
/******************************************************************************/
void	ReleaseTexture( void )
{
	for( int i=0; i<TEX_MAX; i++ )
	{
		if( TEX[i].lpTex != NULL )
		{
			SAFE_RELEASE(TEX[i].lpTex);
		}
	}
}
