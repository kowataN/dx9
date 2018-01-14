/******************************************************************************/
//	�A�j�����C�u����
//		���C���\�[�X
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�A�j���e�N�X�`���f�[�^
ST_AnmTexture	ATT[ANM_MAX];

//	�A�j���f�[�^
ST_AnmData		ANM[ANM_MAX];



/******************************************************************************/
//	���O	�F	�A�j���\���̏�����
//	����	�F	�A�j���\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AnmNo	�A�j���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitAnm( long AnmNo )
{
	if( AnmNo >= ANM_MAX )
	{
		return;
	}

	////////////////////////////////////
	//	�t���O

	//	�g�p�t���OOFF
	ANM[ AnmNo ].Use = OFF;

	////////////////////////////////////
	//	�ԍ�

	//	�t�@�C���ԍ�
	ANM[ AnmNo ].FileNo = 0;
	//	�t���[���ԍ�
	ANM[ AnmNo ].FrmNo = 0;
	//	�f�[�^�ԍ�
	ANM[ AnmNo ].DataNo = 0;
	//	�|���S���ԍ�
	ANM[ AnmNo ].PolygonNo = 0;
	//	�p�^�[���ԍ�
	ANM[ AnmNo ].PatNo = 0;

	////////////////////////////////////
	//	���̑�

	//	�^�C���J�E���^
	ANM[ AnmNo ].TimeCt = 0;
	//	�A�g���r���[�g
	ANM[ AnmNo ].Atr = ATR_NONE;
}

/******************************************************************************/
//	���O	�F	�A�j���\���̑S������
//	����	�F	�A�j���\���̂̑S���������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitAnmAll( void )
{
	//	�\���̏�����
	for( long i=0; i<ANM_MAX; i++ )
	{
		InitAnm( i );
	}
}

/******************************************************************************/
//	���O	�F	�A�j���t�@�C���ǂݍ���
//	����	�F	�A�j���t�@�C���̓ǂݍ��݂��s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	FileNo		�t�@�C���ԍ�
//				[IN]char	*FileName	�ǂݍ��ރA�j���t�@�C��
//				[IN]char	*Buf		�ǂݍ��ݐ�̃o�b�t�@
//				[IN]long	TextureNo	�e�N�X�`���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	LoadAnm( long FileNo, char *FileName, char *Buf, long TextureNo )
{
	if( FileNo >= ANM_MAX )
	{
		return;
	}

	long	i;
	char	*pt,file_path[MAX_PATH];

	/////////////////////////////////////////////////
	//	anm�t�@�C���ǂݍ���
	if( LoadFile( FileName, Buf ) < 0 )
	{
		return;
	}

	/////////////////////////////////////////////////
	//	anm�t�@�C��������
	if( !( Buf[0] == 'A' && Buf[1] == 'N' && Buf[2] == 'M' ) )
	{
		return;
	}

	pt = (char *)Buf;
	pt += 3;

	/////////////////////////////////////////////////
	//	�t�@�C�����擾

	//	�t�@�C���̒���
	long file_len = GetLong( pt );
	pt += 4;

	//	�t�@�C����
	for( i=0; i<file_len; i++ )
	{
		ATT[ FileNo ].FileName[ i ] = *pt;
		pt ++;
	}

	/////////////////////////////////////////////////
	//	�f�B���N�g���ύX

	char	DirStr[MAX_PATH];
	PathToDir( FileName, DirStr );
	PathChange( DirStr, file_path );
	//	�J�����g�f�B���N�g���E�o�b�N�A�b�v
	char	name[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, name );

	/////////////////////////////////////////////////
	//	�e�N�X�`���ǂݍ���
	LoadTexture( TextureNo, ATT[ FileNo ].FileName );

	ATT[ FileNo ].TextureNo = TextureNo;

	//	���̃J�����g�f�B���N�g���ɖ߂�
	_chdir( file_path );
}

