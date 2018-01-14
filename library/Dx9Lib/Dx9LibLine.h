/******************************************************************************/
//	DirectX9���C�u����
//		���C��
/******************************************************************************/

#ifndef	__Dx9LibLine_H__
#define	__Dx9LibLine_H__

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	���C���\����
typedef struct
{
	UChar	Use;						//	�g�p�t���O
	UChar	Disp;						//	�\���t���O
	double	StartX;						//	�J�n�_X
	double	StartY;						//	�J�n�_Y
	double	EndX;						//	�I�_X
	double	EndY;						//	�I�_Y
	UChar	A;							//	�A���t�@
	UChar	R,G,B;						//	�e�F����
	UShort	Atr;						//	�A�g���r���[�g
}ST_LineInfo;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class CDx9LibLine
{
protected:
	LPD3DXLINE		m_lpLine;

public:
	CDx9LibLine();
	~CDx9LibLine();
	void	CreateDevice( void );
	void	InitAll( void );
	void	Init( long No );
	void	DispAll( void );
	void	DrawLine( long No );
	void	DrawBox( long No );
	void	Set( long No, double StartX, double StartY, double EndX, double EndY, UShort Atr );
	void	SetColor( long No, UChar A, UChar R, UChar G, UChar B );
	short	GetNo( void );
	void	DispChange( UChar Flag );
	void	ReleaseDevice( void );
};

#endif	//	__Dx9LibLine_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	ST_LineInfo	LINE[LINE_MAX];
extern	CDx9LibLine	g_LibLine;
