/******************************************************************************/
//	ショットメイン
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./GameMain.h"
#include	"./Obj.h"
#include	"./Shot.h"
#include	"./CharControl.h"

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	ShotInitAll( void );			//	ショット初期化（全体）
void	ShotLoop( void );				//	ショットループ
void	ShotMain( long no );			//	ショットメイン
void	ShotHitChk( long no );			//	ショットの当たり判定
void	ShotDirSet( long no );			//	角度補正
void	ShotSet( double x, double y, double rot, double speed, ULong atr, short chara, long pat );
void	ShotSetNormal( long obj1, long obj2, double speed, ULong atr, double rev, short chr, long pat );
void	ShotSetWide( long obj1, long obj2, double speed, ULong atr, long way, double rot_rev, short chr, long pat );
void	ShotSetCreate( long obj1, long obj2, double speed, ULong atr, short chr, long pat );
void	ShotSetSpline( long obj1, long obj2, double speed, ULong atr, double rot_add, double rev, short chr, long pat );
void	ShotSetHoming( long obj1, long obj2, double speed, ULong atr, double rev, short chr, long pat );
void	ShotLoopSetUp( long no );
void	ShotLoopHoming( long no );
void	ShotLoopSpline( long no );
short	ShotSearch( ULong atr );

void	NormarlShot( void );
void	NormarlShotPush( void );
void	SpecialShot1( void );
void	SpecialShot2( void );
void	ExtraShot1( void );
void	ExtraShot2( void );
void	*ShotTbl[] =
{
	NormarlShot,
	NormarlShotPush,
	SpecialShot1,
	SpecialShot2,
	ExtraShot1,
	ExtraShot2,
};



/******************************************************************************/
//	ショット初期化（全体）
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ShotInitAll( void )
{
	for( long i=OBJ_GAME_SHOT; i<OBJ_MAX; i++ )
	{
		ObjInit( i );
	}
}

/******************************************************************************/
//	ショットループ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ShotLoop( void )
{
	if( SYS.pause_disp )
	{
		return;
	}

	for( long i=OBJ_GAME_SHOT; i<OBJ_MAX; i++ )
	{
		if( !OBJ[ i ].use )
		{
			continue;
		}

		if( ( OBJ[ i ].type & OBJ_TYPE_SHOT ) == OBJ_TYPE_SHOT )
		{
			ShotMain( i );
			ShotDirSet( i );
			ShotHitChk( i );
		}
	}
}

/******************************************************************************/
//	ショットメイン
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
void	ShotMain( long no )
{
	u_long	ATR;
	ATR = OBJ[ no ].atr & ( OBJ_ATR_SHOT_RANK );
	switch( ATR )
	{
		case	OBJ_ATR_SHOT_NORMAL:
			break;

		case	OBJ_ATR_SHOT_SPECIAL1:
			break;

		case	OBJ_ATR_SHOT_SPECIAL2:
			break;

		case	OBJ_ATR_SHOT_EXTRA1:
			break;

		case	OBJ_ATR_SHOT_EXTRA2:
			break;

	}

	ATR = OBJ[ no ].atr & ( OBJ_ATR_SHOT_TYPE );
	switch( ATR )
	{
		//	ホーミング
		case	OBJ_ATR_SHOT_HOMING:
			ShotLoopHoming( no );
			break;

		//	設置式
		case	OBJ_ATR_SHOT_SET_UP:
			ShotLoopSetUp( no );
			break;

		//	スプライン
		case	OBJ_ATR_SHOT_SPLINE:
			ShotLoopSpline( no );
			break;
	}

	ATR = OBJ[ no ].atr & ( OBJ_ATR_ROT_TYPE );
	switch( ATR )
	{
		case	OBJ_ATR_ROT_CHANGE:
			OBJ[ no ].rot += 30;
			break;
	}

	//	汎用カウンタ加算
	OBJ[ no ].disp_ct ++;
}

