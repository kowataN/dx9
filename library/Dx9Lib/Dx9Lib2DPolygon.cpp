/******************************************************************************/
//	DirectX9ライブラリ
//		ポリゴン
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"



/******************************************************************************/
//	名前	：	コンストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibPolygon::CDx9LibPolygon()
{
	//	何もしない
}

/******************************************************************************/
//	名前	：	デストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibPolygon::~CDx9LibPolygon()
{
	//	何もしない
}

/******************************************************************************/
//	名前	：	構造体全初期化
//	説明	：	ポリゴン用構造体の全初期化
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	内部で「InitPolygon」を呼んでいる
/******************************************************************************/
void	CDx9LibPolygon::InitAll( void )
{
	for( long i=0; i<POL_MAX; i++ )
	{
		this->Init( i );
	}
}

/******************************************************************************/
//	名前	：	ポリゴン用構造体初期化
//	説明	：	ポリゴン用構造体の初期化
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::Init( long PolNo )
{
	if( PolNo >= POL_MAX )
	{
		return;
	}

	//	フラグ
	m_Info[ PolNo ].Use		= OFF;	//	使用フラグ
	m_Info[ PolNo ].Disp	= OFF;	//	表示フラグ
	m_Info[ PolNo ].PatUse	= OFF;	//	パターン使用フラグ

	//	番号
	m_Info[ PolNo ].PatNo	= NOT;	//	パターン

	//	表示座標
	m_Info[ PolNo ].PosX	= 0;
	m_Info[ PolNo ].PosY	= 0;
	m_Info[ PolNo ].PosZ	= 0;

	//	バックアップ座標
	m_Info[ PolNo ].PosXBk	= 0;
	m_Info[ PolNo ].PosYBk	= 0;
	m_Info[ PolNo ].PosZBk	= 0;

	//	中心座標
	m_Info[ PolNo ].CenterX	= 0;
	m_Info[ PolNo ].CenterY	= 0;

	//	回転角度
	m_Info[ PolNo ].RotX	= 0;
	m_Info[ PolNo ].RotY	= 0;
	m_Info[ PolNo ].RotZ	= 0;

	//	スケール
	m_Info[ PolNo ].ScaleX	= 1.0;
	m_Info[ PolNo ].ScaleY	= 1.0;

	//	サイズ
	m_Info[ PolNo ].Width	= 0;
	m_Info[ PolNo ].Height	= 0;

	//	各色成分
	m_Info[ PolNo ].A		= 255;
	m_Info[ PolNo ].R		= 255;
	m_Info[ PolNo ].G		= 255;
	m_Info[ PolNo ].B		= 255;

	//	アトリビュート
	m_Info[ PolNo ].Atr		= ATR_NONE;
}

/******************************************************************************/
//	名前	：	ポリゴン全描画
//	説明	：	ポリゴンの描画を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	内部で「DispPolygon」を呼んでいる
/******************************************************************************/
void	CDx9LibPolygon::DispAll( void )
{
	short	i,j,bak;
	short	no[POL_MAX],max=0;

	//	検索
	for( i=0; i<POL_MAX; i++ )
	{
		//	その番号が使われているか
		if( m_Info[i].Use && m_Info[i].Disp )
		{
			//	配列に番号を格納
			no[max] = i;
			max ++;
		}
	}

	//	Ｚソート
	for( i=max-1; i>=1; i-- )	//	後ろ
	{
		for( j=i-1; j>=0; j-- )	//	前
		{
			//	後ろと前を比較
			if( m_Info[ no[i] ].PosZ < m_Info[ no[j] ].PosZ )
			{
				bak   = no[i];
				no[i] = no[j];
				no[j] = bak;
			}
		}
	}

	//	描画
	for( i=0; i<max; i++ )
	{
		this->Disp( no[i] );
	}
}

