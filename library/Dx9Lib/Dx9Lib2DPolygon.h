/******************************************************************************/
//	DirectX9���C�u����
//		�|���S��
/******************************************************************************/

#ifndef	__Dx9Lib2DPolygon_H__
#define	__Dx9Lib2DPolygon_H__

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	�Q�c�|���S���Ŏg�p���钸�_
typedef	struct
{
	float		x,y,z;					//	���W
	float		rhw;
	D3DCOLOR	color;					//	���_�F���w��
	float		tu,tv;					//	�e�N�X�`�����W�w��
}ST_VertexInfo;

////////////////////////////////////////
//	�|���S���p�\����
typedef	struct
{
	UChar	Use;						//	�g�p�t���O
	UChar	Disp;						//	�\���t���O
	UChar	PatUse;						//	�p�^�[���g�p�t���O
	long	PatNo;						//	�p�^�[���ԍ�
	long	Width,Height;				//	�T�C�Y
	double	PosX,PosY,PosZ;				//	�\�����W
	double	PosXBk,PosYBk,PosZBk;		//	�o�b�N�A�b�v���W
	double	ScaleX,ScaleY;				//	�X�P�[��
	double	RotX,RotY,RotZ;				//	�p�x
	short	CenterX,CenterY;			//	���S���W
	UChar	A;							//	�A���t�@
	UChar	R,G,B;						//	�e�F����
	UShort	Atr;						//	�A�g���r���[�g
}ST_PolygonInfo;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CDx9LibPolygon
{
protected:
	ST_PolygonInfo	m_Info[POL_MAX];

public:
	CDx9LibPolygon();
	~CDx9LibPolygon();

	void	InitAll( void );
	void	Init( long PolNo );
	void	DispAll( void );
	void	Set( long PolNo, long w, long h, double x, double y, double z, UShort atr );
	void	SetPat( long PolNo, long PatNo, double PosX, double PosY, double PosZ, UShort Atr );
	void	SetColor( long PolNo, UChar A, UChar R, UChar G, UChar B );
	void	SetColor( long PolNo, UChar A );
	void	SetPos( long PolNo, double PosX, double PosY );
	void	SetPos( long PolNo, double PosX, double PosY, double PosZ );
	void	SetPosX( long PolNo, double PosX );
	void	SetPosY( long PolNo, double PosY );
	void	SetPosZ( long PolNo, double PosZ );
	void	SetCenter( long PolNo );
	void	SetCenter( long PolNo, short CenterX, short CenterY );
	void	SetScale( long PolNo, double Scale );
	void	SetSize( long PolNo, long Width, long Height );
	void	SetWidth( long PolNo, long Width );
	void	SetHeight( long PolNo, long Height );
	void	SetRotY( long PolNo, double Rot );
	void	SetRotZ( long PolNo, double Rot );
	void	SetPatNo( long PolNo, long PatNo );
	ST_PolygonInfo	GetInfo( long PolNo );
	void	SetInfo( long PolNo, ST_PolygonInfo Info );
	void	CopyData( long PolNo, long CopyNo );
	long	GetNo( void );
	void	ChangeDisp( UChar Flag );
	void	ChangeDisp( long PolNo, UChar Flag );
	void	ChangeDispRev( long PolNo );
	void	AddPos( long PolNo, double AddX, double AddY );
	void	AddPosX( long PolNo, double AddX );
	void	AddPosY( long PolNo, double AddY );
	void	AddRot( long PolNo, double AddX, double AddY, double AddZ );
	void	AddScale( long PolNo, double Add );
	void	AddWidth( long PolNo, long Add );
	void	AddHeight( long PolNo, long Add );
	void	AddColor( long PolNo, UChar AddA, UChar AddR, UChar AddG, UChar AddB );
	void	PosBackup( long PolNo );
	void	PosRestore( long PolNo );
	void	ClippingPosX( long PolNo, double MinPos, double MaxPos );
	void	ClippingPosY( long PolNo, double MinPos, double MaxPos );

private:
	void	Disp( long PolNo );
	void	DispBox( long PolNo, ST_VertexInfo *Vertex );
	void	DispPat( long PolNo, ST_VertexInfo *Vertex );
	void	Update( ST_VertexInfo* Vertex, float MoveX, float MoveY, float MoveZ );
};

#endif	//	__Dx9Lib2DPolygon_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	CDx9LibPolygon	g_LibPol;