/******************************************************************************/
//	ショット当たり判定
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
void	ShotHitChk( long no )
{
	long	chara;
	double	x,y;

	if( !OBJ[ no ].use )
	{
		return;
	}

	//	ユニット番号取得
	if( ( OBJ[ no ].atr & OBJ_ATR_UNIT1 ) == OBJ_ATR_UNIT1 )
	{
		chara = 1;
	}
	else if( ( OBJ[ no ].atr & OBJ_ATR_UNIT2 ) == OBJ_ATR_UNIT2 )
	{
		chara = 0;
	}

	//	相手ユニットの座標取得
	x = OBJ[ OBJ_GAME_UNIT + chara ].px;
	y = OBJ[ OBJ_GAME_UNIT + chara ].py;

	//	バズリチェック
	if( HitCheck( OBJ[no].px,OBJ[no].py,OBJ[no].hit_rect, x,y,UNIT[chara].buzz_rect/2 ) )
	{
		//	当たり判定
		if( HitCheck( OBJ[no].px,OBJ[no].py,OBJ[no].hit_rect, x,y,UNIT[chara].hit_rect  ) )
		{
			OBJ[no].disp = OFF;
			OBJ[no].buzz = ON;
			ObjInit( no );
			UNIT[ chara ].life -= 100;
//	デバッグ
#ifdef	__DEBUG__
			sprintf_s( TXT[100].str, "当たった" );
#endif
			return;
		}

		if( OBJ[no].buzz == ON )
		{
			return;
		}

		OBJ[no].buzz = ON;

//	デバッグ
#ifdef	__DEBUG__
		sprintf_s( TXT[100].str, "バズった" );
#endif

////////////////////////////////////////////////////
//	↓仮設定
		u_long ATR = OBJ[no].atr & ( OBJ_ATR_SHOT_RANK );
		switch( ATR )
		{
			case	OBJ_ATR_SHOT_NORMAL:
				UNIT[ chara ].gage += 10;
				sprintf_s( TXT[ 5+chara ].Str, MAX_PATH, "+10" );
				break;

			case	OBJ_ATR_SHOT_SPECIAL1 :
				UNIT[ chara ].gage += 20;
				sprintf_s( TXT[ 5+chara ].Str, MAX_PATH, "+20" );
				break;

			case	OBJ_ATR_SHOT_SPECIAL2:
				UNIT[ chara ].gage += 30;
				sprintf_s( TXT[ 5+chara ].Str, MAX_PATH, "+30" );
				break;

			case	OBJ_ATR_SHOT_EXTRA1:
				UNIT[ chara ].gage += 40;
				sprintf_s( TXT[ 5+chara ].Str, MAX_PATH, "+40" );
				break;

			case	OBJ_ATR_SHOT_EXTRA2:
				UNIT[ chara ].gage += 50;
				sprintf_s( TXT[ 5+chara ].Str, MAX_PATH, "+50" );
				break;
		}

		g_LibText.SetColor( 5+chara, 255,255,255,0 );
		TXT[ 5+chara ].Disp = ON;
		g_LibText.SetTimer( 5+chara, 30 );
	}
//	↑仮設定
////////////////////////////////////////////////////
}

/******************************************************************************/
//	角度補正
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
void	ShotDirSet( long no )
{
	//	移動量取得
	OBJ[ no ].mx = GetMoveX( OBJ[no].speed, OBJ[no].rot_r );
	OBJ[ no ].my = GetMoveY( OBJ[no].speed, OBJ[no].rot_r );
}

