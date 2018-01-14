/******************************************************************************/
//	�L�����N�^�[����
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibANm.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Obj.h"
#include	"./CharControl.h"
#include	"./Shot.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�L�����N�^�[�p�^�[��
UNIT_TBL	UNIT[2];

UNIT_TBL	UNIT_INIT[4] =
{
	//	obj, �T�C�Y,      ���S���W, HIT, �o�Y��, �ڐG, �X�s�[�h, �V���b�g�ԍ�
	{	NOT, 64,0,48,40,  24,17,    2,   30,     64,   4,        0,0,0,0,0 },	//	���j�b�g0
	{	NOT, 56,48,40,24, 20,11,    2,   30,     64,   5,        1,1,1,1,1 },	//	���j�b�g1
	{	NOT, 0,0,64,48,   32,29,    2,   30,     64,   4,        2,2,2,2,2 },	//	���j�b�g2
	{	NOT, 0,48,56,32,  28,15,    2,   30,     64,   3,        3,3,3,3,3 },	//	���j�b�g3

};

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
//	�L�����N�^�[������
void	CharInit( short char_no, short unit_no );
//	�L�����N�^�[���C�����[�v
void	CharMainLoop( void );
//	�L�����N�^�[�ړ�����
void	CharMove( void );
//	�L�����N�^�[���C�t�`�F�b�N
void	CharLifeChk( void );
//	�L�����N�^�[���m�̓����蔻��
void	CharHitChk( void );
//	��������
void	CharTurn( OBJ_TBL *obj1, OBJ_TBL *obj2 );
//	�V���b�g
void	ChrShotLoop( void );
void	ChrShotMain( short no );



/******************************************************************************/
//	�L�����N�^�[������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				char_no		�L�����N�^�[�ԍ�
//				unit_no		���j�b�g�ԍ�
/******************************************************************************/
void	CharInit( short char_no, short unit_no )
{
	//	�ԍ��i�I�u�W�F�N�g�A�p�^�[���j
	long obj = UNIT[ char_no ].obj_no = OBJ_GAME_UNIT + char_no;
	UNIT[ char_no ].pat = UNIT_INIT[ unit_no ].pat;

	//	���S���W
	UNIT[ char_no ].cx = UNIT_INIT[ unit_no ].cx;
	UNIT[ char_no ].cy = UNIT_INIT[ unit_no ].cy;

	//	���̑�
	UNIT[ char_no ].hit_rect = UNIT_INIT[ unit_no ].hit_rect;
	UNIT[ char_no ].buzz_rect = UNIT_INIT[ unit_no ].buzz_rect;
	UNIT[ char_no ].touch = UNIT_INIT[ unit_no ].touch;
	UNIT[ char_no ].speed = UNIT_INIT[ unit_no ].speed;

	UNIT[ char_no ].life = SYS.life[ char_no ];

	UNIT[ char_no ].gage = 100;

	for( long i=0; i<4; i++ )
	{
		UNIT[ char_no ].shot[ i ] = UNIT_INIT[ unit_no ].shot[ i ];
	}

	OBJ[ obj ].type = OBJ_TYPE_UNIT;

	if( char_no == 0 )
	{
		OBJ[ obj ].atr |= OBJ_ATR_UNIT1;
	}
	else
	{
		OBJ[ obj ].atr |= OBJ_ATR_UNIT2;
	}

	OBJ[ obj ].hit_rect = UNIT_INIT[ unit_no ].hit_rect;
	OBJ[ obj ].buzz_rect = UNIT_INIT[ unit_no ].buzz_rect;
}

/******************************************************************************/
//	�L�����N�^�[���C�����[�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CharMainLoop( void )
{
	//	�����`�F�b�N
	CharTurn( &OBJ[OBJ_GAME_UNIT+0], &OBJ[OBJ_GAME_UNIT+1] );

	//	�L�����ړ�
	CharMove();

	//	���C�t�`�F�b�N
	CharLifeChk();

	//	�L�������m�̂����蔻��
	CharHitChk();

	//	�V���b�g
	ChrShotLoop();
}

