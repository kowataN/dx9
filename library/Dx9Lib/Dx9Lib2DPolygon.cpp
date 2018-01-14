/******************************************************************************/
//	DirectX9���C�u����
//		�|���S��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"



/******************************************************************************/
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibPolygon::CDx9LibPolygon()
{
	//	�������Ȃ�
}

/******************************************************************************/
//	���O	�F	�f�X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibPolygon::~CDx9LibPolygon()
{
	//	�������Ȃ�
}

/******************************************************************************/
//	���O	�F	�\���̑S������
//	����	�F	�|���S���p�\���̂̑S������
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�����ŁuInitPolygon�v���Ă�ł���
/******************************************************************************/
void	CDx9LibPolygon::InitAll( void )
{
	for( long i=0; i<POL_MAX; i++ )
	{
		this->Init( i );
	}
}

/******************************************************************************/
//	���O	�F	�|���S���p�\���̏�����
//	����	�F	�|���S���p�\���̂̏�����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::Init( long PolNo )
{
	if( PolNo >= POL_MAX )
	{
		return;
	}

	//	�t���O
	m_Info[ PolNo ].Use		= OFF;	//	�g�p�t���O
	m_Info[ PolNo ].Disp	= OFF;	//	�\���t���O
	m_Info[ PolNo ].PatUse	= OFF;	//	�p�^�[���g�p�t���O

	//	�ԍ�
	m_Info[ PolNo ].PatNo	= NOT;	//	�p�^�[��

	//	�\�����W
	m_Info[ PolNo ].PosX	= 0;
	m_Info[ PolNo ].PosY	= 0;
	m_Info[ PolNo ].PosZ	= 0;

	//	�o�b�N�A�b�v���W
	m_Info[ PolNo ].PosXBk	= 0;
	m_Info[ PolNo ].PosYBk	= 0;
	m_Info[ PolNo ].PosZBk	= 0;

	//	���S���W
	m_Info[ PolNo ].CenterX	= 0;
	m_Info[ PolNo ].CenterY	= 0;

	//	��]�p�x
	m_Info[ PolNo ].RotX	= 0;
	m_Info[ PolNo ].RotY	= 0;
	m_Info[ PolNo ].RotZ	= 0;

	//	�X�P�[��
	m_Info[ PolNo ].ScaleX	= 1.0;
	m_Info[ PolNo ].ScaleY	= 1.0;

	//	�T�C�Y
	m_Info[ PolNo ].Width	= 0;
	m_Info[ PolNo ].Height	= 0;

	//	�e�F����
	m_Info[ PolNo ].A		= 255;
	m_Info[ PolNo ].R		= 255;
	m_Info[ PolNo ].G		= 255;
	m_Info[ PolNo ].B		= 255;

	//	�A�g���r���[�g
	m_Info[ PolNo ].Atr		= ATR_NONE;
}

/******************************************************************************/
//	���O	�F	�|���S���S�`��
//	����	�F	�|���S���̕`����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�����ŁuDispPolygon�v���Ă�ł���
/******************************************************************************/
void	CDx9LibPolygon::DispAll( void )
{
	short	i,j,bak;
	short	no[POL_MAX],max=0;

	//	����
	for( i=0; i<POL_MAX; i++ )
	{
		//	���̔ԍ����g���Ă��邩
		if( m_Info[i].Use && m_Info[i].Disp )
		{
			//	�z��ɔԍ����i�[
			no[max] = i;
			max ++;
		}
	}

	//	�y�\�[�g
	for( i=max-1; i>=1; i-- )	//	���
	{
		for( j=i-1; j>=0; j-- )	//	�O
		{
			//	���ƑO���r
			if( m_Info[ no[i] ].PosZ < m_Info[ no[j] ].PosZ )
			{
				bak   = no[i];
				no[i] = no[j];
				no[j] = bak;
			}
		}
	}

	//	�`��
	for( i=0; i<max; i++ )
	{
		this->Disp( no[i] );
	}
}

