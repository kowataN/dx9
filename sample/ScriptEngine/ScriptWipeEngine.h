/******************************************************************************/
//	�X�N���v�g���C�v
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef		__ScriptWipeEngine__
#define		__ScriptWipeEngine__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//
enum
{
	WIPE_CROSS_FADE = 0,
	WIPE_RIGHT_PAN,
	WIPE_LEFT_PAN,
	WIPE_UP_PAN,
	WIPE_DOWN_PAN,
	WIPE_MAX
};

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	�t�F�[�h�p�\����
typedef	struct
{
	long	TexNo;	//	�e�N�X�`���ԍ�
	long	PatNo;	//	�p�^�[���ԍ�
	long	PolNo;	//	�|���S���ԍ�

	double	A,R,G,B;					//	�J�����g�̐F
	double	StartA;						//	�J�n�F(A)
	double	StartR,StartG,StartB;		//	�J�n�F(�e�F����)
	double	TargetA;					//	�ڕW�F(A)
	double	TargetR,TargetG,TargetB;	//	�ڕW�F(�e�F����)
}WIPE_FADE_TBL;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CScriptWipeEngine
{
private:
	long			m_WipeNo;	//	���C�v�ԍ�
	long			m_State;	//	���
	long			m_Time;		//	��������
	long			m_Count;	//	�J�E���^

	WIPE_FADE_TBL	m_OldBg;		//	�t�F�[�h�\����
	WIPE_FADE_TBL	m_NewBg;		//	�t�F�[�h�\����

public:
	CScriptWipeEngine();
	CScriptWipeEngine(long TexNo, long PatNo, long PolNo );
	~CScriptWipeEngine();
	void	Init( long WipeNo, long Time, char *FileName );
	long	Loop( void );

private:
	void	SetCrossFade( char *FileName );
	void	LoopCrossFade( void );
};

#endif	//	__ScriptWipeEngine__