/******************************************************************************/
//	ショットセット
//		戻り値：
//				なし
//		引数：
//				x,y			座標
//				rot			角度
//				speed		弾速
//				atr			アトリビュート
//				chara		キャラ番号
//				pat			パターン番号
/******************************************************************************/
void	ShotSet( double x, double y, double rot, double speed, ULong atr, short chara, long pat )
{
	long	obj = ObjGetNo( OBJ_GAME_SHOT );

	//	角度補正
	if( rot > 360 )
	{
		rot -= 360;
	}
	else if( rot < 0 )
	{
		rot += 360;
	}

	OBJ[ obj ].use = ON;
	OBJ[ obj ].atr = atr;

	OBJ[ obj ].speed = speed;

	OBJ[ obj ].rot_d = rot;
	OBJ[ obj ].rot_r = DegToRad( rot );

	OBJ[ obj ].buzz = OFF;

	OBJ[ obj ].type = OBJ_TYPE_SHOT;

	OBJ[ obj ].ct = 0;
	OBJ[ obj ].disp_ct = 0;

	ObjSet( obj, obj, pat, x,y,0.5, OBJ[ obj ].type, atr );

	if( chara == 0 )
	{
		OBJ[ obj ].atr |= OBJ_ATR_UNIT1;
	}
	else
	{
		OBJ[ obj ].atr |= OBJ_ATR_UNIT2;
	}

	/////////////////////////////////////////////////
	//	中心座標のセット
	switch( pat - PAT_GAME_SHOT )
	{
		case	0:
			ObjCenterSet( obj, 7,15 );
			OBJ[ obj ].hit_rect = 13;
			break;

		case	1:
			ObjCenterSet( obj, 7,15 );
			OBJ[ obj ].hit_rect = 13;
			break;

		case	2:
			ObjCenterSet( obj, 7,7 );
			OBJ[ obj ].hit_rect = 7;
			break;

		case	3:
			ObjCenterSet( obj, 7,7 );
			OBJ[ obj ].hit_rect = 7;
			break;

		case	4:
			ObjCenterSet( obj, 7,7 );
			OBJ[ obj ].hit_rect = 7;
			break;

		case	5:
			ObjCenterSet( obj, 7,7 );
			OBJ[ obj ].hit_rect = 7;
			break;

		case	6:
			break;

		case	7:
			break;

		case	8:
			break;
	}

	OBJ[ obj ].rot = rot + 90;
}

/******************************************************************************/
//	ノーマルショットセット
//		戻り値：
//				なし
//		引数：
//				obj1		自分の番号
//				obj2		相手の番号
//				speed		弾速
//				atr			アトリビュート
//				chr			キャラクター番号
//				pat			パターン番号
/******************************************************************************/
void	ShotSetNormal( long obj1, long obj2, double speed, ULong atr, double rev, short chr, long pat )
{
	double	rot,x,y;

	//	角度
	rot = GetRot( OBJ[obj1].px,OBJ[obj1].py, OBJ[obj2].px,OBJ[obj2].py );
	rot += DegToRad( rev );

	//	ショット発生
	x = OBJ[ obj1 ].px + cos( rot );
	y = OBJ[ obj1 ].py - sin( rot );
	ShotSet( x,y, RadToDeg(rot), speed, atr, chr, pat );
}

/******************************************************************************/
//	拡散ショット
//		戻り値：
//				なし
//		引数：
//				obj1		自分の番号
//				obj2		相手の番号
//				speed		弾速
//				atr			アトリビュート
//				way			拡散数
//				ret_rev		補正角度
//				chr			キャラクター番号
//				pat			パターン番号
/******************************************************************************/
void	ShotSetWide( long obj1, long obj2, double speed, ULong atr, long way, double rot_rev, short chr, long pat )
{
	double	rot,x,y,way_rot,i_rot,rad;

	if( (atr & OBJ_ATR_SHOT_SET_UP) == OBJ_ATR_SHOT_SET_UP )
	{
		sprintf_s( TXT[100].Str, "設置式だよ～   %ld", MAX_PATH, (1<<13) );
	}

	//	角度
	rot = GetRot( OBJ[obj1].px,OBJ[obj1].py, OBJ[obj2].px,OBJ[obj2].py );
	rot += DegToRad( rot_rev+11 );

	// 弾発生
	if( (way & 1) == 1 )
	{
		// 奇数ＷＡＹ
		x = OBJ[ obj1 ].px + cos( rot );
		y = OBJ[ obj1 ].py - sin( rot );
		ShotSet( x, y, RadToDeg(rot), speed, atr, chr, pat );

		i_rot = 0;
	}
	else
	{
		// 偶数ＷＡＹ
		i_rot = DegToRad( (360 / way) / 2 );
	}

	rad = DegToRad( (360 / way) );
	for( long i=0; i<way/2; i++ )
	{
		way_rot = rad * (i+1);

		x = OBJ[ obj1 ].px + cos( rot - way_rot + i_rot );
		y = OBJ[ obj1 ].py - sin( rot - way_rot + i_rot );
		ShotSet( x, y, RadToDeg(rot - way_rot + i_rot), speed, atr, chr, pat );

		x = OBJ[ obj1 ].px + cos( rot + way_rot - i_rot );
		y = OBJ[ obj1 ].py - sin( rot + way_rot - i_rot );
		ShotSet( x, y, RadToDeg(rot + way_rot - i_rot), speed, atr, chr, pat );
	}
}

