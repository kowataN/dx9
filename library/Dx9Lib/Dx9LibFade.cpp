/******************************************************************************/
//	DirectX9���C�u����
//		�t�F�[�h
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"



/******************************************************************************/
//	���O	�F	�t�F�[�h������
//	����	�F	�t�F�[�h�̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	���l
/******************************************************************************/
void	CDx9LibFade::Init( void )
{
	m_Fade.Use = OFF;
	m_Fade.Disp = OFF;
	m_Fade.Init = OFF;
	m_Fade.State = NOT;

	m_Fade.Time = 0;
	m_Fade.Count = 0;

	m_Fade.A = m_Fade.R = m_Fade.G = m_Fade.B = 0.0;
	m_Fade.StartA = m_Fade.StartR = m_Fade.StartG = m_Fade.StartB = 0.0;
	m_Fade.TargetA = m_Fade.TargetR = m_Fade.TargetG = m_Fade.TargetB = 0.0;
}

/******************************************************************************/
//	���O	�F	�t�F�[�h�Z�b�g
//	����	�F	�t�F�[�h�̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	Time		���v����
//				[IN]double	a1			�J�n�F�A���t�@
//				[IN]double	r1			�J�n�F��
//				[IN]double	g1			�J�n�F��
//				[IN]double	b1			�J�n�F��
//				[IN]double	a2			�ڕW�F�A���t�@
//				[IN]double	r2			�ڕW�F��
//				[IN]double	g2			�ڕW�F��
//				[IN]double	b2			�ڕW�F��
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibFade::Set(
						long Time,
						double a1, double r1, double g1, double b1,
						double a2, double r2, double g2, double b2 )
{
	m_Fade.Use = ON;
	m_Fade.Disp = ON;
	m_Fade.Init = ON;
	m_Fade.State = ON;

	m_Fade.Time = Time;
	m_Fade.Count = 0;

	m_Fade.StartA = m_Fade.A = a1;
	m_Fade.StartR = m_Fade.R = r1;
	m_Fade.StartG = m_Fade.G = g1;
	m_Fade.StartB = m_Fade.B = b1;

	m_Fade.TargetA = a2;
	m_Fade.TargetR = r2;
	m_Fade.TargetG = g2;
	m_Fade.TargetB = b2;
}

/******************************************************************************/
//	���O	�F	�t�F�[�h�`��
//	����	�F	�t�F�[�h�̕`����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibFade::Draw( void )
{
	ST_VertexInfo	fade[4];
	fade[0].x = 0;
	fade[0].y = 0;
	fade[1].x = 1024;
	fade[1].y = 0;
	fade[2].x = 1024;
	fade[2].y = 512;
	fade[3].x = 0;
	fade[3].y = 512;

	for( int i=0; i<4; i++ )
	{
		fade[i].z = 1.0;
		fade[i].rhw = 1.0;
		fade[i].color = D3DCOLOR_ARGB(
			(short)m_Fade.A, (short)m_Fade.R, (short)m_Fade.G, (short)m_Fade.B );
	}

	fade[0].tu = 0.0;
	fade[0].tv = 0.0;
	fade[1].tu = 1.0;
	fade[1].tv = 0.0;
	fade[2].tu = 1.0;
	fade[2].tv = 1.0;
	fade[3].tu = 0.0;
	fade[3].tv = 1.0;

	// �`��J�n
	g_lpD3DDev->BeginScene();

	//	���_�t�H�[�}�b�g�̐ݒ�
	g_lpD3DDev->SetFVF( FVF_TLVERTEX );

	g_lpD3DDev->SetTexture( 0, NULL );

	g_lpD3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, fade, sizeof( ST_VertexInfo ) );

	// �`��I��
	g_lpD3DDev->EndScene();
}

/******************************************************************************/
//	���O	�F	�t�F�[�h���[�v
//	����	�F	���t�F�[�h�������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibFade::Loop( void )
{
	if( !m_Fade.Use )
	{
		return;
	}

	if( m_Fade.State == ON )
	{
		m_Fade.Count ++;
		if( m_Fade.Count >= m_Fade.Time )
		{
			m_Fade.Count = m_Fade.Time;
			m_Fade.State = OFF;
		}
	}

	if( m_Fade.Count == m_Fade.Time )
	{
		/////////////////////////////////////
		//	�t�F�[�h�I��
		m_Fade.A = m_Fade.TargetA;
		m_Fade.R = m_Fade.TargetR;
		m_Fade.G = m_Fade.TargetG;
		m_Fade.B = m_Fade.TargetB;
	}
	else
	{
		/////////////////////////////////////
		//	�t�F�[�h��
		m_Fade.A += ( m_Fade.TargetA - m_Fade.StartA ) / m_Fade.Time;
		m_Fade.R += ( m_Fade.TargetR - m_Fade.StartR ) / m_Fade.Time;
		m_Fade.G += ( m_Fade.TargetG - m_Fade.StartG ) / m_Fade.Time;
		m_Fade.B += ( m_Fade.TargetB - m_Fade.StartB ) / m_Fade.Time;
	}

	//	�F�N���b�s���O
	MaxMin( m_Fade.A, 255, 0 );
	MaxMin( m_Fade.R, 255, 0 );
	MaxMin( m_Fade.G, 255, 0 );
	MaxMin( m_Fade.B, 255, 0 );

	this->Draw();
}

/******************************************************************************/
//	���O	�F	�u���b�N�A�E�g
//	����	�F	�u���b�N�A�E�g���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	Time	�t���[����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibFade::BlackOut( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 0,0,0,0, 255,0,0,0 );
}

/******************************************************************************/
//	���O	�F	�u���b�N�C��
//	����	�F	�u���b�N�C�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	Time	�t���[����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibFade::BlackIn( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 255,0,0,0, 0,0,0,0 );
}

/******************************************************************************/
//	���O	�F	�z���C�g�A�E�g
//	����	�F	�z���C�g�A�E�g���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	Time	�t���[����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibFade::WhiteOut( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 0,255,255,255, 255,255,255,255 );
}

/******************************************************************************/
//	���O	�F	�z���C�g�C��
//	����	�F	�z���C�g�C�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	Time	�t���[����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibFade::WhiteIn( long Time )
{
	//if( m_Fade.State == ON || m_Fade.Init == ON )
	//{
	//	return;
	//}
	this->Set( Time, 255,255,255,255, 0,255,255,255 );
}

/******************************************************************************/
//	���O	�F	��Ԏ擾
//	����	�F	��Ԃ��擾����
//	�߂�l	�F	short	���	0(�I��)
//								1(�t�F�[�h��)
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
short	CDx9LibFade::GetState( void )
{
	return m_Fade.State;
}

/******************************************************************************/
//	���O	�F	�i���󋵎擾
//	����	�F	�i���󋵂��擾����
//	�߂�l	�F	long	�i����
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
long	CDx9LibFade::GetProgress( void )
{
	double Ret = (double)m_Fade.Count / (double)m_Fade.Time * 100;
	return	(long)Ret;
}