/******************************************************************************/
//	���O	�F	�|���S���`��
//	����	�F	�|���S���̕`����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo	�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::Disp( long PolNo )
{
	ST_VertexInfo	Vertex[4];

	/////////////////////////////////////
	//	���_�̏�����
	for( int i=0; i<4; i++ )
	{
		Vertex[ i ].z	= (float)m_Info[ PolNo ].PosZ;
		Vertex[ i ].rhw	= 1.0f;
		Vertex[ i ].color = 
			D3DCOLOR_RGBA( m_Info[ PolNo ].R, m_Info[ PolNo ].G, m_Info[ PolNo ].B, m_Info[ PolNo ].A );
	}

	if( !m_Info[ PolNo ].PatUse )
	{
		this->DispBox( PolNo, Vertex );
	}
	else
	{
		this->DispPat( PolNo, Vertex );
	}
}

/******************************************************************************/
//	���O	�F	�|���S����`�`��
//	����	�F	��`�|���S���̕`����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo	�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::DispBox( long PolNo, ST_VertexInfo *Vertex )
{
	long		PatNo,TexNo;
	short		i,j;
	float		TexW,TexH,PatX,PatY,PatW,PatH,CenterX,CenterY,PolX,PolY;

	//	�l���
	PatNo	= m_Info[ PolNo ].PatNo;			//	�p�^�[���ԍ�
	TexNo	= PAT[ PatNo ].TextureNo;			//	�e�N�X�`���ԍ�

	TexW	= (float)TEX[ TexNo ].Width;		//	�e�N�X�`���T�C�Y�E��
	TexH	= (float)TEX[ TexNo ].Height;		//	�e�N�X�`���T�C�Y�E�c

	PatX	= (float)PAT[ PatNo ].PosX;			//	�p�^�[��X
	PatY	= (float)PAT[ PatNo ].PosY;			//	�p�^�[��Y
	PatW	= (float)m_Info[ PolNo ].Width;		//	�p�^�[���T�C�Y�E��
	PatH	= (float)m_Info[ PolNo ].Height;	//	�p�^�[���T�C�Y�E�c

	PolX	= (float)m_Info[ PolNo ].PosX;		//	�\�����WX
	PolY	= (float)m_Info[ PolNo ].PosY;		//	�\�����WY
	CenterX	= (float)m_Info[ PolNo ].CenterX;	//	���S���WX
	CenterY	= (float)m_Info[ PolNo ].CenterY;	//	���S���WY

	D3DXMATRIX	matWorld,matRotX,matRotY,matRotZ,matTrans;
	D3DXMATRIX	matPos;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRotX);
	D3DXMatrixIdentity(&matRotY);
	D3DXMatrixIdentity(&matRotZ);
	D3DXMatrixIdentity(&matPos);

	//	�p�x�␳
	CorrectRot( m_Info[PolNo].RotX );
	CorrectRot( m_Info[PolNo].RotY );
	CorrectRot( m_Info[PolNo].RotZ );

	//	��]
	D3DXMatrixRotationX( &matRotX, (float)DegToRad( m_Info[PolNo].RotX ) );
	D3DXMatrixRotationY( &matRotY, (float)DegToRad( m_Info[PolNo].RotY ) );
	D3DXMatrixRotationZ( &matRotZ, (float)DegToRad( m_Info[PolNo].RotZ ) );

	Vertex[ 0 ].tu = 0;
	Vertex[ 0 ].tv = 0;
	Vertex[ 1 ].tu = 1;
	Vertex[ 1 ].tv = 0;
	Vertex[ 2 ].tu = 1;
	Vertex[ 2 ].tv = 1;
	Vertex[ 3 ].tu = 0;
	Vertex[ 3 ].tv = 1;

	//	���W
	switch( m_Info[ PolNo ].Atr )
	{
		case	ATR_NONE:
			Vertex[ 0 ].x = PolX - CenterX;
			Vertex[ 0 ].y = PolY - CenterY;
			Vertex[ 1 ].x = PolX - CenterX + PatW;
			Vertex[ 1 ].y = PolY - CenterY;
			Vertex[ 2 ].x = PolX - CenterX + PatW;
			Vertex[ 2 ].y = PolY - CenterY + PatH;
			Vertex[ 3 ].x = PolX - CenterX;
			Vertex[ 3 ].y = PolY - CenterY + PatH;

			//	���_�ɖ߂�
			this->Update( Vertex, -PolX, -PolY, 0.0f );
			//	�e����W�v�Z
			for( i=0; i<4; i++ )
			{
				//	�ړ�
				D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );
				//	�p�x
				matPos *= matRotX;
				matPos *= matRotY;
				matPos *= matRotZ;

				//	���Z���ʂ���
				Vertex[i].x = matPos._41;
				Vertex[i].y = matPos._42;
				Vertex[i].z = matPos._43;
			}

			//	���̈ʒu�ɖ߂�
			this->Update( Vertex, PolX, PolY, 0.0f );

			//	�e�N�X�`��
			g_lpD3DDev->SetTexture( 0, NULL );
			//	���_�t�H�[�}�b�g�̐ݒ�
			g_lpD3DDev->SetFVF( FVF_TLVERTEX );
			//	�|���S���`��
			g_lpD3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( ST_VertexInfo ) );

			break;

		case	ATR_BOX:

			for( j=0; j<2; j++ )
			{
				if( j )
				{
					Vertex[ 0 ].x = PolX - CenterX;
					Vertex[ 0 ].y = PolY - CenterY;
					Vertex[ 1 ].x = PolX - CenterX;
					Vertex[ 1 ].y = PolY - CenterY + PatH;
					Vertex[ 2 ].x = PolX - CenterX + PatW;
					Vertex[ 2 ].y = PolY - CenterY + PatH;
					Vertex[ 3 ].x = PolX - CenterX + PatW;
					Vertex[ 3 ].y = PolY - CenterY;
				}
				else
				{
					Vertex[ 0 ].x = PolX - CenterX;
					Vertex[ 0 ].y = PolY - CenterY;
					Vertex[ 1 ].x = PolX - CenterX + PatW;
					Vertex[ 1 ].y = PolY - CenterY;
					Vertex[ 2 ].x = PolX - CenterX + PatW;
					Vertex[ 2 ].y = PolY - CenterY + PatH;
					Vertex[ 3 ].x = PolX - CenterX;
					Vertex[ 3 ].y = PolY - CenterY + PatH;
				}

				//	���_�ɖ߂�
				this->Update( Vertex, -PolX, -PolY, 0.0f );
				//	�e����W�v�Z
				for( i=0; i<4; i++ )
				{
					//	�ړ�
					D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );
					//	�p�x
					matPos *= matRotX;
					matPos *= matRotY;
					matPos *= matRotZ;

					//	���Z���ʂ���
					Vertex[i].x = matPos._41;
					Vertex[i].y = matPos._42;
					Vertex[i].z = matPos._43;
				}

				//	���̈ʒu�ɖ߂�
				this->Update( Vertex, PolX, PolY, 0.0f );

				//	�e�N�X�`��
				g_lpD3DDev->SetTexture( 0, NULL );
				//	���_�t�H�[�}�b�g�̐ݒ�
				g_lpD3DDev->SetFVF( FVF_TLVERTEX );
				//	�|���S���`��
				g_lpD3DDev->DrawPrimitiveUP( D3DPT_LINESTRIP, 2, Vertex, sizeof( ST_VertexInfo ) );
			}

			break;
	}
}