/******************************************************************************/
//	設置式のショット
//		戻り値：
//				なし
//		引数：
//				obj1	自分の番号
//				obj2	相手の番号
//				speed	弾速
//				atr		アトリビュート
//				chr		キャラクター番号
//				pat		パターン番号
/******************************************************************************/
void	ShotSetCreate( long obj1, long obj2, double speed, ULong atr, short chr, long pat )
{
	double	rot,x,y;

	//	角度
	rot = GetRot( OBJ[obj1].px,OBJ[obj1].py, OBJ[obj2].px,OBJ[obj2].py );

	x = OBJ[ UNIT[chr].obj_no ].px;
	y = OBJ[ UNIT[chr].obj_no ].py;
	ShotSet( x, y, RadToDeg(rot), speed, atr, chr, pat );
}

/******************************************************************************/
//	スプラインショット
//		戻り値：
//				なし
//		引数：
//				obj1		自分の番号
//				obj2		相手の番号
//				speed		弾速
//				atr			アトリビュート
//				rot_add		角度加算値
//				rev			補正
//				chr			キャラクター番号
//				pat			パターン番号
/******************************************************************************/
void	ShotSetSpline( long obj1, long obj2, double speed, ULong atr, double rot_add, double rev, short chr, long pat )
{
	double	rot,x,y;

	//	角度
	rot = GetRot( OBJ[obj1].px,OBJ[obj1].py, OBJ[obj2].px,OBJ[obj2].py );
	rot += DegToRad( rev );

	x = OBJ[ UNIT[chr].obj_no ].px;
	y = OBJ[ UNIT[chr].obj_no ].py;
	ShotSet( x, y, RadToDeg(rot), speed, atr, chr, pat );
}

/******************************************************************************/
//	ホーミングショット
//		戻り値：
//				なし
//		引数：
//				obj1	自分の番号
//				obj2	相手の番号
//				speed	弾速
//				atr		アトリビュート
//				rev		補正角度
//				chr		キャラクター番号
//				pat		パターン番号
/******************************************************************************/
void	ShotSetHoming( long obj1, long obj2, double speed, ULong atr, double rev, short chr, long pat )
{
	double	rot,x,y;

	//	角度
	rot = GetRot( OBJ[obj1].px,OBJ[obj1].py, OBJ[obj2].px,OBJ[obj2].py );
	rot += DegToRad( rev );

	x = OBJ[ UNIT[chr].obj_no ].px;
	y = OBJ[ UNIT[chr].obj_no ].py;
	atr |= OBJ_ATR_SHOT_HOMING;
	ShotSet( x, y, RadToDeg(rot), speed, atr, chr, pat );
}

/******************************************************************************/
//	ショットループ（設置式）
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
void	ShotLoopSetUp( long no )
{
	short	unit_no;

	if( OBJ[ no ].disp_ct > 60 )
	{
		if( ( OBJ[no].atr & OBJ_ATR_UNIT1 ) == OBJ_ATR_UNIT1 )
		{
			unit_no = 1;
		}
		else if( ( OBJ[no].atr & OBJ_ATR_UNIT2 ) == OBJ_ATR_UNIT2 )
		{
			unit_no = 0;
		}

		ShotSetWide( no, UNIT[unit_no].obj_no, 7, OBJ_ATR_SHOT_SPECIAL1, 32, 0, 1-unit_no, PAT_GAME_SHOT );
		ShotSetWide( no, UNIT[unit_no].obj_no, 7, OBJ_ATR_SHOT_SPECIAL1, 32, 16, 1-unit_no, PAT_GAME_SHOT );
		ShotSetWide( no, UNIT[unit_no].obj_no, 6, OBJ_ATR_SHOT_SPECIAL1, 32, 8, 1-unit_no, PAT_GAME_SHOT );
		ShotSetWide( no, UNIT[unit_no].obj_no, 6, OBJ_ATR_SHOT_SPECIAL1, 32, 24, 1-unit_no, PAT_GAME_SHOT );
		ObjInit( no );
	}
}

/******************************************************************************/
//	ショットループ（ホーミング）
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
void	ShotLoopHoming( long no )
{
	short	type,
			ct[5]={10,10,4,6,5};
	long	obj;
	double	rot,
			rot_add[5]={4,90,10,20,20};

	if( ( OBJ[no].atr & OBJ_ATR_UNIT1 ) == OBJ_ATR_UNIT1 )
	{
		obj = UNIT[1].obj_no;
	}
	else if( ( OBJ[no].atr & OBJ_ATR_UNIT2 ) == OBJ_ATR_UNIT2 )
	{
		obj = UNIT[0].obj_no;
	}

	//	ショットと対象の角度を取得
	rot = GetRot( OBJ[no].px,OBJ[no].py, OBJ[obj].px,OBJ[obj].py );
	rot = RadToDeg( rot );
	if( rot >= 360 )
	{
		rot -= 360;
	}
	else if( rot <= 0 )
	{
		rot += 360;
	}

	//	各ホーミングタイプ
	type = NOT;
	if( ( OBJ[ no ].atr & OBJ_ATR_SHOT_HOMING1 ) == OBJ_ATR_SHOT_HOMING1 )
	{
		type = 0;
	}
	else if( ( OBJ[ no ].atr & OBJ_ATR_SHOT_HOMING2 ) == OBJ_ATR_SHOT_HOMING2 )
	{
		type = 1;
	}
	else if( ( OBJ[ no ].atr & OBJ_ATR_SHOT_HOMING3 ) == OBJ_ATR_SHOT_HOMING3 )
	{
		type = 2;
	}
	else if( ( OBJ[ no ].atr & OBJ_ATR_SHOT_HOMING4 ) == OBJ_ATR_SHOT_HOMING4 )
	{
		type = 3;
	}
	else if( ( OBJ[ no ].atr & OBJ_ATR_SHOT_HOMING5 ) == OBJ_ATR_SHOT_HOMING5 )
	{
		type = 4;
	}

	if( type == NOT )
	{
		return;
	}

	//	実処理
	if( OBJ[ no ].disp_ct >= ct[ type ] )
	{
		rot -= OBJ[ no ].rot_d;

		if( (rot>0 && rot<=180)|| (rot<0 && rot<=-180) )
		{
			OBJ[ no ].rot_d += rot_add[ type ];
		}
		else if( (rot>0 && rot>180) || (rot<0 && rot>-180) )
		{
			OBJ[ no ].rot_d -= rot_add[ type ];
		}

		if( OBJ[ no ].rot_d >= 360)
		{
			OBJ[ no ].rot_d -= 360;
		}
		else if( OBJ[ no ].rot_d <= 0)
		{
			OBJ[ no ].rot_d += 360;
		}

		OBJ[ no ].rot = OBJ[ no ].rot_d + 90;
		OBJ[ no ].rot_r = DegToRad( OBJ[ no ].rot_d );

		OBJ[ no ].disp_ct = 0;

		//	徐々に加速
		OBJ[ no ].speed += 0.1;
	}

	//	90フレーム後ホーミングしなくなる
	if( ( OBJ[ no ].ct >= 90 )
	|| ( GetDistance( OBJ[no].px,OBJ[no].py, OBJ[obj].px,OBJ[obj].py ) <= 32 ) )
	{
		switch( type )
		{
			case	0:
				OBJ[ no ].atr &= ~OBJ_ATR_SHOT_HOMING1;
				break;
			case	1:
				OBJ[ no ].atr &= ~OBJ_ATR_SHOT_HOMING2;
				break;
			case	2:
				OBJ[ no ].atr &= ~OBJ_ATR_SHOT_HOMING3;
				break;
			case	3:
				OBJ[ no ].atr &= ~OBJ_ATR_SHOT_HOMING4;
				break;
			case	4:
				OBJ[ no ].atr &= ~OBJ_ATR_SHOT_HOMING5;
				break;
		}

		OBJ[ no ].ct = 0;
	}
	else
	{
		OBJ[ no ].ct ++;
	}
}

