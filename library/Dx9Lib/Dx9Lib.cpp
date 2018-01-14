/******************************************************************************/
//	DirectX9���C�u����
//		���C���t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#ifdef	_DEBUG
#define	_CRTDBG_MAP_ALLOC
#endif	//	_DEBUG

#include	<windows.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	<math.h>
#include	<io.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<direct.h>

//	�O���t�B�b�N
#include	<d3d9.h>
#include	<d3dx9.h>
//	�C���v�b�g
#include	<dinput.h>
//	�I�[�f�B�I
#include	<dsound.h>
//	�G���[
#include	<dxerr9.h>
//	���C�u�����̃w�b�_
#include	"./Dx9Lib.h"

//	�f�o�b�O�p
#ifdef	_DEBUG
#include	<crtdbg.h>
#define	new ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif	//	_DEBUG

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	�E�C���h�E
HINSTANCE				g_hInstApp;
int						g_Adapter;
HWND					g_hWnd;
int						g_EndFlag;
char					g_szAppName[MAX_PATH] = { APP_NAME };
int						g_WindowWidth;
int						g_WindowHeight;
int						g_ScreenMode;
int						g_CloseFlag;
int						g_MultiExec;
int						g_ShowCursor;
short					g_Active = 0;
HANDLE					g_hMutex;
char					g_ExePath[MAX_PATH];
E_WindowStyle			g_WindowStyle;
RECT					g_WindowRect;

////////////////////////////////////////
//	���̑�

//	��ʃN���A�J���[
ULong					g_ColorR;			//	��
ULong					g_ColorG;			//	��
ULong					g_ColorB;			//	��
//	FPS�֘A
DWORD					g_OldTime=0;		//	�ȑO�̎���
DWORD					g_NowTime;			//	���݂̎���
double					g_PrevTime;			//	�V
short					g_FpsCt;			//	FPS�J�E���^

CDx9LibInput			g_LibInput;			//	�C���v�b�g
CDx9LibPolygon			g_LibPol;			//	�|���S��
CDx9LibText				g_LibText;			//	�e�L�X�g
CDx9LibMusic			g_LibMusic;			//	�~���[�W�b�N
CDx9LibLine				g_LibLine;			//	���C��
CDx9LibFade				g_LibFade;			//	�t�F�[�h
CDx9LibLog				g_LibLog;			//	���O�o��

//----------------------------------------------------------------------------//
//	�I�u�W�F�N�g
//----------------------------------------------------------------------------//
LPDIRECT3D9				g_lpD3D = NULL;		//	Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9		g_lpD3DDev = NULL;	//	Direct3D�f�o�C�X�I�u�W�F�N�g
D3DPRESENT_PARAMETERS	g_D3Dpp;			//	�A�_�v�^�̃p�����[�^
D3DDISPLAYMODE			g_Display;			//	���݂̃f�B�X�v���C�A�_�v�^�̏��

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//

//	WinMain
int	PASCAL	WinMain( HINSTANCE hInst, HINSTANCE hPrev, char *CmdLine, int CmdShow );
//	���C���E�C���h�E�v���V�[�W��
LRESULT	CALLBACK	procMain( HWND hWnd, UINT msg, UINT wParam, LONG lParam );
//	�`�揈��
void	DrawMain( void );
//	�f�o�C�X�̏�����
bool	InitDevice( void );
//	�I������
short	EndApp( void );
//	���C�����[�`��
void	MainLoop( void );

////////////////////////////////////////
//	�O���t�B�b�N�֘A
void	GraphicInit( void );

////////////////////////////////////////
//	���̑�
void	Dx9LibInit( void );
void	WaitTime( long time );
void	FpsLoop( void );



