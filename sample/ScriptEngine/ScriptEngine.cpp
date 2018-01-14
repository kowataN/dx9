/******************************************************************************/
//	�X�N���v�g
//		�\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibPad.h"
#include	"./ScriptEngine.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�X�N���v�g�R�}���h������
char	ScriptCmdStr[CommandMax][32] =
{
	"text",			//	0	������\��
	"end",			//	1	�I��
	"jump",			//	2	���x���W�����v
	"sel",			//	3	�I����
	"flag_on",		//	4	�t���OON
	"flag_off",		//	5	�t���OOFF
	"flag_chk",		//	6	�t���O�`�F�b�N
	"black_out",	//	7	�t�F�[�h�A�E�g�i�u���b�N�A�E�g�j
	"black_in",		//	8	�t�F�[�h�C���i�u���b�N�C���j
	"white_out",	//	9	�t�F�[�h�A�E�g�i�z���C�g�A�E�g�j
	"white_in",		//	10	�t�F�[�h�C���i�z���C�g�C���j
	"cursor_blink",	//	11	�J�[�\���u�����N
	"wait",			//	12	�x��
	"scr_change",	//	13	�X�N���v�g�t�@�C���ύX
	"text_input",	//	14	���������
	"set_str",		//	15	��������Z�b�g
	"save_point",	//	16	�Z�[�u�|�C���g
	"shake",		//	17	��ʂ�h�炷
	"load_tex",		//	18	�e�N�X�`���摜�ǂݍ���
	"set_pat",		//	19	�p�^�[���Z�b�g
	"set_pol",		//	20	�|���S���Z�b�g
	"load_bg",		//	21	�w�i�ǂݍ���
	"disp_bg",		//	22	�w�i�\���E��\��
	"wipe",			//	23	���C�v
	"play_bgm",		//	24	BGM�Đ�
	"stop_bgm",		//	25	BGM��~
	"play_se",		//	26	SE�Đ�
	"init_str",		//	27	�Z�b�g�����񏉊���
};



/******************************************************************************/
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CScriptEngine::CScriptEngine()
: m_UseTextureNo(TEX_SCRIPT_BG),
m_UsePatNo(PAT_SCRIPT_BG),
m_UsePolNo(POL_SCRIPT_BG),
m_UseTextNo(TEXT_SCRIPT_NO),
m_UseBGMNo(AUDIO_SCRIPT_BGM),
m_UseSENo(AUDIO_SCRIPT_SE),
m_Wipe(NULL)
{
}

