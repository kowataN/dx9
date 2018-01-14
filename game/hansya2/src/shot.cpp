/************************************************************************************************/
//	�V���b�g���C��
//		�\�[�X�t�@�C��
/************************************************************************************************/


//----------------------------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------------------------//
#include	"dx8_lib.h"
#include	"dx8_anm.h"
#include	"dx8_pad.h"

#include	"main.h"
#include	"shot.h"



//----------------------------------------------------------------------------------------------//
//	�f�t�@�C��
//----------------------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------------------//
//	�O���[�o��
//----------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	�J���[�e�[�u��
COL_TBL	ColTbl[] =
{
	//	0
	{	255,255,0,0	},
	//	1
	{	255,0,255,0	},
	//	2
	{	255,0,0,255	},
	//	3
	{	255,255,255,0 },
};


//----------------------------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------------------------//

//	�V���b�g�\����
SHOT_TBL	SHOT[SHOT_MAX];

SHOT_INIT_TBL	SIT[SHOT_MAX] =
{
	//	5way
	{ 24,24, 0 },
	{ 32,16, 0 },
	{ 40,8, 0 },
	{ 16,32, 0 },
	{ 8,40, 0 },

	//	5way
	{ 16,16, 1 },
	{ 16,16, 1 },
	{ 16,16, 1 },
	{ 16,16, 1 },
	{ 16,16, 1 },

	//	3way
	{ 32,32, 2 },
	{ 32,32, 2 },
	{ 32,32, 2 },

	//	1way
	{ 128,128, 3 },

	//	�I�[
	{	-1	},
};




//----------------------------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------------------------//
void	ShotInitAll( void );			//	�V���b�g�������i�S�́j
void	ShotInit( SHOT_TBL *shot );		//	�V���b�g�������i�P�́j
void	ShotLoop( void );				//	�V���b�g���[�v
void	ShotMain( long no );			//	�V���b�g���C��
long	ShotHitChk( long no );			//	�V���b�g�̓����蔻��
void	ShotDirSet( long no );			//	�p�x�␳

void	ShotSet( double x, double y, double rot, double speed );

void	ShotNormalSet( double speed, double rev );
void	ShotWideSet( double speed, long way, double rot_rev );
void	ShotCreateSet( double speed );
void	ShotSplineSet( double speed, double rot_add, double rev );
void	ShotHomingSet( double speed, double rev );

void	ShotSetUpLoop( long no );
void	ShotHomingLoop( long no );
void	ShotSplineLoop( long no );

short	ShotSearch( u_long atr );

long	ShotGetNo( long no );

