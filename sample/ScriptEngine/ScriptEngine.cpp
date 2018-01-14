/******************************************************************************/
//	スクリプト
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibPad.h"
#include	"./ScriptEngine.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	スクリプトコマンド文字列
char	ScriptCmdStr[CommandMax][32] =
{
	"text",			//	0	文字列表示
	"end",			//	1	終了
	"jump",			//	2	ラベルジャンプ
	"sel",			//	3	選択肢
	"flag_on",		//	4	フラグON
	"flag_off",		//	5	フラグOFF
	"flag_chk",		//	6	フラグチェック
	"black_out",	//	7	フェードアウト（ブラックアウト）
	"black_in",		//	8	フェードイン（ブラックイン）
	"white_out",	//	9	フェードアウト（ホワイトアウト）
	"white_in",		//	10	フェードイン（ホワイトイン）
	"cursor_blink",	//	11	カーソルブリンク
	"wait",			//	12	遅延
	"scr_change",	//	13	スクリプトファイル変更
	"text_input",	//	14	文字列入力
	"set_str",		//	15	文字列をセット
	"save_point",	//	16	セーブポイント
	"shake",		//	17	画面を揺らす
	"load_tex",		//	18	テクスチャ画像読み込み
	"set_pat",		//	19	パターンセット
	"set_pol",		//	20	ポリゴンセット
	"load_bg",		//	21	背景読み込み
	"disp_bg",		//	22	背景表示・非表示
	"wipe",			//	23	ワイプ
	"play_bgm",		//	24	BGM再生
	"stop_bgm",		//	25	BGM停止
	"play_se",		//	26	SE再生
	"init_str",		//	27	セット文字列初期化
};



/******************************************************************************/
//	名前	：	コンストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CScriptEngine::CScriptEngine()
: m_UseTextureNo(TEX_SCRIPT_BG),
m_UsePatNo(PAT_SCRIPT_BG),
m_UsePolNo(POL_SCRIPT_BG),
m_UseTextNo(TEXT_SCRIPT_NO),
m_UseBGMNo(AUDIO_SCRIPT_BGM),
m_UseSENo(AUDIO_SCRIPT_SE),
m_Wipe(NULL)
{
}

/******************************************************************************/
//	名前	：	コンストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	[IN]long	TextureNo	使用するテクスチャ番号
//				[IN]long	PatNo		使用するパターン番号
//				[IN]long	PolNo		使用するポリゴン番号
//				[IN]long	TextNo		使用するテキスト番号
//				[IN]long	BGMno		使用するBGM番号
//				[IN]long	SENo		使用するSE番号
//	備考	：	なし
/******************************************************************************/
CScriptEngine::CScriptEngine(long TextureNo,
							 long PatNo,
							 long PolNo,
							 long TextNo,
							 long BGMNo,
							 long SENo )
: m_UseTextureNo(TEX_SCRIPT_BG),
m_UsePatNo(PAT_SCRIPT_BG),
m_UsePolNo(POL_SCRIPT_BG),
m_UseTextNo(TEXT_SCRIPT_NO),
m_UseBGMNo(AUDIO_SCRIPT_BGM),
m_UseSENo(AUDIO_SCRIPT_SE),
m_Wipe(NULL)
{
	if( TextureNo < TEX_MAX && TextureNo > NOT )
	{
		m_UseTextureNo = TextureNo;
	}
	if( PatNo < PAT_MAX && PatNo > NOT )
	{
		m_UsePatNo = PatNo;
	}
	if( PolNo < POL_MAX && PolNo > NOT)
	{
		m_UsePolNo = PolNo;
	}
	if( TextNo < TXT_MAX && TextNo > NOT )
	{
		m_UseTextNo = TextNo;
	}
	if( BGMNo < AUDIO_MAX && BGMNo > NOT )
	{
		m_UseBGMNo = BGMNo;
	}
	if( SENo < AUDIO_MAX && SENo > NOT )
	{
		m_UseSENo = SENo;
	}
}

/******************************************************************************/
//	名前	：	デストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CScriptEngine::~CScriptEngine()
{
	SAFE_DELETE(m_Wipe);
}

