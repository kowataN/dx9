/******************************************************************************/
//	アニメライブラリ
//		メインソース
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	アニメテクスチャデータ
ST_AnmTexture	ATT[ANM_MAX];

//	アニメデータ
ST_AnmData		ANM[ANM_MAX];



/******************************************************************************/
//	名前	：	アニメ構造体初期化
//	説明	：	アニメ構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	AnmNo	アニメ番号
//	備考	：	なし
/******************************************************************************/
void	InitAnm( long AnmNo )
{
	if( AnmNo >= ANM_MAX )
	{
		return;
	}

	////////////////////////////////////
	//	フラグ

	//	使用フラグOFF
	ANM[ AnmNo ].Use = OFF;

	////////////////////////////////////
	//	番号

	//	ファイル番号
	ANM[ AnmNo ].FileNo = 0;
	//	フレーム番号
	ANM[ AnmNo ].FrmNo = 0;
	//	データ番号
	ANM[ AnmNo ].DataNo = 0;
	//	ポリゴン番号
	ANM[ AnmNo ].PolygonNo = 0;
	//	パターン番号
	ANM[ AnmNo ].PatNo = 0;

	////////////////////////////////////
	//	その他

	//	タイムカウンタ
	ANM[ AnmNo ].TimeCt = 0;
	//	アトリビュート
	ANM[ AnmNo ].Atr = ATR_NONE;
}

/******************************************************************************/
//	名前	：	アニメ構造体全初期化
//	説明	：	アニメ構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	InitAnmAll( void )
{
	//	構造体初期化
	for( long i=0; i<ANM_MAX; i++ )
	{
		InitAnm( i );
	}
}

/******************************************************************************/
//	名前	：	アニメファイル読み込み
//	説明	：	アニメファイルの読み込みを行う
//	戻り値	：	なし
//	引数	：	[IN]long	FileNo		ファイル番号
//				[IN]char	*FileName	読み込むアニメファイル
//				[IN]char	*Buf		読み込み先のバッファ
//				[IN]long	TextureNo	テクスチャ番号
//	備考	：	なし
/******************************************************************************/
void	LoadAnm( long FileNo, char *FileName, char *Buf, long TextureNo )
{
	if( FileNo >= ANM_MAX )
	{
		return;
	}

	long	i;
	char	*pt,file_path[MAX_PATH];

	/////////////////////////////////////////////////
	//	anmファイル読み込み
	if( LoadFile( FileName, Buf ) < 0 )
	{
		return;
	}

	/////////////////////////////////////////////////
	//	anmファイルか識別
	if( !( Buf[0] == 'A' && Buf[1] == 'N' && Buf[2] == 'M' ) )
	{
		return;
	}

	pt = (char *)Buf;
	pt += 3;

	/////////////////////////////////////////////////
	//	ファイル名取得

	//	ファイルの長さ
	long file_len = GetLong( pt );
	pt += 4;

	//	ファイル名
	for( i=0; i<file_len; i++ )
	{
		ATT[ FileNo ].FileName[ i ] = *pt;
		pt ++;
	}

	/////////////////////////////////////////////////
	//	ディレクトリ変更

	char	DirStr[MAX_PATH];
	PathToDir( FileName, DirStr );
	PathChange( DirStr, file_path );
	//	カレントディレクトリ・バックアップ
	char	name[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, name );

	/////////////////////////////////////////////////
	//	テクスチャ読み込み
	LoadTexture( TextureNo, ATT[ FileNo ].FileName );

	ATT[ FileNo ].TextureNo = TextureNo;

	//	元のカレントディレクトリに戻す
	_chdir( file_path );
}

