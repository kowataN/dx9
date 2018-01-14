/******************************************************************************/
//	当たり判定ライブラリ
//		ソースファイル
/******************************************************************************/


//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibHit.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	当たり判定用構造体
ST_HitData	HIT[HIT_MAX];

//	読み込み用構造体
ST_HitLoad	HLT[HIT_MAX];

//	パス
char	HitFilePath[MAX_PATH];



/******************************************************************************/
//	名前	：	ヒットデータ初期化
//	説明	：	ヒット構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	HitNo		ヒット番号
//	備考	：	なし
/******************************************************************************/
void	InitHit( long HitNo )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	HIT[ HitNo ].Use = OFF;
	HIT[ HitNo ].Flag = 0;
	HIT[ HitNo ].State = 0;

	HIT[ HitNo ].AnmNo = NOT;
	HIT[ HitNo ].FrameNo = NOT;

	for( short i=0; i<2; i++ )
	{
		HIT[ HitNo ].AREA.PosX[ i ] = 0;
		HIT[ HitNo ].AREA.PosY[ i ] = 0;
	}
}

/******************************************************************************/
//	名前	：	ヒットデータ全初期化
//	説明	：	ヒット構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	InitHitAll( void )
{
	ZeroMemory( HitFilePath, sizeof(HitFilePath) );

	for( long i=0; i<HIT_MAX; i++ )
	{
		InitHit( i );
	}
}

/******************************************************************************/
//	名前	：	ヒットファイル読み込み
//	説明	：	ヒットファイルの読み込みを行う
//	戻り値	：	なし
//	引数	：	[IN]char	*FileName	ヒットファイル名
//				[OUT]char	*LoadBuf	読み込み先のバッファ
//				[IN]long	HitNo		ヒット番号
//	備考	：	なし
/******************************************************************************/
void	LoadHitData( char *FileName, char *LoadBuf, long HitNo )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	LoadFile( FileName, LoadBuf );
	HLT[ HitNo ].Use = ON;
}

/******************************************************************************/
//	名前	：	ヒットセット
//	説明	：	ヒットデータの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	AnmNo		セットするアニメ番号
//				[IN]long	HitNo		当たり判定番号
//				[IN]long	FileNo		ファイル番号
//				[IN]long	HitDataNo	当たり判定のデータ番号
//				[IN]UChar	Flag		フラグ（0：停止、1：ループ）
//	備考	：	なし
/******************************************************************************/
void	SetHitData(long AnmNo,
				   long HitNo,
				   long FileNo,
				   long HitDataNo,
				   UChar Flag )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	HIT[ HitNo ].Use = ON;
	HIT[ HitNo ].AnmNo = AnmNo;
	HIT[ HitNo ].Flag = Flag;
	HIT[ HitNo ].State = ON;

	for( long i=0; i<2; i++ )
	{
		HIT[ HitNo ].AREA.PosX[ i ] = HLT[ FileNo ].HDT[ HitDataNo ].AREA.PosX[ i ];
		HIT[ HitNo ].AREA.PosY[ i ] = HLT[ FileNo ].HDT[ HitDataNo ].AREA.PosY[ i ];
	}
}

/******************************************************************************/
//	名前	：	当たり判定
//	説明	：	当り判定を行う
//	戻り値	：	short	判定結果	0(未接触)
//									1(接触)
//	引数	：	[IN]long	HitNo1	当たり判定領域１
//				[IN]long	HitNo2	当たり判定領域２
//	備考	：	なし
/******************************************************************************/
short	CheackHit( long HitNo1, long HitNo2 )
{
	if( HitNo1 >= HIT_MAX || HitNo2 >= HIT_MAX )
	{
		return 0;
	}

	short	Ret=0,
			x1,x2,x3,x4,y1,y2,y3,y4;

	if( !HIT[ HitNo1 ].Use || !HIT[ HitNo1 ].State
	|| !HIT[ HitNo2 ].Use || !HIT[ HitNo2 ].State )
	{
		return	Ret;
	}

	//	no1の領域
	long PolNo1 = HIT[ HitNo1 ].PolNo;
	x1 = (short)g_LibPol.GetInfo( PolNo1 ).PosX + HIT[ HitNo1 ].AREA.PosX[ 0 ];
	y1 = (short)g_LibPol.GetInfo( PolNo1 ).PosY + HIT[ HitNo1 ].AREA.PosY[ 0 ];
	x2 = (short)g_LibPol.GetInfo( PolNo1 ).PosX + HIT[ HitNo1 ].AREA.PosX[ 1 ];
	y2 = (short)g_LibPol.GetInfo( PolNo1 ).PosY + HIT[ HitNo1 ].AREA.PosY[ 1 ];

	//	no2の領域
	long PolNo2 = HIT[ HitNo2 ].PolNo;
	x3 = (short)g_LibPol.GetInfo( PolNo2 ).PosX + HIT[ HitNo2 ].AREA.PosX[ 0 ];
	y3 = (short)g_LibPol.GetInfo( PolNo2 ).PosY + HIT[ HitNo2 ].AREA.PosY[ 0 ];
	x4 = (short)g_LibPol.GetInfo( PolNo2 ).PosX + HIT[ HitNo2 ].AREA.PosX[ 1 ];
	y4 = (short)g_LibPol.GetInfo( PolNo2 ).PosY + HIT[ HitNo2 ].AREA.PosY[ 1 ];

	//	接触判定
	if( ( x1 <= x4 ) && ( x2 >= x3 )
	&& ( y1 <= y4 ) && ( y2 >= y3 ) )
	{
		Ret = 1;
	}

	return	Ret;
}

