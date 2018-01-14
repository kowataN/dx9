/******************************************************************************/
//	DirectX9���C�u����
//		�~���[�W�b�N
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
ST_AudioInfo	AUDIO[AUDIO_MAX];



/******************************************************************************/
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
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
//	���O	�F	�f�X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibMusic::~CDx9LibMusic()
{
	ReleaseDevice();
}

/******************************************************************************/
//	���O	�F	DirectMusic�̏�����
//	����	�F	�f�o�C�X�̏��������s��
//	�߂�l	�F	bool	��������	true(����)
//									false(���s)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
bool	CDx9LibMusic::InitDevice( void )
{
	LOG_WRITE( TypeDebugLog, "DirectMusic�̏������J�n" );
	HRESULT	hr;

	////////////////////////////////////
	//	�p�t�H�[�}���X�̍쐬�Ə�����

	//	�p�t�H�[�}���X�̏�����
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		//	�p�t�H�[�}���X�̍쐬
		hr = CoCreateInstance(	CLSID_DirectMusicPerformance,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicPerformance8,
								(void **)&m_pPerformance[i] );
		if( FAILED(hr) )
		{
			LOG_WRITE( TypeErrLog, "DirectMusicPerformance8�I�u�W�F�N�g�̍쐬�Ɏ��s" );
			DXTRACE_ERR( "DirectMusicPerformance8�I�u�W�F�N�g�̍쐬�Ɏ��s", hr );
			return	false;
		}

		hr = m_pPerformance[i]->InitAudio(
				NULL,								//	IDirectMusic�C���^�[�t�F�[�X�͕s�v
				NULL,								//	IDirectSound�C���^�[�t�F�[�X�͕s�v
				g_hWnd,								//	�E�C���h�E�E�n���h��
				DMUS_APATH_SHARED_STEREOPLUSREVERB,	//	�f�t�H���g�̃I�[�f�B�I�p�X
				64,									//	�p�t�H�[�}���X�̃`�����l���̐�
				DMUS_AUDIOF_ALL,					//	�V���Z�T�C�U�̋@�\
				NULL								//	�I�[�f�B�I�E�p�����[�^�ɂ̓f�t�H���g���g�p
				);
		if( FAILED(hr) )
		{
			LOG_WRITE( TypeErrLog, "DirectMusicPerformance8�̏������Ɏ��s" );
			DXTRACE_ERR( "DirectMusicPerformance8�̏������Ɏ��s", hr );
			return	false;
		}
	}

	/////////////////////////////////////
	//	���[�_�[�̍쐬�Ə�����

	//	���[�_�[�̍쐬
	hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL,
						CLSCTX_INPROC, IID_IDirectMusicLoader8,
						(void **)&m_pLoader );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "DirectMusicPerformance8�̏������Ɏ��s" );
		DXTRACE_ERR( "DirectMusicPerformance8�̏������Ɏ��s", hr );
		return	false;
	}

	//	���[�_�[�̏�����
	CHAR	strPath[MAX_PATH];
	DWORD	i = GetCurrentDirectory( MAX_PATH, strPath );

	if( i == 0 || MAX_PATH < i )
	{
		LOG_WRITE( TypeErrLog, "�J�����g�E�f�B���N�g���̎擾�Ɏ��s" );
		DXTRACE_MSG( "�J�����g�E�f�B���N�g���̎擾�Ɏ��s" );
		return	false;
	}

	//	�}���`�E�o�C�g������UNICODE�ɕϊ�
	WCHAR	wstrSearchPath[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH );

	//	���[�_�[�Ɍ����p�X��ݒ�
	hr = m_pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, wstrSearchPath, FALSE );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�����p�X�Ɏ��s" );
		DXTRACE_ERR( "�����p�X�Ɏ��s", hr );
		return	false;
	}

	//	�R���|�[�T�[
	hr = CoCreateInstance( CLSID_DirectMusicComposer,
						NULL,
						CLSCTX_INPROC,
						IID_IDirectMusicComposer8,
						(void **)&m_pComposer );
	if( FAILED(hr) )
	{
		LOG_WRITE( TypeErrLog, "�R���|�[�T�[�̐ݒ�Ɏ��s" );
		DXTRACE_ERR( "�R���|�[�T�[�̐ݒ�Ɏ��s", hr );
		return	false;
	}

	LOG_WRITE( TypeDebugLog, "DirectMusic�̏������I��" );

	return	true;
}

