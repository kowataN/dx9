/******************************************************************************/
//	オブジェクト情報ファイル
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./ObjectInfo.h"



/******************************************************************************/
//	コンストラクタ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
CObjectInfo::CObjectInfo()
{
	memset( &m_Info, 0x00, sizeof(m_Info) );
}

/******************************************************************************/
//	デストラクタ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
CObjectInfo::~CObjectInfo()
{
}

/******************************************************************************/
//	オブジェクト設定
//		戻り値：
//				なし
//		引数：
//				pat_no		パターン番号
//				x,y,z		座標
//				type		タイプ
//				atr			アトリビュート
/******************************************************************************/
void	CObjectInfo::Set( long PatNo,
						 double PosX, double PosY, double PosZ,
						 ULong Type,
						 ULong Atr )
{
	//	フラグ
	m_Info.Disp = ON;

	//	番号
	m_Info.PolNo = GetPolygonNo();
	m_Info.PatNo = PatNo;

	//	座標
	m_Info.PosX = PosX;
	m_Info.PosY = PosY;
	m_Info.PosZ = PosZ;

	//	属性
	m_Info.Type = Type;
	m_Info.Atr = Atr;

	//	当り判定
	m_Info.HitRect = 0;

	//	その他
	m_Info.Scale = 1.0;
	m_Info.RotRad = m_Info.RotDeg = 0;
	m_Info.Rot = 0;
	m_Info.Speed = 0;
	m_Info.Dead = 0;
	m_Info.DeadCt = 0;
	m_Info.Ct = 0;

	//	ポリゴンセット
	SetPolygonPat( m_Info.PolNo, PatNo, PosX,PosY,PosZ, ATR_NONE );
}

/******************************************************************************/
/******************************************************************************/
void	CObjectInfo::Set(long Width, long Height,
						 double PosX, double PosY, double PosZ,
						 ULong Type, ULong Atr )
{
}

/******************************************************************************/
//	オブジェクトの表示座標セット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				x,y,z		表示座標
/******************************************************************************/
void	CObjectInfo::SetPos( double PosX, double PosY, double PosZ )
{
	m_Info.PosX = PosX;
	m_Info.PosY = PosY;
	m_Info.PosZ = PosZ;

	long PolNo = m_Info.PolNo;
	POL[ PolNo ].PosX = PosX;
	POL[ PolNo ].PosY = PosY;
	POL[ PolNo ].PosZ = PosZ;
}

/******************************************************************************/
//	中心座標のセット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				cx,cy		中心座標
/******************************************************************************/
void	CObjectInfo::SetCenter( short CenterX, short CenterY )
{
	m_Info.CenterX = CenterX;
	m_Info.CenterY = CenterY;

	long PolNo = m_Info.PolNo;
	POL[ PolNo ].CenterX = CenterX;
	POL[ PolNo ].CenterY = CenterY;
}

/******************************************************************************/
//	拡大率のセット
//		戻り値：
//				なし
//		引数：
//				obj_no	オブジェクト番号
//				scale	拡大率
/******************************************************************************/
void	CObjectInfo::SetScale( double Scale )
{
	m_Info.Scale = Scale;

	SetPolygonScale( m_Info.PolNo, Scale );
}

/******************************************************************************/
//	移動量セット
//		戻り値；
//				なし
//		引数：
//				obj_no	オブジェクト番号
//				mx,my	移動量
/******************************************************************************/
void	CObjectInfo::SetTransfer( double MoveX, double MoveY )
{
	m_Info.MoveX = MoveX;
	m_Info.MoveY = MoveY;
}

/******************************************************************************/
//	オブジェクトアニメセット
//		戻り値：
//				なし
//		引数：
//				anm_no		アニメ番号
//				flag		フラグ
/******************************************************************************/
void	CObjectInfo::SetAnm( long AnmNo, long LoopFlag )
{
	m_Info.AnmNo = AnmNo;
	m_Info.LoopFlag = LoopFlag;
}

/******************************************************************************/
//	オブジェクト移動
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	CObjectInfo::Move( void )
{
	//	座標のバックアップ
	this->PosBackup();

	m_Info.PosX += m_Info.MoveX;
	m_Info.PosY += m_Info.MoveY;

	RECT	Rect;
	Rect.left = 0;
	Rect.right = WIDTH;
	Rect.top = 0;
	Rect.bottom = HEIGHT;

	switch( m_Info.Type )
	{
		case	_ObjTypePlayer:
			Rect.left = 100;
			Rect.right = 460;
			Rect.top = 0;
			Rect.bottom = HEIGHT;
			break;
		case	_ObjTypeShot:
			Rect.left = -50;
			Rect.right = WIDTH + 50;
			Rect.top = -50;
			Rect.bottom = HEIGHT + 50;
			break;
	}

	//	クリッピング
	this->Clipping(Rect);
}

/******************************************************************************/
//	オブジェクト出現
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	CObjectInfo::Disp( void )
{
	//	番号
	long PatNo = m_Info.PatNo;
	long PolNo = m_Info.PolNo;

	//	座標
	double PosX = m_Info.PosX;
	double PosY = m_Info.PosY;
	double PosZ = m_Info.PosZ;

	//	座標のバックアップ
	this->PosBackup();

	//	ポリゴン再セット
	SetPolygonPat( PolNo, PatNo, PosX,PosY,PosZ, ATR_NONE );

	POL[ PolNo ].Disp = m_Info.Disp;

	//	拡大率
	SetPolygonScale( PolNo, m_Info.Scale );

	//	角度
	POL[ PolNo ].Rot = m_Info.Rot;

	//	中心座標
	SetPolygonCenter( PolNo, m_Info.CenterX, m_Info.CenterY );
}

/******************************************************************************/
//	オブジェクトクリッピング
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
long	CObjectInfo::Clipping( RECT Rect )
{
	long	Ret = 0;

	long PolNo = m_Info.PolNo;
	long Width = POL[ PolNo ].Width;
	long Height = POL[ PolNo ].Height;
	long Flag = 0;

	switch( m_Info.Type )
	{
		case	_ObjTypePlayer:
			if( m_Info.PosX < Rect.left )
			{
				m_Info.PosX = Rect.left;
			}
			else if( m_Info.PosX > Rect.right )
			{
				m_Info.PosX = Rect.right;
			}

			if( m_Info.PosY < Rect.top )
			{
				m_Info.PosY = Rect.top;
			}
			else if( m_Info.PosY > Rect.bottom )
			{
				m_Info.PosY = Rect.bottom;
			}
			break;

		case	_ObjTypeShot:
			if( ( m_Info.PosX < Rect.left ) || ( m_Info.PosX > Rect.right )
				|| ( m_Info.PosY < Rect.top ) || ( m_Info.PosY > Rect.bottom ) )
			{
				memset( &m_Info, 0x00, sizeof(m_Info) );
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
void	CObjectInfo::PosBackup( void )
{
	m_Info.PosXBk = m_Info.PosX;
	m_Info.PosYBk = m_Info.PosY;
	m_Info.PosZBk = m_Info.PosZ;
}

/******************************************************************************/
//	座標を元に戻す
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	CObjectInfo::PosRestore( void )
{
	m_Info.PosX = m_Info.PosXBk;
	m_Info.PosY = m_Info.PosYBk;
	m_Info.PosZ = m_Info.PosZBk;
}

ST_ObjectInfo	CObjectInfo::GetObectInfo( void )
{
	return m_Info;
}

void	CObjectInfo::SetObjectInfo( ST_ObjectInfo Info )
{
	m_Info = Info;
}
