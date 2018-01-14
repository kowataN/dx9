/************************************************************************************************/
//	ショットメイン
//		ソースファイル
/************************************************************************************************/


//----------------------------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------------------------//
#include	"dx8_lib.h"
#include	"dx8_anm.h"
#include	"dx8_pad.h"

#include	"main.h"
#include	"shot.h"



//----------------------------------------------------------------------------------------------//
//	デファイン
//----------------------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------------------//
//	グローバル
//----------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	カラーテーブル
COL_TBL	ColTbl[] =
{
	//	0
	{	255,255,0,0	},
	//	1
	{	255,0,255,0	},
	//	2
	{	255,0,0,255	},
	//	3
	{	255,255,255,0 },
};


//----------------------------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------------------------//

//	ショット構造体
SHOT_TBL	SHOT[SHOT_MAX];

SHOT_INIT_TBL	SIT[SHOT_MAX] =
{
	//	5way
	{ 24,24, 0 },
	{ 32,16, 0 },
	{ 40,8, 0 },
	{ 16,32, 0 },
	{ 8,40, 0 },

	//	5way
	{ 16,16, 1 },
	{ 16,16, 1 },
	{ 16,16, 1 },
	{ 16,16, 1 },
	{ 16,16, 1 },

	//	3way
	{ 32,32, 2 },
	{ 32,32, 2 },
	{ 32,32, 2 },

	//	1way
	{ 128,128, 3 },

	//	終端
	{	-1	},
};




//----------------------------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------------------------//
void	ShotInitAll( void );			//	ショット初期化（全体）
void	ShotInit( SHOT_TBL *shot );		//	ショット初期化（単体）
void	ShotLoop( void );				//	ショットループ
void	ShotMain( long no );			//	ショットメイン
long	ShotHitChk( long no );			//	ショットの当たり判定
void	ShotDirSet( long no );			//	角度補正

void	ShotSet( double x, double y, double rot, double speed );

void	ShotNormalSet( double speed, double rev );
void	ShotWideSet( double speed, long way, double rot_rev );
void	ShotCreateSet( double speed );
void	ShotSplineSet( double speed, double rot_add, double rev );
void	ShotHomingSet( double speed, double rev );

void	ShotSetUpLoop( long no );
void	ShotHomingLoop( long no );
void	ShotSplineLoop( long no );

short	ShotSearch( u_long atr );

long	ShotGetNo( long no );

