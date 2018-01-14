/******************************************************************************/
//	DirectX9ライブラリ
//		ライン
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
ST_LineInfo	LINE[LINE_MAX];



/******************************************************************************/
//	名前	：	コンストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibLine::CDx9LibLine()
{
	m_lpLine = NULL;
}

/******************************************************************************/
//	名前	：	デストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibLine::~CDx9LibLine()
{
	this->ReleaseDevice();
}

/******************************************************************************/
//	名前	：	ライン用デバイス作成
//	説明	：	ライン用デバイスの作成を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::CreateDevice( void )
{
	D3DXCreateLine( g_lpD3DDev, &m_lpLine );
}

/******************************************************************************/
//	名前	：	ライン用構造体初期化
//	説明	：	ライン用構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	No		ライン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::Init( long No )
{
	if( No >= LINE_MAX )
	{
		return;
	}

	//	フラグOFF
	LINE[ No ].Use		= OFF;
	LINE[ No ].Disp		= OFF;

	//	始点
	LINE[ No ].StartX	= 0.0;
	LINE[ No ].StartY	= 0.0;

	//	終点
	LINE[ No ].EndX		= 0.0;
	LINE[ No ].EndY		= 0.0;

	//	アトリビュート
	LINE[ No ].Atr		= ATR_NONE;
}

/******************************************************************************/
//	名前	：	ライン用構造体全初期化
//	説明	：	ライン用構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::InitAll( void )
{
	for( long i=0; i<LINE_MAX; i++ )
	{
		Init( i );
	}
}

/******************************************************************************/
//	名前	：	ラインセット
//	説明	：	ライン情報を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	No		ライン番号
//				[IN]double	StartX	開始座標X
//				[IN]double	StartY	開始座標Y
//				[IN]double	EndX	終了座標X
//				[IN]double	EndY	終了座標Y
//				[IN]UShort	Atr		アトリビュート
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::Set(
						 long No,
						 double StartX, double StartY,
						 double EndX, double EndY,
						 UShort Atr )
{
	if( No >= LINE_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	各メンバの初期化
	LINE[ No ].Use	= ON;		//	使用フラグＯＮ
	LINE[ No ].Disp	= ON;		//	表示フラグＯＮ

	//	表示座標
	LINE[ No ].StartX	= StartX;
	LINE[ No ].StartY	= StartY;

	LINE[ No ].EndX = EndX;
	LINE[ No ].EndY = EndY;

	LINE[No].A = 255;
	LINE[No].R = 255;
	LINE[No].G = 255;
	LINE[No].B = 255;

	LINE[No].Atr = Atr;
}

/******************************************************************************/
//	名前	：	色設定
//	説明	：	デフューズ色を変更する
//	戻り値	：	なし
//	引数	：	[IN]long	No		ライン番号
//				[IN]UChar	A		アルファ
//				[IN]UChar	R		赤成分
//				[IN]UChar	G		緑成分
//				[IN]UChar	B		青成分
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::SetColor( long No, UChar A, UChar R, UChar G, UChar B )
{
	LINE[No].A = A;
	LINE[No].R = R;
	LINE[No].G = G;
	LINE[No].B = B;
}

/******************************************************************************/
//	名前	：	ライン空き番号取得
//	説明	：	ライン構造体の中で未使用の番号を取得する
//	戻り値	：	long	処理結果	-1(空き番号なし)
//									-1以外(空き番号)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
short	CDx9LibLine::GetNo( void )
{
	for( int i=0; i<LINE_MAX; i++ )
	{
		if( !LINE[i].Use )
		{
			return	i;
		}
	}

	return NOT;
}

/******************************************************************************/
//	名前	：	ライン描画
//	説明	：	ラインを描画する
//	戻り値	：	なし
//	引数	：	[IN]long	No		ライン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::DrawLine( long No )
{
	D3DXVECTOR2	vec[2];
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].EndY );

	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );
}

/******************************************************************************/
//	名前	：	矩形描画
//	説明	：	矩形を描画する
//	戻り値	：	なし
//	引数	：	[IN]long	No		ライン番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::DrawBox( long No )
{
	D3DXVECTOR2	vec[2];

	//	上
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].StartY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );

	//	下
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].EndY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].EndY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );

	//	左
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].EndY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );

	//	右
	vec[0] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].EndY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );
}

/******************************************************************************/
//	名前	：	ライン全描画
//	説明	：	ライン全て描画する
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::DispAll( void )
{
	//	描画開始
	m_lpLine->Begin();

	for( int i=0; i<LINE_MAX; i++ )
	{
		if( !LINE[i].Use && !LINE[i].Disp )
		{
			continue;
		}

		if( LINE[i].Atr == ATR_NONE )
		{
			this->DrawLine( i );
		}
		else
		{
			this->DrawBox( i );
		}
	}

	//	描画終了
	m_lpLine->End();
}

/******************************************************************************/
//	名前	：	ライン表示変更
//	説明	：	ラインの表示を切り替える
//	戻り値	：	なし
//	引数	：	[IN]UChar		Flag	表示状態
//	備考	：	Flagの設定値	ON		表示
//								OFF		非表示
/******************************************************************************/
void	CDx9LibLine::DispChange( UChar Flag )
{
	for( int i=0; i<LINE_MAX; i++ )
	{
		if( LINE[i].Use )
		{
			LINE[i].Disp = Flag;
		}
	}
}

/******************************************************************************/
//	名前	：	ラインオブジェクト開放
//	説明	：	ラインオブジェクトを開放する
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibLine::ReleaseDevice( void )
{
	SAFE_RELEASE(m_lpLine);
}
