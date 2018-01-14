/******************************************************************************/
//	DirectX9ライブラリ
//		メインファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
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

//	グラフィック
#include	<d3d9.h>
#include	<d3dx9.h>
//	インプット
#include	<dinput.h>
//	オーディオ
#include	<dsound.h>
//	エラー
#include	<dxerr9.h>
//	ライブラリのヘッダ
#include	"./Dx9Lib.h"

//	デバッグ用
#ifdef	_DEBUG
#include	<crtdbg.h>
#define	new ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif	//	_DEBUG

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	ウインドウ
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
//	その他

//	画面クリアカラー
ULong					g_ColorR;			//	赤
ULong					g_ColorG;			//	緑
ULong					g_ColorB;			//	青
//	FPS関連
DWORD					g_OldTime=0;		//	以前の時間
DWORD					g_NowTime;			//	現在の時間
double					g_PrevTime;			//	〃
short					g_FpsCt;			//	FPSカウンタ

CDx9LibInput			g_LibInput;			//	インプット
CDx9LibPolygon			g_LibPol;			//	ポリゴン
CDx9LibText				g_LibText;			//	テキスト
CDx9LibMusic			g_LibMusic;			//	ミュージック
CDx9LibLine				g_LibLine;			//	ライン
CDx9LibFade				g_LibFade;			//	フェード
CDx9LibLog				g_LibLog;			//	ログ出力

//----------------------------------------------------------------------------//
//	オブジェクト
//----------------------------------------------------------------------------//
LPDIRECT3D9				g_lpD3D = NULL;		//	Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_lpD3DDev = NULL;	//	Direct3Dデバイスオブジェクト
D3DPRESENT_PARAMETERS	g_D3Dpp;			//	アダプタのパラメータ
D3DDISPLAYMODE			g_Display;			//	現在のディスプレイアダプタの状態

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//

//	WinMain
int	PASCAL	WinMain( HINSTANCE hInst, HINSTANCE hPrev, char *CmdLine, int CmdShow );
//	メインウインドウプロシージャ
LRESULT	CALLBACK	procMain( HWND hWnd, UINT msg, UINT wParam, LONG lParam );
//	描画処理
void	DrawMain( void );
//	デバイスの初期化
bool	InitDevice( void );
//	終了処理
short	EndApp( void );
//	メインルーチン
void	MainLoop( void );

////////////////////////////////////////
//	グラフィック関連
void	GraphicInit( void );

////////////////////////////////////////
//	その他
void	Dx9LibInit( void );
void	WaitTime( long time );
void	FpsLoop( void );



