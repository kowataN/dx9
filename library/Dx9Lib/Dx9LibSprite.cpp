/******************************************************************************/
//	DirectX9���C�u����
//		�X�v���C�g
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
ST_SpriteInfo	SP[SP_MAX];	//	�X�v���C�g
LPD3DXSPRITE	lpSprite;	//	�X�v���C�g�I�u�W�F�N�g



/******************************************************************************/
//	���O	�F	�f�o�C�X�쐬
//	����	�F	�f�o�C�X�̍쐬���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CreateSpriteDevice( void )
{
	//	�X�v���C�g�쐬
	D3DXCreateSprite( g_lpD3DDev, &lpSprite );
}

/******************************************************************************/
//	���O	�F	�X�v���C�g������
//	����	�F	�X�v���C�g�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	SpriteNo	�X�v���C�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitSprite( long SpriteNo )
{
	if( SpriteNo >= SP_MAX )
	{
		return;
	}

	//	�t���OOFF
	SP[ SpriteNo ].Use		= OFF;
	SP[ SpriteNo ].Disp		= OFF;

	//	�p�^�[���ԍ�
	SP[ SpriteNo ].PatNo	= 0;

	//	�A�g���r���[�g������
	SP[ SpriteNo ].Atr		= ATR_NONE;

	//	�\�����W
	SP[ SpriteNo ].PosX		= 0.0;
	SP[ SpriteNo ].PosY		= 0.0;
	SP[ SpriteNo ].PosZ		= 1.0;

	//	�ړ���
	SP[ SpriteNo ].MoveX	= 0;
	SP[ SpriteNo ].MoveY	= 0;

	//	�o�b�N�A�b�v���W
	SP[ SpriteNo ].PosXBk	= 0;
	SP[ SpriteNo ].PosYBk	= 0;

	//	���S���W
	SP[ SpriteNo ].CenterX	= 0;
	SP[ SpriteNo ].CenterY	= 0;

	//	�X�P�[�����O
	SP[ SpriteNo ].ScaleX	= 1;
	SP[ SpriteNo ].ScaleY	= 1;

	//	�T�C�Y
	SP[ SpriteNo ].Width	= 0;
	SP[ SpriteNo ].Height	= 0;

	//	�e�F����
	SP[ SpriteNo ].A		= 255;
	SP[ SpriteNo ].R		= 255;
	SP[ SpriteNo ].G		= 255;
	SP[ SpriteNo ].B		= 255;
}

/******************************************************************************/
//	���O	�F	�X�v���C�g�S������
//	����	�F	�X�v���C�g�\���̂̑S���������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitSpriteAll( void )
{
	for( long i=0; i<SP_MAX; i++ )
	{
		InitSprite( i );
	}
}

/******************************************************************************/
//	���O	�F	�X�v���C�g�Z�b�g
//	����	�F	�X�v���C�g���Z�b�g����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	SpriteNo	�X�v���C�g�ԍ�
//				[IN]long	PatNo		�p�^�[���ԍ�
//				[IN]double	PosX		�\�����WX
//				[IN]double	PosY		�\�����WY
//				[IN]double	PosZ		�\�����WZ
//				[IN]UShort	Atr			�A�g���r���[�g
//	���l	�F	Atr�̐ݒ�l	ATR_NONE	�����Ȃ�
//							ATR_REV_W	���E���]
//							ATR_REV_H	�㉺���]
//							ATR_ALL		�㉺���E���]
/******************************************************************************/
void	SetSprite(long SpriteNo, long PatNo,
				  double PosX, double PosY, double PosZ,
				  UShort Atr )
{
	if( SpriteNo >= SP_MAX )
	{
		return;
	}

	/////////////////////////////////////
	//	�e�����o�̏�����
	SP[ SpriteNo ].Use	= ON;		//	�g�p�t���O�n�m
	SP[ SpriteNo ].Disp	= ON;		//	�\���t���O�n�m

	SP[ SpriteNo ].PatNo = PatNo;	//	�p�^�[���ԍ�

	//	�X�v���C�g�̃T�C�Y
	SP[ SpriteNo ].Width	= PAT[ PatNo ].Width;	//	��
	SP[ SpriteNo ].Height	= PAT[ PatNo ].Height;	//	�c

	//	�\�����W
	SP[ SpriteNo ].PosX	= PosX;
	SP[ SpriteNo ].PosY	= PosY;
	SP[ SpriteNo ].PosZ = PosZ;

	//	�A�g���r���[�g
	SP[ SpriteNo ].Atr	= Atr;

	//	��]�p�x
	SP[ SpriteNo ].Rot	= 0.0f;
}

