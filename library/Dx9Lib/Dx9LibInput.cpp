/******************************************************************************/
//	DirectX9ライブラリ
//		インプット
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
LPDIRECTINPUT8			g_DInput;				//	インプットオブジェクト
LPDIRECTINPUTDEVICE8	g_DIDevJoy[JOY_MAX];	//	ジョイスティック用デバイス
ST_MouseInfo			MOUSE;					//	マウス
ST_KeybordInfo			KEY;					//	キーボード
ST_JoystickInfo			JOY[JOY_MAX];			//	ジョイスティック
long					g_JoyNo;				//	ジョイスティック列挙用



/******************************************************************************/
//	名前	：	ジョイスティック列挙
//	説明	：	ジョイスティックを列挙する
//	戻り値	：	BOOL	処理結果	DIENUM_CONTINUE(際検索)
//									DIENUM_STOP(停止)
//	引数	：	[IN]LPDIDEVICEINSTANCE	pdidInstance	インプットデバイス
//	備考	：	なし
/******************************************************************************/
BOOL CALLBACK	EnumJoysticksCallback( LPDIDEVICEINSTANCE pdidInstance, LPVOID )
{
	HRESULT	hr;

	//	列挙したジョイスティックへのインターフェースを取得
	hr = g_DInput->CreateDevice( pdidInstance->guidInstance, &g_DIDevJoy[g_JoyNo], NULL );
	if( hr == DI_OK )
	{
		JOY[ g_JoyNo ].Use = ON;
	}

	g_JoyNo ++;
	if( g_JoyNo >= JOY_MAX )
	{
		return DIENUM_STOP;
	}
	else
	{
		return DIENUM_CONTINUE;
	}
}

/******************************************************************************/
//	名前	：	コンストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibInput::CDx9LibInput()
{
	g_DInput = NULL;
	m_DIDevMs = NULL;
	m_DIDevKey = NULL;
	for( int i=0; i<JOY_MAX; i++)
	{
		g_DIDevJoy[i] = NULL;
	}
}

/******************************************************************************/
//	名前	：	デストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibInput::~CDx9LibInput()
{
	Release();
}

/******************************************************************************/
//	名前	：	インプットの初期化
//	説明	：	インプットデバイスの初期化を行う
//	戻り値	：	bool	処理結果	true(成功)
//									false(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
bool	CDx9LibInput::Initialize( HINSTANCE hInst )
{
	LOG_WRITE( TypeDebugLog, "DirectInputの初期化開始" );
	HRESULT	hr;

	//	オブジェクトの作成
	hr = DirectInput8Create(
		hInst, DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void **)&g_DInput,
		NULL );
	if( FAILED(hr) )
	{
		//	初期化失敗
		LOG_WRITE( TypeErrLog, "DirectInput8オブジェクトの作成に失敗" );
		DXTRACE_ERR( "DirectInput8オブジェクトの作成に失敗", hr );
		return	false;
	}

	////////////////////////////////////
	//	各データの初期化
	this->InitMouse();
	this->InitKeybord();
	this->InitJoystick();

	////////////////////////////////////
	//	マウスデバイスの初期化
	if( !this->InitMouseDevice() )
	{
		return	false;		//	失敗
	}

	////////////////////////////////////
	//	キーボードデバイスの初期化
	if( !this->InitKeyboardDevice() )
	{
		return	false;		//	失敗
	}

	////////////////////////////////////
	//	ジョイスティックデバイスの初期化
	if( JOY_USE )
	{
		this->InitJoystickDevice();
	}

	////////////////////////////////////
	//	マウス表示設定
	ShowCursor( g_ShowCursor );

	LOG_WRITE( TypeDebugLog, "DirectInputの初期化終了" );

	//	初期化成功
	return	true;
}

/******************************************************************************/
//	名前	：	入力開始
//	説明	：	入力を開始する
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibInput::InputAcquire( void )
{
	m_DIDevKey->Acquire();
	m_DIDevMs->Acquire();

	if( m_JoyFlag )
	{
		for( int i=0; i<JOY_MAX; i++ )
		{
			if( JOY[i].Use )
			{
				g_DIDevJoy[i]->Acquire();
			}
		}
	}
}