/******************************************************************************/
//	�L�����N�^�[�ړ�����
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CharMove( void )
{
	if( SYS.pause_disp )
	{
		return;
	}

	for( long i=0; i<2; i++ )
	{
		if( PAD[ i ].Use != ON )
		{
			continue;
		}

		/////////////////////////////////////////////////
		//	��
		if( PAD[ i ].Push[ PAD_UP ] )
		{
			/////////////////////////////////////////////////
			//	����
			if( PAD[ i ].Push[ PAD_LEFT ] )
			{
				OBJ[ OBJ_GAME_UNIT + i ].mx = -UNIT[ i ].speed * 0.701;
				OBJ[ OBJ_GAME_UNIT + i ].my = -UNIT[ i ].speed * 0.701;
			}
			else
			{
				if( PAD[ i ].Push[ PAD_RIGHT ] )
				{
					/////////////////////////////////////////////////
					//	�E��
					OBJ[ OBJ_GAME_UNIT + i ].mx = UNIT[ i ].speed * 0.701;
					OBJ[ OBJ_GAME_UNIT + i ].my = -UNIT[ i ].speed * 0.701;
				}
				else
				{
					OBJ[ OBJ_GAME_UNIT + i ].mx = 0;
					OBJ[ OBJ_GAME_UNIT + i ].my = -UNIT[ i ].speed;
				}
			}
		}
		else
		{
			/////////////////////////////////////////////////
			//	��
			if( PAD[ i ].Push[ PAD_DOWN ] )
			{
				/////////////////////////////////////////////////
				//	����
				if( PAD[ i ].Push[ PAD_LEFT ] )
				{
					OBJ[ OBJ_GAME_UNIT + i ].mx = -UNIT[ i ].speed * 0.701;
					OBJ[ OBJ_GAME_UNIT + i ].my = UNIT[ i ].speed * 0.701;
				}
				else
				{
					if( PAD[ i ].Push[ PAD_RIGHT ] )
					{
						/////////////////////////////////////////////////
						//	�E��
						OBJ[ OBJ_GAME_UNIT + i ].mx = UNIT[ i ].speed * 0.701;
						OBJ[ OBJ_GAME_UNIT + i ].my = UNIT[ i ].speed * 0.701;
					}
					else
					{
						OBJ[ OBJ_GAME_UNIT + i ].mx = 0;
						OBJ[ OBJ_GAME_UNIT + i ].my = UNIT[ i ].speed;
					}
				}
			}
			else
			{
				/////////////////////////////////////////////////
				//	��
				if( PAD[ i ].Push[ PAD_LEFT ] )
				{
					OBJ[ OBJ_GAME_UNIT + i ].mx = -UNIT[ i ].speed;
					OBJ[ OBJ_GAME_UNIT + i ].my = 0;
				}
				else
				{
					/////////////////////////////////////////////////
					//	�E
					if( PAD[ i ].Push[ PAD_RIGHT ] )
					{
						OBJ[ OBJ_GAME_UNIT + i ].mx = UNIT[ i ].speed;
						OBJ[ OBJ_GAME_UNIT + i ].my = 0;
					}
					else
					{
						OBJ[ OBJ_GAME_UNIT + i ].mx = 0;
						OBJ[ OBJ_GAME_UNIT + i ].my = 0;
					}
				}
			}
		}
	}
}

/******************************************************************************/
//	�L�����N�^�[���C�t�`�F�b�N
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CharLifeChk( void )
{
	short	i;
	long	obj,ct;

	for( i=0; i<2; i++ )
	{
		if( UNIT[i].life <= 0 )
		{
//	�f�o�b�O
#ifdef	__DEBUG__
			UNIT[i].life = 1000;
			break;
#endif
			obj = UNIT[i].obj_no;
			OBJ[obj].dead = ON;
			SYS.win = 1 - i;
		}
		else if( UNIT[i].life >= 1000 )
		{
			UNIT[i].life = 1000;
		}
	}

	ct = 0;
	for( i=0; i<2; i++ )
	{
		obj = UNIT[i].obj_no;
		if( OBJ[ obj ].dead == ON )
		{
			ct ++;
		}
	}

	if( ct == 2 )
	{
		SYS.win = 2;
	}
}


/******************************************************************************/
//	�L�����N�^�[���m�̓����蔻��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CharHitChk( void )
{
	short	i;
	double	x1,y1,x2,y2,w,h,len;

	x1 = OBJ[ OBJ_GAME_UNIT+0 ].px;
	y1 = OBJ[ OBJ_GAME_UNIT+0 ].py;
	x2 = OBJ[ OBJ_GAME_UNIT+1 ].px;
	y2 = OBJ[ OBJ_GAME_UNIT+1 ].py;

	//	��
	if( x1 > x2 )
	{
		w = x1 - x2;
	}
	else
	{
		w = x2 - x1;
	}

	//	�c
	if( y1 > y2 )
	{
		h = y1 - y2;
	}
	else
	{
		h = y2 - y1;
	}

	//	����
	len = ( w * w ) + ( h * h );

	if( len < (64*64) )
	{
		for( i=0; i<2; i++ )
		{
			OBJ[ OBJ_GAME_TOUCH + i ].px = OBJ[ OBJ_GAME_UNIT + i ].px;
			OBJ[ OBJ_GAME_TOUCH + i ].py = OBJ[ OBJ_GAME_UNIT + i ].py;
			OBJ[ OBJ_GAME_TOUCH + i ].disp = ON;
		}

		if( OBJ[ OBJ_GAME_UNIT ].px <= OBJ[ OBJ_GAME_UNIT+1 ].px )
		{
			OBJ[ OBJ_GAME_UNIT ].mx -= UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].mx += UNIT[ 1 ].speed;
		}
		else
		{
			OBJ[ OBJ_GAME_UNIT ].mx += UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].mx -= UNIT[ 1 ].speed;
		}

		if( OBJ[ OBJ_GAME_UNIT ].py <= OBJ[ OBJ_GAME_UNIT+1 ].py )
		{
			OBJ[ OBJ_GAME_UNIT ].my -= UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].my += UNIT[ 1 ].speed;
		}
		else
		{
			OBJ[ OBJ_GAME_UNIT ].my += UNIT[ 0 ].speed;
			OBJ[ OBJ_GAME_UNIT+1 ].my -= UNIT[ 1 ].speed;
		}
	}
	else
	{
		OBJ[ OBJ_GAME_TOUCH+0 ].disp = OFF;
		OBJ[ OBJ_GAME_TOUCH+1 ].disp = OFF;
	}
}

