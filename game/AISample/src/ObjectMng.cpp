/******************************************************************************/
//	オブジェクト用ファイル
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#ifdef	_DEBUG
#define	_CRTDBG_MAP_ALLOC
#endif

#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./ObjectMng.h"
#include	"./Main.h"

//	デバッグ用
#ifdef	_DEBUG
#include	<crtdbg.h>
#define	new ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif	//	_DEBUG

//CObjectMng *CObjectMng::m_Instance = NULL;


/******************************************************************************/
//	コンストラクタ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
CObjectMng::CObjectMng()
{
	memset( &m_List, 0x00, sizeof(m_List) );
}

/******************************************************************************/
//	デストラクタ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
CObjectMng::~CObjectMng()
{
	this->Clear();
}

/******************************************************************************/
//	インスタンス取得
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
CObjectMng	&CObjectMng::GetInstance( void )
{
	static CObjectMng Instance;
	return Instance;
}

/******************************************************************************/
//	オブジェクト全初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CObjectMng::Clear( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		this->Remove( i );
	}
}

/******************************************************************************/
//	オブジェクト削除
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CObjectMng::Remove( long ObjNo )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	if( m_List[ObjNo].PolNo != NOT )
	{
		g_LibPol.Init( m_List[ObjNo].PolNo );
	}
	memset( &m_List[ObjNo], 0x00, sizeof(m_List[ObjNo]) );
}

/******************************************************************************/
//	オブジェクト削除(タイプ別)
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CObjectMng::RemoveType( E_ObjectType Type )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		if( m_List[i].Type == Type )
		{
			this->Remove( i );
		}
	}
}

/******************************************************************************/
//	オブジェクト設定
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				pol_no		ポリゴン番号
//				x,y,z		座標
//				type		タイプ
//				atr			アトリビュート
/******************************************************************************/
void	CObjectMng::Set(long ObjNo, long PatNo,
						double PosX, double PosY, double PosZ,
						ULong Type, ULong Atr )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	memset( &m_List[ObjNo], 0x00, sizeof(m_List[ObjNo]) );

	m_List[ObjNo].Use= ON;
	m_List[ObjNo].Disp = ON;

	m_List[ObjNo].PolNo = g_LibPol.GetNo();
	m_List[ObjNo].PatNo = PatNo;

	m_List[ObjNo].PosX = PosX;
	m_List[ObjNo].PosY = PosY;
	m_List[ObjNo].PosZ = PosZ;

	m_List[ObjNo].Scale = 1.0;
	m_List[ObjNo].RotDeg = 0;

	m_List[ObjNo].Type = Type;
	m_List[ObjNo].Atr = Atr;

	g_LibPol.SetPat( m_List[ObjNo].PolNo, PatNo, PosX,PosY,PosZ, ATR_NONE );
}

/******************************************************************************/
//	オブジェクト設定
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				pol_no		ポリゴン番号
//				pat_no		パターン番号
//				x,y,z		座標
//				type		タイプ
//				atr			アトリビュート
/******************************************************************************/
void	CObjectMng::Set(long ObjNo,
						long Width, long Height,
						double PosX, double PosY, double PosZ,
						ULong Type, ULong Atr )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	memset( &m_List[ObjNo], 0x00, sizeof(m_List[ObjNo]) );

	m_List[ObjNo].Use= ON;
	m_List[ObjNo].Disp = ON;

	m_List[ObjNo].PolNo = g_LibPol.GetNo();
	m_List[ObjNo].PatNo = NOT;

	m_List[ObjNo].PosX = PosX;
	m_List[ObjNo].PosY = PosY;
	m_List[ObjNo].PosZ = PosZ;

	m_List[ObjNo].Scale = 1.0;

	m_List[ObjNo].Type = Type;
	m_List[ObjNo].Atr = Atr;

	g_LibPol.Set( m_List[ObjNo].PolNo, Width, Height, PosX,PosY,PosZ, ATR_BOX );
}

/******************************************************************************/
//	オブジェクトの表示座標セット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				x,y,z		表示座標
/******************************************************************************/
void	CObjectMng::SetPos( long ObjNo, double PosX, double PosY, double PosZ )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	m_List[ObjNo].PosX = PosX;
	m_List[ObjNo].PosY = PosY;
	m_List[ObjNo].PosZ = PosZ;

	g_LibPol.SetPos( m_List[ObjNo].PolNo, PosX,PosY );
}

/******************************************************************************/
//	中心座標のセット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				cx,cy		中心座標
/******************************************************************************/
void	CObjectMng::SetCenter( long ObjNo, short CenterX, short CenterY )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	m_List[ObjNo].CenterX = CenterX;
	m_List[ObjNo].CenterY = CenterY;

	g_LibPol.SetCenter( m_List[ ObjNo ].PolNo, CenterX, CenterY );
}

/******************************************************************************/
//	サイズ設定
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				cx,cy		中心座標
/******************************************************************************/
void	CObjectMng::SetSize( long ObjNo, short Width, short Height )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	g_LibPol.SetSize( m_List[ObjNo].PolNo, Width, Height );
}

/******************************************************************************/
//	拡大率のセット
//		戻り値：
//				なし
//		引数：
//				obj_no	オブジェクト番号
//				scale	拡大率
/******************************************************************************/
void	CObjectMng::SetScale( long ObjNo, double Scale )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	m_List[ObjNo].Scale = Scale;

	g_LibPol.SetScale( m_List[ObjNo].PolNo, Scale );
}

/******************************************************************************/
//	色設定
//		戻り値：
//				なし
//		引数：
//				obj_no	オブジェクト番号
/******************************************************************************/
void	CObjectMng::SetColor( long ObjNo, UChar A, UChar R, UChar G, UChar B )
{
	g_LibPol.SetColor( m_List[ ObjNo ].PolNo, A, R, G, B );
}

/******************************************************************************/
//	移動量セット
//		戻り値；
//				なし
//		引数：
//				obj_no	オブジェクト番号
//				mx,my	移動量
/******************************************************************************/
void	CObjectMng::SetTransfer( long ObjNo, double MoveX, double MoveY )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	m_List[ObjNo].MoveX = MoveX;
	m_List[ObjNo].MoveY = MoveY;
}

/******************************************************************************/
//	オブジェクトアニメセット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				anm_no		アニメ番号
//				flag		フラグ
/******************************************************************************/
void	CObjectMng::SetAnm( long ObjNo, long AnmNo, long LoopFlag )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	m_List[ObjNo].AnmNo = AnmNo;
	m_List[ObjNo].LoopFlag = LoopFlag;
}

/******************************************************************************/
//	オブジェクト取得
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				anm_no		アニメ番号
//				flag		フラグ
/******************************************************************************/
ST_ObjectInfo	CObjectMng::GetObjectInfo( long ObjNo )
{
	return m_List[ObjNo];
}

ST_ObjectInfo	*CObjectMng::GetObjectInfo( void )
{
	return m_List;
}

/******************************************************************************/
//	オブジェクト設定
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				anm_no		アニメ番号
//				flag		フラグ
/******************************************************************************/
void	CObjectMng::SetObjectInfo( long ObjNo, ST_ObjectInfo Info )
{
	if( ObjNo<0 || ObjNo>=OBJ_MAX )
	{
		return;
	}

	m_List[ObjNo] = Info;
}

/******************************************************************************/
//	オブジェクト全移動
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	CObjectMng::Loop( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		if( m_List[i].Use == OFF )
		{
			continue;
		}

		//	移動
		this->Move( i );

		if( m_List[i].Disp )
		{
			//	表示
			this->Disp( i );
		}
	}
}

/******************************************************************************/
//	オブジェクト出現
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	CObjectMng::Disp( long ObjNo )
{
	//	番号
	long PolNo = m_List[ObjNo].PolNo;

	//	座標のバックアップ
	this->PosBackup( ObjNo );

	//	ポリゴン再セット
	g_LibPol.SetPos( PolNo, m_List[ObjNo].PosX, m_List[ObjNo].PosY );
	g_LibPol.SetScale( PolNo, m_List[ObjNo].Scale );
	g_LibPol.SetRotZ( PolNo, m_List[ObjNo].RotDeg );
	g_LibPol.SetCenter( PolNo, m_List[ObjNo].CenterX, m_List[ObjNo].CenterY );
	g_LibPol.ChangeDisp( PolNo, m_List[ObjNo].Disp );
}