/******************************************************************************/
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]long	TextureNo	�g�p����e�N�X�`���ԍ�
//				[IN]long	PatNo		�g�p����p�^�[���ԍ�
//				[IN]long	PolNo		�g�p����|���S���ԍ�
//				[IN]long	TextNo		�g�p����e�L�X�g�ԍ�
//				[IN]long	BGMno		�g�p����BGM�ԍ�
//				[IN]long	SENo		�g�p����SE�ԍ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CScriptEngine::CScriptEngine(long TextureNo,
							 long PatNo,
							 long PolNo,
							 long TextNo,
							 long BGMNo,
							 long SENo )
: m_UseTextureNo(TEX_SCRIPT_BG),
m_UsePatNo(PAT_SCRIPT_BG),
m_UsePolNo(POL_SCRIPT_BG),
m_UseTextNo(TEXT_SCRIPT_NO),
m_UseBGMNo(AUDIO_SCRIPT_BGM),
m_UseSENo(AUDIO_SCRIPT_SE),
m_Wipe(NULL)
{
	if( TextureNo < TEX_MAX && TextureNo > NOT )
	{
		m_UseTextureNo = TextureNo;
	}
	if( PatNo < PAT_MAX && PatNo > NOT )
	{
		m_UsePatNo = PatNo;
	}
	if( PolNo < POL_MAX && PolNo > NOT)
	{
		m_UsePolNo = PolNo;
	}
	if( TextNo < TXT_MAX && TextNo > NOT )
	{
		m_UseTextNo = TextNo;
	}
	if( BGMNo < AUDIO_MAX && BGMNo > NOT )
	{
		m_UseBGMNo = BGMNo;
	}
	if( SENo < AUDIO_MAX && SENo > NOT )
	{
		m_UseSENo = SENo;
	}
}

/******************************************************************************/
//	���O	�F	�f�X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CScriptEngine::~CScriptEngine()
{
	SAFE_DELETE(m_Wipe);
}

/******************************************************************************/
//	���O	�F	�X�N���v�g�G���W��������
//	����	�F	�X�N���v�g�t�@�C���̐������݂��s��
//	�߂�l	�F	�Ȃ�
//	����	�F	[IN]char	*FileName	�ǂݍ��ރX�N���v�g�t�@�C��
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CScriptEngine::Init( char *FileName )
{
	//	�\���̏�����
	m_SCR.Mode = 0;				//	�Z�b�g���[�h
	m_SCR.Addr = m_ScrDat;		//	�擪�A�h���X�Z�b�g
	m_SCR.WaitFlag = OFF;		//	�x��
	m_SCR.BlinkCursor = OFF;	//	�J�[�\���_��
	m_SCR.CmdNo = 0;			//	�R�}���h�ԍ�
	m_SCR.SelectNo = 0;			//	�I�����ԍ�

	//	���̑�������
	ZeroMemory( &m_SCR.JumpStr, sizeof(m_SCR.JumpStr) );
	ZeroMemory( &m_SCR.EventFlag, sizeof(m_SCR.EventFlag) );
	ZeroMemory( &m_SCR.SET, sizeof(m_SCR.SET) );
	ZeroMemory( &m_SCR.SHAKE, sizeof(m_SCR.SHAKE) );
	ZeroMemory( &m_TEXT, sizeof(m_TEXT) );

	CDx9LibIni IniMng;
	IniMng.Initialize("ScriptSetup.ini");
	int GetData = IniMng.GetValueInt( "System", "UseSystem" );

	//	�X�N���v�g�t�@�C���ǂݍ���
	LoadFile( FileName, m_ScrDat );

	long	SysTexNo = m_UseTextureNo + TEX_SCRIPT_SYS;
	long	CurPatNo = PAT_SCRIPT_BG + PAT_SCRIPT_CUR;
	long	TextFldPatNo = PAT_SCRIPT_BG +  PAT_SCRIPT_TEXT_FLD;
	long	CurPolNo = POL_SCRIPT_BG + POL_SCRIPT_CUR;
	long	TextFldPolNo = POL_SCRIPT_BG + POL_SCRIPT_TEXT_FLD;

	//	�V�X�e���t�@�C���ǂݍ���
	LoadTexture( SysTexNo, "data\\system.bmp" );

	SetPattern( CurPatNo, SysTexNo, 0,0, 24,24 );
	SetPattern( TextFldPatNo, SysTexNo, 0,24, 640,88 );

	g_LibPol.SetPat( CurPolNo, CurPatNo, TEXT_CURSOR_X,TEXT_CURSOR_Y,1.0, ATR_NONE );
	g_LibPol.SetPat( TextFldPolNo, TextFldPatNo, TEXT_FLD_X,TEXT_FLD_Y,1.0, ATR_NONE );
	g_LibPol.Set( TextFldPolNo+1, 624,72, TEXT_FLD_BG_X, TEXT_FLD_BG_Y,0.9, ATR_NONE );
	g_LibPol.SetColor( TextFldPolNo+1, 255, 0, 0, 0 );

	m_CursorTimer = 0;
}

/******************************************************************************/
//	���O	�F	�X�N���v�g�G���W�����C��
//	����	�F	�X�N���v�g�G���W���̃��C���������s��
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CScriptEngine::Main( void )
{
	if( m_SCR.Mode == 0 )
	{
		Set();	//	�Z�b�g���[�h
	}
	else if( m_SCR.Mode )
	{
		Loop();	//	���[�v���[�h
		DispTextLoop();
	}
}

/******************************************************************************/
//	���O	�F	�X�N���v�g�Z�b�g
//	����	�F	�X�N���v�g�̐ݒ���s��
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CScriptEngine::Set( void )
{
	while( m_SCR.Mode == 0 )
	{
		//	���̃R�}���h�擾
		m_SCR.CmdNo = GetNextCommand();
		switch( m_SCR.CmdNo )
		{
			//	�e�L�X�g�R�}���h�E�Z�b�g
			case	Command_text:
				CommandTextSet();
				break;
			//	�I���R�}���h�E�Z�b�g
			case	Command_end:
				CommandEndSet();
				break;
			//	�W�����v�R�}���h�E�Z�b�g
			case	Command_jump:
				CommandJumpSet();
				break;
			//	�I�����R�}���h�E�Z�b�g
			case	Command_sel:
				CommandSelSet();
				break;
			//	�t���OON�R�}���h�E�Z�b�g
			case	Command_flag_on:
				CommandFlagOnSet();
				break;
			//	�t���OOFF�R�}���h�E�Z�b�g
			case	Command_flag_off:
				CommandFlagOffSet();
				break;
			//	�t���O�`�F�b�N�R�}���h�E�Z�b�g
			case	Command_flag_chk:
				CommandFlagChkSet();
				break;
			//	�u���b�N�A�E�g�R�}���h�E�Z�b�g
			case	Command_black_out:
				CommandBlackOutSet();
				break;
			//	�u���b�N�C���R�}���h�E�Z�b�g
			case	Command_black_in:
				CommandBlackInSet();
				break;
			//	�z���C�g�A�E�g�R�}���h�E�Z�b�g
			case	Command_white_out:
				CommandWhiteOutSet();
				break;
			//	�z���C�g�C���R�}���h�E�Z�b�g
			case	Command_white_in:
				CommandWhiteInSet();
				break;
			//	�J�[�\���_�ŃR�}���h�E�Z�b�g
			case	Command_cursor_blink:
				CommandCursorBlinkSet();
				break;
			//	�x���R�}���h�E�Z�b�g
			case	Command_wait:
				CommandWaitSet();
				break;
			//	�X�N���v�g�t�@�C���ύX�R�}���h�E�Z�b�g
			case	Command_scr_change:
				CommandScriptChangeSet();
				break;
			//	�e�L�X�g���̓R�}���h�E�Z�b�g
			case	Command_text_input:
				CommandTextInputSet();
				break;
			//	������Z�b�g�R�}���h�E�Z�b�g
			case	Command_set_str:
				CommandSetStrSet();
				break;
			//	�Z�[�u�|�C���g�R�}���h�E�Z�b�g
			case	Command_save_point:
				CommandSavePointSet();
				break;
			//	��ʗh�炷�R�}���h�E�Z�b�g
			case	Command_shake:
				CommandShakeSet();
				break;
			//	�e�N�X�`���摜�ǂݍ���
			case	Command_load_tex:
				CommandLoadTex();
				break;
			//	�p�^�[���Z�b�g
			case	Command_set_pat:
				CommandSetPat();
				break;
			//	�|���S���Z�b�g
			case	Command_set_pol:
				CommandSetPol();
				break;
			//	�w�i�ǂݍ���
			case	Command_load_bg:
				CommandLoadBg();
				break;
			//	�w�i�\���E��\��
			case	Command_disp_bg:
				CommandDispBg();
				break;
			//	���C�v
			case	Command_wipe:
				CommandWipe();
				break;
			//	BGM�Đ�
			case	Command_play_bgm:
				CommandPlayBGM();
				break;
			//	BGM��~
			case	Command_stop_bgm:
				CommandStopBGM();
				break;
			//	SE�Đ�
			case	Command_play_se:
				CommandPlaySE();
				break;
			//	�Z�b�g�����񏉊���
			case	Command_init_str:
				CommandInitStr();
				break;
		}
	}
}

