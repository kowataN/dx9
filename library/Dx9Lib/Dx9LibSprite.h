/******************************************************************************/
//	DirectX9���C�u����
//		�X�v���C�g
/******************************************************************************/

#ifndef	__Dx9LibSprite_H__
#define	__Dx9LibSprite_H__

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	�X�v���C�g�p�\����
typedef	struct
{
	UChar	Use;						//	�g�p�t���O
	UChar	Disp;						//	�\���t���O
	long	PatNo;						//	�p�^�[���ԍ�
	long	Width,Height;				//	�T�C�Y
	long	CenterX,CenterY;			//	���S���W
	UChar	A;							//	�A���t�@
	UChar	R,G,B;						//	�e�F����
	double	PosX,PosY,PosZ;				//	�\�����W
	double	MoveX,MoveY;				//	�ړ���
	double	PosXBk,PosYBk;				//	�\�����W�̃o�b�N�A�b�v
	double	ScaleX,ScaleY;				//	�X�P�[��
	double	Rot;						//	�p�x
	UShort	Atr;						//	�A�g���r���[�g
}ST_SpriteInfo;

#endif	//	__Dx9LibSprite_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	ST_SpriteInfo		SP[SP_MAX];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	CreateSpriteDevice( void );
extern	void	InitSpriteAll( void );
extern	void	InitSprite( long SpriteNo );
extern	void	DispSpriteAll( void );
extern	void	DispSprite( long SpriteNo );
extern	void	SetSprite( long SpriteNo, long PatNo, double PosX, double PosY, double PosZ, UShort Atr );
extern	void	SetSpriteCenter( long SpriteNo, long CenterX, long CenterY );
extern	void	SetSpriteColor( long SpriteNo, UChar A, UChar R, UChar G, UChar B );
extern	void	SetSpriteScale( long SpriteNo, double Scale );
extern	void	SetSpritePos( long SpriteNo, double PosX, double PosY );
extern	long	GetSpriteNo( void );
extern	void	DispChangeSprite( UChar Flag );
extern	void	ReleaseSprite( void );
