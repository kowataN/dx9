/******************************************************************************/
//	DirectX9���C�u����
//		�~���[�W�b�N
/******************************************************************************/

#ifndef	__Dx9LibMusic_H__
#define	__Dx9LibMusic_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	���[�v�����O
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
//	�\����
//----------------------------------------------------------------------------//

//	�I�[�f�B�I�p�\����
typedef	struct
{
	UChar			Use;				//	�g�p�t���O
	UChar			Type;				//	�I�[�f�B�I�̎�ށi0�Fwave�A1�Fmidi�j
	enum AudioState	State;				//	��ԁi0:��~���A1:�Đ����j
	UChar			Loop;				//	���[�v�񐔁i�O�Ń��[�v���Ȃ��j
	float			Tempo;				//	�e���|
	long			Vol;				//	�{�����[��
	MUSIC_TIME		Length;				//	����
}ST_AudioInfo;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class CDx9LibMusic
{
private:
	//	�������t���O
	short	WaveInitFlag;
	//	�p�t�H�[�}���X
	IDirectMusicPerformance8*	m_pPerformance[AUDIO_MAX];
	//	���[�_�[
	IDirectMusicLoader8*		m_pLoader;
	//	�I�[�f�B�I�E�p�X
	IDirectMusicAudioPath8*		m_pPath[AUDIO_MAX];
	//	�R���|�[�T�[
	IDirectMusicComposer8*		m_pComposer;
	//	DirectMusic�̃Z�O�����g
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
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	ST_AudioInfo	AUDIO[AUDIO_MAX];
extern	CDx9LibMusic	g_LibMusic;
