/******************************************************************************/
//	���[�u���C�u����
//		���C���\�[�X
/******************************************************************************/


//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibMove.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	���[�u�f�[�^
ST_MoveData		MOV[MOV_MAX];



/******************************************************************************/
//	���O	�F	���[�u�f�[�^������
//	����	�F	���[�u�\���̂̏��������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	MoveNo		���[�u�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitMove( long MoveNo )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	//	�g�p�t���OOFF
	MOV[ MoveNo ].Use = OFF;

	//	�t���[���ԍ�
	MOV[ MoveNo ].FrameNo = 0;

	//	�|���S���ԍ�
	MOV[ MoveNo ].PolNo = NOT;

	//	�^�C���J�E���^
	MOV[ MoveNo ].TimeCount = 0;
}

/******************************************************************************/
//	���O	�F	���[�u�f�[�^�S������
//	����	�F	���[�u�\���̂̑S���������s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	InitMoveAll( void )
{
	//	�\���̏�����
	for( long i=0; i<MOV_MAX; i++ )
	{
		InitMove( i );
	}
}

/******************************************************************************/
//	���O	�F	���[�u�t�@�C���ǂݍ���
//	����	�F	���[�u�t�@�C���̓ǂݍ��݂��s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]char	FileName	�ǂݍ���mov�t�@�C��
//				[OUT]char	*LoadBuf	�ǂݍ��ݐ�̃o�b�t�@
//	���l	�F	�Ȃ�
/******************************************************************************/
void	LoadMoveFile( char *FileName, char *LoadBuf )
{
	//	mov�t�@�C���ǂݍ���
	if( LoadFile( FileName, LoadBuf ) < 0 )
	{
		return;
	}
}

/******************************************************************************/
//	���O	�F	���[�u�Z�b�g
//	����	�F	���[�u�f�[�^�̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	MoveNo		�Z�b�g���郀�[�u�ԍ�
//				[IN]long	PolNo		�|���S���ԍ�
//				[IN]long	DataNo		�ǂݍ��񂾃t�@�C���̃��[�u�ԍ�
//				[IN]char	*DataAddr	�f�[�^�̃A�h���X
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetMove( long MoveNo, long PolNo, long DataNo, char *DataAddr )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	short	i;

	char	*pt = (char *)DataAddr;
	MOV[ MoveNo ].DataAddr = pt;

	for( i=0; i<DataNo; i++ )
	{
		//	�t���[���ő吔
		long FrmMax = GetLong( pt );
		pt += 4;
		pt += FrmMax * MOV_INDEX_SIZE;
	}

	//	�t���[���ő吔
	MOV[ MoveNo ].FrameMax = GetLong( pt );
	pt += 4;

	//	�ړ���
	MOV[ MoveNo ].FRM.MoveX = GetShort( pt );
	pt += 2;
	MOV[ MoveNo ].FRM.MoveY = GetShort( pt );
	pt += 2;

	//	����
	MOV[ MoveNo ].FRM.Time = GetShort( pt );
	pt += 2;

	MOV[ MoveNo ].Use = ON;
	MOV[ MoveNo ].PolNo = PolNo;
	MOV[ MoveNo ].State = ON;
	MOV[ MoveNo ].ExecFlag = MOVE_LOOP;
	MOV[ MoveNo ].FrameNo = 0;
	MOV[ MoveNo ].DataNo = DataNo;
}

/******************************************************************************/
//	���O	�F	���[�u�f�[�^�Z�b�g
//	����	�F	���[�u�f�[�^�̐ݒ���s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	MoveNo		���[�u�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	SetMoveData( long MoveNo )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	short	i;

	//	�擪�A�h���X
	char *pt = (char *)MOV[ MoveNo ].DataAddr;
	for( i=0; i<MOV[MoveNo].DataNo; i++ )
	{
		//	�t���[���ő吔
		long FrmMax = GetLong( pt );
		pt += 4;
		pt += FrmMax * MOV_INDEX_SIZE;
	}

	//	�t���[���ő吔
	MOV[ MoveNo ].FrameMax = GetLong( pt );
	pt += 4;
	pt += MOV_INDEX_SIZE * MOV[ MoveNo ].FrameNo;

	//	�ړ���
	MOV[ MoveNo ].FRM.MoveX = GetShort( pt );
	pt += 2;
	MOV[ MoveNo ].FRM.MoveY = GetShort( pt );
	pt += 2;

	//	����
	MOV[ MoveNo ].FRM.Time = GetShort( pt );
	pt += 2;
}

/******************************************************************************/
//	���O	�F	���[�u�Đ�
//	����	�F	���[�u�̍Đ����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	MoveNo		���[�u�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	LoopMoveOne( long MoveNo )
{
	if( MoveNo >= MOV_MAX )
	{
		return;
	}

	if( !MOV[ MoveNo ].Use || !MOV[ MoveNo ].State )
	{
		return;
	}

	if( MOV[ MoveNo ].TimeCount >= MOV[ MoveNo ].FRM.Time )
	{
		//	�^�C���J�E���^������
		MOV[ MoveNo ].TimeCount = 0;

		//	���̃t���[��
		MOV[ MoveNo ].FrameNo ++;
		if( MOV[ MoveNo ].FrameNo >= MOV[ MoveNo ].FrameMax )
		{
			switch( MOV[ MoveNo ].ExecFlag )
			{
				//	���[�v
				case	MOVE_LOOP:
					MOV[ MoveNo ].State = 1;
					MOV[ MoveNo ].FrameNo = 0;
					break;

				//	��~
				case	MOVE_STOP:
					MOV[ MoveNo ].State = 0;
					MOV[ MoveNo ].FrameNo = MOV[ MoveNo ].FrameMax - 1;
					break;

				//	�I��
				case	MOVE_END:
					MOV[ MoveNo ].State = 0;
					break;
			}
		}

		SetMoveData( MoveNo );
	}

	//	�ړ�
	double MoveX = MOV[ MoveNo ].FRM.MoveX;
	double MoveY = MOV[ MoveNo ].FRM.MoveY;

	long PolNo = MOV[ MoveNo ].PolNo;
	g_LibPol.AddPosX( PolNo, MoveX / MOV[ MoveNo ].FRM.Time );
	g_LibPol.AddPosY( PolNo, MoveY / MOV[ MoveNo ].FRM.Time );

	//	�J�E���^���Z
	MOV[ MoveNo ].TimeCount ++;
}

/******************************************************************************/
//	���O	�F	���[�u�S�Đ�
//	����	�F	���[�u�̑S�Đ����s��
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	LoopMove( void )
{
	for( long i=0; i<MOV_MAX; i++ )
	{
		LoopMoveOne( i );
	}
}
