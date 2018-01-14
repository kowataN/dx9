/******************************************************************************/
//	DirectX9ライブラリ
//		ミュージック
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
ST_AudioInfo	AUDIO[AUDIO_MAX];



/******************************************************************************/
//	名前	：	コンストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibMusic::CDx9LibMusic()
{
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		m_pPerformance[i] = NULL;
		m_pPath[i] = NULL;
		m_Segment[i] = NULL;
	}

	m_pLoader = NULL;
	m_pComposer = NULL;
}

/******************************************************************************/
//	名前	：	デストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibMusic::~CDx9LibMusic()
{
	ReleaseDevice();
}

/******************************************************************************/
//	名前	：	DirectMusicの初期化
//	説明	：	デバイスの初期化を行う
//	戻り値	：	bool	処理結果	true(成功)
//									false(失敗)
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
bool	CDx9LibMusic::InitDevice( void )
{
	LOG_WRITE( TypeDebugLog, "DirectMusicの初期化開始" );
	HRESULT	hr;

	////////////////////////////////////
	//	パフォーマンスの作成と初期化

	//	パフォーマンスの初期化
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		//	パフォーマンスの作成
		hr = CoCreateInstance(	CLSID_DirectMusicPerformance,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicPerformance8,
								(void **)&m_pPerformance[i] );
		if( FAILED(hr) )
		{
			LOG_WRITE( TypeErrLog, "DirectMusicPerformance8オブジェクトの作成に失敗" );
			DXTRACE_ERR( "DirectMusicPerformance8オブジェクトの作成に失敗", hr );
			return	false;
		}

		hr = m_pPerformance[i]->InitAudio(
				NULL,								//	IDirectMusicインターフェースは不要
				NULL,								//	IDirectSoundインターフェースは不要
				g_hWnd,								//	ウインドウ・ハンドル
				DMUS_APATH_SHARED_STEREOPLUSREVERB,	//	デフォルトのオーディオパス
				64,									//	パフォーマンスのチャンネルの数
				DMUS_AUDIOF_ALL,					//	シンセサイザの機能
				NULL								//	オーディオ・パラメータにはデフォルトを使用
				);
		if( FAILED(hr) )
		{
			LOG_WRITE( TypeErrLog, "DirectMusicPerformance8の初期化に失敗" );
			DXTRACE_ERR( "DirectMusicPerformance8の初期化に失敗", hr );
			return	false;
		}
	}

	/////////////////////////////////////
	//	ローダーの作成と初期化

	//	ローダーの作成
	hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL,
						CLSCTX_INPROC, IID_IDirectMusicLoader8,
						(void **)&m_pLoader );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "DirectMusicPerformance8の初期化に失敗" );
		DXTRACE_ERR( "DirectMusicPerformance8の初期化に失敗", hr );
		return	false;
	}

	//	ローダーの初期化
	CHAR	strPath[MAX_PATH];
	DWORD	i = GetCurrentDirectory( MAX_PATH, strPath );

	if( i == 0 || MAX_PATH < i )
	{
		LOG_WRITE( TypeErrLog, "カレント・ディレクトリの取得に失敗" );
		DXTRACE_MSG( "カレント・ディレクトリの取得に失敗" );
		return	false;
	}

	//	マルチ・バイト文字をUNICODEに変換
	WCHAR	wstrSearchPath[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH );

	//	ローダーに検索パスを設定
	hr = m_pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, wstrSearchPath, FALSE );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "検索パスに失敗" );
		DXTRACE_ERR( "検索パスに失敗", hr );
		return	false;
	}

	//	コンポーサー
	hr = CoCreateInstance( CLSID_DirectMusicComposer,
						NULL,
						CLSCTX_INPROC,
						IID_IDirectMusicComposer8,
						(void **)&m_pComposer );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "コンポーサーの設定に失敗" );
		DXTRACE_ERR( "コンポーサーの設定に失敗", hr );
		return	false;
	}

	LOG_WRITE( TypeDebugLog, "DirectMusicの初期化終了" );

	return	true;
}