/******************************************************************************/
//	���O	�F	�p�^�[���t���|���S���`��
//	����	�F	�p�^�[���t���|���S���̕`����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo	�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::DispPat( long PolNo, ST_VertexInfo *Vertex )
{
	long		PatNo,TexNo;
	float		TexW,TexH;
	float		PatX,PatY,PatW,PatH;
	float		PolX,PolY,CenterX,CenterY,ScaleX,ScaleY;

	//	�l���
	PatNo	= m_Info[ PolNo ].PatNo;			//	�p�^�[���ԍ�
	TexNo	= PAT[ PatNo ].TextureNo;			//	�e�N�X�`���ԍ�

	TexW	= (float)TEX[ TexNo ].Width;		//	�e�N�X�`���T�C�Y�E��
	TexH	= (float)TEX[ TexNo ].Height;		//	�e�N�X�`���T�C�Y�E�c

	PatX	= (float)PAT[ PatNo ].PosX;			//	�p�^�[��X
	PatY	= (float)PAT[ PatNo ].PosY;			//	�p�^�[��Y
	PatW	= (float)m_Info[ PolNo ].Width;		//	�p�^�[���T�C�Y�E��
	PatH	= (float)m_Info[ PolNo ].Height;	//	�p�^�[���T�C�Y�E�c

	PolX	= (float)m_Info[ PolNo ].PosX;		//	�\�����WX
	PolY	= (float)m_Info[ PolNo ].PosY;		//	�\�����WY
	CenterX	= (float)m_Info[ PolNo ].CenterX;	//	���S���WX
	CenterY	= (float)m_Info[ PolNo ].CenterY;	//	���S���WY
	ScaleX	= (float)m_Info[ PolNo ].ScaleX;	//	�g�嗦X
	ScaleY	= (float)m_Info[ PolNo ].ScaleY;	//	�g�嗦Y

	D3DXMATRIX	matWorld,matRotX,matRotY,matRotZ,matTrans,matScale;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRotX);
	D3DXMatrixIdentity(&matRotY);
	D3DXMatrixIdentity(&matRotZ);
	D3DXMatrixIdentity(&matScale);

	//	�p�x�␳
	CorrectRot( m_Info[PolNo].RotX );
	CorrectRot( m_Info[PolNo].RotY );
	CorrectRot( m_Info[PolNo].RotZ );

	//	��]
	D3DXMatrixRotationX( &matRotX, (float)DegToRad( m_Info[PolNo].RotX ) );
	D3DXMatrixRotationY( &matRotY, (float)DegToRad( m_Info[PolNo].RotY ) );
	D3DXMatrixRotationZ( &matRotZ, (float)DegToRad( m_Info[PolNo].RotZ ) );

	//	�g��
	D3DXMatrixScaling( &matScale, ScaleX, ScaleY, 1.0f );

	//	���E���]
	if( ( m_Info[ PolNo ].Atr & ATR_REV_W ) == ATR_REV_W )
	{
		Vertex[ 0 ].x = PolX - CenterX + PatW;
		Vertex[ 1 ].x = PolX - CenterX;
		Vertex[ 2 ].x = PolX - CenterX;
		Vertex[ 3 ].x = PolX - CenterX + PatW;
	}
	else
	{
		Vertex[ 0 ].x = PolX - CenterX;
		Vertex[ 1 ].x = PolX - CenterX + PatW;
		Vertex[ 2 ].x = PolX - CenterX + PatW;
		Vertex[ 3 ].x = PolX - CenterX;
	}

	//	�㉺���]
	if( ( m_Info[ PolNo ].Atr &  ATR_REV_H ) == ATR_REV_H )
	{
		Vertex[ 0 ].y = PolY - CenterY + PatH;
		Vertex[ 1 ].y = PolY - CenterY + PatH;
		Vertex[ 2 ].y = PolY - CenterY;
		Vertex[ 3 ].y = PolY - CenterY;
	}
	else
	{
		Vertex[ 0 ].y = PolY - CenterY;
		Vertex[ 1 ].y = PolY - CenterY;
		Vertex[ 2 ].y = PolY - CenterY + PatH;
		Vertex[ 3 ].y = PolY - CenterY + PatH;
	}

	//	���_�ɖ߂�
	this->Update( Vertex, -PolX, -PolY, 0.0f );

	//	�e����W�v�Z
	D3DXMATRIX matPos;
	for( int i=0; i<4; i++ )
	{
		//	�ړ�
		D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );
		//	�p�x
		matPos *= matRotX;
		matPos *= matRotY;
		matPos *= matRotZ;
		//	�g��
		matPos *= matScale;

		//	���Z���ʂ���
		Vertex[i].x = matPos._41;
		Vertex[i].y = matPos._42;
		Vertex[i].z = matPos._43;
	}

	//	���̈ʒu�ɖ߂�
	this->Update( Vertex, PolX, PolY, 0.0f );

	//	�e�N�X�`�����W
	float tu1 = ( PatX + 0.5f ) / TexW;
	float tv1 = ( PatY + 0.5f ) / TexH;
	float tu2 = ( PatX + PatW ) / TexW;
	float tv2 = ( PatY + PatH ) / TexH;
	Vertex[ 0 ].tu = tu1;
	Vertex[ 0 ].tv = tv1;
	Vertex[ 1 ].tu = tu2;
	Vertex[ 1 ].tv = tv1;
	Vertex[ 2 ].tu = tu2;
	Vertex[ 2 ].tv = tv2;
	Vertex[ 3 ].tu = tu1;
	Vertex[ 3 ].tv = tv2;

	//	�e�N�X�`��
	g_lpD3DDev->SetTexture( 0, TEX[ TexNo ].lpTex );

	//	���_�t�H�[�}�b�g�̐ݒ�
	g_lpD3DDev->SetFVF( FVF_TLVERTEX );

	//	�|���S���`��
	g_lpD3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( ST_VertexInfo ) );
}