long	HitCheck( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
void	WallHitCheck( long no );





/************************************************************************************************/
//	ショット初期化（全体）
//		戻り値：
//				なし
//		引数：
//				なし
/************************************************************************************************/
void	ShotInitAll( void )
{
	short	i;

	for( i=0; i<SHOT_MAX; i++ )
		ShotInit( &SHOT[i] );
}



/************************************************************************************************/
//	ショット初期化（単体）
//		戻り値：
//				なし
//		引数：
//				ショット番号
/************************************************************************************************/
void	ShotInit( SHOT_TBL *shot )
{
	shot->use = OFF;
	shot->disp = OFF;

	shot->atr = 0;

	shot->px = shot->py = 0;
	shot->bx = shot->by = 0;
	shot->mx = shot->my = 0;

	shot->ct = 0;

	shot->disp_ct = 0;

	shot->h = shot->w = 0;

	PolyInit( shot->pol_no );
	shot->pol_no = 0;
}



/************************************************************************************************/
//	ショットループ
//		戻り値：
//				なし
//		引数：
//				なし
/************************************************************************************************/
void	ShotLoop( void )
{
	short	i;

	for( i=0; i<SHOT_MAX; i++ )
	{
		if( !SHOT[ i ].use )
			continue;

		WallHitCheck( i );
		ShotMain( i );
/*
		if( ShotHitChk(i) )
		{
			TXT[1].disp = ON;
			Mode ++;
		}
*/
	}
}



/************************************************************************************************/
//	ショットメイン
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/************************************************************************************************/
void	ShotMain( long no )
{
	SHOT[ no ].px += SHOT[ no ].mx;
	SHOT[ no ].py += SHOT[ no ].my;
	POL[ no ].px = SHOT[ no ].px;
	POL[ no ].py = SHOT[ no ].py;

	//	汎用カウンタ加算
	SHOT[ no ].disp_ct ++;
}



/************************************************************************************************/
//	ショット当たり判定
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/************************************************************************************************/
long	ShotHitChk( long no )
{
	double	x1,y1,x2,y2,x3,y3,x4,y4;


	//	座標１取得
	x1 = SHOT[ no ].px;
	y1 = SHOT[ no ].py;
	x2 = x1 + SHOT[ no ].w;
	y2 = y1 + SHOT[ no ].h;

	//	座標２取得
	x3 = POL[ SHOT_MAX ].px;
	y3 = POL[ SHOT_MAX ].py;
	x4 = x3 + POL[ SHOT_MAX ].w;
	y4 = y3 + POL[ SHOT_MAX ].h;


	//	当たり判定
	if( HitCheck( x1,y1,x2,y2, x3,y3,x4,y4  ) )
	{
		ShotInit( &SHOT[no] );
		return( 1 );
	}

	return( 0 );
}



/************************************************************************************************/
//	角度補正
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/************************************************************************************************/
void	ShotDirSet( long no )
{
	//	移動量取得
	SHOT[ no ].mx = MoveGetX( SHOT[no].speed, SHOT[no].rot_r );
	SHOT[ no ].my = MoveGetY( SHOT[no].speed, SHOT[no].rot_r );
}



/************************************************************************************************/
//	ショットセット
//		戻り値：
//				なし
//		引数：
//				x,y			座標
//				rot			角度
//				speed		弾速
/************************************************************************************************/
void	ShotSet( double x, double y, double rot, double speed )
{
	long	shot;
	short	w,h,col;

	shot = ShotGetNo( 0 );

	//	角度補正
	if( rot > 360 )
		rot -= 360;
	else if( rot < 0 )
		rot += 360;

	SHOT[ shot ].use = ON;

	SHOT[ shot ].speed = speed;

	SHOT[ shot ].rot_d = rot;
	SHOT[ shot ].rot_r = DefToRad( rot );

	SHOT[ shot ].ct = 0;

	SHOT[ shot ].w = SIT[ shot ].w;
	SHOT[ shot ].h = SIT[ shot ].h;

	ShotDirSet( shot );

	w = SIT[ shot ].w;
	h = SIT[ shot ].h;
	col = SIT[ shot ].col;
	PolySet( shot, w,h, x,y,0.5, ATR_NONE );
	PolyColSet( shot, ColTbl[col].a,ColTbl[col].r,ColTbl[col].g,ColTbl[col].b );
}



/************************************************************************************************/
//	ノーマルショットセット
//		戻り値：
//				なし
//		引数：
//				speed		弾速
//				atr			アトリビュート
/************************************************************************************************/
void	ShotNormalSet( double speed, double rev )
{
	double	rot,x,y;

	//	角度
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot += DefToRad( rev );

	//	ショット発生
	x = 0 + cos( rot );
	y = 0 - sin( rot );
	ShotSet( x,y, RadToDef(rot), speed );
}



/************************************************************************************************/
//	拡散ショット
//		戻り値：
//				なし
//		引数：
//				speed		弾速
//				way			拡散数
//				ret_rev		補正角度
/************************************************************************************************/
void	ShotWideSet( double speed, long way, double rot_rev )
{
	long	i;
	double	rot,x,y,way_rot,i_rot,rad;

	//	角度
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot += DefToRad( rot_rev+11 );

	// 弾発生
	if( way & 1 == 1 )
	{
		// 奇数ＷＡＹ
		x = 0 + cos( rot );
		y = 0 - sin( rot );
		ShotSet( x,y, RadToDef(rot), speed );

		i_rot = 0;
	}
	else
	{
		// 偶数ＷＡＹ
		i_rot = DefToRad( (360 / way) / 2 );
	}

	rad = DefToRad( (360 / way) );
	for( i=0; i<way/2; i++ )
	{
		way_rot = rad * (i+1);

		x = 0 + cos( rot - way_rot + i_rot );
		y = 0 - sin( rot - way_rot + i_rot );
		ShotSet( x,y, RadToDef(rot - way_rot + i_rot), speed );

		x = 0 + cos( rot + way_rot - i_rot );
		y = 0 - sin( rot + way_rot - i_rot );
		ShotSet( x,y, RadToDef(rot + way_rot - i_rot), speed );
	}
}



/************************************************************************************************/
//	設置式のショット
//		戻り値：
//				なし
//		引数：
//				speed	弾速
/************************************************************************************************/
void	ShotCreateSet( double speed )
{
	double	rot;

	//	角度
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );

	ShotSet( 0,0, RadToDef(rot), speed );
}