/******************************************************************************/
//	名前	：	デバイス初期化
//	説明	：	各種デバイスの初期化を行う
//	戻り値	：	bool	処理結果	true(成功)
//									false(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
bool	InitDevice( void )
{
	LOG_WRITE( TypeDebugLog, "デバイスの初期化開始" );

	// Direct3D オブジェクトを作成
	g_lpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( !g_lpD3D )
	{
		//	オブジェクト作成失敗
		LOG_WRITE( TypeErrLog, "Direct3D の作成に失敗しました。" );
		MessageBox( NULL,"Direct3D の作成に失敗しました。",
			g_szAppName, MB_OK | MB_ICONSTOP );
		//	終了する
		return	false;
	}

	//	ウインドウの作成が完了したので、Direct3D を初期化する
	ZeroMemory( &g_D3Dpp, sizeof( D3DPRESENT_PARAMETERS ) );

	//	現在の画面モードを取得
	HRESULT hRes = g_lpD3D->GetAdapterDisplayMode( g_Adapter, &g_Display );
	if( FAILED(hRes) )
	{
		LOG_WRITE( TypeErrLog, "画面モードの取得に失敗" );
		//	終了する
		return	false;
	}

	//	Direct3D 初期化パラメータの設定
	if( g_WindowStyle == FullScreenMode )
	{
		//	フルスクリーン : ほとんどのアダプタでサポートされているフォーマットを使用
		g_D3Dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

		//	リフレッシュレートの設定
		g_D3Dpp.FullScreen_RefreshRateInHz = FPS; 

		//	画面の更新方法
		g_D3Dpp.SwapEffect = D3DSWAPEFFECT_COPY;

		//	フルスクリーンモード
		g_D3Dpp.Windowed = FALSE;
	}
	else
	{
		//	ウインドウ : 現在の画面モードを使用
		g_D3Dpp.BackBufferFormat = g_Display.Format;

		//	リフレッシュレートの設定
		//	ウインドウモードでは、リフレッシュモードは設定できない
		g_D3Dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

		//	画面の更新方法
		g_D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//	高速に描画

		//	ウインドウモード
		g_D3Dpp.Windowed = TRUE;
	}

	//	バックバッファをロック
	g_D3Dpp.Flags = 0;

	//	バックバッファ数の設定（１：ダブルバッファ、２：トリプルバッファ）
	g_D3Dpp.BackBufferCount = 1;

	// 表示領域サイズの設定
	g_D3Dpp.BackBufferWidth = g_WindowWidth;
	g_D3Dpp.BackBufferHeight = HEIGHT;

	//	レンダリングの設定
	g_D3Dpp.hDeviceWindow = NULL;

	// Z バッファの自動作成
	g_D3Dpp.EnableAutoDepthStencil = 1;
	g_D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;

	/////////////////////////////////////
	//	インプットの初期化
	if( !g_LibInput.Initialize(g_hInstApp) )
	{
		LOG_WRITE( TypeErrLog, "DirectInputの初期化異常終了" );
		return	false;
	}

	// デバイスの作成 - T&L HAL
	if( FAILED( g_lpD3D->CreateDevice( g_Adapter, D3DDEVTYPE_HAL,
		g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_D3Dpp, &g_lpD3DDev ) ) )
	{
		// 失敗したので HAL で試行
		if( FAILED( g_lpD3D->CreateDevice( g_Adapter, D3DDEVTYPE_HAL,
			g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3Dpp, &g_lpD3DDev ) ) )
		{
			// 失敗したので REF で試行
			if( FAILED( g_lpD3D->CreateDevice( g_Adapter, D3DDEVTYPE_REF,
				g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3Dpp, &g_lpD3DDev ) ) )
			{
				// 結局失敗した
				LOG_WRITE( TypeErrLog, "Direct3D の初期化に失敗しました。" );
				MessageBox( NULL, "Direct3D の初期化に失敗しました。",
					g_szAppName, MB_OK | MB_ICONSTOP );

				//	終了処理
				EndApp();

				// 終了する
				return	false;
			}
		}
	}

	LOG_WRITE( TypeDebugLog, "デバイスの初期化終了" );
	return	true;
}

/******************************************************************************/
//	名前	：	グラフィック系の構造体の初期化
//	説明	：	各種構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	GraphicInit( void )
{
	//	テクスチャ初期化
	InitTextureAll();

	//	パターン初期化
	InitPatternAll();

	//	スプライト初期化
	CreateSpriteDevice();
	InitSpriteAll();

	//	ライン初期化
	g_LibLine.CreateDevice();
	g_LibLine.InitAll();

	//	ポリゴン初期化
	g_LibPol.InitAll();

	//	テキスト初期化
	g_LibText.InitAll();

	//	フェード初期化
	g_LibFade.Init();

	g_ColorR = WIN_CLEAR_R;
	g_ColorG = WIN_CLEAR_G;
	g_ColorB = WIN_CLEAR_B;

	/////////////////////////////////////
	//	レンダリング・ステートの設定

	//	Ｚバッファ有効化
	g_lpD3DDev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

	//	両面
	g_lpD3DDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	//	アルファブレンディング有効化
	g_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	// アルファブレンディング方法を設定
	g_lpD3DDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_lpD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// レンダリング時のアルファ値の計算方法の設定
	g_lpD3DDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);

	// ライト無効化
	g_lpD3DDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	// テクスチャの色を使用
	g_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	// 頂点の色を使用
	g_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	/////////////////////////////////////
	//	起動時間取得
	g_PrevTime = clock();
	g_FpsCt = 0;
}

/******************************************************************************/
//	名前	：	終了処理
//	説明	：	各種オブジェクトの解放を行う
//	戻り値	：	short	処理結果	1(成功)
//									0(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
short	EndApp( void )
{
	//	スプライト解放
	ReleaseSprite();

	//	テクスチャ解放
	ReleaseTexture();

	//	Direct3D オブジェクトを解放
	SAFE_RELEASE(g_lpD3DDev);
	SAFE_RELEASE(g_lpD3D);

	//	解放終了
	return	1;
}

