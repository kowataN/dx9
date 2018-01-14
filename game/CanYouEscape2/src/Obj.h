/******************************************************************************/
//	�I�u�W�F�N�g�p�t�@�C��
//		�\�[�X�t�@�C��
/******************************************************************************/

#ifndef	__Obj_H__
#define	__Obj_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//
#define	OBJ_MAX		5000	//	�I�u�W�F�N�g�ő吔

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	�I�u�W�F�N�g�Ǘ��\����
typedef	struct
{
	//	�t���O
	u_char	use;			//	�g�p�t���O
	u_char	disp;			//	�\���t���O
	u_char	buzz;			//	�o�Y���t���O

	//	�ԍ�
	long	pol_no;			//	�|���S���ԍ�
	long	pat_no;			//	�p�^�[���ԍ�
	long	anm_no;			//	�A�j���Ǘ��ԍ�

	//	���W�n
	double	px,py,pz;		//	�\�����W
	double	bx,by,bz;		//	�\�����W�i�o�b�N�A�b�v�j
	double	mx,my;			//	�ړ���
	short	cx,cy;			//	���S���W

	//	�����n
	u_long	type;			//	�^�C�v�t���O(�@�́A�V���b�g�A���̑�)
	u_long	atr;			//	�A�g���r���[�g(���@�A���V���b�g)

	//	�����蔻��
	short	hit_rect;		//	�����蔻��
	short	buzz_rect;		//	�o�Y������

	//	���̑�
	double	scale;			//	�g�嗦
	double	rot_r,rot_d;	//	����(���W�A���A�f�t���[)
	double	rot;			//	�p�x
	double	speed;			//	�ړ����x
	short	dead;			//	���Ńt���O(0:�����A1:���ŁA2:�_��)

	short	disp_ct;		//	�J�E���^�iuse�t���O��ON�ɂȂ��Ă���̎��ԁj
	short	dead_ct;		//	���ŃJ�E���^
	long	ct;				//	�ėp�J�E���^
}OBJ_TBL;

#endif	//	__Obj_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	OBJ_TBL	OBJ[OBJ_MAX];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	ObjInitAll( void );				//	�I�u�W�F�N�g�S������
extern	void	ObjInit( long no );				//	�I�u�W�F�N�g������
extern	void	ObjSet( long obj_no, long pol_no, long pat_no, double x, double y, double z, u_long type, u_long atr );	//	�I�u�W�F�N�g�ݒ�
extern	void	ObjPosSet( long obj_no, double x, double y, double z );		//	�\�����W�Z�b�g
extern	void	ObjCenterSet( long obj_no, short cx, short cy );	//	���S���W�̃Z�b�g
extern	void	ObjScaleSet( long obj_no, double scall );			//	�g�嗦�̃Z�b�g
extern	void	ObjAnmSet( long obj_no, long anm_no, long flag );	//	�I�u�W�F�N�g�A�j���Z�b�g
extern	void	ObjMove( void );				//	�I�u�W�F�N�g�S�ړ�
extern	void	ObjMoveLoop( OBJ_TBL *obj );	//	�I�u�W�F�N�g�ړ�
extern	void	ObjDispAll( void );				//	�I�u�W�F�N�g�S�o��
extern	void	ObjDisp( OBJ_TBL *obj );		//	�I�u�W�F�N�g�o��
extern	long	ObjClipping( long no );	//	�I�u�W�F�N�g�N���b�s���O
extern	void	ObjPosBackUp( OBJ_TBL *obj );	//	���W�o�b�N�A�b�v
extern	void	ObjPosRet( OBJ_TBL *obj );		//	���W�����ɖ߂�
extern	long	ObjGetNo( long no );			//	�󂫔ԍ��擾
