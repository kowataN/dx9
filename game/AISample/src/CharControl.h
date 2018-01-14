/******************************************************************************/
//	�I�u�W�F�N�g�p�t�@�C��
//		�\�[�X�t�@�C��
/******************************************************************************/

#ifndef	__CharControl_H__
#define	__CharControl_H__

#include	"./ObjectMng.h"

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	���j�b�g�p
typedef	struct
{
	long	ObjectNo;			//	�I�u�W�F�N�g�ԍ�

	RECT	PatSize;			//	�p�^�[���T�C�Y
	short	CenterX,CenterY;	//	���S���W
	short	HitRect;			//	������͈�
	double	Speed;				//	�ړ����x

	long	Shot[5];			//	�V���b�g�ԍ�
	//	�V���b�g�t���O
	//	(0:�����ĂȂ��A1:�m�[�}���A2:����P�A3:����Q�A4:�d�w�P�A5:�d�w�Q)
	long	ButNo;
	long	ShotFlag;
	long	ShotLv;				//	�V���b�g���x��

	long	Life;				//	���C�t
	long	LockEnemy;			//	�U���Ώۂ̓G
}UNIT_TBL;

#endif	//	__CharControl_H__

//----------------------------------------------------------------------------//
//	�O���錾
//----------------------------------------------------------------------------//
extern	UNIT_TBL	UNIT;

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	CharInit( void );
extern	void	CharMainLoop( void );
extern	void	CharMove( void );
extern	void	CharHitChk( void );
extern	void	CharTurn( long ObjNo1, long ObjNo2 );
