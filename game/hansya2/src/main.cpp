/************************************************************************************************/
//	ライブラリ試し
/************************************************************************************************/

//----------------------------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------------------------//
#include	"dx8_lib.h"

#include	"main.h"
#include	"shot.h"



//----------------------------------------------------------------------------------------------//
//	デファイン
//----------------------------------------------------------------------------------------------//
#define	MOV_SPEED	3



//----------------------------------------------------------------------------------------------//
//	グローバル
//----------------------------------------------------------------------------------------------//
long	Mode = 0;

char	AnmBuf[2][1024*1024];

long	Flag=0;
long	TimeCt,FrmCt;
short	Level,ShotLv;

short	TimeTbl[] =
{
	0,
	5,
	10,
	15,
	20,
};



//----------------------------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------------------------//
void	MainInit( void );
void	GameInit( void );
void	GameMain( void );

void	MyChrMove( void );
void	MyChrClip( void );
void	FrmSet( void );
void	ShotCreate( void );




/************************************************************************************************/
//	メイン
/************************************************************************************************/
void	MainLoop( void )
{
	switch( Mode )
	{
		//	メイン初期化
		case	0:

			MainInit();

			Mode = 1;

		//	ゲーム初期化
		case	1:

			GameInit();

			Mode = 2;

		//	ゲームメイン
		case	2:

			GameMain();

			break;

		//	ゲームオーバー
		case	3:

			if( KEY.trg[ DIK_SPACE ] )
				Mode = 0;

			break;
	}
}



/************************************************************************************************/
//	初期化
/************************************************************************************************/
void	MainInit( void )
{
	Dx8LibInit();
}



/************************************************************************************************/
//	ゲーム初期化
/************************************************************************************************/
void	GameInit( void )
{
	ShotInitAll();

	PolySet( SHOT_MAX, 16,16, 480,360,1.0, ATR_NONE );
	PolySet( SHOT_MAX+1, 640,480, 0,0,1.0, ATR_BOX );
	PolyColSet( SHOT_MAX+1, 255,255,0,0 );

	TextSet( 0, 0,0, 255,0,255,255, "" );
	TextSet( 1, 320,240, 255,255,0,0, "GameOver" );
	TXT[1].disp = OFF;

	TimeCt = FrmCt = 0;

	Level = 1;
	ShotLv = 0;

	ShotCreate();
}



/************************************************************************************************/
//	ゲームメイン
/************************************************************************************************/
void	GameMain( void )
{
	MyChrMove();

	MyChrClip();

	FrmSet();

	ShotLoop();

	if( FrmCt >= 60 )
	{
		TimeCt ++;
		FrmCt = 0;

		if( TimeCt == TimeTbl[Level] )
		{
			Level ++;
			ShotCreate();
		}
	}
	else
		FrmCt ++;

	sprintf( TXT[0].str, "時間：%ld 秒  レベル：%d", TimeCt, Level );
}