/******************************************************************************/
//	名前	：	アニメセット
//	説明	：	アニメの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	AnmNo		セットするアニメ番号
//				[IN]long	FileNo		ファイル番号
//				[IN]long	DataNo		読み込んだファイルのアニメ番号
//				[IN]long	PatNo		登録するパターンの番号
//				[IN]char	*AnmAddr	アニメファイルのアドレス
//	備考	：	なし
/******************************************************************************/
void	SetAnm( long AnmNo, long FileNo, long DataNo, long PatNo, char *AnmAddr )
{
	if( AnmNo >= ANM_MAX || FileNo >= ANM_MAX || PatNo >= PAT_MAX )
	{
		return;
	}

	short	i;
	long	FileSize,Index[3];
	long	PatMax,AnmMax,HitMax;
	char	*pt;

	/////////////////////////////////////////////////
	//	使用フラグON
	ANM[ AnmNo ].Use	  = ON;

	/////////////////////////////////////////////////
	//	番号
	ANM[ AnmNo ].PatNo  = PatNo;
	ANM[ AnmNo ].DataNo = DataNo;

	pt = (char *)AnmAddr;
	ANM[ AnmNo ].AnmAddr = pt;

	/////////////////////////////////////////////////
	//	識別子スキップ
	pt += 3;

	/////////////////////////////////////////////////
	//	bmpファイル名の長さ取得
	FileSize = GetLong( pt );
	pt += 4;

	/////////////////////////////////////////////////
	//	bmpファイル名の長さ分アドレス加算
	pt += FileSize;

	/////////////////////////////////////////////////
	//	各最大数
	PatMax = GetLong( pt );	//	パターン
	pt += 4;
	AnmMax = GetLong( pt );	//	アニメ
	pt += 4;
	HitMax = GetLong( pt );	//	ヒット
	pt += 4;

	/////////////////////////////////////////////////
	//	各データの先頭アドレス
	Index[0] = GetLong( pt );
	pt += 4;
	Index[1] = GetLong( pt );
	pt += 4;
	Index[2] = GetLong( pt );
	pt += 4;

	/////////////////////////////////////////////////
	//	アニメデータ

	//	アニメデータの先頭アドレス
	pt = (char *)AnmAddr;
	pt += Index[1];

	if( DataNo >= AnmMax )
	{
		return;
	}

	for( i=0; i<DataNo; i++ )
	{
		//	フレーム最大数
		long FrmMax = GetLong( pt );
		pt += 4;
		pt += FrmMax * ANM_INDEX_SIZE;
	}

	//	フレーム最大数
	ANM[ AnmNo ].FrameMax = GetLong( pt );
	pt += 4;

	//	パターン番号
	ANM[ AnmNo ].FRM.PatNo = GetLong( pt );
	pt += 4;

	//	補正座標
	ANM[ AnmNo ].FRM.CorrectX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.CorrectY = GetDouble( pt );
	pt +=8;

	//	拡大率
	ANM[ AnmNo ].FRM.ScaleX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.ScaleY = GetDouble( pt );
	pt +=8;

	//	角度
	ANM[ AnmNo ].FRM.Rot = GetDouble( pt );
	pt +=8;

	//	表示時間
	ANM[ AnmNo ].FRM.Time = GetLong( pt );
	pt +=4;

	//	不透明度
	ANM[ AnmNo ].FRM.A = GetDouble( pt );
	pt +=8;

	//	各色成分
	ANM[ AnmNo ].FRM.R = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.G = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.B = GetDouble( pt );
	pt +=8;

	//	アトリビュート
	ANM[ AnmNo ].FRM.Rev = GetShort( pt );
	pt +=2;

	/////////////////////////////////////////////////
	//	パターンデータ

	//	アドレス
	pt = (char *)AnmAddr;
	pt += Index[0];

	short	x,y,w,h;

	pt += ( ANM[ AnmNo ].FRM.PatNo * PAT_INDEX_SIZE );

	//	x
	x = GetShort( pt );
	pt += 2;
	//	y
	y = GetShort( pt );
	pt += 2;
	//	w
	w = GetShort( pt );
	pt += 2;
	//	h
	h = GetShort( pt );
	pt += 2;

	//	パターン作成
	SetPattern( PatNo, ATT[FileNo].TextureNo,  x, y, w, h );
}

