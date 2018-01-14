/******************************************************************************/
//	スクリプト
//		ヘッダファイル
/******************************************************************************/

#ifndef		__ScriptEngine_H__
#define		__ScriptEngine_H__

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"ScriptWipeEngine.h"

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

//	デフォルト各種使用開始番号
enum
{
	TEX_SCRIPT_BG = 0,
	TEX_SCRIPT_SYS = 5,

	PAT_SCRIPT_BG = 0,
	PAT_SCRIPT_TEXT_FLD = 5,
	PAT_SCRIPT_CUR,

	POL_SCRIPT_BG = 0,
	POL_SCRIPT_TEXT_FLD = 5,
	POL_SCRIPT_FLD_BG,
	POL_SCRIPT_CUR,

	AUDIO_SCRIPT_BGM = 0,
	AUDIO_SCRIPT_SE = 5,

	TEXT_SCRIPT_NO = 0,
};

//	文字列関連
#define	SCR_TEXT_LINE_MAX	4	//	最大行数

//	フラグ関連
#define	SCR_FLAG_MAX	1000	//	フラグの最大数

//	テキスト表示関連
#define	TEXT_DISP_X		16
#define	TEXT_DISP_Y		404
#define	TEXT_FLD_X		0
#define	TEXT_FLD_Y		(480-84)
#define	TEXT_FLD_BG_X	(TEXT_FLD_X+8)
#define	TEXT_FLD_BG_Y	(TEXT_FLD_Y+4)
#define	TEXT_CURSOR_X	604
#define	TEXT_CURSOR_Y	(480-36)
#define	TEXT_DISP_TIME	15
#define	TEXT_PITCH(x)	(FONT_SIZE/2*x)
#define	STR_SET_MAX		16

enum
{
	Command_text,			//	0	文字列表示
	Command_end,			//	1	終了
	Command_jump,			//	2	ラベルジャンプ
	Command_sel,			//	3	選択肢
	Command_flag_on,		//	4	フラグON
	Command_flag_off,		//	5	フラグOFF
	Command_flag_chk,		//	6	フラグチェック
	Command_black_out,		//	7	フェードアウト（ブラックアウト）
	Command_black_in,		//	8	フェードイン（ブラックイン）
	Command_white_out,		//	9	フェードアウト（ホワイトアウト）
	Command_white_in,		//	10	フェードイン（ホワイトイン）
	Command_cursor_blink,	//	11	カーソルブリンク
	Command_wait,			//	12	遅延
	Command_scr_change,		//	13	スクリプトファイル変更
	Command_text_input,		//	14	文字列入力
	Command_set_str,		//	15	文字列をセット
	Command_save_point,		//	16	セーブポイント
	Command_shake,			//	17	画面を揺らす
	Command_load_tex,		//	18	テクスチャ画像読み込み
	Command_set_pat,		//	19	パターンセット
	Command_set_pol,		//	20	ポリゴンセット
	Command_load_bg,		//	21	背景読み込み
	Command_disp_bg,		//	22	背景表示・非表示
	Command_wipe,			//	23	ワイプ
	Command_play_bgm,		//	24	BGM再生
	Command_stop_bgm,		//	25	BGM停止
	Command_play_se,		//	26	SE再生
	Command_init_str,		//	27	セット文字列初期化
	CommandMax,
};

enum E_TextColor
{
	_TextColorNormal,		//	通常
	_TextColorSelect,		//	選択
	_TextColorNotSelect,	//	非選択
};

#define	SHAKAE_ATR_X		1
#define	SHAKAE_ATR_Y		2
#define	SHAKAE_ATR_XY		(SHAKAE_ATR_X|SHAKAE_ATR_Y)

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

////////////////////////////////////////////////
//	文字列用
typedef	struct
{
	E_TextColor	TextColor;		//	文字列の色
	char		Text[MAX_PATH];	//	表示文字列
}SE_STR_TBL;

/////////////////////////////////////////////////
//	背景揺らし用構造体
typedef	struct
{
	long	Value;	//	加算値
	long	Time;	//	揺らす時間（フレーム数）
	long	Count;	//	カウンタ
	long	State;	//	状態
	UChar	Atr;	//	アトリビュート(0:X、1:Y、2:XY)
}SE_SHAKE_TBL;

/////////////////////////////////////////////////
//	文字列管理用構造体
typedef	struct
{
	long		DispFlag;	//	テキスト表示（ON:表示、OFF:停止）
	long		MaxLine;	//	文字列の行数
	long		CrtLine;	//	現在の行数
	long		CrtTime;	//	カレント時間
	long		Count;		//	カウンタ
	long		State;		//	状態
	SE_STR_TBL	SST[SCR_TEXT_LINE_MAX];
}SE_TEXT_TBL;

