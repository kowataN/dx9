/******************************************************************************/
//	�o�g���W���b�N
//		�Q�[�����C���\�[�X�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	<process.h>
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./GameMain.h"
#include	"./StopWatch.h"

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
GAME_MNG_TBL	g_GameMng =
{
	MainModeInit,
	GameModeInit,
	20,20,
	0,0,
	CARD_MAX,CARD_MAX,
	NOT,
	1,
	0,
};

DECK_TBL	DeckCpu[CARD_MAX];

//	�v���C���[��D�\�����W
double	HandPlayerPos[][4][2]=
{
	{	//	1��
		296+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2)
	},
	{	//	2��
		268+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2),
		324+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2)
	},
	{	//	3��
		240+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2),
		296+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2),
		352+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2)
	},
	{	//	4��
		212+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2),
		268+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2),
		324+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2),
		380+(CARD_SIZE_W/2),416+(CARD_SIZE_H/2)
	},
};

//	CPU�J�[�h��D�\�����W
double	HandCpuPos[][4][2] =
{
	{	//	1��
		296+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2)
	},
	{	//	2��
		268+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2),
		324+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2)
	},
	{	//	3��
		240+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2),
		296+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2),
		352+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2)
	},
	{	//	4��
		212+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2),
		268+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2),
		324+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2),
		380+(CARD_SIZE_W/2),8+(CARD_SIZE_H/2)
	},
};

//	�v���C���[�J�[�h�����蔻��
long	HitPlayerCard[][4][4] =
{
	{	//	1��
		296,416,344,472,
	},
	{	//	2��
		268,416,316,472,
		324,416,372,472,
	},
	{	//	3��
		240,416,288,472,
		296,416,344,472,
		352,415,400,472,
	},
	{	//	4��
		212,416,260,472,
		268,416,316,472,
		324,416,372,472,
		380,416,428,472,
	},
};

//	�v���C���[�t�B�[���h�\�����W
double	FldPlayerPos[][4][2] =
{
	{	//	2��
		268,296, 324,296,
	},
	{	//	3��
		240,296, 296,296, 352,296,
	},
	{	//	4��
		212,296, 268,296, 324,296, 380,296,
	},
};

//	CPU�t�B�[���h�\�����W
double	FldCpuPos[][4][2] =
{
	{	//	2��
		268,128, 324,128,
	},
	{	//	3��
		240,128, 296,128, 352,128,
	},
	{	//	4��
		212,128, 268,128, 324,128, 380,128,
	},
};

CHARA_TBL	Chara[2];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
void	MainInit( void );
void	MainEnd( void );
void	GameLoop( void );
void	GameInit( void );
void	GameStart( void );
void	GameCardDraw( void );
void	GameSelectCard( void );
void	GameSelectCardCpu( void );
void	GameFldCardSet( void );
void	DispCardCalc( void );
long	DispCardCalcOne( long UserFlag, long PolNo );
long	CardCalcOne( long UserFlag );
void	GameMain( void );
void	GameEnd( void );
void	InitChara( long Index );
void	InitDeck( DECK_TBL* Deck );
void	DispPoint( void );
void	DispPointOne( long UserFlag, long PolNo );
void	PlayerCardDraw( long Num );
void	CpuCardDraw( long Num );
long	CardCalc( long *SelTbl );
void	PlayerCardFldSet( long Num, long FldIndex );
void	CpuCardFldSet( long Num, long FldIndex );
void	DispAddPointOne( long UserFlag, long PolNo );



/******************************************************************************/
//	���C�����[�v
/******************************************************************************/
void	MainLoop( void )
{
	switch( g_GameMng.MainMode )
	{
		case	MainModeInit:
			MainInit();
			g_GameMng.MainMode = MainModeMain;

		case	MainModeMain:
			GameLoop();
			break;

		case	MainModeEnd:
			MainEnd();
			break;
	}
}

/******************************************************************************/
//	���C��������
/******************************************************************************/
void	MainInit( void )
{
}

/******************************************************************************/
//	���C���I������
/******************************************************************************/
void	MainEnd( void )
{
}

/******************************************************************************/
//	�Q�[�����[�v
/******************************************************************************/
void	GameLoop( void )
{
	switch( g_GameMng.GameMode )
	{
		//	�Q�[��������
		case	GameModeInit:
			GameInit();
			g_GameMng.GameMode = GameModeStart;

		//	�Q�[���J�n
		case	GameModeStart:
			GameStart();
			break;

		//	�J�[�h��z��
		case	GameModeCardDraw:
			GameCardDraw();
			break;

		//	�v���C���[�J�[�h�I��
		case	GameModeSelect:
			GameSelectCard();
			break;

		//	CPU�J�[�h�I��
		case	GameModeCpuSelect:
			GameSelectCardCpu();
			break;

		//	�t�B�[���h�ɃJ�[�h�ݒ�
		case	GameModeFldCardSet:
			GameFldCardSet();
			break;

		//	����
		case	GameModeDispSum:
			DispCardCalc();
			break;

		//	�Q�[���I��
		case	GameModeEnd:
			GameEnd();
			break;
	}
}

