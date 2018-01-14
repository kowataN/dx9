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

#include	"./Main.h"
#include	"./ObjectMng.h"
#include	"./CharControl.h"
#include	"./ShotMng.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�L�����N�^�[�p�^�[��
UNIT_TBL	UNIT;

UNIT_TBL	UNIT_INIT =
{
//	obj, �T�C�Y,      ���S���W, HIT, �X�s�[�h, �V���b�g�ԍ�
	NOT, 64,0,48,48,  24,24,    2,   4,        0,0,0,0,0,   1,1, 3
};

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
void	CharInit( void );
void	CharMainLoop( void );
void	CharMove( void );
void	CharLifeChk( void );
void	CharHitChk( void );
void	CharTurn( long ObjNo1, long ObjNo2 );
void	ChrShotMain( void );



/******************************************************************************/
//	�L�����N�^�[������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				char_no		�L�����N�^�[�ԍ�
//				unit_no		���j�b�g�ԍ�
/******************************************************************************/
void	CharInit( void )
{
	CObjectMng &Obj = CObjectMng::GetInstance();

	//	�ԍ��i�I�u�W�F�N�g�A�p�^�[���j
	long obj = UNIT.ObjectNo = OBJ_GAME_UNIT;
	UNIT.PatSize = UNIT_INIT.PatSize;

	//	���S���W
	UNIT.CenterX = UNIT_INIT.CenterX;
	UNIT.CenterY = UNIT_INIT.CenterY;

	//	���̑�
	UNIT.HitRect = UNIT_INIT.HitRect;
	UNIT.Speed = UNIT_INIT.Speed;
	UNIT.ShotLv = UNIT_INIT.ShotLv;
	UNIT.Life = 1000;

	for( long i=0; i<4; i++ )
	{
		UNIT.Shot[ i ] = UNIT_INIT.Shot[ i ];
	}

	ST_ObjectInfo Info = Obj.GetObjectInfo( obj );
	Info.HitRect = UNIT_INIT.HitRect;
	Obj.SetObjectInfo( obj, Info );
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
	CObjectMng	&Obj = CObjectMng::GetInstance();

	//	�L�����ړ�
	CharMove();

	//	��������
	CharTurn( OBJ_GAME_UNIT, OBJ_GAME_ENEMY + UNIT.LockEnemy );

	//	���C�t�`�F�b�N
	CharLifeChk();

	//	�L�������m�̂����蔻��
	CharHitChk();

	//	�V���b�g
	ChrShotMain();
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
	if( SYS.PauseDisp )
	{
		return;
	}

	if( PAD[0].Use != ON )
	{
		return;
	}

	CObjectMng	&Obj = CObjectMng::GetInstance();
	double MoveX = 0;
	double MoveY = 0;

	/////////////////////////////////////////////////
	//	��
	if( PAD[0].Push[ PAD_UP ] )
	{
		/////////////////////////////////////////////////
		//	����
		if( PAD[0].Push[ PAD_LEFT ] )
		{
			MoveX = -UNIT.Speed * 0.701;
			MoveY = -UNIT.Speed * 0.701;
		}
		else
		{
			if( PAD[ 0 ].Push[ PAD_RIGHT ] )
			{
				/////////////////////////////////////////////////
				//	�E��
				MoveX = UNIT.Speed * 0.701;
				MoveY = -UNIT.Speed * 0.701;
			}
			else
			{
				MoveX = 0;
				MoveY = -UNIT.Speed;
			}
		}
	}
	else
	{
		/////////////////////////////////////////////////
		//	��
		if( PAD[ 0 ].Push[ PAD_DOWN ] )
		{
			/////////////////////////////////////////////////
			//	����
			if( PAD[ 0 ].Push[ PAD_LEFT ] )
			{
				MoveX = -UNIT.Speed * 0.701;
				MoveY = UNIT.Speed * 0.701;
			}
			else
			{
				if( PAD[ 0 ].Push[ PAD_RIGHT ] )
				{
					/////////////////////////////////////////////////
					//	�E��
					MoveX = UNIT.Speed * 0.701;
					MoveY = UNIT.Speed * 0.701;
				}
				else
				{
					MoveX = 0;
					MoveY = UNIT.Speed;
				}
			}
		}
		else
		{
			/////////////////////////////////////////////////
			//	��
			if( PAD[ 0 ].Push[ PAD_LEFT ] )
			{
				MoveX = -UNIT.Speed;
				MoveY = 0;
			}
			else
			{
				/////////////////////////////////////////////////
				//	�E
				if( PAD[ 0 ].Push[ PAD_RIGHT ] )
				{
					MoveX = UNIT.Speed;
					MoveY = 0;
				}
				else
				{
					MoveX = 0;
					MoveY = 0;
				}
			}
		}
	}

	Obj.SetTransfer( OBJ_GAME_UNIT, MoveX, MoveY );
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
	if( UNIT.Life <= 300 )
	{

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
	//CObjectMng	&Obj = CObjectMng::GetInstance();
	//short	i;
	//double	x1,y1,x2,y2,w,h,len;

	//long PolNo = Obj.GetObjectInfo( OBJ_GAME_UNIT ).PolNo;
	//POL_TBL Info1 = g_LibPol.GetInfo( PolNo );
	//x1 = Info1.PosX;
	//y1 = Info1.PosY;
	//POL_TBL Info2 = g_LibPol.GetInfo( PolNo );
	//x2 = Info2.PosX;
	//y2 = Info2.PosY;

	////	��
	//if( x1 > x2 )
	//{
	//	w = x1 - x2;
	//}
	//else
	//{
	//	w = x2 - x1;
	//}

	////	�c
	//if( y1 > y2 )
	//{
	//	h = y1 - y2;
	//}
	//else
	//{
	//	h = y2 - y1;
	//}

	////	����
	//len = ( w * w ) + ( h * h );

	//if( len < (64*64) )
	//{
	//	for( i=0; i<2; i++ )
	//	{
	//		Obj.SetPos( OBJ_GAME_TOUCH + i,
	//			Obj.GetObjectInfo( OBJ_GAME_UNIT + i ).PosX,
	//			Obj.GetObjectInfo( OBJ_GAME_UNIT + i ).PosY,
	//			Obj.GetObjectInfo( OBJ_GAME_UNIT + i ).PosZ
	//			);
	//		Obj.DispChange( OBJ_GAME_TOUCH + i, ON );
	//	}

	//	if( Obj.GetObjectInfo( OBJ_GAME_UNIT ).PosX <= Obj.GetObjectInfo( OBJ_GAME_ENEMY ).PosX )
	//	{
	//		Obj.AddMovePos( OBJ_GAME_UNIT, -UNIT[ 0 ].Speed,0 );
	//		Obj.AddMovePos( OBJ_GAME_ENEMY, UNIT[ 1 ].Speed,0 );
	//	}
	//	else
	//	{
	//		Obj.AddMovePos( OBJ_GAME_UNIT, UNIT[ 0 ].Speed,0 );
	//		Obj.AddMovePos( OBJ_GAME_ENEMY, -UNIT[ 1 ].Speed,0 );
	//	}

	//	if( OBJ[ OBJ_GAME_UNIT ].PosY <= OBJ[ OBJ_GAME_ENEMY ].PosY )
	//	{
	//		Obj.AddMovePos( OBJ_GAME_UNIT, 0,-UNIT[ 0 ].Speed );
	//		Obj.AddMovePos( OBJ_GAME_ENEMY, 0,UNIT[ 1 ].Speed );
	//	}
	//	else
	//	{
	//		Obj.AddMovePos( OBJ_GAME_UNIT, 0,UNIT[ 0 ].Speed );
	//		Obj.AddMovePos( OBJ_GAME_ENEMY, 0,-UNIT[ 1 ].Speed );
	//	}
	//}
	//else
	//{
	//	Obj.DispChange( OBJ_GAME_TOUCH+0, OFF );
	//	Obj.DispChange( OBJ_GAME_TOUCH+1, OFF );
	//}
}

/******************************************************************************/
//	��������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj1	�I�u�W�F�N�g�̃A�h���X�i�����j
//				*obj2	�I�u�W�F�N�g�̃A�h���X�i����j
/******************************************************************************/
void	CharTurn( long ObjNo1, long ObjNo2 )
{
	//	�ꎞ�i�[�p
	CObjectMng &Obj = CObjectMng::GetInstance();
	double	x1,y1,x2,y2;
	ST_ObjectInfo obj1,obj2;

	obj1 = Obj.GetObjectInfo( ObjNo1 );
	obj2 = Obj.GetObjectInfo( ObjNo2 );

	x1 = obj1.PosX;
	y1 = obj1.PosY;
	x2 = obj2.PosX;
	y2 = obj2.PosY;

	obj1.RotRad = GetRot( x1,y1, x2,y2 );
	obj1.RotDeg = RadToDeg( obj1.RotRad ) + 90;

	Obj.SetObjectInfo( ObjNo1, obj1 );
}

/******************************************************************************/
//	�V���b�g�Ǘ�
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�I�u�W�F�N�g�ԍ�
/******************************************************************************/
void	ChrShotMain( void )
{
	////	�m�[�}���V���b�g
	if( PAD[ 0 ].Trg[ PAD_BUT1 ] )
	{
		UNIT.ButNo = ON;
		NormarlShot();
	}

	//if( PAD[ 0 ].Trg[ PAD_BUT2 ] )
	//{
	//	//	�X�y�V�����V���b�g
	//	UNIT.ButNo = ON;
	//	SpecialShot();
	//}
	//if( PAD[0].Trg[ PAD_BUT3 ] )
	//{
	//	UNIT.LockEnemy ++;
	//	if( UNIT.LockEnemy >=4 )
	//	{
	//		UNIT.LockEnemy = 0;
	//	}
	//}

	if( KEY.Trg[ DIK_D ] )
	{
		UNIT.ShotLv = 1;
	}
	else if( KEY.Trg[ DIK_A ] )
	{
		UNIT.ShotLv ++;
	}
}
