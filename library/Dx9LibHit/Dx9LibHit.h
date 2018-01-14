/******************************************************************************/
//	�����蔻�胉�C�u����
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__Dx9LibHit_H__
#define	__Dx9LibHit_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	�����蔻��ő吔
#define	HIT_MAX		1024

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	�����蔻��̈�p�\����
typedef	struct
{
	short			PosX[2];		//	���S����̍��[�A�E�[�̋���
	short			PosY[2];		//	���S����̏�[�A���[�̋���
}ST_HitArea;

////////////////////////////////////////
//	�����蔻��p�\����
typedef	struct
{
	UChar			Use;			//	�g�p�t���O
	UChar			Flag;			//	�t���O�i0:��~�A1:���[�v�j
	UChar			State;			//	��ԁi0:��~�A1:�Đ����j

	long			PolNo;			//	�|���S���ԍ�
	long			AnmNo;			//	�A�j���ԍ�
	long			FrameNo;		//	�t���[���ԍ�

	ST_HitArea		AREA;			//	�����蔻��̈�
}ST_HitData;

////////////////////////////////////////
//	�����蔻��ǂݍ��ݗp�\����
typedef	struct
{
	UChar			Use;			//	�g�p�t���O
	ST_HitData		HDT[HIT_MAX];	//	�ǂݍ��ݗp�\����
}ST_HitLoad;

#endif	//	__Dx9LibHit_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	char		HitFilePath[MAX_PATH];	//	�p�X
extern	ST_HitData	HIT[HIT_MAX];
extern	ST_HitLoad	HLT[HIT_MAX];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	InitHit( long HitNo );
extern	void	InitHitAll( void );
extern	void	LoadHitData( char *FileName, char *LoadBuf, long HitNo );
extern	void	SetHitData( long AnmNo, long HitNo, long FileNo, long HitDataNo, UChar Flag );
extern	short	CheackHit( long HitNo1, long HitNo2 );
extern	void	DispHitAll( long PolNo );
extern	void	DispHit( long HitNo, long DispPolNo );
extern	void	DispHitData( long HitNo, long TextNo, double PosX, double PosY );
