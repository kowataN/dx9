/******************************************************************************/
//	DirectX9ライブラリ
//		フェード
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"



/******************************************************************************/
//	名前	：	フェード初期化
//	説明	：	フェードの初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	備考
/******************************************************************************/
void	CDx9LibFade::Init( void )
{
	m_Fade.Use = OFF;
	m_Fade.Disp = OFF;
	m_Fade.Init = OFF;
	m_Fade.State = NOT;

	m_Fade.Time = 0;
	m_Fade.Count = 0;

	m_Fade.A = m_Fade.R = m_Fade.G = m_Fade.B = 0.0;
	m_Fade.StartA = m_Fade.StartR = m_Fade.StartG = m_Fade.StartB = 0.0;
	m_Fade.TargetA = m_Fade.TargetR = m_Fade.TargetG = m_Fade.TargetB = 0.0;
}

/******************************************************************************/
//	名前	：	フェードセット
//	説明	：	フェードの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	Time		所要時間
//				[IN]double	a1			開始色アルファ
//				[IN]double	r1			開始色赤
//				[IN]double	g1			開始色緑
//				[IN]double	b1			開始色青
//				[IN]double	a2			目標色アルファ
//				[IN]double	r2			目標色赤
//				[IN]double	g2			目標色緑
//				[IN]double	b2			目標色青
//	備考	：	なし
/******************************************************************************/
void	CDx9LibFade::Set(
						long Time,
						double a1, double r1, double g1, double b1,
						double a2, double r2, double g2, double b2 )
{
	m_Fade.Use = ON;
	m_Fade.Disp = ON;
	m_Fade.Init = ON;
	m_Fade.State = ON;

	m_Fade.Time = Time;
	m_Fade.Count = 0;

	m_Fade.StartA = m_Fade.A = a1;
	m_Fade.StartR = m_Fade.R = r1;
	m_Fade.StartG = m_Fade.G = g1;
	m_Fade.StartB = m_Fade.B = b1;

	m_Fade.TargetA = a2;
	m_Fade.TargetR = r2;
	m_Fade.TargetG = g2;
	m_Fade.TargetB = b2;
}

/******************************************************************************/
//	名前	：	フェード描画
//	説明	：	フェードの描画を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibFade::Draw( void )
{
	ST_VertexInfo	fade[4];
	fade[0].x = 0;
	fade[0].y = 0;
	fade[1].x = 1024;
	fade[1].y = 0;
	fade[2].x = 1024;
	fade[2].y = 512;
	fade[3].x = 0;
	fade[3].y = 512;

	for( int i=0; i<4; i++ )
	{
		fade[i].z = 1.0;
		fade[i].rhw = 1.0;
		fade[i].color = D3DCOLOR_ARGB(
			(short)m_Fade.A, (short)m_Fade.R, (short)m_Fade.G, (short)m_Fade.B );
	}

	fade[0].tu = 0.0;
	fade[0].tv = 0.0;
	fade[1].tu = 1.0;
	fade[1].tv = 0.0;
	fade[2].tu = 1.0;
	fade[2].tv = 1.0;
	fade[3].tu = 0.0;
	fade[3].tv = 1.0;

	// 描画開始
	g_lpD3DDev->BeginScene();

	//	頂点フォーマットの設定
	g_lpD3DDev->SetFVF( FVF_TLVERTEX );

	g_lpD3DDev->SetTexture( 0, NULL );

	g_lpD3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, fade, sizeof( ST_VertexInfo ) );

	// 描画終了
	g_lpD3DDev->EndScene();
}

/******************************************************************************/
//	名前	：	フェードループ
//	説明	：	実フェード処理を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibFade::Loop( void )
{
	if( !m_Fade.Use )
	{
		return;
	}

	if( m_Fade.State == ON )
	{
		m_Fade.Count ++;
		if( m_Fade.Count >= m_Fade.Time )
		{
			m_Fade.Count = m_Fade.Time;
			m_Fade.State = OFF;
		}
	}

	if( m_Fade.Count == m_Fade.Time )
	{
		/////////////////////////////////////
		//	フェード終了
		m_Fade.A = m_Fade.TargetA;
		m_Fade.R = m_Fade.TargetR;
		m_Fade.G = m_Fade.TargetG;
		m_Fade.B = m_Fade.TargetB;
	}
	else
	{
		/////////////////////////////////////
		//	フェード中
		m_Fade.A += ( m_Fade.TargetA - m_Fade.StartA ) / m_Fade.Time;
		m_Fade.R += ( m_Fade.TargetR - m_Fade.StartR ) / m_Fade.Time;
		m_Fade.G += ( m_Fade.TargetG - m_Fade.StartG ) / m_Fade.Time;
		m_Fade.B += ( m_Fade.TargetB - m_Fade.StartB ) / m_Fade.Time;
	}

	//	色クリッピング
	MaxMin( m_Fade.A, 255, 0 );
	MaxMin( m_Fade.R, 255, 0 );
	MaxMin( m_Fade.G, 255, 0 );
	MaxMin( m_Fade.B, 255, 0 );

	this->Draw();
}

/******************************************************************************/
//	名前	：	ブラックアウト
//	説明	：	ブラックアウトを行う
//	戻り値	：	なし
//	引数	：	[IN]long	Time	フレーム数
//	備考	：	なし
/******************************************************************************/
void	CDx9LibFade::BlackOut( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 0,0,0,0, 255,0,0,0 );
}

/******************************************************************************/
//	名前	：	ブラックイン
//	説明	：	ブラックインを行う
//	戻り値	：	なし
//	引数	：	[IN]long	Time	フレーム数
//	備考	：	なし
/******************************************************************************/
void	CDx9LibFade::BlackIn( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 255,0,0,0, 0,0,0,0 );
}

/******************************************************************************/
//	名前	：	ホワイトアウト
//	説明	：	ホワイトアウトを行う
//	戻り値	：	なし
//	引数	：	[IN]long	Time	フレーム数
//	備考	：	なし
/******************************************************************************/
void	CDx9LibFade::WhiteOut( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 0,255,255,255, 255,255,255,255 );
}

/******************************************************************************/
//	名前	：	ホワイトイン
//	説明	：	ホワイトインを行う
//	戻り値	：	なし
//	引数	：	[IN]long	Time	フレーム数
//	備考	：	なし
/******************************************************************************/
void	CDx9LibFade::WhiteIn( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 255,255,255,255, 0,255,255,255 );
}

/******************************************************************************/
//	名前	：	状態取得
//	説明	：	状態を取得する
//	戻り値	：	short	状態	0(終了)
//								1(フェード中)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
short	CDx9LibFade::GetState( void )
{
	return m_Fade.State;
}

/******************************************************************************/
//	名前	：	進捗状況取得
//	説明	：	進捗状況を取得する
//	戻り値	：	long	進捗状況
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
long	CDx9LibFade::GetProgress( void )
{
	double Ret = (double)m_Fade.Count / (double)m_Fade.Time * 100;
	return	(long)Ret;
}