/////////////////////////////////////////////////
//	スクリプト管理構造体
typedef	struct
{
	long			Mode;			//	モード（0:セット、1:ループ）
	long			WaitFlag;		//	遅延（ON:遅延あり、OFF:遅延なし）
	long			BlinkCursor;	//	カーソル点滅（ON:点滅中、OFF:点滅なし）

	long			CmdNo;			//	コマンド番号
	long			SelectNo;		//	選択肢番号

	char			JumpStr[SCR_TEXT_LINE_MAX][256];	//	飛び先ラベル
	char			EventFlag[SCR_FLAG_MAX];			//	イベントフラグ
	SE_STR_TBL		SET[STR_SET_MAX];
	SE_SHAKE_TBL	SHAKE;			//	画面揺らし
	char			*Addr;			//	現在のスクリプトアドレス
}SCRIPT_TBL;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CScriptEngine
{
protected:
	char				m_ScrDat[100*1024];
	SCRIPT_TBL			m_SCR;
	SE_TEXT_TBL			m_TEXT;			//	文字列管理構造体
	long				m_CursorTimer;
	long				m_UseTextureNo;
	long				m_UsePatNo;
	long				m_UsePolNo;
	long				m_UseTextNo;
	long				m_UseBGMNo;
	long				m_UseSENo;
	CScriptWipeEngine*	m_Wipe;

public:
	CScriptEngine();
	CScriptEngine( long TextureNo, long PatNo, long PolNo, long TextNo, long BGMNo, long SENo );
	~CScriptEngine();
	void	Init( char *FileName );
	void	Main( void );	//	スクリプトエンジンメイン

protected:
	/////////////////////////////////////////////////
	//	エンジン
	void	Set( void );			//	スクリプトセット
	void	Loop( void );			//	スクリトプトループ
	long	GetNextCommand( void );	//	次のコマンド取得

	/////////////////////////////////////////////////
	//	フレームループ
	void	DispTextLoop( void );

	/////////////////////////////////////////////////
	//	コマンド
	void	CommandTextSet( void );			//	テキストコマンド・セット
	void	CommandEndSet( void );			//	終了コマンド・セット
	void	CommandJumpSet( void );			//	ラベルジャンプコマンド・セット
	void	CommandSelSet( void );			//	選択肢コマンド・セット
	void	CommandFlagOnSet( void );		//	フラグコマンドON・セット
	void	CommandFlagOffSet( void );		//	フラグコマンドOFF・セット
	void	CommandFlagChkSet( void );		//	フラグコマンドチェック・セット
	void	CommandBlackOutSet( void );		//	ブラックアウトコマンド・セット
	void	CommandBlackInSet( void );		//	ブラックインコマンド・セット
	void	CommandWhiteOutSet( void );		//	ホワイトアウトコマンド・セット
	void	CommandWhiteInSet( void );		//	ホワイトインコマンド・セット
	void	CommandCursorBlinkSet( void );	//	カーソル点滅コマンド・セット
	void	CommandWaitSet( void );			//	遅延コマンド・セット
	void	CommandScriptChangeSet( void );	//	スクリプトファイル変更コマンド・セット
	void	CommandTextInputSet( void );	//	テキスト入力コマンド・セット
	void	CommandSetStrSet( void );		//	文字列セットコマンド・セット
	void	CommandSavePointSet( void );	//	セーブポイントコマンド・セット
	void	CommandShakeSet( void );		//	画面揺らすコマンド・セット
	void	CommandLoadTex( void );			//	テクスチャ画像読み込み
	void	CommandSetPat( void );			//	パターンセット
	void	CommandSetPol( void );			//	ポリゴンセット
	void	CommandLoadBg( void );			//	背景読み込み
	void	CommandDispBg( void );			//	背景表示・非表示
	void	CommandWipe( void );			//	ワイプ
	void	CommandPlayBGM( void );			//	BGM再生
	void	CommandStopBGM( void );			//	BGM停止
	void	CommandPlaySE( void );			//	SE再生
	void	CommandInitStr( void );			//	セット文字列初期化

	void	CommandTextLoop( void );		//	テキストコマンド・ループ
	void	CommandSelLoop( void );			//	選択肢コマンド・ループ
	void	CommandBlackOutLoop( void );	//	ブラックアウトコマンド・ループ
	void	CommandBlackInLoop( void );		//	ブラックインコマンド・ループ
	void	CommandWhiteOutLoop( void );	//	ホワイトアウトコマンド・ループ
	void	CommandWhiteInLoop( void );		//	ホワイトインコマンド・ループ
	void	CommandCursorBlinkLoop( void );	//	カーソル点滅コマンド・ループ
	void	CommandShakeLoop( void );		//	画面揺らすコマンド・ループ

	/////////////////////////////////////////////////
	//	スクリプト汎用
	char	*ScriptSearchChar( char *pt, char ch );		//	文字検索
	char	*ScriptSkipSpace( char *pt );				//	空白のスキップ
	char	*ScriptSkipStr( char *pt );					//	文字列スキップ
	char	*ScriptGetStr( char *pt, char *str );		//	文字取得
	char	*ScriptGetStrDouble( char *pt, char *str );	//	文字列取得
	char	*ScriptLabelJump( char *addr, char *str );	//	ラベルジャンプ
	long	ScriptStrComp( char *str1, char *str2 );	//	文字列比較
	char	*ScriptSkipComment( char *pt );				//	コメントスキップ
	void	SwapStr( char *Str );						//	セット文字列変更
};

class SETextColor
{
public:
	static const E_TextColor Normal = _TextColorNormal;
	static const E_TextColor Select = _TextColorSelect;
	static const E_TextColor NotSelect = _TextColorNotSelect;
};

#endif	//	__ScriptEngine_H__