/******************************************************************************/
//	���O	�F	�X�N���v�g���[�v
//	����	�F	�X�N���v�g�̃��[�v�������s��
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
void	CScriptEngine::Loop( void )
{
	//	�e�R�}���h�̃��[�v����
	switch( m_SCR.CmdNo )
	{
		//	�e�L�X�g�R�}���h�E���[�v
		case	Command_text:
			CommandTextLoop();
			break;
		//	�I�����R�}���h�E���[�v
		case	Command_sel:
			CommandSelLoop();
			break;
		//	�u���b�N�A�E�g�R�}���h�E���[�v
		case	Command_black_out:
			CommandBlackOutLoop();
			break;
		//	�u���b�N�C���R�}���h�E���[�v
		case	Command_black_in:
			CommandBlackInLoop();
			break;
		//	�z���C�g�A�E�g�R�}���h�E���[�v
		case	Command_white_out:
			CommandWhiteOutLoop();
			break;
		//	�z���C�g�C���R�}���h�E���[�v
		case	Command_white_in:
			CommandWhiteInLoop();
			break;
		//	�J�[�\���_�ŃR�}���h�E���[�v
		case	Command_cursor_blink:
			CommandCursorBlinkLoop();
			break;
		//	��ʗh�炷�R�}���h�E���[�v
		case	Command_shake:
			CommandShakeLoop();
			break;
		//	���C�v
		case	Command_wipe:
			if( !m_Wipe->Loop() )
			{
				m_SCR.Mode = 0;
			}
			break;

	}
}

/******************************************************************************/
//	���O	�F	���̃R�}���h�擾
//	����	�F	���̃R�}���h���擾����
//	�߂�l	�F	long	�R�}���h�ԍ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
long	CScriptEngine::GetNextCommand( void )
{
	long	ret;

	//	���́u*�v������
	m_SCR.Addr = ScriptSearchChar( m_SCR.Addr, '*' );

	ret = 1000;

	for( int i=0; i<CommandMax; i++ )
	{
		if( ScriptStrComp( m_SCR.Addr, ScriptCmdStr[i] ) == 1 )
		{
			//	�R�}���h����
			ret = i;

			//	�A�h���X���R�}���h���̌��Ɉړ�
			m_SCR.Addr = ScriptSkipStr( m_SCR.Addr );

			break;
		}
	}

	return ret;
}

/******************************************************************************/
//	������\��
//		�����F
//				�Ȃ�
//		�߂�l�F
//				�Ȃ�
/******************************************************************************/
void	CScriptEngine::DispTextLoop( void )
{
	//	�e�L�X�g�o�^
	for( int i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		//	������̐F
		switch( m_TEXT.SST[i].TextColor )
		{
			case	SETextColor::Normal:	//	�ʏ�
				g_LibText.SetColor( m_UseTextNo+i, 255,255,255,255 );
				break;
			case	SETextColor::Select:	//	�I��
				g_LibText.SetColor( m_UseTextNo+i, 255,200,230,255 );
				break;
			case	SETextColor::NotSelect:	//	��I��
				g_LibText.SetColor( m_UseTextNo+i, 255,120,120,120 );
				break;
		}
	}
}

/******************************************************************************/
//	�e�L�X�g�Z�b�g
//		�����F
//				�Ȃ�
//		�߂�l�F
//				�Ȃ�
/******************************************************************************/
void	CScriptEngine::CommandTextSet( void )
{
	int i;

	//	TEXT_LINE_MAX�����[�v
	for( i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		//	�u"�v���̕�����𒊏o
		m_SCR.Addr = ScriptGetStrDouble( m_SCR.Addr, m_TEXT.SST[i].Text );

		//	���̍s�͂���̂�����
		m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

		//	������̐F�ݒ�
		m_TEXT.SST[i].TextColor = SETextColor::Normal;

		//	�Z�b�g���������񂪂��邩����
		if( strstr( m_TEXT.SST[i].Text, "STR" ) != NULL )
		{
			//	���݂���
			SwapStr( m_TEXT.SST[i].Text );
		}

		g_LibText.Set( m_UseTextNo+i,
			TEXT_DISP_X,TEXT_DISP_Y+i*FONT_SIZE,
			255,255,255,255, m_TEXT.SST[i].Text, ATR_ONCE ); 

		TXT[ m_UseTextNo+i ].Disp = OFF;


		//	���̕������u"�v���ǂ���
		if( *m_SCR.Addr != '"' )
		{
			break;
		}
	}

	m_TEXT.MaxLine = i + 1;
	m_TEXT.Count = 0;
	m_TEXT.CrtLine = 0;
	m_TEXT.CrtTime = 0;
	m_TEXT.State = 0;

	m_SCR.Mode = 1;
}