/******************************************************************************/
//	���O	�F	�A�j���Z�b�g
//	����	�F	�A�j���̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AnmNo		�Z�b�g����A�j���ԍ�
//				[IN]long	FileNo		�t�@�C���ԍ�
//				[IN]long	DataNo		�ǂݍ��񂾃t�@�C���̃A�j���ԍ�
//				[IN]long	PatNo		�o�^����p�^�[���̔ԍ�
//				[IN]char	*AnmAddr	�A�j���t�@�C���̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetAnm( long AnmNo, long FileNo, long DataNo, long PatNo, char *AnmAddr )
{
	if( AnmNo >= ANM_MAX || FileNo >= ANM_MAX || PatNo >= PAT_MAX )
	{
		return;
	}

	short	i;
	long	FileSize,Index[3];
	long	PatMax,AnmMax,HitMax;
	char	*pt;

	/////////////////////////////////////////////////
	//	�g�p�t���OON
	ANM[ AnmNo ].Use	  = ON;

	/////////////////////////////////////////////////
	//	�ԍ�
	ANM[ AnmNo ].PatNo  = PatNo;
	ANM[ AnmNo ].DataNo = DataNo;

	pt = (char *)AnmAddr;
	ANM[ AnmNo ].AnmAddr = pt;

	/////////////////////////////////////////////////
	//	���ʎq�X�L�b�v
	pt += 3;

	/////////////////////////////////////////////////
	//	bmp�t�@�C�����̒����擾
	FileSize = GetLong( pt );
	pt += 4;

	/////////////////////////////////////////////////
	//	bmp�t�@�C�����̒������A�h���X���Z
	pt += FileSize;

	/////////////////////////////////////////////////
	//	�e�ő吔
	PatMax = GetLong( pt );	//	�p�^�[��
	pt += 4;
	AnmMax = GetLong( pt );	//	�A�j��
	pt += 4;
	HitMax = GetLong( pt );	//	�q�b�g
	pt += 4;

	/////////////////////////////////////////////////
	//	�e�f�[�^�̐擪�A�h���X
	Index[0] = GetLong( pt );
	pt += 4;
	Index[1] = GetLong( pt );
	pt += 4;
	Index[2] = GetLong( pt );
	pt += 4;

	/////////////////////////////////////////////////
	//	�A�j���f�[�^

	//	�A�j���f�[�^�̐擪�A�h���X
	pt = (char *)AnmAddr;
	pt += Index[1];

	if( DataNo >= AnmMax )
	{
		return;
	}

	for( i=0; i<DataNo; i++ )
	{
		//	�t���[���ő吔
		long FrmMax = GetLong( pt );
		pt += 4;
		pt += FrmMax * ANM_INDEX_SIZE;
	}

	//	�t���[���ő吔
	ANM[ AnmNo ].FrameMax = GetLong( pt );
	pt += 4;

	//	�p�^�[���ԍ�
	ANM[ AnmNo ].FRM.PatNo = GetLong( pt );
	pt += 4;

	//	�␳���W
	ANM[ AnmNo ].FRM.CorrectX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.CorrectY = GetDouble( pt );
	pt +=8;

	//	�g�嗦
	ANM[ AnmNo ].FRM.ScaleX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.ScaleY = GetDouble( pt );
	pt +=8;

	//	�p�x
	ANM[ AnmNo ].FRM.Rot = GetDouble( pt );
	pt +=8;

	//	�\������
	ANM[ AnmNo ].FRM.Time = GetLong( pt );
	pt +=4;

	//	�s�����x
	ANM[ AnmNo ].FRM.A = GetDouble( pt );
	pt +=8;

	//	�e�F����
	ANM[ AnmNo ].FRM.R = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.G = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.B = GetDouble( pt );
	pt +=8;

	//	�A�g���r���[�g
	ANM[ AnmNo ].FRM.Rev = GetShort( pt );
	pt +=2;

	/////////////////////////////////////////////////
	//	�p�^�[���f�[�^

	//	�A�h���X
	pt = (char *)AnmAddr;
	pt += Index[0];

	short	x,y,w,h;

	pt += ( ANM[ AnmNo ].FRM.PatNo * PAT_INDEX_SIZE );

	//	x
	x = GetShort( pt );
	pt += 2;
	//	y
	y = GetShort( pt );
	pt += 2;
	//	w
	w = GetShort( pt );
	pt += 2;
	//	h
	h = GetShort( pt );
	pt += 2;

	//	�p�^�[���쐬
	SetPattern( PatNo, ATT[FileNo].TextureNo,  x, y, w, h );
}