/******************************************************************************/
//	名前	：	COM初期化
//	説明	：	COMの初期化を行う
//	戻り値	：	BOOL	処理結果	TRUE(成功)
//									FALSE(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
BOOL	InitCom( void )
{
	HRESULT	hr;

	//	ＣＯＭ初期化
	hr = CoInitialize( NULL );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeDebugLog, "COMの初期化に失敗" );
		DXTRACE_ERR( "COMの初期化に失敗", hr );
		return	FALSE;
	}

	return	TRUE;
}

/******************************************************************************/
//	名前	：	ライブラリセットアップ
//	説明	：	ライブラリの各種設定を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	「Dx9LibSetup.ini」が存在するのが前提
/******************************************************************************/
void	SetupDx9Lib( void )
{
	LOG_WRITE( TypeDebugLog, "ライブラリ初期化開始" );

	//	Exeパス取得
	char	BackupPath[MAX_PATH];
	if( GetModuleFileName(NULL, BackupPath, MAX_PATH ) == 0 )
	{
		LOG_WRITE( TypeDebugLog, "パスの取得に失敗しました。" );
		return;
	}
	PathToDir( BackupPath, g_ExePath );
	PathChange( g_ExePath, BackupPath );

	//	構成ファイル読み込み
	CDx9LibIni IniMng;
	IniMng.Initialize("Dx9LibSetup.ini");

	//	アプリケーション名設定
	sprintf_s( g_szAppName, MAX_PATH, IniMng.GetValueString( "AppInit", "AppName").c_str() );
	if( g_szAppName == "" )
	{
		sprintf_s( g_szAppName, MAX_PATH, APP_NAME );
	}

	//	サイズ設定
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

	//	スクリーンモード設定
	g_ScreenMode = IniMng.GetValueInt( "AppInit", "ScreenMode" );
	if( g_ScreenMode < 0 )
	{
		g_ScreenMode = SCREEN_MODE;
	}

	//	終了確認設定
	g_CloseFlag = IniMng.GetValueInt( "AppInit", "Close" );
	if( g_CloseFlag < 0 )
	{
		g_CloseFlag = WIN_CLOSE;
	}

	//	多重起動
	g_MultiExec = IniMng.GetValueInt( "AppInit", "MultiExec" );
	if( g_MultiExec < 0 )
	{
		g_MultiExec = MULTIPLEX_FLAG;
	}

	//	カーソル表示
	g_ShowCursor = IniMng.GetValueInt( "AppInit", "ShowCursor" );
	if( g_ShowCursor < 0 )
	{
		g_ShowCursor = CURSOR_DISP;
	}

	LOG_WRITE( TypeDebugLog, "ライブラリ初期化終了" );
}

/******************************************************************************/
//	名前	：	ウインメイン
//	説明	：	メインルーチン
//	戻り値	：	int		処理結果	1(成功)
//									0(失敗)
//	引数	：	[IN]HINSTANCE	hInst		インスタンスハンドル
//				[IN]HINSTANCE	hPrev		インスタンスハンドル
//				[IN]char		*CmdLine	コマンドライン引数
//				[IN]int			CmdShow		ウインドウの表示方式
//	備考	：	なし
/******************************************************************************/
int	PASCAL	WinMain( HINSTANCE hInst, HINSTANCE hPrev, char *CmdLine, int CmdShow )
{
#ifdef	_DEBUG
	//	メモリリーク検出設定
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif	//	_DEBUG

	LOG_WRITE( TypeDebugLog, "WinMain開始" );
	//g_LibLog.Write( TypeDebugLog, "WinMain開始", __FILE__, __LINE__ );

	g_hInstApp = hInst;

	//	ライブラリ設定
	SetupDx9Lib();

	//	多重起動
	if( g_MultiExec )
	{
		g_hMutex = CreateMutex( NULL, false, g_szAppName );
		if( GetLastError() == ERROR_ALREADY_EXISTS )
		{
			LOG_WRITE( TypeErrLog, "%sは既に起動中です。", g_szAppName );
			MessageBox( NULL, "起動中です", g_szAppName, MB_OK );
			return 0;
		}
	}

	//	ウインドウスタイル設定
	g_WindowStyle = (E_WindowStyle)g_ScreenMode;

	//	使用するアダプタ番号
	g_Adapter = 0;

	//	メインウインドウを作成する
	g_EndFlag = OFF;

	WNDCLASS	wc;

	//	メモリクリア
	ZeroMemory( &wc, sizeof( WNDCLASS ) );

	wc.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );	//	ウインドウの背景
	wc.hIcon		 = LoadIcon( hInst, "IDI_ICON1" );
	wc.hCursor		 = LoadCursor( NULL, IDC_ARROW );	//	カーソルスタイル
	wc.hInstance	 = hInst;		//	このインスタンスのハンドル
	wc.lpfnWndProc	 = procMain;	//	ウインドウ関数
	wc.lpszClassName = g_szAppName;	//	ウインドウクラス名

	RegisterClass( &wc );