/******************************************************************************/
//	�Q�[��������
/******************************************************************************/
void	GameInit( void )
{
	g_LibPol.InitAll();
	g_GameMng.GameModeState = 0;
	g_GameMng.GameCount = 1;
	for( int i=0; i<MaxPlayer; i++ )
	{
		g_GameMng.CardMax[i] = CARD_MAX;
		g_GameMng.Point[i] = 20;
		g_GameMng.PointAdd[i] = 0;
	}

	/////////////////////////////////////////////////
	//	�e�N�X�`���ǂݍ���

	//	�w�i
	LoadTexture( TEX_GAME_BG, "img\\bg.bmp" );
	//	�V�X�e��
	LoadTexture( TEX_GAME_SYSTEM, "img\\system.bmp" );
	//	�J�[�h
	LoadTexture( TEX_GAME_CARD, "img\\card.bmp" );

	/////////////////////////////////////////////////
	//	�p�^�[���ݒ�

	//	�w�i
	SetPattern( PAT_GAME_BG, TEX_GAME_BG, 0,0, 640,480 );

	//	�R�D
	SetPattern( PAT_GAME_DECK_PLAYER, TEX_GAME_CARD,
		DECK_PLAYER_PAT_X,DECK_PLAYER_PAT_Y,
		DECK_PLAYER_PAT_W,DECK_PLAYER_PAT_H );
	SetPattern( POL_GAME_DECK_CPU, TEX_GAME_CARD,
		DECK_CPU_PAT_X,DECK_CPU_PAT_Y,
		DECK_CPU_PAT_W,DECK_CPU_PAT_H );

	//	�{�^��
	SetPattern( PAT_GAME_BUT_OK, TEX_GAME_SYSTEM,
		BUT_OK_PAT_X,BUT_OK_PAT_Y,
		BUT_OK_PAT_W,BUT_OK_PAT_H );
	SetPattern( PAT_GAME_BUT_CANCEL, TEX_GAME_SYSTEM,
		BUT_CANSEL_PAT_X,BUT_CANSEL_PAT_Y,
		BUT_CANSEL_PAT_W,BUT_CANSEL_PAT_H );

	//	�|�C���g
	for( int i=0; i<13; i++ )
	{
		SetPattern( PAT_GAME_NUMBER0 + i, TEX_GAME_SYSTEM,
			(NUMBER_SIZE_W*i), 0, NUMBER_SIZE_W,NUMBER_SIZE_H );
	}

	//	�J�[�h
	for( int i=0; i<USE_NUMBER; i++ )
	{
		//	�n�[�g
		SetPattern( PAT_GAME_HEART + i,TEX_GAME_CARD,
			CARD_SIZE_W*i, CARD_SIZE_H*0, CARD_SIZE_W,CARD_SIZE_H );
		//	�_�C��
		SetPattern( PAT_GAME_DIAMOND + i,TEX_GAME_CARD,
			CARD_SIZE_W*i, CARD_SIZE_H*1, CARD_SIZE_W,CARD_SIZE_H );
		//	�N���u
		SetPattern( PAT_GAME_CLUB + i,TEX_GAME_CARD,
			CARD_SIZE_W*i, CARD_SIZE_H*2, CARD_SIZE_W,CARD_SIZE_H );
		//	�X�y�[�h
		SetPattern( PAT_GAME_SPADE + i,TEX_GAME_CARD,
			CARD_SIZE_W*i, CARD_SIZE_H*3, CARD_SIZE_W,CARD_SIZE_H );
	}

	//	���s�p
	SetPattern( PAT_GAME_WIN, TEX_GAME_SYSTEM, WIN_PAT_X,WIN_PAT_Y, WIN_PAT_W,WIN_PAT_H );
	SetPattern( PAT_GAME_LOSE, TEX_GAME_SYSTEM, LOSE_PAT_X,LOSE_PAT_Y, LOSE_PAT_W,LOSE_PAT_H );
	SetPattern( PAT_GAME_DRAW, TEX_GAME_SYSTEM, DRAW_PAT_X,DRAW_PAT_Y, DRAW_PAT_W,DRAW_PAT_H );
	SetPattern( PAT_GAME_JUDGE_FLD, TEX_GAME_SYSTEM, WIN_FLD_PAT_X,WIN_FLD_PAT_Y, WIN_FLD_PAT_W,WIN_FLD_PAT_H );

	//	���
	SetPattern( PAT_GAME_COUNT_STR, TEX_GAME_SYSTEM, SENME_PAT_X,SENME_PAT_Y, SENME_PAT_W,SENME_PAT_H );

	//	�I������
	SetPattern( PAT_GAME_END_STR, TEX_GAME_SYSTEM, END_STR_PAT_X,END_STR_PAT_Y, END_STR_PAT_W,END_STR_PAT_H );

	/////////////////////////////////////////////////
	//	�|���S���\��

	//	�w�i
	g_LibPol.SetPat( POL_GAME_BG, PAT_GAME_BG, 0,0,0.0, ATR_NONE );

	//	�R�D
	g_LibPol.SetPat( POL_GAME_DECK_PLAYER, PAT_GAME_DECK_PLAYER,
		DECK_PLAYER_POS_X,DECK_PLAYER_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_DECK_CPU, POL_GAME_DECK_CPU,
		DECK_CPU_POS_X,DECK_CPU_POS_Y,0.1, ATR_NONE );

	//	�|�C���g
	for( int i=0; i<3; i++ )
	{
		g_LibPol.SetPat( POL_GAME_POINT_PLAYER+i, PAT_GAME_NUMBER_NONE,
			POINT_PLAYER_POS_X+(i*16),POINT_PLAYER_POS_Y,0.1, ATR_NONE );

		g_LibPol.SetPat( POL_GAME_POINT_CPU+i, PAT_GAME_NUMBER_NONE,
			POINT_CPU_POS_X+(i*16),POINT_CPU_POS_Y,0.1, ATR_NONE );
	}
	//	�|�C���g�p�}�C�i�X�L��
	g_LibPol.SetPat( POL_GAME_POINT_PLAYER+5, PAT_GAME_MINUS,
		MINUS_PLAYER_POS_X,MINUS_PLAYER_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_POINT_CPU+5, PAT_GAME_MINUS,
		MINUS_CPU_POS_X,MINUS_CPU_POS_Y,0.1, ATR_NONE );
	g_LibPol.ChangeDisp( POL_GAME_POINT_PLAYER+5, OFF );
	g_LibPol.ChangeDisp( POL_GAME_POINT_CPU+5, OFF );

	//	���v�l�\���p
	g_LibPol.SetPat( POL_GAME_CALC_PLAYER, PAT_GAME_NUMBER0,
		CALC_PLAYER_POS_X,CALC_PLAYER_POS_Y,0.5, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_CALC_PLAYER+1, PAT_GAME_NUMBER0,
		CALC_PLAYER_POS_X + NUMBER_SIZE_W,CALC_PLAYER_POS_Y,0.5, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_CALC_CPU, PAT_GAME_NUMBER0,
		CALC_CPU_POS_X,CALC_CPU_POS_Y,0.5, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_CALC_CPU+1, PAT_GAME_NUMBER0,
		CALC_CPU_POS_X + NUMBER_SIZE_W,CALC_CPU_POS_Y,0.5, ATR_NONE );
	g_LibPol.ChangeDisp( POL_GAME_CALC_PLAYER, OFF );
	g_LibPol.ChangeDisp( POL_GAME_CALC_PLAYER+1, OFF );
	g_LibPol.ChangeDisp( POL_GAME_CALC_CPU, OFF );
	g_LibPol.ChangeDisp( POL_GAME_CALC_CPU+1, OFF );

	//	�{�^���֘A
	g_LibPol.Set( POL_GAME_BUT_FLD,
		BUT_FLD_SIZE_W,BUT_FLD_SIZE_H,
		BUT_FLD_POS_X,BUT_FLD_POS_Y, 0.6, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_BUT_OK, PAT_GAME_BUT_OK,
		BUT_OK_POS_X, BUT_OK_POS_Y, 0.7, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_BUT_CANCEL, PAT_GAME_BUT_CANCEL,
		BUT_CANCEL_POS_X, BUT_CANCEL_POS_Y, 0.7, ATR_NONE );
	g_LibPol.SetColor( POL_GAME_BUT_FLD, 128,0,0,0 );
	g_LibPol.SetColor( POL_GAME_BUT_OK, 255,128,128,128 );
	g_LibPol.SetColor( POL_GAME_BUT_CANCEL, 255,128,128,128 );

	//	���Z�l
	g_LibPol.SetPat( POL_GAME_PLAYER_OPT, PAT_GAME_PLUS, OPT_PLAYER_POS_X,OPT_PLAYER_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_PLAYER_ADD1, PAT_GAME_NUMBER0, ADD1_PLAYER_POS_X,ADD1_PLAYER_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_PLAYER_ADD2, PAT_GAME_NUMBER0, ADD2_PLAYER_POS_X,ADD2_PLAYER_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_PLAYER_ADD3, PAT_GAME_NUMBER0, ADD3_PLAYER_POS_X,ADD3_PLAYER_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_CPU_OPT, PAT_GAME_PLUS, OPT_CPU_POS_X,OPT_CPU_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_CPU_ADD1, PAT_GAME_NUMBER0, ADD1_CPU_POS_X,ADD1_CPU_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_CPU_ADD2, PAT_GAME_NUMBER0, ADD2_CPU_POS_X,ADD2_CPU_POS_Y,0.1, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_CPU_ADD3, PAT_GAME_NUMBER0, ADD3_CPU_POS_X,ADD3_CPU_POS_Y,0.1, ATR_NONE );
	g_LibPol.ChangeDisp( POL_GAME_PLAYER_OPT, OFF);
	g_LibPol.ChangeDisp( POL_GAME_PLAYER_ADD1, OFF);
	g_LibPol.ChangeDisp( POL_GAME_PLAYER_ADD2, OFF);
	g_LibPol.ChangeDisp( POL_GAME_PLAYER_ADD3, OFF);
	g_LibPol.ChangeDisp( POL_GAME_CPU_OPT, OFF);
	g_LibPol.ChangeDisp( POL_GAME_CPU_ADD1, OFF);
	g_LibPol.ChangeDisp( POL_GAME_CPU_ADD2, OFF);
	g_LibPol.ChangeDisp( POL_GAME_CPU_ADD3, OFF);

	//	���s�֘A
	g_LibPol.SetPat( POL_GAME_JUDGE_FLD, PAT_GAME_JUDGE_FLD,
		WIN_FLD_POS_X,WIN_FLD_POS_Y, 0.5, ATR_NONE );
	g_LibPol.SetPat( POL_GAME_JUDGE, PAT_GAME_WIN,
		WIN_POS_X,WIN_POS_Y,0.6, ATR_NONE );
	g_LibPol.SetColor( POL_GAME_JUDGE+1, 255,128,128,128 );
	g_LibPol.ChangeDisp( POL_GAME_JUDGE_FLD, OFF);
	g_LibPol.ChangeDisp( POL_GAME_JUDGE, OFF);

	//	�L�����N�^�[�\���̏�����
	InitChara( GamePlayer );
	InitChara( GameCpu );

	//	����������
	srand( (unsigned int)time(NULL) );

	//	�R�D������
	InitDeck( Chara[GamePlayer].Deck );
	InitDeck( Chara[GameCpu].Deck );

	//	�|�C���g�\��
	DispPoint();
}