/******************************************************************************/
//	���O	�F	�f�o�C�X������
//	����	�F	�e��f�o�C�X�̏��������s��
//	�߂�l	�F	bool	��������	true(����)
//									false(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
bool	InitDevice( void )
{
	LOG_WRITE( TypeDebugLog, "�f�o�C�X�̏������J�n" );

	// Direct3D �I�u�W�F�N�g���쐬
	g_lpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( !g_lpD3D )
	{
		//	�I�u�W�F�N�g�쐬���s
		LOG_WRITE( TypeErrLog, "Direct3D �̍쐬�Ɏ��s���܂����B" );
		MessageBox( NULL,"Direct3D �̍쐬�Ɏ��s���܂����B",
			g_szAppName, MB_OK | MB_ICONSTOP );
		//	�I������
		return	false;
	}

	//	�E�C���h�E�̍쐬�����������̂ŁADirect3D ������������
	ZeroMemory( &g_D3Dpp, sizeof( D3DPRESENT_PARAMETERS ) );

	//	���݂̉�ʃ��[�h���擾
	HRESULT hRes = g_lpD3D->GetAdapterDisplayMode( g_Adapter, &g_Display );
	if( FAILED(hRes) )
	{
		LOG_WRITE( TypeErrLog, "��ʃ��[�h�̎擾�Ɏ��s" );
		//	�I������
		return	false;
	}

	//	Direct3D �������p�����[�^�̐ݒ�
	if( g_WindowStyle == FullScreenMode )
	{
		//	�t���X�N���[�� : �قƂ�ǂ̃A�_�v�^�ŃT�|�[�g����Ă���t�H�[�}�b�g���g�p
		g_D3Dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

		//	���t���b�V�����[�g�̐ݒ�
		g_D3Dpp.FullScreen_RefreshRateInHz = FPS; 

		//	��ʂ̍X�V���@
		g_D3Dpp.SwapEffect = D3DSWAPEFFECT_COPY;

		//	�t���X�N���[�����[�h
		g_D3Dpp.Windowed = FALSE;
	}
	else
	{
		//	�E�C���h�E : ���݂̉�ʃ��[�h���g�p
		g_D3Dpp.BackBufferFormat = g_Display.Format;

		//	���t���b�V�����[�g�̐ݒ�
		//	�E�C���h�E���[�h�ł́A���t���b�V�����[�h�͐ݒ�ł��Ȃ�
		g_D3Dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

		//	��ʂ̍X�V���@
		g_D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//	�����ɕ`��

		//	�E�C���h�E���[�h
		g_D3Dpp.Windowed = TRUE;
	}

	//	�o�b�N�o�b�t�@�����b�N
	g_D3Dpp.Flags = 0;

	//	�o�b�N�o�b�t�@���̐ݒ�i�P�F�_�u���o�b�t�@�A�Q�F�g���v���o�b�t�@�j
	g_D3Dpp.BackBufferCount = 1;

	// �\���̈�T�C�Y�̐ݒ�
	g_D3Dpp.BackBufferWidth = g_WindowWidth;
	g_D3Dpp.BackBufferHeight = HEIGHT;

	//	�����_�����O�̐ݒ�
	g_D3Dpp.hDeviceWindow = NULL;

	// Z �o�b�t�@�̎����쐬
	g_D3Dpp.EnableAutoDepthStencil = 1;
	g_D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;

	/////////////////////////////////////
	//	�C���v�b�g�̏�����
	if( !g_LibInput.Initialize(g_hInstApp) )
	{
		LOG_WRITE( TypeErrLog, "DirectInput�̏������ُ�I��" );
		return	false;
	}

	// �f�o�C�X�̍쐬 - T&L HAL
	if( FAILED( g_lpD3D->CreateDevice( g_Adapter, D3DDEVTYPE_HAL,
		g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_D3Dpp, &g_lpD3DDev ) ) )
	{
		// ���s�����̂� HAL �Ŏ��s
		if( FAILED( g_lpD3D->CreateDevice( g_Adapter, D3DDEVTYPE_HAL,
			g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3Dpp, &g_lpD3DDev ) ) )
		{
			// ���s�����̂� REF �Ŏ��s
			if( FAILED( g_lpD3D->CreateDevice( g_Adapter, D3DDEVTYPE_REF,
				g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3Dpp, &g_lpD3DDev ) ) )
			{
				// ���ǎ��s����
				LOG_WRITE( TypeErrLog, "Direct3D �̏������Ɏ��s���܂����B" );
				MessageBox( NULL, "Direct3D �̏������Ɏ��s���܂����B",
					g_szAppName, MB_OK | MB_ICONSTOP );

				//	�I������
				EndApp();

				// �I������
				return	false;
			}
		}
	}

	LOG_WRITE( TypeDebugLog, "�f�o�C�X�̏������I��" );
	return	true;
}

