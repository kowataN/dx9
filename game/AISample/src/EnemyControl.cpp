/******************************************************************************/
//	�G�l�~�[����
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
#include	"./EnemyControl.h"
#include	"./ShotMng.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
ST_Enemy	ENEMY_INIT =
{
//	obj,     �T�C�Y,      ���S���W, HIT, �X�s�[�h, �V���b�g�ԍ�
	NOT,NOT, 56,48,40,24, 20,12,    2,   4,        0,0,0,0,0,   1,1, 3
};

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
short	CHkPointInCircle( double PosX, double PosY, ST_ObjectInfo MyInfo );



/******************************************************************************/
//	�R���X�g���N�^
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
CEnemy::CEnemy()
{
}

/******************************************************************************/
//	�f�X�g���N�^
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
CEnemy::~CEnemy()
{
}

/******************************************************************************/
//	�G�l�~�[������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CEnemy::Init( long ObjNo, long TargetNo, double PosX, double PosY )
{
	CObjectMng &Obj = CObjectMng::GetInstance();

	//	�ԍ��i�I�u�W�F�N�g�A�p�^�[���j
	m_Info.ObjectNo = ObjNo;
	m_Info.TargetObjNo = TargetNo;
	m_Info.PatSize = ENEMY_INIT.PatSize;

	//	���S���W
	m_Info.CenterX = ENEMY_INIT.CenterX;
	m_Info.CenterY = ENEMY_INIT.CenterY;

	//	���̑�
	m_Info.HitRect = ENEMY_INIT.HitRect;
	m_Info.Speed = ENEMY_INIT.Speed;
	m_Info.ShotLv = ENEMY_INIT.ShotLv;
	m_Info.ShotTimer = 20;
	m_Info.ShotCt = 0;
	m_Info.MoveTimer = 5;
	m_Info.MoveCt = 0;
	m_Info.MoveFlag = _MoveFlagSet;

	for( long i=0; i<4; i++ )
	{
		m_Info.Shot[ i ] = ENEMY_INIT.Shot[ i ];
	}

	SetPattern( PAT_GAME_ENEMY,TEX_GAME_UNIT,
		m_Info.PatSize.left, m_Info.PatSize.top, m_Info.PatSize.right, m_Info.PatSize.bottom );
	Obj.Set( m_Info.ObjectNo, PAT_GAME_ENEMY, PosX,PosY,0.5, g_ObjType.Enemy, OBJ_ATR_UNIT );
	Obj.SetCenter( m_Info.ObjectNo, m_Info.CenterX,m_Info.CenterY );
	ST_ObjectInfo Info = Obj.GetObjectInfo( m_Info.ObjectNo );
	Info.HitRect = m_Info.HitRect;
	Obj.SetObjectInfo( m_Info.ObjectNo, Info );

	this->m_InitFlag = ON;
}

/******************************************************************************/
//	�G�l�~�[���C�����[�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CEnemy::MainLoop( void )
{
	if( m_InitFlag == OFF )
	{
		return;
	}

	//	�ʒu�擾
	this->SetPostion();
	//	�����擾
	this->SetDistance();

	//	��������
	this->Turn( m_Info.ObjectNo , m_Info.TargetObjNo );

	this->Move();
	this->Atack();

	//	�����蔻��
	this->HitChk();

#ifdef	_DEBUG
	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo MyInfo = Obj.GetObjectInfo( m_Info.ObjectNo );
	char	PosStr[_PosMax][16] =
	{
		"����","��","�E��",
		"��","�^��","�E",
		"����","��","�E��",
	};
	//g_LibText.Set( 100, 0,0, 255,255,0,0, "" );
	//sprintf_s( TXT[100].Str, "�ʒu�F%5s", PosStr[m_Info.Pos] );
	//g_LibText.Set( 101, 0,16, 255,255,0,0, "" );
	//sprintf_s( TXT[101].Str, "�����F%f", m_Info.Distance );
	//g_LibText.Set( 102, 0,32, 255,255,0,0, "" );
	//sprintf_s( TXT[102].Str, "���W�i%5.2f,%5.2f�j", MyInfo.PosX,MyInfo.PosY );
	//g_LibText.Set( 103, 0,48, 255,255,0,0, "" );
	//sprintf_s( TXT[103].Str, "�ړ��ʁi%5.2f,%5.2f�j", MyInfo.MoveX, MyInfo.MoveY );
#endif
}

