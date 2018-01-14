/******************************************************************************/
//	DirectX9ライブラリ
//		コモン
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//

//	汎用ワークエリア
char	g_FreeWork[FREE_WORK_SIZE];



/******************************************************************************/
//	名前	：	ファイルロード
//	説明	：	ファイルを読み込む
//	戻り値	：	long		処理結果	-1(失敗)
//										-1以外(ファイルサイズ)
//	引数	：	[IN]char	*FileName	ファイル名
//				[OUT]char	*LoadBuf	読み込みアドレス
//	備考	：	なし
/******************************************************************************/
long	LoadFile( char *FileName, char *LoadBuf )
{
	long	Ret = -1;
	FILE	*fp;
	errno_t	Err;
	int		Index = 0;
	struct	stat St;

	//	ファイル情報取得
	Index = stat( FileName, &St );
	if( Index != 0 )
	{
		//	取得失敗
		return	Ret;
	}

	for( int i=0; i<10; i++ )
	{
		//	ファイルオープン
		Err = fopen_s( &fp, FileName, "rb" );
		if( Err == 0 )
		{
			//	ファイルオープンに成功
			if( fseek( fp, 0, SEEK_SET ) == 0 )
			{
				//	ファイルロード
				if( fread( LoadBuf, St.st_size, 1, fp ) >= 1 )
				{
					Ret = St.st_size;
				}
			}
		}

		//	ファイルクローズ
		fclose( fp );
		if( Ret >= 0 )
		{
			break;
		}
	}

	return Ret;
}

/******************************************************************************/
//	名前	：	ファイルセーブ
//	説明	：	ファイルを保存する
//	戻り値	：	long		処理結果	1(成功)
//										0(失敗)
//	引数	：	[IN]char	*FileName	ファイル名
//				[IN]char	*SaveBuf	読み込みアドレス
//				[IN]long	size		サイズ
//	備考	：	なし
/******************************************************************************/
long	SaveFile( char *FileName, char *SaveBuf, long Size )
{
	long	Ret,ct;
	FILE	*fp;
	errno_t	Err;

	Ret = 0;
	for( int i=0; i<10; i++ )
	{
		//	ファイルオープン
		Err = fopen_s( &fp, FileName, "wb" );
		if( Err == 0 )
		{
			fseek( fp, 0, SEEK_SET );

			ct = 0;
			while( 1 )
			{
				if( Size == 0 )
				{
					break;
				}
				
				if( Size < 256 )
				{
					fwrite( &SaveBuf[ct], Size, 1, fp );
					break;
				}

				//	セーブ
				fwrite( &SaveBuf[ct], Size, 1, fp );
				ct += 256;
				Size -= 256;
				fseek( fp, ct, SEEK_SET );
			}

			Ret = 1;
		}

		//	ファイルクローズ
		fclose( fp );

		if( Ret == 1 )
		{
			break;
		}
	}

	if( Ret < 0 )
	{
		Ret = 0;
	}

	return Ret;
}

/******************************************************************************/
//	名前	：	パスからファイル名取得
//	説明	：	パスからファイル名を取得する
//	戻り値	：	なし
//	引数	：	[IN]char	*Path		フルパス
//				[OUT]char	*FileName	格納するファイル名
//	備考	：	なし
/******************************************************************************/
void	PathToName( char *Path, char *FileName )
{
	char	*pt = strrchr( Path, 0x5c );
	if( pt == NULL )
	{
		wsprintf( FileName, Path );
	}
	else
	{
		wsprintf( FileName, (pt+1) );
	}
}

/******************************************************************************/
//	名前	：	パスから拡張子取得
//	説明	：	パスから拡張子を取得する
//	戻り値	：	なし
//	引数	：	[IN]char	*Path		フルパス
//				[OUT]char	*FileName	格納拡張子
//	備考	：	なし
/******************************************************************************/
void	PathToOpt( char *Path, char *FileName )
{
	// 後ろから１個目の'.'
	char	*pt = strrchr( Path, '.' );
	if( pt == NULL )
	{
		wsprintf( FileName, "" );
	}
	else
	{
		wsprintf( FileName, (pt+1) );
	}
}

