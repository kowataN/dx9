/******************************************************************************/
//	DirectX9ライブラリ
//		コモン
/******************************************************************************/

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
//	汎用
extern	char	g_FreeWork[FREE_WORK_SIZE];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	long	LoadFile( char *FileName, char *LoadBuf );							//	ファイルロード
extern	long	SaveFile( char *FileName, char *SaveBuf, long Size );				//	ファイルセーブ
extern	void	PathToName( char *Path, char *FileName);							//	パスからファイル名取得
extern	void	PathToOpt( char *Path, char *FileName );							//	パスから拡張子取得
extern	void	PathToDir( char *Path, char *Dir );									//	パスからディレクトリ取得
extern	void	PathChange( char *NewPath, char *BakupPath );						//	パス変更
extern	long	Secret( char *FileName );											//	暗号化
extern	void	SaveSecret( char *FileName, char *SaveFileName );					//	暗号ファイル保存
extern	long	AntiSecret( char *SecretFile, char *FileName );						//	解読
extern	void	SetShort( short Dat, char *pt );									//	ショート型を設定
extern	void	SetLong( long Dat, char *pt );										//	ロング型を設定
extern	void	SetFloat( float Dat, char *pt );									//	フロート型を設定
extern	void	SetDouble( double Dat, char *pt );									//	ダブル型をを設定
extern	short	GetShort( char *pt );												//	ショート型データの取得
extern	long	GetLong( char *pt );												//	ロング型データの取得
extern	float	GetFloat( char *pt );												//	フロート型データの取得
extern	double	GetDouble( char *pt );												//	ダブル型データの取得
extern	bool	LoadFileDialog( char *Filter, char *Ext, char *Title, char *Path );	//	ファイルオープンダイアログ表示
extern	bool	SaveFileDialog( char *Filter, char *Ext, char *Title, char *Path );	//	ファイルセーブダイアログ表示
extern	double	GetRot( double x1, double y1, double x2, double y2 );				//	角度取得
extern	double	GetDistance( double x1, double y1, double x2, double y2 );			//	距離取得
extern	void	CorrectRot( double &Rot );
extern	double	RadToDeg( double Rot );												//	ラジアンをデグリーに変換
extern	double	DegToRad( double Rot );												//	デグリーをラジアンに変換
extern	double	GetMoveX( double Speed, double Rot );								//	移動量取得(X)
extern	double	GetMoveY( double Speed, double Rot );								//	移動量取得(Y)
extern	void	MaxMin( double& Target, double Max, double Min );
extern	void	MaxMin( long& Target, long Max, long Min );
extern	ULong	ToDec( const char *Target );
extern	void	WaitTime( long time );
extern	vector<string>	SplitText( string Str, string delim );