/******************************************************************************/
//	���O	�F	�\�����W�X�V
//	����	�F	�\�����W�̍X�V���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN/OUT]TLVERTEX*	Vertex	���_���
//				[IN]float			MoveX	���WX
//				[IN]float			MoveY	���WY
//				[IN]float			MoveZ	���WZ
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::Update( ST_VertexInfo* Vertex, float MoveX, float MoveY, float MoveZ )
{
	D3DXMATRIX	matPos,matMove;

	//	������
	D3DXMatrixIdentity( &matPos );
	D3DXMatrixIdentity( &matMove );

	//	�ړ��ʂ�ݒ�
	D3DXMatrixTranslation( &matMove, MoveX, MoveY, MoveZ );

	for( int i=0;i <4; i++ )
	{
		//	���݂̒�����ݒ�
		D3DXMatrixTranslation( &matPos, Vertex[i].x, Vertex[i].y, Vertex[i].z );

		//	�s��v�Z
		matPos += matMove;

		//	���_���W���X�V
		Vertex[i].x = matPos._41;
		Vertex[i].y = matPos._42;
		Vertex[i].z = matPos._43;
	}
}

/******************************************************************************/
//	���O	�F	2D�|���S���Z�b�g
//	����	�F	�|���S���̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	Width		���T�C�Y
//				[IN]long	Height		�c�T�C�Y
//				[IN]double	PosX		X���W
//				[IN]double	PosY		Y���W
//				[IN]double	PosZ		Z���W
//				[IN]UShort	Atr			�A�g���r���[�g
//	���l	�F	Atr�̐ݒ�l	ATR_NONE	�Ȃ�
//							ATR_BOX		��`�ŕ`��
/******************************************************************************/
void	CDx9LibPolygon::Set(long PolNo,
							long Width, long Height,
							double PosX, double PosY, double PosZ,
							UShort Atr )
{
	if( PolNo >= POL_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	�e�����o������

	//	�t���O
	m_Info[ PolNo ].Use		= ON;
	m_Info[ PolNo ].Disp	= ON;
	m_Info[ PolNo ].PatUse	= OFF;

	m_Info[ PolNo ].PatNo	= NOT;

	//	���W������
	m_Info[ PolNo ].PosX	= PosX;
	m_Info[ PolNo ].PosY	= PosY;
	m_Info[ PolNo ].PosZ	= PosZ;

	//	�T�C�Y
	m_Info[ PolNo ].Width	= Width;
	m_Info[ PolNo ].Height	= Height;

	m_Info[ PolNo ].Atr		= Atr;
}

/******************************************************************************/
//	���O	�F	�|���S���p�^�[���Z�b�g
//	����	�F	�p�^�[���t���|���S���̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	PatNo		�p�^�[���ԍ�
//				[IN]double	PosX		X���W
//				[IN]double	PosY		Y���W
//				[IN]double	PosZ		Z���W
//				[IN]UShort	Atr			�A�g���r���[�g
//	���l	�F	Atr�̐ݒ�l	ATR_NONE	�Ȃ�
//							ATR_REV_W	���E���]
//							ATR_REV_H	�㉺���]
//							ATR_ALL		�㉺���E���]
/******************************************************************************/
void	CDx9LibPolygon::SetPat( long PolNo, long PatNo,
								double PosX, double PosY, double PosZ,
								UShort Atr )
{
	if( PolNo >= POL_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	�e�����o������

	//	�t���O
	m_Info[ PolNo ].Use		= ON;
	m_Info[ PolNo ].Disp	= ON;
	m_Info[ PolNo ].PatUse	= ON;

	m_Info[ PolNo ].PatNo	= PatNo;

	//	���W������
	m_Info[ PolNo ].PosX	= PosX;
	m_Info[ PolNo ].PosY	= PosY;
	m_Info[ PolNo ].PosZ	= PosZ;

	//	�T�C�Y
	m_Info[ PolNo ].Width	= PAT[ PatNo ].Width;
	m_Info[ PolNo ].Height	= PAT[ PatNo ].Height;

	//	�A�g���r���[�g
	m_Info[ PolNo ].Atr		= Atr;
}

/******************************************************************************/
//	���O	�F	�|���S���J���[�Z�b�g
//	����	�F	�|���S���̃f�t���[�Y�F��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]UChar	A			�A���t�@
//				[IN]UChar	R			�Ԑ���
//				[IN]UChar	G			�ΐ���
//				[IN]UChar	B			����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetColor( long PolNo, UChar A, UChar R, UChar G, UChar B )
{
	m_Info[ PolNo ].A = A;
	m_Info[ PolNo ].R = R;
	m_Info[ PolNo ].G = G;
	m_Info[ PolNo ].B = B;
}

/******************************************************************************/
//	���O	�F	�|���S���J���[�Z�b�g
//	����	�F	�|���S���̃f�t���[�Y�F��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]UChar	A			�A���t�@
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetColor( long PolNo, UChar A )
{
	m_Info[ PolNo ].A = A;
}

/******************************************************************************/
//	���O	�F	�|���S���\�����W�Z�b�g
//	����	�F	�|���S���̕\�����W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	PosX		X���W
//				[IN]double	PosY		Y���W
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetPos( long PolNo, double PosX, double PosY )
{
	 this->SetPosX( PolNo, PosX );
	 this->SetPosY( PolNo, PosY );
}

/******************************************************************************/
//	���O	�F	�|���S���\�����W�Z�b�g
//	����	�F	�|���S���̕\�����W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	PosX		X���W
//				[IN]double	PosY		Y���W
//				[IN]double	PosZ		Z���W
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetPos( long PolNo, double PosX, double PosY, double PosZ )
{
	 this->SetPosX( PolNo, PosX );
	 this->SetPosY( PolNo, PosY );
	 this->SetPosZ( PolNo, PosZ );
}

/******************************************************************************/
//	���O	�F	�|���S���\�����W�Z�b�g
//	����	�F	�|���S���̕\�����W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	PosX		X���W
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetPosX( long PolNo, double PosX )
{
	m_Info[ PolNo ].PosX = PosX;
}

/******************************************************************************/
//	���O	�F	�|���S���\�����W�Z�b�g
//	����	�F	�|���S���̕\�����W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	PosY		Y���W
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetPosY( long PolNo, double PosY )
{
	m_Info[ PolNo ].PosY = PosY;
}

/******************************************************************************/
//	���O	�F	�|���S���\�����W�Z�b�g
//	����	�F	�|���S���̕\�����W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	PosZ		Z���W
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetPosZ( long PolNo, double PosZ )
{
	m_Info[ PolNo ].PosZ = PosZ;
}

/******************************************************************************/
//	���O	�F	���S���W�Z�b�g
//	����	�F	���S���W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetCenter( long PolNo )
{
	long PatNo = m_Info[ PolNo ].PatNo;
	m_Info[ PolNo ].CenterX = (short)( PAT[PatNo].Width / 2 );
	m_Info[ PolNo ].CenterY = (short)( PAT[PatNo].Height / 2 );
}

/******************************************************************************/
//	���O	�F	���S���W�Z�b�g
//	����	�F	���S���W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]short	CenterX		���S���WX
//				[IN]short	CenterY		���S���WY
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetCenter( long PolNo, short CenterX, short CenterY )
{
	m_Info[ PolNo ].CenterX = CenterX;
	m_Info[ PolNo ].CenterY = CenterY;
}

/******************************************************************************/
//	���O	�F	�g�嗦�ݒ�
//	����	�F	�g�嗦��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	Scale		�g�嗦
//	���l	�F	�c�������ꏏ�ɐݒ肷��
/******************************************************************************/
void	CDx9LibPolygon::SetScale( long PolNo, double Scale )
{
	m_Info[ PolNo ].ScaleX = Scale;
	m_Info[ PolNo ].ScaleY = Scale;
}

/******************************************************************************/
//	���O	�F	�T�C�Y�ݒ�
//	����	�F	�T�C�Y��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	Width		���T�C�Y
//				[IN]long	Height		�c�T�C�Y
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetSize( long PolNo, long Width, long Height )
{
	this->SetWidth( PolNo, Width );
	this->SetHeight( PolNo, Height );
}

/******************************************************************************/
//	���O	�F	���T�C�Y�ݒ�
//	����	�F	���T�C�Y��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	Width		���T�C�Y
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetWidth( long PolNo, long Width )
{
	m_Info[ PolNo ].Width = Width;
}

/******************************************************************************/
//	���O	�F	�c�T�C�Y�ݒ�
//	����	�F	�c�T�C�Y��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	Height		�c�T�C�Y
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetHeight( long PolNo, long Height )
{
	m_Info[ PolNo ].Height = Height;
}

/******************************************************************************/
//	���O	�F	�p�x�ݒ�
//	����	�F	�p�x��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	Rot			�p�x
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetRotY( long PolNo, double Rot )
{
	m_Info[ PolNo ].RotY = Rot;
}

/******************************************************************************/
//	���O	�F	�p�x�ݒ�
//	����	�F	�p�x��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	Rot			�p�x
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetRotZ( long PolNo, double Rot )
{
	m_Info[ PolNo ].RotZ = Rot;
}

/******************************************************************************/
//	���O	�F	�p�^�[���ԍ��ݒ�
//	����	�F	�p�^�[���ԍ���ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	PatNo		�p�^�[���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetPatNo( long PolNo, long PatNo )
{
	m_Info[ PolNo ].PatNo = PatNo;
}

/******************************************************************************/
//	���O	�F	�|���S�����擾
//	����	�F	�|���S�������擾����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
ST_PolygonInfo	CDx9LibPolygon::GetInfo( long PolNo )
{
	return m_Info[ PolNo ];
}

/******************************************************************************/
//	���O	�F	�|���S�����ݒ�
//	����	�F	�|���S������ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]POL_TBL	Info		�|���S�����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::SetInfo( long PolNo, ST_PolygonInfo Info )
{
	m_Info[ PolNo ] = Info;
}

/******************************************************************************/
//	���O	�F	�|���S���f�[�^�R�s�[
//	����	�F	�|���S���f�[�^���R�s�[����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�R�s�[��|���S���ԍ�
//				[IN]long	CopyNo		�R�s�[���|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::CopyData( long PolNo, long CopyNo )
{
	m_Info[ PolNo ] = m_Info[ CopyNo ];
}

/******************************************************************************/
//	���O	�F	�|���S���󂫔ԍ��擾
//	����	�F	�|���S���\���̂̒��Ŗ��g�p�̔ԍ����擾����
//	�߂�l	�F	long	��������	-1(�󂫔ԍ��Ȃ�)
//									-1�ȊO(�󂫔ԍ�)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
long	CDx9LibPolygon::GetNo( void )
{
	long Ret = NOT;

	for( int i=0; i<POL_MAX; i++ )
	{
		if( !m_Info[i].Use )
		{
			Ret = i;
			break;
		}
	}

	return Ret;
}

/******************************************************************************/
//	���O	�F	�|���S���\���ؑ�
//	����	�F	�|���S���̕\����؂�ւ���
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]UChar		Flag	�\�����
//	���l	�F	Flag�̐ݒ�l	ON		�\��
//								OFF		��\��
/******************************************************************************/
void	CDx9LibPolygon::ChangeDisp( UChar Flag )
{
	for( long i=0; i<POL_MAX; i++ )
	{
		if( m_Info[ i ].Use == ON )
		{
			m_Info[ i ].Disp = Flag;
		}
	}
}

/******************************************************************************/
//	���O	�F	�|���S���\���ؑ�
//	����	�F	�|���S���̕\����؂�ւ���
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long		PolNo	�|���S���ԍ�
//				[IN]UChar		Flag	�\�����
//	���l	�F	Flag�̐ݒ�l	ON		�\��
//								OFF		��\��
/******************************************************************************/
void	CDx9LibPolygon::ChangeDisp( long PolNo, UChar Flag )
{
	if( m_Info[ PolNo ].Use == ON )
	{
		m_Info[ PolNo ].Disp = Flag;
	}
}

/******************************************************************************/
//	���O	�F	�|���S���\����Ԕ��]
//	����	�F	�|���S���̕\����Ԃ𔽓]����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long		PolNo	�|���S���ԍ�
//	���l	�F	�\������\���@��\�����\��
/******************************************************************************/
void	CDx9LibPolygon::ChangeDispRev( long PolNo )
{
	m_Info[ PolNo ].Disp ^= ON;
}

/******************************************************************************/
//	���O	�F	�\�����W���Z
//	����	�F	�\�����W�����Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	AddX		���Z�lX
//				[IN]double	AddY		���Z�lY
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddPos( long PolNo, double AddX, double AddY )
{
	this->AddPosX( PolNo, AddX );
	this->AddPosY( PolNo, AddY );
}

/******************************************************************************/
//	���O	�F	�\�����W���Z
//	����	�F	�\�����W�����Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	AddX		���Z�lX
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddPosX( long PolNo, double AddX )
{
	m_Info[ PolNo ].PosX += AddX;
}

/******************************************************************************/
//	���O	�F	�\�����W���Z
//	����	�F	�\�����W�����Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	AddY		���Z�lY
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddPosY( long PolNo, double AddY )
{
	m_Info[ PolNo ].PosY += AddY;
}

/******************************************************************************/
//	���O	�F	�p�x���Z
//	����	�F	�p�x�����Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	AddX		���Z�lX
//				[IN]double	AddY		���Z�lY
//				[IN]double	AddZ		���Z�lZ
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddRot( long PolNo, double AddX, double AddY, double AddZ )
{
	m_Info[ PolNo ].RotX += AddX;
	m_Info[ PolNo ].RotY += AddY;
	m_Info[ PolNo ].RotZ += AddZ;
}

/******************************************************************************/
//	���O	�F	�g�嗦���Z
//	����	�F	�g�嗦�����Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	Add			���Z�l
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddScale( long PolNo, double Add )
{
	m_Info[ PolNo ].ScaleX += Add;
	m_Info[ PolNo ].ScaleY += Add;
}

/******************************************************************************/
//	���O	�F	���T�C�Y���Z
//	����	�F	���T�C�Y���Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	Add			���Z�l
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddWidth( long PolNo, long Add )
{
	m_Info[ PolNo ].Width += Add;
}

/******************************************************************************/
//	���O	�F	�c�T�C�Y���Z
//	����	�F	�c�T�C�Y�����Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	Add			���Z�l
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddHeight( long PolNo, long Add )
{
	m_Info[ PolNo ].Height += Add;
}

/******************************************************************************/
//	���O	�F	�F���Z
//	����	�F	�F�����Z����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	AddA		���Z�lA
//				[IN]long	AddR		���Z�lR
//				[IN]long	AddG		���Z�lG
//				[IN]long	AddB		���Z�lB
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::AddColor( long PolNo, UChar AddA, UChar AddR, UChar AddG, UChar AddB )
{
	m_Info[ PolNo ].A += AddA;
	m_Info[ PolNo ].R += AddR;
	m_Info[ PolNo ].G += AddG;
	m_Info[ PolNo ].B += AddB;
}

/******************************************************************************/
//	���O	�F	���W�o�b�N�A�b�v
//	����	�F	���W�̃o�b�N�A�b�v���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::PosBackup( long PolNo )
{
	m_Info[ PolNo ].PosXBk = m_Info[ PolNo ].PosX;
	m_Info[ PolNo ].PosYBk = m_Info[ PolNo ].PosY;
	m_Info[ PolNo ].PosZBk = m_Info[ PolNo ].PosZ;
}

/******************************************************************************/
//	���O	�F	���W���X�g�A
//	����	�F	���W�̃��X�g�A���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::PosRestore( long PolNo )
{
	m_Info[ PolNo ].PosX = m_Info[ PolNo ].PosXBk;
	m_Info[ PolNo ].PosY = m_Info[ PolNo ].PosYBk;
	m_Info[ PolNo ].PosZ = m_Info[ PolNo ].PosZBk;
}

/******************************************************************************/
//	���O	�F	X���W�N���b�s���O
//	����	�F	X���W�̃N���b�s���O���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	MinPos		�ŏ��l
//				[IN]double	MaxPos		�ő�l
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::ClippingPosX( long PolNo, double MinPos, double MaxPos )
{
	if( m_Info[ PolNo ].PosX > MaxPos )
	{
		m_Info[ PolNo ].PosX = MaxPos;
	}
	else if( m_Info[ PolNo ].PosX < MinPos )
	{
		m_Info[ PolNo ].PosX = MinPos;
	}
}

/******************************************************************************/
//	���O	�F	Y���W�N���b�s���O
//	����	�F	Y���W�̃N���b�s���O���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]double	MinPos		�ŏ��l
//				[IN]double	MaxPos		�ő�l
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibPolygon::ClippingPosY( long PolNo, double MinPos, double MaxPos )
{
	if( m_Info[ PolNo ].PosY > MaxPos )
	{
		m_Info[ PolNo ].PosY = MaxPos;
	}
	else if( m_Info[ PolNo ].PosY < MinPos )
	{
		m_Info[ PolNo ].PosY = MinPos;
	}
}