/******************************************************************************/
//	名前	：	パスからディレクトリ抽出
//	説明	：	パスからディレクトリを抽出する
//	戻り値	：	なし
//	引数	：	[IN]char	*Path	フルパス
//				[OUT]char	*Dir	格納ディレクトリ
//	備考	：	なし
/******************************************************************************/
void	PathToDir( char *Path, char *Dir )
{
	char	*pt;
	// 後ろから１個目の'\'
	pt = strrchr( Path, 0x5c );
	if( pt == NULL )
	{
		wsprintf( Dir, "" );
	}
	else
	{
		wsprintf( Dir, Path );
		*(Dir + (pt - Path)) = 0;
	}
}

/******************************************************************************/
//	名前	：	パス変更
//	説明	：	作業用パスを変更する
//	戻り値	：	なし
//	引数	：	[IN]char	*NewPath		フルパス
//				[OUT]char	*BackupPath		変更前のディレクトリ格納アドレス
//	備考	：	なし
/******************************************************************************/
void	PathChange( char *NewPath, char *BakupPath )
{
	//char	FilePath[MAX_PATH];

	//	カレントディレクトリ・バックアップ
	GetCurrentDirectory( MAX_PATH, BakupPath );
	//	ディレクトリ抽出
	//PathToDir( NewPath, FilePath );
	//	ディレクトリ変更
	SetCurrentDirectory( NewPath );
}

/******************************************************************************/
//	名前	：	暗号化
//	説明	：	暗号化を行う
//	戻り値	：	long	サイズ
//	引数	：	[IN/OUT]char	*FileName	暗号化するファイルのポインタ
//	備考	：	なし
/******************************************************************************/
long	Secret( char *FileName )
{
	char	Rnd = (char)137;
	long	Size = LoadFile( FileName, g_FreeWork );
	UChar	*pt = (UChar *)g_FreeWork;

	if( Size > 0 )
	{
		//	暗号化
		for( int i=0; i<Size; i++ )
		{
			Rnd = Rnd * 5 - 3;
			*pt = *pt + Rnd;
			pt++;
		}
	}

	return Size;
}

/******************************************************************************/
//	名前	：	暗号ファイルのセーブ
//	説明	：	暗号後にファイルを保存する
//	戻り値	：	なし
//	引数	：	[OUT]char	*FileName		暗号ファイル
//				[IN]char	SaveFileName	保存ファイル名
//	備考	：	なし
/******************************************************************************/
void	SaveSecret( char *FileName, char *SaveFileName )
{
	long Size = Secret( FileName );
	SaveFile( SaveFileName, FileName, Size );
}

/******************************************************************************/
//	名前	：	暗号ファイルの解読
//	説明	：	暗号化されたファイルを解読する
//	戻り値	：	long	読み込んだファイルのサイズ
//	引数	：	[IN]char	*SecretFile	解読するファイル
//				[OUT]char	*FileName	解読先アドレス
//	備考	：	なし
/******************************************************************************/
long	AntiSecret( char *SecretFile, char *FileName )
{
	char	Rnd = (char)137;
	long	Size = LoadFile( SecretFile, FileName );
	UChar	*pt = (UChar *)FileName;

	if( Size > 0 )
	{
		//	解読化
		for( int i=0; i<Size; i++ )
		{
			Rnd = Rnd * 5 - 3;
			*pt = *pt - Rnd;
			pt++;
		}
	}

	return	Size;
}

