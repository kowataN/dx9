/******************************************************************************/
//	�X�N���v�g
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef		__ScriptEngine_H__
#define		__ScriptEngine_H__

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"ScriptWipeEngine.h"

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	�f�t�H���g�e��g�p�J�n�ԍ�
enum
{
	TEX_SCRIPT_BG = 0,
	TEX_SCRIPT_SYS = 5,

	PAT_SCRIPT_BG = 0,
	PAT_SCRIPT_TEXT_FLD = 5,
	PAT_SCRIPT_CUR,

	POL_SCRIPT_BG = 0,
	POL_SCRIPT_TEXT_FLD = 5,
	POL_SCRIPT_FLD_BG,
	POL_SCRIPT_CUR,

	AUDIO_SCRIPT_BGM = 0,
	AUDIO_SCRIPT_SE = 5,

	TEXT_SCRIPT_NO = 0,
};

//	������֘A
#define	SCR_TEXT_LINE_MAX	4	//	�ő�s��

//	�t���O�֘A
#define	SCR_FLAG_MAX	1000	//	�t���O�̍ő吔

//	�e�L�X�g�\���֘A
#define	TEXT_DISP_X		16
#define	TEXT_DISP_Y		404
#define	TEXT_FLD_X		0
#define	TEXT_FLD_Y		(480-84)
#define	TEXT_FLD_BG_X	(TEXT_FLD_X+8)
#define	TEXT_FLD_BG_Y	(TEXT_FLD_Y+4)
#define	TEXT_CURSOR_X	604
#define	TEXT_CURSOR_Y	(480-36)
#define	TEXT_DISP_TIME	15
#define	TEXT_PITCH(x)	(FONT_SIZE/2*x)
#define	STR_SET_MAX		16

enum
{
	Command_text,			//	0	������\��
	Command_end,			//	1	�I��
	Command_jump,			//	2	���x���W�����v
	Command_sel,			//	3	�I����
	Command_flag_on,		//	4	�t���OON
	Command_flag_off,		//	5	�t���OOFF
	Command_flag_chk,		//	6	�t���O�`�F�b�N
	Command_black_out,		//	7	�t�F�[�h�A�E�g�i�u���b�N�A�E�g�j
	Command_black_in,		//	8	�t�F�[�h�C���i�u���b�N�C���j
	Command_white_out,		//	9	�t�F�[�h�A�E�g�i�z���C�g�A�E�g�j
	Command_white_in,		//	10	�t�F�[�h�C���i�z���C�g�C���j
	Command_cursor_blink,	//	11	�J�[�\���u�����N
	Command_wait,			//	12	�x��
	Command_scr_change,		//	13	�X�N���v�g�t�@�C���ύX
	Command_text_input,		//	14	���������
	Command_set_str,		//	15	��������Z�b�g
	Command_save_point,		//	16	�Z�[�u�|�C���g
	Command_shake,			//	17	��ʂ�h�炷
	Command_load_tex,		//	18	�e�N�X�`���摜�ǂݍ���
	Command_set_pat,		//	19	�p�^�[���Z�b�g
	Command_set_pol,		//	20	�|���S���Z�b�g
	Command_load_bg,		//	21	�w�i�ǂݍ���
	Command_disp_bg,		//	22	�w�i�\���E��\��
	Command_wipe,			//	23	���C�v
	Command_play_bgm,		//	24	BGM�Đ�
	Command_stop_bgm,		//	25	BGM��~
	Command_play_se,		//	26	SE�Đ�
	Command_init_str,		//	27	�Z�b�g�����񏉊���
	CommandMax,
};

enum E_TextColor
{
	_TextColorNormal,		//	�ʏ�
	_TextColorSelect,		//	�I��
	_TextColorNotSelect,	//	��I��
};

