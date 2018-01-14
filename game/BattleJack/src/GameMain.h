/******************************************************************************/
//	�o�g���W���b�N
//		�Q�[�����C���w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__GameMain_H__
#define	__GameMain_H__

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	���C�����[�h�t���O
enum
{
	MainModeInit,
	MainModeMain,
	MainModeEnd,
};

////////////////////////////////////////
//	�Q�[�����[�h�t���O
enum
{
	GameModeInit,
	GameModeStart,
	GameModeCardDraw,
	GameModeSelect,
	GameModeCpuSelect,
	GameModeFldCardSet,
	GameModeDispSum,
	GameModeEnd,
};

//	�e�N�X�`���ԍ�
#define	TEX_GAME_BG				0
#define	TEX_GAME_SYSTEM			1
#define	TEX_GAME_CARD			2

//	�p�^�[���ԍ�
#define	PAT_GAME_BG				0
#define	PAT_GAME_DECK_PLAYER	5
#define	PAT_GAME_DECK_CPU		(PAT_GAME_DECK_PLAYER+1)
#define	PAT_GAME_NUMBER0		10
#define	PAT_GAME_NUMBER1		(PAT_GAME_NUMBER0+1)
#define	PAT_GAME_NUMBER2		(PAT_GAME_NUMBER1+1)
#define	PAT_GAME_NUMBER3		(PAT_GAME_NUMBER2+1)
#define	PAT_GAME_NUMBER4		(PAT_GAME_NUMBER3+1)
#define	PAT_GAME_NUMBER5		(PAT_GAME_NUMBER4+1)
#define	PAT_GAME_NUMBER6		(PAT_GAME_NUMBER5+1)
#define	PAT_GAME_NUMBER7		(PAT_GAME_NUMBER6+1)
#define	PAT_GAME_NUMBER8		(PAT_GAME_NUMBER7+1)
#define	PAT_GAME_NUMBER9		(PAT_GAME_NUMBER8+1)
#define	PAT_GAME_PLUS			(PAT_GAME_NUMBER9+1)
#define	PAT_GAME_MINUS			(PAT_GAME_PLUS+1)
#define	PAT_GAME_NUMBER_NONE	(PAT_GAME_MINUS+1)
#define	PAT_GAME_HEART			30
#define	PAT_GAME_DIAMOND		50
#define	PAT_GAME_CLUB			70
#define	PAT_GAME_SPADE			90
#define	PAT_GAME_BUT_OK			110
#define	PAT_GAME_BUT_CANCEL		(PAT_GAME_BUT_OK+1)
#define	PAT_GAME_WIN			120
#define	PAT_GAME_LOSE			(PAT_GAME_WIN+1)
#define	PAT_GAME_DRAW			(PAT_GAME_LOSE+1)
#define	PAT_GAME_JUDGE_FLD		(PAT_GAME_DRAW+1)
#define	PAT_GAME_COUNT_STR		130
#define	PAT_GAME_END_STR		135

//	�|���S���ԍ�
#define	POL_GAME_BG				0
#define	POL_GAME_DECK_PLAYER	5
#define	POL_GAME_DECK_CPU		(POL_GAME_DECK_PLAYER+1)
#define	POL_GAME_POINT_PLAYER	10
#define	POL_GAME_POINT_CPU		20
#define	POL_GAME_HAND_PLAYER	40
#define	POL_GAME_HAND_CPU		45
#define	POL_GAME_BUT_OK			60
#define	POL_GAME_BUT_CANCEL		(POL_GAME_BUT_OK+1)
#define	POL_GAME_BUT_FLD		(POL_GAME_BUT_CANCEL+1)
#define	POL_GAME_CALC_PLAYER	70
#define	POL_GAME_CALC_CPU		75
#define	POL_GAME_JUDGE			80
#define	POL_GAME_JUDGE_FLD		(POL_GAME_JUDGE+1)
#define	POL_GAME_PLAYER_OPT		100
#define	POL_GAME_PLAYER_ADD1	(POL_GAME_PLAYER_OPT+1)
#define	POL_GAME_PLAYER_ADD2	(POL_GAME_PLAYER_ADD1+1)
#define	POL_GAME_PLAYER_ADD3	(POL_GAME_PLAYER_ADD2+1)
#define	POL_GAME_CPU_OPT		105
#define	POL_GAME_CPU_ADD1		(POL_GAME_CPU_OPT+1)
#define	POL_GAME_CPU_ADD2		(POL_GAME_CPU_ADD1+1)
#define	POL_GAME_CPU_ADD3		(POL_GAME_CPU_ADD2+1)
#define	POL_GAME_COUNT1			110
#define	POL_GAME_COUNT2			(POL_GAME_COUNT1+1)
#define	POL_GAME_COUNT_STR		(POL_GAME_COUNT2+1)
#define	POL_GAME_COUNT_FLD		(POL_GAME_COUNT_STR+1)
#define	POL_GAME_END_STR		115

////////////////////////////////////////
//	�g�p�J�[�h��(1�`13)
#define	USE_NUMBER		13
//	�g�p�}�[�N��
#define	USE_MARK		4

////////////////////////////////////////
//	�R�D�ő吔
#define	CARD_MAX		(USE_NUMBER*USE_MARK)

enum
{	//	�v���C���[
	GamePlayer =0,
	//	CPU
	GameCpu,
	//	�v���C�ő吔
	MaxPlayer
};

//	��D�ő吔
#define	HAND_NUM_MAX	4

////////////////////////////////////////
//	�p�^�[���T�C�Y

//	�J�[�h�T�C�Y
#define	CARD_SIZE_W			48
#define	CARD_SIZE_H			56
//	�v���C���[�R�D�T�C�Y
#define	DECK_PLAYER_PAT_X	0
#define	DECK_PLAYER_PAT_Y	224
#define	DECK_PLAYER_PAT_W	CARD_SIZE_W
#define	DECK_PLAYER_PAT_H	CARD_SIZE_H
//	CPU�R�D�T�C�Y
#define	DECK_CPU_PAT_X		48
#define	DECK_CPU_PAT_Y		224
#define	DECK_CPU_PAT_W		CARD_SIZE_W
#define	DECK_CPU_PAT_H		CARD_SIZE_H
//	�����T�C�Y
#define	NUMBER_SIZE_W		16
#define	NUMBER_SIZE_H		24
//	�{�^���T�C�Y
#define	BUT_OK_PAT_X		0
#define	BUT_OK_PAT_Y		64
#define	BUT_OK_PAT_W		88
#define	BUT_OK_PAT_H		40
#define	BUT_CANSEL_PAT_X	88
#define	BUT_CANSEL_PAT_Y	64
#define	BUT_CANSEL_PAT_W	88
#define	BUT_CANSEL_PAT_H	40
#define	BUT_FLD_SIZE_W		208
#define	BUT_FLD_SIZE_H		56
//	���s�p�T�C�Y
#define	WIN_PAT_X			0
#define	WIN_PAT_Y			104
#define	WIN_PAT_W			120
#define	WIN_PAT_H			32
#define	LOSE_PAT_X			0
#define	LOSE_PAT_Y			136
#define	LOSE_PAT_W			120
#define	LOSE_PAT_H			32
#define	DRAW_PAT_X			0
#define	DRAW_PAT_Y			168
#define	DRAW_PAT_W			120
#define	DRAW_PAT_H			32
#define	WIN_FLD_PAT_X		0
#define	WIN_FLD_PAT_Y		200
#define	WIN_FLD_PAT_W		216
#define	WIN_FLD_PAT_H		32
//	���
#define	SENME_PAT_X			192
#define	SENME_PAT_Y			0
#define	SENME_PAT_W			48
#define	SENME_PAT_H			24
//	�I������
#define	END_STR_PAT_X		0
#define	END_STR_PAT_Y		24
#define	END_STR_PAT_W		288
#define	END_STR_PAT_H		40

////////////////////////////////////////
//	�\�����W

//	�R�D
#define	DECK_PLAYER_POS_X	488
#define	DECK_PLAYER_POS_Y	344
#define	DECK_CPU_POS_X		104
#define	DECK_CPU_POS_Y		80
//	�|�C���g�\��
#define	POINT_PLAYER_POS_X	104
#define	POINT_PLAYER_POS_Y	256
#define	MINUS_PLAYER_POS_X	88
#define	MINUS_PLAYER_POS_Y	256
#define	POINT_CPU_POS_X		576
#define	POINT_CPU_POS_Y		200
#define	MINUS_CPU_POS_X		560
#define	MINUS_CPU_POS_Y		200
//	�v���C���[�|�C���g���Z�l
#define	OPT_PLAYER_POS_X	88
#define	OPT_PLAYER_POS_Y	224
#define	ADD1_PLAYER_POS_X	104
#define	ADD1_PLAYER_POS_Y	(OPT_PLAYER_POS_Y)
#define	ADD2_PLAYER_POS_X	120
#define	ADD2_PLAYER_POS_Y	(OPT_PLAYER_POS_Y)
#define	ADD3_PLAYER_POS_X	136
#define	ADD3_PLAYER_POS_Y	(OPT_PLAYER_POS_Y)
//	CPU�|�C���g���Z�l
#define	OPT_CPU_POS_X		560
#define	OPT_CPU_POS_Y		232
#define	ADD1_CPU_POS_X		576
#define	ADD1_CPU_POS_Y		(OPT_CPU_POS_Y)
#define	ADD2_CPU_POS_X		592
#define	ADD2_CPU_POS_Y		(OPT_CPU_POS_Y)
#define	ADD3_CPU_POS_X		608
#define	ADD3_CPU_POS_Y		(OPT_CPU_POS_Y)
//	OK�ECENCEL�_�C�A���O
#define	BUT_FLD_POS_X		216
#define	BUT_FLD_POS_Y		296
#define	BUT_OK_POS_X		224
#define	BUT_OK_POS_Y		304
#define	BUT_CANCEL_POS_X	328
#define	BUT_CANCEL_POS_Y	(BUT_OK_POS_Y)
//	�J�[�h���v�l
#define	CALC_PLAYER_POS_X	304
#define	CALC_PLAYER_POS_Y	256
#define	CALC_CPU_POS_X		304
#define	CALC_CPU_POS_Y		200
//	���s
#define	WIN_POS_X			256
#define	WIN_POS_Y			224
#define	WIN_FLD_POS_X		208
#define	WIN_FLD_POS_Y		224
//	���
#define	COUNT_POS_X			280
#define	COUNT_POS_Y			228

////////////////////////////////////////
//	�f�o�b�O�t���O
#ifdef	_DEBUG
#define	__DEBUG__
#endif

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	�Q�[���Ǘ��\����
typedef	struct
{
	long		MainMode;				//	���C�����[�h
	long		GameMode;				//	�Q�[�����[�h
	long		Point[MaxPlayer];		//	�|�C���g
	long		PointAdd[MaxPlayer];	//	�|�C���g���Z�l
	long		CardMax[MaxPlayer];		//	�R�D�ő吔
	UChar		Judge;					//	����t���O
	long		GameCount;				//	�Q�[����
	long		GameModeState;			//	�ėp�t���O
}GAME_MNG_TBL;

////////////////////////////////////////
//	�R�D�Ǘ��\����
typedef	struct
{
	long		Mark;					//	�}�[�N
	long		Number;					//	����
}DECK_TBL;

////////////////////////////////////////
//	�L�����N�^�[�Ǘ��\����
typedef	struct
{
	long		DeckIndex;
	long		HandNumber;
	long		StateFlag[HAND_NUM_MAX];	//	�ėp��ԃt���O(Player�p)
	long		SelCardCt;
	long		SelectCard[HAND_NUM_MAX];
	double		HandCardRotR[HAND_NUM_MAX];
	DECK_TBL	Deck[CARD_MAX];
}CHARA_TBL;

#endif	// __GameMain_H__

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
