/******************************************************************************/
//	�G���h���C��
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Obj.h"
#include	"./Shot.h"
#include	"./End.h"

//----------------------------------------------------------------------------//
//	�O���[�o��
//----------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	�G���h���[�h�t���O
long	EndFlag;
long	EndIconPos;

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
void	EndInit( void );	//	�G���h������
void	EndMain( void );	//	�G���h���C��



/******************************************************************************/
//	�G���h������
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	EndInit( void )
{
	ShotInitAll();

	EndFlag = 0;
	EndIconPos = 0;

	/////////////////////////////////////////////////
	//	�摜�ǂݍ���
	LoadTexture( TEX_GAME_END_TEXT, "img\\sys\\win.img" );
	LoadTexture( TEX_GAME_END_ICON, "img\\sys\\system.img" );

	/////////////////////////////////////////////////
	//	�p�^�[���J�b�g
	SetPattern( PAT_GAME_END_TEXT+0, TEX_GAME_END_TEXT, 0,0,256,32 );	//	�uwinner�v
	SetPattern( PAT_GAME_END_TEXT+1, TEX_GAME_END_TEXT, 0,32,256,48 );	//	�uplayer�v
	SetPattern( PAT_GAME_END_TEXT+2, TEX_GAME_END_TEXT, 0,80,192,32 );	//	�udrow�v
	SetPattern( PAT_GAME_END_TEXT+3, TEX_GAME_END_TEXT, 0,112,56,40 );	//	�u�P�v
	SetPattern( PAT_GAME_END_TEXT+4, TEX_GAME_END_TEXT, 56,112,56,40 );	//	�u�Q�v
	SetPattern( PAT_GAME_END_TEXT+5, TEX_GAME_END_TEXT, 0,152,144,24 );	//	�u�^�C�g���ɖ߂�v
	SetPattern( PAT_GAME_END_TEXT+6, TEX_GAME_END_TEXT, 0,176,120,24 );	//	�u�Q�[���ĊJ�v
	SetPattern( PAT_GAME_END_ICON,   TEX_GAME_END_ICON, 136,56,8,8 );	//	�A�C�R��

	/////////////////////////////////////////////////
	//	�I�u�W�F�N�g
	ObjSet( OBJ_GAME_END_TEXT+0, POL_GAME_END_TEXT+0, PAT_GAME_END_TEXT, 56,88,1.0, OBJ_TYPE_BG, ATR_NONE );	//	winner
	ObjSet( OBJ_GAME_END_TEXT+5, POL_GAME_END_TEXT+5, PAT_GAME_END_TEXT+5, 248,344,1.0, OBJ_TYPE_BG, ATR_NONE );	//	�u�^�C�g���ɖ߂�v
	ObjSet( OBJ_GAME_END_TEXT+6, POL_GAME_END_TEXT+6, PAT_GAME_END_TEXT+6, 264,392,1.0, OBJ_TYPE_BG, ATR_NONE );	//	�u�Q�[���ĊJ�v
	ObjSet( OBJ_GAME_END_ICON,	 POL_GAME_END_ICON,	  PAT_GAME_END_ICON, 232,352,1.0, OBJ_TYPE_BG, ATR_NONE );		//	�A�C�R��

	if( SYS.win == 0 )
	{
		//	�P�o��
		ObjSet( OBJ_GAME_END_TEXT+1, POL_GAME_END_TEXT+1, PAT_GAME_END_TEXT+3, 240,184,1.0, OBJ_TYPE_BG, ATR_NONE );	//	1
		ObjSet( OBJ_GAME_END_TEXT+2, POL_GAME_END_TEXT+2, PAT_GAME_END_TEXT+1, 320,176,1.0, OBJ_TYPE_BG, ATR_NONE );	//	player
	}
	else if( SYS.win == 1 )
	{
		//	�P�o����
		ObjSet( OBJ_GAME_END_TEXT+1, POL_GAME_END_TEXT+1, PAT_GAME_END_TEXT+4, 240,184,1.0, OBJ_TYPE_BG, ATR_NONE );	//	2
		ObjSet( OBJ_GAME_END_TEXT+2, POL_GAME_END_TEXT+2, PAT_GAME_END_TEXT+1, 320,176,1.0, OBJ_TYPE_BG, ATR_NONE );	//	player
	}
	else if( SYS.win == 2 )
	{
		//	drow
		ObjSet( OBJ_GAME_END_TEXT+1, POL_GAME_END_TEXT+1, PAT_GAME_END_TEXT+2, 320-96,176,1.0, OBJ_TYPE_BG, ATR_NONE );	//	drow
		OBJ[ OBJ_GAME_END_TEXT ].disp = OFF;
	}

	/////////////////////////////////////////////////
	//	SE�̓ǂݍ���
	g_LibMusic.Load( SE_CURSOR, "se\\se_cursor.wav" );
	g_LibMusic.Load( SE_BUT, "se\\se_button00.wav" );
	g_LibMusic.Load( SE_START, "se\\se_button01.wav" );
	g_LibMusic.Stop( BGM_BATTLE );
}

/******************************************************************************/
//	�^�C�g�����C��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	EndMain( void )
{
	double	pos[2][2] = 
	{
		232,352,
		232,400,
	};

	switch( EndFlag )
	{
		/////////////////////////////////////////////////
		//	�L�[���͑҂�
		case	0:
			if( PAD[0].Trg[ PAD_UP ] || PAD[0].Trg[ PAD_DOWN ] 
			|| PAD[1].Trg[ PAD_UP ] || PAD[1].Trg[ PAD_DOWN ] )
			{
				g_LibMusic.Play( SE_CURSOR, ONE );
				EndIconPos ^= 1;
			}

			if( PAD[0].Trg[ PAD_START ] || PAD[0].Trg[ PAD_BUT1 ]
			|| PAD[0].Trg[ PAD_START ] || PAD[0].Trg[ PAD_BUT1 ] )
			{
				g_LibMusic.Stop( SE_BUT );
				g_LibMusic.Play( SE_START, ONE );

				if( EndIconPos == 0 )
				{
					SYS.main_mode = MAIN_MODE_INIT;
				}
				else if( EndIconPos == 1 )
				{
					SYS.main_mode = MAIN_MODE_GAME_INIT;
				}
			}

			OBJ[ OBJ_GAME_END_ICON ].px = pos[ EndIconPos ][0];
			OBJ[ OBJ_GAME_END_ICON ].py = pos[ EndIconPos ][1];

			break;
	}
}