/******************************************************************************/
//	名前	：	オーディオ構造体の全初期化
//	説明	：	オーディオ構造体の善処貴下を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibMusic::InitAll( void )
{
	for( long i=0; i<AUDIO_MAX; i++ )
	{
		Init( i );
	}
}

/******************************************************************************/
//	名前	：	オーディオ構造体の初期化
//	説明	：	オーディオ構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	AudioNo		オーディオ番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibMusic::Init( long AudioNo )
{
	//	各メンバ初期化
	AUDIO[ AudioNo ].Use	= OFF;
	AUDIO[ AudioNo ].Loop	= 0;
	AUDIO[ AudioNo ].State	= StateStop;
	AUDIO[ AudioNo ].Type	= NOT;
	AUDIO[ AudioNo ].Tempo	= 1.0f;
	AUDIO[ AudioNo ].Vol	= -4800;
	AUDIO[ AudioNo ].Length	= 0;

	SAFE_RELEASE(m_Segment[AudioNo]);
}

/******************************************************************************/
//	名前	：	オーディオファイルロード
//	説明	：	オーディオ入るの読み込みを行う
//	戻り値	：	BOOL		処理結果	TRUE(成功)
//										FALSE(失敗)
//	引数	：	[IN]long	AudioNo		オーディオ番号
//				[IN]char	*FileName	ファイル名
//	備考	：	対応形式（*.wav、*.mid）
/******************************************************************************/
BOOL	CDx9LibMusic::Load( long AudioNo, char *FileName )
{
	if( AudioNo >= AUDIO_MAX )
	{
		return FALSE;
	}

	//	使用フラグON
	AUDIO[AudioNo].Use = ON;

	//	ファイル読み込み
	long size = LoadFile( FileName, g_FreeWork );

	//	WAV、MIDIファイルかどうか識別
	if( g_FreeWork[0] == 'R' && g_FreeWork[1] == 'I'
		&& g_FreeWork[2] == 'F' && g_FreeWork[3] == 'F'  )
	{
		AUDIO[AudioNo].Type = 0;	//	WAVEファイル
	}
	else if( g_FreeWork[0] == 'M' && g_FreeWork[1] == 'T'
		&& g_FreeWork[2] == 'h' && g_FreeWork[3] == 'd' )
	{
		AUDIO[AudioNo].Type = 1;	//	MIDIファイル
	}

	//	サイズチェック
	if( AUDIO_SIZE_MAX < size )
	{
		return	FALSE;
	}

	if( AUDIO[AudioNo].Type )
	{
		AUDIO[AudioNo].Tempo = 0.0;
	}

	HRESULT	hr;
	WCHAR	wstrFileName[MAX_PATH];

	/////////////////////////////////////
	//	セグメント作成
	MultiByteToWideChar( CP_ACP, 0, FileName, -1, wstrFileName, MAX_PATH );
	hr = m_pLoader->LoadObjectFromFile(
			CLSID_DirectMusicSegment,	//	オブジェクトクラス
			IID_IDirectMusicSegment8,	//	インターフェイス
			wstrFileName,				//	オブジェクトを保存するファイル
			(LPVOID *)&m_Segment[AudioNo]	//	受け取る変数アドレス
			);
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "セグメントのロードに失敗", hr );
		return	FALSE;
	}

	//	セグメントの長さを取得
	hr = m_Segment[AudioNo]->GetLength( &AUDIO[AudioNo].Length );
	if ( FAILED(hr) )
	{
		DXTRACE_ERR( "セグメントの長さの取得に失敗", hr );
		return	FALSE;
	}

	//	再生用のオーディオパスを取得
	hr = m_pPerformance[AudioNo]->CreateStandardAudioPath(
			DMUS_APATH_DYNAMIC_STEREO,
			64,
			TRUE,
			&m_pPath[AudioNo] );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "オーディオ・パスの作成に失敗", hr );
		return FALSE;
	}

	//	ループ範囲設定
	MUSIC_TIME	start_time=0;	//	ループ再生の開始時刻を設定しておく
	MUSIC_TIME	end_time=0;		//	ループ再生の終了時刻を設定しておく

	hr = m_Segment[AudioNo]->SetLoopPoints( start_time, end_time );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "ループの範囲の設定に失敗", hr );
		return	FALSE;
	}

	//	MIDI用セグメントの設定
	if( AUDIO[AudioNo].Type == 1 )
	{
		hr = m_Segment[AudioNo]->SetParam(
				GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL );
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "MIDI用パラメータの設定に失敗", hr );
			return	FALSE;
		}
	}

	//	バンドのダウンロード（手動）
	hr = m_Segment[AudioNo]->Download( m_pPerformance[AudioNo] );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "セグメントのバンドのダウンロードに失敗", hr );
		return	FALSE;
	}

	return	TRUE;
}