/******************************************************************************/
//	名前	：	スクリプトエンジン初期化
//	説明	：	スクリプトファイルの世も込みを行う
//	戻り値	：	なし
//	引数	：	[IN]char	*FileName	読み込むスクリプトファイル
//	備考	：	なし
/******************************************************************************/
void	CScriptEngine::Init( char *FileName )
{
	//	構造体初期化
	m_SCR.Mode = 0;				//	セットモード
	m_SCR.Addr = m_ScrDat;		//	先頭アドレスセット
	m_SCR.WaitFlag = OFF;		//	遅延
	m_SCR.BlinkCursor = OFF;	//	カーソル点滅
	m_SCR.CmdNo = 0;			//	コマンド番号
	m_SCR.SelectNo = 0;			//	選択肢番号

	//	その他初期化
	ZeroMemory( &m_SCR.JumpStr, sizeof(m_SCR.JumpStr) );
	ZeroMemory( &m_SCR.EventFlag, sizeof(m_SCR.EventFlag) );
	ZeroMemory( &m_SCR.SET, sizeof(m_SCR.SET) );
	ZeroMemory( &m_SCR.SHAKE, sizeof(m_SCR.SHAKE) );
	ZeroMemory( &m_TEXT, sizeof(m_TEXT) );

	CDx9LibIni IniMng;
	IniMng.Initialize("ScriptSetup.ini");
	int GetData = IniMng.GetValueInt( "System", "UseSystem" );

	//	スクリプトファイル読み込み
	LoadFile( FileName, m_ScrDat );

	long	SysTexNo = m_UseTextureNo + TEX_SCRIPT_SYS;
	long	CurPatNo = PAT_SCRIPT_BG + PAT_SCRIPT_CUR;
	long	TextFldPatNo = PAT_SCRIPT_BG +  PAT_SCRIPT_TEXT_FLD;
	long	CurPolNo = POL_SCRIPT_BG + POL_SCRIPT_CUR;
	long	TextFldPolNo = POL_SCRIPT_BG + POL_SCRIPT_TEXT_FLD;

	//	システムファイル読み込み
	LoadTexture( SysTexNo, "data\\system.bmp" );

	SetPattern( CurPatNo, SysTexNo, 0,0, 24,24 );
	SetPattern( TextFldPatNo, SysTexNo, 0,24, 640,88 );

	g_LibPol.SetPat( CurPolNo, CurPatNo, TEXT_CURSOR_X,TEXT_CURSOR_Y,1.0, ATR_NONE );
	g_LibPol.SetPat( TextFldPolNo, TextFldPatNo, TEXT_FLD_X,TEXT_FLD_Y,1.0, ATR_NONE );
	g_LibPol.Set( TextFldPolNo+1, 624,72, TEXT_FLD_BG_X, TEXT_FLD_BG_Y,0.9, ATR_NONE );
	g_LibPol.SetColor( TextFldPolNo+1, 255, 0, 0, 0 );

	m_CursorTimer = 0;
}

/******************************************************************************/
//	名前	：	スクリプトエンジンメイン
//	説明	：	スクリプトエンジンのメイン処理を行う
//	引数	：	なし
//	戻り値	：	なし
//	備考	：	なし
/******************************************************************************/
void	CScriptEngine::Main( void )
{
	if( m_SCR.Mode == 0 )
	{
		Set();	//	セットモード
	}
	else if( m_SCR.Mode )
	{
		Loop();	//	ループモード
		DispTextLoop();
	}
}

/******************************************************************************/
//	名前	：	スクリプトセット
//	説明	：	スクリプトの設定を行う
//	引数	：	なし
//	戻り値	：	なし
//	備考	：	なし
/******************************************************************************/
void	CScriptEngine::Set( void )
{
	while( m_SCR.Mode == 0 )
	{
		//	次のコマンド取得
		m_SCR.CmdNo = GetNextCommand();
		switch( m_SCR.CmdNo )
		{
			//	テキストコマンド・セット
			case	Command_text:
				CommandTextSet();
				break;
			//	終了コマンド・セット
			case	Command_end:
				CommandEndSet();
				break;
			//	ジャンプコマンド・セット
			case	Command_jump:
				CommandJumpSet();
				break;
			//	選択肢コマンド・セット
			case	Command_sel:
				CommandSelSet();
				break;
			//	フラグONコマンド・セット
			case	Command_flag_on:
				CommandFlagOnSet();
				break;
			//	フラグOFFコマンド・セット
			case	Command_flag_off:
				CommandFlagOffSet();
				break;
			//	フラグチェックコマンド・セット
			case	Command_flag_chk:
				CommandFlagChkSet();
				break;
			//	ブラックアウトコマンド・セット
			case	Command_black_out:
				CommandBlackOutSet();
				break;
			//	ブラックインコマンド・セット
			case	Command_black_in:
				CommandBlackInSet();
				break;
			//	ホワイトアウトコマンド・セット
			case	Command_white_out:
				CommandWhiteOutSet();
				break;
			//	ホワイトインコマンド・セット
			case	Command_white_in:
				CommandWhiteInSet();
				break;
			//	カーソル点滅コマンド・セット
			case	Command_cursor_blink:
				CommandCursorBlinkSet();
				break;
			//	遅延コマンド・セット
			case	Command_wait:
				CommandWaitSet();
				break;
			//	スクリプトファイル変更コマンド・セット
			case	Command_scr_change:
				CommandScriptChangeSet();
				break;
			//	テキスト入力コマンド・セット
			case	Command_text_input:
				CommandTextInputSet();
				break;
			//	文字列セットコマンド・セット
			case	Command_set_str:
				CommandSetStrSet();
				break;
			//	セーブポイントコマンド・セット
			case	Command_save_point:
				CommandSavePointSet();
				break;
			//	画面揺らすコマンド・セット
			case	Command_shake:
				CommandShakeSet();
				break;
			//	テクスチャ画像読み込み
			case	Command_load_tex:
				CommandLoadTex();
				break;
			//	パターンセット
			case	Command_set_pat:
				CommandSetPat();
				break;
			//	ポリゴンセット
			case	Command_set_pol:
				CommandSetPol();
				break;
			//	背景読み込み
			case	Command_load_bg:
				CommandLoadBg();
				break;
			//	背景表示・非表示
			case	Command_disp_bg:
				CommandDispBg();
				break;
			//	ワイプ
			case	Command_wipe:
				CommandWipe();
				break;
			//	BGM再生
			case	Command_play_bgm:
				CommandPlayBGM();
				break;
			//	BGM停止
			case	Command_stop_bgm:
				CommandStopBGM();
				break;
			//	SE再生
			case	Command_play_se:
				CommandPlaySE();
				break;
			//	セット文字列初期化
			case	Command_init_str:
				CommandInitStr();
				break;
		}
	}
}

