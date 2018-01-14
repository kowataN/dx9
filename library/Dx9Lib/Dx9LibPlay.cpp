/******************************************************************************/
//	DirectX9ライブラリ
//		プレイ
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
IDirectPlay8Peer		*lpDPlayPeer = NULL;		// 　ピアツーピア用
LPDIRECTPLAY8ADDRESS	lpDPlayDeviceInfo = NULL;	//	アドレスオブジェクト



/******************************************************************************/
//	メッセージ処理用コールバック関数
//	引数：	pvUserContext	Initalize関数の第一引数
//			dwMessageType	メッセージの種類
//			pMessage		メッセージに付随するデータ
/******************************************************************************/
HRESULT	CALLBACK	MessageHandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage )
{
	switch( dwMessageType )
	{
		//	プレイヤーがセッションに参加
		case	DPN_MSGID_CREATE_PLAYER:
			return DPN_OK;

		//	プレイヤーがセッションから切断
		case	DPN_MSGID_DESTROY_PLAYER:
			return DPN_OK;

		//	プレイヤーからメッセージ受信
		case	DPN_MSGID_RECEIVE:
			return DPN_OK;
	}

	return DPN_OK;
}

/******************************************************************************/
//	デバイス作成
/******************************************************************************/
void	CreatePlayDevice( void )
{
	HRESULT hr;

	//	オブジェクト作成
	hr = CoCreateInstance( CLSID_DirectPlay8Peer, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8Peer, (void **)&lpDPlayPeer );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "IDirectPlay8Peerの作成に失敗", hr );
		return;
	}

	//	オブジェクト初期化
	hr = lpDPlayPeer->Initialize( NULL, &MessageHandler, 0 );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "IDirectPlay8Peerの初期化に失敗", hr );
		return;
	}

	//	アドレスオブジェクト作成
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER,
		IID_IDirectPlay8Address, (void **)&lpDPlayDeviceInfo );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "lpDPlayDeviceInfoの初期化に失敗", hr );
		return;
	}

	//	サービス・プロバイダの設定
	hr = lpDPlayDeviceInfo->SetSP( &CLSID_DP8SP_TCPIP );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "サービス・プロバイダの設定に失敗", hr );
		return;
	}

	//	ホスト名の設定
	char *HostName = CONNECT_IP;
	hr = lpDPlayDeviceInfo->AddComponent(
		DPNA_KEY_HOSTNAME,
		HostName,
		(DWORD)strlen(HostName)+1,
		DPNA_DATATYPE_STRING_ANSI
		);
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "ホストの設定に失敗", hr );
		return;
	}

	//	ポートの設定
	DWORD	Port = CONNECT_PORT;
	hr = lpDPlayDeviceInfo->AddComponent(
		DPNA_KEY_PORT,
		&Port,
		(DWORD)sizeof(Port),
		DPNA_DATATYPE_DWORD
		);
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "ポートの設定に失敗", hr );
		return;
	}

}