long	HitCheck( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
void	WallHitCheck( long no );





/************************************************************************************************/
//	�V���b�g�������i�S�́j
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/************************************************************************************************/
void	ShotInitAll( void )
{
	short	i;

	for( i=0; i<SHOT_MAX; i++ )
		ShotInit( &SHOT[i] );
}



/************************************************************************************************/
//	�V���b�g�������i�P�́j
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�V���b�g�ԍ�
/************************************************************************************************/
void	ShotInit( SHOT_TBL *shot )
{
	shot->use = OFF;
	shot->disp = OFF;

	shot->atr = 0;

	shot->px = shot->py = 0;
	shot->bx = shot->by = 0;
	shot->mx = shot->my = 0;

	shot->ct = 0;

	shot->disp_ct = 0;

	shot->h = shot->w = 0;

	PolyInit( shot->pol_no );
	shot->pol_no = 0;
}



/************************************************************************************************/
//	�V���b�g���[�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/************************************************************************************************/
void	ShotLoop( void )
{
	short	i;

	for( i=0; i<SHOT_MAX; i++ )
	{
		if( !SHOT[ i ].use )
			continue;

		WallHitCheck( i );
		ShotMain( i );
/*
		if( ShotHitChk(i) )
		{
			TXT[1].disp = ON;
			Mode ++;
		}
*/
	}
}



/************************************************************************************************/
//	�V���b�g���C��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/************************************************************************************************/
void	ShotMain( long no )
{
	SHOT[ no ].px += SHOT[ no ].mx;
	SHOT[ no ].py += SHOT[ no ].my;
	POL[ no ].px = SHOT[ no ].px;
	POL[ no ].py = SHOT[ no ].py;

	//	�ėp�J�E���^���Z
	SHOT[ no ].disp_ct ++;
}



/************************************************************************************************/
//	�V���b�g�����蔻��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/************************************************************************************************/
long	ShotHitChk( long no )
{
	double	x1,y1,x2,y2,x3,y3,x4,y4;


	//	���W�P�擾
	x1 = SHOT[ no ].px;
	y1 = SHOT[ no ].py;
	x2 = x1 + SHOT[ no ].w;
	y2 = y1 + SHOT[ no ].h;

	//	���W�Q�擾
	x3 = POL[ SHOT_MAX ].px;
	y3 = POL[ SHOT_MAX ].py;
	x4 = x3 + POL[ SHOT_MAX ].w;
	y4 = y3 + POL[ SHOT_MAX ].h;


	//	�����蔻��
	if( HitCheck( x1,y1,x2,y2, x3,y3,x4,y4  ) )
	{
		ShotInit( &SHOT[no] );
		return( 1 );
	}

	return( 0 );
}



/************************************************************************************************/
//	�p�x�␳
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/************************************************************************************************/
void	ShotDirSet( long no )
{
	//	�ړ��ʎ擾
	SHOT[ no ].mx = MoveGetX( SHOT[no].speed, SHOT[no].rot_r );
	SHOT[ no ].my = MoveGetY( SHOT[no].speed, SHOT[no].rot_r );
}



/************************************************************************************************/
//	�V���b�g�Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				x,y			���W
//				rot			�p�x
//				speed		�e��
/************************************************************************************************/
void	ShotSet( double x, double y, double rot, double speed )
{
	long	shot;
	short	w,h,col;

	shot = ShotGetNo( 0 );

	//	�p�x�␳
	if( rot > 360 )
		rot -= 360;
	else if( rot < 0 )
		rot += 360;

	SHOT[ shot ].use = ON;

	SHOT[ shot ].speed = speed;

	SHOT[ shot ].rot_d = rot;
	SHOT[ shot ].rot_r = DefToRad( rot );

	SHOT[ shot ].ct = 0;

	SHOT[ shot ].w = SIT[ shot ].w;
	SHOT[ shot ].h = SIT[ shot ].h;

	ShotDirSet( shot );

	w = SIT[ shot ].w;
	h = SIT[ shot ].h;
	col = SIT[ shot ].col;
	PolySet( shot, w,h, x,y,0.5, ATR_NONE );
	PolyColSet( shot, ColTbl[col].a,ColTbl[col].r,ColTbl[col].g,ColTbl[col].b );
}



/************************************************************************************************/
//	�m�[�}���V���b�g�Z�b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				speed		�e��
//				atr			�A�g���r���[�g
/************************************************************************************************/
void	ShotNormalSet( double speed, double rev )
{
	double	rot,x,y;

	//	�p�x
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot += DefToRad( rev );

	//	�V���b�g����
	x = 0 + cos( rot );
	y = 0 - sin( rot );
	ShotSet( x,y, RadToDef(rot), speed );
}



/************************************************************************************************/
//	�g�U�V���b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				speed		�e��
//				way			�g�U��
//				ret_rev		�␳�p�x
/************************************************************************************************/
void	ShotWideSet( double speed, long way, double rot_rev )
{
	long	i;
	double	rot,x,y,way_rot,i_rot,rad;

	//	�p�x
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot += DefToRad( rot_rev+11 );

	// �e����
	if( way & 1 == 1 )
	{
		// ��v�`�x
		x = 0 + cos( rot );
		y = 0 - sin( rot );
		ShotSet( x,y, RadToDef(rot), speed );

		i_rot = 0;
	}
	else
	{
		// �����v�`�x
		i_rot = DefToRad( (360 / way) / 2 );
	}

	rad = DefToRad( (360 / way) );
	for( i=0; i<way/2; i++ )
	{
		way_rot = rad * (i+1);

		x = 0 + cos( rot - way_rot + i_rot );
		y = 0 - sin( rot - way_rot + i_rot );
		ShotSet( x,y, RadToDef(rot - way_rot + i_rot), speed );

		x = 0 + cos( rot + way_rot - i_rot );
		y = 0 - sin( rot + way_rot - i_rot );
		ShotSet( x,y, RadToDef(rot + way_rot - i_rot), speed );
	}
}



/************************************************************************************************/
//	�ݒu���̃V���b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				speed	�e��
/************************************************************************************************/
void	ShotCreateSet( double speed )
{
	double	rot;

	//	�p�x
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );

	ShotSet( 0,0, RadToDef(rot), speed );
}



/************************************************************************************************/
//	�X�v���C���V���b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				shot		�����̔ԍ�
//				speed		�e��
//				rot_add		�p�x���Z�l
//				rev			�␳
/************************************************************************************************/
void	ShotSplineSet( double speed, double rot_add, double rev )
{
	double	rot;

	//	�p�x
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot += DefToRad( rev );
	ShotSet( 0,0, RadToDef(rot), speed );
}



/************************************************************************************************/
//	�z�[�~���O�V���b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				shpot	�����̔ԍ�
//				speed	�e��
//				rev		�␳�p�x
/************************************************************************************************/
void	ShotHomingSet( double speed, double rev )
{
	double	rot;

	//	�p�x
	rot = RotGet( 0,0, POL[SHOT_MAX].px,SHOT[SHOT_MAX].py );
	rot += DefToRad( rev );

	ShotSet( 0,0, RadToDef(rot), speed );
}