/******************************************************************************/
//	���O	�F	�A�j���f�[�^�̃Z�b�g
//	����	�F	�A�j���f�[�^�̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AnmNo		�A�j���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetAnmData( long AnmNo )
{
	short	i;
	long	file_size,index[3],pat_no,
			pat_max,anm_max,hit_max;
	char	*pt;

	pt = (char *)ANM[ AnmNo ].AnmAddr;
	pt += 3;

	/////////////////////////////////////////////////
	//	bmp�t�@�C�����̒����擾
	file_size = GetLong( pt );
	pt += 4;

	/////////////////////////////////////////////////
	//	bmp�t�@�C�����̒������A�h���X���Z
	pt += file_size;

	/////////////////////////////////////////////////
	//	�e�f�[�^�̍ő吔
	pat_max = GetLong( pt );	//	�p�^�[��
	pt += 4;
	anm_max = GetLong( pt );	//	�A�j��
	pt += 4;
	hit_max = GetLong( pt );	//	�q�b�g
	pt += 4;

	/////////////////////////////////////////////////
	//	�e�f�[�^�̐擪�A�h���X
	index[0] = GetLong( pt );	//	�p�^�[��
	pt += 4;
	index[1] = GetLong( pt );	//	�A�j��
	pt += 4;
	index[2] = GetLong( pt );	//	�q�b�g
	pt += 4;

	/////////////////////////////////////////////////
	//	�A�j���f�[�^

	//	�A�j���f�[�^�̐擪�A�h���X
	pt = (char *)ANM[ AnmNo ].AnmAddr;
	pt += index[1];

	for( i=0; i<ANM[AnmNo].DataNo; i++ )
	{
		//	�t���[���ő吔
		long frm_max = GetLong( pt );
		pt += 4;
		pt += frm_max * ANM_INDEX_SIZE;
	}

	//	�t���[���ő吔
	ANM[ AnmNo ].FrameMax = GetLong( pt );
	pt += 4;
	pt += ANM_INDEX_SIZE * ANM[ AnmNo ].FrmNo;

	//	�p�^�[���ԍ�
	ANM[ AnmNo ].FRM.PatNo = GetLong( pt );
	pt += 4;

	//	�␳���W
	ANM[ AnmNo ].FRM.CorrectX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.CorrectY = GetDouble( pt );
	pt +=8;

	//	�g�嗦
	ANM[ AnmNo ].FRM.ScaleX = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.ScaleY = GetDouble( pt );
	pt +=8;

	//	�p�x
	ANM[ AnmNo ].FRM.Rot = GetDouble( pt );
	pt +=8;

	//	�\������
	ANM[ AnmNo ].FRM.Time = GetLong( pt );
	pt +=4;

	//	�s�����x
	ANM[ AnmNo ].FRM.A = GetDouble( pt );
	pt +=8;

	//	�e�F����
	ANM[ AnmNo ].FRM.R = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.G = GetDouble( pt );
	pt +=8;
	ANM[ AnmNo ].FRM.B = GetDouble( pt );
	pt +=8;

	//	�A�g���r���[�g
	ANM[ AnmNo ].FRM.Rev = GetShort( pt );
	pt +=2;

	/////////////////////////////////////////////////
	//	�p�^�[���f�[�^

	//	�A�h���X
	pt = (char *)ANM[ AnmNo ].AnmAddr;
	pt += index[0];

	pt += ( ANM[ AnmNo ].FRM.PatNo * PAT_INDEX_SIZE );

	pat_no = ANM[ AnmNo ].PatNo;

	//	x
	ANM[ AnmNo ].PAT.PosX = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].PosX = ANM[ AnmNo ].PAT.PosX;

	//	y
	ANM[ AnmNo ].PAT.PosY = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].PosY = ANM[ AnmNo ].PAT.PosY;

	//	w
	ANM[ AnmNo ].PAT.Width = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].Width = ANM[ AnmNo ].PAT.Width;

	//	h
	ANM[ AnmNo ].PAT.Height = GetShort( pt );
	pt += 2;
	PAT[ pat_no ].Height = ANM[ AnmNo ].PAT.Height;
}