/******************************************************************************/
//	名前	：	short型をポインタへ
//	説明	：	short型のデータを1バイトづつ格納する
//	戻り値	：	なし
//	引数	：	[IN]short		dat		ポインタに追加する変数
//				[IN/OUT]char	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
void	SetShort( short Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<2; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	名前	：	long型をポインタへ
//	説明	：	long型のデータを1バイトづつ格納する
//	戻り値	：	なし
//	引数	：	[IN]long		dat		ポインタに追加する変数
//				[IN/OUT]char	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
void	SetLong( long Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<4; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	名前	：	float型をポインタへ
//	説明	：	float型のデータを1バイトづつ格納する
//	戻り値	：	なし
//	引数	：	[IN]float		dat		ポインタに追加する変数
//				[IN/OUT]char	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
void	SetFloat( float Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<4; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	名前	：	double型をポインタへ
//	説明	：	double型のデータを1バイトづつ格納する
//	戻り値	：	なし
//	引数	：	[IN]double		dat		ポインタに追加する変数
//				[IN/OUT]double	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
void	SetDouble( double Dat, char *pt )
{
	char	*p = (char *)&Dat;
	for( int i=0; i<8; i++ )
	{
		*pt = *p;
		p ++;
		pt ++;
	}
}

/******************************************************************************/
//	名前	：	short型取得
//	説明	：	short型のデータを取得する
//	戻り値	：	short	short型のデータ
//	引数	：	[IN]char	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
short	GetShort( char *pt )
{
	union
	{
		char	dat[2];
		short	data;
	}SRT;

	for( int i=0; i<2; i++ )
	{
		SRT.dat[ i ] = *( pt + i );
	}
	return	SRT.data;
}

/******************************************************************************/
//	名前	：	long型取得
//	説明	：	long型のデータを取得する
//	戻り値	：	long	long型のデータ
//	引数	：	[IN]char	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
long	GetLong( char *pt )
{
	union
	{
		char	dat[4];
		long	data;
	}LNG;

	for( int i=0; i<4; i++ )
	{
		LNG.dat[ i ] = *( pt + i );
	}

	return	LNG.data;
}

/******************************************************************************/
//	名前	：	float型取得
//	説明	：	float型のデータを取得する
//	戻り値	：	float	float型のデータ
//	引数	：	[IN]char	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
float	GetFloat( char *pt )
{
	union
	{
		char	dat[4];
		float	data;
	}FLT;

	for( int i=0; i<4; i++ )
	{
		FLT.dat[ i ] = *( pt + i );
	}

	return	FLT.data;
}

/******************************************************************************/
//	名前	：	double型取得
//	説明	：	double型のデータを取得する
//	戻り値	：	double	double型のデータ
//	引数	：	[IN]char	*pt		ポインタのアドレス
//	備考	：	なし
/******************************************************************************/
double	GetDouble( char *pt )
{
	union
	{
		char	dat[4];
		double	data;
	}DBL;

	for( int i=0; i<8; i++ )
	{
		DBL.dat [ i ] = *( pt + i );
	}

	return	DBL.data;
}

/******************************************************************************/
//	名前	：	ファイルオープンダイアログ表示
//	説明	：	ファイルオープンダイアログを表示する
//	戻り値	：	bool	処理結果	true(成功)
//									false(失敗)
//	引数	：	[IN]char		*Filter		フィルター
//				[IN]char		*Ext		読み込む拡張子名
//				[IN]char		*Title		読み込む時のタイトル
//				[IN/OUT]char	*Path		パス
//	備考	：	なし
/******************************************************************************/
bool	LoadFileDialog( char *Filter, char *Ext, char *Title, char *Path )
{
	OPENFILENAME	ofn;
	char	FileTitle[64] = { "" };

	memset( &ofn, 0, sizeof( OPENFILENAME ) );

	ofn.lStructSize		= sizeof( OPENFILENAME );
	ofn.hwndOwner		= g_hWnd;

	ofn.lpstrFilter		= Filter;
	ofn.lpstrFile		= Path;

	ofn.nMaxFile		= 0xFFFF;
	ofn.Flags			= OFN_NODEREFERENCELINKS;
	ofn.lpstrDefExt		= Ext;
	ofn.nMaxFileTitle	= 64;
	ofn.lpstrFileTitle	= FileTitle;
	ofn.lpstrTitle		= Title;

	if( GetOpenFileName( &ofn ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************/
//	名前	：	ファイルセーブダイアログ表示
//	説明	：	ファイルセーブダイアログを表示する
//	戻り値	：	bool	処理結果	true(成功)
//									false(失敗)
//	引数	：	[IN]char		*Filter		フィルター
//				[IN]char		*Ext		読み込む拡張子名
//				[IN]char		*Title		読み込む時のタイトル
//				[IN/OUT]char	*Path		パス
//	備考	：	なし
/******************************************************************************/
bool	SaveFileDialog( char *Filter, char *Ext, char *Title, char *Path )
{
	OPENFILENAME	ofn;
	char	FileTitle[64] = { "" };

	memset( &ofn, 0, sizeof( OPENFILENAME ) );

	ofn.lStructSize		= sizeof( OPENFILENAME );
	ofn.hwndOwner		= g_hWnd;

	ofn.lpstrFilter		= Filter;
	ofn.lpstrFile		= Path;

	ofn.nMaxFile		= 0xFFFF;
	ofn.Flags			= OFN_NODEREFERENCELINKS;
	ofn.lpstrDefExt		= Ext;
	ofn.nMaxFileTitle	= 64;
	ofn.lpstrFileTitle	= FileTitle;
	ofn.lpstrTitle		= Title;

	if( GetSaveFileName( &ofn ) )
	{
		return	true;
	}
	else
	{
		return	false;
	}
}

/******************************************************************************/
//	名前	：	角度取得
//	説明	：	角度を計算する
//	戻り値	：	double		角度（ラジアン）
//	引数	：	[IN]double	x1	座標X1
//				[IN]double	y1	座標Y1
//				[IN]double	x2	座標X2
//				[IN]double	y2	座標Y2
//	備考	：	なし
/******************************************************************************/
double	GetRot( double x1, double y1, double x2, double y2 )
{
	double w = x2 - x1;
	double h = y2 - y1;
	double rot = atan2( h, w );

	return rot;
}

/******************************************************************************/
//	名前	：	距離取得
//	説明	：	距離を計算する
//	戻り値	：	double	２点間の距離
//	引数	：	[IN]double	x1	座標X1
//				[IN]double	y1	座標Y1
//				[IN]double	x2	座標X2
//				[IN]double	y2	座標Y2
//	備考	：	なし
/******************************************************************************/
double	GetDistance( double x1, double y1, double x2, double y2 )
{
	double w = x2 - x1;
	double h = y2 - y1;
	double dis = sqrt( (h*h)+(w*w) );

	return dis;
}

/******************************************************************************/
//	名前	：	角度補正
//	説明	：	角度を補正する
//	戻り値	：	なし
//	引数	：	[IN/OUT]double	&Rot	角度
//	備考	：	角度を0～359にする
/******************************************************************************/
void	CorrectRot( double &Rot )
{
	// クリッピング
	if( Rot >= 360 )
	{
		Rot -= 360;
	}
	else if( Rot < 0 )
	{
		Rot += 360;
	}
}

/******************************************************************************/
//	名前	：	ラジアンをデグリーに変換
//	説明	：	ラジアンをデグリーに変換する
//	戻り値	：	double	角度（デグリー）
//	引数	：	[IN]double	Rot		角度（ラジアン）
//	備考	：	なし
/******************************************************************************/
double	RadToDeg( double Rot )
{
	double	Ret = (Rot * 180.0 ) / PI;
	CorrectRot( Ret );
	return Ret;
}

/******************************************************************************/
//	名前	：	デグリーをラジアンに変換
//	説明	：	デグリーをラジアンに変換する
//	戻り値	：	double	角度（ラジアン）
//	引数	：	[IN]double	Rot		角度（デグリー）
//	備考	：	なし
/******************************************************************************/
double	DegToRad( double Rot )
{
	double	Ret = ( Rot * PI ) / 180.0;

	if( Ret > 2 * PI )
	{
		Ret -= 2 * PI;
	}

	if( Ret < 0 )
	{
		Ret += 2 * PI;
	}

	return Ret;
}

/******************************************************************************/
//	名前	：	移動量取得(X)
//	説明	：	移動量を計算する
//	戻り値	：	double	移動量
//	引数	：	[IN]double	Speed		速度
//				[IN]double	Rot			角度(ラジアン)
//	備考	：	なし
/******************************************************************************/
double	GetMoveX( double Speed, double Rot )
{
	double x = Speed * cos( Rot );
	return x;
}

/******************************************************************************/
//	名前	：	移動量取得(Y)
//	説明	：	移動量を計算する
//	戻り値	：	double	移動量
//	引数	：	[IN]double	Speed		速度
//				[IN]double	Rot			角度(ラジアン)
//	備考	：	なし
/******************************************************************************/
double	GetMoveY( double Speed, double Rot )
{
	double y = Speed * sin( Rot );
	return y;
}

/******************************************************************************/
//	名前	：	最大値最小値判定
//	説明	：	最大値最小値の判定を行う
//	戻り値	：	なし
//	引数	：	[IN/OUT]double	Target	判定対象データ
//				[IN]double		Max		最大値
//				[IN]double		Min		最小値
//	備考	：	なし
/******************************************************************************/
void	MaxMin( double& Target, double Max, double Min )
{
	if( Target > Max )
	{
		Target = Max;
	}
	else if( Target < Min )
	{
		Target = Min;
	}
}

/******************************************************************************/
//	名前	：	最大値最小値判定
//	説明	：	最大値最小値の判定を行う
//	戻り値	：	なし
//	引数	：	[IN/OUT]long	Target	判定対象データ
//				[IN]long		Max		最大値
//				[IN]long		Min		最小値
//	備考	：	なし
/******************************************************************************/
void	MaxMin( long& Target, long Max, long Min )
{
	if( Target > Max )
	{
		Target = Max;
	}
	else if( Target < Min )
	{
		Target = Min;
	}
}

/******************************************************************************/
//	名前	：	16進文字列数値変換
//	説明	：	16進の文字列を数値に変換する
//	戻り値	：	UChar	変換後の数値
//	引数	：	[IN]char	*Target	16進文字列
//	備考	：	なし
/******************************************************************************/
ULong	ToDec( const char *Target )
{
	long	Index = 0;
	ULong	Ret = 0;
	char	cTarget;

	//	末尾までループ
	while( Target[Index] != '\0' )
	{
		long	Num = 0;
		cTarget = tolower( Target[Index] );

		//	0～9判定
		if( '0' <= cTarget && cTarget <= '9' )
		{
			//	数値に変換
			Num = cTarget - '0';
		}
		else if( 'a' <= cTarget && cTarget <= 'f' )
		{
			//	a～f
			Num = cTarget - 'a' + 10;
		}

		//	インデックス加算
		Index ++;

		Ret = Ret * 16 + Num;
	}

	return Ret;
}

/******************************************************************************/
//	名前	：	ウエイト
//	説明	：	一定数処理を止める
//	戻り値	：	なし
//	引数	：	[IN]long	time	フレーム数
//	備考	：	なし
/******************************************************************************/
void	WaitTime( long time )
{
	clock_t	sTime;
	sTime = clock() + ( (clock_t)time * CLOCKS_PER_SEC ) / 60;
	while( clock() < sTime );
}

/******************************************************************************/
//	名前	：	文字列分割
//	説明	：	文字列の分割を行う
//	戻り値	：	list<string>			分割後の文字列
//	引数	：	[IN]string	Str			分割対象の文字列
//				[IN]string	delim		区切り文字
//	備考	：	なし
/******************************************************************************/
vector<string>	SplitText( string Str, string delim )
{
	vector<string>	Res;
	int	Count;

	while( (Count = (int)Str.find_first_of(delim) ) != Str.npos )
	{
		if( Count > 0 )
		{
			//	バッファに格納
			Res.push_back( Str.substr( 0, Count ) );
		}
		//	検索位置をずらす
		Str = Str.substr( Count + 1 );
	}

	if( Str.length() > 0 )
	{
		Res.push_back(Str);
	}

	return Res;
}
