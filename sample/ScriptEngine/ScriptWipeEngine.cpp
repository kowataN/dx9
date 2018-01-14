/******************************************************************************/
//	スクリプトワイプ
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibPad.h"
#include	"./ScriptWipeEngine.h"

/******************************************************************************/
//	コンストラクタ
/******************************************************************************/
CScriptWipeEngine::CScriptWipeEngine()
: m_WipeNo(NOT), m_State(0), m_Time(0), m_Count(0)
{
	ZeroMemory( &m_OldBg, sizeof(WIPE_FADE_TBL) );
	ZeroMemory( &m_NewBg, sizeof(WIPE_FADE_TBL) );
}

/******************************************************************************/
//	コンストラクタ
/******************************************************************************/
CScriptWipeEngine::CScriptWipeEngine(long TexNo, long PatNo, long PolNo )
: m_WipeNo(NOT), m_State(0), m_Time(0), m_Count(0)
{
	ZeroMemory( &m_OldBg, sizeof(WIPE_FADE_TBL) );
	ZeroMemory( &m_NewBg, sizeof(WIPE_FADE_TBL) );

	//	元の画像関連の番号
	if( TexNo < TEX_MAX )
	{
		m_OldBg.TexNo = TexNo;
	}
	if( PatNo < PAT_MAX )
	{
		m_OldBg.PatNo = PatNo;
	}
	if( PolNo < POL_MAX )
	{
		m_OldBg.PolNo = PolNo;
	}

	//	元の画像関連の番号
	if( TexNo+1 < TEX_MAX )
	{
		m_NewBg.TexNo = TexNo+1;
	}
	if( PatNo+1 < PAT_MAX )
	{
		m_NewBg.PatNo = PatNo+1;
	}
	if( PolNo+1 < POL_MAX )
	{
		m_NewBg.PolNo = PolNo+1;
	}
}

/******************************************************************************/
//	デストラクタ
/******************************************************************************/
CScriptWipeEngine::~CScriptWipeEngine()
{
}

/******************************************************************************/
//	初期化
/******************************************************************************/
void	CScriptWipeEngine::Init( long WipeNo, long Time, char *FileName )
{
	if( WipeNo >= WIPE_MAX )
	{
		return;
	}

	m_WipeNo = WipeNo;
	m_Time = Time;
	m_Count = 0;

	switch( WipeNo )
	{
		//	クロスフェード
		case	WIPE_CROSS_FADE:
			SetCrossFade( FileName );
			break;
	}
}

/******************************************************************************/
//	ループ処理
/******************************************************************************/
long	CScriptWipeEngine::Loop( void )
{
	switch( m_WipeNo )
	{
		case	WIPE_CROSS_FADE:
			LoopCrossFade();
			break;
	}

	return m_State;
}

/******************************************************************************/
//	クロスフェード設定
/******************************************************************************/
void	CScriptWipeEngine::SetCrossFade( char *FileName )
{
	LoadTexture( m_NewBg.TexNo, FileName );
	SetPattern( m_NewBg.PatNo, m_NewBg.TexNo, 0,0, g_WindowWidth,g_WindowHeight );
	g_LibPol.SetPat( m_NewBg.PolNo, m_NewBg.PatNo, 0,0,0.0, ATR_NONE );
	g_LibPol.SetColor( m_NewBg.PolNo, 0, 255,255,255 );

	//	色初期化
	m_OldBg.StartA = g_LibPol.GetInfo( m_OldBg.PolNo ).A;
	m_OldBg.StartR = g_LibPol.GetInfo( m_OldBg.PolNo ).R;
	m_OldBg.StartG = g_LibPol.GetInfo( m_OldBg.PolNo ).G;
	m_OldBg.StartB = g_LibPol.GetInfo( m_OldBg.PolNo ).B;
	m_OldBg.TargetA = 0;
	m_OldBg.TargetR = g_LibPol.GetInfo( m_OldBg.PolNo ).R;
	m_OldBg.TargetG = g_LibPol.GetInfo( m_OldBg.PolNo ).G;
	m_OldBg.TargetB = g_LibPol.GetInfo( m_OldBg.PolNo ).B;
	m_OldBg.A = m_OldBg.StartA;
	m_OldBg.R = m_OldBg.StartR;
	m_OldBg.G = m_OldBg.StartG;
	m_OldBg.B = m_OldBg.StartB;

	m_NewBg.StartA = g_LibPol.GetInfo( m_NewBg.PolNo ).A;
	m_NewBg.StartR = g_LibPol.GetInfo( m_NewBg.PolNo ).R;
	m_NewBg.StartG = g_LibPol.GetInfo( m_NewBg.PolNo ).G;
	m_NewBg.StartB = g_LibPol.GetInfo( m_NewBg.PolNo ).B;
	m_NewBg.TargetA = 255;
	m_NewBg.TargetR = g_LibPol.GetInfo( m_NewBg.PolNo ).R;
	m_NewBg.TargetG = g_LibPol.GetInfo( m_NewBg.PolNo ).G;
	m_NewBg.TargetB = g_LibPol.GetInfo( m_NewBg.PolNo ).B;
	m_NewBg.A = m_NewBg.StartA;
	m_NewBg.R = m_NewBg.StartR;
	m_NewBg.G = m_NewBg.StartG;
	m_NewBg.B = m_NewBg.StartB;

	m_State = ON;
}