/******************************************************************************/
//	���O	�F	���S���W�Z�b�g
//	����	�F	���S���W��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	SpriteNo	�X�v���C�g�ԍ�
//				[IN]long	CenterX		���S���WX
//				[IN]long	CenterY		���S���WY
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetSpriteCenter( long SpriteNo, long CenterX, long CenterY )
{
	//	���S���W
	SP[ SpriteNo ].CenterX	= CenterX;
	SP[ SpriteNo ].CenterY	= CenterY;
}

/******************************************************************************/
//	���O	�F	���_�F�ݒ�
//	����	�F	�f�t���[�Y�F��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	SpriteNo	�X�v���C�g�ԍ�
//				[IN]UChar	A			�A���t�@
//				[IN]UChar	R			�Ԑ���
//				[IN]UChar	G			�ΐ���
//				[IN]UChar	B			����
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetSpriteColor( long SpriteNo, UChar A, UChar R, UChar G, UChar B )
{
	SP[ SpriteNo ].A = A;
	SP[ SpriteNo ].R = R;
	SP[ SpriteNo ].G = G;
	SP[ SpriteNo ].B = B;
}

/******************************************************************************/
//	���O	�F	�g�嗦�ݒ�
//	����	�F	�g�嗦��ݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	SpriteNo	�X�v���C�g�ԍ�
//				[IN]double	Scale		�g�嗦
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetSpriteScale( long SpriteNo, double Scale )
{
	SP[ SpriteNo ].ScaleX = Scale;
	SP[ SpriteNo ].ScaleY = Scale;
}

/******************************************************************************/
//	���O	�F	�\�����W�ݒ�
//	����	�F	�\�����W�̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	SpriteNo	�X�v���C�g�ԍ�
//				[IN]double	PosX		�\�����WX
//				[IN]double	PosY		�\�����WY
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetSpritePos( long SpriteNo, double PosX, double PosY )
{
	SP[ SpriteNo ].PosX = PosX;
	SP[ SpriteNo ].PosY = PosY;
}

/******************************************************************************/
//	���O	�F	�󂫃X�v���C�g�ԍ��擾
//	����	�F	�󂫃X�v���C�g�ԍ����擾����
//	�߂�l	�F	long		�󂫔ԍ�
//							-1�ŋ󂫔ԍ��Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
long	GetSpriteNo( void )
{
	for( long i=0; i<SP_MAX; i++ )
	{
		if( !SP[i].Use )
		{
			return	i;
		}
	}

	return NOT;
}

/******************************************************************************/
//	���O	�F	�X�v���C�g�`��
//	����	�F	�X�v���C�g�̕`����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	SpriteNo	�X�v���C�g�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	DispSprite( long SpriteNo )
{
	if( !SP[SpriteNo].Use && !SP[SpriteNo].Disp )
	{
		return;
	}

	float ScaleX = 1.0;
	float ScaleY = 1.0;
	UShort m = SP[SpriteNo].Atr & ( ATR_NONE | ATR_REV_W | ATR_REV_H | ATR_ALL );
	if( m )
	{
		switch( m )
		{
			//	���E���]
			case	ATR_REV_W:
				ScaleX *= -1;
				break;

			//	�㉺���]
			case	ATR_REV_H:
				ScaleY *= -1;
				break;

			//	�㉺���E���]
			case	ATR_ALL:
				ScaleX *= -1;
				ScaleY *= -1;
				break;
		}
	}

	D3DXMATRIX	matWorld,matRot,matScale,matTrans;
	D3DXMatrixIdentity(&matWorld);

	//	�p�x�␳
	CorrectRot( SP[SpriteNo].Rot );

	//	Z��]
	D3DXMatrixRotationZ( &matRot, (float)DegToRad( SP[SpriteNo].Rot ) );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matRot );
	//	�g��
	D3DXMatrixScaling( &matScale,
		(float)SP[SpriteNo].ScaleX*ScaleX,
		(float)SP[SpriteNo].ScaleY*ScaleY,
		0.0f );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matScale );
	//	�ړ�
	D3DXMatrixTranslation( &matTrans,
		(float)SP[SpriteNo].PosX,
		(float)SP[SpriteNo].PosY,
		(float)SP[SpriteNo].PosZ );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );

	lpSprite->SetTransform( &matWorld );

	RECT	rect;
	long PatNo = SP[SpriteNo].PatNo;
	SetRect( &rect,
		PAT[ PatNo ].PosX,
		PAT[ PatNo ].PosY,
		PAT[ PatNo ].PosX + PAT[ PatNo ].Width,
		PAT[ PatNo ].PosY + PAT[ PatNo ].Height
		);

	lpSprite->Draw(
		TEX[ PAT[ PatNo ].TextureNo ].lpTex,
		&rect,
		&D3DXVECTOR3( (float)SP[SpriteNo].CenterX, (float)SP[SpriteNo].CenterY, 0 ),
		NULL,
		D3DCOLOR_ARGB( SP[SpriteNo].A, SP[SpriteNo].R, SP[SpriteNo].G, SP[SpriteNo].B )
		);
}

/******************************************************************************/
//	���O	�F	�X�v���C�g�S�`��
//	����	�F	�X�v���C�g�̕`����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	DispSpriteAll( void )
{
	long	i;
	long	Max = 0;
	long	DispNo[SP_MAX];

	//	�`�搔����
	for( i=0; i<SP_MAX; i++ )
	{
		if( SP[i].Use && SP[i].Disp )
		{
			//	�`��X�v���C�g�ԍ��i�[
			DispNo[Max] = i;
			Max ++;
		}
	}

	//	Z�\�[�g
	for( i=Max-1; i>=1; i-- )
	{
		for( int j=i-1; j>=0; j-- )
		{
			//	���ƑO���r
			if( SP[ DispNo[i] ].PosZ < SP[ DispNo[j] ].PosZ )
			{
				long BakNo = DispNo[i];
				DispNo[i] = DispNo[j];
				DispNo[j] = BakNo;
			}
		}
	}

	//	�`��J�n
	if( !lpSprite )
	{
		lpSprite->Begin(NULL);

		for( int i=0; i<Max; i++ )
		{
			DispSprite( DispNo[i] );
 		}

		//	�`��I��
		lpSprite->End();
	}
}

/******************************************************************************/
//	���O	�F	�X�v���C�g�\���ύX
//	����	�F	�X�v���C�g�̕\����ύX����
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]UChar	Flag	�\���t���O
//	���l	�F	�Ȃ�
/******************************************************************************/
void	DispChangeSprite( UChar Flag )
{
	for( int i=0; i<SP_MAX; i++ )
	{
		if( SP[i].Use == ON )
		{
			SP[i].Disp = Flag;
		}
	}
}

/******************************************************************************/
//	���O	�F	�I�u�W�F�N�g���
//	����	�F	�I�u�W�F�N�g�̉�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	ReleaseSprite( void )
{
	SAFE_RELEASE(lpSprite);
}
