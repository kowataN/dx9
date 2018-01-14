/******************************************************************************/
//	DirectX9���C�u����
//		�e�L�X�g
/******************************************************************************/

#ifndef	__Dx9LibText_H__
#define	__Dx9LibText_H__

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	<string>
#include	<vector>
using namespace std;

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	�e�L�X�g�A�g���r���[�g
#define	ATR_ONCE	1	//	�P�����Â\��
//#define	ATR_ITALI	1	//	�C�^���b�N
//#define	ATR_UNDAR	2	//	�A���_�[���C��
//#define	ATR_DENY	4	//	�ł�������

//	�J���[�ݒ�
#define	COLOR_RED	D3DCOLOR_ARGB( 255,255,0,0)
#define	COLOR_GREEN	D3DCOLOR_ARGB( 255,0,255,0)
#define	COLOR_BLUE	D3DCOLOR_ARGB( 255,0,0,255)

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//
typedef	struct
{
	UChar	Disp;						//	�\���t���O
	UChar	Use;						//	�g�p�t���O
	double	PosX,PosY;					//	�\�����W
	double	CenterX,CenterY;			//	���S���W
	long	Width,Height;				//	�\���T�C�Y
	long	Time;						//	�\������
	UChar	A,R,G,B;					//	�e�F����
	char	Str[MAX_PATH];				//	������
	long	Step;						//	���݂̕\��������
	long	Speed;						//	�\�����x
	long	Count;						//	�J�E���^
	UChar	State;						//	�\�����(0:�\�����A1:�\������)
	UChar	Atr;						//	�A�g���r���[�g
}ST_TextInfo;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CDx9LibText
{
private:
	LPD3DXFONT		m_lpFont;		//	�e�L�X�g�f�o�C�X
	LPD3DXSPRITE	m_lpTxtSprite;	//	�X�v���C�g�I�u�W�F�N�g

public:
	CDx9LibText();
	~CDx9LibText();

	void	InitDevice( void );
	void	InitAll( void );
	void	Init( long TextNo );
	void	DispAll( void );
	void	Set( long TextNo, double PosX, double PosY, UChar A, UChar R, UChar G, UChar B, char *Str, UChar Atr=ATR_NONE, long Speed=1 );
	void	ChangeDisp( UChar Flag );
	void	ChangeDisp( long TextNo, UChar Flag );
	void	SetColor( long TextNo, UChar A, UChar R, UChar G, UChar B );
	void	SetPos( long TextNo, double PosX, double PosY );
	void	SetCenter( long TextNo );
	void	SetRect( long TextNo, long Width, long Height );
	void	CopyData( long TextNo, long CopyNo );
	void	SetTimer( long TextNo, long Time );
	void	Release();
private:
	void	Disp( long TextNo );
};

#endif	//	__Dx9LibText_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	ST_TextInfo		TXT[TXT_MAX];
extern	CDx9LibText	g_LibText;