/******************************************************************************/
//	�I���E�Z�b�g
//		�����F
//				�Ȃ�
//		�߂�l�F
//				�Ȃ�
/******************************************************************************/
void	CScriptEngine::CommandEndSet( void )
{
	m_SCR.Mode = 100;
	PostQuitMessage( 0 );
}

/******************************************************************************/
//	���x���W�����v
//		�����F
//				�Ȃ�
//		�߂�l�F
//				�Ȃ�
/******************************************************************************/
void	CScriptEngine::CommandJumpSet( void )
{
	//	���x���܂ŃX�L�b�v
	m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

	//	���x�����o
	m_SCR.JumpStr[0][0] = '#';	//	���x���p�L��
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, &m_SCR.JumpStr[0][1] );

	//	���x���W�����v
	m_SCR.Addr = ScriptLabelJump( m_ScrDat, m_SCR.JumpStr[0] );
}

/******************************************************************************/
//	�I�����R�}���h�E�Z�b�g
//		�����F
//				�Ȃ�
//		�߂�l�F
//				�Ȃ�
/******************************************************************************/
void	CScriptEngine::CommandSelSet( void )
{
	long	i;

	//	�s�����[�v
	for( i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		//	�󔒕����񒊏o
		m_SCR.JumpStr[i][0] = '#';
		m_SCR.Addr = ScriptGetStr( m_SCR.Addr, &m_SCR.JumpStr[i][1] );

		m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

		//	�����u"�v������
		if( *m_SCR.Addr == '"' )
		{
			m_TEXT.MaxLine = i + 1;
			break;
		}
	}

	//	�I�����̐������[�v
	for( i=0; i<m_TEXT.MaxLine; i++ )
	{
		//	�u"�v���̕����񒊏o
		m_SCR.Addr = ScriptGetStrDouble( m_SCR.Addr, m_TEXT.SST[i].Text );

		//	���̍s�͂���̂�����
		m_SCR.Addr = ScriptSkipSpace( m_SCR.Addr );

		//	���̕�����̐F�𔻒�
		m_TEXT.SST[i].TextColor = SETextColor::NotSelect;	//	��I��

		g_LibText.Set( m_UseTextNo+i,
			TEXT_DISP_X,
			TEXT_DISP_Y+i*FONT_SIZE,
			255,255,255,255, m_TEXT.SST[i].Text );

		if( *m_SCR.Addr != '"' )
		{
			break;
		}
	}

	//	�I�����ԍ�������
	m_SCR.SelectNo = 0;

	//	���[�v
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	�t���O�R�}���hON�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandFlagOnSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�t���O�ԍ�������̒��o
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//	�����̕�����𐔒l�֕ϊ�
	long no = atol( GetStr );
	if( no >= SCR_FLAG_MAX || no < 0 )
	{
		return;
	}

	//	�t���OON
	m_SCR.EventFlag[ no ] = ON;
}

/******************************************************************************/
//	�t���O�R�}���hOFF�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandFlagOffSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�t���O�ԍ�������̒��o
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//	�����̕�����𐔒l�֕ϊ�
	long no = atol( GetStr );

	//	�t���OON
	m_SCR.EventFlag[ no ] = OFF;
}

/******************************************************************************/
//	�t���O�R�}���h�`�F�b�N�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandFlagChkSet( void )
{
	//	�t���O�ԍ�������̒��o
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, m_SCR.JumpStr[0] );
	//	�����̕�����𐔒l�֕ϊ�
	long no = atol( m_SCR.JumpStr[0] );
	if( no >= SCR_FLAG_MAX || no < 0 )
	{
		return;
	}

	//	��ѐ�̃��x���̒��o
	m_SCR.JumpStr[1][0] = '#';
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, &m_SCR.JumpStr[1][1] );

	//	�t���O����
	if( m_SCR.EventFlag[ no ] )
	{
		//	���x���W�����v
		m_SCR.Addr = ScriptLabelJump( m_ScrDat, m_SCR.JumpStr[1] );
	}
}

/******************************************************************************/
//	�u���b�N�A�E�g�R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandBlackOutSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	���Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.BlackOut( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	�u���b�N�C���R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandBlackInSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	���Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.BlackIn( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	�z���C�g�A�E�g�R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandWhiteOutSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	���Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.WhiteOut( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	�z���C�g�C���R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandWhiteInSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	���Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol(GetStr);

	g_LibFade.WhiteIn( Time );
	m_SCR.Mode = 1;
}

/******************************************************************************/
//	�J�[�\���_�ŃR�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandCursorBlinkSet( void )
{
	//	�e�L�X�g�\������
	m_TEXT.DispFlag = OFF;

	//	�J�[�\���_�ŊJ�n
	m_SCR.BlinkCursor = ON;

	//	���[�h�ύX
	m_SCR.Mode = 1;

	//	�J�[�\���\��
	g_LibPol.ChangeDisp( m_UsePolNo + POL_SCRIPT_CUR, ON );
}

/******************************************************************************/
//	�x���R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandWaitSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	���Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Time = atol( GetStr );

	WaitTime( Time );
}

/******************************************************************************/
//	�X�N���v�g�t�@�C���ύX�R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandScriptChangeSet( void )
{
}

/******************************************************************************/
//	�e�L�X�g���̓R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandTextInputSet( void )
{
	//DialogTest();
}