//	ShowCursor( CURSOR_DISP );

	//	ウインドウの大きさを計算
	if( g_WindowStyle == FullScreenMode )
	{
		//	フルスクリーンモードで作成
		g_hWnd = CreateWindow(g_szAppName,	//	ウインドウクラスの名前
							g_szAppName,	//	ウインドウ名
							WS_POPUP,		//	ウインドウのスタイル
							0,0,			//	表示座標
							g_WindowWidth,g_WindowHeight,	//	ウインドウの幅
							NULL,			//	親ウインドウのハンドル
							NULL,			//	子ウインドウのID
							hInst,			//	インスタンスハンドル
							NULL			//	不明
							);
	}
	else
	{
		short	width,height;

		//	ウインドウ時はウインドウの外枠のサイズを考慮する
		width  = g_WindowWidth  + GetSystemMetrics( SM_CXDLGFRAME ) * 2;
		height = g_WindowHeight + GetSystemMetrics( SM_CYDLGFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );

		//	ウインドウモードで作成
		g_hWnd = CreateWindow(g_szAppName,	//	ウインドウクラスの名前
							g_szAppName,	//	ウインドウ名
							WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	//	ウインドウのスタイル
							CW_USEDEFAULT,CW_USEDEFAULT,	//	表示座標
							width,height,	//	ウインドウの幅
							NULL,			//	親ウインドウのハンドル
							NULL,			//	子ウインドウのID
							hInst,			//	インスタンスハンドル
							NULL			//	不明
							);
	}

	if( g_hWnd == NULL )
	{
		LOG_WRITE( TypeErrLog, "ウインドウの作成に失敗" );
		MessageBox( NULL, "ウインドウの作成に失敗", g_szAppName, MB_OK );
		return 0;
	}

	if( !InitCom() )
	{
		return	0;
	}

	/////////////////////////////////////
	//	デバイスの初期化
	if( !InitDevice() )
	{
		LOG_WRITE( TypeErrLog, "デバイスの初期化異常終了" );
		return	0;
	}

	/////////////////////////////////////
	//	オーディオの初期化
	if( !g_LibMusic.InitDevice() )
	{
		LOG_WRITE( TypeErrLog, "DirectMusicの初期化異常終了" );
		return	0;
	}

	/////////////////////////////////////
	//	テキスト初期化
	g_LibText.InitDevice();

	/////////////////////////////////////
	//	全初期化
	GraphicInit();

	//	指定したウィンドウの表示状態を設定
	ShowWindow( g_hWnd, CmdShow );

	//	ウィンドウの更新領域を調べる
	UpdateWindow( g_hWnd );

	//	メインウインドウループ
	MSG msg;
	while( !g_EndFlag )
	{
		//	メッセージが来ているか?
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			//	メッセージの処理
			//	仮想キーメッセージを文字メッセージに変換
			TranslateMessage( &msg );

			//	ウィンドウプロシージャにメッセージを送出する
			DispatchMessage( &msg );
		}
		else
		{
			//	FPS管理
			FpsLoop();
		}
	}

	//	終了処理
	EndApp();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif	//	_DEBUG

	LOG_WRITE( TypeDebugLog, "WinMain終了" );

	//	終了する
	return	0;
}