/******************************************************************************/
//	クロスフェードループ
/******************************************************************************/
void	CScriptWipeEngine::LoopCrossFade( void )
{
	m_Count ++;
	if( m_Count >= m_Time )
	{
		m_Count = m_Time;
		m_State = OFF;
		g_LibPol.SetInfo( m_OldBg.PolNo, g_LibPol.GetInfo( m_NewBg.PolNo ) );
		PAT[ m_OldBg.PatNo ] = PAT[ m_NewBg.PatNo ];
		TEX[ m_OldBg.TexNo ] = TEX[ m_NewBg.TexNo ];

		InitTexture( m_NewBg.TexNo );
		InitPattern( m_NewBg.PatNo );
		g_LibPol.Init( m_NewBg.PolNo );

		g_LibPol.SetColor( m_OldBg.PolNo, 255,255,255,255 );
		g_LibPol.ChangeDisp( m_OldBg.PolNo, ON );
		return;
	}

	//	ループ処理
	if( m_Count == m_Time )
	{
		/////////////////////////////////////
		//	フェード終了
		m_OldBg.A = m_OldBg.TargetA;
		m_OldBg.R = m_OldBg.TargetR;
		m_OldBg.G = m_OldBg.TargetG;
		m_OldBg.B = m_OldBg.TargetB;

		m_NewBg.A = m_NewBg.TargetA;
		m_NewBg.R = m_NewBg.TargetR;
		m_NewBg.G = m_NewBg.TargetG;
		m_NewBg.B = m_NewBg.TargetB;
	}
	else
	{
		/////////////////////////////////////
		//	フェード中
		m_OldBg.A += ( m_OldBg.TargetA - m_OldBg.StartA ) / m_Time;
		m_OldBg.R += ( m_OldBg.TargetR - m_OldBg.StartR ) / m_Time;
		m_OldBg.G += ( m_OldBg.TargetG - m_OldBg.StartG ) / m_Time;
		m_OldBg.B += ( m_OldBg.TargetB - m_OldBg.StartB ) / m_Time;

		m_NewBg.A += ( m_NewBg.TargetA - m_NewBg.StartA ) / m_Time;
		m_NewBg.R += ( m_NewBg.TargetR - m_NewBg.StartR ) / m_Time;
		m_NewBg.G += ( m_NewBg.TargetG - m_NewBg.StartG ) / m_Time;
		m_NewBg.B += ( m_NewBg.TargetB - m_NewBg.StartB ) / m_Time;
	}

	//	色クリッピング
	MaxMin( m_OldBg.A, 255, 0 );
	MaxMin( m_OldBg.R, 255, 0 );
	MaxMin( m_OldBg.G, 255, 0 );
	MaxMin( m_OldBg.B, 255, 0 );

	MaxMin( m_NewBg.A, 255, 0 );
	MaxMin( m_NewBg.R, 255, 0 );
	MaxMin( m_NewBg.G, 255, 0 );
	MaxMin( m_NewBg.B, 255, 0 );

	g_LibPol.SetColor( m_OldBg.PolNo,
		(UChar)m_OldBg.A, (UChar)m_OldBg.R, (UChar)m_OldBg.G, (UChar)m_OldBg.B );
	g_LibPol.SetColor( m_NewBg.PolNo,
		(UChar)m_NewBg.A, (UChar)m_NewBg.R, (UChar)m_NewBg.G, (UChar)m_NewBg.B );
}
