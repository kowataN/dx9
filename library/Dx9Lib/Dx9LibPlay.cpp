/******************************************************************************/
//	DirectX9���C�u����
//		�v���C
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
IDirectPlay8Peer		*lpDPlayPeer = NULL;		// �@�s�A�c�[�s�A�p
LPDIRECTPLAY8ADDRESS	lpDPlayDeviceInfo = NULL;	//	�A�h���X�I�u�W�F�N�g



/******************************************************************************/
//	���b�Z�[�W�����p�R�[���o�b�N�֐�
//	�����F	pvUserContext	Initalize�֐��̑�����
//			dwMessageType	���b�Z�[�W�̎��
//			pMessage		���b�Z�[�W�ɕt������f�[�^
/******************************************************************************/
HRESULT	CALLBACK	MessageHandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage )
{
	switch( dwMessageType )
	{
		//	�v���C���[���Z�b�V�����ɎQ��
		case	DPN_MSGID_CREATE_PLAYER:
			return DPN_OK;

		//	�v���C���[���Z�b�V��������ؒf
		case	DPN_MSGID_DESTROY_PLAYER:
			return DPN_OK;

		//	�v���C���[���烁�b�Z�[�W��M
		case	DPN_MSGID_RECEIVE:
			return DPN_OK;
	}

	return DPN_OK;
}

/******************************************************************************/
//	�f�o�C�X�쐬
/******************************************************************************/
void	CreatePlayDevice( void )
{
	HRESULT hr;

	//	�I�u�W�F�N�g�쐬
	hr = CoCreateInstance( CLSID_DirectPlay8Peer, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8Peer, (void **)&lpDPlayPeer );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "IDirectPlay8Peer�̍쐬�Ɏ��s", hr );
		return;
	}

	//	�I�u�W�F�N�g������
	hr = lpDPlayPeer->Initialize( NULL, &MessageHandler, 0 );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "IDirectPlay8Peer�̏������Ɏ��s", hr );
		return;
	}

	//	�A�h���X�I�u�W�F�N�g�쐬
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8Address, (void **)&lpDPlayDeviceInfo );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "lpDPlayDeviceInfo�̏������Ɏ��s", hr );
		return;
	}

	//	�T�[�r�X�E�v���o�C�_�̐ݒ�
	hr = lpDPlayDeviceInfo->SetSP( &CLSID_DP8SP_TCPIP );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "�T�[�r�X�E�v���o�C�_�̐ݒ�Ɏ��s", hr );
		return;
	}

	//	�z�X�g���̐ݒ�
	char *HostName = CONNECT_IP;
	hr = lpDPlayDeviceInfo->AddComponent(
		DPNA_KEY_HOSTNAME,
		HostName,
		(DWORD)strlen(HostName)+1,
		DPNA_DATATYPE_STRING_ANSI
		);
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "�z�X�g�̐ݒ�Ɏ��s", hr );
		return;
	}

	//	�|�[�g�̐ݒ�
	DWORD	Port = CONNECT_PORT;
	hr = lpDPlayDeviceInfo->AddComponent(
		DPNA_KEY_PORT,
		&Port,
		(DWORD)sizeof(Port),
		DPNA_DATATYPE_DWORD
		);
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "�|�[�g�̐ݒ�Ɏ��s", hr );
		return;
	}

}