/******************************************************************************/
//	名前	：	スクリプトループ
//	説明	：	スクリプトのループ処理を行う
//	引数	：	なし
//	戻り値	：	なし
//	備考	：	なし
/******************************************************************************/
void	CScriptEngine::Loop( void )
{
	//	各コマンドのループ処理
	switch( m_SCR.CmdNo )
	{
		//	テキストコマンド・ループ
		case	Command_text:
			CommandTextLoop();
			break;
		//	選択肢コマンド・ループ
		case	Command_sel:
			CommandSelLoop();
			break;
		//	ブラックアウトコマンド・ループ
		case	Command_black_out:
			CommandBlackOutLoop();
			break;
		//	ブラックインコマンド・ループ
		case	Command_black_in:
			CommandBlackInLoop();
			break;
		//	ホワイトアウトコマンド・ループ
		case	Command_white_out:
			CommandWhiteOutLoop();
			break;
		//	ホワイトインコマンド・ループ
		case	Command_white_in:
			CommandWhiteInLoop();
			break;
		//	カーソル点滅コマンド・ループ
		case	Command_cursor_blink:
			CommandCursorBlinkLoop();
			break;
		//	画面揺らすコマンド・ループ
		case	Command_shake:
			CommandShakeLoop();
			break;
		//	ワイプ
		case	Command_wipe:
			if( !m_Wipe->Loop() )
			{
				m_SCR.Mode = 0;
			}
			break;

	}
}

/******************************************************************************/
//	名前	：	次のコマンド取得
//	説明	：	次のコマンドを取得する
//	戻り値	：	long	コマンド番号
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
long	CScriptEngine::GetNextCommand( void )
{
	long	ret;

	//	次の「*」を検索
	m_SCR.Addr = ScriptSearchChar( m_SCR.Addr, '*' );

	ret = 1000;

	for( int i=0; i<CommandMax; i++ )
	{
		if( ScriptStrComp( m_SCR.Addr, ScriptCmdStr[i] ) == 1 )
		{
			//	コマンド発見
			ret = i;

			//	アドレスをコマンド名の後ろに移動
			m_SCR.Addr = ScriptSkipStr( m_SCR.Addr );

			break;
		}
	}

	return ret;
}

/******************************************************************************/
//	文字列表示
//		引数：
//				なし
//		戻り値：
//				なし
/******************************************************************************/
void	CScriptEngine::DispTextLoop( void )
{
	//	テキスト登録
	for( int i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		//	文字列の色
		switch( m_TEXT.SST[i].TextColor )
		{
			case	SETextColor::Normal:	//	通常
				g_LibText.SetColor( m_UseTextNo+i, 255,255,255,255 );
				break;
			case	SETextColor::Select:	//	選択
				g_LibText.SetColor( m_UseTextNo+i, 255,200,230,255 );
				break;
			case	SETextColor::NotSelect:	//	非選択
				g_LibText.SetColor( m_UseTextNo+i, 255,120,120,120 );
				break;
		}
	}
}

/******************************************************************************/
//	テキストセット
//		引数：
//				なし
//		戻り値：
//				なし
/******************************************************************************/
void	CScriptEngine::CommandTextSet( void )
{
	int i;

	//	TEXT_LINE_MAX分ループ
	for( i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		//	「"」内の文字列を抽出
		m_SCR.Addr = ScriptGetStrDouble( m_SCR.Addr, m_TEXT.SST[i].Text );

		//	次の行はあるのか判定
		m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

		//	文字列の色設定
		m_TEXT.SST[i].TextColor = SETextColor::Normal;

		//	セットした文字列があるか判定
		if( strstr( m_TEXT.SST[i].Text, "STR" ) != NULL )
		{
			//	存在する
			SwapStr( m_TEXT.SST[i].Text );
		}

		g_LibText.Set( m_UseTextNo+i,
			TEXT_DISP_X,TEXT_DISP_Y+i*FONT_SIZE,
			255,255,255,255, m_TEXT.SST[i].Text, ATR_ONCE ); 

		TXT[ m_UseTextNo+i ].Disp = OFF;


		//	次の文字が「"」かどうか
		if( *m_SCR.Addr != '"' )
		{
			break;
		}
	}

	m_TEXT.MaxLine = i + 1;
	m_TEXT.Count = 0;
	m_TEXT.CrtLine = 0;
	m_TEXT.CrtTime = 0;
	m_TEXT.State = 0;

	m_SCR.Mode = 1;
}

