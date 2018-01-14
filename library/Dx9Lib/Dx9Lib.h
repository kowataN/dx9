/******************************************************************************/
//	DirectX9ライブラリ
//		外部宣言用ヘッダファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#ifdef	_DEBUG
#define	_CRTDBG_MAP_ALLOC
#endif	//	_DEBUG

#include	<windows.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<time.h>
#include	<math.h>
#include	<io.h>
#include	<conio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<direct.h>
#include	<string>
#include	<list>
using namespace std;

//	DirectX
#include	<d3d9.h>
#include	<d3dx9.h>
#include	<dinput.h>
#include	<dplay8.h>
#include	<dxerr9.h>
#include	<dmusici.h>

//	ライブラリのヘッダ
#include	"Dx9LibDefine.h"
#include	"Dx9LibInput.h"
#include	"Dx9LibTexture.h"
#include	"Dx9LibPattern.h"
#include	"Dx9LibSprite.h"
#include	"Dx9LibLine.h"
#include	"Dx9Lib2DPolygon.h"
#include	"Dx9LibMusic.h"
#include	"Dx9LibPlay.h"
#include	"Dx9LibText.h"
#include	"Dx9LibFade.h"
#include	"Dx9LibCommon.h"
#include	"Dx9LibIni.h"
#include	"Dx9LibDebug.h"
#include	"Dx9LibLog.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	ウインドウハンドル
extern	HWND				g_hWnd;
extern	int					g_WindowWidth;
extern	int					g_WindowHeight;
extern	int					g_ScreenMode;
extern	int					g_ShowCursor;

//	画面クリアカラー
extern	ULong				g_ColorR;
extern	ULong				g_ColorG;
extern	ULong				g_ColorB;

extern	LPDIRECT3D9			g_lpD3D;
extern	LPDIRECT3DDEVICE9	g_lpD3DDev;
extern	CDx9LibInput		g_LibInput;

//	汎用
extern	char				g_FreeWork[FREE_WORK_SIZE];
extern	char				g_ExePath[MAX_PATH];

//----------------------------------------------------------------------------//
//	関数の外部宣言
//----------------------------------------------------------------------------//
extern	void	MainLoop( void );
extern	void	Dx9LibInit( void );
