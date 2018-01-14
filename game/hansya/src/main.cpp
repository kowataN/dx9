/************************************************************************************************/
//	ライブラリ試し
/************************************************************************************************/

//----------------------------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------------------------//
#include	"dx8_lib.h"
#include	"dx8_anm.h"
#include	"dx8_hit.h"

#include	"main.h"



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
short	Level;

short	TimeTbl[] =
{
	5,
	10,
	13,
	14,
};



//----------------------------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------------------------//
void	MainInit( void );
void	GameInit( void );
void	GameMain( void );

void	ObjInit( void );
void	ObjMove( void );
void	MyChrMove( void );
void	MyChrClip( void );
void	HitChk( long no );
short	ObjHitChk( void );
void	FrmSet( void );



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
				Mode = 1;

			break;
	}
}


/************************************************************************************************/
//	初期化
/************************************************************************************************/
void	MainInit( void )
{
}


/************************************************************************************************/
//	ゲーム初期化
/************************************************************************************************/
void	GameInit( void )
{
	ObjInit();

	PolySet( OBJ_MAX, 16,16, 480,360,1.0, ATR_NONE );
	PolySet( OBJ_MAX+1, 640,480, 0,0,1.0, ATR_BOX );
	PolyColSet( OBJ_MAX+1, 255,255,0,0 );

	TextSet( 0, 0,0, 255,0,255,255, "" );
	TextSet( 1, 320,240, 255,255,0,0, "GameOver" );
	TXT[1].disp = OFF;

	TimeCt = FrmCt = 0;

	Level = 0;
}


/************************************************************************************************/
//	ゲームメイン
/************************************************************************************************/
void	GameMain( void )
{
	long	i;

	ObjMove();

	MyChrMove();

	MyChrClip();

	FrmSet();

	for( i=0; i<TimeTbl[Level]; i++ )
		POL[i].disp = 1;;

	if( ObjHitChk() )
	{
		TXT[1].disp = ON;
		Mode ++;
	}

	if( FrmCt >= 60 )
	{
		TimeCt ++;
		if( (TimeCt % 10 ) == 1 )
		{
			Level ++;
			if( Level >= 99 )
				Level = 99;
		}
		FrmCt = 0;
	}
	else
		FrmCt ++;

	sprintf( TXT[0].str, "時間：%ld 秒　　レベル：%ld", TimeCt, Level );

}


/************************************************************************************************/
//	ゲームメイン
/************************************************************************************************/
void	ObjMove( void )
{
	short	i;

	for( i=0; i<TimeTbl[Level]; i++ )
	{
		if( OBJ[i].ct >= OBJ[i].time )
		{
			POL[i].disp = ON;
			POL[i].px += OBJ[i].mx;
			POL[i].py += OBJ[i].my;

			HitChk( i );

			OBJ[i].ct = 0;
		}

		OBJ[i].ct++;
	}
}


