/******************************************************************************/
//	�^�C�g�����C��
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__Title_H__
#define	__Title_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//
enum
{
	//	�e�N�X�`���ԍ�
	TEX_TITLE_BG = 0,	//	�w�i
	TEX_TITLE_SYS,		//	���̑�

	//	�p�^�[���ԍ�
	PAT_TITLE_BG = 0,	//	�w�i
	PAT_TITLE_TEXT1,	//	�uCAN YOU�v
	PAT_TITLE_TEXT2,	//	�uESCAPA2�v
	PAT_TITLE_TEXT3,	//	�uPUSH START�v
	PAT_TITLE_TEXT4,	//	�uGAME_START�v
	PAT_TITLE_TEXT5,	//	�uOPTION�v

	//	�|���S���ԍ�
	POL_TITLE_BG = 0,	//	�w�i
	POL_TITLE_TEXT1,	//	�uCAN YOU�v
	POL_TITLE_TEXT2,	//	�uESCAPA2�v
	POL_TITLE_TEXT3,	//	�uPUSH START�v
	POL_TITLE_TEXT4,	//	�uGAME_START�v
	POL_TITLE_TEXT5,	//	�uOPTION�v

	//	�I�u�W�F�N�g�ԍ�
	OBJ_TITLE_BG = 0,	//	�w�i
	OBJ_TITLE_TEXT1,	//	�uCAN YOU�v
	OBJ_TITLE_TEXT2,	//	�uESCAPA2�v
	OBJ_TITLE_TEXT3,	//	�uPUSH START�v
	OBJ_TITLE_TEXT4,	//	�uGAME_START�v
	OBJ_TITLE_TEXT5,	//	�uOPTION�v

	//	���̑�
	TITLE_TIME = 60,
};

#endif	//	__Title_H__

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	TitleInit( void );	//	�^�C�g��������
extern	short	TitleMain( void );	//	�^�C�g�����C��
