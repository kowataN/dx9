/******************************************************************************/
//	DirectX9ライブラリ
//		ミュージック
/******************************************************************************/

#ifndef	__Dx9LibMusic_H__
#define	__Dx9LibMusic_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

//	ループユラグ
enum
{
	ONE = 0,
	LOOP
};

enum AudioState
{
	StateStop,StatePlay
};

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	オーディオ用構造体
typedef	struct
{
	UChar			Use;				//	使用フラグ
	UChar			Type;				//	オーディオの種類（0：wave、1：midi）
	enum AudioState	State;				//	状態（0:停止中、1:再生中）
	UChar			Loop;				//	ループ回数（０でループしない）
	float			Tempo;				//	テンポ
	long			Vol;				//	ボリューム
	MUSIC_TIME		Length;				//	長さ
}ST_AudioInfo;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class CDx9LibMusic
{
private:
	//	初期化フラグ
	short	WaveInitFlag;
	//	パフォーマンス
	IDirectMusicPerformance8*	m_pPerformance[AUDIO_MAX];
	//	ローダー
	IDirectMusicLoader8*		m_pLoader;
	//	オーディオ・パス
	IDirectMusicAudioPath8*		m_pPath[AUDIO_MAX];
	//	コンポーサー
	IDirectMusicComposer8*		m_pComposer;
	//	DirectMusicのセグメント
	IDirectMusicSegment8*		m_Segment[AUDIO_MAX];

public:
	CDx9LibMusic();
	~CDx9LibMusic();
	bool	InitDevice( void );
	void	InitAll( void );
	void	Init( long AudioNo );
	BOOL	Load( long AudioNo, char *FileName );
	void	Play( long AudioNo, short Loop );
	void	Stop( long AudioNo );
	void	ReleaseDevice( void );
};

#endif	//	__Dx9LibMusic_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	ST_AudioInfo	AUDIO[AUDIO_MAX];
extern	CDx9LibMusic	g_LibMusic;