/************************************************************************************************/
//	�V���b�g���[�v�i�ݒu���j
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/************************************************************************************************/
void	ShotLoopSetUp( long no )
{
/*
	short	unit_no;

	if( SHOT[ no ].disp_ct > 60 )
	{
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 7, SHOT_ATR_SHOT_SPECIAL1, 32, 0, 1-unit_no, PAT_GAME_SHOT );
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 7, SHOT_ATR_SHOT_SPECIAL1, 32, 16, 1-unit_no, PAT_GAME_SHOT );
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 6, SHOT_ATR_SHOT_SPECIAL1, 32, 8, 1-unit_no, PAT_GAME_SHOT );
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 6, SHOT_ATR_SHOT_SPECIAL1, 32, 24, 1-unit_no, PAT_GAME_SHOT );
		SHOTInit( no );
	}
*/
}


/************************************************************************************************/
//	�V���b�g���[�v�i�z�[�~���O�j
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/************************************************************************************************/
void	ShotHomingLoop( long no )
{
	double	rot;

	//	�V���b�g�ƑΏۂ̊p�x���擾
	rot = RotGet( SHOT[no].px,SHOT[no].py, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot = RadToDef( rot );
	if( rot >= 360 )	rot -= 360;
	else if( rot <= 0 )	rot += 360;

	//	������
	if( SHOT[ no ].disp_ct >= 10 )
	{
		rot -= SHOT[ no ].rot_d;

		if( (rot>0 && rot<=180)|| (rot<0 && rot<=-180) )
			SHOT[ no ].rot_d += 4;
		else if( (rot>0 && rot>180) || (rot<0 && rot>-180) )
			SHOT[ no ].rot_d -= 4;

		if( SHOT[ no ].rot_d >= 360)	SHOT[ no ].rot_d -= 360;
		else if( SHOT[ no ].rot_d <= 0)	SHOT[ no ].rot_d += 360;

//		SHOT[ no ].rot = SHOT[ no ].rot_d + 90;
		SHOT[ no ].rot_r = DefToRad( SHOT[ no ].rot_d );

		SHOT[ no ].disp_ct = 0;

		//	���X�ɉ���
		SHOT[ no ].speed += 0.1;
	}

	//	90�t���[����z�[�~���O���Ȃ��Ȃ�
	if( ( SHOT[ no ].ct >= 90 )
	|| ( DistanceGet( SHOT[no].px,SHOT[no].py, POL[SHOT_MAX].px,POL[SHOT_MAX].py ) <= 32 ) )
	{
		SHOT[ no ].atr = SHOT_ATR_NORMAL;

		SHOT[ no ].ct = 0;
	}
	else
		SHOT[ no ].ct ++;
}



/************************************************************************************************/
//	�V���b�g���[�v�i�X�v���C���j
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/************************************************************************************************/
void	ShotLoopSpline( long no )
{
}



/************************************************************************************************/
//	�V���b�g�̌���
//		�߂�l�F
//				�P�F����A�O�F�Ȃ�
//		�����F
//				�A�g���r���[�g
/************************************************************************************************/
short	ShotSearch( u_long atr )
{
	long	i;
	short	ret = 0;

	for( i=0; i<SHOT_MAX; i++ )
	{
		if( ( SHOT[ i ].atr & atr ) == atr )
			ret = ON;
	}

	return( ret );
}



/************************************************************************************************/
//	�V���b�g�󂫔ԍ��擾
/************************************************************************************************/
long	ShotGetNo( long no )
{
	long	i;
	for( i=no; i<SHOT_MAX; i++ )
	{
		if( !SHOT[i].use )
			return( i );
	}

	return( NOT );
}



/************************************************************************************************/
//	���蔻��
/************************************************************************************************/
long	HitCheck( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 )
{
	if( ( x2 > x3 ) && ( x1 < x4 )
	&& ( y2 > y3 ) && ( y1 < y4 ) )
	{
		return( ON );
	}

	return( OFF );
}



/************************************************************************************************/
//	�ǔ���
/************************************************************************************************/
void	WallHitCheck( long no )
{
	//	���[����
	if( SHOT[no].px < 0 )
	{
		SHOT[no].px = 0;
		SHOT[no].mx *= -1;
	}

	//	�E�[����
	if( SHOT[no].px+SHOT[no].w >= 640 )
	{
		SHOT[no].px = 640 - SHOT[no].w;
		SHOT[no].mx *= -1;
	}

	//	��[����
	if( SHOT[no].py < 0 )
	{
		SHOT[no].py = 0;
		SHOT[no].my *= -1;
	}

	//	���[����
	if( SHOT[no].py+SHOT[no].h >= 480 )
	{
		SHOT[no].py = 480 - SHOT[no].h;
		SHOT[no].my *= -1;
	}
}
