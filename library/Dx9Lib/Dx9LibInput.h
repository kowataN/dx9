/******************************************************************************/
//	DirectX9���C�u����
//		�C���v�b�g
/******************************************************************************/

#ifndef	__Dx9LibInput_H__
#define	__Dx9LibInput_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//
enum
{
	//	�}�E�X
	MOUSE_LEFT = 0,	//	��
	MOUSE_RIGHT,	//	�E
	MOUSE_CENTER,	//	�Z���^�[

	//	�W���C�X�e�B�b�N
	JOY_UP = 0,		//	��
	JOY_DOWN,		//	��
	JOY_LEFT,		//	��
	JOY_RIGHT,		//	�E
	JOY_BUT1,		//	�{�^���P
	JOY_BUT2,		//	�{�^���Q
	JOY_BUT3,		//	�{�^���R
	JOY_BUT4,		//	�{�^���S
	JOY_BUT5,		//	�{�^���T
	JOY_BUT6,		//	�{�^���U
	JOY_BUT7,		//	�{�^���V
	JOY_BUT8,		//	�{�^���W
	JOY_BUT9,		//	�{�^���X
	JOY_BUT10,		//	�{�^���P�O
	JOY_BUT11,		//	�{�^���P�P
	JOY_BUT12,		//	�{�^���P�Q
	JOY_BUT_MAX,	//	�ő�l
};

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	�}�E�X�p�\����
typedef	struct
{
	long	PosX,PosY;					//	���W
	long	PosXBk,PosYBk;				//	�o�b�N�A�b�v���W
	long	MoveX,MoveY;				//	�ړ���
	UChar	Trg[3];						//	�g���K
	UChar	Push[3];					//	�v�b�V��
	long	Count[3];					//	�v�b�V���J�E���g
	UChar	WheelUp,WheelDown;			//	�z�C�[��
	UChar	WheelUpCt,WheelDownCt;		//	�z�C�[���J�E���g
	RECT	Rect;						//	�̈�
}ST_MouseInfo;

////////////////////////////////////////
//	�L�[�{�[�h�p�\����
typedef	struct
{
	UChar	Flag;						//	�t���O
	UChar	Trg[256];					//	�g���K
	UChar	Push[256];					//	�v�b�V��
	long	Count[256];					//	�v�b�V���J�E���g
}ST_KeybordInfo;

////////////////////////////////////////
//	�W���C�X�e�B�b�N�p�\����
typedef	struct
{
	UChar	Use;						//	�g�p�t���O
	UChar	Trg[BUTTON_MAX];			//	�g���K
	UChar	Push[BUTTON_MAX];			//	�v�b�V��
	long	Count[BUTTON_MAX];			//	�v�b�V���J�E���g
	long	ButtonMax;
}ST_JoystickInfo;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CDx9LibInput
{
private:
	LPDIRECTINPUTDEVICE8	m_DIDevMs;				//	�}�E�X�p�f�o�C�X
	LPDIRECTINPUTDEVICE8	m_DIDevKey;				//	�L�[�{�[�h�p�f�o�C�X
	POINT					m_MousePoint;			//	�}�E�X�̍��W
	DIDEVCAPS				m_DICaps;				//	�f�o�C�X�\��
	long					m_JoyFlag;				//	�W���C�X�e�B�b�N�̎g�p�t���O

public:
	CDx9LibInput();
	~CDx9LibInput();
	bool	Initialize( HINSTANCE hInst );
	void	InputAcquire( void );
	void	InputUnacquire( void );
	void	GetInputData( short Active );
	long	GetJoyNo( void );
	void	SetJoyNo( long JoyNo );
	long	GetJoyUseFlag( void );
	void	Release( void );

private:
	BOOL	InitMouseDevice( void );
	BOOL	InitKeyboardDevice( void );
	BOOL	InitJoystickDevice( void );

	void	InitMouse( void );
	void	InitKeybord( void );
	void	InitJoystick( void );

	BOOL	GetMouseData( short Active );
	BOOL	GetKeybordData( short Active );
	BOOL	GetJoystickData( short Active );
};

#endif	//	__Dx9LibInput_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
extern	ST_MouseInfo	MOUSE;			//	�}�E�X
extern	ST_KeybordInfo	KEY;			//	�L�[�{�[�h
extern	ST_JoystickInfo JOY[JOY_MAX];	//	�W���C�X�e�B�b�N