/******************************************************************************/
//	��������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj1	�I�u�W�F�N�g�̃A�h���X�i�����j
//				*obj2	�I�u�W�F�N�g�̃A�h���X�i����j
/******************************************************************************/
void	CharTurn( OBJ_TBL *obj1, OBJ_TBL *obj2 )
{
	//	�ꎞ�i�[�p
	double	x1,y1,x2,y2;

	x1 = obj1->px;
	y1 = obj1->py;
	x2 = obj2->px;
	y2 = obj2->py;

	obj1->rot_r = GetRot( x1,y1, x2,y2 );
	obj2->rot_r = GetRot( x2,y2, x1,y1 );
	obj1->rot_d = RadToDeg( obj1->rot_r ) + 90;
	obj2->rot_d = RadToDeg( obj2->rot_r ) + 90;

	obj1->rot = obj1->rot_d;
	obj2->rot = obj2->rot_d;
}

/******************************************************************************/
//	�V���b�g
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	ChrShotLoop( void )
{
	for( short i=0; i<2; i++ )
	{
		ChrShotMain( i );
	}
}

/******************************************************************************/
//	�V���b�g�Ǘ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/******************************************************************************/
void	ChrShotMain( short no )
{
	short	ret;
	void(*func)(void);

	ret = NOT;

	//	�m�[�}���V���b�g
	if( PAD[ no ].Trg[ PAD_BUT1 ] )
	{
		ret = 0;
	}
	else if( PAD[ no ].Rpt[ PAD_BUT1 ] )
	{
		ret = 1;
	}

	//	�X�y�V�����V���b�g�P
	if( PAD[ no ].Trg[ PAD_BUT2 ] )
	{
		if( UNIT[ no ].gage >= 20 )
		{
			ret = 2;
			UNIT[no].gage -= 40;
			g_LibText.SetColor( 5+no, 255,0,255,255 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-20" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 30 );
		}
		else
		{
			g_LibText.SetColor( 5+no, 255,255,0,0 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 6 );
			g_LibMusic.Play( SE_RELOAD, ONE );
		}
	}
	else if( PAD[ no ].Trg[ PAD_BUT3 ] )
	{
		//	�X�y�V�����V���b�g�Q
		if( UNIT[ no ].gage >= 60 )
		{
			ret = 3;
			UNIT[no].gage -= 60;
			g_LibText.SetColor( 5+no, 255,0,255,255 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-60" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 30 );
		}
		else
		{
			g_LibText.SetColor( 5+no, 255,255,0,0 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 6 );
			g_LibMusic.Play( SE_RELOAD, ONE );
		}
	}
	else if( PAD[ no ].Trg[ PAD_BUT4 ] )
	{
		//	EX�V���b�g�P
		if( UNIT[ no ].gage >= 80 )
		{
			ret = 4;
			UNIT[no].gage -= 80;
			g_LibText.SetColor( 5+no, 255,0,255,255 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-80" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 30 );
		}
		else
		{
			g_LibText.SetColor( 5+no, 255,255,0,0 );
			sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
			TXT[ 5+no ].Disp = ON;
			g_LibText.SetTimer( 5+no, 6 );
			g_LibMusic.Play( SE_RELOAD, ONE );
		}
	}
	else if( PAD[ no ].Trg[ PAD_BUT5 ] )
	{
		////	EX�V���b�g�Q
		//if( UNIT[ no ].gage >= 160 )
		//{
		//	ret = 5;
		//	UNIT[no].gage -= 160;
		//	g_LibText.SetColor( 5+no, 255,0,255,255 );
		//	sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "-160" );
		//	TXT[ 5+no ].Disp = ON;
		//	g_LibText.SetTimer( 5+no, 30 );
		//}
		//else
		//{
		//	g_LibText.SetColor( 5+no, 255,255,0,0 );
		//	sprintf_s( TXT[ 5+no ].Str, MAX_PATH, "NO GAGE" );
		//	TXT[ 5+no ].Disp = ON;
		//	g_LibText.SetTimer( 5+no, 6 );
		//	g_LibMusic.Play( SE_RELOAD, ONE );
		//}
	}

	if( ret < 0 )
	{
		return;
	}

	UNIT[ no ].but_no = ON;
	func = ( void(*)(void) )ShotTbl[ ret ];
	(*func)();
}
