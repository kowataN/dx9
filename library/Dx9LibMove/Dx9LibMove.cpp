/******************************************************************************/
//	ムーブライブラリ
//		メインソース
/******************************************************************************/


//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibMove.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	ムーブデータ
ST_MoveData		MOV[MOV_MAX];



/******************************************************************************/
//	名前	：	ムーブデータ初期化
//	説明	：	ムーブ構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	MoveNo		ムーブ番号
//	備考	：	なし
/******************************************************************************/
void	InitMove( long MoveNo )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	//	使用フラグOFF
	MOV[ MoveNo ].Use = OFF;

	//	フレーム番号
	MOV[ MoveNo ].FrameNo = 0;

	//	ポリゴン番号
	MOV[ MoveNo ].PolNo = NOT;

	//	タイムカウンタ
	MOV[ MoveNo ].TimeCount = 0;
}

/******************************************************************************/
//	名前	：	ムーブデータ全初期化
//	説明	：	ムーブ構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	InitMoveAll( void )
{
	//	構造体初期化
	for( long i=0; i<MOV_MAX; i++ )
	{
		InitMove( i );
	}
}

/******************************************************************************/
//	名前	：	ムーブファイル読み込み
//	説明	：	ムーブファイルの読み込みを行う
//	戻り値	：	なし
//	引数	：	[IN]char	FileName	読み込むmovファイル
//				[OUT]char	*LoadBuf	読み込み先のバッファ
//	備考	：	なし
/******************************************************************************/
void	LoadMoveFile( char *FileName, char *LoadBuf )
{
	//	movファイル読み込み
	if( LoadFile( FileName, LoadBuf ) < 0 )
	{
		return;
	}
}

/******************************************************************************/
//	名前	：	ムーブセット
//	説明	：	ムーブデータの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	MoveNo		セットするムーブ番号
//				[IN]long	PolNo		ポリゴン番号
//				[IN]long	DataNo		読み込んだファイルのムーブ番号
//				[IN]char	*DataAddr	データのアドレス
//	備考	：	なし
/******************************************************************************/
void	SetMove( long MoveNo, long PolNo, long DataNo, char *DataAddr )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	short	i;

	char	*pt = (char *)DataAddr;
	MOV[ MoveNo ].DataAddr = pt;

	for( i=0; i<DataNo; i++ )
	{
		//	フレーム最大数
		long FrmMax = GetLong( pt );
		pt += 4;
		pt += FrmMax * MOV_INDEX_SIZE;
	}

	//	フレーム最大数
	MOV[ MoveNo ].FrameMax = GetLong( pt );
	pt += 4;

	//	移動量
	MOV[ MoveNo ].FRM.MoveX = GetShort( pt );
	pt += 2;
	MOV[ MoveNo ].FRM.MoveY = GetShort( pt );
	pt += 2;

	//	時間
	MOV[ MoveNo ].FRM.Time = GetShort( pt );
	pt += 2;

	MOV[ MoveNo ].Use = ON;
	MOV[ MoveNo ].PolNo = PolNo;
	MOV[ MoveNo ].State = ON;
	MOV[ MoveNo ].ExecFlag = MOVE_LOOP;
	MOV[ MoveNo ].FrameNo = 0;
	MOV[ MoveNo ].DataNo = DataNo;
}

/******************************************************************************/
//	名前	：	ムーブデータセット
//	説明	：	ムーブデータの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	MoveNo		ムーブ番号
//	備考	：	なし
/******************************************************************************/
void	SetMoveData( long MoveNo )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	short	i;

	//	先頭アドレス
	char *pt = (char *)MOV[ MoveNo ].DataAddr;
	for( i=0; i<MOV[MoveNo].DataNo; i++ )
	{
		//	フレーム最大数
		long FrmMax = GetLong( pt );
		pt += 4;
		pt += FrmMax * MOV_INDEX_SIZE;
	}

	//	フレーム最大数
	MOV[ MoveNo ].FrameMax = GetLong( pt );
	pt += 4;
	pt += MOV_INDEX_SIZE * MOV[ MoveNo ].FrameNo;

	//	移動量
	MOV[ MoveNo ].FRM.MoveX = GetShort( pt );
	pt += 2;
	MOV[ MoveNo ].FRM.MoveY = GetShort( pt );
	pt += 2;

	//	時間
	MOV[ MoveNo ].FRM.Time = GetShort( pt );
	pt += 2;
}

/******************************************************************************/
//	名前	：	ムーブ再生
//	説明	：	ムーブの再生を行う
//	戻り値	：	なし
//	引数	：	[IN]long	MoveNo		ムーブ番号
//	備考	：	なし
/******************************************************************************/
void	LoopMoveOne( long MoveNo )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	if( !MOV[ MoveNo ].Use || !MOV[ MoveNo ].State )
	{
		return;
	}

	if( MOV[ MoveNo ].TimeCount >= MOV[ MoveNo ].FRM.Time )
	{
		//	タイムカウンタ初期化
		MOV[ MoveNo ].TimeCount = 0;

		//	次のフレーム
		MOV[ MoveNo ].FrameNo ++;
		if( MOV[ MoveNo ].FrameNo >= MOV[ MoveNo ].FrameMax )
		{
			switch( MOV[ MoveNo ].ExecFlag )
			{
				//	ループ
				case	MOVE_LOOP:
					MOV[ MoveNo ].State = 1;
					MOV[ MoveNo ].FrameNo = 0;
					break;

				//	停止
				case	MOVE_STOP:
					MOV[ MoveNo ].State = 0;
					MOV[ MoveNo ].FrameNo = MOV[ MoveNo ].FrameMax - 1;
					break;

				//	終了
				case	MOVE_END:
					MOV[ MoveNo ].State = 0;
					break;
			}
		}

		SetMoveData( MoveNo );
	}

	//	移動
	double MoveX = MOV[ MoveNo ].FRM.MoveX;
	double MoveY = MOV[ MoveNo ].FRM.MoveY;

	long PolNo = MOV[ MoveNo ].PolNo;
	g_LibPol.AddPosX( PolNo, MoveX / MOV[ MoveNo ].FRM.Time );
	g_LibPol.AddPosY( PolNo, MoveY / MOV[ MoveNo ].FRM.Time );

	//	カウンタ加算
	MOV[ MoveNo ].TimeCount ++;
}

/******************************************************************************/
//	名前	：	ムーブ全再生
//	説明	：	ムーブの全再生を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	LoopMove( void )
{
	for( long i=0; i<MOV_MAX; i++ )
	{
		LoopMoveOne( i );
	}
}