/******************************************************************************/
//	終了・セット
//		引数：
//				なし
//		戻り値：
//				なし
/******************************************************************************/
void	CScriptEngine::CommandEndSet( void )
{
	m_SCR.Mode = 100;
	PostQuitMessage( 0 );
}

/******************************************************************************/
//	ラベルジャンプ
//		引数：
//				なし
//		戻り値：
//				なし
/******************************************************************************/
void	CScriptEngine::CommandJumpSet( void )
{
	//	ラベルまでスキップ
	m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

	//	ラベル抽出
	m_SCR.JumpStr[0][0] = '#';	//	ラベル用記号
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, &m_SCR.JumpStr[0][1] );

	//	ラベルジャンプ
	m_SCR.Addr = ScriptLabelJump( m_ScrDat, m_SCR.JumpStr[0] );
}

/******************************************************************************/
//	選択肢コマンド・セット
//		引数：
//				なし
//		戻り値：
//				なし
/******************************************************************************/
void	CScriptEngine::CommandSelSet( void )
{
	long	i;

	//	行数ループ
	for( i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		//	空白文字列抽出
		m_SCR.JumpStr[i][0] = '#';
		m_SCR.Addr = ScriptGetStr( m_SCR.Addr, &m_SCR.JumpStr[i][1] );

		m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

		//	次が「"」か判定
		if( *m_SCR.Addr == '"' )
		{
			m_TEXT.MaxLine = i + 1;
			break;
		}
	}

	//	選択肢の数分ループ
	for( i=0; i<m_TEXT.MaxLine; i++ )
	{
		//	「"」内の文字列抽出
		m_SCR.Addr = ScriptGetStrDouble( m_SCR.Addr, m_TEXT.SST[i].Text );

		//	次の行はあるのか判定
		m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

		//	次の文字列の色を判定
		m_TEXT.SST[i].TextColor = SETextColor::NotSelect;	//	非選択

		g_LibText.Set( m_UseTextNo+i,
			TEXT_DISP_X,
			TEXT_DISP_Y+i*FONT_SIZE,
			255,255,255,255, m_TEXT.SST[i].Text );

		if( *m_SCR.Addr != '"' )
		{
			break;
		}
	}

	//	選択肢番号初期化
	m_SCR.SelectNo = 0;

	//	ループ
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	フラグコマンドON・セット
/******************************************************************************/
void	CScriptEngine::CommandFlagOnSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	フラグ番号文字列の抽出
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//	数字の文字列を数値へ変換
	long no = atol( GetStr );
	if( no >= SCR_FLAG_MAX || no < 0 )
	{
		return;
	}

	//	フラグON
	m_SCR.EventFlag[ no ] = ON;
}

/******************************************************************************/
//	フラグコマンドOFF・セット
/******************************************************************************/
void	CScriptEngine::CommandFlagOffSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	フラグ番号文字列の抽出
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//	数字の文字列を数値へ変換
	long no = atol( GetStr );

	//	フラグON
	m_SCR.EventFlag[ no ] = OFF;
}

/******************************************************************************/
//	フラグコマンドチェック・セット
/******************************************************************************/
void	CScriptEngine::CommandFlagChkSet( void )
{
	//	フラグ番号文字列の抽出
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, m_SCR.JumpStr[0] );
	//	数字の文字列を数値へ変換
	long no = atol( m_SCR.JumpStr[0] );
	if( no >= SCR_FLAG_MAX || no < 0 )
	{
		return;
	}

	//	飛び先のラベルの抽出
	m_SCR.JumpStr[1][0] = '#';
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, &m_SCR.JumpStr[1][1] );

	//	フラグ判定
	if( m_SCR.EventFlag[ no ] )
	{
		//	ラベルジャンプ
		m_SCR.Addr = ScriptLabelJump( m_ScrDat, m_SCR.JumpStr[1] );
	}
}

/******************************************************************************/
//	ブラックアウトコマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandBlackOutSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	時間取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.BlackOut( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	ブラックインコマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandBlackInSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	時間取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.BlackIn( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	ホワイトアウトコマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandWhiteOutSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	時間取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.WhiteOut( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	ホワイトインコマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandWhiteInSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	時間取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.WhiteIn( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	カーソル点滅コマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandCursorBlinkSet( void )
{
	//	テキスト表示解除
	m_TEXT.DispFlag = OFF;

	//	カーソル点滅開始
	m_SCR.BlinkCursor = ON;

	//	モード変更
	m_SCR.Mode = 1;

	//	カーソル表示
	g_LibPol.ChangeDisp( m_UsePolNo + POL_SCRIPT_CUR, ON );
}

/******************************************************************************/
//	遅延コマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandWaitSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	時間取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol( GetStr );

	WaitTime( Time );
}

/******************************************************************************/
//	スクリプトファイル変更コマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandScriptChangeSet( void )
{
}

