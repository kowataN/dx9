/******************************************************************************/
//	DirectX9ライブラリ
//		スプライト
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
ST_SpriteInfo	SP[SP_MAX];	//	スプライト
LPD3DXSPRITE	lpSprite;	//	スプライトオブジェクト



/******************************************************************************/
//	名前	：	デバイス作成
//	説明	：	デバイスの作成を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CreateSpriteDevice( void )
{
	//	スプライト作成
	D3DXCreateSprite( g_lpD3DDev, &lpSprite );
}

/******************************************************************************/
//	名前	：	スプライト初期化
//	説明	：	スプライト構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	SpriteNo	スプライト番号
//	備考	：	なし
/******************************************************************************/
void	InitSprite( long SpriteNo )
{
	if( SpriteNo >= SP_MAX )
	{
		return;
	}

	//	フラグOFF
	SP[ SpriteNo ].Use		= OFF;
	SP[ SpriteNo ].Disp		= OFF;

	//	パターン番号
	SP[ SpriteNo ].PatNo	= 0;

	//	アトリビュート初期化
	SP[ SpriteNo ].Atr		= ATR_NONE;

	//	表示座標
	SP[ SpriteNo ].PosX		= 0.0;
	SP[ SpriteNo ].PosY		= 0.0;
	SP[ SpriteNo ].PosZ		= 1.0;

	//	移動量
	SP[ SpriteNo ].MoveX	= 0;
	SP[ SpriteNo ].MoveY	= 0;

	//	バックアップ座標
	SP[ SpriteNo ].PosXBk	= 0;
	SP[ SpriteNo ].PosYBk	= 0;

	//	中心座標
	SP[ SpriteNo ].CenterX	= 0;
	SP[ SpriteNo ].CenterY	= 0;

	//	スケーリング
	SP[ SpriteNo ].ScaleX	= 1;
	SP[ SpriteNo ].ScaleY	= 1;

	//	サイズ
	SP[ SpriteNo ].Width	= 0;
	SP[ SpriteNo ].Height	= 0;

	//	各色成分
	SP[ SpriteNo ].A		= 255;
	SP[ SpriteNo ].R		= 255;
	SP[ SpriteNo ].G		= 255;
	SP[ SpriteNo ].B		= 255;
}

/******************************************************************************/
//	名前	：	スプライト全初期化
//	説明	：	スプライト構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	InitSpriteAll( void )
{
	for( long i=0; i<SP_MAX; i++ )
	{
		InitSprite( i );
	}
}

