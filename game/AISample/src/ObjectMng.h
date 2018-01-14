/******************************************************************************/
//	�I�u�W�F�N�g�Ǘ��t�@�C��
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__ObjectMng_H__
#define	__ObjectMng_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//
#define	OBJ_MAX	5000

enum E_ObjectType
{
	_ObjTypeNone,	//	�Ȃ��i�����l�j
	_ObjTypeBG,		//	�w�i
	_ObjTypeSystem,	//	�V�X�e��
	_ObjTypeRect,	//	�̈�
	_ObjTypePlayer,	//	�v���C���[
	_ObjTypeEnemy,	//	�G
	_ObjTypeShot,	//	�V���b�g
};

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	�I�u�W�F�N�g�Ǘ��\����
typedef	struct
{
	//	�t���O
	UChar	Use;					//	�g�p�t���O
	UChar	Disp;					//	�\���t���O
	long	LoopFlag;				//	�A�j�����[�v�t���O

	//	�ԍ�
	long	PatNo;					//	�p�^�[���ԍ�
	long	PolNo;					//	�|���S���ԍ�
	long	AnmNo;					//	�A�j���Ǘ��ԍ�

	//	���W�n
	double	PosX,PosY,PosZ;			//	�\�����W
	double	PosXBk,PosYBk,PosZBk;	//	�o�b�N�A�b�v���W
	double	MoveX,MoveY;			//	�ړ���
	short	CenterX,CenterY;		//	���S���W

	//	�����n
	ULong	Type;					//	�^�C�v�t���O(�@�́A�V���b�g�A���̑�)
	ULong	Atr;					//	�A�g���r���[�g(���@�A���V���b�g)

	short	HitRect;				//	�����蔻��

	//	���̑�
	double	Scale;					//	�g�嗦
	double	RotRad,RotDeg;			//	����(���W�A���A�f�O���[)
	double	Speed;					//	�ړ����x
	short	Dead;					//	���Ńt���O(0:�����A1:���ŁA2:�_��)

	short	DispCt;					//	�J�E���^�iuse�t���O��ON�ɂȂ��Ă���̎��ԁj
	short	DeadCt;					//	���ŃJ�E���^
	long	Ct;						//	�ėp�J�E���^
}ST_ObjectInfo;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CObjectMng
{
protected:
	ST_ObjectInfo	m_List[OBJ_MAX];

public:
	CObjectMng();
	~CObjectMng();
	static	CObjectMng	&GetInstance( void );
	void	Clear( void );
	void	Remove( long ObjNo );
	void	RemoveType( E_ObjectType Type );
	void	Set( long ObjNo, long PatNo, double PosX, double PosY, double PosZ, ULong Type, ULong Atr );
	void	Set(long ObjNo, long Width, long Height, double PosX, double PosY, double PosZ, ULong Type, ULong Atr );
	void	SetPos( long ObjNo, double PosX, double PosY, double PosZ );
	void	SetCenter( long ObjNo, short CenterX, short CenterY );
	void	SetSize( long ObjNo, short Width, short Height );
	void	SetScale( long ObjNo, double Scale );
	void	SetColor( long ObjNo, UChar A, UChar R, UChar G, UChar B );
	void	SetTransfer( long ObjNo, double MoveX, double MoveY );
	void	SetAnm( long ObjNo, long AnmNo, long LoopFlag );
	ST_ObjectInfo	GetObjectInfo( long ObjNo );
	ST_ObjectInfo	*GetObjectInfo( void );
	void	SetObjectInfo( long ObjNo, ST_ObjectInfo Info );
	void	Loop( void );
	long	GetCount( ST_ObjectInfo Info );
	long	GetObjectNo( void );
	void	AddMovePos( long ObjNo, double AddX, double AddY );
	void	DispChange( long ObjNo, UChar Flag );

protected:
	void	Disp( long ObjNo );
	void	Move( long ObjNo );
	long	Clipping( long ObjNo, RECT Rect );
	void	PosBackup( long ObjNo );
	void	PosRestore( long ObjNo );
};

class CObjectType
{
public:
	static const E_ObjectType None = _ObjTypeNone;
	static const E_ObjectType BG = _ObjTypeBG;
	static const E_ObjectType System = _ObjTypeSystem;
	static const E_ObjectType Rect = _ObjTypeRect;
	static const E_ObjectType Player = _ObjTypePlayer;
	static const E_ObjectType Enemy = _ObjTypeEnemy;
	static const E_ObjectType Shot = _ObjTypeShot;
};

#endif	//	__ObjectMng_H__

extern class	CObjectType g_ObjType;