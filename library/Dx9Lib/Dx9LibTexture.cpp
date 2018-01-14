/******************************************************************************/
//	DirectX9���C�u����
//		�e�N�X�`��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
ST_TextureInfo		TEX[TEX_MAX];	//	�e�N�X�`��



/******************************************************************************/
//	���O	�F	�e�N�X�`��������
//	����	�F	�e�N�X�`���\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TexNo		�e�N�X�`���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitTexture( long TexNo )
{
	if( TexNo >= TEX_MAX )
	{
		return;
	}

	TEX[ TexNo ].Use = OFF;

	TEX[ TexNo ].Width = 0;
	TEX[ TexNo ].Height = 0;

	TEX[ TexNo ].CenterX = 0;
	TEX[ TexNo ].CenterY = 0;

	TEX[ TexNo ].lpTex = NULL;
}

/******************************************************************************/
//	���O	�F	�e�N�X�`���S������
//	����	�F	�e�N�X�`���\���̂̑S���������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitTextureAll( void )
{
	for( long i=0; i<TEX_MAX; i++ )
	{
		InitTexture( i );
	}
}

/******************************************************************************/
//	���O	�F	�e�N�X�`�����W�擾
//	����	�F	�e�N�X�`�����W�̎擾���s��
//	�߂�l	�F	long		�e�N�X�`�����W
//	����	�F	[IN]long	TexSize		�e�N�X�`���̃T�C�Y
//	���l	�F	�Ȃ�
/******************************************************************************/
long	GetTexPos( long TexSize )
{
	long	size[2],
			ret=256;

	size[0] = TexSize / 256;
	size[1] = TexSize % 256;

	switch( size[0] )
	{
		case	1:
			if( size[1] == 0 )
			{
				ret = 256;
			}
			else
			{
				ret = 512;
			}
			break;

		case	2:
			if( size[1] == 0 )
			{
				ret = 512;
			}
			else
			{
				ret = 1024;
			}
			break;

		case	4:
			if( size[1] == 0 )
			{
				ret = 1024;
			}
			else
			{
				ret = 2048;
			}
			break;
	}

	return	ret;
}

/******************************************************************************/
//	���O	�F	�e�N�X�`�����[�h
//	����	�F	�摜�t�@�C���̓ǂݍ��݂��s��
//	�߂�l	�F	bool		��������	true(����)
//										false(���s)
//	����	�F	[IN]long	TexNo		�e�N�X�`���ԍ�
//				[IN]char	*FileName	�t�@�C����
//	���l	�F	�Ȃ�
/******************************************************************************/
bool	LoadTexture( long TexNo, char *FileName )
{
	char	str[4] = { NULL };
	long	size = 0;

	if( TexNo >= TEX_MAX )
	{
		return false;
	}

	//	�g�p�t���O
	TEX[ TexNo ].Use = ON;

	PathToOpt( FileName, str );

	char *string;
	errno_t Err = _strlwr_s( string = _strdup(str), strlen(str)+1 );
	if( Err != 0 )
	{
		SAFE_DELETE(string);
		return false;
	}

	//	�J�����g�f�B���N�g���E�o�b�N�A�b�v
	char	name[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, name );

	//	�Í��t�@�C�����ǂ�������
	if( strcmp( "img", string ) == 0 )
	{
		size = AntiSecret( FileName, g_FreeWork );
	}
	else
	{
		size = LoadFile( FileName, g_FreeWork );
	}

	if( size < 0 )
	{
		//	�ǂݍ��ݎ��s
		SAFE_DELETE(string);
		return false;
	}

	BITMAPINFOHEADER	*bmp_info;
	bmp_info = (BITMAPINFOHEADER *)&g_FreeWork[sizeof(BITMAPFILEHEADER)];

	//	�������[����e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFileInMemoryEx(
		g_lpD3DDev,			//	DirectX9�f�o�C�X
		g_FreeWork,			//	���������̃|�C���^
		size,				//	���������̃t�@�C���T�C�Y
		D3DX_DEFAULT,		//	��
		D3DX_DEFAULT,		//	����
		//D3DX_DEFAULT,		//	�~�b�v�}�b�v���x����
		1,					//	�~�b�v�}�b�v���x����
		0,					//	�p�@
		D3DFMT_A8R8G8B8,	//	�s�N�Z���t�H�[�}�b�g
		D3DPOOL_MANAGED,	//	�e�N�X�`���z�u��
		D3DX_FILTER_NONE,	//	�t�B���^�����O
		D3DX_FILTER_NONE,	//	�t�B���^�����O�i�~�b�v�}�b�v�j
		TRANS,				//	�����F
		NULL,				//	�C���[�W���
		NULL,				//	�p���b�g
		&TEX[ TexNo ].lpTex	//	�e�N�X�`���I�u�W�F�N�g
		);

	D3DSURFACE_DESC	Desc;
	TEX[ TexNo ].lpTex->GetLevelDesc( 0, &Desc );

	//D3DXIMAGE_INFO	Info;
	//D3DXGetImageInfoFromFile(FileName, &Info );

		//	�T�C�Y�擾
	//TEX[ TexNo ].Width = GetTexPos( Info->biWidth );
	//TEX[ TexNo ].Height = GetTexPos( bmp_info->biHeight );
	TEX[ TexNo ].Width = Desc.Width;
	TEX[ TexNo ].Height = Desc.Height;

	SAFE_DELETE(string);
	return	true;
}

/******************************************************************************/
//	���O	�F	�I�u�W�F�N�g���
//	����	�F	�I�u�W�F�N�g�̉�����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�G	�Ȃ�
/******************************************************************************/
void	ReleaseTexture( void )
{
	for( int i=0; i<TEX_MAX; i++ )
	{
		if( TEX[i].lpTex != NULL )
		{
			SAFE_RELEASE(TEX[i].lpTex);
		}
	}
}