/******************************************************************************/
//	テキスト入力コマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandTextInputSet( void )
{
	//DialogTest();
}

/******************************************************************************/
//	文字列セットコマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandSetStrSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long StrNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );

	if( StrNo >= STR_SET_MAX )
	{
		return;
	}

	//	名前取得
	ZeroMemory( &m_SCR.SET[StrNo], sizeof(m_SCR.SET[StrNo]) );
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, m_SCR.SET[StrNo].Text );
}

/******************************************************************************/
//	セーブポイントコマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandSavePointSet( void )
{
}

/******************************************************************************/
//	画面揺らすコマンド・セット
/******************************************************************************/
void	CScriptEngine::CommandShakeSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	強さ取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	m_SCR.SHAKE.Value = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	時間取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	m_SCR.SHAKE.Time = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	属性取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	m_SCR.SHAKE.Atr = 0;
	if( strcmp( GetStr, "XY" ) == 0 )
	{
		m_SCR.SHAKE.Atr = SHAKAE_ATR_XY;
	}
	else if( strcmp( GetStr, "X" ) == 0 )
	{
		m_SCR.SHAKE.Atr = SHAKAE_ATR_X;
	}
	else if( strcmp( GetStr, "Y" ) == 0 )
	{
		m_SCR.SHAKE.Atr = SHAKAE_ATR_Y;
	}

	//	テキスト表示解除
	m_TEXT.DispFlag = OFF;
	//	モード変更
	m_SCR.Mode = 1;
	//	カウンタ初期化
	m_SCR.SHAKE.Count = 0;
	//	状態初期化
	m_SCR.SHAKE.State = 0;
}

/******************************************************************************/
//	テクスチャ画像読み込み
/******************************************************************************/
void	CScriptEngine::CommandLoadTex( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	テクスチャ番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long TexNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	ファイル名取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	//	テクスチャ読み込み
	LoadTexture( TexNo, GetStr );
}

/******************************************************************************/
//	パターンセット
/******************************************************************************/
void	CScriptEngine::CommandSetPat( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	パターン番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PatNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	テクスチャ番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long TexNo = atol( GetStr );
	//	開始座標X取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PosX = atol( GetStr );
	//	開始座標Y取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PosY = atol( GetStr );
	//	横サイズ取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Width = atol( GetStr );
	//	縦サイズ取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Height = atol( GetStr );

	//	パターンセット
	SetPattern( PatNo, TexNo, PosX,PosY, Width,Height );
}

/******************************************************************************/
//	ポリゴンセット
/******************************************************************************/
void	CScriptEngine::CommandSetPol( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	ポリゴン番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PolNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	パターン番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PatNo = atol( GetStr );
	//	表示座標X取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	double PosX = atof( GetStr );
	//	表示座標Y取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	double PosY = atof( GetStr );
	//	表示座標Z取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	double PosZ = atof( GetStr );
	//	アトリビュート取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	UShort Atr = ATR_NONE;
	if( strcmp( GetStr, "ATR_REV_W" ) == 0 )
	{
		Atr = ATR_REV_W;
	}
	else if( strcmp( GetStr, "ATR_REV_H" ) == 0 )
	{
		Atr = ATR_REV_H;
	}
	else if( strcmp( GetStr, "ATR_ALL" ) == 0 )
	{
		Atr = ATR_ALL;
	}

	//	ポリゴンセット
	g_LibPol.SetPat( PolNo, PatNo, PosX,PosY,PosZ, Atr );
}

/******************************************************************************/
//	背景読み込み
/******************************************************************************/
void	CScriptEngine::CommandLoadBg( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	背景番号取得
	//m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//long BgNo = atol( GetStr );
	//ZeroMemory( &GetStr, sizeof(GetStr) );

	//	ファイル名取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	//	テクスチャ読み込み
	LoadTexture( m_UseTextureNo, GetStr );
	SetPattern( m_UsePatNo,
		m_UseTextureNo,
		0,0,
		g_WindowWidth,
		g_WindowHeight
		);
	g_LibPol.SetPat( m_UsePolNo, m_UsePatNo, 0,0,0, ATR_NONE );
	g_LibPol.ChangeDisp( m_UsePolNo, OFF );
}
	
/******************************************************************************/
//	背景表示・非表示
/******************************************************************************/
void	CScriptEngine::CommandDispBg( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	背景番号取得
	//m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//long BgNo = atol( GetStr );
	//ZeroMemory( &GetStr, sizeof(GetStr) );

	//	状態取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	UChar Flag = ON;
	if( strcmp( GetStr, "ON" ) == 0 )
	{
		Flag = ON;
	}
	else if( strcmp( GetStr, "OFF" ) == 0 )
	{
		Flag = OFF;
	}

	g_LibPol.ChangeDisp( m_UsePolNo, Flag );
}

/******************************************************************************/
//	ワイプ
/******************************************************************************/
void	CScriptEngine::CommandWipe( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	ワイプ番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long	WipeNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	時間取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long	Time = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	ファイル名取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	SAFE_DELETE(m_Wipe);
	m_Wipe = new CScriptWipeEngine( m_UseTextureNo, m_UsePatNo, m_UsePolNo );
	m_Wipe->Init( WipeNo, Time, GetStr );

	m_SCR.Mode = 1;
}