/******************************************************************************/
//	���O	�F	�I�[�f�B�I�\���̂̑S������
//	����	�F	�I�[�f�B�I�\���̂̑P���M�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibMusic::InitAll( void )
{
	for( long i=0; i<AUDIO_MAX; i++ )
	{
		Init( i );
	}
}

/******************************************************************************/
//	���O	�F	�I�[�f�B�I�\���̂̏�����
//	����	�F	�I�[�f�B�I�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AudioNo		�I�[�f�B�I�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibMusic::Init( long AudioNo )
{
	//	�e�����o������
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
//	���O	�F	�I�[�f�B�I�t�@�C�����[�h
//	����	�F	�I�[�f�B�I����̓ǂݍ��݂��s��
//	�߂�l	�F	BOOL		��������	TRUE(����)
//										FALSE(���s)
//	����	�F	[IN]long	AudioNo		�I�[�f�B�I�ԍ�
//				[IN]char	*FileName	�t�@�C����
//	���l	�F	�Ή��`���i*.wav�A*.mid�j
/******************************************************************************/
BOOL	CDx9LibMusic::Load( long AudioNo, char *FileName )
{
	if( AudioNo >= AUDIO_MAX )
	{
		return FALSE;
	}

	//	�g�p�t���OON
	AUDIO[AudioNo].Use = ON;

	//	�t�@�C���ǂݍ���
	long size = LoadFile( FileName, g_FreeWork );

	//	WAV�AMIDI�t�@�C�����ǂ�������
	if( g_FreeWork[0] == 'R' && g_FreeWork[1] == 'I'
		&& g_FreeWork[2] == 'F' && g_FreeWork[3] == 'F'  )
	{
		AUDIO[AudioNo].Type = 0;	//	WAVE�t�@�C��
	}
	else if( g_FreeWork[0] == 'M' && g_FreeWork[1] == 'T'
		&& g_FreeWork[2] == 'h' && g_FreeWork[3] == 'd' )
	{
		AUDIO[AudioNo].Type = 1;	//	MIDI�t�@�C��
	}

	//	�T�C�Y�`�F�b�N
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
	//	�Z�O�����g�쐬
	MultiByteToWideChar( CP_ACP, 0, FileName, -1, wstrFileName, MAX_PATH );
	hr = m_pLoader->LoadObjectFromFile(
			CLSID_DirectMusicSegment,	//	�I�u�W�F�N�g�N���X
			IID_IDirectMusicSegment8,	//	�C���^�[�t�F�C�X
			wstrFileName,				//	�I�u�W�F�N�g��ۑ�����t�@�C��
			(LPVOID *)&m_Segment[AudioNo]	//	�󂯎��ϐ��A�h���X
			);
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "�Z�O�����g�̃��[�h�Ɏ��s", hr );
		return	FALSE;
	}

	//	�Z�O�����g�̒������擾
	hr = m_Segment[AudioNo]->GetLength( &AUDIO[AudioNo].Length );
	if ( FAILED(hr) )
	{
		DXTRACE_ERR( "�Z�O�����g�̒����̎擾�Ɏ��s", hr );
		return	FALSE;
	}

	//	�Đ��p�̃I�[�f�B�I�p�X���擾
	hr = m_pPerformance[AudioNo]->CreateStandardAudioPath(
			DMUS_APATH_DYNAMIC_STEREO,
			64,
			TRUE,
			&m_pPath[AudioNo] );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "�I�[�f�B�I�E�p�X�̍쐬�Ɏ��s", hr );
		return FALSE;
	}

	//	���[�v�͈͐ݒ�
	MUSIC_TIME	start_time=0;	//	���[�v�Đ��̊J�n������ݒ肵�Ă���
	MUSIC_TIME	end_time=0;		//	���[�v�Đ��̏I��������ݒ肵�Ă���

	hr = m_Segment[AudioNo]->SetLoopPoints( start_time, end_time );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "���[�v�͈̔͂̐ݒ�Ɏ��s", hr );
		return	FALSE;
	}

	//	MIDI�p�Z�O�����g�̐ݒ�
	if( AUDIO[AudioNo].Type == 1 )
	{
		hr = m_Segment[AudioNo]->SetParam(
				GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL );
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "MIDI�p�p�����[�^�̐ݒ�Ɏ��s", hr );
			return	FALSE;
		}
	}

	//	�o���h�̃_�E�����[�h�i�蓮�j
	hr = m_Segment[AudioNo]->Download( m_pPerformance[AudioNo] );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "�Z�O�����g�̃o���h�̃_�E�����[�h�Ɏ��s", hr );
		return	FALSE;
	}

	return	TRUE;
}