/******************************************************************************/
//	��~����
/******************************************************************************/
void	CEnemy::Stop( void )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	Obj.SetTransfer( m_Info.ObjectNo, 0, 0 );
}

/******************************************************************************/
//	�ړ�����
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CEnemy::Move( void )
{
	switch( m_Info.MoveFlag )
	{
		case	_MoveFlagSet:
			this->SetMove();
			m_Info.MoveFlag = _MoveFlagLoop;

		case	_MoveFlagLoop:
			this->LoopMove();
			break;
	}
}

/******************************************************************************/
//	�U������
/******************************************************************************/
void	CEnemy::Atack( void )
{
	if( m_Info.ShotTimer <= m_Info.ShotCt )
	{
		ULong Atr = OBJ_ATR_SHOT | OBJ_ATR_SHOT_NORMAL;
		if( m_Info.ObjectNo == OBJ_GAME_UNIT )
		{
			Atr |= OBJ_ATR_UNIT;
		}
		else
		{
			Atr |= OBJ_ATR_ENEMY;
		}
		if( rand()%20 < 1 )
		{
			ShotSetWide( m_Info.ObjectNo, m_Info.TargetObjNo, 5, Atr, 16, 0, 0, PAT_GAME_SHOT+1 );
		}
		else
		{
			ShotSetNormal( m_Info.ObjectNo, m_Info.TargetObjNo, 5, Atr, 0, 0, PAT_GAME_SHOT+1 );
			//ShotSetNormal( m_Info.ObjectNo, m_Info.TargetObjNo, 5, Atr, 5, 0, PAT_GAME_SHOT+1 );
			//ShotSetNormal( m_Info.ObjectNo, m_Info.TargetObjNo, 5, Atr, -5, 0, PAT_GAME_SHOT+1 );
		}
		m_Info.ShotCt = 0;
	}
	else
	{
		m_Info.ShotCt ++;
		m_Info.State = _StateStop;
	}
}

/******************************************************************************/
//	�����蔻��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	CEnemy::HitChk( void )
{
}

/******************************************************************************/
//	��������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				*obj1	�I�u�W�F�N�g�̃A�h���X�i�����j
//				*obj2	�I�u�W�F�N�g�̃A�h���X�i����j
/******************************************************************************/
void	CEnemy::Turn( long ObjNo1, long ObjNo2 )
{
	CObjectMng &Obj = CObjectMng::GetInstance();

	//	�ꎞ�i�[�p
	double	x1,y1,x2,y2;
	ST_ObjectInfo obj1,obj2;

	obj1 = Obj.GetObjectInfo( ObjNo1 );
	obj2 = Obj.GetObjectInfo( ObjNo2 );

	x1 = obj1.PosX;
	y1 = obj1.PosY;
	x2 = obj2.PosX;
	y2 = obj2.PosY;

	obj2.RotRad = GetRot( x2,y2, x1,y1 );
	obj2.RotDeg = RadToDeg( obj2.RotRad ) + 90;

	Obj.SetObjectInfo( ObjNo2, obj2 );
}

vector<ST_ObjectInfo> CEnemy::GetShot( void )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	vector<ST_ObjectInfo> Ret;
	ST_ObjectInfo MyInfo = Obj.GetObjectInfo( m_Info.ObjectNo );

	for( int i=0; i<OBJ_MAX; i++ )
	{
		ST_ObjectInfo Info = Obj.GetObjectInfo( i );
		Obj.SetColor( i, 255,255,255,255 );
		if( Info.Use == OFF )
		{
			continue;
		}

		if( !( ( Info.Atr & OBJ_ATR_SHOT ) == OBJ_ATR_SHOT 
			&& ( Info.Atr & OBJ_ATR_UNIT ) == OBJ_ATR_UNIT ) )
		{
			//continue;
		}

		//	�����Ɍ������Ă���e�𔻒�
		double RotR = GetRot( Info.PosX,Info.PosY, MyInfo.PosX,MyInfo.PosY );
		double RotD = RadToDeg( RotR )+90;
		if( RotD-5 <= Info.RotDeg && Info.RotDeg <= RotD+5 )
		{
			Obj.SetColor( i, 255,255,0,255 );
			Ret.push_back( Info );
			continue;
		}

	}

	return Ret;
}

void	CEnemy::SetPostion( void )
{
	double ChkPos[_PosMax][4] =
	{
		0,		0,		146,	146,
		147,	0,		292,	146,
		293,	0,		440,	146,
		0,		147,	146,	292,
		147,	147,	292,	292,
		293,	147,	440,	292,
		0,		293,	146,	440,
		147,	293,	292,	440,
		293,	293,	440,	440,
	};

	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo MyInfo = Obj.GetObjectInfo( m_Info.ObjectNo );

	for( int i=0; i<_PosMax; i++ )
	{
		if( ChkPos[i][0]+100 <= MyInfo.PosX && MyInfo.PosX <= ChkPos[i][2]+100
		&& ChkPos[i][1]+20 <= MyInfo.PosY && MyInfo.PosY <= ChkPos[i][3]+20 )
		{
			m_Info.Pos = (E_Position)i;
			break;
		}
	}
}

void	CEnemy::SetDistance( void )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo MyInfo = Obj.GetObjectInfo( m_Info.ObjectNo );
	ST_ObjectInfo Info = Obj.GetObjectInfo( m_Info.TargetObjNo );
	m_Info.Distance = GetDistance( MyInfo.PosX, MyInfo.PosY, Info.PosX, Info.PosY );
}

void	CEnemy::SetMove( void )
{
	//	�ړ���
	double MoveX,MoveY;
	MoveX = MoveY = 0.0;

	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo MyInfo = Obj.GetObjectInfo( m_Info.ObjectNo );
	ST_ObjectInfo Player = Obj.GetObjectInfo( m_Info.TargetObjNo );

	//	�����Ɍ������Ă���e�擾
	vector<ST_ObjectInfo> Shot = this->GetShot();
	//if( Shot.size() == 0 )
	{
		//	�����Ɍ������Ă���e���Ȃ��̂Ń����_���ړ�
		this->SetMovePos( MyInfo, Player, MoveX, MoveY );
	}
	//else
	//{
	//	//	�����Ɍ������Ă���e������̂ŉ��
	//	this->SetEscape( MyInfo, Shot, MoveX, MoveY );
	//}

	Obj.SetTransfer( m_Info.ObjectNo, MoveX, MoveY );
	m_Info.MoveCt = 0;

	//	���@�Ƃ̋����ňړ����ԕύX
	switch( this->GetLange() )
	{
		//	�ߋ���
		case	_LngShort:
			m_Info.MoveTimer = 10;
			break;
		//	������
		case	_LngMiddle:
			m_Info.MoveTimer = 20;
			break;
		//	������
		case	_LngLong:
			m_Info.MoveTimer = 30;
			break;
	}
}

void	CEnemy::SetMovePos( ST_ObjectInfo MyInfo, ST_ObjectInfo Player, double &MoveX, double &MoveY )
{
	E_Lange Lange = this->GetLange();

	// �����Ƒ���̈ʒu�֌W�ňړ������𔻒�
	switch( m_Info.Pos )
	{
		case	_PosLeftUp:
		case	_PosCenterUp:
		case	_PosRightUp:
			if( Lange == _LngShort )
			{
				//	�߂��̂ő���Ƃ̈ʒu�֌W�ňړ���������
				if( MyInfo.PosX <= Player.PosX )
				{
					MoveX = -1;
				}
				else
				{
					MoveX = 1;
				}

				MoveY = 1;
			}
			else
			{
				//	����������̂Ŏ��R�Ɉړ�
				if( rand()%2 )
				{
					MoveX = 1;
				}
				else
				{
					MoveX = -1;
				}

				if( rand()%2 )
				{
					MoveY = 1;
				}
				else
				{
					MoveY = -1;
				}
			}
			break;

		case	_PosLeft:
		case	_PosCenter:
		case	_PosRight:
			if( Lange == _LngShort )
			{
				//	�߂��̂ő���Ƃ̈ʒu�֌W�ňړ���������
				if( MyInfo.PosX <= Player.PosX )
				{
					MoveX = -1;
				}
				else
				{
					MoveX = 1;
				}
			}
			else
			{
				//	����������̂Ŏ��R�Ɉړ�
				if( rand()%2 )
				{
					MoveX = 1;
				}
				else
				{
					MoveX = -1;
				}
			}

			if( rand()%2 )
			{
				MoveY = 1;
			}
			else
			{
				MoveY = -1;
			}
			break;

		case	_PosLeftDown:
		case	_PosCenterDown:
		case	_PosRightDown:
			if( Lange == _LngShort )
			{
				//	�߂��̂ő���Ƃ̈ʒu�֌W�ňړ���������
				if( MyInfo.PosX <= Player.PosX )
				{
					MoveX = -1;
				}
				else
				{
					MoveX = 1;
				}

				MoveY = -1;
			}
			else
			{
				//	����������̂Ŏ��R�Ɉړ�
				if( rand()%2 )
				{
					MoveX = 1;
				}
				else
				{
					MoveX = -1;
				}

				if( rand()%2 )
				{
					MoveY = 1;
				}
				else
				{
					MoveY = -1;
				}
			}
			break;
	}

	MoveX *= 2;
	MoveY *= 2;
}

void	CEnemy::SetEscape( ST_ObjectInfo MyInfo, vector<ST_ObjectInfo> Shot, double &MoveX, double &MoveY )
{
	vector<ST_ObjectInfo>::iterator Ite;

	for( Ite=Shot.begin(); Ite!=Shot.end(); Ite++ )
	{
		E_Lange Lange = this->GetLange( MyInfo.PosX, MyInfo.PosY, (*Ite).PosX, (*Ite).PosY );

		//	�ߋ����̒e���珈������
		MoveX = -GetMoveX( 2, MyInfo.RotRad );
		MoveY = GetMoveY( 2, MyInfo.RotRad );
	}

}

void	CEnemy::LoopMove( void )
{
	if( m_Info.MoveTimer <= m_Info.MoveCt )
	{
		m_Info.State = _StateStop;
		m_Info.MoveFlag = _MoveFlagSet;
	}
	else
	{
		m_Info.MoveCt ++;
	}
}

//	�v���C��[�Ƃ̋����֌W���擾
E_Lange	CEnemy::GetLange( void )
{
	E_Lange Ret = _LngMiddle;

	//	���@�Ƃ̋����ňړ����ԕύX
	if( m_Info.Distance <= 100 )
	{
		//	�ߋ���
		Ret = _LngShort;
	}
	else if( 100 < m_Info.Distance && m_Info.Distance <= 200 )
	{
		//	������
		Ret = _LngMiddle;
	}
	else
	{
		//	������
		Ret = _LngLong;
	}

	return Ret;
}

E_Lange	CEnemy::GetLange( double PosX1, double PosY1, double PosX2, double PosY2 )
{
	E_Lange Ret = _LngMiddle;
	double Dis = GetDistance( PosX1,PosY1, PosX2,PosY2 );

	//	���@�Ƃ̋����ňړ����ԕύX
	if( Dis <= 100 )
	{
		//	�ߋ���
		Ret = _LngShort;
	}
	else if( 100 < Dis && Dis <= 200 )
	{
		//	������
		Ret = _LngMiddle;
	}
	else
	{
		//	������
		Ret = _LngLong;
	}

	return Ret;
}

//	�~�̒��ɓ_�����邩����
short	CHkPointInCircle( double PosX, double PosY, ST_ObjectInfo MyInfo )
{
	double	dx = PosX - MyInfo.PosX;
	double	dy = PosY - MyInfo.PosY;
	double	r = MyInfo.HitRect;

	if( dx*dx + dy*dy < r*r )
	{
		return ON;
	}
	else
	{
		return OFF;
	}
}