/******************************************************************************/
//	DirectX9ライブラリ
//		インプット
/******************************************************************************/

#ifndef	__Dx9LibInput_H__
#define	__Dx9LibInput_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//
enum
{
	//	マウス
	MOUSE_LEFT = 0,	//	左
	MOUSE_RIGHT,	//	右
	MOUSE_CENTER,	//	センター

	//	ジョイスティック
	JOY_UP = 0,		//	上
	JOY_DOWN,		//	下
	JOY_LEFT,		//	左
	JOY_RIGHT,		//	右
	JOY_BUT1,		//	ボタン１
	JOY_BUT2,		//	ボタン２
	JOY_BUT3,		//	ボタン３
	JOY_BUT4,		//	ボタン４
	JOY_BUT5,		//	ボタン５
	JOY_BUT6,		//	ボタン６
	JOY_BUT7,		//	ボタン７
	JOY_BUT8,		//	ボタン８
	JOY_BUT9,		//	ボタン９
	JOY_BUT10,		//	ボタン１０
	JOY_BUT11,		//	ボタン１１
	JOY_BUT12,		//	ボタン１２
	JOY_BUT_MAX,	//	最大値
};

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	マウス用構造体
typedef	struct
{
	long	PosX,PosY;					//	座標
	long	PosXBk,PosYBk;				//	バックアップ座標
	long	MoveX,MoveY;				//	移動量
	UChar	Trg[3];						//	トリガ
	UChar	Push[3];					//	プッシュ
	long	Count[3];					//	プッシュカウント
	UChar	WheelUp,WheelDown;			//	ホイール
	UChar	WheelUpCt,WheelDownCt;		//	ホイールカウント
	RECT	Rect;						//	領域
}ST_MouseInfo;

////////////////////////////////////////
//	キーボード用構造体
typedef	struct
{
	UChar	Flag;						//	フラグ
	UChar	Trg[256];					//	トリガ
	UChar	Push[256];					//	プッシュ
	long	Count[256];					//	プッシュカウント
}ST_KeybordInfo;

////////////////////////////////////////
//	ジョイスティック用構造体
typedef	struct
{
	UChar	Use;						//	使用フラグ
	UChar	Trg[BUTTON_MAX];			//	トリガ
	UChar	Push[BUTTON_MAX];			//	プッシュ
	long	Count[BUTTON_MAX];			//	プッシュカウント
	long	ButtonMax;
}ST_JoystickInfo;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CDx9LibInput
{
private:
	LPDIRECTINPUTDEVICE8	m_DIDevMs;				//	マウス用デバイス
	LPDIRECTINPUTDEVICE8	m_DIDevKey;				//	キーボード用デバイス
	POINT					m_MousePoint;			//	マウスの座標
	DIDEVCAPS				m_DICaps;				//	デバイス能力
	long					m_JoyFlag;				//	ジョイスティックの使用フラグ

public:
	CDx9LibInput();
	~CDx9LibInput();
	bool	Initialize( HINSTANCE hInst );
	void	InputAcquire( void );
	void	InputUnacquire( void );
	void	GetInputData( short Active );
	long	GetJoyNo( void );
	void	SetJoyNo( long JoyNo );
	long	GetJoyUseFlag( void );
	void	Release( void );

private:
	BOOL	InitMouseDevice( void );
	BOOL	InitKeyboardDevice( void );
	BOOL	InitJoystickDevice( void );

	void	InitMouse( void );
	void	InitKeybord( void );
	void	InitJoystick( void );

	BOOL	GetMouseData( short Active );
	BOOL	GetKeybordData( short Active );
	BOOL	GetJoystickData( short Active );
};

#endif	//	__Dx9LibInput_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	ST_MouseInfo	MOUSE;			//	マウス
extern	ST_KeybordInfo	KEY;			//	キーボード
extern	ST_JoystickInfo JOY[JOY_MAX];	//	ジョイスティック