/******************************************************************************/
//	���O	�F	�O���t�B�b�N�n�̍\���̂̏�����
//	����	�F	�e��\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	GraphicInit( void )
{
	//	�e�N�X�`��������
	InitTextureAll();

	//	�p�^�[��������
	InitPatternAll();

	//	�X�v���C�g������
	CreateSpriteDevice();
	InitSpriteAll();

	//	���C��������
	g_LibLine.CreateDevice();
	g_LibLine.InitAll();

	//	�|���S��������
	g_LibPol.InitAll();

	//	�e�L�X�g������
	g_LibText.InitAll();

	//	�t�F�[�h������
	g_LibFade.Init();

	g_ColorR = WIN_CLEAR_R;
	g_ColorG = WIN_CLEAR_G;
	g_ColorB = WIN_CLEAR_B;

	/////////////////////////////////////
	//	�����_�����O�E�X�e�[�g�̐ݒ�

	//	�y�o�b�t�@�L����
	g_lpD3DDev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

	//	����
	g_lpD3DDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	//	�A���t�@�u�����f�B���O�L����
	g_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	// �A���t�@�u�����f�B���O���@��ݒ�
	g_lpD3DDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_lpD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// �����_�����O���̃A���t�@�l�̌v�Z���@�̐ݒ�
	g_lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);

	// ���C�g������
	g_lpD3DDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	// �e�N�X�`���̐F���g�p
	g_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	// ���_�̐F���g�p
	g_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	/////////////////////////////////////
	//	�N�����Ԏ擾
	g_PrevTime = clock();
	g_FpsCt = 0;
}

/******************************************************************************/
//	���O	�F	�I������
//	����	�F	�e��I�u�W�F�N�g�̉�����s��
//	�߂�l	�F	short	��������	1(����)
//									0(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
short	EndApp( void )
{
	//	�X�v���C�g���
	ReleaseSprite();

	//	�e�N�X�`�����
	ReleaseTexture();

	//	Direct3D �I�u�W�F�N�g�����
	SAFE_RELEASE(g_lpD3DDev);
	SAFE_RELEASE(g_lpD3D);

	//	����I��
	return	1;
}

/******************************************************************************/
//	���O	�F	COM������
//	����	�F	COM�̏��������s��
//	�߂�l	�F	BOOL	��������	TRUE(����)
//									FALSE(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
BOOL	InitCom( void )
{
	HRESULT	hr;

	//	�b�n�l������
	hr = CoInitialize( NULL );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeDebugLog, "COM�̏������Ɏ��s" );
		DXTRACE_ERR( "COM�̏������Ɏ��s", hr );
		return	FALSE;
	}

	return	TRUE;
}

/******************************************************************************/
//	���O	�F	���C�u�����Z�b�g�A�b�v
//	����	�F	���C�u�����̊e��ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�uDx9LibSetup.ini�v�����݂���̂��O��
/******************************************************************************/
void	SetupDx9Lib( void )
{
	LOG_WRITE( TypeDebugLog, "���C�u�����������J�n" );

	//	Exe�p�X�擾
	char	BackupPath[MAX_PATH];
	if( GetModuleFileName(NULL, BackupPath, MAX_PATH ) == 0 )
	{
		LOG_WRITE( TypeDebugLog, "�p�X�̎擾�Ɏ��s���܂����B" );
		return;
	}
	PathToDir( BackupPath, g_ExePath );
	PathChange( g_ExePath, BackupPath );

	//	�\���t�@�C���ǂݍ���
	CDx9LibIni IniMng;
	IniMng.Initialize("Dx9LibSetup.ini");

	//	�A�v���P�[�V�������ݒ�
	sprintf_s( g_szAppName, MAX_PATH, IniMng.GetValueString( "AppInit", "AppName").c_str() );
	if( g_szAppName == "" )
	{
		sprintf_s( g_szAppName, MAX_PATH, APP_NAME );
	}

	//	�T�C�Y�ݒ�
	g_WindowWidth = IniMng.GetValueInt( "AppInit", "Width" );
	if( g_WindowWidth < 0 )
	{
		g_WindowWidth = WIDTH;
	}
	g_WindowHeight = IniMng.GetValueInt( "AppInit", "Height" );
	if( g_WindowHeight < 0 )
	{
		g_WindowHeight = HEIGHT;
	}

	//	�X�N���[�����[�h�ݒ�
	g_ScreenMode = IniMng.GetValueInt( "AppInit", "ScreenMode" );
	if( g_ScreenMode < 0 )
	{
		g_ScreenMode = SCREEN_MODE;
	}

	//	�I���m�F�ݒ�
	g_CloseFlag = IniMng.GetValueInt( "AppInit", "Close" );
	if( g_CloseFlag < 0 )
	{
		g_CloseFlag = WIN_CLOSE;
	}

	//	���d�N��
	g_MultiExec = IniMng.GetValueInt( "AppInit", "MultiExec" );
	if( g_MultiExec < 0 )
	{
		g_MultiExec = MULTIPLEX_FLAG;
	}

	//	�J�[�\���\��
	g_ShowCursor = IniMng.GetValueInt( "AppInit", "ShowCursor" );
	if( g_ShowCursor < 0 )
	{
		g_ShowCursor = CURSOR_DISP;
	}

	LOG_WRITE( TypeDebugLog, "���C�u�����������I��" );
}