/******************************************************************************/
//	名前	：	入力終了
//	説明	：	入力を終了する
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibInput::InputUnacquire( void )
{
	if( m_DIDevMs != NULL )
	{
		m_DIDevMs->Unacquire();
	}

	if( m_DIDevKey != NULL )
	{
		m_DIDevKey->Unacquire();
	}

	if( m_JoyFlag )
	{
		for( int i=0; i<JOY_MAX; i++ )
		{
			if( JOY[ i ].Use && g_DIDevJoy[i] != NULL )
			{
				g_DIDevJoy[i]->Unacquire();
			}
		}
	}
}

/******************************************************************************/
//	名前	：	インプットデータ取得
//	説明	：	インプットデータを取得する
//	戻り値	：	なし
//	引数	：	[IN]short	Active	アクティブフラグ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibInput::GetInputData( short Active )
{
	this->GetMouseData( Active );
	this->GetKeybordData( Active );
	if( m_JoyFlag )
	{
		this->GetJoystickData( Active );
	}
}

/******************************************************************************/
//	名前	：	ジョイスティック数取得
//	説明	：	ジョイスティック数を取得する
//	戻り値	：	long	ジョイスティック数
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
long CDx9LibInput::GetJoyNo( void )
{
	return g_JoyNo;
}

/******************************************************************************/
//	名前	：	ジョイスティック数設定
//	説明	：	ジョイスティック数を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	JouNo	ジョイスティック数
//	備考	：	なし
/******************************************************************************/
void CDx9LibInput::SetJoyNo( long JoyNo )
{
	g_JoyNo = JoyNo;
}

/******************************************************************************/
//	名前	：	ジョイスティック使用フラグ取得
//	説明	：	ジョイスティック使用フラグを取得する
//	戻り値	：	long	ジョイスティック使用フラグ
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
long CDx9LibInput::GetJoyUseFlag( void )
{
	return this->m_JoyFlag;
}

/******************************************************************************/
//	名前	：	インプットデバイス開放
//	説明	：	デバイスを開放する
//	戻り値	：	なし
//	引数	：	なし
//	備考	；	なし
/******************************************************************************/
void	CDx9LibInput::Release( void )
{
	this->InputUnacquire();

	//	ジョイスティック用デバイスの解放
	for( int i=0; i<JOY_MAX; i++ )
	{
		SAFE_RELEASE(g_DIDevJoy[i]);
	}

	//	キーボード用デバイスの解放
	SAFE_RELEASE(m_DIDevKey);

	//	マウス用デバイスの解放
	SAFE_RELEASE(m_DIDevMs);

	//	インプットオブジェクトの解放
	SAFE_RELEASE(g_DInput);
}

/******************************************************************************/
//	名前	：	マウス用デバイス初期化
//	説明	：	マウス用デバイスの初期化を行う
//	戻り値	：	BOOL	処理結果	TRUE(成功)
//									FALSE(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
BOOL	CDx9LibInput::InitMouseDevice( void )
{
	HRESULT	hr;

	//	デバイス・オブジェクトの作成
	hr = g_DInput->CreateDevice( GUID_SysMouse, &m_DIDevMs, NULL );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "マウスのデバイス・オブジェクトの作成に失敗" );
		DXTRACE_ERR( "マウスのデバイス・オブジェクトの作成に失敗", hr );
		return	FALSE;
	}

	//	データフォーマットの設定
	hr = m_DIDevMs->SetDataFormat( &c_dfDIMouse2 );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "マウスのデータフォーマットに失敗" );
		DXTRACE_ERR( "マウスのデータフォーマットに失敗", hr );
		return	FALSE;
	}

	//	モードの設定
	hr = m_DIDevMs->SetCooperativeLevel( g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "マウスのモードの設定に失敗" );
		DXTRACE_ERR( "マウスのモードの設定に失敗", hr );
		return	FALSE;
	}

	//	軸モードの設定
	DIPROPDWORD	dipropMs;
	dipropMs.diph.dwSize		= sizeof( dipropMs );
	dipropMs.diph.dwHeaderSize	= sizeof( dipropMs.diph );
	dipropMs.diph.dwObj			= 0;
	dipropMs.diph.dwHow			= DIPH_DEVICE;
	dipropMs.dwData				= DIPROPAXISMODE_REL;

	//	デバイスのプロパティの設定
	hr = m_DIDevMs->SetProperty( DIPROP_AXISMODE, &dipropMs.diph );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "マウスの軸モードの設定に失敗" );
		DXTRACE_ERR( "マウスの軸モードの設定に失敗",hr );
		return	FALSE;
	}

	//	バッファリング・データを取得
	dipropMs.dwData = DIDEVICE_BUFFERSIZE;

	//	デバイスのプロパティの設定
	hr = m_DIDevMs->SetProperty( DIPROP_BUFFERSIZE, &dipropMs.diph );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "マウスのバッファ・サイズの設定に失敗" );
		DXTRACE_ERR( "マウスのバッファ・サイズの設定に失敗", hr );
		return	FALSE;
	}

	return	TRUE;
}