/******************************************************************************/
//	���O	�F	�A�j���̃��[�v�Đ�
//	����	�F	�A�j���̒P�̂ł̍Đ�
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AnmNo		�A�j���[�V��������A�j���ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	LoopAnmOne( long AnmNo )
{
	if( AnmNo >= ANM_MAX )
	{
		return;
	}

	char	*pt;

	if( ANM[ AnmNo ].TimeCt >= ANM[ AnmNo ].FRM.Time )
	{
		//	�^�C���J�E���^������
		ANM[ AnmNo ].TimeCt = 0;

		//	���̃t���[��
		ANM[ AnmNo ].FrmNo ++;
		if( ANM[ AnmNo ].FrmNo >= ANM[ AnmNo ].FrameMax )
		{
			switch( ANM[ AnmNo ].Flag )
			{
				//	���[�v
				case	ANM_LOOP:
					ANM[ AnmNo ].State = 1;
					ANM[ AnmNo ].FrmNo = 0;
					break;

				//	��~
				case	ANM_STOP:
					ANM[ AnmNo ].State = 0;
					ANM[ AnmNo ].FrmNo = ANM[ AnmNo ].FrameMax - 1;
					break;

				//	�I��
				case	ANM_END:
					ANM[ AnmNo ].State = 0;
					g_LibPol.ChangeDisp( ANM[ AnmNo ].PolygonNo, OFF );
					break;
			}
		}

		SetAnmData( AnmNo );
	}
	else
	{
		/////////////////////////////////////
		//	�g��E��]�E�F�����E�␳���W
		double	sx1,sx2,sy1,sy2;
		double rot1,rot2;
		double	a1,r1,g1,b1;
		double	a2,r2,g2,b2;

		//	�g��E��]
		sx1 = ANM[ AnmNo ].FRM.ScaleX;
		sy1 = ANM[ AnmNo ].FRM.ScaleY;
		rot1 = ANM[ AnmNo ].FRM.Rot;

		//	�s�����x�E�e�F����
		a1 = ANM[ AnmNo ].FRM.A;
		r1 = ANM[ AnmNo ].FRM.R;
		g1 = ANM[ AnmNo ].FRM.G;
		b1 = ANM[ AnmNo ].FRM.B;

		if( ANM[ AnmNo ].FrmNo + 1 >= ANM[ AnmNo ].FrameMax )
		{
			//	���t���[���f�[�^�̃R�s�[
			sx2 = sx1;
			sy2 = sy1;
			rot2 = rot1;
			a2 = a1;
			r2 = r1;
			g2 = g1;
			b2 = b1;
		}
		else
		{
			pt = ANM[ AnmNo ].AnmAddr;
			pt += 3;	//	����
			pt += GetLong( pt );	pt += 4;	//	bmp�t�@�C�����̒����擾
			pt += 4 * 3;	//	�e�f�[�^�̍ő吔
			pt += 4;	//	�p�^�[���f�[�^�擪�A�h���X
			long	index = GetLong( pt );	pt += 4;	//	�A�j���f�[�^�擪�A�h���X
			pt += 4;	//	�q�b�g�f�[�^�擪�A�h���X

			/////////////////////////////////////////////////
			//	�A�j���f�[�^�̐擪�A�h���X
			pt = (char *)ANM[ AnmNo ].AnmAddr;
			pt += index;

			for( int i=0; i<ANM[ AnmNo ].DataNo; i++ )
			{
				//	�t���[���ő吔
				long frm_max = GetLong( pt );pt += 4;
				pt += frm_max * ANM_INDEX_SIZE;
			}

			pt += ANM_INDEX_SIZE * (ANM[ AnmNo ].FrmNo+1);
			pt += 4+4+16;

			//	���̃t���[���f�[�^�̃R�s�[
			sx2 = GetDouble( pt );	pt += 8;
			sy2 = GetDouble( pt );	pt += 8;
			rot2 = GetDouble( pt );	pt += 8+4;
			a2 = GetDouble( pt );	pt += 8;
			r2 = GetDouble( pt );	pt += 8;
			g2 = GetDouble( pt );	pt += 8;
			b2 = GetDouble( pt );	pt += 8;
		}

		//	���₷���悤�ɑ��
		ST_PolygonInfo Info = g_LibPol.GetInfo( ANM[ AnmNo ].PolygonNo );

		//	���]
		ANM[ AnmNo ].Atr = ANM[ AnmNo ].FRM.Rev;
		Info.Atr |= ANM[ AnmNo ].Atr;

		//	�g�嗦
		Info.ScaleX += ( sx2 - sx1 ) / ANM[ AnmNo ].FRM.Time;
		Info.ScaleY += ( sy2 - sy1 ) / ANM[ AnmNo ].FRM.Time;

		//	��]�p�x
		Info.RotZ += ( rot2 - rot1 ) / ANM[ AnmNo ].FRM.Time;

		//	�s�����x
		Info.A += (UShort)( ( a2 - a1 ) / ANM[ AnmNo ].FRM.Time );

		//	�e�F����
		Info.R += (UShort)( ( r2 - r1 ) / ANM[ AnmNo ].FRM.Time );
		Info.G += (UShort)( ( g2 - g1 ) / ANM[ AnmNo ].FRM.Time );
		Info.B += (UShort)( ( b2 - b1 ) / ANM[ AnmNo ].FRM.Time );

		//	�␳���W
		Info.CenterX = (short)ANM[ AnmNo ].FRM.CorrectX;
		Info.CenterY = (short)ANM[ AnmNo ].FRM.CorrectY;

		g_LibPol.SetInfo( ANM[ AnmNo ].PolygonNo, Info );
	}

	//	�J�E���^���Z
	ANM[ AnmNo ].TimeCt ++;
}

/******************************************************************************/
//	���O	�F	�A�j���|���S���ɃZ�b�g
//	����	�F	�A�j�����|���S���ɐݒ肷��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	AnmNo		�A�j���ԍ�
//				[IN]double	PosX		X���W
//				[IN]double	PosY		Y���W
//				[IN]double	PosZ		Z���W
//				[IN]UChar	Flag		�Đ��t���O
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetAnmPolygon(long PolNo,
					  long AnmNo,
					  double PosX, double PosY, double PosZ,
					  UChar Flag )
{
	if( PolNo >= POL_MAX || AnmNo >= ANM_MAX )
	{
		return;
	}

	long	pat;

	//	�|���S���ԍ�
	ANM[ AnmNo ].PolygonNo = PolNo;

	//	�p�^�[���ԍ�
	pat = ANM[ AnmNo ].PatNo;

	//	�A�g���r���[�g
	ANM[ AnmNo ].Atr = ANM[ AnmNo ].FRM.Rev;

	//	�|���S���Z�b�g
	g_LibPol.SetPat( PolNo, pat, PosX, PosY, PosZ, ANM[ AnmNo ].Atr );
	//	���S���W
	g_LibPol.SetCenter( PolNo, (short)ANM[ AnmNo ].FRM.CorrectX, (short)ANM[ AnmNo ].FRM.CorrectY );
	//	�g��
	g_LibPol.SetScale( PolNo, ANM[ AnmNo ].FRM.ScaleX );
	//	��]
	g_LibPol.SetRotZ( PolNo, ANM[ AnmNo ].FRM.Rot );
	//	�s�����x
	g_LibPol.SetColor( PolNo,
		(UChar)ANM[ AnmNo ].FRM.A,
		(UChar)ANM[ AnmNo ].FRM.R,
		(UChar)ANM[ AnmNo ].FRM.G,
		(UChar)ANM[ AnmNo ].FRM.B );

	//	�t���O
	ANM[ AnmNo ].Flag = Flag;

	//	���
	ANM[ AnmNo ].State = 1;

	//	�A�j�����[�v�P��
	LoopAnmOne( AnmNo );
}

/******************************************************************************/
//	���O	�F	�A�j���̑S���[�v�Đ�
//	����	�F	�A�j���̍Đ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	LoopAnm( void )
{
	for( long i=0; i<ANM_MAX; i++ )
	{
		if( !ANM[i].Use || !ANM[i].State )
		{
			continue;
		}
		LoopAnmOne( i );
	}
}

/******************************************************************************/
//	���O	�F	�A�j���f�[�^�R�s�[
//	����	�F	�A�j���f�[�^�̃R�s�[���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	AnmNo		�R�s�[��A�j���ԍ�
//				[IN]long	CopyAnmNo	�R�s�[���A�j���ԍ�
/******************************************************************************/
void	CopyAnmData( long AnmNo, long CopyAnmNo )
{
	if( AnmNo >= ANM_MAX || CopyAnmNo >= ANM_MAX )
	{
		return;
	}

	ANM[ AnmNo ] = ANM[ CopyAnmNo ];
}