/******************************************************************************/
//	���O	�F	�E�C�����C��
//	����	�F	���C�����[�`��
//	�߂�l	�F	int		��������	1(����)
//									0(���s)
//	����	�F	[IN]HINSTANCE	hInst		�C���X�^���X�n���h��
//				[IN]HINSTANCE	hPrev		�C���X�^���X�n���h��
//				[IN]char		*CmdLine	�R�}���h���C������
//				[IN]int			CmdShow		�E�C���h�E�̕\������
//	���l	�F	�Ȃ�
/******************************************************************************/
int	PASCAL	WinMain( HINSTANCE hInst, HINSTANCE hPrev, char *CmdLine, int CmdShow )
{
#ifdef	_DEBUG
	//	���������[�N���o�ݒ�
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif	//	_DEBUG

	LOG_WRITE( TypeDebugLog, "WinMain�J�n" );
	//g_LibLog.Write( TypeDebugLog, "WinMain�J�n", __FILE__, __LINE__ );

	g_hInstApp = hInst;

	//	���C�u�����ݒ�
	SetupDx9Lib();

	//	���d�N��
	if( g_MultiExec )
	{
		g_hMutex = CreateMutex( NULL, false, g_szAppName );
		if( GetLastError() == ERROR_ALREADY_EXISTS )
		{
			LOG_WRITE( TypeErrLog, "%s�͊��ɋN�����ł��B", g_szAppName );
			MessageBox( NULL, "�N�����ł�", g_szAppName, MB_OK );
			return 0;
		}
	}

	//	�E�C���h�E�X�^�C���ݒ�
	g_WindowStyle = (E_WindowStyle)g_ScreenMode;

	//	�g�p����A�_�v�^�ԍ�
	g_Adapter = 0;

	//	���C���E�C���h�E���쐬����
	g_EndFlag = OFF;

	WNDCLASS	wc;

	//	�������N���A
	ZeroMemory( &wc, sizeof( WNDCLASS ) );

	wc.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );	//	�E�C���h�E�̔w�i
	wc.hIcon		 = LoadIcon( hInst, "IDI_ICON1" );
	wc.hCursor		 = LoadCursor( NULL, IDC_ARROW );	//	�J�[�\���X�^�C��
	wc.hInstance	 = hInst;		//	���̃C���X�^���X�̃n���h��
	wc.lpfnWndProc	 = procMain;	//	�E�C���h�E�֐�
	wc.lpszClassName = g_szAppName;	//	�E�C���h�E�N���X��

	RegisterClass( &wc );