/******************************************************************************/
//	名前	：	アニメデータのセット
//	説明	：	アニメデータの設定を行う
//	戻り値	：	なし
//	引数	：	[IN]long	AnmNo		アニメ番号
//	備考	：	なし
/******************************************************************************/
void	SetAnmData( long AnmNo )
{
	short	i;
	long	file_size,index[3],pat_no,
			pat_max,anm_max,hit_max;
	char	*pt;

	pt = (char *)ANM[ AnmNo ].AnmAddr;
	pt += 3;

	/////////////////////////////////////////////////
	//	bmpファイル名の長さ取得
	file_size = GetLong( pt );
	pt += 4;

	/////////////////////////////////////////////////
	//	bmpファイル名の長さ分アドレス加算
	pt += file_size;

	/////////////////////////////////////////////////
	//	各データの最大数
	pat_max = GetLong( pt );	//	パターン
	pt += 4;
	anm_max = GetLong( pt );	//	アニメ
	pt += 4;
	hit_max = GetLong( pt );	//	ヒット
	pt += 4;

	/////////////////////////////////////////////////
	//	各データの先頭アドレス
	index[0] = GetLong( pt );	//	パターン
	pt += 4;
	index[1] = GetLong( pt );	//	アニメ
	pt += 4;
	index[2] = GetLong( pt );	//	ヒット
	pt += 4;

	/////////////////////////////////////////////////
	//	アニメデータ

	//	アニメデータの先頭アドレス
	pt = (char *)ANM[ AnmNo ].AnmAddr;
	pt += index[1];

	for( i=0; i<ANM[AnmNo].DataNo; i++ )
	{
		//	フレーム最大数
		long frm_max = GetLong( pt );
		pt += 4;
		pt += frm_max * ANM_INDEX_SIZE;
	}

	//	フレーム最大数
	ANM[ AnmNo ].FrameMax = GetLong( pt );
	pt += 4;
	pt += ANM_INDEX_SIZE * ANM[ AnmNo ].FrmNo;

	//	パターン番号
	ANM[ AnmNo ].FRM.PatNo = GetLong( pt );
	pt += 4;

	//	補正座標
	ANM[ AnmNo ].FRM.CorrectX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.CorrectY = GetDouble( pt );
	pt +=8;

	//	拡大率
	ANM[ AnmNo ].FRM.ScaleX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.ScaleY = GetDouble( pt );
	pt +=8;

	//	角度
	ANM[ AnmNo ].FRM.Rot = GetDouble( pt );
	pt +=8;

	//	表示時間
	ANM[ AnmNo ].FRM.Time = GetLong( pt );
	pt +=4;

	//	不透明度
	ANM[ AnmNo ].FRM.A = GetDouble( pt );
	pt +=8;

	//	各色成分
	ANM[ AnmNo ].FRM.R = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.G = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.B = GetDouble( pt );
	pt +=8;

	//	アトリビュート
	ANM[ AnmNo ].FRM.Rev = GetShort( pt );
	pt +=2;

	/////////////////////////////////////////////////
	//	パターンデータ

	//	アドレス
	pt = (char *)ANM[ AnmNo ].AnmAddr;
	pt += index[0];

	pt += ( ANM[ AnmNo ].FRM.PatNo * PAT_INDEX_SIZE );

	pat_no = ANM[ AnmNo ].PatNo;

	//	x
	ANM[ AnmNo ].PAT.PosX = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].PosX = ANM[ AnmNo ].PAT.PosX;

	//	y
	ANM[ AnmNo ].PAT.PosY = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].PosY = ANM[ AnmNo ].PAT.PosY;

	//	w
	ANM[ AnmNo ].PAT.Width = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].Width = ANM[ AnmNo ].PAT.Width;

	//	h
	ANM[ AnmNo ].PAT.Height = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].Height = ANM[ AnmNo ].PAT.Height;
}

