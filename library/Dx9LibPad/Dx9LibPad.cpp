/******************************************************************************/
//	パッドライブラリ
//		メインファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibPad.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	キーボード初期化
short	SetKeyDat[PAD_MAX][PAD_BUTTON_MAX] =
{
	//	上 下         左        右         ①           ②           ③           ④            スタート
	DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_NUMPAD1, DIK_NUMPAD2, DIK_NUMPAD3, DIK_NUMPAD4,  156,
	DIK_W,  DIK_Z,    DIK_A,    DIK_S,     DIK_C,       DIK_V,       DIK_B,       DIK_N,        DIK_RETURN,
};

//	ジョイスティック初期化
short	SetJoyDat[PAD_MAX][PAD_BUTTON_MAX] = 
{
	//	上  下        左        右         ①        ②        ③        ④        スタート
	JOY_UP, JOY_DOWN, JOY_LEFT, JOY_RIGHT, JOY_BUT1, JOY_BUT2, JOY_BUT3, JOY_BUT4, JOY_BUT5,
	JOY_UP, JOY_DOWN, JOY_LEFT, JOY_RIGHT, JOY_BUT1, JOY_BUT2, JOY_BUT3, JOY_BUT4, JOY_BUT5,
};

//	パッド構造体
ST_PadData	PAD[PAD_MAX];



/******************************************************************************/
//	名前	：	パッド初期化
//	説明	：	パッド構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PadNo		パッド番号
//	備考	：	なし
/******************************************************************************/
void	InitPad( long PadNo )
{
	if( PadNo >= PAD_MAX )
	{
		return;
	}

	PAD[PadNo].Use = OFF;
	PAD[PadNo].JoyUse = OFF;

	for( long i=0; i<PAD_BUTTON_MAX; i++ )
	{
		PAD[PadNo].Trg[i] = OFF;
		PAD[PadNo].TrgBk[i] = OFF;
		PAD[PadNo].Push[i] = OFF;
		PAD[PadNo].PushBk[i] = OFF;
		PAD[PadNo].Rpt[i] = 0;
		PAD[PadNo].Count[i] = 0;

		PAD[PadNo].KeyNo[i] = SetKeyDat[PadNo][i];
		PAD[PadNo].JoyNo[i] = SetJoyDat[PadNo][i];
	}
}

/******************************************************************************/
//	名前	：	パッド初期化
//	説明	：	パッド構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	InitPadAll( void )
{
	for( long i=0; i<PAD_MAX; i++ )
	{
		InitPad( i );
	}
}

/******************************************************************************/
//	名前	：	パッドループ
//	説明	：	パッドのループ処理を行う
//	戻り値	：	なし
//	引数	：	[IN]long	PadNo		パッド番号
//	備考	：	なし
/******************************************************************************/
void	LoopPadOne( long PadNo )
{
	if( PadNo >= PAD_MAX )
	{
		return;
	}

	long	i;

	//	使用フラグ確認
	if( PAD[PadNo].Use == OFF )
	{
		return;
	}

	///////////////////////////////
	//	バックアップ
	for( i=0; i<PAD_BUTTON_MAX; i++ )
	{
		//	トリガ
		PAD[PadNo].TrgBk[i] = PAD[PadNo].Trg[i];

		//	プッシュ
		PAD[PadNo].PushBk[i] = PAD[PadNo].Push[i];

		//	初期化
		PAD[PadNo].Trg[i] = OFF;
		PAD[PadNo].Push[i] = OFF;
	}

	///////////////////////////////
	//	キーボードの判定
	for( i=0; i<PAD_BUTTON_MAX; i++ )
	{
		//	一時格納
		long KeyNo = PAD[PadNo].KeyNo[i];

		//	トリガ
		PAD[PadNo].Trg[i] |= KEY.Trg[KeyNo];

		//	プッシュ
		PAD[PadNo].Push[i] |= KEY.Push[KeyNo];
	}

	///////////////////////////////
	//	ジョイスティックの判定
	if( PAD[PadNo].JoyUse )
	{
		//	方向キー
		for( i=0; i<PAD_BUTTON_MAX; i++ )
		{
			long	JoyNo = PAD[PadNo].JoyNo[i];

			//	トリガ
			PAD[PadNo].Trg[i] |= JOY[PadNo].Trg[JoyNo];

			//	プッシュ
			PAD[PadNo].Push[i] |= JOY[PadNo].Push[JoyNo];
		}
	}

	///////////////////////////////
	//	カウンタ、キーリピート
	for( i=0; i<PAD_BUTTON_MAX; i++ )
	{
		//	カウンタ
		if( PAD[PadNo].Push[i] )
		{
			PAD[PadNo].Count[i] ++;
		}
		else
		{
			PAD[PadNo].Count[i] = 0;
		}

		//	キーリピート
		if( ( PAD[PadNo].Trg[i] )
		|| ( ( PAD[PadNo].Count[i] >= KEY_REPEAT_START )
		&& ( PAD[PadNo].Count[i] % KEY_REPEAT_TIME == 0 ) ) )
		{
			PAD[PadNo].Rpt[i] = ON;
		}
		else
		{
			PAD[PadNo].Rpt[i] = OFF;
		}
	}
}

/******************************************************************************/
//	名前	：	パッド全ループ
//	説明	：	パッドの全ループ処理を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	LoopPad( void )
{
	for( long i=0; i<PAD_MAX; i++ )
	{
		LoopPadOne( i );
	}
}

/******************************************************************************/
//	名前	：	パッドデータ読み込み
//	説明	：	パッッドデータの読み込みを行う
//	戻り値	：	なし
//	引数	：	[IN]char	*FileName	読み込むファイル名
/******************************************************************************/
void	LoadPadData( char *FileName )
{
	char	LoadBuf[1024];

	//	ファイル読み込み
	if( LoadFile( FileName, LoadBuf ) < 0 )
	{
		return;
	}

	//	データ取得
	char	*pt = (char*)LoadBuf;
	long	PadMax = GetLong( pt );
	pt += 4;

	for( long PadNo=0; PadNo<PadMax; PadNo++ )
	{
		for( long ButNo=0; ButNo<PAD_BUTTON_MAX; ButNo++ )
		{
			SetJoyDat[ PadNo ][ ButNo ] = GetShort( pt );
			pt += 2;
		}
	}
}