//	ShowCursor( CURSOR_DISP );

	//	�E�C���h�E�̑傫�����v�Z
	if( g_WindowStyle == FullScreenMode )
	{
		//	�t���X�N���[�����[�h�ō쐬
		g_hWnd = CreateWindow(g_szAppName,	//	�E�C���h�E�N���X�̖��O
							g_szAppName,	//	�E�C���h�E��
							WS_POPUP,		//	�E�C���h�E�̃X�^�C��
							0,0,			//	�\�����W
							g_WindowWidth,g_WindowHeight,	//	�E�C���h�E�̕�
							NULL,			//	�e�E�C���h�E�̃n���h��
							NULL,			//	�q�E�C���h�E��ID
							hInst,			//	�C���X�^���X�n���h��
							NULL			//	�s��
							);
	}
	else
	{
		short	width,height;

		//	�E�C���h�E���̓E�C���h�E�̊O�g�̃T�C�Y���l������
		width  = g_WindowWidth  + GetSystemMetrics( SM_CXDLGFRAME ) * 2;
		height = g_WindowHeight + GetSystemMetrics( SM_CYDLGFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );

		//	�E�C���h�E���[�h�ō쐬
		g_hWnd = CreateWindow(g_szAppName,	//	�E�C���h�E�N���X�̖��O
							g_szAppName,	//	�E�C���h�E��
							WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	//	�E�C���h�E�̃X�^�C��
							CW_USEDEFAULT,CW_USEDEFAULT,	//	�\�����W
							width,height,	//	�E�C���h�E�̕�
							NULL,			//	�e�E�C���h�E�̃n���h��
							NULL,			//	�q�E�C���h�E��ID
							hInst,			//	�C���X�^���X�n���h��
							NULL			//	�s��
							);
	}

	if( g_hWnd == NULL )
	{
		LOG_WRITE( TypeErrLog, "�E�C���h�E�̍쐬�Ɏ��s" );
		MessageBox( NULL, "�E�C���h�E�̍쐬�Ɏ��s", g_szAppName, MB_OK );
		return 0;
	}

	if( !InitCom() )
	{
		return	0;
	}

	/////////////////////////////////////
	//	�f�o�C�X�̏�����
	if( !InitDevice() )
	{
		LOG_WRITE( TypeErrLog, "�f�o�C�X�̏������ُ�I��" );
		return	0;
	}

	/////////////////////////////////////
	//	�I�[�f�B�I�̏�����
	if( !g_LibMusic.InitDevice() )
	{
		LOG_WRITE( TypeErrLog, "DirectMusic�̏������ُ�I��" );
		return	0;
	}

	/////////////////////////////////////
	//	�e�L�X�g������
	g_LibText.InitDevice();

	/////////////////////////////////////
	//	�S������
	GraphicInit();

	//	�w�肵���E�B���h�E�̕\����Ԃ�ݒ�
	ShowWindow( g_hWnd, CmdShow );

	//	�E�B���h�E�̍X�V�̈�𒲂ׂ�
	UpdateWindow( g_hWnd );

	//	���C���E�C���h�E���[�v
	MSG msg;
	while( !g_EndFlag )
	{
		//	���b�Z�[�W�����Ă��邩?
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			//	���b�Z�[�W�̏���
			//	���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
			TranslateMessage( &msg );

			//	�E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
			DispatchMessage( &msg );
		}
		else
		{
			//	FPS�Ǘ�
			FpsLoop();
		}
	}

	//	�I������
	EndApp();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif	//	_DEBUG

	LOG_WRITE( TypeDebugLog, "WinMain�I��" );

	//	�I������
	return	0;
}