/******************************************************************************/
//	名前	：	キーボード用デバイス初期化
//	説明	：	キーボード用デバイスの初期化を行う
//	戻り値	：	BOOL	処理結果	TRUE(成功)
//									FALSE(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
BOOL	CDx9LibInput::InitKeyboardDevice( void )
{
	HRESULT	hr;

	//	デバイス・オブジェクトを作成
	hr = g_DInput->CreateDevice( GUID_SysKeyboard, &m_DIDevKey, NULL );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "キーボードのデバイス・オブジェクトの作成に失敗" );
		DXTRACE_ERR( "キーボードのデバイス・オブジェクトの作成に失敗", hr );
		return	FALSE;
	}

	//	データ形式を設定
	hr = m_DIDevKey->SetDataFormat( &c_dfDIKeyboard );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "キーボードのデータフォーマットに失敗" );
		DXTRACE_ERR( "キーボードのデータフォーマットに失敗", hr );
		return	FALSE;
	}

	//	モードを設定
	hr = m_DIDevKey->SetCooperativeLevel( g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "キーボードのモード設定に失敗" );
		DXTRACE_ERR( "キーボードのモード設定に失敗", hr );
		return	FALSE;
	}

	//	バッファリングを設定
	DIPROPDWORD	dipropKey;
	dipropKey.diph.dwSize		= sizeof( dipropKey );
	dipropKey.diph.dwHeaderSize	= sizeof( dipropKey.diph );
	dipropKey.diph.dwObj		= 0;
	dipropKey.diph.dwHow		= DIPH_DEVICE;
	dipropKey.dwData			= DIDEVICE_BUFFERSIZE;

	//	デバイスのプロパティの設定
	hr = m_DIDevKey->SetProperty( DIPROP_BUFFERSIZE, &dipropKey.diph );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "キーボードのバッファリング設定に失敗" );
		DXTRACE_ERR( "キーボードのバッファリング設定に失敗", hr );
		return	FALSE;
	}

	return TRUE;
}

/******************************************************************************/
//	名前	：	ジョイスティック用デバイス初期化
//	説明	：	ジョイスティック用デバイスの初期化を行う
//	戻り値	：	BOOL	処理結果	TRUE(成功)
//									FALSE(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
BOOL	CDx9LibInput::InitJoystickDevice( void )
{
	HRESULT	hr;
	DIPROPRANGE	dipropJoy;
	long	i;

	g_JoyNo = 0;
	for( i=0; i<JOY_MAX; i++ )
	{
		JOY[ i ].Use = OFF;
		g_DIDevJoy[i] = NULL;
	}

	//　デバイスを列挙して作成
	g_DInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY );

	//	設定
	for( i=0; i<JOY_MAX; i++ )
	{
		if( JOY[ i ].Use == OFF )
		{
			continue;
		}

		JOY[ i ].Use = OFF;

		//	データ形式を設定
		hr = g_DIDevJoy[i]->SetDataFormat( &c_dfDIJoystick );
		if( hr != DI_OK )
		{
			//	失敗
			SAFE_RELEASE( g_DIDevJoy[ i ] );
			continue;
		}

		//	モードの設定
		hr = g_DIDevJoy[i]->SetCooperativeLevel(
			g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
		if( hr != DI_OK )
		{
			SAFE_RELEASE(g_DIDevJoy[i]);
			continue;
		}

		//	軸モードを設定
		dipropJoy.diph.dwSize		= sizeof( dipropJoy );
		dipropJoy.diph.dwHeaderSize	= sizeof( dipropJoy.diph );
		dipropJoy.diph.dwObj		= DIJOFS_X;
		dipropJoy.diph.dwHow		= DIPH_BYOFFSET;
		dipropJoy.lMin				= -256;
		dipropJoy.lMax				= 256;

		//	デバイスのプロパティの設定
		hr = g_DIDevJoy[i]->SetProperty( DIPROP_RANGE, &dipropJoy.diph );
		dipropJoy.diph.dwObj = DIJOFS_Y;
		hr |= g_DIDevJoy[i]->SetProperty( DIPROP_RANGE, &dipropJoy.diph );
		if( hr != DI_OK )
		{
			SAFE_RELEASE(g_DIDevJoy[i]);
			continue;
		}

		//	ボタン数取得
		m_DICaps.dwSize = sizeof(DIDEVCAPS);
		hr = g_DIDevJoy[i]->GetCapabilities(&m_DICaps);
		if( FAILED(hr) )
		{
			SAFE_RELEASE(g_DIDevJoy[i]);
			continue;
		}
		JOY[ i ].ButtonMax = m_DICaps.dwButtons - 4;

		//	入力開始
		hr = g_DIDevJoy[i]->Acquire();
		JOY[ i ].Use = ON;
	}

	if( g_JoyNo == 0 )
	{
		return	FALSE;
	}

	m_JoyFlag = ON;
	return	TRUE;
}

