/******************************************************************************/
//	�I�u�W�F�N�g���t�@�C��
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./ObjectInfo.h"



/******************************************************************************/
//	�R���X�g���N�^
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
CObjectInfo::CObjectInfo()
{
	memset( &m_Info, 0x00, sizeof(m_Info) );
}

/******************************************************************************/
//	�f�X�g���N�^
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
CObjectInfo::~CObjectInfo()
{
}

/******************************************************************************/
//	�I�u�W�F�N�g�ݒ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				pat_no		�p�^�[���ԍ�
//				x,y,z		���W
//				type		�^�C�v
//				atr			�A�g���r���[�g
/******************************************************************************/
void	CObjectInfo::Set( long PatNo,
						 double PosX, double PosY, double PosZ,
						 ULong Type,
						 ULong Atr )
{
	//	�t���O
	m_Info.Disp = ON;

	//	�ԍ�
	m_Info.PolNo = GetPolygonNo();
	m_Info.PatNo = PatNo;

	//	���W
	m_Info.PosX = PosX;
	m_Info.PosY = PosY;
	m_Info.PosZ = PosZ;

	//	����
	m_Info.Type = Type;
	m_Info.Atr = Atr;

	//	���蔻��
	m_Info.HitRect = 0;

	//	���̑�
	m_Info.Scale = 1.0;
	m_Info.RotRad = m_Info.RotDeg = 0;
	m_Info.Rot = 0;
	m_Info.Speed = 0;
	m_Info.Dead = 0;
	m_Info.DeadCt = 0;
	m_Info.Ct = 0;

	//	�|���S���Z�b�g
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
//	�I�u�W�F�N�g�̕\�����W�Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				x,y,z		�\�����W
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
//	���S���W�̃Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				cx,cy		���S���W
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
//	�g�嗦�̃Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no	�I�u�W�F�N�g�ԍ�
//				scale	�g�嗦
/******************************************************************************/
void	CObjectInfo::SetScale( double Scale )
{
	m_Info.Scale = Scale;

	SetPolygonScale( m_Info.PolNo, Scale );
}

/******************************************************************************/
//	�ړ��ʃZ�b�g
//		�߂�l�G
//				�Ȃ�
//		�����F
//				obj_no	�I�u�W�F�N�g�ԍ�
//				mx,my	�ړ���
/******************************************************************************/
void	CObjectInfo::SetTransfer( double MoveX, double MoveY )
{
	m_Info.MoveX = MoveX;
	m_Info.MoveY = MoveY;
}

/******************************************************************************/
//	�I�u�W�F�N�g�A�j���Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				anm_no		�A�j���ԍ�
//				flag		�t���O
/******************************************************************************/
void	CObjectInfo::SetAnm( long AnmNo, long LoopFlag )
{
	m_Info.AnmNo = AnmNo;
	m_Info.LoopFlag = LoopFlag;
}

/******************************************************************************/
//	�I�u�W�F�N�g�ړ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	CObjectInfo::Move( void )
{
	//	���W�̃o�b�N�A�b�v
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

	//	�N���b�s���O
	this->Clipping(Rect);
}

/******************************************************************************/
//	�I�u�W�F�N�g�o��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	CObjectInfo::Disp( void )
{
	//	�ԍ�
	long PatNo = m_Info.PatNo;
	long PolNo = m_Info.PolNo;

	//	���W
	double PosX = m_Info.PosX;
	double PosY = m_Info.PosY;
	double PosZ = m_Info.PosZ;

	//	���W�̃o�b�N�A�b�v
	this->PosBackup();

	//	�|���S���ăZ�b�g
	SetPolygonPat( PolNo, PatNo, PosX,PosY,PosZ, ATR_NONE );

	POL[ PolNo ].Disp = m_Info.Disp;

	//	�g�嗦
	SetPolygonScale( PolNo, m_Info.Scale );

	//	�p�x
	POL[ PolNo ].Rot = m_Info.Rot;

	//	���S���W
	SetPolygonCenter( PolNo, m_Info.CenterX, m_Info.CenterY );
}

/******************************************************************************/
//	�I�u�W�F�N�g�N���b�s���O
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
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
//	���W�o�b�N�A�b�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	CObjectInfo::PosBackup( void )
{
	m_Info.PosXBk = m_Info.PosX;
	m_Info.PosYBk = m_Info.PosY;
	m_Info.PosZBk = m_Info.PosZ;
}

/******************************************************************************/
//	���W�����ɖ߂�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
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
