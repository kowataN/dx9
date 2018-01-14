/******************************************************************************/
//	DirectX9ライブラリ
//		デバッグ
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"



/******************************************************************************/
//	名前	：	デバッグ出力
//	説明	：	出力ウインドウにデバッグ文字を出力する
//	戻り値	：	なし
//	引数	：	[IN]char	*Format	フォーマット（"%d"など）
//	備考	：	なし
/******************************************************************************/
void	OutputDebug( char *Fromat, ... )
{
	va_list	argp;
	char pszBuf[ 256];
	va_start(argp, Fromat );
	vsprintf_s( pszBuf, 256, Fromat, argp);
	va_end(argp);
	OutputDebugString( pszBuf );
}
