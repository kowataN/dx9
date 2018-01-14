/******************************************************************************/
//	DirectX9���C�u����
//		�e�L�X�g
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
ST_TextInfo			TXT[TXT_MAX];



/******************************************************************************/
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibText::CDx9LibText()
{
	m_lpFont = NULL;
	m_lpTxtSprite = NULL;
}

/******************************************************************************/
//	���O	�F	�f�X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibText::~CDx9LibText()
{
	this->Release();
}

/******************************************************************************/
//	���O	�F	�e�L�X�g�f�o�C�X������
//	����	�F	�f�o�C�X�̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::InitDevice( void )
{
	//	�t�H���g�̐ݒ�
	D3DXFONT_DESC	FontDesc;
	FontDesc.Height = FONT_SIZE;
	FontDesc.Width = 0;
	FontDesc.Weight = FW_NORMAL;
	FontDesc.MipLevels = 0;
	FontDesc.Italic = FALSE;
	FontDesc.CharSet = DEFAULT_CHARSET;
	FontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	FontDesc.Quality = DEFAULT_QUALITY;
	FontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s( FontDesc.FaceName, LF_FACESIZE, "�l�r �S�V�b�N" );

	// �t�H���g�f�o�C�X�̍쐬
	D3DXCreateFontIndirect(
		g_lpD3DDev,
		&FontDesc,
		&m_lpFont );

	//	�X�v���C�g�쐬
	D3DXCreateSprite( g_lpD3DDev, &m_lpTxtSprite );
}

/******************************************************************************/
//	���O	�F	�e�L�X�g�S������
//	����	�F	�e�L�X�g�\���̂̑S���������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::InitAll( void )
{
	for( long i=0; i<TXT_MAX; i++ )
	{
		this->Init( i );
	}
}

/******************************************************************************/
//	���O	�F	�e�L�X�g������
//	����	�F	�e�L�X�g�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::Init( long TextNo )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	//	�t���O
	TXT[ TextNo ].Disp = OFF;
	TXT[ TextNo ].Use = OFF;

	//	���W�n
	TXT[ TextNo ].PosX = 0;
	TXT[ TextNo ].PosY = 0;
	TXT[ TextNo ].CenterX = 0;
	TXT[ TextNo ].CenterY = 0;

	//	�\���T�C�Y
	TXT[ TextNo ].Width = g_WindowWidth;
	TXT[ TextNo ].Height = g_WindowHeight;

	//	�e�F�����̏�����
	TXT[ TextNo ].A = 255;
	TXT[ TextNo ].R = 255;
	TXT[ TextNo ].G = 255;
	TXT[ TextNo ].B = 255;

	TXT[ TextNo ].Time = -2;
	TXT[ TextNo ].Atr = ATR_NONE;
	TXT[ TextNo ].Speed = 10;
	TXT[ TextNo ].Count = 0;
	TXT[ TextNo ].Step = -1;
	TXT[ TextNo ].State = 0;

	//	�i�[�p������
	ZeroMemory( &TXT[ TextNo ].Str, sizeof(TXT[ TextNo ].Str) );
}

/******************************************************************************/
//	���O	�F	�e�L�X�g�S�\��
//	����	�F	�e�L�X�g�̕\�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::DispAll( void )
{
	m_lpTxtSprite->Begin(NULL);

	for( long i=0; i<TXT_MAX; i++ )
	{
		if( TXT[i].Use == OFF || TXT[i].Disp == OFF )
		{
			continue;
		}

		g_LibText.Disp( i );

		if( TXT[i].Time > -2 )
		{
			TXT[i].Time --;
		}
		if( TXT[i].Time == 0 )
		{
			TXT[i].Time = 0;
			TXT[i].Disp = OFF;
		}
	}

	m_lpTxtSprite->End();
}