/******************************************************************************/
//	名前	：	アニメのループ再生
//	説明	：	アニメの単体での再生
//	戻り値	：	なし
//	引数	：	[IN]long	AnmNo		アニメーションするアニメ番号
//	備考	：	なし
/******************************************************************************/
void	LoopAnmOne( long AnmNo )
{
	if( AnmNo >= ANM_MAX )
	{
		return;
	}

	char	*pt;

	if( ANM[ AnmNo ].TimeCt >= ANM[ AnmNo ].FRM.Time )
	{
		//	タイムカウンタ初期化
		ANM[ AnmNo ].TimeCt = 0;

		//	次のフレーム
		ANM[ AnmNo ].FrmNo ++;
		if( ANM[ AnmNo ].FrmNo >= ANM[ AnmNo ].FrameMax )
		{
			switch( ANM[ AnmNo ].Flag )
			{
				//	ループ
				case	ANM_LOOP:
					ANM[ AnmNo ].State = 1;
					ANM[ AnmNo ].FrmNo = 0;
					break;

				//	停止
				case	ANM_STOP:
					ANM[ AnmNo ].State = 0;
					ANM[ AnmNo ].FrmNo = ANM[ AnmNo ].FrameMax - 1;
					break;

				//	終了
				case	ANM_END:
					ANM[ AnmNo ].State = 0;
					g_LibPol.ChangeDisp( ANM[ AnmNo ].PolygonNo, OFF );
					break;
			}
		}

		SetAnmData( AnmNo );
	}
	else
	{
		/////////////////////////////////////
		//	拡大・回転・色成分・補正座標
		double	sx1,sx2,sy1,sy2;
		double rot1,rot2;
		double	a1,r1,g1,b1;
		double	a2,r2,g2,b2;

		//	拡大・回転
		sx1 = ANM[ AnmNo ].FRM.ScaleX;
		sy1 = ANM[ AnmNo ].FRM.ScaleY;
		rot1 = ANM[ AnmNo ].FRM.Rot;

		//	不透明度・各色成分
		a1 = ANM[ AnmNo ].FRM.A;
		r1 = ANM[ AnmNo ].FRM.R;
		g1 = ANM[ AnmNo ].FRM.G;
		b1 = ANM[ AnmNo ].FRM.B;

		if( ANM[ AnmNo ].FrmNo + 1 >= ANM[ AnmNo ].FrameMax )
		{
			//	現フレームデータのコピー
			sx2 = sx1;
			sy2 = sy1;
			rot2 = rot1;
			a2 = a1;
			r2 = r1;
			g2 = g1;
			b2 = b1;
		}
		else
		{
			pt = ANM[ AnmNo ].AnmAddr;
			pt += 3;	//	識別
			pt += GetLong( pt );	pt += 4;	//	bmpファイル名の長さ取得
			pt += 4 * 3;	//	各データの最大数
			pt += 4;	//	パターンデータ先頭アドレス
			long	index = GetLong( pt );	pt += 4;	//	アニメデータ先頭アドレス
			pt += 4;	//	ヒットデータ先頭アドレス

			/////////////////////////////////////////////////
			//	アニメデータの先頭アドレス
			pt = (char *)ANM[ AnmNo ].AnmAddr;
			pt += index;

			for( int i=0; i<ANM[ AnmNo ].DataNo; i++ )
			{
				//	フレーム最大数
				long frm_max = GetLong( pt );pt += 4;
				pt += frm_max * ANM_INDEX_SIZE;
			}

			pt += ANM_INDEX_SIZE * (ANM[ AnmNo ].FrmNo+1);
			pt += 4+4+16;

			//	次のフレームデータのコピー
			sx2 = GetDouble( pt );	pt += 8;
			sy2 = GetDouble( pt );	pt += 8;
			rot2 = GetDouble( pt );	pt += 8+4;
			a2 = GetDouble( pt );	pt += 8;
			r2 = GetDouble( pt );	pt += 8;
			g2 = GetDouble( pt );	pt += 8;
			b2 = GetDouble( pt );	pt += 8;
		}

		//	見やすいように代入
		ST_PolygonInfo Info = g_LibPol.GetInfo( ANM[ AnmNo ].PolygonNo );

		//	反転
		ANM[ AnmNo ].Atr = ANM[ AnmNo ].FRM.Rev;
		Info.Atr |= ANM[ AnmNo ].Atr;

		//	拡大率
		Info.ScaleX += ( sx2 - sx1 ) / ANM[ AnmNo ].FRM.Time;
		Info.ScaleY += ( sy2 - sy1 ) / ANM[ AnmNo ].FRM.Time;

		//	回転角度
		Info.RotZ += ( rot2 - rot1 ) / ANM[ AnmNo ].FRM.Time;

		//	不透明度
		Info.A += (UShort)( ( a2 - a1 ) / ANM[ AnmNo ].FRM.Time );

		//	各色成分
		Info.R += (UShort)( ( r2 - r1 ) / ANM[ AnmNo ].FRM.Time );
		Info.G += (UShort)( ( g2 - g1 ) / ANM[ AnmNo ].FRM.Time );
		Info.B += (UShort)( ( b2 - b1 ) / ANM[ AnmNo ].FRM.Time );

		//	補正座標
		Info.CenterX = (short)ANM[ AnmNo ].FRM.CorrectX;
		Info.CenterY = (short)ANM[ AnmNo ].FRM.CorrectY;

		g_LibPol.SetInfo( ANM[ AnmNo ].PolygonNo, Info );
	}

	//	カウンタ加算
	ANM[ AnmNo ].TimeCt ++;
}