/******************************************************************************/
//	������Z�b�g�R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandSetStrSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long StrNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );

	if( StrNo >= STR_SET_MAX )
	{
		return;
	}

	//	���O�擾
	ZeroMemory( &m_SCR.SET[StrNo], sizeof(m_SCR.SET[StrNo]) );
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, m_SCR.SET[StrNo].Text );
}

/******************************************************************************/
//	�Z�[�u�|�C���g�R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandSavePointSet( void )
{
}

/******************************************************************************/
//	��ʗh�炷�R�}���h�E�Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandShakeSet( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�����擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	m_SCR.SHAKE.Value = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	���Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	m_SCR.SHAKE.Time = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	�����擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	m_SCR.SHAKE.Atr = 0;
	if( strcmp( GetStr, "XY" ) == 0 )
	{
		m_SCR.SHAKE.Atr = SHAKAE_ATR_XY;
	}
	else if( strcmp( GetStr, "X" ) == 0 )
	{
		m_SCR.SHAKE.Atr = SHAKAE_ATR_X;
	}
	else if( strcmp( GetStr, "Y" ) == 0 )
	{
		m_SCR.SHAKE.Atr = SHAKAE_ATR_Y;
	}

	//	�e�L�X�g�\������
	m_TEXT.DispFlag = OFF;
	//	���[�h�ύX
	m_SCR.Mode = 1;
	//	�J�E���^������
	m_SCR.SHAKE.Count = 0;
	//	��ԏ�����
	m_SCR.SHAKE.State = 0;
}

/******************************************************************************/
//	�e�N�X�`���摜�ǂݍ���
/******************************************************************************/
void	CScriptEngine::CommandLoadTex( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�e�N�X�`���ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long TexNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	�t�@�C�����擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	//	�e�N�X�`���ǂݍ���
	LoadTexture( TexNo, GetStr );
}

/******************************************************************************/
//	�p�^�[���Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandSetPat( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�p�^�[���ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PatNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	�e�N�X�`���ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long TexNo = atol( GetStr );
	//	�J�n���WX�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PosX = atol( GetStr );
	//	�J�n���WY�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PosY = atol( GetStr );
	//	���T�C�Y�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Width = atol( GetStr );
	//	�c�T�C�Y�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long Height = atol( GetStr );

	//	�p�^�[���Z�b�g
	SetPattern( PatNo, TexNo, PosX,PosY, Width,Height );
}

/******************************************************************************/
//	�|���S���Z�b�g
/******************************************************************************/
void	CScriptEngine::CommandSetPol( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�|���S���ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PolNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	�p�^�[���ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long PatNo = atol( GetStr );
	//	�\�����WX�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	double PosX = atof( GetStr );
	//	�\�����WY�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	double PosY = atof( GetStr );
	//	�\�����WZ�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	double PosZ = atof( GetStr );
	//	�A�g���r���[�g�擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	UShort Atr = ATR_NONE;
	if( strcmp( GetStr, "ATR_REV_W" ) == 0 )
	{
		Atr = ATR_REV_W;
	}
	else if( strcmp( GetStr, "ATR_REV_H" ) == 0 )
	{
		Atr = ATR_REV_H;
	}
	else if( strcmp( GetStr, "ATR_ALL" ) == 0 )
	{
		Atr = ATR_ALL;
	}

	//	�|���S���Z�b�g
	g_LibPol.SetPat( PolNo, PatNo, PosX,PosY,PosZ, Atr );
}

/******************************************************************************/
//	�w�i�ǂݍ���
/******************************************************************************/
void	CScriptEngine::CommandLoadBg( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�w�i�ԍ��擾
	//m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//long BgNo = atol( GetStr );
	//ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�t�@�C�����擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	//	�e�N�X�`���ǂݍ���
	LoadTexture( m_UseTextureNo, GetStr );
	SetPattern( m_UsePatNo,
		m_UseTextureNo,
		0,0,
		g_WindowWidth,
		g_WindowHeight
		);
	g_LibPol.SetPat( m_UsePolNo, m_UsePatNo, 0,0,0, ATR_NONE );
	g_LibPol.ChangeDisp( m_UsePolNo, OFF );
}
	
/******************************************************************************/
//	�w�i�\���E��\��
/******************************************************************************/
void	CScriptEngine::CommandDispBg( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�w�i�ԍ��擾
	//m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	//long BgNo = atol( GetStr );
	//ZeroMemory( &GetStr, sizeof(GetStr) );

	//	��Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	UChar Flag = ON;
	if( strcmp( GetStr, "ON" ) == 0 )
	{
		Flag = ON;
	}
	else if( strcmp( GetStr, "OFF" ) == 0 )
	{
		Flag = OFF;
	}

	g_LibPol.ChangeDisp( m_UsePolNo, Flag );
}

/******************************************************************************/
//	���C�v
/******************************************************************************/
void	CScriptEngine::CommandWipe( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	���C�v�ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long	WipeNo = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );
	//	���Ԏ擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long	Time = atol( GetStr );
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�t�@�C�����擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	SAFE_DELETE(m_Wipe);
	m_Wipe = new CScriptWipeEngine( m_UseTextureNo, m_UsePatNo, m_UsePolNo );
	m_Wipe->Init( WipeNo, Time, GetStr );

	m_SCR.Mode = 1;
}

/******************************************************************************/
//	BGM�Đ�
/******************************************************************************/
void	CScriptEngine::CommandPlayBGM( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�t�@�C�����擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	g_LibMusic.Load( m_UseBGMNo, GetStr );
	g_LibMusic.Play( m_UseBGMNo, LOOP );
}