/******************************************************************************/
//	オブジェクト番号取得
//		戻り値：
//				オブジェクト番号
//		引数：
//				なし
/******************************************************************************/
long	CObjectMng::GetObjectNo( void )
{
	long Ret = NOT;

	for( int i=0; i<OBJ_MAX; i++ )
	{
		if( m_List[i].Use == OFF )
		{
			Ret = i;
			break;
		}
	}

	return Ret;
}

void	CObjectMng::AddMovePos( long ObjNo, double AddX, double AddY )
{
	m_List[ ObjNo ].MoveX += AddX;
	m_List[ ObjNo ].MoveY += AddY;
}

void	CObjectMng::DispChange( long ObjNo, UChar Flag )
{
	m_List[ ObjNo ].Disp = Flag;
}

/******************************************************************************/
//	オブジェクト移動
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	CObjectMng::Move( long ObjNo )
{
	//	座標のバックアップ
	this->PosBackup( ObjNo );

	long PolNo = m_List[ObjNo].PolNo;
	m_List[ObjNo].PosX += m_List[ObjNo].MoveX;
	m_List[ObjNo].PosY += m_List[ObjNo].MoveY;

	RECT	Rect;
	Rect.left = 0;
	Rect.right = WIDTH;
	Rect.top = 0;
	Rect.bottom = HEIGHT;

	ST_PolygonInfo Info = g_LibPol.GetInfo( m_List[ OBJ_GAME_BG ].PolNo );
	switch( m_List[ObjNo].Type )
	{
		case	_ObjTypeEnemy:
		case	_ObjTypePlayer:
			Rect.left = (LONG)( Info.PosX - Info.CenterX );
			Rect.right = (LONG)( Info.PosX + Info.Width - Info.CenterX );
			Rect.top = (LONG)( Info.PosY - Info.CenterY );
			Rect.bottom = (LONG)( Info.PosY + Info.Height - Info.CenterY );
			break;
		case	_ObjTypeShot:
			Rect.left = -50;
			Rect.right = WIDTH + 50;
			Rect.top = -50;
			Rect.bottom = HEIGHT + 50;
			break;
	}

	//	クリッピング
	this->Clipping( ObjNo, Rect );
}

/******************************************************************************/
//	オブジェクトクリッピング
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
long	CObjectMng::Clipping( long ObjNo, RECT Rect )
{
	long	Ret = 0;

	long PolNo = m_List[ObjNo].PolNo;
	long Width = g_LibPol.GetInfo( PolNo ).Width;
	long Height = g_LibPol.GetInfo( PolNo ).Height;
	long Flag = 0;

	switch( m_List[ObjNo].Type )
	{
		case	_ObjTypeEnemy:
		case	_ObjTypePlayer:
			if( m_List[ObjNo].PosX < Rect.left )
			{
				m_List[ObjNo].PosX = Rect.left;
			}
			else if( m_List[ObjNo].PosX > Rect.right )
			{
				m_List[ObjNo].PosX = Rect.right;
			}

			if( m_List[ObjNo].PosY < Rect.top )
			{
				m_List[ObjNo].PosY = Rect.top;
			}
			else if( m_List[ObjNo].PosY > Rect.bottom )
			{
				m_List[ObjNo].PosY = Rect.bottom;
			}
			break;

		case	_ObjTypeShot:
			if( ( m_List[ObjNo].PosX < Rect.left ) || ( m_List[ObjNo].PosX > Rect.right )
				|| ( m_List[ObjNo].PosY < Rect.top ) || ( m_List[ObjNo].PosY > Rect.bottom ) )
			{
				this->Remove( ObjNo );
			}
			break;
	}

	return Ret;
}

/******************************************************************************/
//	座標バックアップ
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	CObjectMng::PosBackup( long ObjNo )
{
	m_List[ObjNo].PosXBk = m_List[ObjNo].PosX;
	m_List[ObjNo].PosYBk = m_List[ObjNo].PosY;
	m_List[ObjNo].PosZBk = m_List[ObjNo].PosZ;
	g_LibPol.PosBackup( m_List[ObjNo].PolNo );
}

/******************************************************************************/
//	座標を元に戻す
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	CObjectMng::PosRestore( long ObjNo )
{
	m_List[ObjNo].PosX = m_List[ObjNo].PosXBk;
	m_List[ObjNo].PosY = m_List[ObjNo].PosYBk;
	m_List[ObjNo].PosZ = m_List[ObjNo].PosZBk;
	g_LibPol.PosRestore( m_List[ObjNo].PolNo );
}