/******************************************************************************/
//	名前	：	マウス用構造体初期化
//	説明	：	マウス用構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibInput::InitMouse( void )
{
	//	マウスの座標
	GetCursorPos( &m_MousePoint );
	MOUSE.PosX = (short)m_MousePoint.x;
	MOUSE.PosY = (short)m_MousePoint.y;

	//	バックアップ座標
	MOUSE.PosXBk = MOUSE.PosX;
	MOUSE.PosYBk = MOUSE.PosY;

	//	移動量
	MOUSE.MoveX = 0;
	MOUSE.MoveY = 0;

	//	ボタンの初期化
	for( int i=0; i<3; i++ )
	{
		MOUSE.Trg[ i ] = OFF;
	}
}

/******************************************************************************/
//	名前	：	キーボード用構造体初期化
//	説明	：	キーボード用構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibInput::InitKeybord( void )
{
	KEY.Flag = OFF;
	for( int i=0; i<256; i++ )
	{
		KEY.Trg[ i ] = OFF;
		KEY.Push[ i ] = OFF;
		KEY.Count[ i ] = OFF;
	}
}

/******************************************************************************/
//	名前	：	ジョイスティック用構造体初期化
//	説明	：	ジョイスティック用構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibInput::InitJoystick( void )
{
	for( int no=0; no<JOY_MAX; no++ )
	{
		for( int i=0; i<BUTTON_MAX; i++ )
		{
			JOY[ no ].Trg[ i ] = OFF;
			JOY[ no ].Push[ i ] = OFF;
			JOY[ no ].Count[ i ] = OFF;
		}
	}
}