/******************************************************************************/
//	�Q�[���J�n
/******************************************************************************/
void	GameStart( void )
{
	long Count[2] = {0};
	Count[0] = g_GameMng.GameCount / 10;
	Count[1] = g_GameMng.GameCount % 10;

	switch( g_GameMng.GameModeState )
	{
		//	������
		case	0:
			g_LibPol.SetPat( POL_GAME_COUNT1, PAT_GAME_NUMBER0+Count[0], WIDTH,228, 0.8, ATR_NONE );
			g_LibPol.SetPat( POL_GAME_COUNT2, PAT_GAME_NUMBER0+Count[1], WIDTH+NUMBER_SIZE_W,228, 0.8, ATR_NONE );
			g_LibPol.SetPat( POL_GAME_COUNT_STR, PAT_GAME_COUNT_STR,
				g_LibPol.GetInfo(POL_GAME_COUNT2).PosX+NUMBER_SIZE_W,228, 0.8, ATR_NONE );
			g_LibPol.Set( POL_GAME_COUNT_FLD, WIDTH,0, 0,240,0.7, ATR_NONE );
			g_LibPol.SetColor( POL_GAME_COUNT_FLD, 255,0,0,0 );
			g_GameMng.GameModeState = 1;
			break;

		//	�g�\��
		case	1:
			g_LibPol.AddHeight( POL_GAME_COUNT_FLD, 2 );
			g_LibPol.AddPosY( POL_GAME_COUNT_FLD, -1 );
			if( g_LibPol.GetInfo(POL_GAME_COUNT_FLD).Height >= WIN_FLD_PAT_H )
			{
				g_GameMng.GameModeState = 2;
			}
			break;

		//	�����\��
		case	2:
			g_LibPol.AddPosX( POL_GAME_COUNT1, -20 );
			g_LibPol.AddPosX( POL_GAME_COUNT2, -20 );
			g_LibPol.AddPosX( POL_GAME_COUNT_STR, -20 );
			if( g_LibPol.GetInfo(POL_GAME_COUNT1).PosX <= COUNT_POS_X )
			{
				g_GameMng.GameModeState = 3;
			}
			break;

		//	�E�G�C�g
		case	3:
			WaitTime( 30 );
			g_GameMng.GameModeState = 4;
			break;

		//	�����ړ�
		case	4:
			g_LibPol.AddPosX( POL_GAME_COUNT1, -20 );
			g_LibPol.AddPosX( POL_GAME_COUNT2, -20 );
			g_LibPol.AddPosX( POL_GAME_COUNT_STR, -20 );
			if( g_LibPol.GetInfo(POL_GAME_COUNT_STR).PosX+SENME_PAT_W <= 0 )
			{
				g_GameMng.GameModeState = 5;
			}
			break;

		//	�g����
		case	5:
			g_LibPol.AddHeight( POL_GAME_COUNT_FLD, -2 );
			g_LibPol.AddPosY( POL_GAME_COUNT_FLD, 1 );
			if( g_LibPol.GetInfo(POL_GAME_COUNT_FLD).Height <= 0)
			{
				g_GameMng.GameModeState = 10;
			}
			break;;

		case	10:
			g_GameMng.GameMode = GameModeCardDraw;
			for( int i=0; i<4; i++ )
			{
				Chara[ GamePlayer ].StateFlag[i] = 0;
				Chara[ GameCpu ].StateFlag[i] = 0;
			}
			break;
	}
}

/******************************************************************************/
//	�J�[�h��z��
/******************************************************************************/
void	GameCardDraw( void )
{
	for( int i=0; i<4; i++ )
	{
		PlayerCardDraw( i );
		CpuCardDraw( i );
	}

	if( ( Chara[GamePlayer].StateFlag[0] == 4 )
		&& ( Chara[GamePlayer].StateFlag[1] == 4 )
		&& ( Chara[GamePlayer].StateFlag[2] == 4 )
		&& ( Chara[GamePlayer].StateFlag[3] == 4 )
		&& ( Chara[GameCpu].StateFlag[0] == 2 )
		&& ( Chara[GameCpu].StateFlag[1] == 2 )
		&& ( Chara[GameCpu].StateFlag[2] == 2 )
		&& ( Chara[GameCpu].StateFlag[3] == 2 ) )
	{
		g_GameMng.GameMode = GameModeSelect;
		Chara[GamePlayer].DeckIndex += 4;
		Chara[GameCpu].DeckIndex += 4;

		//	�I����Ԃ�������
		memset( &Chara[ GamePlayer ].SelectCard, 0x00, sizeof(Chara[ GamePlayer ].SelectCard) );
		memset( &Chara[ GameCpu ].SelectCard, 0x00, sizeof(Chara[ GameCpu ].SelectCard) );
	}
}

