/******************************************************************************/
//	�G�l�~�[����
//		�\�[�X�t�@�C��
/******************************************************************************/

#ifndef	__EnemyControl_H__
#define	__EnemyControl_H__

#include	<vector>
#include	"./ObjectMng.h"
using namespace std;

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	���
enum E_State
{
	_StateStop,		//	��~
	_StateMove,		//	�ړ�
	_StateAtack,	//	�U��
};

//	�ʒu
enum	E_Position
{
	_PosLeftUp,		//	����
	_PosCenterUp,	//	��
	_PosRightUp,	//	�E��
	_PosLeft,		//	��
	_PosCenter,		//	�^��
	_PosRight,		//	�E
	_PosLeftDown,	//	����
	_PosCenterDown,	//	��
	_PosRightDown,	//	�E��
	_PosMax,		//	�ő�l
};

//	����
enum E_Lange
{
	_LngShort,
	_LngMiddle,
	_LngLong,
};

//	�ړ��t���O
enum	E_MoveFlag
{
	_MoveFlagSet,	//	�ݒ�
	_MoveFlagLoop,	//	���[�v
};

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	���j�b�g�p
typedef	struct
{
	long	ObjectNo;			//	�I�u�W�F�N�g�ԍ�
	long	TargetObjNo;		//	�ڕW�I�u�W�F�N�g�ԍ�

	RECT	PatSize;			//	�p�^�[���T�C�Y
	short	CenterX,CenterY;	//	���S���W
	short	HitRect;			//	������͈�
	double	Speed;				//	�ړ����x

	long	Shot[5];			//	�V���b�g�ԍ�
	//	�V���b�g�t���O
	//	(0:�����ĂȂ��A1:�m�[�}���A2:����P�A3:����Q�A4:�d�w�P�A5:�d�w�Q)
	long	ButNo;
	long	ShotLv;				//	�V���b�g���x��

	long	Life;				//	���C�t
	E_State	State;				//	���
	double	Distance;			//	����
	E_Position	Pos;			//	�ʒu
	long	ShotTimer;			//	�V���b�g�^�C�}
	long	ShotCt;				//	�V���b�g�J�E���^
	E_MoveFlag	MoveFlag;			//	�ړ��t���O
	long	MoveTimer;			//	�ړ��^�C�}
	long	MoveCt;				//	�ړ��J�E���^
}ST_Enemy;

#endif	//	__EnemyControl_H__

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CEnemy
{
private:
	ST_Enemy	m_Info;
	UChar		m_InitFlag;

public:
	CEnemy();
	~CEnemy();
	void	Init( long OjbjNo, long TargetNo, double PosX, double PosY );
	void	MainLoop( void );

private:
	void	Stop( void );
	void	Move( void );
	void	Atack( void );
	void	HitChk( void );
	void	Turn( long ObjNo1, long ObjNo2 );
	vector<ST_ObjectInfo> GetShot( void );
	void	SetPostion( void );
	void	SetDistance( void );
	void	SetMove( void );
	void	SetMovePos( ST_ObjectInfo MyInfo, ST_ObjectInfo Player, double &MoveX, double &MoveY );
	void	SetEscape( ST_ObjectInfo MyInfo, vector<ST_ObjectInfo> Shot, double &MoveX, double &MoveY );
	void	LoopMove( void );
	E_Lange	GetLange( void );
	E_Lange	GetLange( double PosX1, double PosY1, double PosX2, double PosY2 );
};