/******************************************************************************/
//	名前	：	オーディオ再生
//	説明	：	オーディオの再生を行う
//	戻り値	：	なし
//	引数	：	[IN]long	AudioNo		オーディオ番号
//				[IN]short	Loop		ループフラグ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibMusic::Play( long AudioNo, short Loop )
{
	HRESULT	hr;

	if( m_Segment[ AudioNo ] != NULL )
	{
		if( AUDIO[ AudioNo ].State == StatePlay )
		{
			Stop( AudioNo );
		}

		//	再生
		AUDIO[ AudioNo ].State = StatePlay;

		/////////////////////////////////////
		//	ループの設定
		if( Loop )
		{
			hr = m_Segment[ AudioNo ]->SetRepeats( DMUS_SEG_REPEAT_INFINITE );
		}
		else
		{
			hr = m_Segment[ AudioNo ]->SetRepeats( 1 );
		}
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "ループ回数の設定に失敗", hr );
			return;
		}

		/////////////////////////////////////
		//	ボリュームの設定
		hr = m_pPath[ AudioNo ]->SetVolume( AUDIO[ AudioNo ].Vol, 0 );
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "ボリュームの設定に失敗", hr );
			return;
		}

		/////////////////////////////////////
		//	テンポの設定
		hr = m_pPerformance[ AudioNo ]->SetGlobalParam(
				GUID_PerfMasterTempo, (VOID *)&AUDIO[ AudioNo ].Tempo, sizeof(float) );
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "テンポの設定に失敗", hr );
			return;
		}

		//	再生
		hr = m_pPerformance[ AudioNo ]->PlaySegmentEx(
			m_Segment[ AudioNo ],	//	再生するオブジェクト
			NULL,					//	NULLを渡す（まだ未対応）
			NULL,					//	テンプレートセグメント
			0,						//	再生するタイミング
			0,						//	セグメントを再生するパフォーマンスの時刻
			NULL,					//	再生中のセグメントインスタンス
			NULL,					//	停止するセグメントの状態または、オーディオパス
			NULL					//	再生に使うオーディオパス
			);
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "再生に失敗", hr );
			return;
		}
	}
}

/******************************************************************************/
//	名前	：	オーディオ停止
//	説明	：	オーディオの停止を行う
//	戻り値	：	なし
//	引数	：	[IN]long	AudioNo		オーディオ番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibMusic::Stop( long AudioNo )
{
	if( AUDIO[ AudioNo ].Use )
	{
		m_pPerformance[ AudioNo ]->StopEx( NULL, NULL, 0 );
	}
}

/******************************************************************************/
//	名前	：	デバイス解放
//	説明	：	デバイスの解放を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：なし
/******************************************************************************/
void	CDx9LibMusic::ReleaseDevice( void )
{
	//	オーディオ停止
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		if( AUDIO[i].State == StatePlay )
		{
			Stop( i );
		}
	}

	//	コンポーザー解放
	//SAFE_RELEASE(m_pComposer);

	//	ローダー解放
	SAFE_RELEASE(m_pLoader);

	//	セグメントステート解放
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		//	パフォーマンス終了
		if( m_pPerformance[i] != NULL )
		{
			//m_pPerformance[i]->CloseDown();
		}
		//SAFE_RELEASE(m_Segment[i]);
	}

	//	パフォーマンス解放
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		SAFE_RELEASE(m_pPerformance[i]);
	}

	//	COMの終了処理
	CoUninitialize();
}