/******************************************************************************/
//	���O	�F	�I�[�f�B�I�Đ�
//	����	�F	�I�[�f�B�I�̍Đ����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AudioNo		�I�[�f�B�I�ԍ�
//				[IN]short	Loop		���[�v�t���O
//	���l	�F	�Ȃ�
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

		//	�Đ�
		AUDIO[ AudioNo ].State = StatePlay;

		/////////////////////////////////////
		//	���[�v�̐ݒ�
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
			DXTRACE_ERR( "���[�v�񐔂̐ݒ�Ɏ��s", hr );
			return;
		}

		/////////////////////////////////////
		//	�{�����[���̐ݒ�
		hr = m_pPath[ AudioNo ]->SetVolume( AUDIO[ AudioNo ].Vol, 0 );
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "�{�����[���̐ݒ�Ɏ��s", hr );
			return;
		}

		/////////////////////////////////////
		//	�e���|�̐ݒ�
		hr = m_pPerformance[ AudioNo ]->SetGlobalParam(
				GUID_PerfMasterTempo, (VOID *)&AUDIO[ AudioNo ].Tempo, sizeof(float) );
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "�e���|�̐ݒ�Ɏ��s", hr );
			return;
		}

		//	�Đ�
		hr = m_pPerformance[ AudioNo ]->PlaySegmentEx(
			m_Segment[ AudioNo ],	//	�Đ�����I�u�W�F�N�g
			NULL,					//	NULL��n���i�܂����Ή��j
			NULL,					//	�e���v���[�g�Z�O�����g
			0,						//	�Đ�����^�C�~���O
			0,						//	�Z�O�����g���Đ�����p�t�H�[�}���X�̎���
			NULL,					//	�Đ����̃Z�O�����g�C���X�^���X
			NULL,					//	��~����Z�O�����g�̏�Ԃ܂��́A�I�[�f�B�I�p�X
			NULL					//	�Đ��Ɏg���I�[�f�B�I�p�X
			);
		if( FAILED(hr) )
		{
			DXTRACE_ERR( "�Đ��Ɏ��s", hr );
			return;
		}
	}
}

/******************************************************************************/
//	���O	�F	�I�[�f�B�I��~
//	����	�F	�I�[�f�B�I�̒�~���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AudioNo		�I�[�f�B�I�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibMusic::Stop( long AudioNo )
{
	if( AUDIO[ AudioNo ].Use )
	{
		m_pPerformance[ AudioNo ]->StopEx( NULL, NULL, 0 );
	}
}

/******************************************************************************/
//	���O	�F	�f�o�C�X���
//	����	�F	�f�o�C�X�̉�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F�Ȃ�
/******************************************************************************/
void	CDx9LibMusic::ReleaseDevice( void )
{
	//	�I�[�f�B�I��~
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		if( AUDIO[i].State == StatePlay )
		{
			Stop( i );
		}
	}

	//	�R���|�[�U�[���
	//SAFE_RELEASE(m_pComposer);

	//	���[�_�[���
	SAFE_RELEASE(m_pLoader);

	//	�Z�O�����g�X�e�[�g���
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		//	�p�t�H�[�}���X�I��
		if( m_pPerformance[i] != NULL )
		{
			//m_pPerformance[i]->CloseDown();
		}
		//SAFE_RELEASE(m_Segment[i]);
	}

	//	�p�t�H�[�}���X���
	for( int i=0; i<AUDIO_MAX; i++ )
	{
		SAFE_RELEASE(m_pPerformance[i]);
	}

	//	COM�̏I������
	CoUninitialize();
}
