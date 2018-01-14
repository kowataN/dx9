/******************************************************************************/
//	�A�j�����C�u����
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__Dx9LibAnm_H__
#define	__Dx9LibAnm_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	�A�j���ő吔
#define	ANM_MAX		100

//	�t���O
enum
{
	ANM_LOOP = 0,	//	���[�v
	ANM_STOP,		//	��~
	ANM_END,		//	�I��
};

//	�e�C���f�b�N�X�T�C�Y
#define	PAT_INDEX_SIZE		(2+2+2+2)
#define	ANM_INDEX_SIZE		(4+8+8+8+8+8+4+8+8+8+8+2)

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	�A�j���e�N�X�`���f�[�^
typedef	struct
{
	long	TextureNo;					//	�e�N�X�`���ԍ�
	char	FileName[MAX_PATH];			//	�t�@�C����
}ST_AnmTexture;

////////////////////////////////////////
//	�A�j���p�^�[���f�[�^�p�\����
typedef	struct
{
	long	TextureNo;					//	�e�N�X�`���ԍ�
	short	PosX,PosY;					//	�؂���J�n���W
	short	Width,Height;				//	�T�C�Y
}ST_AnmPattern;

////////////////////////////////////////
//	�A�j���[�V�����p�t���[���\����
typedef	struct
{
	long	PatNo;						//	�p�^�[���ԍ�
	double	CorrectX;					//	�␳���W�w
	double	CorrectY;					//	�␳���W�x
	double	ScaleX;						//	�g�嗦�w
	double	ScaleY;						//	�g�嗦�x
	double	Rot;						//	��]�p�x
	long	Time;						//	�\���t���[����
	double	A;							//	�s�����x
	double	R,G,B;						//	�e�F����
	short	Rev;						//	���]�t���O
}ST_AnmFrame;

////////////////////////////////////////
//	�A�j���[�V�����p�\����
typedef	struct
{
	UChar			Use;				//	�g�p�t���O
	UChar			Flag;				//	�t���O�i0:���[�v�A1:��~�A2:�I���j
	UChar			State;				//	��ԁi0:��~�A1:�Đ����j

	long			FileNo;				//	�ǂݍ��񂾃t�@�C���ԍ�
	long			FrameMax;			//	�t���[���̍ő吔
	long			FrmNo;				//	�t���[���ԍ�
	long			DataNo;				//	�A�j���f�[�^�ԍ�

	long			PolygonNo;			//	�|���S���ԍ�
	long			PatNo;				//	�p�^�[���ԍ�

	ST_AnmFrame		FRM;				//	�t���[���f�[�^(�J�����g�f�[�^)
	ST_AnmPattern	PAT;				//	�p�^�[���f�[�^(�J�����g�f�[�^)

	long			TimeCt;				//	�^�C���J�E���^

	UShort			Atr;				//	�A�g���r���[�g
	char			*AnmAddr;			//	�A�j���f�[�^�̃A�h���X
}ST_AnmData;

#endif	//	__Dx9LibAnm_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�A�j���[�V�����p�\����
extern	ST_AnmData	ANM[ANM_MAX];
extern	char		AnmFilePath[MAX_PATH];
extern	char		AnmFileName[MAX_PATH];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	InitAnmAll( void );
extern	void	InitAnm( long AnmNo );
extern	void	LoadAnm( long FileNo, char *FileName, char *Buf, long TextureNo );
extern	void	SetAnm( long AnmNo, long FileNo, long DataNo, long PatNo, char *AnmAddr );
extern	void	SetAnmPolygon( long PolNo, long AnmNo, double PosX, double PosY, double PosZ, UChar Flag );
extern	void	LoopAnm( void );
extern	void	CopyAnmData( long AnmNo, long CopyAnmNo );

