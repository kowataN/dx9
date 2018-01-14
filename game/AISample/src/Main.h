/******************************************************************************/
//	�Q�[�����C��
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__Main_H__
#define	__Main_H__

#include	"./ObjectMng.h"

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//
enum
{
	/////////////////////////////////////////////////
	//	�Q�[�����[�h
	MAIN_MODE_INIT = 0,
	MAIN_MODE_TITLE_INIT = 10,
	MAIN_MODE_TITLE_MAIN,
	MAIN_MODE_OPTION_INIT = 20,
	MAIN_MODE_OPTION_MAIN,
	MAIN_MODE_CHAR_SELECT_INIT = 30,
	MAIN_MODE_CHAR_SELECT_MAIN,
	MAIN_MODE_GAME_INIT = 40,
	MAIN_MODE_GAME_MAIN,
	MAIN_MODE_END_INIT = 50,
	MAIN_MODE_END_MAIN,

	/////////////////////////////////////////////////
	//	�e�N�X�`���ԍ�
	TEX_GAME_BG	= 0,
	TEX_GAME_SYSTEM,
	TEX_GAME_END_TEXT,
	TEX_GAME_SHOT,
	TEX_GAME_UNIT,
	TEX_GAME_END_ICON,

	/////////////////////////////////////////////////
	//	�p�^�[���ԍ�
	PAT_GAME_BG = 0,
	PAT_GAME_TIME = 10,
	PAT_GAME_GAGE_FRM = 25,
	PAT_GAME_GAGE_BAR = 30,
	PAT_GAME_CONTER = 35,
	PAT_GAME_PLAYER_NO = 50,
	PAT_GAME_PAUSE = 53,
	PAT_GAME_ICON,
	PAT_GAME_UNIT = 55,
	PAT_GAME_ENEMY,
	PAT_GAME_BUZZ = 65,
	PAT_GAME_TOUCH = 70,
	PAT_GAME_EFFECT = 75,
	PAT_GAME_END_TEXT = 100,
	PAT_GAME_END_ICON = 110,
	PAT_GAME_SHOT = 120,

	/////////////////////////////////////////////////
	//	�I�u�W�F�N�g�ԍ�
	OBJ_GAME_BG = 0,
	OBJ_GAME_TIME = 10,
	OBJ_GAME_GAGE_FRM = 20,
	OBJ_GAME_GAGE_BAR = 25,
	OBJ_GAME_PLAYER_NO = 30,
	OBJ_GAME_CONTER = 35,
	OBJ_GAME_PAUSE = 38,
	OBJ_GAME_ICON,
	OBJ_GAME_UNIT = 40,
	OBJ_GAME_ENEMY,
	OBJ_GAME_BUZZ = 45,
	OBJ_GAME_TOUCH = 50,
	OBJ_GAME_EFFECT = 60,
	OBJ_GAME_END_TEXT = 80,
	OBJ_GAME_END_ICON = 90,
	OBJ_GAME_SHOT = 100,

	/////////////////////////////////////////////////
	//	�A�j���ԍ�

	/////////////////////////////////////////////////
	//	�a�f�l���r�d

	//	BGM
	BGM_BATTLE = 0,

	//	SE
	SE_CURSOR,
	SE_START,
	SE_BUT,
	SE_SHOT1,
	SE_SHOT2,
	SE_SHOT3,
	SE_BUZZ,
	SE_RELOAD,
	SE_BAKUHATU,
	SE_STOCK,
	SE_WARNING,
};

/////////////////////////////////////////////////
//	�A�g���r���[�g
#define	OBJ_ATR_NONE			(0)		//	�Ȃ��i�����l�j
#define	OBJ_ATR_UNIT			(1<<1)	//	���@
#define	OBJ_ATR_ENEMY			(1<<2)	//	�G�@
#define	OBJ_ATR_SHOT			(1<<3)	//	�e

#define	OBJ_ATR_SHOT_NORMAL		(1<<4)	//	�m�[�}���V���b�g
#define	OBJ_ATR_SHOT_SPECIAL1	(1<<5)	//	����V���b�g�P
#define	OBJ_ATR_SHOT_SPECIAL2	(1<<6)	//	����V���b�g�Q
#define	OBJ_ATR_SHOT_EXTRA1		(1<<7)	//	EX�V���b�g�P
#define	OBJ_ATR_SHOT_EXTRA2		(1<<8)	//	EX�V���b�g�Q

#define	OBJ_ATR_SHOT_HOMING1	(1<<9)	//	�z�[�~���O1
#define	OBJ_ATR_SHOT_HOMING2	(1<<10)	//	�z�[�~���O2
#define	OBJ_ATR_SHOT_HOMING3	(1<<11)	//	�z�[�~���O3
#define	OBJ_ATR_SHOT_HOMING4	(1<<12)	//	�z�[�~���O4
#define	OBJ_ATR_SHOT_HOMING5	(1<<13)	//	�z�[�~���O5
#define	OBJ_ATR_SHOT_HOMING		(1<<14)	//	�z�[�~���O
#define	OBJ_ATR_SHOT_SET_UP		(1<<15)	//	�ݒu��
#define	OBJ_ATR_SHOT_SPLINE		(1<<16)	//	�X�v���C��

#define	OBJ_ATR_ROT_NONE		(1<<17)	//	��]�Ȃ�
#define	OBJ_ATR_ROT_USE			(1<<18)	//	��]����
#define	OBJ_ATR_ROT_CHANGE		(1<<19)	//	�p�x�ύX

#define	OBJ_ATR_SHOT_RANK		(OBJ_ATR_SHOT_NORMAL|OBJ_ATR_SHOT_SPECIAL1|OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_EXTRA2)
#define	OBJ_ATR_SHOT_TYPE		(OBJ_ATR_SHOT_HOMING|OBJ_ATR_SHOT_SET_UP|OBJ_ATR_SHOT_SPLINE)
#define	OBJ_ATR_ROT_TYPE		(OBJ_ATR_ROT_NONE|OBJ_ATR_ROT_USE|OBJ_ATR_ROT_CHANGE)

/////////////////////////////////////////////////
//	�f�o�b�O�X�C�b�`
//#define		__DEBUG__

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	�V�X�e���p
typedef	struct
{
	long	MainMode;	//	���C�����[�h
	short	StageNo;	//	�X�e�[�W�ԍ�
	short	Time;		//	�퓬�̎���
	long	FrameCt;	//	�t���[���J�E���^
	UChar	PauseDisp;	//	�|�[�Y�\���t���O
	long	Life;		//	���C�t�i�������p�j
}SYSTEM_TBL;

#endif	//	__Main_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�V�X�e��
extern	SYSTEM_TBL	SYS;
extern	CObjectMng	g_Obj;


//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	long	HitCheck( double x1, double y1, double r1, double x2, double y2, double r2 );