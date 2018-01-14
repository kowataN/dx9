/******************************************************************************/
//	DirectX9���C�u����
//		���C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
ST_LineInfo	LINE[LINE_MAX];



/******************************************************************************/
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibLine::CDx9LibLine()
{
	m_lpLine = NULL;
}

/******************************************************************************/
//	���O	�F	�f�X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibLine::~CDx9LibLine()
{
	this->ReleaseDevice();
}

/******************************************************************************/
//	���O	�F	���C���p�f�o�C�X�쐬
//	����	�F	���C���p�f�o�C�X�̍쐬���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::CreateDevice( void )
{
	D3DXCreateLine( g_lpD3DDev, &m_lpLine );
}

/******************************************************************************/
//	���O	�F	���C���p�\���̏�����
//	����	�F	���C���p�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	No		���C���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::Init( long No )
{
	if( No >= LINE_MAX )
	{
		return;
	}

	//	�t���OOFF
	LINE[ No ].Use		= OFF;
	LINE[ No ].Disp		= OFF;

	//	�n�_
	LINE[ No ].StartX	= 0.0;
	LINE[ No ].StartY	= 0.0;

	//	�I�_
	LINE[ No ].EndX		= 0.0;
	LINE[ No ].EndY		= 0.0;

	//	�A�g���r���[�g
	LINE[ No ].Atr		= ATR_NONE;
}

/******************************************************************************/
//	���O	�F	���C���p�\���̑S������
//	����	�F	���C���p�\���̂̑S���������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::InitAll( void )
{
	for( long i=0; i<LINE_MAX; i++ )
	{
		Init( i );
	}
}

/******************************************************************************/
//	���O	�F	���C���Z�b�g
//	����	�F	���C������ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	No		���C���ԍ�
//				[IN]double	StartX	�J�n���WX
//				[IN]double	StartY	�J�n���WY
//				[IN]double	EndX	�I�����WX
//				[IN]double	EndY	�I�����WY
//				[IN]UShort	Atr		�A�g���r���[�g
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::Set(
						 long No,
						 double StartX, double StartY,
						 double EndX, double EndY,
						 UShort Atr )
{
	if( No >= LINE_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	�e�����o�̏�����
	LINE[ No ].Use	= ON;		//	�g�p�t���O�n�m
	LINE[ No ].Disp	= ON;		//	�\���t���O�n�m

	//	�\�����W
	LINE[ No ].StartX	= StartX;
	LINE[ No ].StartY	= StartY;

	LINE[ No ].EndX = EndX;
	LINE[ No ].EndY = EndY;

	LINE[No].A = 255;
	LINE[No].R = 255;
	LINE[No].G = 255;
	LINE[No].B = 255;

	LINE[No].Atr = Atr;
}

/******************************************************************************/
//	���O	�F	�F�ݒ�
//	����	�F	�f�t���[�Y�F��ύX����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	No		���C���ԍ�
//				[IN]UChar	A		�A���t�@
//				[IN]UChar	R		�Ԑ���
//				[IN]UChar	G		�ΐ���
//				[IN]UChar	B		����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::SetColor( long No, UChar A, UChar R, UChar G, UChar B )
{
	LINE[No].A = A;
	LINE[No].R = R;
	LINE[No].G = G;
	LINE[No].B = B;
}

/******************************************************************************/
//	���O	�F	���C���󂫔ԍ��擾
//	����	�F	���C���\���̂̒��Ŗ��g�p�̔ԍ����擾����
//	�߂�l	�F	long	��������	-1(�󂫔ԍ��Ȃ�)
//									-1�ȊO(�󂫔ԍ�)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
short	CDx9LibLine::GetNo( void )
{
	for( int i=0; i<LINE_MAX; i++ )
	{
		if( !LINE[i].Use )
		{
			return	i;
		}
	}

	return NOT;
}

/******************************************************************************/
//	���O	�F	���C���`��
//	����	�F	���C����`�悷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	No		���C���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::DrawLine( long No )
{
	D3DXVECTOR2	vec[2];
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].EndY );

	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );
}

/******************************************************************************/
//	���O	�F	��`�`��
//	����	�F	��`��`�悷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	No		���C���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::DrawBox( long No )
{
	D3DXVECTOR2	vec[2];

	//	��
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].StartY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );

	//	��
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].EndY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].EndY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );

	//	��
	vec[0] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].StartX, (float)LINE[No].EndY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );

	//	�E
	vec[0] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].StartY );
	vec[1] = D3DXVECTOR2( (float)LINE[No].EndX, (float)LINE[No].EndY);
	m_lpLine->Draw( vec,2,
		D3DCOLOR_ARGB( LINE[No].A, LINE[No].R, LINE[No].G, LINE[No].B ) );
}

/******************************************************************************/
//	���O	�F	���C���S�`��
//	����	�F	���C���S�ĕ`�悷��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::DispAll( void )
{
	//	�`��J�n
	m_lpLine->Begin();

	for( int i=0; i<LINE_MAX; i++ )
	{
		if( !LINE[i].Use && !LINE[i].Disp )
		{
			continue;
		}

		if( LINE[i].Atr == ATR_NONE )
		{
			this->DrawLine( i );
		}
		else
		{
			this->DrawBox( i );
		}
	}

	//	�`��I��
	m_lpLine->End();
}

/******************************************************************************/
//	���O	�F	���C���\���ύX
//	����	�F	���C���̕\����؂�ւ���
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]UChar		Flag	�\�����
//	���l	�F	Flag�̐ݒ�l	ON		�\��
//								OFF		��\��
/******************************************************************************/
void	CDx9LibLine::DispChange( UChar Flag )
{
	for( int i=0; i<LINE_MAX; i++ )
	{
		if( LINE[i].Use )
		{
			LINE[i].Disp = Flag;
		}
	}
}

/******************************************************************************/
//	���O	�F	���C���I�u�W�F�N�g�J��
//	����	�F	���C���I�u�W�F�N�g���J������
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibLine::ReleaseDevice( void )
{
	SAFE_RELEASE(m_lpLine);
}