/******************************************************************************/
//	���O	�F	�E�C���h�E���[�h�؂�ւ�
//	����	�F	�E�C���h�E���[�h�̐؂�ւ����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	ChangeWindowMode( void )
{
	if( g_WindowStyle == FullScreenMode )
	{
		g_WindowStyle = WindowMode;

		//	�E�C���h�E�T�C�Y�ݒ�
		SetWindowPos( g_hWnd, HWND_NOTOPMOST,
			g_WindowRect.left, g_WindowRect.top,
			g_WindowRect.right - g_WindowRect.left,
			g_WindowRect.bottom - g_WindowRect.top,
			SWP_SHOWWINDOW );
		//	�t���X�N���[�����E�C���h�E
		SetWindowLong( g_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE );
	}
	else
	{
		g_WindowStyle = FullScreenMode;

		//	�E�C���h�E�T�C�Y�擾
		GetWindowRect( g_hWnd, &g_WindowRect );
		//	�E�C���h�E���t���X�N���[��
		SetWindowLong( g_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
	}

	InitDevice();
}

/******************************************************************************/
//	���O	�F	���C���E�C���h�E�v���V�[�W��
//	����	�F	���b�Z�[�W�������s��
//	�߂�l	�F	LRESULT		��������	1(�I��)
//										0(���[�v)
//	����	�F	[IN]HWND	hWnd		�E�C���h�E�n���h��
//				[IN]UINT	msg			���b�Z�[�W
//				[IN]UINT	wParam		���b�Z�[�W�̃p�����[�^
//				[IN]LONG	lParam		���b�Z�[�W�̃p�����[�^
//	���l	�F	�Ȃ�
/******************************************************************************/
LRESULT	CALLBACK	procMain( HWND hWnd, UINT msg, UINT wParam, LONG lParam )
{
	PAINTSTRUCT	ps;
	switch( msg )
	{
		//	�\��
		case	WM_PAINT:
			BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			break;

		//	�A�N�e�B�u�̎�
		case	WM_ACTIVATE:
			g_Active = !( (short)HIWORD(wParam) );
			if( wParam == WA_INACTIVE )
			{
				//	�A�N�Z�X���J��
				g_LibInput.InputUnacquire();
			}
			else
			{
				g_LibInput.InputAcquire();
			}
			break;

		//	�L�[�������ꂽ��
		case	WM_KEYDOWN:
			switch( wParam )
			{
				// ESC�L�[�ŏI��
				case	VK_ESCAPE:
					// �I���m�F
					if( g_CloseFlag )
					{
						if( MessageBox( hWnd, 
							"�I�����Ă���낵���ł����H", "�I���m�F",
							MB_YESNO | MB_TOPMOST | MB_ICONQUESTION ) == IDYES )
						{
							//	�E�C���h�E�j��
							DestroyWindow( g_hWnd );
							g_EndFlag = ON;
							return	1L;
						}
					}
					else
					{
						//	�E�C���h�E�j��
						DestroyWindow( hWnd );
						g_EndFlag = ON;
						return	1L;
					}
					break;

				case	VK_F4:
					if( CHANGE_SCREEN_MODE )
					{
						//	�E�C���h�E�؂�ւ�
						ChangeWindowMode();
					}
					break;
			}
			break;

		//	�E�C���h�E�����
		case	WM_CLOSE:
			g_EndFlag = ON;
			//	�E�C���h�E�j��
			DestroyWindow( hWnd );
			return	1L;

		//	�E�C���h�E��j��
		case	WM_DESTROY:
			PostQuitMessage( 0 );
			ReleaseMutex(g_hMutex);
			break;

		//	���̑�
		default:
			return	DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0L;
}

/******************************************************************************/
//	���O	�F	�`�揈��
//	����	�F	�`�揈�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	DrawMain( void )
{
	g_NowTime = clock();

	if( !g_lpD3D || !g_lpD3DDev )
	{
		return;
	}

	//	�}�E�X�ƃL�[�{�[�h�̏��̎擾
	g_LibInput.GetInputData(g_Active);

	// �o�b�N�o�b�t�@�� Z �o�b�t�@���N���A
	g_lpD3DDev->Clear( 0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0,g_ColorR,g_ColorG,g_ColorB), 1.0f, 0 );

	// �V�[���J�n
	g_lpD3DDev->BeginScene();

	//	�X�v���C�g
	DispSpriteAll();

	//	�|���S��
	g_LibPol.DispAll();

	//	���C��
	g_LibLine.DispAll();

	//	�e�L�X�g�����`��
	g_LibText.DispAll();

	// �V�[���I��
	g_lpD3DDev->EndScene();

	//	�t�F�[�h�`��
	g_LibFade.Loop();

	g_FpsCt ++;
	if( g_NowTime - g_OldTime >= 1000 )
	{
		if( FPS_DISP )
		{
			char	str[256];
			wsprintf( str, "%s : FPS=%d", g_szAppName, g_FpsCt );
			SetWindowText( g_hWnd, str );
		}

		//	FPS�J�E���^������
		g_FpsCt = 0;

		g_OldTime = g_NowTime;
	}

	// �o�b�N�o�b�t�@���v���C�}���o�b�t�@�ɃR�s�[
	if( FAILED( g_lpD3DDev->Present( NULL, NULL, NULL, NULL ) ) )
	{
		g_lpD3DDev->Reset( &g_D3Dpp );
	}
}

/******************************************************************************/
//	���O	�F	���C�u����������
//	����	�F	���C�u�����̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	Dx9LibInit( void )
{
	InitTextureAll();
	InitPatternAll();
	InitSpriteAll();
	g_LibPol.InitAll();
	g_LibText.InitAll();
	g_LibFade.Init();
	g_LibLine.InitAll();
	g_LibMusic.InitAll();
}

/******************************************************************************/
//	���O	�F	�e�o�r����
//	����	�F	FPS�̐�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	FpsLoop( void )
{
	if( clock() < g_PrevTime )
	{
		g_PrevTime = clock();
	}

	double time = fabs( clock() - g_PrevTime );
	if( time >= 1000/FPS )
	{
		do
		{
			time -= 1000/FPS;
			g_PrevTime += 1000 / FPS;
		}
		while( time >= 1000/FPS );

		//	���C�����[�v
		MainLoop();
		//	�`�揈��
		DrawMain();
	}
}