#define	SHAKAE_ATR_X		1
#define	SHAKAE_ATR_Y		2
#define	SHAKAE_ATR_XY		(SHAKAE_ATR_X|SHAKAE_ATR_Y)

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////////////
//	������p
typedef	struct
{
	E_TextColor	TextColor;		//	������̐F
	char		Text[MAX_PATH];	//	�\��������
}SE_STR_TBL;

/////////////////////////////////////////////////
//	�w�i�h�炵�p�\����
typedef	struct
{
	long	Value;	//	���Z�l
	long	Time;	//	�h�炷���ԁi�t���[�����j
	long	Count;	//	�J�E���^
	long	State;	//	���
	UChar	Atr;	//	�A�g���r���[�g(0:X�A1:Y�A2:XY)
}SE_SHAKE_TBL;

/////////////////////////////////////////////////
//	������Ǘ��p�\����
typedef	struct
{
	long		DispFlag;	//	�e�L�X�g�\���iON:�\���AOFF:��~�j
	long		MaxLine;	//	������̍s��
	long		CrtLine;	//	���݂̍s��
	long		CrtTime;	//	�J�����g����
	long		Count;		//	�J�E���^
	long		State;		//	���
	SE_STR_TBL	SST[SCR_TEXT_LINE_MAX];
}SE_TEXT_TBL;

