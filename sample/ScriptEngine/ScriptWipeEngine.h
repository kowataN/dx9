/******************************************************************************/
//	スクリプトワイプ
//		ヘッダファイル
/******************************************************************************/

#ifndef		__ScriptWipeEngine__
#define		__ScriptWipeEngine__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//
enum
{
	WIPE_CROSS_FADE = 0,
	WIPE_RIGHT_PAN,
	WIPE_LEFT_PAN,
	WIPE_UP_PAN,
	WIPE_DOWN_PAN,
	WIPE_MAX
};

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	フェード用構造体
typedef	struct
{
	long	TexNo;	//	テクスチャ番号
	long	PatNo;	//	パターン番号
	long	PolNo;	//	ポリゴン番号

	double	A,R,G,B;					//	カレントの色
	double	StartA;						//	開始色(A)
	double	StartR,StartG,StartB;		//	開始色(各色成分)
	double	TargetA;					//	目標色(A)
	double	TargetR,TargetG,TargetB;	//	目標色(各色成分)
}WIPE_FADE_TBL;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CScriptWipeEngine
{
private:
	long			m_WipeNo;	//	ワイプ番号
	long			m_State;	//	状態
	long			m_Time;		//	処理時間
	long			m_Count;	//	カウンタ

	WIPE_FADE_TBL	m_OldBg;		//	フェード構造体
	WIPE_FADE_TBL	m_NewBg;		//	フェード構造体

public:
	CScriptWipeEngine();
	CScriptWipeEngine(long TexNo, long PatNo, long PolNo );
	~CScriptWipeEngine();
	void	Init( long WipeNo, long Time, char *FileName );
	long	Loop( void );

private:
	void	SetCrossFade( char *FileName );
	void	LoopCrossFade( void );
};

#endif	//	__ScriptWipeEngine__