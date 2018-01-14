/******************************************************************************/
//	DirectX9���C�u����
//		�p�^�[��
/******************************************************************************/

#ifndef	__Dx9LibPattern_H__
#define	__Dx9LibPattern_H__

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	�p�^�[���p�\����
typedef	struct
{
	UChar	Use;			//	�p�^�[���̎g�p�t���O
	long	TextureNo;		//	�e�N�X�`���ԍ�
	long	PosX,PosY;		//	�؂���J�n���W
	long	Width,Height;	//	�T�C�Y
}ST_PatternInfo;

#endif	//	__Dx9LibPattern_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	ST_PatternInfo	PAT[PAT_MAX];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	InitPattern( long PatNo );
extern	void	InitPatternAll( void );
extern	void	SetPattern( long PatNo, long TexureNo, long PosX, long PosY, long Width, long Height );
extern	void	CopyPatternData( long DstPatNo, long SrcPatNo );
extern	long	GetPatternNo( void );