/////////////////////////////////////////////////
//	�X�N���v�g�Ǘ��\����
typedef	struct
{
	long			Mode;			//	���[�h�i0:�Z�b�g�A1:���[�v�j
	long			WaitFlag;		//	�x���iON:�x������AOFF:�x���Ȃ��j
	long			BlinkCursor;	//	�J�[�\���_�ŁiON:�_�Œ��AOFF:�_�łȂ��j

	long			CmdNo;			//	�R�}���h�ԍ�
	long			SelectNo;		//	�I�����ԍ�

	char			JumpStr[SCR_TEXT_LINE_MAX][256];	//	��ѐ惉�x��
	char			EventFlag[SCR_FLAG_MAX];			//	�C�x���g�t���O
	SE_STR_TBL		SET[STR_SET_MAX];
	SE_SHAKE_TBL	SHAKE;			//	��ʗh�炵
	char			*Addr;			//	���݂̃X�N���v�g�A�h���X
}SCRIPT_TBL;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CScriptEngine
{
protected:
	char				m_ScrDat[100*1024];
	SCRIPT_TBL			m_SCR;
	SE_TEXT_TBL			m_TEXT;			//	������Ǘ��\����
	long				m_CursorTimer;
	long				m_UseTextureNo;
	long				m_UsePatNo;
	long				m_UsePolNo;
	long				m_UseTextNo;
	long				m_UseBGMNo;
	long				m_UseSENo;
	CScriptWipeEngine*	m_Wipe;

public:
	CScriptEngine();
	CScriptEngine( long TextureNo, long PatNo, long PolNo, long TextNo, long BGMNo, long SENo );
	~CScriptEngine();
	void	Init( char *FileName );
	void	Main( void );	//	�X�N���v�g�G���W�����C��

protected:
	/////////////////////////////////////////////////
	//	�G���W��
	void	Set( void );			//	�X�N���v�g�Z�b�g
	void	Loop( void );			//	�X�N���g�v�g���[�v
	long	GetNextCommand( void );	//	���̃R�}���h�擾

	/////////////////////////////////////////////////
	//	�t���[�����[�v
	void	DispTextLoop( void );

	/////////////////////////////////////////////////
	//	�R�}���h
	void	CommandTextSet( void );			//	�e�L�X�g�R�}���h�E�Z�b�g
	void	CommandEndSet( void );			//	�I���R�}���h�E�Z�b�g
	void	CommandJumpSet( void );			//	���x���W�����v�R�}���h�E�Z�b�g
	void	CommandSelSet( void );			//	�I�����R�}���h�E�Z�b�g
	void	CommandFlagOnSet( void );		//	�t���O�R�}���hON�E�Z�b�g
	void	CommandFlagOffSet( void );		//	�t���O�R�}���hOFF�E�Z�b�g
	void	CommandFlagChkSet( void );		//	�t���O�R�}���h�`�F�b�N�E�Z�b�g
	void	CommandBlackOutSet( void );		//	�u���b�N�A�E�g�R�}���h�E�Z�b�g
	void	CommandBlackInSet( void );		//	�u���b�N�C���R�}���h�E�Z�b�g
	void	CommandWhiteOutSet( void );		//	�z���C�g�A�E�g�R�}���h�E�Z�b�g
	void	CommandWhiteInSet( void );		//	�z���C�g�C���R�}���h�E�Z�b�g
	void	CommandCursorBlinkSet( void );	//	�J�[�\���_�ŃR�}���h�E�Z�b�g
	void	CommandWaitSet( void );			//	�x���R�}���h�E�Z�b�g
	void	CommandScriptChangeSet( void );	//	�X�N���v�g�t�@�C���ύX�R�}���h�E�Z�b�g
	void	CommandTextInputSet( void );	//	�e�L�X�g���̓R�}���h�E�Z�b�g
	void	CommandSetStrSet( void );		//	������Z�b�g�R�}���h�E�Z�b�g
	void	CommandSavePointSet( void );	//	�Z�[�u�|�C���g�R�}���h�E�Z�b�g
	void	CommandShakeSet( void );		//	��ʗh�炷�R�}���h�E�Z�b�g
	void	CommandLoadTex( void );			//	�e�N�X�`���摜�ǂݍ���
	void	CommandSetPat( void );			//	�p�^�[���Z�b�g
	void	CommandSetPol( void );			//	�|���S���Z�b�g
	void	CommandLoadBg( void );			//	�w�i�ǂݍ���
	void	CommandDispBg( void );			//	�w�i�\���E��\��
	void	CommandWipe( void );			//	���C�v
	void	CommandPlayBGM( void );			//	BGM�Đ�
	void	CommandStopBGM( void );			//	BGM��~
	void	CommandPlaySE( void );			//	SE�Đ�
	void	CommandInitStr( void );			//	�Z�b�g�����񏉊���

	void	CommandTextLoop( void );		//	�e�L�X�g�R�}���h�E���[�v
	void	CommandSelLoop( void );			//	�I�����R�}���h�E���[�v
	void	CommandBlackOutLoop( void );	//	�u���b�N�A�E�g�R�}���h�E���[�v
	void	CommandBlackInLoop( void );		//	�u���b�N�C���R�}���h�E���[�v
	void	CommandWhiteOutLoop( void );	//	�z���C�g�A�E�g�R�}���h�E���[�v
	void	CommandWhiteInLoop( void );		//	�z���C�g�C���R�}���h�E���[�v
	void	CommandCursorBlinkLoop( void );	//	�J�[�\���_�ŃR�}���h�E���[�v
	void	CommandShakeLoop( void );		//	��ʗh�炷�R�}���h�E���[�v

	/////////////////////////////////////////////////
	//	�X�N���v�g�ėp
	char	*ScriptSearchChar( char *pt, char ch );		//	��������
	char	*ScriptSkipSpace( char *pt );				//	�󔒂̃X�L�b�v
	char	*ScriptSkipStr( char *pt );					//	������X�L�b�v
	char	*ScriptGetStr( char *pt, char *str );		//	�����擾
	char	*ScriptGetStrDouble( char *pt, char *str );	//	������擾
	char	*ScriptLabelJump( char *addr, char *str );	//	���x���W�����v
	long	ScriptStrComp( char *str1, char *str2 );	//	�������r
	char	*ScriptSkipComment( char *pt );				//	�R�����g�X�L�b�v
	void	SwapStr( char *Str );						//	�Z�b�g������ύX
};

class SETextColor
{
public:
	static const E_TextColor Normal = _TextColorNormal;
	static const E_TextColor Select = _TextColorSelect;
	static const E_TextColor NotSelect = _TextColorNotSelect;
};

#endif	//	__ScriptEngine_H__