/************************************************************************************************/
//	スプラインショット
//		戻り値：
//				なし
//		引数：
//				shot		自分の番号
//				speed		弾速
//				rot_add		角度加算値
//				rev			補正
/************************************************************************************************/
void	ShotSplineSet( double speed, double rot_add, double rev )
{
	double	rot;

	//	角度
	rot = RotGet( 0,0, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot += DefToRad( rev );
	ShotSet( 0,0, RadToDef(rot), speed );
}



/************************************************************************************************/
//	ホーミングショット
//		戻り値：
//				なし
//		引数：
//				shpot	自分の番号
//				speed	弾速
//				rev		補正角度
/************************************************************************************************/
void	ShotHomingSet( double speed, double rev )
{
	double	rot;

	//	角度
	rot = RotGet( 0,0, POL[SHOT_MAX].px,SHOT[SHOT_MAX].py );
	rot += DefToRad( rev );

	ShotSet( 0,0, RadToDef(rot), speed );
}



/************************************************************************************************/
//	ショットループ（設置式）
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/************************************************************************************************/
void	ShotLoopSetUp( long no )
{
/*
	short	unit_no;

	if( SHOT[ no ].disp_ct > 60 )
	{
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 7, SHOT_ATR_SHOT_SPECIAL1, 32, 0, 1-unit_no, PAT_GAME_SHOT );
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 7, SHOT_ATR_SHOT_SPECIAL1, 32, 16, 1-unit_no, PAT_GAME_SHOT );
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 6, SHOT_ATR_SHOT_SPECIAL1, 32, 8, 1-unit_no, PAT_GAME_SHOT );
		ShotWideSet( no, UNIT[unit_no].SHOT_no, 6, SHOT_ATR_SHOT_SPECIAL1, 32, 24, 1-unit_no, PAT_GAME_SHOT );
		SHOTInit( no );
	}
*/
}


/************************************************************************************************/
//	ショットループ（ホーミング）
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/************************************************************************************************/
void	ShotHomingLoop( long no )
{
	double	rot;

	//	ショットと対象の角度を取得
	rot = RotGet( SHOT[no].px,SHOT[no].py, POL[SHOT_MAX].px,POL[SHOT_MAX].py );
	rot = RadToDef( rot );
	if( rot >= 360 )	rot -= 360;
	else if( rot <= 0 )	rot += 360;

	//	実処理
	if( SHOT[ no ].disp_ct >= 10 )
	{
		rot -= SHOT[ no ].rot_d;

		if( (rot>0 && rot<=180)|| (rot<0 && rot<=-180) )
			SHOT[ no ].rot_d += 4;
		else if( (rot>0 && rot>180) || (rot<0 && rot>-180) )
			SHOT[ no ].rot_d -= 4;

		if( SHOT[ no ].rot_d >= 360)	SHOT[ no ].rot_d -= 360;
		else if( SHOT[ no ].rot_d <= 0)	SHOT[ no ].rot_d += 360;

//		SHOT[ no ].rot = SHOT[ no ].rot_d + 90;
		SHOT[ no ].rot_r = DefToRad( SHOT[ no ].rot_d );

		SHOT[ no ].disp_ct = 0;

		//	徐々に加速
		SHOT[ no ].speed += 0.1;
	}

	//	90フレーム後ホーミングしなくなる
	if( ( SHOT[ no ].ct >= 90 )
	|| ( DistanceGet( SHOT[no].px,SHOT[no].py, POL[SHOT_MAX].px,POL[SHOT_MAX].py ) <= 32 ) )
	{
		SHOT[ no ].atr = SHOT_ATR_NORMAL;

		SHOT[ no ].ct = 0;
	}
	else
		SHOT[ no ].ct ++;
}



/************************************************************************************************/
//	ショットループ（スプライン）
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/************************************************************************************************/
void	ShotLoopSpline( long no )
{
}



/************************************************************************************************/
//	ショットの検索
//		戻り値：
//				１：ある、０：ない
//		引数：
//				アトリビュート
/************************************************************************************************/
short	ShotSearch( u_long atr )
{
	long	i;
	short	ret = 0;

	for( i=0; i<SHOT_MAX; i++ )
	{
		if( ( SHOT[ i ].atr & atr ) == atr )
			ret = ON;
	}

	return( ret );
}



/************************************************************************************************/
//	ショット空き番号取得
/************************************************************************************************/
long	ShotGetNo( long no )
{
	long	i;
	for( i=no; i<SHOT_MAX; i++ )
	{
		if( !SHOT[i].use )
			return( i );
	}

	return( NOT );
}



/************************************************************************************************/
//	当り判定
/************************************************************************************************/
long	HitCheck( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 )
{
	if( ( x2 > x3 ) && ( x1 < x4 )
	&& ( y2 > y3 ) && ( y1 < y4 ) )
	{
		return( ON );
	}

	return( OFF );
}



/************************************************************************************************/
//	壁判定
/************************************************************************************************/
void	WallHitCheck( long no )
{
	//	左端判定
	if( SHOT[no].px < 0 )
	{
		SHOT[no].px = 0;
		SHOT[no].mx *= -1;
	}

	//	右端判定
	if( SHOT[no].px+SHOT[no].w >= 640 )
	{
		SHOT[no].px = 640 - SHOT[no].w;
		SHOT[no].mx *= -1;
	}

	//	上端判定
	if( SHOT[no].py < 0 )
	{
		SHOT[no].py = 0;
		SHOT[no].my *= -1;
	}

	//	下端判定
	if( SHOT[no].py+SHOT[no].h >= 480 )
	{
		SHOT[no].py = 480 - SHOT[no].h;
		SHOT[no].my *= -1;
	}
}
