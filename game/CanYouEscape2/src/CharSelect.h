/******************************************************************************/
//	�L�����N�^�[�Z���N�g
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__CharSelect_H__
#define	__CharSelect_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//
enum
{
	//	�e�N�X�`���ԍ�
	TEX_CHAR_SELECT_BG = 0,
	TEX_CHAR_SELECT_CHAR,
	TEX_CHAR_SELECT_SHOT,

	//	�p�^�[���ԍ�
	PAT_CHAR_SELECT_BG = 0,
	PAT_CHAR_SELECT_1P,
	PAT_CHAR_SELECT_2P,
	PAT_CHAR_SELECT_SHOT,

	//	�|���S���ԍ�
	POL_CHAR_SELECT_BG = 0,
	POL_CHAR_SELECT_CUR_1P,
	POL_CHAR_SELECT_CUR_2P,
	POL_CHAR_SELECT_1P,
	POL_CHAR_SELECT_2P,
	POL_CHAR_SELECT_SHOT,

	//	�I�u�W�F�N�g�ԍ�
	OBJ_CHAR_SELECT_BG = 0,
	OBJ_CHAR_SELECT_ICON,
	OBJ_CHAR_SELECT_CUR_1P,
	OBJ_CHAR_SELECT_CUR_2P,
	OBJ_CHAR_SELECT_1P,
	OBJ_CHAR_SELECT_2P,
	OBJ_CHAR_SELECT_SHOT,
};

#endif	//	__CharSelect_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	short	SelectFlag[2];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	CharSelectInit( void );	//	�L�����Z���N�g������
extern	void	CharSelectMain( void );	//	�L�����Z���N�g���C��
