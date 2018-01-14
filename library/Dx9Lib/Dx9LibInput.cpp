/******************************************************************************/
//	DirectX9���C�u����
//		�C���v�b�g
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
LPDIRECTINPUT8			g_DInput;				//	�C���v�b�g�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8	g_DIDevJoy[JOY_MAX];	//	�W���C�X�e�B�b�N�p�f�o�C�X
ST_MouseInfo			MOUSE;					//	�}�E�X
ST_KeybordInfo			KEY;					//	�L�[�{�[�h
ST_JoystickInfo			JOY[JOY_MAX];			//	�W���C�X�e�B�b�N
long					g_JoyNo;				//	�W���C�X�e�B�b�N�񋓗p



/******************************************************************************/
//	���O	�F	�W���C�X�e�B�b�N��
//	����	�F	�W���C�X�e�B�b�N��񋓂���
//	�߂�l	�F	BOOL	��������	DIENUM_CONTINUE(�ی���)
//									DIENUM_STOP(��~)
//	����	�F	[IN]LPDIDEVICEINSTANCE	pdidInstance	�C���v�b�g�f�o�C�X
//	���l	�F	�Ȃ�
/******************************************************************************/
BOOL CALLBACK	EnumJoysticksCallback( LPDIDEVICEINSTANCE pdidInstance, LPVOID )
{
	HRESULT	hr;

	//	�񋓂����W���C�X�e�B�b�N�ւ̃C���^�[�t�F�[�X���擾
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
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
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
//	���O	�F	�f�X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibInput::~CDx9LibInput()
{
	Release();
}

/******************************************************************************/
//	���O	�F	�C���v�b�g�̏�����
//	����	�F	�C���v�b�g�f�o�C�X�̏��������s��
//	�߂�l	�F	bool	��������	true(����)
//									false(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
bool	CDx9LibInput::Initialize( HINSTANCE hInst )
{
	LOG_WRITE( TypeDebugLog, "DirectInput�̏������J�n" );
	HRESULT	hr;

	//	�I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create(
		hInst, DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void **)&g_DInput,
		NULL );
	if( FAILED(hr) )
	{
		//	���������s
		LOG_WRITE( TypeErrLog, "DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s" );
		DXTRACE_ERR( "DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s", hr );
		return	false;
	}

	////////////////////////////////////
	//	�e�f�[�^�̏�����
	this->InitMouse();
	this->InitKeybord();
	this->InitJoystick();

	////////////////////////////////////
	//	�}�E�X�f�o�C�X�̏�����
	if( !this->InitMouseDevice() )
	{
		return	false;		//	���s
	}

	////////////////////////////////////
	//	�L�[�{�[�h�f�o�C�X�̏�����
	if( !this->InitKeyboardDevice() )
	{
		return	false;		//	���s
	}

	////////////////////////////////////
	//	�W���C�X�e�B�b�N�f�o�C�X�̏�����
	if( JOY_USE )
	{
		this->InitJoystickDevice();
	}

	////////////////////////////////////
	//	�}�E�X�\���ݒ�
	ShowCursor( g_ShowCursor );

	LOG_WRITE( TypeDebugLog, "DirectInput�̏������I��" );

	//	����������
	return	true;
}

/******************************************************************************/
//	���O	�F	���͊J�n
//	����	�F	���͂��J�n����
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
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
//	���O	�F	���͏I��
//	����	�F	���͂��I������
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
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
//	���O	�F	�C���v�b�g�f�[�^�擾
//	����	�F	�C���v�b�g�f�[�^���擾����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]short	Active	�A�N�e�B�u�t���O
//	���l	�F	�Ȃ�
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
//	���O	�F	�W���C�X�e�B�b�N���擾
//	����	�F	�W���C�X�e�B�b�N�����擾����
//	�߂�l	�F	long	�W���C�X�e�B�b�N��
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
long CDx9LibInput::GetJoyNo( void )
{
	return g_JoyNo;
}

/******************************************************************************/
//	���O	�F	�W���C�X�e�B�b�N���ݒ�
//	����	�F	�W���C�X�e�B�b�N����ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	JouNo	�W���C�X�e�B�b�N��
//	���l	�F	�Ȃ�
/******************************************************************************/
void CDx9LibInput::SetJoyNo( long JoyNo )
{
	g_JoyNo = JoyNo;
}

/******************************************************************************/
//	���O	�F	�W���C�X�e�B�b�N�g�p�t���O�擾
//	����	�F	�W���C�X�e�B�b�N�g�p�t���O���擾����
//	�߂�l	�F	long	�W���C�X�e�B�b�N�g�p�t���O
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
long CDx9LibInput::GetJoyUseFlag( void )
{
	return this->m_JoyFlag;
}

/******************************************************************************/
//	���O	�F	�C���v�b�g�f�o�C�X�J��
//	����	�F	�f�o�C�X���J������
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�G	�Ȃ�
/******************************************************************************/
void	CDx9LibInput::Release( void )
{
	this->InputUnacquire();

	//	�W���C�X�e�B�b�N�p�f�o�C�X�̉��
	for( int i=0; i<JOY_MAX; i++ )
	{
		SAFE_RELEASE(g_DIDevJoy[i]);
	}

	//	�L�[�{�[�h�p�f�o�C�X�̉��
	SAFE_RELEASE(m_DIDevKey);

	//	�}�E�X�p�f�o�C�X�̉��
	SAFE_RELEASE(m_DIDevMs);

	//	�C���v�b�g�I�u�W�F�N�g�̉��
	SAFE_RELEASE(g_DInput);
}

/******************************************************************************/
//	���O	�F	�}�E�X�p�f�o�C�X������
//	����	�F	�}�E�X�p�f�o�C�X�̏��������s��
//	�߂�l	�F	BOOL	��������	TRUE(����)
//									FALSE(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
BOOL	CDx9LibInput::InitMouseDevice( void )
{
	HRESULT	hr;

	//	�f�o�C�X�E�I�u�W�F�N�g�̍쐬
	hr = g_DInput->CreateDevice( GUID_SysMouse, &m_DIDevMs, NULL );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�}�E�X�̃f�o�C�X�E�I�u�W�F�N�g�̍쐬�Ɏ��s" );
		DXTRACE_ERR( "�}�E�X�̃f�o�C�X�E�I�u�W�F�N�g�̍쐬�Ɏ��s", hr );
		return	FALSE;
	}

	//	�f�[�^�t�H�[�}�b�g�̐ݒ�
	hr = m_DIDevMs->SetDataFormat( &c_dfDIMouse2 );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�}�E�X�̃f�[�^�t�H�[�}�b�g�Ɏ��s" );
		DXTRACE_ERR( "�}�E�X�̃f�[�^�t�H�[�}�b�g�Ɏ��s", hr );
		return	FALSE;
	}

	//	���[�h�̐ݒ�
	hr = m_DIDevMs->SetCooperativeLevel( g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�}�E�X�̃��[�h�̐ݒ�Ɏ��s" );
		DXTRACE_ERR( "�}�E�X�̃��[�h�̐ݒ�Ɏ��s", hr );
		return	FALSE;
	}

	//	�����[�h�̐ݒ�
	DIPROPDWORD	dipropMs;
	dipropMs.diph.dwSize		= sizeof( dipropMs );
	dipropMs.diph.dwHeaderSize	= sizeof( dipropMs.diph );
	dipropMs.diph.dwObj			= 0;
	dipropMs.diph.dwHow			= DIPH_DEVICE;
	dipropMs.dwData				= DIPROPAXISMODE_REL;

	//	�f�o�C�X�̃v���p�e�B�̐ݒ�
	hr = m_DIDevMs->SetProperty( DIPROP_AXISMODE, &dipropMs.diph );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�}�E�X�̎����[�h�̐ݒ�Ɏ��s" );
		DXTRACE_ERR( "�}�E�X�̎����[�h�̐ݒ�Ɏ��s",hr );
		return	FALSE;
	}

	//	�o�b�t�@�����O�E�f�[�^���擾
	dipropMs.dwData = DIDEVICE_BUFFERSIZE;

	//	�f�o�C�X�̃v���p�e�B�̐ݒ�
	hr = m_DIDevMs->SetProperty( DIPROP_BUFFERSIZE, &dipropMs.diph );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�}�E�X�̃o�b�t�@�E�T�C�Y�̐ݒ�Ɏ��s" );
		DXTRACE_ERR( "�}�E�X�̃o�b�t�@�E�T�C�Y�̐ݒ�Ɏ��s", hr );
		return	FALSE;
	}

	return	TRUE;
}