/******************************************************************************/
//	名前	：	スプライトセット
//	説明	：	スプライトをセットする
//	戻り値	：	なし
//	引数	：	[IN]long	SpriteNo	スプライト番号
//				[IN]long	PatNo		パターン番号
//				[IN]double	PosX		表示座標X
//				[IN]double	PosY		表示座標Y
//				[IN]double	PosZ		表示座標Z
//				[IN]UShort	Atr			アトリビュート
//	備考	：	Atrの設定値	ATR_NONE	何もなし
//							ATR_REV_W	左右反転
//							ATR_REV_H	上下反転
//							ATR_ALL		上下左右反転
/******************************************************************************/
void	SetSprite(long SpriteNo, long PatNo,
				  double PosX, double PosY, double PosZ,
				  UShort Atr )
{
	if( SpriteNo >= SP_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	各メンバの初期化
	SP[ SpriteNo ].Use	= ON;		//	使用フラグＯＮ
	SP[ SpriteNo ].Disp	= ON;		//	表示フラグＯＮ

	SP[ SpriteNo ].PatNo = PatNo;	//	パターン番号

	//	スプライトのサイズ
	SP[ SpriteNo ].Width	= PAT[ PatNo ].Width;	//	横
	SP[ SpriteNo ].Height	= PAT[ PatNo ].Height;	//	縦

	//	表示座標
	SP[ SpriteNo ].PosX	= PosX;
	SP[ SpriteNo ].PosY	= PosY;
	SP[ SpriteNo ].PosZ = PosZ;

	//	アトリビュート
	SP[ SpriteNo ].Atr	= Atr;

	//	回転角度
	SP[ SpriteNo ].Rot	= 0.0f;
}

/******************************************************************************/
//	名前	：	中心座標セット
//	説明	：	中心座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	SpriteNo	スプライト番号
//				[IN]long	CenterX		中心座標X
//				[IN]long	CenterY		中心座標Y
//	備考	：	なし
/******************************************************************************/
void	SetSpriteCenter( long SpriteNo, long CenterX, long CenterY )
{
	//	中心座標
	SP[ SpriteNo ].CenterX	= CenterX;
	SP[ SpriteNo ].CenterY	= CenterY;
}

/******************************************************************************/
//	名前	：	頂点色設定
//	説明	：	デフューズ色を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	SpriteNo	スプライト番号
//				[IN]UChar	A			アルファ
//				[IN]UChar	R			赤成分
//				[IN]UChar	G			緑成分
//				[IN]UChar	B			青成分
//	備考	：	なし
/******************************************************************************/
void	SetSpriteColor( long SpriteNo, UChar A, UChar R, UChar G, UChar B )
{
	SP[ SpriteNo ].A = A;
	SP[ SpriteNo ].R = R;
	SP[ SpriteNo ].G = G;
	SP[ SpriteNo ].B = B;
}

/******************************************************************************/
//	名前	：	拡大率設定
//	説明	：	拡大率を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	SpriteNo	スプライト番号
//				[IN]double	Scale		拡大率
//	備考	：	なし
/******************************************************************************/
void	SetSpriteScale( long SpriteNo, double Scale )
{
	SP[ SpriteNo ].ScaleX = Scale;
	SP[ SpriteNo ].ScaleY = Scale;
}

/******************************************************************************/
//	名前	：	表示座標設定
//	説明	：	表示座標の設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	SpriteNo	スプライト番号
//				[IN]double	PosX		表示座標X
//				[IN]double	PosY		表示座標Y
//	備考	：	なし
/******************************************************************************/
void	SetSpritePos( long SpriteNo, double PosX, double PosY )
{
	SP[ SpriteNo ].PosX = PosX;
	SP[ SpriteNo ].PosY = PosY;
}

/******************************************************************************/
//	名前	：	空きスプライト番号取得
//	説明	：	空きスプライト番号を取得する
//	戻り値	：	long		空き番号
//							-1で空き番号なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
long	GetSpriteNo( void )
{
	for( long i=0; i<SP_MAX; i++ )
	{
		if( !SP[i].Use )
		{
			return	i;
		}
	}

	return NOT;
}

/******************************************************************************/
//	名前	：	スプライト描画
//	説明	：	スプライトの描画を行う
//	戻り値	：	なし
//	引数	：	[IN]long	SpriteNo	スプライト番号
//	備考	：	なし
/******************************************************************************/
void	DispSprite( long SpriteNo )
{
	if( !SP[SpriteNo].Use && !SP[SpriteNo].Disp )
	{
		return;
	}

	float ScaleX = 1.0;
	float ScaleY = 1.0;
	UShort m = SP[SpriteNo].Atr & ( ATR_NONE | ATR_REV_W | ATR_REV_H | ATR_ALL );
	if( m )
	{
		switch( m )
		{
			//	左右反転
			case	ATR_REV_W:
				ScaleX *= -1;
				break;

			//	上下反転
			case	ATR_REV_H:
				ScaleY *= -1;
				break;

			//	上下左右反転
			case	ATR_ALL:
				ScaleX *= -1;
				ScaleY *= -1;
				break;
		}
	}

	D3DXMATRIX	matWorld,matRot,matScale,matTrans;
	D3DXMatrixIdentity(&matWorld);

	//	角度補正
	CorrectRot( SP[SpriteNo].Rot );

	//	Z回転
	D3DXMatrixRotationZ( &matRot, (float)DegToRad( SP[SpriteNo].Rot ) );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matRot );
	//	拡大
	D3DXMatrixScaling( &matScale,
		(float)SP[SpriteNo].ScaleX*ScaleX,
		(float)SP[SpriteNo].ScaleY*ScaleY,
		0.0f );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matScale );
	//	移動
	D3DXMatrixTranslation( &matTrans,
		(float)SP[SpriteNo].PosX,
		(float)SP[SpriteNo].PosY,
		(float)SP[SpriteNo].PosZ );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );

	lpSprite->SetTransform( &matWorld );

	RECT	rect;
	long PatNo = SP[SpriteNo].PatNo;
	SetRect( &rect,
		PAT[ PatNo ].PosX,
		PAT[ PatNo ].PosY,
		PAT[ PatNo ].PosX + PAT[ PatNo ].Width,
		PAT[ PatNo ].PosY + PAT[ PatNo ].Height
		);

	lpSprite->Draw(
		TEX[ PAT[ PatNo ].TextureNo ].lpTex,
		&rect,
		&D3DXVECTOR3( (float)SP[SpriteNo].CenterX, (float)SP[SpriteNo].CenterY, 0 ),
		NULL,
		D3DCOLOR_ARGB( SP[SpriteNo].A, SP[SpriteNo].R, SP[SpriteNo].G, SP[SpriteNo].B )
		);
}

/******************************************************************************/
//	名前	：	スプライト全描画
//	説明	：	スプライトの描画を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	DispSpriteAll( void )
{
	long	i;
	long	Max = 0;
	long	DispNo[SP_MAX];

	//	描画数検索
	for( i=0; i<SP_MAX; i++ )
	{
		if( SP[i].Use && SP[i].Disp )
		{
			//	描画スプライト番号格納
			DispNo[Max] = i;
			Max ++;
		}
	}

	//	Zソート
	for( i=Max-1; i>=1; i-- )
	{
		for( int j=i-1; j>=0; j-- )
		{
			//	後ろと前を比較
			if( SP[ DispNo[i] ].PosZ < SP[ DispNo[j] ].PosZ )
			{
				long BakNo = DispNo[i];
				DispNo[i] = DispNo[j];
				DispNo[j] = BakNo;
			}
		}
	}

	//	描画開始
	if( !lpSprite )
	{
		lpSprite->Begin(NULL);

		for( int i=0; i<Max; i++ )
		{
			DispSprite( DispNo[i] );
 		}

		//	描画終了
		lpSprite->End();
	}
}

/******************************************************************************/
//	名前	：	スプライト表示変更
//	説明	：	スプライトの表示を変更する
//	戻り値	：	なし
//	引数	：	[IN]UChar	Flag	表示フラグ
//	備考	：	なし
/******************************************************************************/
void	DispChangeSprite( UChar Flag )
{
	for( int i=0; i<SP_MAX; i++ )
	{
		if( SP[i].Use == ON )
		{
			SP[i].Disp = Flag;
		}
	}
}

/******************************************************************************/
//	名前	：	オブジェクト解放
//	説明	：	オブジェクトの解放を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	ReleaseSprite( void )
{
	SAFE_RELEASE(lpSprite);
}
