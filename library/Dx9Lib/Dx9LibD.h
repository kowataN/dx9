/******************************************************************************/
//	DirectX9���C�u����
//		�萔
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//	�萔�i�ύX�j

////////////////////////////////////////
//	�E�C���h�E�֘A

//	�A�v���P�[�V�����̖��O
#define	APP_NAME	"GameMain"

//	��ʂ̃T�C�Y
#define	WIDTH		640
#define	HEIGHT		480

//	�E�C���h�E���[�h
//		�P�F�t���X�N���[�����[�h
//		�O�F�E�C���h�E���[�h
#define	SCREEN_MODE		0

//	�E�C���h�E���[�h�؂�ւ�
//		ON �F�؂�ւ��L��
//		OFF�F�؂�ւ�����
#define	CHANGE_SCREEN_MODE	OFF

//	�I���m�F
//		ON �F����
//		OFF�F�Ȃ�
#define	WIN_CLOSE	OFF

//	�E�C���h�E�N���A�J���[
#define	WIN_CLEAR_R		0
#define	WIN_CLEAR_G		0
#define	WIN_CLEAR_B		0

//	���d�@������t���O
//		ON �F���d�N���h�~
//		OFF�F���d�N������
#define	MULTIPLEX_FLAG	OFF

//	�ǂݍ��݉\�ő�T�C�Y�i�f�t�H�łP�O�l�j
#define	FREE_WORK_SIZE	(10*1024*1024)

////////////////////////////////////////
//	�O���t�B�b�N�֘A

//	�e�N�X�`���ő吔
#define	TEX_MAX		128

//	�p�^�[���ő吔
#define	PAT_MAX		512

//	�X�v���C�g�ő吔
#define	SP_MAX		2048

//	���C���ő吔
#define	LINE_MAX	1024

//	�|���S���ő吔
#define	POL_MAX		2048

//	�����F	ARGB
#define	TRANS		0xFF00FF00

////////////////////////////////////////
//	�C���v�b�g�֘A

//	�}�E�X�J�[�\���\���؂�ւ�
//		TRUE�F�\��
//		FALAE�F��\��
#define	CURSOR_DISP		TRUE

//	�W���C�X�e�B�b�N�̎g�p�t���O
//		ON�F�g�p
//		OFF�F���g�p
#define	JOY_USE		ON

//	�W���C�X�e�B�b�N�̍ő吔
#define	JOY_MAX		4

//	�W���C�X�e�B�b�N�̃{�^���ő吔
#define	BUTTON_MAX	(JOY_BUT_MAX)

////////////////////////////////////////
//	�I�[�f�B�I�֘A

//	�I�[�f�B�I�̍ő吔
#define	AUDIO_MAX		16

//	�I�[�f�B�I�t�@�C���̍ő吔
#define	AUDIO_SIZE_MAX	(2*1024*1024)	//	�Q�l�܂�

////////////////////////////////////////
//	�e�L�X�g�֘A

//	�e�L�X�g�ő吔
#define	TXT_MAX		1024

//	������
#define	TXT_SIZE	1024

//	�t�H���g�T�C�Y
#define	FONT_SIZE	14

//	�t�H���g
#define	FONT_TYPE	"�l�r�S�V�b�N"

////////////////////////////////////////
//	�v���C�֘A

//	IP�A�h���X
#define	CONNECT_IP		"114.167.198.222"

//	�|�[�g
#define	CONNECT_PORT	30000

////////////////////////////////////////
//	���̑�

//	�t���[����
#define	FPS		60

//	FPS�\���t���O
//		ON�F�\��
//		OFF�F��\��
#define	FPS_DISP	ON

////////////////////////////////////////////////////////////////////////////////
//	�萔�i�ύX�֎~�j

////////////////////////////////////////
//	�O���t�B�b�N�֘A

//	�o�b�t�@�T�C�Y
#define	DIDEVICE_BUFFERSIZE	100

//	�A�g���r���[�g
#define	ATR_NONE	0	//	�������Ȃ�
#define	ATR_REV_W	1	//	���E���]
#define	ATR_REV_H	2	//	�㉺���]
#define	ATR_ALL		( ATR_REV_W | ATR_REV_H )	//	���E�㉺���]
#define	ATR_BOX		1

////////////////////////////////////////
//	���̑�

//	�~����
#define	PI	3.1415926

//	�t���O�p
#define	ON	1
#define	OFF	0
#define	NOT	(-1)

////////////////////////////////////////
//	unsigned�^
typedef	unsigned char	UChar;
typedef	unsigned short	UShort;
typedef	unsigned long	ULong;

////////////////////////////////////////////////////////////////////////////////
//	�}�N��
#define	SAFE_DELETE(x)		if(x){delete x; x = NULL;};
#define	SAFE_DELETE_ARY(x)	if(x){delete [] x; x = NULL;};
#define	SAFE_RELEASE(X)		if(X){X->Release();X=NULL;}
#define	FVF_TLVERTEX		(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

////////////////////////////////////////////////////////////////////////////////
//	��`
enum	E_WindowStyle
{
	WindowMode,
	FullScreenMode
};