/******************************************************************************/
//	���O	�F	�L�[�{�[�h�p�f�o�C�X������
//	����	�F	�L�[�{�[�h�p�f�o�C�X�̏��������s��
//	�߂�l	�F	BOOL	��������	TRUE(����)
//									FALSE(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
BOOL	CDx9LibInput::InitKeyboardDevice( void )
{
	HRESULT	hr;

	//	�f�o�C�X�E�I�u�W�F�N�g���쐬
	hr = g_DInput->CreateDevice( GUID_SysKeyboard, &m_DIDevKey, NULL );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�L�[�{�[�h�̃f�o�C�X�E�I�u�W�F�N�g�̍쐬�Ɏ��s" );
		DXTRACE_ERR( "�L�[�{�[�h�̃f�o�C�X�E�I�u�W�F�N�g�̍쐬�Ɏ��s", hr );
		return	FALSE;
	}

	//	�f�[�^�`����ݒ�
	hr = m_DIDevKey->SetDataFormat( &c_dfDIKeyboard );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g�Ɏ��s" );
		DXTRACE_ERR( "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g�Ɏ��s", hr );
		return	FALSE;
	}

	//	���[�h��ݒ�
	hr = m_DIDevKey->SetCooperativeLevel( g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�L�[�{�[�h�̃��[�h�ݒ�Ɏ��s" );
		DXTRACE_ERR( "�L�[�{�[�h�̃��[�h�ݒ�Ɏ��s", hr );
		return	FALSE;
	}

	//	�o�b�t�@�����O��ݒ�
	DIPROPDWORD	dipropKey;
	dipropKey.diph.dwSize		= sizeof( dipropKey );
	dipropKey.diph.dwHeaderSize	= sizeof( dipropKey.diph );
	dipropKey.diph.dwObj		= 0;
	dipropKey.diph.dwHow		= DIPH_DEVICE;
	dipropKey.dwData			= DIDEVICE_BUFFERSIZE;

	//	�f�o�C�X�̃v���p�e�B�̐ݒ�
	hr = m_DIDevKey->SetProperty( DIPROP_BUFFERSIZE, &dipropKey.diph );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�L�[�{�[�h�̃o�b�t�@�����O�ݒ�Ɏ��s" );
		DXTRACE_ERR( "�L�[�{�[�h�̃o�b�t�@�����O�ݒ�Ɏ��s", hr );
		return	FALSE;
	}

	return TRUE;
}

/******************************************************************************/
//	���O	�F	�W���C�X�e�B�b�N�p�f�o�C�X������
//	����	�F	�W���C�X�e�B�b�N�p�f�o�C�X�̏��������s��
//	�߂�l	�F	BOOL	��������	TRUE(����)
//									FALSE(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
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

	//�@�f�o�C�X��񋓂��č쐬
	g_DInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY );

	//	�ݒ�
	for( i=0; i<JOY_MAX; i++ )
	{
		if( JOY[ i ].Use == OFF )
		{
			continue;
		}

		JOY[ i ].Use = OFF;

		//	�f�[�^�`����ݒ�
		hr = g_DIDevJoy[i]->SetDataFormat( &c_dfDIJoystick );
		if( hr != DI_OK )
		{
			//	���s
			SAFE_RELEASE( g_DIDevJoy[ i ] );
			continue;
		}

		//	���[�h�̐ݒ�
		hr = g_DIDevJoy[i]->SetCooperativeLevel(
			g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
		if( hr != DI_OK )
		{
			SAFE_RELEASE(g_DIDevJoy[i]);
			continue;
		}

		//	�����[�h��ݒ�
		dipropJoy.diph.dwSize		= sizeof( dipropJoy );
		dipropJoy.diph.dwHeaderSize	= sizeof( dipropJoy.diph );
		dipropJoy.diph.dwObj		= DIJOFS_X;
		dipropJoy.diph.dwHow		= DIPH_BYOFFSET;
		dipropJoy.lMin				= -256;
		dipropJoy.lMax				= 256;

		//	�f�o�C�X�̃v���p�e�B�̐ݒ�
		hr = g_DIDevJoy[i]->SetProperty( DIPROP_RANGE, &dipropJoy.diph );
		dipropJoy.diph.dwObj = DIJOFS_Y;
		hr |= g_DIDevJoy[i]->SetProperty( DIPROP_RANGE, &dipropJoy.diph );
		if( hr != DI_OK )
		{
			SAFE_RELEASE(g_DIDevJoy[i]);
			continue;
		}

		//	�{�^�����擾
		m_DICaps.dwSize = sizeof(DIDEVCAPS);
		hr = g_DIDevJoy[i]->GetCapabilities(&m_DICaps);
		if( FAILED(hr) )
		{
			SAFE_RELEASE(g_DIDevJoy[i]);
			continue;
		}
		JOY[ i ].ButtonMax = m_DICaps.dwButtons - 4;

		//	���͊J�n
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
//	���O	�F	�}�E�X�p�\���̏�����
//	����	�F	�}�E�X�p�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibInput::InitMouse( void )
{
	//	�}�E�X�̍��W
	GetCursorPos( &m_MousePoint );
	MOUSE.PosX = (short)m_MousePoint.x;
	MOUSE.PosY = (short)m_MousePoint.y;

	//	�o�b�N�A�b�v���W
	MOUSE.PosXBk = MOUSE.PosX;
	MOUSE.PosYBk = MOUSE.PosY;

	//	�ړ���
	MOUSE.MoveX = 0;
	MOUSE.MoveY = 0;

	//	�{�^���̏�����
	for( int i=0; i<3; i++ )
	{
		MOUSE.Trg[ i ] = OFF;
	}
}

/******************************************************************************/
//	���O	�F	�L�[�{�[�h�p�\���̏�����
//	����	�F	�L�[�{�[�h�p�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
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
//	���O	�F	�W���C�X�e�B�b�N�p�\���̏�����
//	����	�F	�W���C�X�e�B�b�N�p�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
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
//	���O	�F	�}�E�X�f�[�^�擾
//	����	�F	�}�E�X�f�[�^�̎擾���s��
//	�߂�l	�F	BOOL	��������	TRUE(����)
//									FALSE(���s)
//	����	�F	[IN]short	Active	�A�N�e�B�u�t���O
//	���l	�F	�Ȃ�
/******************************************************************************/
BOOL	CDx9LibInput::GetMouseData( short Active )
{
	//	�e���W�̃o�b�N�A�b�v
	MOUSE.PosXBk = MOUSE.PosX;
	MOUSE.PosYBk = MOUSE.PosY;

	if( m_DIDevMs != NULL )
	{
		HRESULT	hr;
		DIMOUSESTATE2	dims;

		//	�f�o�C�X���璼�ڃf�[�^�̎擾
		hr = m_DIDevMs->GetDeviceState( sizeof(DIMOUSESTATE2), &dims );
		if( SUCCEEDED(hr) )
		{
			//	�f�[�^�̎擾�ɐ���
			GetCursorPos( &m_MousePoint );

			//	���W�i�[
			if( g_ScreenMode )
			{
				MOUSE.PosX = m_MousePoint.x;
				MOUSE.PosY = m_MousePoint.y;
			}
			else
			{
				//	�X�N���[�����W�Ŏ擾
				GetWindowRect( g_hWnd, &MOUSE.Rect );

				//	���W�i�[
				MOUSE.PosX = m_MousePoint.x - MOUSE.Rect.left
							- GetSystemMetrics( SM_CYDLGFRAME );
				MOUSE.PosY = m_MousePoint.y - MOUSE.Rect.top
							- GetSystemMetrics( SM_CYDLGFRAME )
							- GetSystemMetrics( SM_CYCAPTION );
			}

			//	�e���W�̈ړ��ʎ擾
			MOUSE.MoveX = MOUSE.PosX - MOUSE.PosXBk;
			MOUSE.MoveY = MOUSE.PosY - MOUSE.PosYBk;

			for( int i=0; i<3; i++ )
			{
				/////////////////////////////////////
				//	�}�E�X�̃{�^���������ꂽ��
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
			//	�z�C�[������ɃX�N���[�����ꂽ��
			if( dims.lZ > 10 )
			{
				MOUSE.WheelUp = ON;
				MOUSE.WheelUpCt ++;
				if( !MOUSE.WheelUpCt )
				{
					MOUSE.WheelUp = OFF;
				}
			}
			//	�X�N���[���I��
			else
			{
				MOUSE.WheelUp = MOUSE.WheelUpCt = OFF;
			}

			/////////////////////////////////////
			//	�z�C�[�������ɃX�N���[�����ꂽ��
			if( dims.lZ < -10 )
			{
				MOUSE.WheelDown = ON;
				MOUSE.WheelDownCt ++;
				if( !MOUSE.WheelDownCt )
				{
					MOUSE.WheelDown = OFF;
				}
			}
			//	�X�N���[���I��
			else
			{
				MOUSE.WheelDown = MOUSE.WheelDownCt = OFF;
			}
		}
		else if( Active && hr == DIERR_INPUTLOST )
		{
			//	���͊J�n
			m_DIDevMs->Acquire();
		}
	}
	return	TRUE;
}

/******************************************************************************/
//	���O	�F	�L�[�{�[�h�f�[�^�擾
//	����	�F	�L�[�{�[�h�f�[�^�̎擾���s��
//	�߂�l	�F	BOOL	��������	TRUE(����)
//									FALSE(���s)
//	����	�F	[IN]short	Active	�A�N�e�B�u�t���O
//	���l	�F	�Ȃ�
/******************************************************************************/
BOOL	CDx9LibInput::GetKeybordData( short Active )
{
	HRESULT	hr;

	BYTE	KeyState[256];
	int	i;

	//	�f�o�C�X���璼�ڃf�[�^�̎擾
	hr = m_DIDevKey->GetDeviceState( 256, KeyState );
	if( SUCCEEDED(hr) )
	{
		//	�f�[�^�̎擾�ɐ���
		for( i=0; i<256; i++ )
		{
			if( KeyState[i] & 0x80 )
			{
				/////////////////////////////////////
				//	�L�[�������ꂽ��
				KEY.Flag	  = ON;
				KEY.Trg[ i ]  = ON;
				KEY.Push[ i ] = ON;
				KEY.Count[ i ] ++;
			}
			else
			{
				/////////////////////////////////////
				//	�L�[�������ꂽ��
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
		//	���͊J�n
		m_DIDevKey->Acquire();
	}
	return	TRUE;
}

/******************************************************************************/
//	���O	�F	�W���C�X�e�B�b�N�f�[�^�擾
//	����	�F	�W���C�X�e�B�b�N�f�[�^�擾���s��
//	�߂�l	�F	BOOL	��������	TRUE(����)
//									FALSE(���s)
//	����	�F	[IN]short	Active	�A�N�e�B�u�t���O
//	���l	�F	�Ȃ�
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

		//	���݂̏�Ԃ��擾
		hr = g_DIDevJoy[no]->Poll();
		if( FAILED(hr) )
		{
			//	���͊J�n
			if( Active && hr == DIERR_INPUTLOST )
			{
				hr = g_DIDevJoy[no]->Acquire();
			}
		}

		//	�f�o�C�X�̒��ڃf�[�^���擾����
		hr = g_DIDevJoy[no]->GetDeviceState( sizeof(DIJOYSTATE), &dijs );
		if( hr == DI_OK )
		{
			////////////////////////////////////////////////////////////////////
			//	�v�b�V��

			//////////////////////////////////
			//	��
			if( dijs.lY < -128 )
			{
				JOY[ no ].Trg[ JOY_UP ] = ON;
				JOY[ no ].Push[ JOY_UP ] = ON;
				JOY[ no ].Count[ JOY_UP ] ++;
			}
			else
			{
				//	�����ꂽ��
				JOY[ no ].Trg[ JOY_UP ] = OFF;
				JOY[ no ].Push[ JOY_UP ] = OFF;
				JOY[ no ].Count[ JOY_UP ] = 0;
			}

			//////////////////////////////////
			//	��
			if( dijs.lY > 0 )
			{
				JOY[ no ].Trg[ JOY_DOWN ] = ON;
				JOY[ no ].Push[ JOY_DOWN ] = ON;
				JOY[ no ].Count[ JOY_DOWN ] ++;
			}
			else
			{
				//	�����ꂽ��
				JOY[ no ].Trg[ JOY_DOWN ] = OFF;
				JOY[ no ].Push[ JOY_DOWN ] = OFF;
				JOY[ no ].Count[ JOY_DOWN ] = 0;
			}

			//////////////////////////////////
			//	��
			if( dijs.lX < -128 )
			{
				JOY[ no ].Trg[ JOY_LEFT ] = ON;
				JOY[ no ].Push[ JOY_LEFT ] = ON;
				JOY[ no ].Count[ JOY_LEFT ] ++;
			}
			else
			{
				//	�����ꂽ��
				JOY[ no ].Trg[ JOY_LEFT ] = OFF;
				JOY[ no ].Push[ JOY_LEFT ] = OFF;
				JOY[ no ].Count[ JOY_LEFT ] = 0;
			}

			//////////////////////////////////
			//	�E
			if( dijs.lX > 128 )
			{
				JOY[ no ].Trg[ JOY_RIGHT ] = ON;
				JOY[ no ].Push[ JOY_RIGHT ] = ON;
				JOY[ no ].Count[ JOY_RIGHT ] ++;
			}
			else
			{
				//	�����ꂽ��
				JOY[ no ].Trg[ JOY_RIGHT ] = OFF;
				JOY[ no ].Push[ JOY_RIGHT ] = OFF;
				JOY[ no ].Count[ JOY_RIGHT ] = 0;
			}

			////////////////////////////////////////////////////////////////////
			//	�g���K
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
			//	�e�{�^��
			for( i=0; i<BUTTON_MAX-4; i++ )
			{
				//	�e�{�^���������ꂽ��
				if( dijs.rgbButtons[i] & 0x80 )
				{
					JOY[ no ].Trg[ i+4 ] = ON;
					JOY[ no ].Push[ i+4 ] = ON;
					JOY[ no ].Count[ i+4 ] ++;
				}
				else
				{
					//	�e�{�^���������ꂽ��
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
			//	���͊J�n
			g_DIDevJoy[ no ]->Acquire();
		}
	}
	return	TRUE;
}