/******************************************************************************/
//	名前	：	当たり判定表示
//	説明	：	当り判定領域を表示する
//	戻り値	：	なし
//	引数	：	[IN]long	HitNo		当たり判定番号
//				[IN]long	DispPolNo	ポリゴン番号
//	備考	：	なし
/******************************************************************************/
void	DispHit( long HitNo, long DispPolNo )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	double	px,py;
	short	w,h;

	long PolNo = HIT[ HitNo ].PolNo;

	px = g_LibPol.GetInfo( PolNo ).PosX + HIT[ HitNo ].AREA.PosX[0];
	py = g_LibPol.GetInfo( PolNo ).PosY + HIT[ HitNo ].AREA.PosY[0];

	//	横サイズ
	if( HIT[ HitNo ].AREA.PosX[0] > HIT[ HitNo ].AREA.PosX[1] )
	{
		w = HIT[ HitNo ].AREA.PosX[0] - HIT[ HitNo ].AREA.PosX[1];
	}
	else
	{
		w = HIT[ HitNo ].AREA.PosX[1] - HIT[ HitNo ].AREA.PosX[0];
	}

	//	縦サイズ
	if( HIT[ HitNo ].AREA.PosY[0] > HIT[ HitNo ].AREA.PosY[1] )
	{
		h = HIT[ HitNo ].AREA.PosY[0] - HIT[ HitNo ].AREA.PosY[1];
	}
	else
	{
		h = HIT[ HitNo ].AREA.PosY[1] - HIT[ HitNo ].AREA.PosY[0];
	}

	g_LibPol.Set( DispPolNo, w, h, px,py,0.0, ATR_BOX );
	if( !HIT[ HitNo ].State )
	{
		g_LibPol.SetColor( DispPolNo, 128 );
	}
	else
	{
		g_LibPol.SetColor( DispPolNo, 255 );
	}
}

/******************************************************************************/
//	名前	：	当たり判定全表示
//	説明	：	当り判定領域を全て表示する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo	ポリゴン番号（最低数）
//	備考	：	なし
/******************************************************************************/
void	DispHitAll( long PolNo )
{
	for( long i=0; i<HIT_MAX; i++ )
	{
		DispHit( i, PolNo );
		PolNo ++;
	}
}

/******************************************************************************/
//	名前	：	当たり判定データ表示
//	説明	：	当り判定データを表示する
//	戻り値	：	なし
//	引数	：	[IN]long	HitNo	当たり判定番号
//				[IN]long	TextNo	表示に使うテキスト番号
//				[IN]double	PosX	表示座標X
//				[IN]double	PosY	表示座標Y
//	備考	：	なし
/******************************************************************************/
void	DispHitData( long HitNo, long TextNo, double PosX, double PosY )
{
	if( HitNo >= HIT_MAX )
	{
		return;
	}

	short	x1,x2,y1,y2,w,h;

	long PolNo = HIT[ HitNo ].PolNo;

	x1 = (short)g_LibPol.GetInfo( PolNo ).PosX + HIT[ HitNo ].AREA.PosX[0];
	y1 = (short)g_LibPol.GetInfo( PolNo ).PosY + HIT[ HitNo ].AREA.PosY[0];
	x2 = (short)g_LibPol.GetInfo( PolNo ).PosX + HIT[ HitNo ].AREA.PosX[1];
	y2 = (short)g_LibPol.GetInfo( PolNo ).PosY + HIT[ HitNo ].AREA.PosY[1];

	w = HIT[ HitNo ].AREA.PosX[1] - HIT[ HitNo ].AREA.PosX[0];
	h = HIT[ HitNo ].AREA.PosY[1] - HIT[ HitNo ].AREA.PosY[0];

	if( TXT[ TextNo ].Use == OFF )
	{
		g_LibText.Set( TextNo, PosX,PosY, 255,255,0,0, "" );
	}

	sprintf_s( TXT[ TextNo ].Str, MAX_PATH,
		"左上点(%d,%d)　右下点(%d,%d)　サイズ(%d,%d)", x1,y1, x2,y2, w,h );
}