/******************************************************************************/
//	名前	：	ポリゴン描画
//	説明	：	ポリゴンの描画を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo	ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::Disp( long PolNo )
{
	ST_VertexInfo	Vertex[4];

	/////////////////////////////////////
	//	頂点の初期化
	for( int i=0; i<4; i++ )
	{
		Vertex[ i ].z	= (float)m_Info[ PolNo ].PosZ;
		Vertex[ i ].rhw	= 1.0f;
		Vertex[ i ].color = 
			D3DCOLOR_RGBA( m_Info[ PolNo ].R, m_Info[ PolNo ].G, m_Info[ PolNo ].B, m_Info[ PolNo ].A );
	}

	if( !m_Info[ PolNo ].PatUse )
	{
		this->DispBox( PolNo, Vertex );
	}
	else
	{
		this->DispPat( PolNo, Vertex );
	}
}

/******************************************************************************/
//	名前	：	ポリゴン矩形描画
//	説明	：	矩形ポリゴンの描画を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo	ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::DispBox( long PolNo, ST_VertexInfo *Vertex )
{
	long		PatNo,TexNo;
	short		i,j;
	float		TexW,TexH,PatX,PatY,PatW,PatH,CenterX,CenterY,PolX,PolY;

	//	値代入
	PatNo	= m_Info[ PolNo ].PatNo;			//	パターン番号
	TexNo	= PAT[ PatNo ].TextureNo;			//	テクスチャ番号

	TexW	= (float)TEX[ TexNo ].Width;		//	テクスチャサイズ・横
	TexH	= (float)TEX[ TexNo ].Height;		//	テクスチャサイズ・縦

	PatX	= (float)PAT[ PatNo ].PosX;			//	パターンX
	PatY	= (float)PAT[ PatNo ].PosY;			//	パターンY
	PatW	= (float)m_Info[ PolNo ].Width;		//	パターンサイズ・横
	PatH	= (float)m_Info[ PolNo ].Height;	//	パターンサイズ・縦

	PolX	= (float)m_Info[ PolNo ].PosX;		//	表示座標X
	PolY	= (float)m_Info[ PolNo ].PosY;		//	表示座標Y
	CenterX	= (float)m_Info[ PolNo ].CenterX;	//	中心座標X
	CenterY	= (float)m_Info[ PolNo ].CenterY;	//	中心座標Y

	D3DXMATRIX	matWorld,matRotX,matRotY,matRotZ,matTrans;
	D3DXMATRIX	matPos;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRotX);
	D3DXMatrixIdentity(&matRotY);
	D3DXMatrixIdentity(&matRotZ);
	D3DXMatrixIdentity(&matPos);

	//	角度補正
	CorrectRot( m_Info[PolNo].RotX );
	CorrectRot( m_Info[PolNo].RotY );
	CorrectRot( m_Info[PolNo].RotZ );

	//	回転
	D3DXMatrixRotationX( &matRotX, (float)DegToRad( m_Info[PolNo].RotX ) );
	D3DXMatrixRotationY( &matRotY, (float)DegToRad( m_Info[PolNo].RotY ) );
	D3DXMatrixRotationZ( &matRotZ, (float)DegToRad( m_Info[PolNo].RotZ ) );

	Vertex[ 0 ].tu = 0;
	Vertex[ 0 ].tv = 0;
	Vertex[ 1 ].tu = 1;
	Vertex[ 1 ].tv = 0;
	Vertex[ 2 ].tu = 1;
	Vertex[ 2 ].tv = 1;
	Vertex[ 3 ].tu = 0;
	Vertex[ 3 ].tv = 1;

	//	座標
	switch( m_Info[ PolNo ].Atr )
	{
		case	ATR_NONE:
			Vertex[ 0 ].x = PolX - CenterX;
			Vertex[ 0 ].y = PolY - CenterY;
			Vertex[ 1 ].x = PolX - CenterX + PatW;
			Vertex[ 1 ].y = PolY - CenterY;
			Vertex[ 2 ].x = PolX - CenterX + PatW;
			Vertex[ 2 ].y = PolY - CenterY + PatH;
			Vertex[ 3 ].x = PolX - CenterX;
			Vertex[ 3 ].y = PolY - CenterY + PatH;

			//	原点に戻す
			this->Update( Vertex, -PolX, -PolY, 0.0f );
			//	各種座標計算
			for( i=0; i<4; i++ )
			{
				//	移動
				D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );
				//	角度
				matPos *= matRotX;
				matPos *= matRotY;
				matPos *= matRotZ;

				//	演算結果を代入
				Vertex[i].x = matPos._41;
				Vertex[i].y = matPos._42;
				Vertex[i].z = matPos._43;
			}

			//	元の位置に戻す
			this->Update( Vertex, PolX, PolY, 0.0f );

			//	テクスチャ
			g_lpD3DDev->SetTexture( 0, NULL );
			//	頂点フォーマットの設定
			g_lpD3DDev->SetFVF( FVF_TLVERTEX );
			//	ポリゴン描画
			g_lpD3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( ST_VertexInfo ) );

			break;

		case	ATR_BOX:

			for( j=0; j<2; j++ )
			{
				if( j )
				{
					Vertex[ 0 ].x = PolX - CenterX;
					Vertex[ 0 ].y = PolY - CenterY;
					Vertex[ 1 ].x = PolX - CenterX;
					Vertex[ 1 ].y = PolY - CenterY + PatH;
					Vertex[ 2 ].x = PolX - CenterX + PatW;
					Vertex[ 2 ].y = PolY - CenterY + PatH;
					Vertex[ 3 ].x = PolX - CenterX + PatW;
					Vertex[ 3 ].y = PolY - CenterY;
				}
				else
				{
					Vertex[ 0 ].x = PolX - CenterX;
					Vertex[ 0 ].y = PolY - CenterY;
					Vertex[ 1 ].x = PolX - CenterX + PatW;
					Vertex[ 1 ].y = PolY - CenterY;
					Vertex[ 2 ].x = PolX - CenterX + PatW;
					Vertex[ 2 ].y = PolY - CenterY + PatH;
					Vertex[ 3 ].x = PolX - CenterX;
					Vertex[ 3 ].y = PolY - CenterY + PatH;
				}

				//	原点に戻す
				this->Update( Vertex, -PolX, -PolY, 0.0f );
				//	各種座標計算
				for( i=0; i<4; i++ )
				{
					//	移動
					D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );
					//	角度
					matPos *= matRotX;
					matPos *= matRotY;
					matPos *= matRotZ;

					//	演算結果を代入
					Vertex[i].x = matPos._41;
					Vertex[i].y = matPos._42;
					Vertex[i].z = matPos._43;
				}

				//	元の位置に戻す
				this->Update( Vertex, PolX, PolY, 0.0f );

				//	テクスチャ
				g_lpD3DDev->SetTexture( 0, NULL );
				//	頂点フォーマットの設定
				g_lpD3DDev->SetFVF( FVF_TLVERTEX );
				//	ポリゴン描画
				g_lpD3DDev->DrawPrimitiveUP( D3DPT_LINESTRIP, 2, Vertex, sizeof( ST_VertexInfo ) );
			}

			break;
	}
}