/************************************************************************************************/
//	オブジェクトの初期化
/************************************************************************************************/
void	ObjInit( void )
{
	long	i,col;

	for( i=0; i<OBJ_MAX; i++ )
	{
		if( OBJ_INIT[i].mx < 0 )
			return;

		OBJ[i].mx = OBJ_INIT[i].mx;
		OBJ[i].my = OBJ_INIT[i].my;
		OBJ[i].time = OBJ_INIT[i].time;
		OBJ[i].w = OBJ_INIT[i].w;
		OBJ[i].h = OBJ_INIT[i].h;
		OBJ[i].type = OBJ_INIT[i].type;
		OBJ[i].ct = OBJ_INIT[i].ct;

		if( OBJ[ i ].mx < 0 )
			continue;

		PolySet( i, OBJ[i].w,OBJ[i].h, 0,0,1.0, ATR_NONE );

		//	色の設定
		col = OBJ[i].type;
		PolyColSet( i, ColTbl[col].a,ColTbl[col].r,ColTbl[col].g,ColTbl[col].b );

		POL[i].disp = OFF;
	}
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
			POL[ OBJ_MAX ].px -= 0.7 * MOV_SPEED;
			POL[ OBJ_MAX ].py -= 0.7 * MOV_SPEED;
		}
		else
		{
			if( KEY.push[ DIK_RIGHT ] || JOY[0].push[ JOY_RIGHT ] )
			{
				POL[ OBJ_MAX ].px += 0.7 * MOV_SPEED;
				POL[ OBJ_MAX ].py -= 0.7 * MOV_SPEED;
			}
			else
			{
				POL[ OBJ_MAX ].py -= MOV_SPEED;
			}
		}
	}
	else
	{
		if( KEY.push[ DIK_DOWN ] || JOY[0].push[ JOY_DOWN ] )
		{
			if( KEY.push[ DIK_LEFT ] || JOY[0].push[ JOY_LEFT ] )
			{
				POL[ OBJ_MAX ].px -= 0.7 * MOV_SPEED;
				POL[ OBJ_MAX ].py += 0.7 * MOV_SPEED;
			}
			else
			{
				if( KEY.push[ DIK_RIGHT ] || JOY[0].push[ JOY_RIGHT ] )
				{
					POL[ OBJ_MAX ].px += 0.7 * MOV_SPEED;
					POL[ OBJ_MAX ].py += 0.7 * MOV_SPEED;
				}
				else
				{
					POL[ OBJ_MAX ].py += MOV_SPEED;
				}
			}
		}
		else
		{
			if( KEY.push[ DIK_LEFT ] || JOY[0].push[ JOY_LEFT ] )
			{
				POL[ OBJ_MAX ].px -= MOV_SPEED;
			}
			else
			{
				if( KEY.push[ DIK_RIGHT ] || JOY[0].push[ JOY_RIGHT ] )
				{
					POL[ OBJ_MAX ].px += MOV_SPEED;
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
	if( POL[OBJ_MAX].px < POL[OBJ_MAX+1].px )
	{
		POL[OBJ_MAX].px = POL[OBJ_MAX+1].px;
	}

	if( POL[OBJ_MAX].py < POL[OBJ_MAX+1].py )
	{
		POL[OBJ_MAX].py = POL[OBJ_MAX+1].py;
	}

	if( POL[OBJ_MAX].px+POL[OBJ_MAX].w > POL[OBJ_MAX+1].w+POL[OBJ_MAX+1].px )
	{
		POL[OBJ_MAX].px = POL[OBJ_MAX+1].w + POL[OBJ_MAX+1].px - POL[OBJ_MAX].w;
	}

	if( POL[OBJ_MAX].py+POL[OBJ_MAX].h > POL[OBJ_MAX+1].h+POL[OBJ_MAX+1].py )
	{
		POL[OBJ_MAX].py = POL[OBJ_MAX+1].h + POL[OBJ_MAX+1].py - POL[OBJ_MAX].h;
	}
}


/************************************************************************************************/
//	当たり判定
/************************************************************************************************/
void	HitChk( long no )
{
	//	左端判定
	if( POL[no].px < 0 )
	{
		POL[no].px = 0;

		OBJ[no].mx *= -1;
	}

	//	右端判定
	if( POL[no].px+POL[no].w > 640 )
	{
		POL[no].px = 640 - POL[no].w;

		OBJ[no].mx *= -1;
	}

	//	上端判定
	if( POL[no].py < 0 )
	{
		POL[no].py = 0;

		OBJ[no].my *= -1;
	}

	//	下端判定
	if( POL[no].py+POL[no].h > 480 )
	{
		POL[no].py = 480 - POL[no].h;

		OBJ[no].my *= -1;
	}
}


/************************************************************************************************/
//	キャラとの当たり判定
/************************************************************************************************/
short	ObjHitChk( void )
{
	long	i;
	short	x1,y1,x2,y2,x3,y3,x4,y4;


	x1 = (short)POL[OBJ_MAX].px;
	y1 = (short)POL[OBJ_MAX].py;
	x2 = x1 + (short)POL[OBJ_MAX].w;
	y2 = y1 + (short)POL[OBJ_MAX].h;


	for( i=0; i<TimeTbl[Level]; i++ )
	{
		x3 = (short)POL[i].px;
		y3 = (short)POL[i].py;
		x4 = x3 + (short)OBJ[i].w;
		y4 = y3 + (short)OBJ[i].h;

		if( ( x2 > x3 ) && ( x1 < x4 )
		&& ( y2 > y3 ) && ( y1 < y4 ) )
		{
			return	1;
		}
	}

	return	0;
}


/************************************************************************************************/
//	フレームの拡大縮小
/************************************************************************************************/
void	FrmSet( void )
{
	POL[ OBJ_MAX+1 ].px = 16 * Level;
	POL[ OBJ_MAX+1 ].py = 16 * Level;

	POL[ OBJ_MAX+1 ].w = 640 - (16 * Level*2);
	POL[ OBJ_MAX+1 ].h = 480 - (16 * Level*2);
}
