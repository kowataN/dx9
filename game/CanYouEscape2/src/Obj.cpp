/******************************************************************************/
//	�I�u�W�F�N�g�p�t�@�C��
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./Obj.h"
#include	"./GameMain.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
OBJ_TBL	OBJ[OBJ_MAX];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
void	ObjInitAll( void );				//	�I�u�W�F�N�g�S������
void	ObjInit( long no );				//	�I�u�W�F�N�g������
void	ObjSet( long obj_no, long pol_no, long pat_no, double x, double y, double z, u_long type, u_long atr );
void	ObjPosSet( long obj_no, double x, double y, double z );		//	�\�����W�Z�b�g
void	ObjCenterSet( long obj_no, short cx, short cy );	//	���S���W�Z�b�g
void	ObjScaleSet( long obj_no, double scale );			//	�g�嗦�Z�b�g
void	ObjMovePosSet( long obj_no, double mx, double my );	//	�ړ��ʃZ�b�g
void	ObjAnmSet( long obj_no, long anm_no, long flag );	//	�I�u�W�F�N�g�A�j���Z�b�g
void	ObjMove( void );				//	�I�u�W�F�N�g�S�ړ�
void	ObjMoveLoop( long no );			//	�I�u�W�F�N�g�ړ�
void	ObjDispAll( void );				//	�I�u�W�F�N�g�S�o��
void	ObjDisp( OBJ_TBL *obj );		//	�I�u�W�F�N�g�o��
long	ObjClipping( long no );			//	�I�u�W�F�N�g�N���b�s���O
void	ObjPosBackUp( OBJ_TBL *obj );	//	���W�o�b�N�A�b�v
void	ObjPosRet( OBJ_TBL *obj );		//	���W�����ɖ߂�
long	ObjGetNo( long no );			//	�󂫔ԍ��擾
void	ObjDelete( short no, long time );	//	�I�u�W�F�N�g����



/******************************************************************************/
//	�I�u�W�F�N�g�S������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	ObjInitAll( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		ObjInit( i );
	}
}

