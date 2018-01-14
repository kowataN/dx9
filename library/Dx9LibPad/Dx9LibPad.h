/******************************************************************************/
//	�p�b�h���C�u����
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__Dx9LibPad_H__
#define	__Dx9LibPad_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	�p�b�h�ő吔
#define	PAD_MAX		2

//	�L�[���s�[�g
#define	KEY_REPEAT_START	10
#define	KEY_REPEAT_TIME		15

//	�{�^���ԍ�
enum
{
	PAD_UP = 0,
	PAD_DOWN,
	PAD_LEFT,
	PAD_RIGHT,

	PAD_BUT1,
	PAD_BUT2,
	PAD_BUT3,
	PAD_BUT4,
	PAD_BUT5,
	PAD_BUT6,
	PAD_BUT7,
	PAD_BUT8,
	PAD_BUT9,
	PAD_BUT10,
	PAD_BUT11,
	PAD_BUT12,
	PAD_BUTTON_MAX
};

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	�p�b�h�\����
typedef	struct
{
	UChar	Use;					//	�g�p�t���O
	UChar	JoyUse;					//	�W���C�X�e�B�b�N�g�p�t���O

	UChar	Trg[PAD_BUTTON_MAX];	//	�g���K
	UChar	TrgBk[PAD_BUTTON_MAX];	//	�g���K�i�o�b�N�A�b�v�j
	UChar	Push[PAD_BUTTON_MAX];	//	�v�b�V��
	UChar	PushBk[PAD_BUTTON_MAX];	//	�v�b�V���i�o�b�N�A�b�v�j
	UChar	Rpt[PAD_BUTTON_MAX];	//	���s�[�g
	long	Count[PAD_BUTTON_MAX];	//	�J�E���^

	short	KeyNo[PAD_BUTTON_MAX];	//	���蓖�Ă��Ă���L�[�{�[�h�ԍ�
	short	JoyNo[PAD_BUTTON_MAX];	//	���蓖�Ă��Ă���W���C�X�e�B�b�N�̃L�[�ԍ�
}ST_PadData;

#endif	//	__Dx9LibPad_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	short		SetKeyDat[PAD_MAX][PAD_BUTTON_MAX];
extern	short		SetJoyDat[PAD_MAX][PAD_BUTTON_MAX];
extern	ST_PadData	PAD[PAD_MAX];

//----------------------------------------------------------------------------//
//	�O���錾
//----------------------------------------------------------------------------//
extern	void	InitPadAll( void );
extern	void	InitPad( long PadNo );
extern	void	LoopPad( void );
extern	void	LoadPadData( char *FileName );