/******************************************************************************/
//	�v���C���[�ɃJ�[�h��z��
/******************************************************************************/
void	PlayerCardDraw( long Num )
{
	//	���݂��S�����J�[�h��z��
	switch( Chara[ GamePlayer ].StateFlag[Num] )
	{
		//	�J�[�h������
		case	0:
			g_LibPol.SetPat( POL_GAME_HAND_PLAYER+Num, PAT_GAME_DECK_PLAYER,
				DECK_PLAYER_POS_X + (CARD_SIZE_W/2),
				DECK_PLAYER_POS_Y + (CARD_SIZE_H/2),
				0.1, ATR_NONE );
			g_LibPol.SetCenter( POL_GAME_HAND_PLAYER+Num );

			Chara[ GamePlayer ].HandCardRotR[Num] = GetRot(
					HandPlayerPos[3][Num][0],
					HandPlayerPos[3][Num][1],
					DECK_PLAYER_POS_X + (CARD_SIZE_W/2),
					DECK_PLAYER_POS_Y + (CARD_SIZE_H/2)
					);
			Chara[ GamePlayer ].StateFlag[Num] = 1;

		case	1:
			g_LibPol.AddPos( POL_GAME_HAND_PLAYER+Num,
				-( GetMoveX( 20, Chara[ GamePlayer ].HandCardRotR[Num] ) ),
				-( GetMoveY( 20, Chara[ GamePlayer ].HandCardRotR[Num] ) ) );
			if( g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+Num).PosX <= HandPlayerPos[3][Num][0] )
			{
				Chara[ GamePlayer ].StateFlag[Num] = 2;
				g_LibPol.SetPos( POL_GAME_HAND_PLAYER+Num,
					HandPlayerPos[3][Num][0], HandPlayerPos[3][Num][1] );
			}
			break;

		case	2:
			g_LibPol.AddRot( POL_GAME_HAND_PLAYER+Num, 0, 10, 0 );
			if( g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+Num).RotY >= 90 )
			{
				Chara[ GamePlayer ].StateFlag[Num] = 3;
				g_LibPol.SetPatNo( POL_GAME_HAND_PLAYER+Num,
					Chara[ GamePlayer ].Deck[Chara[ GamePlayer ].DeckIndex+Num].Mark
					+ (Chara[ GamePlayer ].Deck[Chara[ GamePlayer ].DeckIndex+Num].Number-1) );
				ST_PolygonInfo Info = g_LibPol.GetInfo( POL_GAME_HAND_PLAYER + Num );
				Info.Atr |= ATR_REV_W;
				g_LibPol.SetInfo( POL_GAME_HAND_PLAYER+Num, Info );
			}
			break;

		case		3:
			g_LibPol.AddRot( POL_GAME_HAND_PLAYER+Num, 0, 5, 0 );
			if( g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+Num).RotY >= 180 )
			{
				g_LibPol.SetCenter( POL_GAME_HAND_PLAYER+Num );
				g_LibPol.SetPos( POL_GAME_HAND_PLAYER+Num, HandPlayerPos[3][Num][0], HandPlayerPos[3][Num][1] );
				ST_PolygonInfo Info = g_LibPol.GetInfo( POL_GAME_HAND_PLAYER+Num );
				Info.RotY = 0;
				Info.Atr &= ~ATR_REV_W;
				g_LibPol.SetInfo( POL_GAME_HAND_PLAYER+Num, Info );
				Chara[ GamePlayer ].StateFlag[Num] = 4;
			}
			break;
	}
}

/******************************************************************************/
//	CPU�ɃJ�[�h��z��
/******************************************************************************/
void	CpuCardDraw( long Num )
{
	//	���݂��S�����J�[�h��z��
	switch( Chara[ GameCpu ].StateFlag[Num] )
	{
		//	�J�[�h������
		case	0:
			g_LibPol.SetPat( POL_GAME_HAND_CPU+Num, PAT_GAME_DECK_CPU,
				DECK_CPU_POS_X + (CARD_SIZE_W/2),
				DECK_CPU_POS_Y + (CARD_SIZE_H/2),
				0.1, ATR_NONE );
			g_LibPol.SetCenter( POL_GAME_HAND_CPU+Num );

			Chara[ GameCpu ].HandCardRotR[Num]
				= GetRot(
					HandCpuPos[3][Num][0],
					HandCpuPos[3][Num][1],
					DECK_CPU_POS_X + (CARD_SIZE_W/2),
					DECK_CPU_POS_Y + (CARD_SIZE_H/2)
					);
			Chara[ GameCpu ].StateFlag[Num] = 1;

		case	1:
			g_LibPol.AddPos( POL_GAME_HAND_CPU+Num,
				-( GetMoveX( 20, Chara[ GameCpu ].HandCardRotR[Num] ) ),
				-( GetMoveY( 20, Chara[ GameCpu ].HandCardRotR[Num] ) ) );
			if( g_LibPol.GetInfo(POL_GAME_HAND_CPU+Num).PosX >= HandCpuPos[3][Num][0] )
			{
				Chara[ GameCpu ].StateFlag[Num] = 2;
				g_LibPol.SetPos( POL_GAME_HAND_CPU+Num,
					HandCpuPos[3][Num][0], HandCpuPos[3][Num][1] );
			}
			break;
	}
}

/******************************************************************************/
//	�J�[�h�I��
/******************************************************************************/
void	GameSelectCard( void )
{
	g_LibPol.ChangeDisp( POL_GAME_BUT_FLD, ON );
	g_LibPol.ChangeDisp( POL_GAME_BUT_OK, ON );
	g_LibPol.ChangeDisp( POL_GAME_BUT_CANCEL, ON );

	g_LibText.Set( 0, 168,360, 255,255,255,255,
		"���v�Q�P�ɂȂ�悤�ɂQ���ȏ�I�����ĉ������B" );

	if( Chara[GamePlayer].SelCardCt >=1 )
	{
		g_LibPol.SetColor( POL_GAME_BUT_CANCEL, 255,255,255,255 );
	}
	else
	{
		g_LibPol.SetColor( POL_GAME_BUT_CANCEL, 255,128,128,128 );
	}
	if( Chara[GamePlayer].SelCardCt > 1 && 21>=CardCalcOne( GamePlayer ) )
	{
		g_LibPol.SetColor( POL_GAME_BUT_OK, 255,255,255,255 );
	}
	else
	{
		g_LibPol.SetColor( POL_GAME_BUT_OK, 255,128,128,128 );
	}

	long HandNum = Chara[GamePlayer].HandNumber-1;

	if( MOUSE.Trg[ MOUSE_LEFT ] )
	{
		if( ( MOUSE.PosX >= BUT_OK_POS_X )
		&& ( MOUSE.PosX <= BUT_OK_POS_X+BUT_OK_PAT_W )
		&& ( MOUSE.PosY >= BUT_OK_POS_Y )
		&& ( MOUSE.PosY <= BUT_OK_POS_Y+BUT_OK_PAT_H ) )
		{
			//	OK�{�^������
			if( Chara[GamePlayer].SelCardCt >= 2 && 21>=CardCalcOne( GamePlayer ) )
			{
				TXT[0].Disp = OFF;
				g_GameMng.GameMode = GameModeCpuSelect;
			}
		}

		if( ( MOUSE.PosX >= BUT_CANCEL_POS_X )
			&& ( MOUSE.PosX <= BUT_CANCEL_POS_X+BUT_CANSEL_PAT_W )
			&& ( MOUSE.PosY >= BUT_CANCEL_POS_Y )
			&& ( MOUSE.PosY <= BUT_CANCEL_POS_Y+BUT_CANSEL_PAT_H ) )
		{
			//	CANCEL�{�^������
			for( int i=0; i<HAND_NUM_MAX; i++ )
			{
				//	�J�[�h�����̈ʒu�ɖ߂�
				g_LibPol.SetPosY( POL_GAME_HAND_PLAYER+i, HandPlayerPos[HandNum][i][1] );
				//	���I���ɂ���
				Chara[ GamePlayer ].SelectCard[i] = OFF;
			}
		}

		//	�J�[�h�����蔻��
		for( int i=0; i<HAND_NUM_MAX; i++ )
		{
			if( HitPlayerCard[ HandNum ][i] == NULL )
			{
				continue;
			}

			double X1 = g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).PosX - g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).CenterX;
			double X2 = g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).PosX - g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).CenterX + g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).Width;
			double Y1 = g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).PosY - g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).CenterY;
			double Y2 = g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).PosY - g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).CenterY + g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+i).Height;
			if( ( MOUSE.PosX >= X1 )
			&& ( MOUSE.PosX <= X2 )
			&& ( MOUSE.PosY >= Y1 )
			&& ( MOUSE.PosY <= Y2 ) )
			{
				//	�I������Ă��邩����
				if( Chara[ GamePlayer ].SelectCard[ i ] == ON )
				{
					long Index = Chara[ GamePlayer ].SelectCard[ i ];
					g_LibPol.SetPosY( POL_GAME_HAND_PLAYER + i, HandPlayerPos[HandNum][i][1] );
					Chara[ GamePlayer ].SelectCard[ i ] = OFF;
				}
				else
				{
					Chara[ GamePlayer ].SelectCard[ i ] = ON;
				}
			}
		}
	}

	//	�I�𐔎擾
	Chara[ GamePlayer ].SelCardCt = 0;
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		if( Chara[ GamePlayer ].SelectCard[ i ] == ON )
		{
			Chara[ GamePlayer ].SelCardCt ++;
		}
	}

	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		long Index = Chara[ GamePlayer ].SelectCard[ i ];
		if( Index == OFF )
		{
			continue;
		}
		g_LibPol.SetPosY( POL_GAME_HAND_PLAYER + i, HandPlayerPos[HandNum][i][1] - 32 );
	}

	DispCardCalcOne( GamePlayer, POL_GAME_CALC_PLAYER );
}

/******************************************************************************/
//	CPU�J�[�h�I��
/******************************************************************************/
void	GameSelectCardCpu( void )
{
	long	CalcTbl[][4] =
	{
		{ 0, 1, -1, -1 },
		{ 0, 1,  2, -1 },
		{ 0, 1,  2,  3 },
		{ 0, 1,  3, -1 },
		{ 0, 2, -1, -1 },
		{ 0, 2,  3, -1 },
		{ 0, 3, -1, -1 },
		{ 1, 2, -1, -1 },
		{ 1, 2,  3, -1 },
		{ 1, 3, -1, -1 },
		{ 2, 3, -1, -1 },
	};
	//	�z��̗v�f���擾
	long Size = sizeof(CalcTbl) / (sizeof(long)*4);

	//	��D�擾
	long Hand[4];
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		long Index = Chara[ GameCpu ].DeckIndex-HAND_NUM_MAX;
		Hand[i] = Chara[ GameCpu ].Deck[ Index+i ].Number;
	}

	//	������ōő�l���擾
	long MaxCalc = 0;
	long MaxCallcIndex = 0;
	long Card[4] = {NOT};
	for( int i=0; i<Size; i++ )
	{
		memset( &Card, -1, sizeof(Card) );
		//	�v�Z�p�̃e�[�u���쐬
		long Index = 0;
		for( int j=0; j<HAND_NUM_MAX; j++ )
		{
			if( CalcTbl[i][j] != NOT )
			{
				Card[Index] = Hand[ CalcTbl[i][j] ];
				Index ++;
			}
		}
		long TempCalc = CardCalc ( Card );
		char Str[MAX_PATH+1];
		memset( &Str, 0x00, sizeof(Str) );
		sprintf_s( Str, MAX_PATH, "%2d ���v�F%d\n",i, TempCalc );
		TRACE(Str);
		if( TempCalc < 22 && MaxCalc <= TempCalc )
		{
			MaxCalc = TempCalc;
			MaxCallcIndex = i;
		}
	}

	//	�I���J�[�h���v�Z
	Chara[ GameCpu ].SelCardCt = 0;
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		if( CalcTbl[MaxCallcIndex][i] != NOT )
		{
			Chara[ GameCpu ].SelCardCt ++;
			Chara[ GameCpu ].SelectCard[ CalcTbl[MaxCallcIndex][i] ] = ON;
		}
	}

	char Str[MAX_PATH+1];
	memset( &Str, 0x00, sizeof(Str) );
	sprintf_s( Str, MAX_PATH, "�ő�IDX�F%2d �ő�l�F%d\n", MaxCallcIndex, MaxCalc );
	TRACE(Str);

	g_GameMng.GameMode = GameModeFldCardSet;

	g_LibPol.ChangeDisp( POL_GAME_BUT_FLD, OFF );
	g_LibPol.ChangeDisp( POL_GAME_BUT_OK, OFF );
	g_LibPol.ChangeDisp( POL_GAME_BUT_CANCEL, OFF );

	memset( &Chara[ GamePlayer ].StateFlag, 0x00, sizeof(Chara[ GamePlayer ].StateFlag) );
	memset( &Chara[ GameCpu ].StateFlag, 0x00, sizeof(Chara[ GameCpu ].StateFlag) );

#ifdef	_DEBUG
	//g_LibText.Set( 10, 0,0, 255,255,255,255, "" );
	//sprintf_s( TXT[10].Str, MAX_PATH, "��D�F[%2d]  [%2d]  [%2d]  [%2d]",
	//	Hand[0], Hand[1], Hand[2], Hand[3] );
#endif
}

/******************************************************************************/
//	�t�B�[���h�ɃJ�[�h�ݒ�
/******************************************************************************/
void	GameFldCardSet( void )
{
	long Index = 0;

	//	CPU�J�[�h�Z�b�g
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		if( Chara[ GameCpu ].SelectCard[i] )
		{
			CpuCardFldSet( i, Index );
			Index ++;
		}
		else
		{
			Chara[ GameCpu ].StateFlag[i] = 4;
		}
	}

	//	�v���C���[�J�[�h�Z�b�g
	Index = 0;
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		if( Chara[ GamePlayer ].SelectCard[i] )
		{
			PlayerCardFldSet( i, Index );
			Index ++;
		}
		else
		{
			Chara[ GamePlayer ].StateFlag[i] = 2;
		}
	}

	if( ( Chara[GamePlayer].StateFlag[0] == 2 )
		&& ( Chara[GamePlayer].StateFlag[1] == 2 )
		&& ( Chara[GamePlayer].StateFlag[2] == 2 )
		&& ( Chara[GamePlayer].StateFlag[3] == 2 )
		&& ( Chara[GameCpu].StateFlag[0] == 4 )
		&& ( Chara[GameCpu].StateFlag[1] == 4 )
		&& ( Chara[GameCpu].StateFlag[2] == 4 )
		&& ( Chara[GameCpu].StateFlag[3] == 4 )
		)
	{
		//	����
		g_GameMng.GameMode = GameModeDispSum;
		g_GameMng.GameModeState = 0;
	}
}

/******************************************************************************/
//	�v���C���[�J�[�h�t�B�[���h�Z�b�g
/******************************************************************************/
void	PlayerCardFldSet( long Num, long FldIndex )
{
	long SelCt = Chara[ GamePlayer ].SelCardCt-2;
	//	�I�𒆂̃J�[�h�ړ�
	switch( Chara[ GamePlayer ].StateFlag[Num] )
	{
		//	�J�[�h������
		case	0:
			Chara[ GamePlayer ].HandCardRotR[Num]
				= GetRot(
					g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+Num).PosX,
					g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+Num).PosY,
					FldPlayerPos[ SelCt ][FldIndex][0] + (CARD_SIZE_W/2),
					FldPlayerPos[ SelCt ][FldIndex][1] + (CARD_SIZE_H/2)
					);
			Chara[ GamePlayer ].StateFlag[Num] = 1;
			break;

		case	1:
			//	�w���W�ړ�
			g_LibPol.AddPosX( POL_GAME_HAND_PLAYER+Num, GetMoveX( 20, Chara[ GamePlayer ].HandCardRotR[Num] ) );
			//	�x���W�ړ�
			g_LibPol.AddPosY( POL_GAME_HAND_PLAYER+Num, GetMoveY( 20, Chara[ GamePlayer ].HandCardRotR[Num] ) );

			if( g_LibPol.GetInfo(POL_GAME_HAND_PLAYER+Num).PosY <= FldPlayerPos[ SelCt ][FldIndex][1] + (CARD_SIZE_H/2) )
			{
				Chara[ GamePlayer ].StateFlag[Num] = 2;
				g_LibPol.SetPos( POL_GAME_HAND_PLAYER+Num,
					FldPlayerPos[ SelCt ][FldIndex][0] + (CARD_SIZE_W/2),
					FldPlayerPos[ SelCt ][FldIndex][1] + (CARD_SIZE_H/2)
					);
			}
			break;

		case	2:
			break;
	}
}

/******************************************************************************/
//	CPU�J�[�h�t�B�[���h�Z�b�g
/******************************************************************************/
void	CpuCardFldSet( long Num, long FldIndex )
{
	long SelCt = Chara[ GameCpu ].SelCardCt-2;
	//	�I�𒆂̃J�[�h�ړ�
	switch( Chara[ GameCpu ].StateFlag[Num] )
	{
		//	�J�[�h������
		case	0:
			Chara[ GameCpu ].HandCardRotR[Num]
			= GetRot(
				g_LibPol.GetInfo(POL_GAME_HAND_CPU+Num).PosX,
				g_LibPol.GetInfo(POL_GAME_HAND_CPU+Num).PosY,
				FldCpuPos[ SelCt ][FldIndex][0] + (CARD_SIZE_W/2),
				FldCpuPos[ SelCt ][FldIndex][1] + (CARD_SIZE_H/2)
				);
			Chara[ GameCpu ].StateFlag[Num] = 1;
			break;

		case	1:
			//	�w���W�ړ�
			g_LibPol.AddPosX( POL_GAME_HAND_CPU+Num, GetMoveX( 20, Chara[ GameCpu ].HandCardRotR[Num] ) );
			//	�x���W�ړ�
			g_LibPol.AddPosY( POL_GAME_HAND_CPU+Num, GetMoveY( 20, Chara[ GameCpu ].HandCardRotR[Num] ) );

			if( g_LibPol.GetInfo(POL_GAME_HAND_CPU+Num).PosY <= FldCpuPos[ SelCt ][FldIndex][1] + (CARD_SIZE_H/2) )
			{
				Chara[ GameCpu ].StateFlag[Num] = 2;
				g_LibPol.SetPos( POL_GAME_HAND_CPU+Num,
					FldCpuPos[ SelCt ][FldIndex][0] + (CARD_SIZE_W/2),
					FldCpuPos[ SelCt ][FldIndex][1] + (CARD_SIZE_H/2)
					);
			}
			break;

		case	2:
			g_LibPol.AddRot( POL_GAME_HAND_CPU+Num, 0,10,0 );
			if( g_LibPol.GetInfo(POL_GAME_HAND_CPU+Num).RotY >= 90 )
			{
				Chara[ GameCpu ].StateFlag[Num] = 3;
				//	�p�^�[���ύX
				g_LibPol.SetPatNo( POL_GAME_HAND_CPU+Num,
					Chara[ GameCpu ].Deck[ Chara[ GameCpu ].DeckIndex-4+Num ].Mark
					+ (Chara[ GameCpu ].Deck[ Chara[ GameCpu ].DeckIndex-4+Num ].Number-1) );
				ST_PolygonInfo Info = g_LibPol.GetInfo( POL_GAME_HAND_CPU+Num );
				Info.Atr |= ATR_REV_W;
				g_LibPol.SetInfo( POL_GAME_HAND_CPU+Num, Info );
			}
			break;

		case		3:
			g_LibPol.AddRot( POL_GAME_HAND_CPU+Num, 0,5,0 );
			if( g_LibPol.GetInfo(POL_GAME_HAND_CPU+Num).RotY >= 180 )
			{
				g_LibPol.SetRotY( POL_GAME_HAND_CPU+Num , 0 );
				g_LibPol.SetCenter( POL_GAME_HAND_CPU+Num );
				ST_PolygonInfo Info = g_LibPol.GetInfo( POL_GAME_HAND_CPU+Num );
				Info.Atr &= ~ATR_REV_W;
				g_LibPol.SetInfo( POL_GAME_HAND_CPU+Num, Info );
				Chara[ GameCpu ].StateFlag[Num] = 4;
			}
			break;
	}
}

/******************************************************************************/
//	���v�l�\��
/******************************************************************************/
void	DispCardCalc( void )
{
	long	PlayCalc;
	long	CpuClac;

	switch( g_GameMng.GameModeState )
	{
		//	������
		case	0:
			//	�ʂɍ��v�l�\��
			PlayCalc = DispCardCalcOne( GamePlayer, POL_GAME_CALC_PLAYER );
			CpuClac = DispCardCalcOne( GameCpu, POL_GAME_CALC_CPU );

			g_GameMng.PointAdd[ GamePlayer ] = PlayCalc - CpuClac;
			g_GameMng.PointAdd[ GameCpu ] = CpuClac - PlayCalc;
			g_GameMng.PointAdd[ GamePlayer ] *= 2;
			g_GameMng.PointAdd[ GameCpu ] *= 2;
			if( PlayCalc == CpuClac )
			{
				g_GameMng.GameModeState = 5;
				break;
			}
			else
			{
				g_GameMng.GameModeState = 1;
			}

			//	���W������
			g_LibPol.SetPos( POL_GAME_PLAYER_OPT, OPT_PLAYER_POS_X,OPT_PLAYER_POS_Y );
			g_LibPol.SetPos( POL_GAME_PLAYER_ADD1, ADD1_PLAYER_POS_X,ADD1_PLAYER_POS_Y );
			g_LibPol.SetPos( POL_GAME_PLAYER_ADD2, ADD2_PLAYER_POS_X,ADD2_PLAYER_POS_Y );
			g_LibPol.SetPos( POL_GAME_PLAYER_ADD3, ADD3_PLAYER_POS_X,ADD3_PLAYER_POS_Y );
			g_LibPol.SetPos( POL_GAME_CPU_OPT, OPT_CPU_POS_X,OPT_CPU_POS_Y );
			g_LibPol.SetPos( POL_GAME_CPU_ADD1, ADD1_CPU_POS_X,ADD1_CPU_POS_Y );
			g_LibPol.SetPos( POL_GAME_CPU_ADD2, ADD2_CPU_POS_X,ADD2_CPU_POS_Y );
			g_LibPol.SetPos( POL_GAME_CPU_ADD3, ADD3_CPU_POS_X,ADD3_CPU_POS_Y );
			//	�F������
			g_LibPol.SetColor( POL_GAME_PLAYER_OPT, 255,255,255,255 );
			g_LibPol.SetColor( POL_GAME_PLAYER_ADD1, 255,255,255,255 );
			g_LibPol.SetColor( POL_GAME_PLAYER_ADD2, 255,255,255,255 );
			g_LibPol.SetColor( POL_GAME_PLAYER_ADD3, 255,255,255,255 );
			g_LibPol.SetColor( POL_GAME_CPU_OPT, 255,255,255,255 );
			g_LibPol.SetColor( POL_GAME_CPU_ADD1, 255,255,255,255 );
			g_LibPol.SetColor( POL_GAME_CPU_ADD2, 255,255,255,255 );
			g_LibPol.SetColor( POL_GAME_CPU_ADD3, 255,255,255,255 );
			break;

		//	�E�G�C�g
		case	1:
			WaitTime( 45 );
			g_GameMng.GameModeState = 2;
			break;

		//	���Z�l�\��
		case	2:
			DispAddPointOne( GamePlayer, POL_GAME_PLAYER_OPT );
			DispAddPointOne( GameCpu, POL_GAME_CPU_OPT );
			g_GameMng.GameModeState = 3;
			break;

		//	�E�G�C�g
		case	3:
			WaitTime( 45 );
			g_GameMng.GameModeState = 4;
			break;

		//	���Z
		case	4:
			for( int i=0; i<4; i++ )
			{
				//	�v���C���[�͉��Ɉړ�
				g_LibPol.AddPosY( POL_GAME_PLAYER_OPT + i, 4 );
				g_LibPol.AddColor( POL_GAME_PLAYER_OPT + i, -48,0,0,0 );
				//	CPU�͏�Ɉړ�
				g_LibPol.AddPosY( POL_GAME_CPU_OPT + i, -4 );
				g_LibPol.AddColor( POL_GAME_CPU_OPT + i, -48,0,0,0 );
			}
			if( g_LibPol.GetInfo(POL_GAME_PLAYER_OPT).PosY >= POINT_PLAYER_POS_Y )
			{
				for( int i=0; i<4; i++ )
				{
					g_LibPol.ChangeDisp( POL_GAME_PLAYER_OPT + i, OFF );
					g_LibPol.ChangeDisp( POL_GAME_CPU_OPT + i, OFF );
				}
				//	�|�C���g���Z
				g_GameMng.Point[ GamePlayer ] += g_GameMng.PointAdd[ GamePlayer ];
				g_GameMng.Point[ GameCpu ] += g_GameMng.PointAdd[ GameCpu ];
				g_GameMng.GameModeState = 5;
			}
			break;

		//	�E�G�C�g
		case	5:
			WaitTime( 45 );
			g_GameMng.GameModeState = 6;
			break;

		//	�J�[�h��z��
		case	6:
			g_GameMng.GameCount ++;
			//	TODO �e�X�g
			if( g_GameMng.GameCount < 14 )
			{
				g_GameMng.GameMode = GameModeStart;
			}
			else
			{
				g_GameMng.GameMode = GameModeEnd;
			}
			g_GameMng.GameModeState = 0;
#ifdef	__DEBUG__
			////	���s����
			//if( Chara[GamePlayer].DeckIndex > CARD_MAX-HAND_NUM_MAX )
			//{
			//	Chara[GamePlayer].DeckIndex = 0;
			//	Chara[GameCpu].DeckIndex = 0;
			//}
#endif
			g_LibPol.ChangeDisp( POL_GAME_CALC_PLAYER+0, OFF );
			g_LibPol.ChangeDisp( POL_GAME_CALC_PLAYER+1, OFF );
			g_LibPol.ChangeDisp( POL_GAME_CALC_CPU+0, OFF );
			g_LibPol.ChangeDisp( POL_GAME_CALC_CPU+1, OFF );

			for( int i=0; i<4; i++ )
			{
				g_LibPol.ChangeDisp( POL_GAME_PLAYER_OPT + i, OFF );
				g_LibPol.ChangeDisp( POL_GAME_CPU_OPT + i, OFF );
			}
			break;
	}

	DispPoint();
}

/******************************************************************************/
//	�Q�[���I������
/******************************************************************************/
void	GameEnd( void )
{
	long	PatNo = 0;

	switch( g_GameMng.GameModeState )
	{
		//	������
		case	0:
			//	�J�[�h��\��
			for( int i=0; i<HAND_NUM_MAX; i++ )
			{
				g_LibPol.ChangeDisp( POL_GAME_HAND_PLAYER + i, OFF );
				g_LibPol.ChangeDisp( POL_GAME_HAND_CPU + i, OFF );
			}
			//	�S�̂��Â�����
			g_LibPol.Set( POL_GAME_JUDGE_FLD+1, WIDTH,HEIGHT, 0,0,0.7, ATR_NONE );
			g_LibPol.SetColor(POL_GAME_JUDGE_FLD+1, 128,0,0,0 );

			//	���蕶��
			if( g_GameMng.Point[GamePlayer] > g_GameMng.Point[GameCpu] )
			{
				//	�v���C���[�̏���
				PatNo = PAT_GAME_WIN;
			}
			else if( g_GameMng.Point[GamePlayer] == g_GameMng.Point[GameCpu] )
			{
				//	�h���[
				PatNo = PAT_GAME_DRAW;
			}
			else
			{
				//	�v���C���[�̕���
				PatNo = PAT_GAME_LOSE;
			}
			g_LibPol.SetPat( POL_GAME_JUDGE_FLD, PAT_GAME_JUDGE_FLD, WIN_FLD_POS_X,WIN_FLD_POS_Y,0.8, ATR_NONE );
			g_LibPol.SetPat( POL_GAME_JUDGE, PatNo, WIN_POS_X,WIN_POS_Y,0.9, ATR_NONE );

			//	����p�t�B�[���h�\��
			g_LibPol.Set( POL_GAME_JUDGE_FLD+2, END_STR_PAT_W+32,336, 320-((END_STR_PAT_W+32)/2),72,0.9, ATR_NONE );
			g_LibPol.SetColor(POL_GAME_JUDGE_FLD+2, 255,0,0,0 );

			//	�I�������\��
			g_LibPol.SetPat( POL_GAME_END_STR, PAT_GAME_END_STR,
				g_LibPol.GetInfo(POL_GAME_JUDGE_FLD+2).PosX+16,-END_STR_PAT_H,1.0, ATR_NONE );
			g_GameMng.GameModeState = 1;
			break;

		//	�I�������ړ�
		case	1:
			g_LibPol.AddPosY( POL_GAME_END_STR, 25 );
			if( g_LibPol.GetInfo(POL_GAME_END_STR).PosY >= (HEIGHT/2)-(END_STR_PAT_H/2) )
			{
				g_LibPol.SetPosY( POL_GAME_END_STR, (HEIGHT/2)-(END_STR_PAT_H/2) );
				g_GameMng.GameModeState = 2;
			}
			break;

		//	�E�G�C�g
		case	2:
			WaitTime( 45 );
			g_GameMng.GameModeState = 3;
			break;

		//	�I�������ړ�
		case	3:
			g_LibPol.AddPosY( POL_GAME_END_STR, 25 );
			if( g_LibPol.GetInfo(POL_GAME_END_STR).PosY >= HEIGHT )
			{
				g_LibPol.ChangeDisp( POL_GAME_END_STR, OFF );
				g_GameMng.GameModeState = 4;
			}
			break;

		//	�g����
		case	4:
			if( g_LibPol.GetInfo(POL_GAME_JUDGE_FLD+2).PosX > 0 )
			{
				g_LibPol.AddPosX( POL_GAME_JUDGE_FLD+2, -(((END_STR_PAT_W+32)/60) / 2) );
				g_LibPol.AddWidth( POL_GAME_JUDGE_FLD+2, (END_STR_PAT_W+32) / 60 );
			}
			if( g_LibPol.GetInfo( POL_GAME_JUDGE_FLD+2 ).Height > 0 )
			{
				g_LibPol.AddHeight( POL_GAME_JUDGE_FLD+2, -(HEIGHT/60) );
				g_LibPol.AddPosY( POL_GAME_JUDGE_FLD+2, ((HEIGHT/60)/2) );
			}
			if( g_LibPol.GetInfo( POL_GAME_JUDGE_FLD+2 ).PosX <= 0 
				&& g_LibPol.GetInfo( POL_GAME_JUDGE_FLD+2 ).Height <= 0
				&& g_LibPol.GetInfo( POL_GAME_JUDGE_FLD+2 ).Width >= WIDTH )
			{
				g_LibPol.ChangeDisp( POL_GAME_JUDGE_FLD+2, OFF );
				g_GameMng.GameModeState = 5;
			}
			break;

		case	5:
			if( MOUSE.Trg[ MOUSE_RIGHT ] )
			{
				g_GameMng.GameMode = GameModeInit;
			}
			break;
	}
}