/************************************************************************************************/
//	キャラクターの移動
/************************************************************************************************/
void	MyChrMove( void )
{
	if( KEY.push[ DIK_UP ] || JOY[0].push[ JOY_UP ] )
	{
		if( KEY.push[ DIK_LEFT ] || JOY[0].push[ JOY_LEFT ] )
		{
			POL[ SHOT_MAX ].px -= 0.7 * MOV_SPEED;
			POL[ SHOT_MAX ].py -= 0.7 * MOV_SPEED;
		}
		else
		{
			if( KEY.push[ DIK_RIGHT ] || JOY[0].push[ JOY_RIGHT ] )
			{
				POL[ SHOT_MAX ].px += 0.7 * MOV_SPEED;
				POL[ SHOT_MAX ].py -= 0.7 * MOV_SPEED;
			}
			else
			{
				POL[ SHOT_MAX ].py -= MOV_SPEED;
			}
		}
	}
	else
	{
		if( KEY.push[ DIK_DOWN ] || JOY[0].push[ JOY_DOWN ] )
		{
			if( KEY.push[ DIK_LEFT ] || JOY[0].push[ JOY_LEFT ] )
			{
				POL[ SHOT_MAX ].px -= 0.7 * MOV_SPEED;
				POL[ SHOT_MAX ].py += 0.7 * MOV_SPEED;
			}
			else
			{
				if( KEY.push[ DIK_RIGHT ] || JOY[0].push[ JOY_RIGHT ] )
				{
					POL[ SHOT_MAX ].px += 0.7 * MOV_SPEED;
					POL[ SHOT_MAX ].py += 0.7 * MOV_SPEED;
				}
				else
				{
					POL[ SHOT_MAX ].py += MOV_SPEED;
				}
			}
		}
		else
		{
			if( KEY.push[ DIK_LEFT ] || JOY[0].push[ JOY_LEFT ] )
			{
				POL[ SHOT_MAX ].px -= MOV_SPEED;
			}
			else
			{
				if( KEY.push[ DIK_RIGHT ] || JOY[0].push[ JOY_RIGHT ] )
				{
					POL[ SHOT_MAX ].px += MOV_SPEED;
				}
			}
		}
	}
}



/************************************************************************************************/
//	クリッピング
/************************************************************************************************/
void	MyChrClip( void )
{
	if( POL[SHOT_MAX].px < POL[SHOT_MAX+1].px )
	{
		POL[SHOT_MAX].px = POL[SHOT_MAX+1].px;
	}

	if( POL[SHOT_MAX].py < POL[SHOT_MAX+1].py )
	{
		POL[SHOT_MAX].py = POL[SHOT_MAX+1].py;
	}

	if( POL[SHOT_MAX].px+POL[SHOT_MAX].w > POL[SHOT_MAX+1].w+POL[SHOT_MAX+1].px )
	{
		POL[SHOT_MAX].px = POL[SHOT_MAX+1].w + POL[SHOT_MAX+1].px - POL[SHOT_MAX].w;
	}

	if( POL[SHOT_MAX].py+POL[SHOT_MAX].h > POL[SHOT_MAX+1].h+POL[SHOT_MAX+1].py )
	{
		POL[SHOT_MAX].py = POL[SHOT_MAX+1].h + POL[SHOT_MAX+1].py - POL[SHOT_MAX].h;
	}
}



/************************************************************************************************/
//	フレームの拡大縮小
/************************************************************************************************/
void	FrmSet( void )
{
	POL[ SHOT_MAX+1 ].px = 8 * Level;
	POL[ SHOT_MAX+1 ].py = 8 * Level;

	POL[ SHOT_MAX+1 ].w = 640 - (8 * Level*2);
	POL[ SHOT_MAX+1 ].h = 480 - (8 * Level*2);

	//	クリッピング
	if( POL[ SHOT_MAX+1 ].px >= 160 )
		POL[ SHOT_MAX+1 ].px = 160;

	if( POL[ SHOT_MAX+1 ].py >= 120 )
		POL[ SHOT_MAX+1 ].px = 120;

	if( POL[ SHOT_MAX+1 ].w <= 320 )
		POL[ SHOT_MAX+1 ].w = 320;

	if( POL[ SHOT_MAX+1 ].h <= 240 )
		POL[ SHOT_MAX+1 ].h = 240;
}



/************************************************************************************************/
//	ショット発生
/************************************************************************************************/
void	ShotCreate( void )
{
	switch( ShotLv )
	{
		case	0:
//			ShotNormalSet( 4, 0 );
			ShotWideSet( 4, 5,0 );
			ShotLv ++;
			break;

		case	1:
			ShotWideSet( 4, 5,0 );
			ShotLv ++;
			break;

		case	2:
			ShotWideSet( 3, 3, 0 );
			ShotLv ++;
			break;

		case	3:
			ShotNormalSet( 3, 0 );
			ShotLv ++;
			break;

		default:
			break;
	}
}