/******************************************************************************/
//	���O	�F	�e�L�X�g�Z�b�g
//	����	�F	�e�L�X�g���Z�b�g����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//				[IN]double	PosX		�\�����WX
//				[IN]double	PosY		�\�����WY
//				[IN]UChar	A			�A���t�@
//				[IN]UChar	R			�Ԑ���
//				[IN]UChar	G			�ΐ���
//				[IN]UChar	B			����
//				[IN]char	*str		�o�^���镶����
//				[IN]UChar	Atr			�A�g���r���[�g
//				[IN]long	Speed		�\�����x
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::Set(long TextNo,
						 double PosX, double PosY,
						 UChar A, UChar R, UChar G, UChar B,
						 char *Str,
						 UChar Atr,
						 long Speed )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].Use= ON;
	TXT[ TextNo ].Disp = ON;

	strcpy_s( TXT[ TextNo ].Str, MAX_PATH, Str );

	TXT[ TextNo ].PosX = PosX;
	TXT[ TextNo ].PosY = PosY;

	TXT[ TextNo ].A = A;
	TXT[ TextNo ].R = R;
	TXT[ TextNo ].G = G;
	TXT[ TextNo ].B = B;

	TXT[ TextNo ].Time = -2;
	TXT[ TextNo ].Atr = Atr;
	TXT[ TextNo ].Speed = Speed;
	TXT[ TextNo ].Count = 0;
	if( Atr == ATR_ONCE )
	{
		TXT[ TextNo ].Step = 0;
	}
}

/******************************************************************************/
//	���O	�F	�\���ύX
//	����	�F	�e�L�X�g�̕\����ύX����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]UChar	Flag	�\���t���O
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::ChangeDisp( UChar Flag )
{
	for( long i=0; i<TXT_MAX; i++ )
	{
		if( TXT[ i ].Use == ON )
		{
			TXT[ i ].Disp = Flag;
		}
	}
}

void	CDx9LibText::ChangeDisp( long TextNo, UChar Flag )
{
	TXT[ TextNo ].Disp = Flag;
}

/******************************************************************************/
//	���O	�F	�J���[�Z�b�g
//	����	�F	�e�L�X�g�̐F�ύX
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//				[IN]UChar	A			�A���t�@
//				[IN]UChar	R			�Ԑ���
//				[IN]UChar	G			�ΐ���
//				[IN]UChar	B			����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::SetColor( long TextNo, UChar A, UChar R, UChar G, UChar B )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].A = A;
	TXT[ TextNo ].R = R;
	TXT[ TextNo ].G = G;
	TXT[ TextNo ].B = B;
}

/******************************************************************************/
//	���O	�F	�\�����W�ݒ�
//	����	�F	�\�����W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//				[IN]double	PosX		�\�����WX
//				[IN]double	PosY		�\�����WY
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::SetPos( long TextNo, double PosX, double PosY )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].PosX = PosX;
	TXT[ TextNo ].PosY = PosY;
}

/******************************************************************************/
//	���O	�F	���S���W�ݒ�
//	����	�F	���S���W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::SetCenter( long TextNo )
{
	char	*pt;
	UChar	uc;
	long	len,max;

	if( TextNo >= TXT_MAX )
	{
		return;
	}

	//	����
	len = (long)strlen( TXT[ TextNo ].Str );
	pt = (char*)TXT[ TextNo ].Str;
	max = 0;

	for( int i=0; i<len; i++ )
	{
		uc = *pt;
		if( (uc<128) || (uc>=160) )
		{
			//	���p
			max ++;
			pt++;
		}
		else
		{
			//	�S�p
			max += 2;
			pt += 2;
		}
	}

	TXT[ TextNo ].CenterX = ( len * FONT_SIZE ) / 4;
	TXT[ TextNo ].CenterY = FONT_SIZE / 2;
}

/******************************************************************************/
//	���O	�F	�\���̈�ݒ�
//	����	�F	�\���̈��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//				[IN]short	Witdh		���T�C�Y
//				[IN]short	Height		�c�T�C�Y
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::SetRect( long TextNo, long Width, long Height )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].Width = Width;
	TXT[ TextNo ].Height = Height;
}

/******************************************************************************/
//	���O	�F	�f�[�^�R�s�[
//	����	�F	�f�[�^���R�s�[����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//				[IN]long	CopyNo		�R�s�[���e�L�X�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::CopyData( long TextNo, long CopyNo )
{
	if( TextNo >= TXT_MAX || CopyNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ] = TXT[ CopyNo ];
}

/******************************************************************************/
//	���O	�F	�^�C�}�Z�b�g
//	����	�F	�^�C�}��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//				[IN]long	Time		�\���t���[����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::SetTimer( long TextNo, long Time )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].Time = Time;
}

/******************************************************************************/
//	���O	�F	�I�u�W�F�N�g���
//	����	�F	�I�u�W�F�N�g�̉�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::Release()
{
	SAFE_RELEASE(m_lpTxtSprite);
	SAFE_RELEASE(m_lpFont);
}

