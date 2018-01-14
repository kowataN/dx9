/******************************************************************************/
//	DirectX9���C�u����
//		�t�F�[�h
/******************************************************************************/

#ifndef	__Dx9LibFade_H__
#define	__Dx9LibFade_H__

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	�t�F�[�h�p�\����
typedef	struct
{
	UChar	Use;						//	�g�p�t���O
	UChar	Disp;						//	�\���t���O
	UChar	Init;						//	�������t���O
	UChar	State;						//	��ԁi0:�I���A1:�t�F�[�h���j
	long	Time;						//	���v����
	long	Count;						//	�J�E���^
	double	A,R,G,B;					//	�J�����g�̐F
	double	StartA;						//	�J�n�F(A)
	double	StartR,StartG,StartB;		//	�J�n�F(�e�F����)
	double	TargetA;					//	�ڕW�F(A)
	double	TargetR,TargetG,TargetB;	//	�ڕW�F(�e�F����)
}ST_FadeInfo;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CDx9LibFade
{
private:
	ST_FadeInfo	m_Fade;

public:
	void	Init( void );				//	�t�F�[�h������
	void	Set( long Time, double a1, double r1, double g1, double b1, double a2, double r2, double g2, double b2 );
	void	Loop( void );				//	�t�F�[�h���[�v
	void	BlackOut( long Time );		//	�u���b�N�A�E�g
	void	BlackIn( long Time );		//	�u���b�N�C��
	void	WhiteOut( long Time );		//	�z���C�g�A�E�g
	void	WhiteIn( long Time );		//	�z���C�g�C��
	short	GetState( void );			//	��Ԏ擾
	long	GetProgress( void );		//	�i�s�󋵎擾
private:
	void	Draw( void );				//	�`��
};

#endif	//	__Dx9LibFade_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	CDx9LibFade	g_LibFade;