/******************************************************************************/
//	名前	：	マウスデータ取得
//	説明	：	マウスデータの取得を行う
//	戻り値	：	BOOL	処理結果	TRUE(成功)
//									FALSE(失敗)
//	引数	：	[IN]short	Active	アクティブフラグ
//	備考	：	なし
/******************************************************************************/
BOOL	CDx9LibInput::GetMouseData( short Active )
{
	//	各座標のバックアップ
	MOUSE.PosXBk = MOUSE.PosX;
	MOUSE.PosYBk = MOUSE.PosY;

	if( m_DIDevMs != NULL )
	{
		HRESULT	hr;
		DIMOUSESTATE2	dims;

		//	デバイスから直接データの取得
		hr = m_DIDevMs->GetDeviceState( sizeof(DIMOUSESTATE2), &dims );
		if( SUCCEEDED(hr) )
		{
			//	データの取得に成功
			GetCursorPos( &m_MousePoint );

			//	座標格納
			if( g_ScreenMode )
			{
				MOUSE.PosX = m_MousePoint.x;
				MOUSE.PosY = m_MousePoint.y;
			}
			else
			{
				//	スクリーン座標で取得
				GetWindowRect( g_hWnd, &MOUSE.Rect );

				//	座標格納
				MOUSE.PosX = m_MousePoint.x - MOUSE.Rect.left
							- GetSystemMetrics( SM_CYDLGFRAME );
				MOUSE.PosY = m_MousePoint.y - MOUSE.Rect.top
							- GetSystemMetrics( SM_CYDLGFRAME )
							- GetSystemMetrics( SM_CYCAPTION );
			}

			//	各座標の移動量取得
			MOUSE.MoveX = MOUSE.PosX - MOUSE.PosXBk;
			MOUSE.MoveY = MOUSE.PosY - MOUSE.PosYBk;

			for( int i=0; i<3; i++ )
			{
				/////////////////////////////////////
				//	マウスのボタンが押された時
				if( dims.rgbButtons[i] & 0x80 )
				{
					MOUSE.Trg[ i ]   = ON;
					MOUSE.Push[ i ]  = ON;
					MOUSE.Count[ i ] ++;
				}
				else
				{
					MOUSE.Trg[ i ]   = OFF;
					MOUSE.Push[ i ]  = OFF;
					MOUSE.Count[ i ] = 0;
				}

				if( MOUSE.Count[ i ] > 1 && MOUSE.Push[ i ] )
				{
					MOUSE.Trg[ i ] = OFF;
				}
			}

			/////////////////////////////////////
			//	ホイールが上にスクロールされた時
			if( dims.lZ > 10 )
			{
				MOUSE.WheelUp = ON;
				MOUSE.WheelUpCt ++;
				if( !MOUSE.WheelUpCt )
				{
					MOUSE.WheelUp = OFF;
				}
			}
			//	スクロール終了
			else
			{
				MOUSE.WheelUp = MOUSE.WheelUpCt = OFF;
			}

			/////////////////////////////////////
			//	ホイールが下にスクロールされた時
			if( dims.lZ < -10 )
			{
				MOUSE.WheelDown = ON;
				MOUSE.WheelDownCt ++;
				if( !MOUSE.WheelDownCt )
				{
					MOUSE.WheelDown = OFF;
				}
			}
			//	スクロール終了
			else
			{
				MOUSE.WheelDown = MOUSE.WheelDownCt = OFF;
			}
		}
		else if( Active && hr == DIERR_INPUTLOST )
		{
			//	入力開始
			m_DIDevMs->Acquire();
		}
	}
	return	TRUE;
}

/******************************************************************************/
//	名前	：	キーボードデータ取得
//	説明	：	キーボードデータの取得を行う
//	戻り値	：	BOOL	処理結果	TRUE(成功)
//									FALSE(失敗)
//	引数	：	[IN]short	Active	アクティブフラグ
//	備考	：	なし
/******************************************************************************/
BOOL	CDx9LibInput::GetKeybordData( short Active )
{
	HRESULT	hr;

	BYTE	KeyState[256];
	int	i;

	//	デバイスから直接データの取得
	hr = m_DIDevKey->GetDeviceState( 256, KeyState );
	if( SUCCEEDED(hr) )
	{
		//	データの取得に成功
		for( i=0; i<256; i++ )
		{
			if( KeyState[i] & 0x80 )
			{
				/////////////////////////////////////
				//	キーが押された時
				KEY.Flag	  = ON;
				KEY.Trg[ i ]  = ON;
				KEY.Push[ i ] = ON;
				KEY.Count[ i ] ++;
			}
			else
			{
				/////////////////////////////////////
				//	キーが離された時
				KEY.Flag	  = OFF;
				KEY.Trg[ i ]  = OFF;
				KEY.Push[ i ] = OFF;
				KEY.Count[ i ] = OFF;
			}

			if( KEY.Count[ i ] > 1 && KEY.Push[ i ] )
			{
				KEY.Trg[ i ] = OFF;
			}
		}
	}
	else if( Active && hr == DIERR_INPUTLOST )
	{
		//	入力開始
		m_DIDevKey->Acquire();
	}
	return	TRUE;
}

