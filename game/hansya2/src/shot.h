/************************************************************************************************/
//	�V���b�g���C��
//		�w�b�_�t�@�C��
/************************************************************************************************/


#ifndef		__SHOT_H__
#define		__SHOT_H__



//----------------------------------------------------------------------------------------------//
//	�f�t�F�C��
//----------------------------------------------------------------------------------------------//

#define	SHOT_MAX	256

enum
{
	SHOT_ATR_NORMAL = 0,
	SHOT_ATR_HOMING,
};



//----------------------------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	�V���b�g�\����
typedef	struct
{
	//	�t���O
	u_char	use;		//	�g�p�t���O
	u_char	disp;		//	�\���t���O

	//	�ԍ�
	long	pol_no;		//	�|���S���ԍ�

	//	���W�n
	double	px,py,pz;	//	�\�����W
	double	bx,by,bz;	//	�\�����W�i�o�b�N�A�b�v�j
	double	mx,my;		//	�ړ���
	short	w,h;		//	�T�C�Y

	//	�����n
	u_long	atr;		//	�A�g���r���[�g(���@�A���V���b�g

	//	���̑�
	double	rot_r,rot_d;	//	����(���W�A���A�f�t���[)
	double	speed;		//	�ړ����x

	short	disp_ct;	//	�J�E���^�iuse�t���O��ON�ɂȂ��Ă���̎��ԁj
	long	ct;			//	�ėp�J�E���^
}SHOT_TBL;


/////////////////////////////////////////////////
//	�J���[�e�[�u��
typedef	struct
{
	u_char	a;
	u_char	r,g,b;
}COL_TBL;


/////////////////////////////////////////////////
//	�V���b�g�f�[�^
typedef	struct
{
	short	w,h;
	short	col;
}SHOT_INIT_TBL;


#endif


//----------------------------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------------------------//
extern	void	ShotInitAll( void );			//	�V���b�g�������i�S�́j
extern	void	ShotInit( SHOT_TBL *shot );		//	�V���b�g�������i�P�́j
extern	void	ShotLoop( void );				//	�V���b�g���[�v
extern	void	ShotMain( SHOT_TBL *shot );		//	�V���b�g���C��
extern	long	GetShotNo( void );				//	�V���b�g�󂫔ԍ��擾
extern	void	ShotRotSet( SHOT_TBL *shot );	//	�V���b�g�p�x�Z�b�g
extern	long	ShotHitChk( long no );			//	�V���b�g�̓����蔻��
extern	long	HitCheck( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );

extern	void	ShotNormalSet( double speed, double rev );
extern	void	ShotWideSet( double speed, long way, double rot_rev );
extern	void	ShotCreateSet( double speed );
extern	void	ShotSplineSet( double speed, double rot_add, double rev );
extern	void	ShotHomingSet( double speed, double rev );


//	�V���b�g�\����
extern	SHOT_TBL	SHOT[SHOT_MAX];