/******************************************************************************/
//	BGM再生
/******************************************************************************/
void	CScriptEngine::CommandPlayBGM( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	ファイル名取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	g_LibMusic.Load( m_UseBGMNo, GetStr );
	g_LibMusic.Play( m_UseBGMNo, LOOP );
}

/******************************************************************************/
//	BGM停止
/******************************************************************************/
void	CScriptEngine::CommandStopBGM( void )
{
	g_LibMusic.Stop( m_UseBGMNo );
}

/******************************************************************************/
//	SE再生
/******************************************************************************/
void	CScriptEngine::CommandPlaySE( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	ファイル名取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	g_LibMusic.Load( m_UseSENo, GetStr );
	g_LibMusic.Play( m_UseSENo, ONE );
}

/******************************************************************************/
//	セット文字列初期化
/******************************************************************************/
void	CScriptEngine::CommandInitStr( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	番号取得
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long	StrNo = atol( GetStr );
	if( StrNo >= STR_SET_MAX )
	{
		return;
	}

	ZeroMemory( &m_SCR.SET[StrNo], sizeof(m_SCR.SET[StrNo]) );
}

/******************************************************************************/
//	テキストコマンド・ループ
/******************************************************************************/
void	CScriptEngine::CommandTextLoop( void )
{
	long TextNo = m_UseTextNo+m_TEXT.CrtLine;
	TXT[ TextNo ].Disp = ON;

	if( TXT[ TextNo ].State )
	{
		m_TEXT.CrtLine ++;
		if( m_TEXT.CrtLine >= m_TEXT.MaxLine )
		{
			m_SCR.Mode = 0;
		}
	}

	if( MOUSE.Trg[ MOUSE_LEFT ] || KEY.Trg[ DIK_SPACE ] )
	{
		for( int i=0; i<m_TEXT.MaxLine; i++ )
		{
			TXT[ m_UseTextNo + i ].Disp = ON;
			TXT[ m_UseTextNo + i ].State = ON;
			TXT[ m_UseTextNo + i ].Step = -1;
		}
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	選択肢・ループ
//		引数：
//				なし
//		戻り値：
//				なし
/******************************************************************************/
void	CScriptEngine::CommandSelLoop( void )
{
	long	i;

	//	入力判定・選択キー
	if( KEY.Trg[ DIK_UP ] )
	{
		//	上
		if( m_SCR.SelectNo > 0 )
		{
			m_SCR.SelectNo --;
		}
	}
	else if( KEY.Trg[ DIK_DOWN ] )
	{
		//	下
		if( m_SCR.SelectNo < m_TEXT.MaxLine-1 )
		{
			m_SCR.SelectNo ++;
		}
	}

	//	文字列の色設定
	for( i=0; i<m_TEXT.MaxLine; i++ )
	{
		if( m_SCR.SelectNo == i )
		{
			m_TEXT.SST[i].TextColor = SETextColor::Select;
		}
		else
		{
			m_TEXT.SST[i].TextColor = SETextColor::NotSelect;
		}
	}

	//	選択判定・決定きー
	if( KEY.Trg[ DIK_SPACE ] || MOUSE.Trg[ MOUSE_LEFT ] )
	{
		//	ラベルジャンプ
		m_SCR.Addr = ScriptLabelJump( m_ScrDat, m_SCR.JumpStr[m_SCR.SelectNo] );

		//	文字列クリア
		ZeroMemory( &m_TEXT.SST, sizeof(m_TEXT.SST) );

		//	セット
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	ブラックアウトコマンド・ループ
/******************************************************************************/
void	CScriptEngine::CommandBlackOutLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	ブラックインコマンド・ループ
/******************************************************************************/
void	CScriptEngine::CommandBlackInLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	ホワイトアウトコマンド・ループ
/******************************************************************************/
void	CScriptEngine::CommandWhiteOutLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	ホワイトインコマンド・ループ
/******************************************************************************/
void	CScriptEngine::CommandWhiteInLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	カーソル点滅コマンド・ループ
/******************************************************************************/
void	CScriptEngine::CommandCursorBlinkLoop( void )
{
	long	i,but=NOT;
	static long	bFlag = OFF;

	for( i=0; i<256; i++ )
	{
		if( KEY.Trg[ i ] )
		{
			sprintf_s( TXT[10].Str, MAX_PATH, "%d", i );
			but = ON;
			break;
		}
	}
	if( MOUSE.Trg[ MOUSE_LEFT ] )
	{
		but = ON;
	}

	if( but == NOT )
	{
		if( m_CursorTimer >= 20 )
		{
			g_LibPol.ChangeDispRev( m_UsePolNo + POL_SCRIPT_CUR );
			m_CursorTimer = 0;
		}
		else
		{
			m_CursorTimer ++;
		}

		return;
	}

	//	テキスト表示開始
	m_TEXT.DispFlag = ON;

	//	カーソル点滅解除
	m_SCR.BlinkCursor = OFF;

	//	モード変更
	m_SCR.Mode = 0;

	//	文字列クリア
	for( i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		ZeroMemory( &m_TEXT.SST[i].Text, sizeof(m_TEXT.SST[i].Text) );
		g_LibText.Init( m_UseTextNo + i );
	}

	//	カーソル非表示
	g_LibPol.ChangeDisp( m_UsePolNo + POL_SCRIPT_CUR, OFF );
}

/******************************************************************************/
//	画面揺らすコマンド・ループ
/******************************************************************************/
void	CScriptEngine::CommandShakeLoop( void )
{
	long	AddValue[]=
	{
		0,3,5,8,10
	};
	long	PolNo = m_UsePolNo;
	long	CrtTime = m_SCR.SHAKE.Time / 16;
	long	Flag = m_SCR.SHAKE.State / 4;

	if( m_SCR.SHAKE.State < 16 )
	{
		if( m_SCR.SHAKE.State % 2 )
		{
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_X ) == SHAKAE_ATR_X )
			{
				g_LibPol.AddPosX( PolNo, -(m_SCR.SHAKE.Value-AddValue[Flag]) );
			}
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_Y ) == SHAKAE_ATR_Y )
			{
				g_LibPol.AddPosY( PolNo, -(m_SCR.SHAKE.Value-AddValue[Flag]) );
			}
		}
		else
		{
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_X ) == SHAKAE_ATR_X )
			{
				g_LibPol.AddPosX( PolNo, m_SCR.SHAKE.Value-AddValue[Flag] );
			}
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_Y ) == SHAKAE_ATR_Y )
			{
				g_LibPol.AddPosY( PolNo, m_SCR.SHAKE.Value-AddValue[Flag] );
			}
		}
		if( CrtTime <= m_SCR.SHAKE.Count )
		{
			m_SCR.SHAKE.State ++;
			m_SCR.SHAKE.Count = 0;
		}
	}
	else
	{
		g_LibPol.SetPos( PolNo, 0, 0 );

		//	モード変更
		m_SCR.Mode = 0;
	}

	m_SCR.SHAKE.Count ++;
}