/******************************************************************************/
//	BGM��~
/******************************************************************************/
void	CScriptEngine::CommandStopBGM( void )
{
	g_LibMusic.Stop( m_UseBGMNo );
}

/******************************************************************************/
//	SE�Đ�
/******************************************************************************/
void	CScriptEngine::CommandPlaySE( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�t�@�C�����擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );

	g_LibMusic.Load( m_UseSENo, GetStr );
	g_LibMusic.Play( m_UseSENo, ONE );
}

/******************************************************************************/
//	�Z�b�g�����񏉊���
/******************************************************************************/
void	CScriptEngine::CommandInitStr( void )
{
	char	GetStr[MAX_PATH];
	ZeroMemory( &GetStr, sizeof(GetStr) );

	//	�ԍ��擾
	m_SCR.Addr = ScriptGetStr( m_SCR.Addr, GetStr );
	long	StrNo = atol( GetStr );
	if( StrNo >= STR_SET_MAX )
	{
		return;
	}

	ZeroMemory( &m_SCR.SET[StrNo], sizeof(m_SCR.SET[StrNo]) );
}

/******************************************************************************/
//	�e�L�X�g�R�}���h�E���[�v
/******************************************************************************/
void	CScriptEngine::CommandTextLoop( void )
{
	long TextNo = m_UseTextNo+m_TEXT.CrtLine;
	TXT[ TextNo ].Disp = ON;

	if( TXT[ TextNo ].State )
	{
		m_TEXT.CrtLine ++;
		if( m_TEXT.CrtLine >= m_TEXT.MaxLine )
		{
			m_SCR.Mode = 0;
		}
	}

	if( MOUSE.Trg[ MOUSE_LEFT ] || KEY.Trg[ DIK_SPACE ] )
	{
		for( int i=0; i<m_TEXT.MaxLine; i++ )
		{
			TXT[ m_UseTextNo + i ].Disp = ON;
			TXT[ m_UseTextNo + i ].State = ON;
			TXT[ m_UseTextNo + i ].Step = -1;
		}
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	�I�����E���[�v
//		�����F
//				�Ȃ�
//		�߂�l�F
//				�Ȃ�
/******************************************************************************/
void	CScriptEngine::CommandSelLoop( void )
{
	long	i;

	//	���͔���E�I���L�[
	if( KEY.Trg[ DIK_UP ] )
	{
		//	��
		if( m_SCR.SelectNo > 0 )
		{
			m_SCR.SelectNo --;
		}
	}
	else if( KEY.Trg[ DIK_DOWN ] )
	{
		//	��
		if( m_SCR.SelectNo < m_TEXT.MaxLine-1 )
		{
			m_SCR.SelectNo ++;
		}
	}

	//	������̐F�ݒ�
	for( i=0; i<m_TEXT.MaxLine; i++ )
	{
		if( m_SCR.SelectNo == i )
		{
			m_TEXT.SST[i].TextColor = SETextColor::Select;
		}
		else
		{
			m_TEXT.SST[i].TextColor = SETextColor::NotSelect;
		}
	}

	//	�I�𔻒�E���肫�[
	if( KEY.Trg[ DIK_SPACE ] || MOUSE.Trg[ MOUSE_LEFT ] )
	{
		//	���x���W�����v
		m_SCR.Addr = ScriptLabelJump( m_ScrDat, m_SCR.JumpStr[m_SCR.SelectNo] );

		//	������N���A
		ZeroMemory( &m_TEXT.SST, sizeof(m_TEXT.SST) );

		//	�Z�b�g
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	�u���b�N�A�E�g�R�}���h�E���[�v
/******************************************************************************/
void	CScriptEngine::CommandBlackOutLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	�u���b�N�C���R�}���h�E���[�v
/******************************************************************************/
void	CScriptEngine::CommandBlackInLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	�z���C�g�A�E�g�R�}���h�E���[�v
/******************************************************************************/
void	CScriptEngine::CommandWhiteOutLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	�z���C�g�C���R�}���h�E���[�v
/******************************************************************************/
void	CScriptEngine::CommandWhiteInLoop( void )
{
	g_LibFade.Loop();
	if( g_LibFade.GetState() == 0 )
	{
		m_SCR.Mode = 0;
	}
}

/******************************************************************************/
//	�J�[�\���_�ŃR�}���h�E���[�v
/******************************************************************************/
void	CScriptEngine::CommandCursorBlinkLoop( void )
{
	long	i,but=NOT;
	static long	bFlag = OFF;

	for( i=0; i<256; i++ )
	{
		if( KEY.Trg[ i ] )
		{
			sprintf_s( TXT[10].Str, MAX_PATH, "%d", i );
			but = ON;
			break;
		}
	}
	if( MOUSE.Trg[ MOUSE_LEFT ] )
	{
		but = ON;
	}

	if( but == NOT )
	{
		if( m_CursorTimer >= 20 )
		{
			g_LibPol.ChangeDispRev( m_UsePolNo + POL_SCRIPT_CUR );
			m_CursorTimer = 0;
		}
		else
		{
			m_CursorTimer ++;
		}

		return;
	}

	//	�e�L�X�g�\���J�n
	m_TEXT.DispFlag = ON;

	//	�J�[�\���_�ŉ���
	m_SCR.BlinkCursor = OFF;

	//	���[�h�ύX
	m_SCR.Mode = 0;

	//	������N���A
	for( i=0; i<SCR_TEXT_LINE_MAX; i++ )
	{
		ZeroMemory( &m_TEXT.SST[i].Text, sizeof(m_TEXT.SST[i].Text) );
		g_LibText.Init( m_UseTextNo + i );
	}

	//	�J�[�\����\��
	g_LibPol.ChangeDisp( m_UsePolNo + POL_SCRIPT_CUR, OFF );
}

/******************************************************************************/
//	��ʗh�炷�R�}���h�E���[�v
/******************************************************************************/
void	CScriptEngine::CommandShakeLoop( void )
{
	long	AddValue[]=
	{
		0,3,5,8,10
	};
	long	PolNo = m_UsePolNo;
	long	CrtTime = m_SCR.SHAKE.Time / 16;
	long	Flag = m_SCR.SHAKE.State / 4;

	if( m_SCR.SHAKE.State < 16 )
	{
		if( m_SCR.SHAKE.State % 2 )
		{
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_X ) == SHAKAE_ATR_X )
			{
				g_LibPol.AddPosX( PolNo, -(m_SCR.SHAKE.Value-AddValue[Flag]) );
			}
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_Y ) == SHAKAE_ATR_Y )
			{
				g_LibPol.AddPosY( PolNo, -(m_SCR.SHAKE.Value-AddValue[Flag]) );
			}
		}
		else
		{
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_X ) == SHAKAE_ATR_X )
			{
				g_LibPol.AddPosX( PolNo, m_SCR.SHAKE.Value-AddValue[Flag] );
			}
			if( ( m_SCR.SHAKE.Atr & SHAKAE_ATR_Y ) == SHAKAE_ATR_Y )
			{
				g_LibPol.AddPosY( PolNo, m_SCR.SHAKE.Value-AddValue[Flag] );
			}
		}
		if( CrtTime <= m_SCR.SHAKE.Count )
		{
			m_SCR.SHAKE.State ++;
			m_SCR.SHAKE.Count = 0;
		}
	}
	else
	{
		g_LibPol.SetPos( PolNo, 0, 0 );

		//	���[�h�ύX
		m_SCR.Mode = 0;
	}

	m_SCR.SHAKE.Count ++;
}