/******************************************************************************/
//	�I�u�W�F�N�g������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	ObjInit( long no )
{
	POL[ no ].Disp = OFF;

	//	�t���O
	OBJ[ no ].use = OFF;				//	�g�p
	OBJ[ no ].disp = OFF;				//	�\��
	OBJ[ no ].buzz = OFF;				//	�o�Y��

	//	�ԍ�
	OBJ[ no ].pol_no = NOT;
	OBJ[ no ].pat_no = NOT;
	OBJ[ no ].anm_no = NOT;

	//	���W
	ObjPosSet( no, 0,0,0 )	;	//	�\�����W
	ObjPosBackUp( &OBJ[ no ] );	//	�o�b�N�A�b�v���W
	ObjMovePosSet( no, 0,0 );	//	�ړ���
	ObjCenterSet( no, 0,0 );	//	���S���W

	//	����
	OBJ[ no ].type = 0;			//	�^�C�v
	OBJ[ no ].atr = OBJ_ATR_NONE;	//	�A�g���r���[�g

	//	�����蔻��
	OBJ[ no ].hit_rect = 0;
	OBJ[ no ].buzz_rect = 0;

	//	���̑�
	OBJ[ no ].scale = 1.0;	//	�g�嗦
	OBJ[ no ].rot_r = OBJ[ no ].rot_d = 0;	//	����
	OBJ[ no ].rot = 0;		//	�p�x
	OBJ[ no ].speed = 0;		//	�X�s�[�h
	OBJ[ no ].dead = 0;		//	���S�t���O
	OBJ[ no ].dead_ct = 0;	//	���S�J�E���^
	OBJ[ no ].ct = 0;		//	�ėp�J�E���^
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
void	ObjSet( long obj_no, long pol_no, long pat_no, double x, double y, double z, u_long type, u_long atr )
{
	//	�t���O
	OBJ[ obj_no ].use = ON;
	OBJ[ obj_no ].disp = ON;

	//	�ԍ�
	OBJ[ obj_no ].pol_no = pol_no;
	OBJ[ obj_no ].pat_no = pat_no;

	//	���W
	OBJ[ obj_no ].px = x;
	OBJ[ obj_no ].py = y;
	OBJ[ obj_no ].pz = z;

	//	����
	OBJ[ obj_no ].type = type;
	OBJ[ obj_no ].atr = atr;

	//	�|���S���Z�b�g
	SetPolygonPat( pol_no, pat_no, x,y,z, ATR_NONE );
}

/******************************************************************************/
//	�I�u�W�F�N�g�̕\�����W�Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				x,y,z		�\�����W
/******************************************************************************/
void	ObjPosSet( long obj_no, double x, double y,double z )
{
	OBJ[ obj_no ].px = x;
	OBJ[ obj_no ].py = y;
	OBJ[ obj_no ].pz = z;

	long pol = OBJ[ obj_no ].pol_no;
	POL[ pol ].PosX = x;
	POL[ pol ].PosY = y;
	POL[ pol ].PosZ = z;
}

/******************************************************************************/
//	���S���W�̃Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no		�I�u�W�F�N�g�ԍ�
//				cx,cy		���S���W
/******************************************************************************/
void	ObjCenterSet( long obj_no, short cx, short cy )
{
	OBJ[ obj_no ].cx = cx;
	OBJ[ obj_no ].cy = cy;

	long pol = OBJ[ obj_no ].pol_no;
	POL[ pol ].CenterX = cx;
	POL[ pol ].CenterY = cy;
}

/******************************************************************************/
//	�g�嗦�̃Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				obj_no	�I�u�W�F�N�g�ԍ�
//				scale	�g�嗦
/******************************************************************************/
void	ObjScaleSet( long obj_no, double scale )
{
	OBJ[ obj_no ].scale = scale;

	long pol = OBJ[ obj_no ].pol_no;
	SetPolygonScale( pol, scale );
}

/******************************************************************************/
//	�ړ��ʃZ�b�g
//		�߂�l�G
//				�Ȃ�
//		�����F
//				obj_no	�I�u�W�F�N�g�ԍ�
//				mx,my	�ړ���
/******************************************************************************/
void	ObjMovePosSet( long obj_no, double mx, double my )
{
	OBJ[ obj_no ].mx = mx;
	OBJ[ obj_no ].my = my;
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
void	ObjAnmSet( long obj_no, long anm_no, long flag )
{
}

/******************************************************************************/
//	�I�u�W�F�N�g�S�ړ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	ObjMove( void )
{
	for( long i=0; i<OBJ_MAX; i++ )
	{
		ObjMoveLoop( i );
	}
}

/******************************************************************************/
//	�I�u�W�F�N�g�ړ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	ObjMoveLoop( long no )
{
	ObjPosBackUp( &OBJ[ no ] );

	OBJ[ no ].px += OBJ[ no ].mx;
	OBJ[ no ].py += OBJ[ no ].my;

	ObjClipping( no );
}

/******************************************************************************/
//	�I�u�W�F�N�g�S�o��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	ObjDispAll( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		if( OBJ[ i ].use )
		{
			ObjDisp( &OBJ[ i ] );
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
void	ObjDisp( OBJ_TBL *obj )
{
	//	�ꎞ�i�[�p
	long	pol,pat;
	double	x,y,z;

	//	�ԍ�
	pat = obj->pat_no;
	pol = obj->pol_no;

	//	���W
	x = obj->px;
	y = obj->py;
	z = obj->pz;

	//	���W�̃o�b�N�A�b�v
	obj->bx = obj->px;
	obj->by = obj->py;
	obj->bz = obj->pz;

	//	�|���S���ăZ�b�g
	SetPolygonPat( pol, pat, x,y,z, ATR_NONE );

	POL[ pol ].Disp = obj->disp;

	//	�g�嗦
	POL[ pol ].ScaleX = obj->scale;
	POL[ pol ].ScaleY = obj->scale;

	//	�p�x
	POL[ pol ].Rot = obj->rot;

	//	���S���W
	POL[ pol ].CenterX = obj->cx;
	POL[ pol ].CenterY = obj->cy;
}

/******************************************************************************/
//	�I�u�W�F�N�g�N���b�s���O
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
long	ObjClipping( long no )
{
	long	ret = 0;
	long	w,h,flag;


	w = POL[ OBJ[ no ].pol_no ].Width;
	h = POL[ OBJ[ no ].pol_no ].Height;

	flag = 0;

	switch( OBJ[ no ].type )
	{
		case	OBJ_TYPE_UNIT:
			if( OBJ[ no ].px < 0 )
			{
				OBJ[ no ].px = 0;
			}
			else if( OBJ[ no ].px > WIDTH )
			{
				OBJ[ no ].px = WIDTH;
			}

			if( OBJ[ no ].py < 0 )
			{
				OBJ[ no ].py = 0;
			}
			else if( OBJ[ no ].py > HEIGHT )
			{
				OBJ[ no ].py = HEIGHT;
			}
			break;

		case	OBJ_TYPE_SHOT:
			if( ( OBJ[ no ].px < -50 ) || ( OBJ[ no ].px > 690 )
			|| ( OBJ[ no ].py < -50 ) || ( OBJ[ no ].py > 530 ) )
			{
				ObjInit( no );
			}
			break;
	}

	return ret;
}

/******************************************************************************/
//	���W�o�b�N�A�b�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	ObjPosBackUp( OBJ_TBL *obj )
{
	obj->bx = obj->px;
	obj->by = obj->py;
	obj->bz = obj->pz;
}

/******************************************************************************/
//	���W�����ɖ߂�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̃A�h���X
/******************************************************************************/
void	ObjPosRet( OBJ_TBL *obj )
{
	obj->px = obj->bx;
	obj->py = obj->by;
}

/******************************************************************************/
//	�󂫔ԍ��擾
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj	�I�u�W�F�N�g�̔ԍ�
/******************************************************************************/
long	ObjGetNo( long no )
{
	long	i;

	for( i=no; i<OBJ_MAX; i++ )
	{
		if( OBJ[i].use == OFF )
		{
			return i;
		}
	}

	return NOT;
}

/******************************************************************************/
//	�I�u�W�F�N�g����
//		�߂�l�F
//				�Ȃ�
//		�����F
//				time		���ł܂ł̎��ԁi�t���[�����j
/******************************************************************************/
void	ObjDelete( short no, long time )
{
	if( OBJ[ no ].disp == OFF )
	{
		return;
	}
}