/******************************************************************************/
//	文字検索
//		引数：
//				*pt		文字列ポインタ
//				ch		検索する文字
//		戻り値：
//				なし
/******************************************************************************/
char	*CScriptEngine::ScriptSearchChar( char *pt, char ch )
{
	UChar	uc;

	while( 1 )
	{
		//	コメントスキップ
		pt = ScriptSkipComment( pt );

		if( *pt == ch )
		{
			break;
		}

		uc = (UChar)*pt;

		if( (uc<128) || (uc>=160) )
		{
			pt ++;		//	半角
		}
		else
		{
			pt += 2;	//	全角
		}
	}

	pt ++;
	return pt;
}

/******************************************************************************/
//	次の文字列までの空白のスキップ
//		（スペース、タブ、改行、カンマ、NULL以外まで）
//		引数：
//				*pt		文字列ポインタ
//		戻り値：
//				変更後の文字列ポインタ
/******************************************************************************/
char	*CScriptEngine::ScriptSkipSpace( char *pt )
{
	char	c;

	while( 1 )
	{
		//	コメントスキップ
		pt = ScriptSkipComment( pt );

		c = *pt;

		//	スペース、タブ、改行、カンマ判定
		if( (c!=' ') && (c!=0x09) && (c!=0x0d) && (c!=0x0a) && (c!=',') && (c!=0) )
		{
			break;
		}

		pt ++;
	}

	return pt;
}

/******************************************************************************/
//	次の空白までの文字列をスキップ
//		（スペース、タブ、改行、カンマ、NULLまで）
//		引数：
//				*pt		文字列ポインタ
//		戻り値：
//				変更後の文字列ポインタ
/******************************************************************************/
char	*CScriptEngine::ScriptSkipStr( char *pt )
{
	UChar	uc;

	while( 1 )
	{
		//	コメントスキップ
		pt = ScriptSkipComment( pt );

		uc = (UChar)*pt;

		if( (uc<128) || (uc>=160) )
		{
			// スペース、タブ、改行、カンマ判定
			if( (uc==' ') || (uc==0x09) || (uc==0x0d) || (uc==0x0a) || (uc==',') || (uc==0) )
			{
				break;
			}

			//	半角分進める
			pt ++;
		}
		else
		{
			//	全角分進める
			pt += 2;
		}
	}

	return pt;
}

/******************************************************************************/
//	空白でくくられた文字列の抽出
//		（スペース、タブ、改行、カンマ、NULLまで）
//		引数：
//				pt		文字列ポインタ
//				str		文字列格納アドレス
//		戻り値：
//				変更後の文字列ポインタ
/******************************************************************************/
char	*CScriptEngine::ScriptGetStr( char *pt, char *str )
{
	char	c;

	//	空白スキップ
	pt = ScriptSkipSpace( pt );

	while( 1 )
	{
		c = *pt;

		// スペース、タブ、改行、カンマ判定
		if( (c == ' ') || (c == 0x09) || (c == 0x0d) || (c == 0x0a) || (c == ',') || (c == 0) )
		{
			break;
		}

		//	文字列の登録
		*str = c;

		//	アドレス加算
		str ++;
		pt ++;
	}

	//	ヌルコード追加
	*str = 0;

	return pt;
}