/******************************************************************************/
//	名前	：	パターン付きポリゴン描画
//	説明	：	パターン付きポリゴンの描画を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo	ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::DispPat( long PolNo, ST_VertexInfo *Vertex )
{
	long		PatNo,TexNo;
	float		TexW,TexH;
	float		PatX,PatY,PatW,PatH;
	float		PolX,PolY,CenterX,CenterY,ScaleX,ScaleY;

	//	値代入
	PatNo	= m_Info[ PolNo ].PatNo;			//	パターン番号
	TexNo	= PAT[ PatNo ].TextureNo;			//	テクスチャ番号

	TexW	= (float)TEX[ TexNo ].Width;		//	テクスチャサイズ・横
	TexH	= (float)TEX[ TexNo ].Height;		//	テクスチャサイズ・縦

	PatX	= (float)PAT[ PatNo ].PosX;			//	パターンX
	PatY	= (float)PAT[ PatNo ].PosY;			//	パターンY
	PatW	= (float)m_Info[ PolNo ].Width;		//	パターンサイズ・横
	PatH	= (float)m_Info[ PolNo ].Height;	//	パターンサイズ・縦

	PolX	= (float)m_Info[ PolNo ].PosX;		//	表示座標X
	PolY	= (float)m_Info[ PolNo ].PosY;		//	表示座標Y
	CenterX	= (float)m_Info[ PolNo ].CenterX;	//	中心座標X
	CenterY	= (float)m_Info[ PolNo ].CenterY;	//	中心座標Y
	ScaleX	= (float)m_Info[ PolNo ].ScaleX;	//	拡大率X
	ScaleY	= (float)m_Info[ PolNo ].ScaleY;	//	拡大率Y

	D3DXMATRIX	matWorld,matRotX,matRotY,matRotZ,matTrans,matScale;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRotX);
	D3DXMatrixIdentity(&matRotY);
	D3DXMatrixIdentity(&matRotZ);
	D3DXMatrixIdentity(&matScale);

	//	角度補正
	CorrectRot( m_Info[PolNo].RotX );
	CorrectRot( m_Info[PolNo].RotY );
	CorrectRot( m_Info[PolNo].RotZ );

	//	回転
	D3DXMatrixRotationX( &matRotX, (float)DegToRad( m_Info[PolNo].RotX ) );
	D3DXMatrixRotationY( &matRotY, (float)DegToRad( m_Info[PolNo].RotY ) );
	D3DXMatrixRotationZ( &matRotZ, (float)DegToRad( m_Info[PolNo].RotZ ) );

	//	拡大
	D3DXMatrixScaling( &matScale, ScaleX, ScaleY, 1.0f );

	//	左右反転
	if( ( m_Info[ PolNo ].Atr & ATR_REV_W ) == ATR_REV_W )
	{
		Vertex[ 0 ].x = PolX - CenterX + PatW;
		Vertex[ 1 ].x = PolX - CenterX;
		Vertex[ 2 ].x = PolX - CenterX;
		Vertex[ 3 ].x = PolX - CenterX + PatW;
	}
	else
	{
		Vertex[ 0 ].x = PolX - CenterX;
		Vertex[ 1 ].x = PolX - CenterX + PatW;
		Vertex[ 2 ].x = PolX - CenterX + PatW;
		Vertex[ 3 ].x = PolX - CenterX;
	}

	//	上下反転
	if( ( m_Info[ PolNo ].Atr &  ATR_REV_H ) == ATR_REV_H )
	{
		Vertex[ 0 ].y = PolY - CenterY + PatH;
		Vertex[ 1 ].y = PolY - CenterY + PatH;
		Vertex[ 2 ].y = PolY - CenterY;
		Vertex[ 3 ].y = PolY - CenterY;
	}
	else
	{
		Vertex[ 0 ].y = PolY - CenterY;
		Vertex[ 1 ].y = PolY - CenterY;
		Vertex[ 2 ].y = PolY - CenterY + PatH;
		Vertex[ 3 ].y = PolY - CenterY + PatH;
	}

	//	原点に戻す
	this->Update( Vertex, -PolX, -PolY, 0.0f );

	//	各種座標計算
	D3DXMATRIX matPos;
	for( int i=0; i<4; i++ )
	{
		//	移動
		D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );
		//	角度
		matPos *= matRotX;
		matPos *= matRotY;
		matPos *= matRotZ;
		//	拡大
		matPos *= matScale;

		//	演算結果を代入
		Vertex[i].x = matPos._41;
		Vertex[i].y = matPos._42;
		Vertex[i].z = matPos._43;
	}

	//	元の位置に戻す
	this->Update( Vertex, PolX, PolY, 0.0f );

	//	テクスチャ座標
	float tu1 = ( PatX + 0.5f ) / TexW;
	float tv1 = ( PatY + 0.5f ) / TexH;
	float tu2 = ( PatX + PatW ) / TexW;
	float tv2 = ( PatY + PatH ) / TexH;
	Vertex[ 0 ].tu = tu1;
	Vertex[ 0 ].tv = tv1;
	Vertex[ 1 ].tu = tu2;
	Vertex[ 1 ].tv = tv1;
	Vertex[ 2 ].tu = tu2;
	Vertex[ 2 ].tv = tv2;
	Vertex[ 3 ].tu = tu1;
	Vertex[ 3 ].tv = tv2;

	//	テクスチャ
	g_lpD3DDev->SetTexture( 0, TEX[ TexNo ].lpTex );

	//	頂点フォーマットの設定
	g_lpD3DDev->SetFVF( FVF_TLVERTEX );

	//	ポリゴン描画
	g_lpD3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( ST_VertexInfo ) );
}

