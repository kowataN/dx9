/******************************************************************************/
//	�I�u�W�F�N�g�p�t�@�C��
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#ifdef	_DEBUG
#define	_CRTDBG_MAP_ALLOC
#endif

#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./ObjectMng.h"
#include	"./Main.h"

//	�f�o�b�O�p
#ifdef	_DEBUG
#include	<crtdbg.h>
#define	new ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif	//	_DEBUG

//CObjectMng *CObjectMng::m_Instance = NULL;


/******************************************************************************/
//	�R���X�g���N�^
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
CObjectMng::CObjectMng()
{
	memset( &m_List, 0x00, sizeof(m_List) );
}

/******************************************************************************/
//	�f�X�g���N�^
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
CObjectMng::~CObjectMng()
{
	this->Clear();
}

/******************************************************************************/
//	�C���X�^���X�擾
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
CObjectMng	&CObjectMng::GetInstance( void )
{
	static CObjectMng Instance;
	return Instance;
}

/******************************************************************************/
//	�I�u�W�F�N�g�S������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CObjectMng::Clear( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		this->Remove( i );
	}
}

/******************************************************************************/
//	�I�u�W�F�N�g�폜
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
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
//	�I�u�W�F�N�g�폜(�^�C�v��)
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
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
//	�I�u�W�F�N�g�ݒ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				pol_no		�|���S���ԍ�
//				x,y,z		���W
//				type		�^�C�v
//				atr			�A�g���r���[�g
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
//	�I�u�W�F�N�g�ݒ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				pol_no		�|���S���ԍ�
//				pat_no		�p�^�[���ԍ�
//				x,y,z		���W
//				type		�^�C�v
//				atr			�A�g���r���[�g
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
//	�I�u�W�F�N�g�̕\�����W�Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				x,y,z		�\�����W
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
//	���S���W�̃Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				cx,cy		���S���W
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
//	�T�C�Y�ݒ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				cx,cy		���S���W
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
//	�g�嗦�̃Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no	�I�u�W�F�N�g�ԍ�
//				scale	�g�嗦
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
//	�F�ݒ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no	�I�u�W�F�N�g�ԍ�
/******************************************************************************/
void	CObjectMng::SetColor( long ObjNo, UChar A, UChar R, UChar G, UChar B )
{
	g_LibPol.SetColor( m_List[ ObjNo ].PolNo, A, R, G, B );
}

/******************************************************************************/
//	�ړ��ʃZ�b�g
//		�߂�l�G
//				�Ȃ�
//		�����F
//				obj_no	�I�u�W�F�N�g�ԍ�
//				mx,my	�ړ���
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
//	�I�u�W�F�N�g�A�j���Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				anm_no		�A�j���ԍ�
//				flag		�t���O
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
//	�I�u�W�F�N�g�擾
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				anm_no		�A�j���ԍ�
//				flag		�t���O
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
//	�I�u�W�F�N�g�ݒ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				anm_no		�A�j���ԍ�
//				flag		�t���O
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
//	�I�u�W�F�N�g�S�ړ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CObjectMng::Loop( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		if( m_List[i].Use == OFF )
		{
			continue;
		}

		//	�ړ�
		this->Move( i );

		if( m_List[i].Disp )
		{
			//	�\��
			this->Disp( i );
		}
	}
}

/******************************************************************************/
//	�I�u�W�F�N�g�o��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	CObjectMng::Disp( long ObjNo )
{
	//	�ԍ�
	long PolNo = m_List[ObjNo].PolNo;

	//	���W�̃o�b�N�A�b�v
	this->PosBackup( ObjNo );

	//	�|���S���ăZ�b�g
	g_LibPol.SetPos( PolNo, m_List[ObjNo].PosX, m_List[ObjNo].PosY );
	g_LibPol.SetScale( PolNo, m_List[ObjNo].Scale );
	g_LibPol.SetRotZ( PolNo, m_List[ObjNo].RotDeg );
	g_LibPol.SetCenter( PolNo, m_List[ObjNo].CenterX, m_List[ObjNo].CenterY );
	g_LibPol.ChangeDisp( PolNo, m_List[ObjNo].Disp );
}

/******************************************************************************/
//	�I�u�W�F�N�g�ԍ��擾
//		�߂�l�F
//				�I�u�W�F�N�g�ԍ�
//		�����F
//				�Ȃ�
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
//	�I�u�W�F�N�g�ړ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	CObjectMng::Move( long ObjNo )
{
	//	���W�̃o�b�N�A�b�v
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

	//	�N���b�s���O
	this->Clipping( ObjNo, Rect );
}

/******************************************************************************/
//	�I�u�W�F�N�g�N���b�s���O
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
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
//	���W�o�b�N�A�b�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	CObjectMng::PosBackup( long ObjNo )
{
	m_List[ObjNo].PosXBk = m_List[ObjNo].PosX;
	m_List[ObjNo].PosYBk = m_List[ObjNo].PosY;
	m_List[ObjNo].PosZBk = m_List[ObjNo].PosZ;
	g_LibPol.PosBackup( m_List[ObjNo].PolNo );
}

/******************************************************************************/
//	���W�����ɖ߂�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	CObjectMng::PosRestore( long ObjNo )
{
	m_List[ObjNo].PosX = m_List[ObjNo].PosXBk;
	m_List[ObjNo].PosY = m_List[ObjNo].PosYBk;
	m_List[ObjNo].PosZ = m_List[ObjNo].PosZBk;
	g_LibPol.PosRestore( m_List[ObjNo].PolNo );
}