/******************************************************************************/
//	「"」でくくられた文字列の抽出
//		引数：
//				*pt		文字列ポインタ
//				*str	文字列格納アドレス
//		戻り値：
//				抽出後の文字列ポインタ
/******************************************************************************/
char	*CScriptEngine::ScriptGetStrDouble( char *pt, char *str )
{
	long	i;
	UChar	uc;

	//	「"」までスキップ
	pt = ScriptSearchChar( pt, '"' );

	while( 1 )
	{
		//	「"」の判定
		if( *pt == '"' )
		{
			pt ++;	//	「"」の次に移動
			break;
		}

		//文字列の登録
		uc = *pt;
		if( (uc<128) || (uc>=160) )
		{
			//	半角文字コピー
			*str = *pt;
			pt ++;
			str ++;
		}
		else
		{
			//	全角文字コピー
			for( i=0; i<2; i++ )
			{
				*str = *pt;
				pt ++;
				str ++;
			}
		}
	}

	//	ヌルコード追加
	*str = 0;

	return pt;
}

/******************************************************************************/
//	特定文字列へのラベルジャンプ
//		引数：
//				addr	検索開始アドレス
//				str		ラベル文字列
//		戻り値：
//				検索後アドレス
/******************************************************************************/
char	*CScriptEngine::ScriptLabelJump( char *addr, char *str )
{
	//	見つかるまでループ
	while( 1 )
	{
		if( ScriptStrComp( addr, str ) )
		{
			break;	//	発見
		}

		addr ++;
	}

	return addr;
}

/******************************************************************************/
//	文字列比較
//	（終端は、スペース、タブ、改行、カンマ、NULLで判定）
//	引数：
//			str1	文字列１
//			str2	文字列２
//
//	戻り値：
//			0:照合失敗、1:照合成功
/******************************************************************************/
long	CScriptEngine::ScriptStrComp( char *str1, char *str2 )
{
	long	i,ret=0;
	char	a,b;

	for( i=0; i<256; i++ )
	{
		a = *str1;
		b = *str2;

		//	コメントスキップ
		str1 = ScriptSkipComment( str1 );
		str2 = ScriptSkipComment( str2 );

		if( (a==' ') || (a==0x09) || (a==0x0d) || (a==0x0a) || (a==',') || (a==0) )
		{
			if( (b==' ') || (b==0x09) || (b==0x0d) || (b==0x0a) || (b==',') || (b==0) )
			{
				ret = 1;	//	照合成功
			}
			break;
		}

		if ( a != b )
		{
			break;	//	照合失敗
		}

		str1 ++;
		str2 ++;
	}

	return ret;
}

/******************************************************************************/
//	コメントスキップ
/******************************************************************************/
char	*CScriptEngine::ScriptSkipComment( char *pt )
{
	long	mode;
	char	ch1,ch2;
	UChar	uc;

	while( 1 )
	{
		//	１文字目チェック
		if( *pt != '/' )
		{
			//	コメントではない
			break;
		}

		//	２文字目チェック
		if( *(pt+1) == '/' )
		{
			mode = 0;
		}
		else if( *(pt+1) == '*' )
		{
			mode = 1;
		}
		else
		{
			break;
		}

		if( mode == 0 )
		{
			//	コメント終端は改行コード
			ch1 = 0x0d;
			ch2 = 0x0a;
		}
		else
		{
			//	コメント終端は「*/」
			ch1 = '*';
			ch2 = '/';
		}

		//	コメント終端までスキップ
		while( 1 )
		{
			if( (*pt==ch1) && (*(pt+1)==ch2) )
			{
				//	次の文字へ
				pt += 2;
				break;
			}

			uc = *pt;
			if( (uc<128) || (uc>=160) )
			{
				//	半角分進める
				pt ++;
			}
			else
			{
				//	全角分進める
				pt += 2;
			}
		}
	}

	return pt;
}

/******************************************************************************/
//	セット文字列変更
/******************************************************************************/
void	CScriptEngine::SwapStr( char *Str )
{
	char	*Num;
	Num = strstr( Str, "STR" );
	long	Pos = (long)(Num-Str)+1;

	char	cStrNo[8]={NULL};
	cStrNo[0] = Str[Pos+2];
	cStrNo[1] = Str[Pos+3];

	long	StrNo = atol( cStrNo );
	char	NewStr[MAX_PATH]={NULL};

	if( strlen(m_SCR.SET[StrNo].Text) == 0 )
	{
		return;
	}

	strncpy_s( NewStr, MAX_PATH, Str, Pos-1 );
	strcat_s( NewStr, MAX_PATH, m_SCR.SET[StrNo].Text );
	strcat_s( NewStr, MAX_PATH, &Str[Pos+4] );
	strcpy_s( Str, MAX_PATH, NewStr );
}