/******************************************************************************/
//	名前	：	表示座標更新
//	説明	：	表示座標の更新を行う
//	戻り値	：	なし
//	引数	：	[IN/OUT]TLVERTEX*	Vertex	頂点情報
//				[IN]float			MoveX	座標X
//				[IN]float			MoveY	座標Y
//				[IN]float			MoveZ	座標Z
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::Update( ST_VertexInfo* Vertex, float MoveX, float MoveY, float MoveZ )
{
	D3DXMATRIX	matPos,matMove;

	//	初期化
	D3DXMatrixIdentity( &matPos );
	D3DXMatrixIdentity( &matMove );

	//	移動量を設定
	D3DXMatrixTranslation( &matMove, MoveX, MoveY, MoveZ );

	for( int i=0;i <4; i++ )
	{
		//	現在の頂情報を設定
		D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );

		//	行列計算
		matPos += matMove;

		//	頂点座標を更新
		Vertex[i].x = matPos._41;
		Vertex[i].y = matPos._42;
		Vertex[i].z = matPos._43;
	}
}

/******************************************************************************/
//	名前	：	2Dポリゴンセット
//	説明	：	ポリゴンの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	Width		横サイズ
//				[IN]long	Height		縦サイズ
//				[IN]double	PosX		X座標
//				[IN]double	PosY		Y座標
//				[IN]double	PosZ		Z座標
//				[IN]UShort	Atr			アトリビュート
//	備考	：	Atrの設定値	ATR_NONE	なし
//							ATR_BOX		矩形で描画
/******************************************************************************/
void	CDx9LibPolygon::Set(long PolNo,
							long Width, long Height,
							double PosX, double PosY, double PosZ,
							UShort Atr )
{
	if( PolNo >= POL_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	各メンバ初期化

	//	フラグ
	m_Info[ PolNo ].Use		= ON;
	m_Info[ PolNo ].Disp	= ON;
	m_Info[ PolNo ].PatUse	= OFF;

	m_Info[ PolNo ].PatNo	= NOT;

	//	座標初期化
	m_Info[ PolNo ].PosX	= PosX;
	m_Info[ PolNo ].PosY	= PosY;
	m_Info[ PolNo ].PosZ	= PosZ;

	//	サイズ
	m_Info[ PolNo ].Width	= Width;
	m_Info[ PolNo ].Height	= Height;

	m_Info[ PolNo ].Atr		= Atr;
}

/******************************************************************************/
//	名前	：	ポリゴンパターンセット
//	説明	：	パターン付きポリゴンの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	PatNo		パターン番号
//				[IN]double	PosX		X座標
//				[IN]double	PosY		Y座標
//				[IN]double	PosZ		Z座標
//				[IN]UShort	Atr			アトリビュート
//	備考	：	Atrの設定値	ATR_NONE	なし
//							ATR_REV_W	左右反転
//							ATR_REV_H	上下反転
//							ATR_ALL		上下左右反転
/******************************************************************************/
void	CDx9LibPolygon::SetPat( long PolNo, long PatNo,
								double PosX, double PosY, double PosZ,
								UShort Atr )
{
	if( PolNo >= POL_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	各メンバ初期化

	//	フラグ
	m_Info[ PolNo ].Use		= ON;
	m_Info[ PolNo ].Disp	= ON;
	m_Info[ PolNo ].PatUse	= ON;

	m_Info[ PolNo ].PatNo	= PatNo;

	//	座標初期化
	m_Info[ PolNo ].PosX	= PosX;
	m_Info[ PolNo ].PosY	= PosY;
	m_Info[ PolNo ].PosZ	= PosZ;

	//	サイズ
	m_Info[ PolNo ].Width	= PAT[ PatNo ].Width;
	m_Info[ PolNo ].Height	= PAT[ PatNo ].Height;

	//	アトリビュート
	m_Info[ PolNo ].Atr		= Atr;
}

/******************************************************************************/
//	名前	：	ポリゴンカラーセット
//	説明	：	ポリゴンのデフューズ色を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]UChar	A			アルファ
//				[IN]UChar	R			赤成分
//				[IN]UChar	G			緑成分
//				[IN]UChar	B			青成分
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetColor( long PolNo, UChar A, UChar R, UChar G, UChar B )
{
	m_Info[ PolNo ].A = A;
	m_Info[ PolNo ].R = R;
	m_Info[ PolNo ].G = G;
	m_Info[ PolNo ].B = B;
}

/******************************************************************************/
//	名前	：	ポリゴンカラーセット
//	説明	：	ポリゴンのデフューズ色を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]UChar	A			アルファ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetColor( long PolNo, UChar A )
{
	m_Info[ PolNo ].A = A;
}

/******************************************************************************/
//	名前	：	ポリゴン表示座標セット
//	説明	：	ポリゴンの表示座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	PosX		X座標
//				[IN]double	PosY		Y座標
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetPos( long PolNo, double PosX, double PosY )
{
	 this->SetPosX( PolNo, PosX );
	 this->SetPosY( PolNo, PosY );
}

/******************************************************************************/
//	名前	：	ポリゴン表示座標セット
//	説明	：	ポリゴンの表示座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	PosX		X座標
//				[IN]double	PosY		Y座標
//				[IN]double	PosZ		Z座標
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetPos( long PolNo, double PosX, double PosY, double PosZ )
{
	 this->SetPosX( PolNo, PosX );
	 this->SetPosY( PolNo, PosY );
	 this->SetPosZ( PolNo, PosZ );
}

/******************************************************************************/
//	名前	：	ポリゴン表示座標セット
//	説明	：	ポリゴンの表示座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	PosX		X座標
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetPosX( long PolNo, double PosX )
{
	m_Info[ PolNo ].PosX = PosX;
}

/******************************************************************************/
//	名前	：	ポリゴン表示座標セット
//	説明	：	ポリゴンの表示座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	PosY		Y座標
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetPosY( long PolNo, double PosY )
{
	m_Info[ PolNo ].PosY = PosY;
}

/******************************************************************************/
//	名前	：	ポリゴン表示座標セット
//	説明	：	ポリゴンの表示座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	PosZ		Z座標
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetPosZ( long PolNo, double PosZ )
{
	m_Info[ PolNo ].PosZ = PosZ;
}

/******************************************************************************/
//	名前	：	中心座標セット
//	説明	：	中心座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetCenter( long PolNo )
{
	long PatNo = m_Info[ PolNo ].PatNo;
	m_Info[ PolNo ].CenterX = (short)( PAT[PatNo].Width / 2 );
	m_Info[ PolNo ].CenterY = (short)( PAT[PatNo].Height / 2 );
}

/******************************************************************************/
//	名前	：	中心座標セット
//	説明	：	中心座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]short	CenterX		中心座標X
//				[IN]short	CenterY		中心座標Y
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetCenter( long PolNo, short CenterX, short CenterY )
{
	m_Info[ PolNo ].CenterX = CenterX;
	m_Info[ PolNo ].CenterY = CenterY;
}

/******************************************************************************/
//	名前	：	拡大率設定
//	説明	：	拡大率を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	Scale		拡大率
//	備考	：	縦横両方一緒に設定する
/******************************************************************************/
void	CDx9LibPolygon::SetScale( long PolNo, double Scale )
{
	m_Info[ PolNo ].ScaleX = Scale;
	m_Info[ PolNo ].ScaleY = Scale;
}

/******************************************************************************/
//	名前	：	サイズ設定
//	説明	：	サイズを設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	Width		横サイズ
//				[IN]long	Height		縦サイズ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetSize( long PolNo, long Width, long Height )
{
	this->SetWidth( PolNo, Width );
	this->SetHeight( PolNo, Height );
}

/******************************************************************************/
//	名前	：	横サイズ設定
//	説明	：	横サイズを設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	Width		横サイズ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetWidth( long PolNo, long Width )
{
	m_Info[ PolNo ].Width = Width;
}

/******************************************************************************/
//	名前	：	縦サイズ設定
//	説明	：	縦サイズを設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	Height		縦サイズ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetHeight( long PolNo, long Height )
{
	m_Info[ PolNo ].Height = Height;
}

/******************************************************************************/
//	名前	：	角度設定
//	説明	：	角度を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	Rot			角度
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetRotY( long PolNo, double Rot )
{
	m_Info[ PolNo ].RotY = Rot;
}

/******************************************************************************/
//	名前	：	角度設定
//	説明	：	角度を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	Rot			角度
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetRotZ( long PolNo, double Rot )
{
	m_Info[ PolNo ].RotZ = Rot;
}

/******************************************************************************/
//	名前	：	パターン番号設定
//	説明	：	パターン番号を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	PatNo		パターン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetPatNo( long PolNo, long PatNo )
{
	m_Info[ PolNo ].PatNo = PatNo;
}

/******************************************************************************/
//	名前	：	ポリゴン情報取得
//	説明	：	ポリゴン情報を取得する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//	備考	：	なし
/******************************************************************************/
ST_PolygonInfo	CDx9LibPolygon::GetInfo( long PolNo )
{
	return m_Info[ PolNo ];
}

/******************************************************************************/
//	名前	：	ポリゴン情報設定
//	説明	：	ポリゴン情報を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]POL_TBL	Info		ポリゴン情報
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::SetInfo( long PolNo, ST_PolygonInfo Info )
{
	m_Info[ PolNo ] = Info;
}

/******************************************************************************/
//	名前	：	ポリゴンデータコピー
//	説明	：	ポリゴンデータをコピーする
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		コピー先ポリゴン番号
//				[IN]long	CopyNo		コピー元ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::CopyData( long PolNo, long CopyNo )
{
	m_Info[ PolNo ] = m_Info[ CopyNo ];
}

/******************************************************************************/
//	名前	：	ポリゴン空き番号取得
//	説明	：	ポリゴン構造体の中で未使用の番号を取得する
//	戻り値	：	long	処理結果	-1(空き番号なし)
//									-1以外(空き番号)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
long	CDx9LibPolygon::GetNo( void )
{
	long Ret = NOT;

	for( int i=0; i<POL_MAX; i++ )
	{
		if( !m_Info[i].Use )
		{
			Ret = i;
			break;
		}
	}

	return Ret;
}

/******************************************************************************/
//	名前	：	ポリゴン表示切替
//	説明	：	ポリゴンの表示を切り替える
//	戻り値	：	なし
//	引数	：	[IN]UChar		Flag	表示状態
//	備考	：	Flagの設定値	ON		表示
//								OFF		非表示
/******************************************************************************/
void	CDx9LibPolygon::ChangeDisp( UChar Flag )
{
	for( long i=0; i<POL_MAX; i++ )
	{
		if( m_Info[ i ].Use == ON )
		{
			m_Info[ i ].Disp = Flag;
		}
	}
}

/******************************************************************************/
//	名前	：	ポリゴン表示切替
//	説明	：	ポリゴンの表示を切り替える
//	戻り値	：	なし
//	引数	：	[IN]long		PolNo	ポリゴン番号
//				[IN]UChar		Flag	表示状態
//	備考	：	Flagの設定値	ON		表示
//								OFF		非表示
/******************************************************************************/
void	CDx9LibPolygon::ChangeDisp( long PolNo, UChar Flag )
{
	if( m_Info[ PolNo ].Use == ON )
	{
		m_Info[ PolNo ].Disp = Flag;
	}
}

/******************************************************************************/
//	名前	：	ポリゴン表示状態反転
//	説明	：	ポリゴンの表示状態を反転する
//	戻り値	：	なし
//	引数	：	[IN]long		PolNo	ポリゴン番号
//	備考	：	表示→非表示　非表示→表示
/******************************************************************************/
void	CDx9LibPolygon::ChangeDispRev( long PolNo )
{
	m_Info[ PolNo ].Disp ^= ON;
}

/******************************************************************************/
//	名前	：	表示座標加算
//	説明	：	表示座標を加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	AddX		加算値X
//				[IN]double	AddY		加算値Y
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddPos( long PolNo, double AddX, double AddY )
{
	this->AddPosX( PolNo, AddX );
	this->AddPosY( PolNo, AddY );
}

/******************************************************************************/
//	名前	：	表示座標加算
//	説明	：	表示座標を加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	AddX		加算値X
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddPosX( long PolNo, double AddX )
{
	m_Info[ PolNo ].PosX += AddX;
}

/******************************************************************************/
//	名前	：	表示座標加算
//	説明	：	表示座標を加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	AddY		加算値Y
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddPosY( long PolNo, double AddY )
{
	m_Info[ PolNo ].PosY += AddY;
}

/******************************************************************************/
//	名前	：	角度加算
//	説明	：	角度を加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	AddX		加算値X
//				[IN]double	AddY		加算値Y
//				[IN]double	AddZ		加算値Z
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddRot( long PolNo, double AddX, double AddY, double AddZ )
{
	m_Info[ PolNo ].RotX += AddX;
	m_Info[ PolNo ].RotY += AddY;
	m_Info[ PolNo ].RotZ += AddZ;
}

/******************************************************************************/
//	名前	：	拡大率加算
//	説明	：	拡大率を加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	Add			加算値
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddScale( long PolNo, double Add )
{
	m_Info[ PolNo ].ScaleX += Add;
	m_Info[ PolNo ].ScaleY += Add;
}

/******************************************************************************/
//	名前	：	横サイズ加算
//	説明	：	横サイズ加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	Add			加算値
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddWidth( long PolNo, long Add )
{
	m_Info[ PolNo ].Width += Add;
}

/******************************************************************************/
//	名前	：	縦サイズ加算
//	説明	：	縦サイズを加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	Add			加算値
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddHeight( long PolNo, long Add )
{
	m_Info[ PolNo ].Height += Add;
}

/******************************************************************************/
//	名前	：	色加算
//	説明	：	色を加算する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	AddA		加算値A
//				[IN]long	AddR		加算値R
//				[IN]long	AddG		加算値G
//				[IN]long	AddB		加算値B
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::AddColor( long PolNo, UChar AddA, UChar AddR, UChar AddG, UChar AddB )
{
	m_Info[ PolNo ].A += AddA;
	m_Info[ PolNo ].R += AddR;
	m_Info[ PolNo ].G += AddG;
	m_Info[ PolNo ].B += AddB;
}

/******************************************************************************/
//	名前	：	座標バックアップ
//	説明	：	座標のバックアップを行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::PosBackup( long PolNo )
{
	m_Info[ PolNo ].PosXBk = m_Info[ PolNo ].PosX;
	m_Info[ PolNo ].PosYBk = m_Info[ PolNo ].PosY;
	m_Info[ PolNo ].PosZBk = m_Info[ PolNo ].PosZ;
}

/******************************************************************************/
//	名前	：	座標リストア
//	説明	：	座標のリストアを行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::PosRestore( long PolNo )
{
	m_Info[ PolNo ].PosX = m_Info[ PolNo ].PosXBk;
	m_Info[ PolNo ].PosY = m_Info[ PolNo ].PosYBk;
	m_Info[ PolNo ].PosZ = m_Info[ PolNo ].PosZBk;
}

/******************************************************************************/
//	名前	：	X座標クリッピング
//	説明	：	X座標のクリッピングを行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	MinPos		最小値
//				[IN]double	MaxPos		最大値
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::ClippingPosX( long PolNo, double MinPos, double MaxPos )
{
	if( m_Info[ PolNo ].PosX > MaxPos )
	{
		m_Info[ PolNo ].PosX = MaxPos;
	}
	else if( m_Info[ PolNo ].PosX < MinPos )
	{
		m_Info[ PolNo ].PosX = MinPos;
	}
}

/******************************************************************************/
//	名前	：	Y座標クリッピング
//	説明	：	Y座標のクリッピングを行う
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]double	MinPos		最小値
//				[IN]double	MaxPos		最大値
//	備考	：	なし
/******************************************************************************/
void	CDx9LibPolygon::ClippingPosY( long PolNo, double MinPos, double MaxPos )
{
	if( m_Info[ PolNo ].PosY > MaxPos )
	{
		m_Info[ PolNo ].PosY = MaxPos;
	}
	else if( m_Info[ PolNo ].PosY < MinPos )
	{
		m_Info[ PolNo ].PosY = MinPos;
	}
}