/******************************************************************************/
//	名前	：	ウインドウモード切り替え
//	説明	：	ウインドウモードの切り替えを行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	ChangeWindowMode( void )
{
	if( g_WindowStyle == FullScreenMode )
	{
		g_WindowStyle = WindowMode;

		//	ウインドウサイズ設定
		SetWindowPos( g_hWnd, HWND_NOTOPMOST,
			g_WindowRect.left, g_WindowRect.top,
			g_WindowRect.right - g_WindowRect.left,
			g_WindowRect.bottom - g_WindowRect.top,
			SWP_SHOWWINDOW );
		//	フルスクリーン→ウインドウ
		SetWindowLong( g_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE );
	}
	else
	{
		g_WindowStyle = FullScreenMode;

		//	ウインドウサイズ取得
		GetWindowRect( g_hWnd, &g_WindowRect );
		//	ウインドウ→フルスクリーン
		SetWindowLong( g_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
	}

	InitDevice();
}

/******************************************************************************/
//	名前	：	メインウインドウプロシージャ
//	説明	：	メッセージ処理を行う
//	戻り値	：	LRESULT		処理結果	1(終了)
//										0(ループ)
//	引数	：	[IN]HWND	hWnd		ウインドウハンドル
//				[IN]UINT	msg			メッセージ
//				[IN]UINT	wParam		メッセージのパラメータ
//				[IN]LONG	lParam		メッセージのパラメータ
//	備考	：	なし
/******************************************************************************/
LRESULT	CALLBACK	procMain( HWND hWnd, UINT msg, UINT wParam, LONG lParam )
{
	PAINTSTRUCT	ps;
	switch( msg )
	{
		//	表示
		case	WM_PAINT:
			BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			break;

		//	アクティブの時
		case	WM_ACTIVATE:
			g_Active = !( (short)HIWORD(wParam) );
			if( wParam == WA_INACTIVE )
			{
				//	アクセス権開放
				g_LibInput.InputUnacquire();
			}
			else
			{
				g_LibInput.InputAcquire();
			}
			break;

		//	キーが押された時
		case	WM_KEYDOWN:
			switch( wParam )
			{
				// ESCキーで終了
				case	VK_ESCAPE:
					// 終了確認
					if( g_CloseFlag )
					{
						if( MessageBox( hWnd, 
							"終了してもよろしいですか？", "終了確認",
							MB_YESNO | MB_TOPMOST | MB_ICONQUESTION ) == IDYES )
						{
							//	ウインドウ破棄
							DestroyWindow( g_hWnd );
							g_EndFlag = ON;
							return	1L;
						}
					}
					else
					{
						//	ウインドウ破棄
						DestroyWindow( hWnd );
						g_EndFlag = ON;
						return	1L;
					}
					break;

				case	VK_F4:
					if( CHANGE_SCREEN_MODE )
					{
						//	ウインドウ切り替え
						ChangeWindowMode();
					}
					break;
			}
			break;

		//	ウインドウを閉じる
		case	WM_CLOSE:
			g_EndFlag = ON;
			//	ウインドウ破棄
			DestroyWindow( hWnd );
			return	1L;

		//	ウインドウを破棄
		case	WM_DESTROY:
			PostQuitMessage( 0 );
			ReleaseMutex(g_hMutex);
			break;

		//	その他
		default:
			return	DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0L;
}

/******************************************************************************/
//	名前	：	描画処理
//	説明	：	描画処理を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	DrawMain( void )
{
	g_NowTime = clock();

	if( !g_lpD3D || !g_lpD3DDev )
	{
		return;
	}

	//	マウスとキーボードの情報の取得
	g_LibInput.GetInputData(g_Active);

	// バックバッファと Z バッファをクリア
	g_lpD3DDev->Clear( 0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0,g_ColorR,g_ColorG,g_ColorB), 1.0f, 0 );

	// シーン開始
	g_lpD3DDev->BeginScene();

	//	スプライト
	DispSpriteAll();

	//	ポリゴン
	g_LibPol.DispAll();

	//	ライン
	g_LibLine.DispAll();

	//	テキスト強制描画
	g_LibText.DispAll();

	// シーン終了
	g_lpD3DDev->EndScene();

	//	フェード描画
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

		//	FPSカウンタ初期化
		g_FpsCt = 0;

		g_OldTime = g_NowTime;
	}

	// バックバッファをプライマリバッファにコピー
	if( FAILED( g_lpD3DDev->Present( NULL, NULL, NULL, NULL ) ) )
	{
		g_lpD3DDev->Reset( &g_D3Dpp );
	}
}

/******************************************************************************/
//	名前	：	ライブラリ初期化
//	説明	：	ライブラリの初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
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
//	名前	：	ＦＰＳ制御
//	説明	：	FPSの制御を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
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

		//	メインループ
		MainLoop();
		//	描画処理
		DrawMain();
	}
}