/******************************************************************************/
//	ショットループ（スプライン）
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
void	ShotLoopSpline( long no )
{
}

/******************************************************************************/
//	ショットの検索
//		戻り値：
//				１：ある、０：ない
//		引数：
//				アトリビュート
/******************************************************************************/
short	ShotSearch( u_long atr )
{
	short	ret = 0;

	for( long i=OBJ_GAME_SHOT; i<OBJ_MAX; i++ )
	{
		if( ( OBJ[ i ].atr & atr ) == atr )
		{
			ret = ON;
		}
	}

	return ret;
}

/******************************************************************************/
//	ノーマルショット
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	NormarlShot( void )
{
	for( short i=0; i<2; i++ )
	{
		if( UNIT[ i ].but_no )
		{
			switch( UNIT[ i ].shot[0] ) 
			{
				case	0:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_NORMAL|OBJ_ATR_ROT_CHANGE, 0, i, PAT_GAME_SHOT+2 );
					break;

				case	1:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_NORMAL|OBJ_ATR_ROT_CHANGE, 0, i, PAT_GAME_SHOT+3 );
					break;

				case	2:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_NORMAL, 0, i, PAT_GAME_SHOT );
					break;

				case	3:
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_NORMAL|OBJ_ATR_SHOT_HOMING1, 0, i, PAT_GAME_SHOT+1 );
					break;
			}

			UNIT[ i ].but_no = OFF;
		}
	}
}

/******************************************************************************/
//	ノーマルショット（プッシュ）
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	NormarlShotPush( void )
{
	for( short i=0; i<2; i++ )
	{
		if( UNIT[ i ].but_no )
		{
			switch( UNIT[i].shot[0] )
			{
				case	0:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 6, OBJ_ATR_SHOT_NORMAL|OBJ_ATR_ROT_CHANGE, 0, i, PAT_GAME_SHOT+2 );
					break;

				case	1:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 6, OBJ_ATR_SHOT_NORMAL|OBJ_ATR_ROT_CHANGE, 0, i, PAT_GAME_SHOT+3 );
					break;

				case	2:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 6, OBJ_ATR_SHOT_NORMAL, 0, i, PAT_GAME_SHOT );
					break;

				case	3:
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 6, OBJ_ATR_SHOT_NORMAL|OBJ_ATR_SHOT_HOMING1, 0, i, PAT_GAME_SHOT+1 );
					break;
			}

			UNIT[ i ].but_no = OFF;
		}
	}
}

/******************************************************************************/
//	特殊ショット１
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	SpecialShot1( void )
{
	for( short i=0; i<2; i++ )
	{
		if( UNIT[ i ].but_no )
		{
			switch( UNIT[ i ].shot[1] )
			{
				case	0:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL1, 0, i, PAT_GAME_SHOT );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL1, 5, i, PAT_GAME_SHOT );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL1, -5, i, PAT_GAME_SHOT );

					UNIT[i].gage -= 20;
					g_LibText.SetColor( 5+i, 255,0,255,255 );
					sprintf_s( TXT[ 5+i ].Str, MAX_PATH, "-20" );
					TXT[ 5+i ].Disp = ON;
					g_LibText.SetTimer( 5+i, 30 );
					break;

				case	1:
					ShotSetCreate( UNIT[i].obj_no, UNIT[1-i].obj_no, 0, OBJ_ATR_SHOT_SPECIAL1|OBJ_ATR_SHOT_SET_UP, i, PAT_GAME_SHOT+5 );
					break;

				case	2:
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 7.6, OBJ_ATR_SHOT_SPECIAL1, 16, 0, i, PAT_GAME_SHOT );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 7.2, OBJ_ATR_SHOT_SPECIAL1, 16, 4, i, PAT_GAME_SHOT );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.8, OBJ_ATR_SHOT_SPECIAL1, 16, 8, i, PAT_GAME_SHOT );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.4, OBJ_ATR_SHOT_SPECIAL1, 16, 12, i, PAT_GAME_SHOT );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.0, OBJ_ATR_SHOT_SPECIAL1, 16, 16, i, PAT_GAME_SHOT );

					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.6, OBJ_ATR_SHOT_SPECIAL1, 16, 12, i, PAT_GAME_SHOT );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.2, OBJ_ATR_SHOT_SPECIAL1, 16, 8, i, PAT_GAME_SHOT );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 4.8, OBJ_ATR_SHOT_SPECIAL1, 16, 4, i, PAT_GAME_SHOT );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 4.4, OBJ_ATR_SHOT_SPECIAL1, 16, 0, i, PAT_GAME_SHOT );
					break;

				case	3:
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 6, OBJ_ATR_SHOT_SPECIAL1|OBJ_ATR_SHOT_HOMING2, 0, i, PAT_GAME_SHOT+4 );
					break;
			}

			UNIT[ i ].but_no = OFF;
		}
	}
}

/******************************************************************************/
//	特殊ショット２
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	SpecialShot2( void )
{
	for( short i=0; i<2; i++ )
	{
		if( UNIT[ i ].but_no )
		{
			switch( UNIT[ i ].shot[2] )
			{
				case	0:
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING3, 45, i, PAT_GAME_SHOT+3 );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING3, -45, i, PAT_GAME_SHOT+3 );
					break;

				case	1:
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 8, OBJ_ATR_SHOT_SPECIAL2, 16, 0, i, PAT_GAME_SHOT+3 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6, OBJ_ATR_SHOT_SPECIAL2, 16, 16, i, PAT_GAME_SHOT+3 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6, OBJ_ATR_SHOT_SPECIAL2, 16, -16, i, PAT_GAME_SHOT+3 );
					break;

				case	2:
					for( long j=0; j<25; j++ )
					{
						double	speed = 9.0 - ( j * 0.3 );
						ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, speed, OBJ_ATR_SHOT_SPECIAL2, 0, i, PAT_GAME_SHOT+3 );
						ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, speed, OBJ_ATR_SHOT_SPECIAL2, 8, i, PAT_GAME_SHOT+3 );
						ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, speed, OBJ_ATR_SHOT_SPECIAL2, 16, i, PAT_GAME_SHOT+3 );
						ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, speed, OBJ_ATR_SHOT_SPECIAL2, -8, i, PAT_GAME_SHOT+3 );
						ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, speed, OBJ_ATR_SHOT_SPECIAL2, -16, i, PAT_GAME_SHOT+3 );
					}
					break;

				case	3:
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING3, 30, i, PAT_GAME_SHOT+3 );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING3, -30, i, PAT_GAME_SHOT+3 );

					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING4, 90, i, PAT_GAME_SHOT+3 );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING4, -90, i, PAT_GAME_SHOT+3 );

					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING4, 120, i, PAT_GAME_SHOT+3 );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_SPECIAL2|OBJ_ATR_SHOT_HOMING4, -120, i, PAT_GAME_SHOT+3 );
					break;
			}

			UNIT[ i ].but_no = OFF;
		}
	}
}

/******************************************************************************/
//	ＥＸショット１
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ExtraShot1( void )
{
	for( short i=0; i<2; i++ )
	{
		if( UNIT[ i ].but_no )
		{
			switch( UNIT[ i ].shot[3] )
			{
				case	0:
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 7.0, OBJ_ATR_SHOT_EXTRA1, 32, 0, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.8, OBJ_ATR_SHOT_EXTRA1, 32, 8, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.6, OBJ_ATR_SHOT_EXTRA1, 32, 16, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.4, OBJ_ATR_SHOT_EXTRA1, 32, 0, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.2, OBJ_ATR_SHOT_EXTRA1, 32, 8, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.0, OBJ_ATR_SHOT_EXTRA1, 32, 16, i, PAT_GAME_SHOT+2 );

					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.8, OBJ_ATR_SHOT_EXTRA1, 32, 16, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.6, OBJ_ATR_SHOT_EXTRA1, 32, 8, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.4, OBJ_ATR_SHOT_EXTRA1, 32, 0, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.2, OBJ_ATR_SHOT_EXTRA1, 32, 16, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.0, OBJ_ATR_SHOT_EXTRA1, 32, 8, i, PAT_GAME_SHOT+2 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 4.8, OBJ_ATR_SHOT_EXTRA1, 32, 0, i, PAT_GAME_SHOT+2 );
					break;

				case	1:
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 7.0, OBJ_ATR_SHOT_EXTRA1, 32, 0, i, PAT_GAME_SHOT+4 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.8, OBJ_ATR_SHOT_EXTRA1, 32, 0, i, PAT_GAME_SHOT+4 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.6, OBJ_ATR_SHOT_EXTRA1, 32, 0, i, PAT_GAME_SHOT+4 );

					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.2, OBJ_ATR_SHOT_EXTRA1, 32, 8, i, PAT_GAME_SHOT+4 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 6.0, OBJ_ATR_SHOT_EXTRA1, 32, 8, i, PAT_GAME_SHOT+4 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.8, OBJ_ATR_SHOT_EXTRA1, 32, 8, i, PAT_GAME_SHOT+4 );

					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.4, OBJ_ATR_SHOT_EXTRA1, 32, 16, i, PAT_GAME_SHOT+4 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.2, OBJ_ATR_SHOT_EXTRA1, 32, 16, i, PAT_GAME_SHOT+4 );
					ShotSetWide( UNIT[i].obj_no, UNIT[1-i].obj_no, 5.0, OBJ_ATR_SHOT_EXTRA1, 32, 16, i, PAT_GAME_SHOT+4 );
					break;

				case	2:
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 10, OBJ_ATR_SHOT_EXTRA1, 0, i, PAT_GAME_SHOT+3 );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 10, OBJ_ATR_SHOT_EXTRA1, 10, i, PAT_GAME_SHOT+3 );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 10, OBJ_ATR_SHOT_EXTRA1, -10, i, PAT_GAME_SHOT+3 );

					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 9.5, OBJ_ATR_SHOT_EXTRA1, 0, i, PAT_GAME_SHOT+3 );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 9.5, OBJ_ATR_SHOT_EXTRA1, 10, i, PAT_GAME_SHOT+3 );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 9.5, OBJ_ATR_SHOT_EXTRA1, -10, i, PAT_GAME_SHOT+3 );

					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 9, OBJ_ATR_SHOT_EXTRA1, 0, i, PAT_GAME_SHOT+3 );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 9, OBJ_ATR_SHOT_EXTRA1, 10, i, PAT_GAME_SHOT+3 );
					ShotSetNormal( UNIT[i].obj_no, UNIT[1-i].obj_no, 9, OBJ_ATR_SHOT_EXTRA1, -10, i, PAT_GAME_SHOT+3 );
					break;

				case	3:
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 8, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING4, 0, i, PAT_GAME_SHOT );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING4, -30, i, PAT_GAME_SHOT );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING4, 30, i, PAT_GAME_SHOT );
					break;
			}

			UNIT[ i ].but_no = OFF;
		}
	}
}


/******************************************************************************/
//	ＥＸショット２
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ExtraShot2( void )
{
	for( short i=0; i<2; i++ )
	{
		if( UNIT[ i ].but_no )
		{
			switch( UNIT[ i ].shot[4] )
			{
				case	0:
					break;
				case	1:
					break;
				case	2:
					break;
				case	3:
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 8, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING5, 0, i, PAT_GAME_SHOT );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING5, -30, i, PAT_GAME_SHOT );
					ShotSetHoming( UNIT[i].obj_no, UNIT[1-i].obj_no, 7, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING5, 30, i, PAT_GAME_SHOT );
					break;
			}

			UNIT[ i ].but_no = OFF;
		}
	}
}