/******************************************************************************/
//	��������
//		�����F
//				*pt		������|�C���^
//				ch		�������镶��
//		�߂�l�F
//				�Ȃ�
/******************************************************************************/
char	*CScriptEngine::ScriptSearchChar( char *pt, char ch )
{
	UChar	uc;

	while( 1 )
	{
		//	�R�����g�X�L�b�v
		pt = ScriptSkipComment( pt );

		if( *pt == ch )
		{
			break;
		}

		uc = (UChar)*pt;

		if( (uc<128) || (uc>=160) )
		{
			pt ++;		//	���p
		}
		else
		{
			pt += 2;	//	�S�p
		}
	}

	pt ++;
	return pt;
}

/******************************************************************************/
//	���̕�����܂ł̋󔒂̃X�L�b�v
//		�i�X�y�[�X�A�^�u�A���s�A�J���}�ANULL�ȊO�܂Łj
//		�����F
//				*pt		������|�C���^
//		�߂�l�F
//				�ύX��̕�����|�C���^
/******************************************************************************/
char	*CScriptEngine::ScriptSkipSpace( char *pt )
{
	char	c;

	while( 1 )
	{
		//	�R�����g�X�L�b�v
		pt = ScriptSkipComment( pt );

		c = *pt;

		//	�X�y�[�X�A�^�u�A���s�A�J���}����
		if( (c!=' ') && (c!=0x09) && (c!=0x0d) && (c!=0x0a) && (c!=',') && (c!=0) )
		{
			break;
		}

		pt ++;
	}

	return pt;
}

/******************************************************************************/
//	���̋󔒂܂ł̕�������X�L�b�v
//		�i�X�y�[�X�A�^�u�A���s�A�J���}�ANULL�܂Łj
//		�����F
//				*pt		������|�C���^
//		�߂�l�F
//				�ύX��̕�����|�C���^
/******************************************************************************/
char	*CScriptEngine::ScriptSkipStr( char *pt )
{
	UChar	uc;

	while( 1 )
	{
		//	�R�����g�X�L�b�v
		pt = ScriptSkipComment( pt );

		uc = (UChar)*pt;

		if( (uc<128) || (uc>=160) )
		{
			// �X�y�[�X�A�^�u�A���s�A�J���}����
			if( (uc==' ') || (uc==0x09) || (uc==0x0d) || (uc==0x0a) || (uc==',') || (uc==0) )
			{
				break;
			}

			//	���p���i�߂�
			pt ++;
		}
		else
		{
			//	�S�p���i�߂�
			pt += 2;
		}
	}

	return pt;
}

/******************************************************************************/
//	�󔒂ł�����ꂽ������̒��o
//		�i�X�y�[�X�A�^�u�A���s�A�J���}�ANULL�܂Łj
//		�����F
//				pt		������|�C���^
//				str		������i�[�A�h���X
//		�߂�l�F
//				�ύX��̕�����|�C���^
/******************************************************************************/
char	*CScriptEngine::ScriptGetStr( char *pt, char *str )
{
	char	c;

	//	�󔒃X�L�b�v
	pt = ScriptSkipSpace( pt );

	while( 1 )
	{
		c = *pt;

		// �X�y�[�X�A�^�u�A���s�A�J���}����
		if( (c == ' ') || (c == 0x09) || (c == 0x0d) || (c == 0x0a) || (c == ',') || (c == 0) )
		{
			break;
		}

		//	������̓o�^
		*str = c;

		//	�A�h���X���Z
		str ++;
		pt ++;
	}

	//	�k���R�[�h�ǉ�
	*str = 0;

	return pt;
}

