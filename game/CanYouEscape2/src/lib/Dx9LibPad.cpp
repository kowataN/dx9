/******************************************************************************/
//	�p�b�h���C�u����
//		���C���t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"Dx9LibPad.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�L�[�{�[�h������
short	SetKeyDat[PAD_MAX][PAD_BUTTON_MAX] =
{
	//	�� ��         ��        �E         �@           �A           �B           �C            �X�^�[�g
	DIK_W,  DIK_Z,    DIK_A,    DIK_S,     DIK_C,       DIK_V,       DIK_B,       DIK_N,       DIK_M,       DIK_RETURN,
	DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_NUMPAD1, DIK_NUMPAD2, DIK_NUMPAD3, DIK_NUMPAD4, DIK_NUMPAD5, 156,
};

//	�W���C�X�e�B�b�N������
short	SetJoyDat[PAD_MAX][PAD_BUTTON_MAX] = 
{
	//	��  ��        ��        �E         �@        �A        �B        �C        �X�^�[�g
	JOY_UP, JOY_DOWN, JOY_LEFT, JOY_RIGHT, JOY_BUT1, JOY_BUT2, JOY_BUT3, JOY_BUT4, JOY_BUT5,
	JOY_UP, JOY_DOWN, JOY_LEFT, JOY_RIGHT, JOY_BUT1, JOY_BUT2, JOY_BUT3, JOY_BUT4, JOY_BUT5,
};

//	�p�b�h�\����
PAD_TBL	PAD[PAD_MAX];



/******************************************************************************/
//	�p�b�h�������i�P�́j
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�p�b�h�ԍ�
/******************************************************************************/
void	InitPad( long PadNo )
{
	PAD[PadNo].Use = OFF;
	PAD[PadNo].JoyUse = OFF;

	for( long i=0; i<PAD_BUTTON_MAX; i++ )
	{
		PAD[PadNo].Trg[i] = OFF;
		PAD[PadNo].TrgBk[i] = OFF;
		PAD[PadNo].Push[i] = OFF;
		PAD[PadNo].PushBk[i] = OFF;
		PAD[PadNo].Rpt[i] = 0;
		PAD[PadNo].Count[i] = 0;

		PAD[PadNo].KeyNo[i] = SetKeyDat[PadNo][i];
		PAD[PadNo].JoyNo[i] = SetJoyDat[PadNo][i];
	}
}

/******************************************************************************/
//	�p�b�h�������i�S�́j
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	InitPadAll( void )
{
	for( long i=0; i<PAD_MAX; i++ )
	{
		InitPad( i );
	}
}

/******************************************************************************/
//	�p�b�h���C��
//		�߂�l�F
//				�Ȃ�
//		�����F
//				no		�p�b�h�ԍ�
/******************************************************************************/
void	MainPad( long PadNo )
{
	long	i;

	//	�g�p�t���O�m�F
	if( PAD[PadNo].Use == OFF )
	{
		return;
	}

	///////////////////////////////
	//	�o�b�N�A�b�v
	for( i=0; i<PAD_BUTTON_MAX; i++ )
	{
		//	�g���K
		PAD[PadNo].TrgBk[i] = PAD[PadNo].Trg[i];

		//	�v�b�V��
		PAD[PadNo].PushBk[i] = PAD[PadNo].Push[i];

		//	������
		PAD[PadNo].Trg[i] = OFF;
		PAD[PadNo].Push[i] = OFF;
	}

	///////////////////////////////
	//	�L�[�{�[�h�̔���
	for( i=0; i<PAD_BUTTON_MAX; i++ )
	{
		//	�ꎞ�i�[
		long KeyNo = PAD[PadNo].KeyNo[i];

		//	�g���K
		PAD[PadNo].Trg[i] |= KEY.Trg[KeyNo];

		//	�v�b�V��
		PAD[PadNo].Push[i] |= KEY.Push[KeyNo];
	}

	///////////////////////////////
	//	�W���C�X�e�B�b�N�̔���
	if( PAD[PadNo].JoyUse )
	{
		//	�����L�[
		for( i=0; i<PAD_BUTTON_MAX; i++ )
		{
			long	JoyNo = PAD[PadNo].JoyNo[i];

			//	�g���K
			PAD[PadNo].Trg[i] |= JOY[PadNo].Trg[JoyNo];

			//	�v�b�V��
			PAD[PadNo].Push[i] |= JOY[PadNo].Push[JoyNo];
		}
	}

	///////////////////////////////
	//	�J�E���^�A�L�[���s�[�g
	for( i=0; i<PAD_BUTTON_MAX; i++ )
	{
		//	�J�E���^
		if( PAD[PadNo].Push[i] )
		{
			PAD[PadNo].Count[i] ++;
		}
		else
		{
			PAD[PadNo].Count[i] = 0;
		}

		//	�L�[���s�[�g
		if( ( PAD[PadNo].Trg[i] )
		|| ( ( PAD[PadNo].Count[i] >= KEY_REPEAT_START )
		&& ( PAD[PadNo].Count[i] % KEY_REPEAT_TIME == 0 ) ) )
		{
			PAD[PadNo].Rpt[i] = ON;
		}
		else
		{
			PAD[PadNo].Rpt[i] = OFF;
		}
	}
}

/******************************************************************************/
//	�p�b�h���[�v
//		�߂�l�F
//				�Ȃ�
//		�����F
//				�Ȃ�
/******************************************************************************/
void	LoopPad( void )
{
	for( long i=0; i<PAD_MAX; i++ )
	{
		MainPad( i );
	}
}

/******************************************************************************/
//	�p�b�h�f�[�^�ǂݍ���
/******************************************************************************/
void	LoadPadData( char* FileName )
{
	char	LoadBuf[1024];

	//	�t�@�C���ǂݍ���
	if( LoadFile( FileName, LoadBuf ) < 0 )
	{
		return;
	}

	//	�f�[�^�擾
	char	*pt = (char*)LoadBuf;
	long	PadMax = GetLong( pt );
	pt += 4;

	for( long PadNo=0; PadNo<PadMax; PadNo++ )
	{
		for( long ButNo=0; ButNo<PAD_BUTTON_MAX; ButNo++ )
		{
			SetJoyDat[ PadNo ][ ButNo ] = GetShort( pt );
			pt += 2;
		}
	}
}