/******************************************************************************/
//	�L�����N�^�[�\���̏�����
/******************************************************************************/
void	InitChara( long Index )
{
	memset( &Chara[Index], 0x00, sizeof(Chara[Index]) );
	Chara[Index].HandNumber = HAND_NUM_MAX;
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		Chara[Index].SelectCard[ i ] = OFF;
	}
}

/******************************************************************************/
//	�R�D�V���b�t��
/******************************************************************************/
void	InitDeck( DECK_TBL* Deck )
{
	//	�܂��͎R�D�����Ԃɕ��ׂ�
	for( int i=0; i<USE_NUMBER; i++ )
	{
		Deck[ i ].Mark = PAT_GAME_HEART;
		Deck[ i ].Number = i+1;
		Deck[ USE_NUMBER+i ].Mark = PAT_GAME_DIAMOND;
		Deck[ USE_NUMBER+i ].Number = i+1;
		Deck[ (USE_NUMBER*2)+i ].Mark = PAT_GAME_CLUB;
		Deck[ (USE_NUMBER*2)+i ].Number = i+1;
		Deck[ (USE_NUMBER*3)+i ].Mark = PAT_GAME_SPADE;
		Deck[ (USE_NUMBER*3)+i ].Number = i+1;
	}

	long Count = CARD_MAX;
	DECK_TBL	Temp;

	//	�R�D�ő吔���V���b�t��
	for( int i=0; i<CARD_MAX; i++ )
	{
		//	Fisher-Yates�@�g�p���A100��V���b�t������
		while( Count > 1 )
		{
			long Index = (int)( rand() % CARD_MAX );
			Count --;
			Temp = Deck[Count];
			Deck[Count] = Deck[Index];
			Deck[Index] = Temp;
		}
	}

#ifdef	__DEBUG__
	char	MarkTbl[USE_MARK][10]=
	{
		"�n�[�g","�_�C��","�N���u","�X�y�[�h",
	};
	char	DispStr[MAX_PATH];
	for( int No=0; No<CARD_MAX; No++ )
	{
		memset( &DispStr, 0x00, sizeof(DispStr) );
		sprintf_s( DispStr, MAX_PATH, "%2d %s%d\n",
			No, MarkTbl[ (Deck[No].Mark-30)/20 ], Deck[No].Number );
		TRACE(DispStr);
	}
#endif
}

/******************************************************************************/
//	�|�C���g�\��
/******************************************************************************/
void	DispPoint( void )
{
	DispPointOne( GamePlayer, POL_GAME_POINT_PLAYER );
	DispPointOne( GameCpu, POL_GAME_POINT_CPU );
}

/******************************************************************************/
//	�ʃ|�C���g�\��
/******************************************************************************/
void	DispPointOne( long UserFlag, long PolNo )
{
	long	cScore[3] = {0};
	long	Max = 100;
	long	scr = g_GameMng.Point[UserFlag];

	if( scr < 0 )
	{
		scr *= -1;
		g_LibPol.ChangeDisp( PolNo+5, ON );
	}

	for( int i=0; i<3; i++ )
	{
		cScore[i] = scr / Max;
		scr %= Max;
		Max /=10;
	}

	for( int i=0; i<3; i++ )
	{
		g_LibPol.ChangeDisp( PolNo + i, ON );
		g_LibPol.SetPatNo( PolNo + i, PAT_GAME_NUMBER0 + cScore[i] );
	}

	//	�\���ݒ�
	if( cScore[0] == 0 )
	{
		g_LibPol.ChangeDisp( PolNo, OFF );
	}
	for( int i=1; i<2; i++ )
	{
		if( cScore[i] == 0 )
		{
			if( cScore[i-1] == 0 && g_LibPol.GetInfo( PolNo + i-1 ).Disp == OFF )
			{
				g_LibPol.ChangeDisp( PolNo + i, OFF );
			}
		}
	}
}

/******************************************************************************/
//	���v�v�Z
/******************************************************************************/
long	CardCalc( long *SelTbl )
{
	long Point = 0;
	long AceCt = 0;

	//	���v
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		if( SelTbl[i] == NOT )
		{
			continue;
		}

		if( SelTbl[i] > 1 && SelTbl[i] < 10 )
		{
			//	1�`9�����Z
			Point += SelTbl[i];
		}
		else
		{
			//	10�`��10�Ƃ��Čv�Z
			if( SelTbl[i] >= 10 )
			{
				Point += 10;
			}
		}
	}

	//	ACE�J�E���g
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		if( SelTbl[i] == 1 )
		{
			AceCt ++;
		}
	}

	//	ACE���Z
	for( int i=0; i<AceCt; i++ )
	{
		if( Point+11 > 21 )
		{
			if( i>0 && Point + 1 > 21 )
			{
				Point -= 11;
			}
			Point += 1;
		}
		else
		{
			Point += 11;
		}
	}

	return Point;
}

/******************************************************************************/
//	�ʃJ�[�h���v�l�\��
/******************************************************************************/
long	DispCardCalcOne( long UserFlag, long PolNo )
{
	//	���v�l�擾
	long Calc = CardCalcOne( UserFlag );
	long CalcAry[2] ={0};
	CalcAry[0] = Calc / 10;
	CalcAry[1] = Calc % 10;

	g_LibPol.SetPatNo( PolNo, PAT_GAME_NUMBER0 + CalcAry[0] );
	g_LibPol.SetPatNo( PolNo+1, PAT_GAME_NUMBER0 + CalcAry[1] );

	//	���v�l�\��
	g_LibPol.ChangeDisp( PolNo, ON );
	g_LibPol.ChangeDisp( PolNo+1, ON );

	return Calc;
}

/******************************************************************************/
//	�ʃJ�[�h���v�l�v�Z
/******************************************************************************/
long	CardCalcOne( long UserFlag )
{
	long Sel[HAND_NUM_MAX] ={NOT};
	for( int i=0; i<HAND_NUM_MAX; i++ )
	{
		if( Chara[ UserFlag ].SelectCard[ i ] == ON )
		{
			long Index = Chara[ UserFlag ].DeckIndex-HAND_NUM_MAX;
			Sel[i] = Chara[ UserFlag ].Deck[ Index+i ].Number;
		}
	}

	//	���v�l�擾
	return CardCalc( Sel );
}

/******************************************************************************/
//	�ʃ|�C���g���Z�l�\��
/******************************************************************************/
void	DispAddPointOne( long UserFlag, long PolNo )
{
	long	cScore[3] = {0};
	long	Max = 100;
	long	scr = g_GameMng.PointAdd[UserFlag];

	if( scr < 0 )
	{
		scr *= -1;
		g_LibPol.SetPatNo( PolNo, PAT_GAME_MINUS );
	}
	else
	{
		g_LibPol.SetPatNo( PolNo, PAT_GAME_PLUS );
	}

	for( int i=0; i<3; i++ )
	{
		cScore[i] = scr / Max;
		scr %= Max;
		Max /=10;
	}

	g_LibPol.ChangeDisp( PolNo, ON );
	PolNo ++;
	for( int i=0; i<3; i++ )
	{
		g_LibPol.ChangeDisp( PolNo + i, ON );
		g_LibPol.SetPatNo( PolNo + i, PAT_GAME_NUMBER0 + cScore[i] );
	}

	//	�\���ݒ�
	if( cScore[0] == 0 )
	{
		g_LibPol.ChangeDisp( PolNo, OFF );
	}

	for( int i=1; i<2; i++ )
	{
		if( cScore[i] == 0 )
		{
			if( cScore[i-1] == 0 && g_LibPol.GetInfo( PolNo + i-1 ).Disp == OFF )
			{
				g_LibPol.ChangeDisp( PolNo + i, OFF );
			}
		}
	}
}