/******************************************************************************/
//	�u"�v�ł�����ꂽ������̒��o
//		�����F
//				*pt		������|�C���^
//				*str	������i�[�A�h���X
//		�߂�l�F
//				���o��̕�����|�C���^
/******************************************************************************/
char	*CScriptEngine::ScriptGetStrDouble( char *pt, char *str )
{
	long	i;
	UChar	uc;

	//	�u"�v�܂ŃX�L�b�v
	pt = ScriptSearchChar( pt, '"' );

	while( 1 )
	{
		//	�u"�v�̔���
		if( *pt == '"' )
		{
			pt ++;	//	�u"�v�̎��Ɉړ�
			break;
		}

		//������̓o�^
		uc = *pt;
		if( (uc<128) || (uc>=160) )
		{
			//	���p�����R�s�[
			*str = *pt;
			pt ++;
			str ++;
		}
		else
		{
			//	�S�p�����R�s�[
			for( i=0; i<2; i++ )
			{
				*str = *pt;
				pt ++;
				str ++;
			}
		}
	}

	//	�k���R�[�h�ǉ�
	*str = 0;

	return pt;
}

/******************************************************************************/
//	���蕶����ւ̃��x���W�����v
//		�����F
//				addr	�����J�n�A�h���X
//				str		���x��������
//		�߂�l�F
//				������A�h���X
/******************************************************************************/
char	*CScriptEngine::ScriptLabelJump( char *addr, char *str )
{
	//	������܂Ń��[�v
	while( 1 )
	{
		if( ScriptStrComp( addr, str ) )
		{
			break;	//	����
		}

		addr ++;
	}

	return addr;
}

/******************************************************************************/
//	�������r
//	�i�I�[�́A�X�y�[�X�A�^�u�A���s�A�J���}�ANULL�Ŕ���j
//	�����F
//			str1	������P
//			str2	������Q
//
//	�߂�l�F
//			0:�ƍ����s�A1:�ƍ�����
/******************************************************************************/
long	CScriptEngine::ScriptStrComp( char *str1, char *str2 )
{
	long	i,ret=0;
	char	a,b;

	for( i=0; i<256; i++ )
	{
		a = *str1;
		b = *str2;

		//	�R�����g�X�L�b�v
		str1 = ScriptSkipComment( str1 );
		str2 = ScriptSkipComment( str2 );

		if( (a==' ') || (a==0x09) || (a==0x0d) || (a==0x0a) || (a==',') || (a==0) )
		{
			if( (b==' ') || (b==0x09) || (b==0x0d) || (b==0x0a) || (b==',') || (b==0) )
			{
				ret = 1;	//	�ƍ�����
			}
			break;
		}

		if ( a != b )
		{
			break;	//	�ƍ����s
		}

		str1 ++;
		str2 ++;
	}

	return ret;
}

/******************************************************************************/
//	�R�����g�X�L�b�v
/******************************************************************************/
char	*CScriptEngine::ScriptSkipComment( char *pt )
{
	long	mode;
	char	ch1,ch2;
	UChar	uc;

	while( 1 )
	{
		//	�P�����ڃ`�F�b�N
		if( *pt != '/' )
		{
			//	�R�����g�ł͂Ȃ�
			break;
		}

		//	�Q�����ڃ`�F�b�N
		if( *(pt+1) == '/' )
		{
			mode = 0;
		}
		else if( *(pt+1) == '*' )
		{
			mode = 1;
		}
		else
		{
			break;
		}

		if( mode == 0 )
		{
			//	�R�����g�I�[�͉��s�R�[�h
			ch1 = 0x0d;
			ch2 = 0x0a;
		}
		else
		{
			//	�R�����g�I�[�́u*/�v
			ch1 = '*';
			ch2 = '/';
		}

		//	�R�����g�I�[�܂ŃX�L�b�v
		while( 1 )
		{
			if( (*pt==ch1) && (*(pt+1)==ch2) )
			{
				//	���̕�����
				pt += 2;
				break;
			}

			uc = *pt;
			if( (uc<128) || (uc>=160) )
			{
				//	���p���i�߂�
				pt ++;
			}
			else
			{
				//	�S�p���i�߂�
				pt += 2;
			}
		}
	}

	return pt;
}

/******************************************************************************/
//	�Z�b�g������ύX
/******************************************************************************/
void	CScriptEngine::SwapStr( char *Str )
{
	char	*Num;
	Num = strstr( Str, "STR" );
	long	Pos = (long)(Num-Str)+1;

	char	cStrNo[8]={NULL};
	cStrNo[0] = Str[Pos+2];
	cStrNo[1] = Str[Pos+3];

	long	StrNo = atol( cStrNo );
	char	NewStr[MAX_PATH]={NULL};

	if( strlen(m_SCR.SET[StrNo].Text) == 0 )
	{
		return;
	}

	strncpy_s( NewStr, MAX_PATH, Str, Pos-1 );
	strcat_s( NewStr, MAX_PATH, m_SCR.SET[StrNo].Text );
	strcat_s( NewStr, MAX_PATH, &Str[Pos+4] );
	strcpy_s( Str, MAX_PATH, NewStr );
}