/******************************************************************************/
//	名前	：	ジョイスティックデータ取得
//	説明	：	ジョイスティックデータ取得を行う
//	戻り値	：	BOOL	処理結果	TRUE(成功)
//									FALSE(失敗)
//	引数	：	[IN]short	Active	アクティブフラグ
//	備考	：	なし
/******************************************************************************/
BOOL	CDx9LibInput::GetJoystickData( short Active )
{
	HRESULT	hr;
	short	i,no;
	DIJOYSTATE2	dijs;

	for( no=0; no<JOY_MAX; no++ )
	{
		if( JOY[ no ].Use == OFF )
		{
			continue;
		}

		//	現在の状態を取得
		hr = g_DIDevJoy[no]->Poll();
		if( FAILED(hr) )
		{
			//	入力開始
			if( Active && hr == DIERR_INPUTLOST )
			{
				hr = g_DIDevJoy[no]->Acquire();
			}
		}

		//	デバイスの直接データを取得する
		hr = g_DIDevJoy[no]->GetDeviceState( sizeof(DIJOYSTATE), &dijs );
		if( hr == DI_OK )
		{
			////////////////////////////////////////////////////////////////////
			//	プッシュ

			//////////////////////////////////
			//	上
			if( dijs.lY < -128 )
			{
				JOY[ no ].Trg[ JOY_UP ] = ON;
				JOY[ no ].Push[ JOY_UP ] = ON;
				JOY[ no ].Count[ JOY_UP ] ++;
			}
			else
			{
				//	離された時
				JOY[ no ].Trg[ JOY_UP ] = OFF;
				JOY[ no ].Push[ JOY_UP ] = OFF;
				JOY[ no ].Count[ JOY_UP ] = 0;
			}

			//////////////////////////////////
			//	下
			if( dijs.lY > 0 )
			{
				JOY[ no ].Trg[ JOY_DOWN ] = ON;
				JOY[ no ].Push[ JOY_DOWN ] = ON;
				JOY[ no ].Count[ JOY_DOWN ] ++;
			}
			else
			{
				//	離された時
				JOY[ no ].Trg[ JOY_DOWN ] = OFF;
				JOY[ no ].Push[ JOY_DOWN ] = OFF;
				JOY[ no ].Count[ JOY_DOWN ] = 0;
			}

			//////////////////////////////////
			//	左
			if( dijs.lX < -128 )
			{
				JOY[ no ].Trg[ JOY_LEFT ] = ON;
				JOY[ no ].Push[ JOY_LEFT ] = ON;
				JOY[ no ].Count[ JOY_LEFT ] ++;
			}
			else
			{
				//	離された時
				JOY[ no ].Trg[ JOY_LEFT ] = OFF;
				JOY[ no ].Push[ JOY_LEFT ] = OFF;
				JOY[ no ].Count[ JOY_LEFT ] = 0;
			}

			//////////////////////////////////
			//	右
			if( dijs.lX > 128 )
			{
				JOY[ no ].Trg[ JOY_RIGHT ] = ON;
				JOY[ no ].Push[ JOY_RIGHT ] = ON;
				JOY[ no ].Count[ JOY_RIGHT ] ++;
			}
			else
			{
				//	離された時
				JOY[ no ].Trg[ JOY_RIGHT ] = OFF;
				JOY[ no ].Push[ JOY_RIGHT ] = OFF;
				JOY[ no ].Count[ JOY_RIGHT ] = 0;
			}

			////////////////////////////////////////////////////////////////////
			//	トリガ
			for( i=0; i<4; i++ )
			{
				if( (JOY[ no ].Count[ i ] > 1) && JOY[ no ].Push[ i ] )
				{
					JOY[ no ].Trg[ i ] = OFF;
				}
				else
				{
					JOY[ no ].Push[ i ] = OFF;
				}
			}

			//////////////////////////////////
			//	各ボタン
			for( i=0; i<BUTTON_MAX-4; i++ )
			{
				//	各ボタンが押された時
				if( dijs.rgbButtons[i] & 0x80 )
				{
					JOY[ no ].Trg[ i+4 ] = ON;
					JOY[ no ].Push[ i+4 ] = ON;
					JOY[ no ].Count[ i+4 ] ++;
				}
				else
				{
					//	各ボタンが離された時
					JOY[ no ].Trg[ i+4 ] = OFF;
					JOY[ no ].Push[ i+4 ] = OFF;
					JOY[ no ].Count[ i+4 ] = OFF;
				}

				if( (JOY[ no ].Count[ i+4 ] > 1) && JOY[ no ].Push[ i+4 ] )
				{
					JOY[ no ].Trg[ i+4 ] = OFF;
				}
				else
				{
					JOY[ no ].Push[ i+4 ] = OFF;
				}
			}
		}
		else if( Active && hr == DIERR_INPUTLOST )
		{
			//	入力開始
			g_DIDevJoy[ no ]->Acquire();
		}
	}
	return	TRUE;
}
