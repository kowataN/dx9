/******************************************************************************/
//	���[�u���C�u����
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__Dx9LibMove_H__
#define	__Dx9LibMove_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	���[�u�f�[�^�ő吔
#define	MOV_MAX		100

//	�e�C���f�b�N�X�T�C�Y
#define	MOV_INDEX_SIZE		(2+2+2)

//	�t���O
enum
{
	MOVE_LOOP = 0,	//	���[�v
	MOVE_STOP,		//	��~
	MOVE_END,		//	�I��
};

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	���[�u�p�t���[���\����
typedef	struct
{
	long			Time;			//	�t���[������
	double			InitX,InitY;	//	�������W
	double			PosX,PosY;		//	�\�����W
	short			MoveX,MoveY;	//	�ړ���
}ST_MoveFrame;

////////////////////////////////////////
//	���[�u�p�\����
typedef	struct
{
	UChar			Use;			//	�g�p�t���O
	UChar			ExecFlag;		//	�t���O�i0:���[�v�A1:��~�A2:�I���j
	UChar			State;			//	��ԁi0:��~�A1:�Đ����j

	long			FrameMax;		//	�t���[���̍ő吔
	long			FrameNo;		//	�t���[���ԍ�
	long			DataNo;			//	�f�[�^�ԍ�
	long			PolNo;			//	�|���S���ԍ�

	long			TimeCount;		//	�^�C���J�E���^
	ST_MoveFrame	FRM;			//	�t���[���f�[�^(�J�����g�f�[�^)
	char			*DataAddr;		//	���[�u�f�[�^�̃A�h���X
}ST_MoveData;

#endif	//	__Dx9LibMove_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	ST_MoveData	MOV[MOV_MAX];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	InitMoveAll( void );
extern	void	InitMove( long MoveNo );
extern	void	LoadMoveFile( char *FileName, char *LoadBuf );
extern	void	SetMove( long MoveNo, long PolNo, long DataNo, char *DataAddr );
extern	void	LoopMove( void );
