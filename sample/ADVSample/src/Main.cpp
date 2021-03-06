/******************************************************************************/
//	メインソース
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./ScriptEngine.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
long	MainMode = 0;
long	TestFlag = 0;
char	AnmBuf[1024];
char	MoveBuf[1024];
CScriptEngine SCR;

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	MainInit( void );					//	メイン初期化
void	GameMain( void );					//	ゲームメイン



/******************************************************************************/
//	メインルーチン
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	MainLoop( void )
{
	switch( MainMode )
	{
		case	0:
			MainInit();
			MainMode = 1;

		case	1:
			GameMain();
			break;
	}
}

/******************************************************************************/
//	起動初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	MainInit( void )
{
	Dx9LibInit();
	g_LibMusic.InitAll();

	SCR.Init( "data\\data.txt" );
}

/******************************************************************************/
//	ゲームメインルーチン
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	GameMain( void )
{
	SCR.Main();
}