/******************************************************************************/
//	���O	�F	�e�L�X�g�\��
//	����	�F	�e�L�X�g�̕\�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextNo		�e�L�X�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CDx9LibText::Disp( long TextNo )
{
	long	len,format;
	RECT	rect;

	//	�f�t�H���g�F�ݒ�
	D3DCOLOR color
		= D3DCOLOR_ARGB(TXT[TextNo].A,TXT[TextNo].R,TXT[TextNo].G,TXT[TextNo].B);

	len = (long)strlen( TXT[TextNo].Str );
	format = DT_WORDBREAK;
	rect.left = (long)( TXT[TextNo ].PosX - (TXT[TextNo].CenterX) ); 
	rect.right = TXT[TextNo].Width;
	rect.top = (long)( TXT[TextNo ].PosY - (TXT[TextNo].CenterY) );
	rect.bottom = TXT[TextNo].Height;

	// �N���b�s���O
	if( (rect.right < 1) || (rect.left >= TXT[TextNo].Width)
	|| (rect.bottom < 1) || (rect.top >= TXT[TextNo].Height) )
	{
		return;
	}

	// ���[�␳
	if( rect.left < 0 )
	{
		rect.left = 0;
		format |= DT_RIGHT;	// �E����
	}

	// ��[�␳
	if( rect.top < 0 )
	{
		rect.top = 0;
		format |= DT_BOTTOM | DT_SINGLELINE;	// �������A�P�s
	}

	//	�����񂩂�h[#�h����������
	char	*pTok = strstr( TXT[TextNo].Str, "[#" );
	if( pTok == NULL )
	{
		if( TXT[TextNo].Atr == ATR_ONCE )
		{
			long	Length = (long)strlen(TXT[TextNo].Str);
			if( Length > TXT[TextNo].Step && !TXT[TextNo].State )
			{
				TXT[TextNo].Count ++;
				if( TXT[TextNo].Count > TXT[TextNo].Speed )
				{
					TXT[TextNo].Count = 0;
					TXT[TextNo].Step ++;
					if( TXT[TextNo].Str[TXT[TextNo].Step] < 128 || TXT[TextNo].Str[TXT[TextNo].Step]>= 160 )
					{
						//	���p�Ȃ̂ŉ������Ȃ�
					}
					else
					{
						TXT[TextNo].Step ++;
					}
				}
			}
			else
			{
				TXT[TextNo].State = ON;
			}
		}
		else
		{
			TXT[TextNo].State = ON;
		}

		// �f�o�C�X��ɁA���`���� ANSI �e�L�X�g��`�悷��
		m_lpFont->DrawText(NULL, TXT[TextNo].Str,
			//-1,
			TXT[TextNo].Step,
			&rect, format, color );
		//m_lpFont->DrawText(lpTxtSprite, TXT[No].Str, -1, &rect, format, color );
	}
	else
	{
		string	TargetStr = TXT[TextNo].Str;
		//	�����񕪊�
		vector<string>	SplitStr = SplitText(TargetStr, "[");
		vector<string>::iterator	Ite;
		long Pos = (long)TXT[TextNo].PosX;

		for( Ite=SplitStr.begin(); Ite!=SplitStr.end(); Ite++ )
		{
			//	�ēx�����񕪊�
			vector<string>	SplitStr2 = SplitText( *(Ite), "]" );
			vector<string>::iterator	Ite2;
			string	DispStr;
			for( Ite2=SplitStr2.begin(); Ite2!=SplitStr2.end(); Ite2++ )
			{
				if( "#RED" == *(Ite2) )
				{
					color = COLOR_RED;
				}
				else if( "#GREEN" == *(Ite2) )
				{
					color = COLOR_GREEN;
				}
				else if( "#BLUE" == *(Ite2) )
				{
					color = COLOR_BLUE;
				}
				else if( "#DEF" == *(Ite2) )
				{
					color
						= D3DCOLOR_ARGB( TXT[TextNo].A, TXT[TextNo].R, TXT[TextNo].G, TXT[TextNo].B );
				}
				else if( Ite2->find_first_of("#") != Ite2->npos )
				{
					//	�u#00000000�v�`�u#FFFFFFFF�v�𔭌�
					color = ToDec( Ite2->c_str() );
				}
				else
				{
					DispStr = *(Ite2);
				}
			}

			m_lpFont->DrawText(m_lpTxtSprite, DispStr.c_str(), -1, &rect, format, color );
			//m_lpFont->DrawText(NULL, DispStr.c_str(), -1, &rect, format, color );
			rect.left += (long)DispStr.length() * FONT_SIZE /2;
		}

		TXT[TextNo].State = 1;
	}
}