/******************************************************************************/
//	名前	：	アニメポリゴンにセット
//	説明	：	アニメをポリゴンに設定する
//	戻り値	：	なし
//	引数	：	[IN]long	PolNo		ポリゴン番号
//				[IN]long	AnmNo		アニメ番号
//				[IN]double	PosX		X座標
//				[IN]double	PosY		Y座標
//				[IN]double	PosZ		Z座標
//				[IN]UChar	Flag		再生フラグ
//	備考	：	なし
/******************************************************************************/
void	SetAnmPolygon(long PolNo,
					  long AnmNo,
					  double PosX, double PosY, double PosZ,
					  UChar Flag )
{
	if( PolNo >= POL_MAX || AnmNo >= ANM_MAX )
	{
		return;
	}

	long	pat;

	//	ポリゴン番号
	ANM[ AnmNo ].PolygonNo = PolNo;

	//	パターン番号
	pat = ANM[ AnmNo ].PatNo;

	//	アトリビュート
	ANM[ AnmNo ].Atr = ANM[ AnmNo ].FRM.Rev;

	//	ポリゴンセット
	g_LibPol.SetPat( PolNo, pat, PosX, PosY, PosZ, ANM[ AnmNo ].Atr );
	//	中心座標
	g_LibPol.SetCenter( PolNo, (short)ANM[ AnmNo ].FRM.CorrectX, (short)ANM[ AnmNo ].FRM.CorrectY );
	//	拡大
	g_LibPol.SetScale( PolNo, ANM[ AnmNo ].FRM.ScaleX );
	//	回転
	g_LibPol.SetRotZ( PolNo, ANM[ AnmNo ].FRM.Rot );
	//	不透明度
	g_LibPol.SetColor( PolNo,
		(UChar)ANM[ AnmNo ].FRM.A,
		(UChar)ANM[ AnmNo ].FRM.R,
		(UChar)ANM[ AnmNo ].FRM.G,
		(UChar)ANM[ AnmNo ].FRM.B );

	//	フラグ
	ANM[ AnmNo ].Flag = Flag;

	//	状態
	ANM[ AnmNo ].State = 1;

	//	アニメループ単体
	LoopAnmOne( AnmNo );
}

/******************************************************************************/
//	名前	：	アニメの全ループ再生
//	説明	：	アニメの再生
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	LoopAnm( void )
{
	for( long i=0; i<ANM_MAX; i++ )
	{
		if( !ANM[i].Use || !ANM[i].State )
		{
			continue;
		}
		LoopAnmOne( i );
	}
}

/******************************************************************************/
//	名前	：	アニメデータコピー
//	説明	：	アニメデータのコピーを行う
//	戻り値	：	なし
//	引数	：	[IN]long	AnmNo		コピー先アニメ番号
//				[IN]long	CopyAnmNo	コピー元アニメ番号
/******************************************************************************/
void	CopyAnmData( long AnmNo, long CopyAnmNo )
{
	if( AnmNo >= ANM_MAX || CopyAnmNo >= ANM_MAX )
	{
		return;
	}

	ANM[ AnmNo ] = ANM[ CopyAnmNo ];
}

