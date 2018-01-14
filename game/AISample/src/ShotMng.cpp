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

#include	"./ObjectMng.h"
#include	"./ShotMng.h"
#include	"./Main.h"
#include	"./CharControl.h"

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	ShotInitAll( void );			//	ショット初期化（全体）
void	ShotLoop( void );				//	ショットループ
void	ShotMain( long no );			//	ショットメイン
short	ShotHitChk( ST_ObjectInfo Info );			//	ショットの当たり判定
void	ShotDirSet( long no );			//	角度補正
void	ShotSet( double x, double y, double rot, double speed, ULong atr, short chara, long pat );
void	ShotSetNormalDef( long obj1, double speed, ULong atr, double rev, short chr, long pat );
void	ShotSetNormal( long obj1, long obj2, double speed, ULong atr, double rev, short chr, long pat );
void	ShotSetWideDef( long obj1, double speed, ULong atr, long way, double rot_rev, short chr, long pat );
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
void	SpecialShot( void );
void	SpecialShotPush( void );
void	ExtraShot1( void );
void	ExtraShot2( void );



/******************************************************************************/
//	ショット初期化（全体）
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ShotInitAll( void )
{
	CObjectMng::GetInstance().Remove( g_ObjType.Shot );
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
	if( SYS.PauseDisp )
	{
		return;
	}

#ifdef _DEBUG
	short Ct = 100;
	//g_LibLine.InitAll();
#endif

	ST_ObjectInfo *List = CObjectMng::GetInstance().GetObjectInfo();
	for( int i=0; i<OBJ_MAX; i++ )
	{
		if( ( List[i].Type & g_ObjType.Shot ) == g_ObjType.Shot )
		{
			ShotMain( i );
			ShotDirSet( i );
			if( ShotHitChk( List[i] ) )
			{
				CObjectMng::GetInstance().Remove( i );
			}

#ifdef	_DEBUG
			//	ショットの軌跡を描画
			//double EndX = List[i].PosX;
			//double EndY = List[i].PosY;
			//EndX += GetMoveX( WIDTH*2, List[i].RotRad );
			//EndY += GetMoveY( HEIGHT*2, List[i].RotRad );
			//g_LibLine.Set( Ct, List[i].PosX, List[i].PosY, EndX,EndY, ATR_NONE );
			//g_LibLine.SetColor( Ct, 255,255,0,0 );
			//Ct ++;
#endif

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
	CObjectMng	&Obj = CObjectMng::GetInstance();
	u_long	ATR = Obj.GetObjectInfo(no).Atr & ( OBJ_ATR_SHOT_RANK );
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

	ATR = Obj.GetObjectInfo(no).Atr & ( OBJ_ATR_SHOT_TYPE );
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

	ATR = Obj.GetObjectInfo(no).Atr & ( OBJ_ATR_ROT_TYPE );
	switch( ATR )
	{
		case	OBJ_ATR_ROT_CHANGE:
			ST_ObjectInfo Info = Obj.GetObjectInfo(no);
			g_LibPol.AddRot( Info.PolNo, 0,0,30 );
			Obj.SetObjectInfo( no, Info );
			break;
	}

	//	汎用カウンタ加算
	ST_ObjectInfo Info = Obj.GetObjectInfo(no);
	Info.DispCt ++;
	Obj.SetObjectInfo( no, Info );
}

/******************************************************************************/
//	ショット当たり判定
//		戻り値：
//				なし
//		引数：
//				no		オブジェクト番号
/******************************************************************************/
short	ShotHitChk( ST_ObjectInfo Info )
{
	short	Ret = OFF;
	CObjectMng	&Obj = CObjectMng::GetInstance();

	//	プレイヤー判定
	long	chara;
	if( ( Info.Atr & OBJ_ATR_ENEMY ) == OBJ_ATR_ENEMY )
	{
		chara = OBJ_GAME_UNIT;
	}
	else
	{
		chara = OBJ_GAME_ENEMY;
	}

	ST_ObjectInfo MyInfo = Obj.GetObjectInfo( chara );

	//	当たり判定
	if( HitCheck( Info.PosX,Info.PosY,Info.HitRect, MyInfo.PosX,MyInfo.PosY,MyInfo.HitRect  ) )
	{
		Ret = ON;
		Obj.SetColor( chara, 255,255,0,0 );
		if( chara == OBJ_GAME_UNIT )
		{
			UNIT.Life -= 100;
		}
	}
	else
	{
		Obj.SetColor( chara, 255,255,255,255 );
	}

	return Ret;
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
	CObjectMng	&Obj = CObjectMng::GetInstance();
	//	移動量取得
	ST_ObjectInfo Info = Obj.GetObjectInfo(no);
	Info.MoveX = GetMoveX( Info.Speed, Info.RotRad );
	Info.MoveY = GetMoveY( Info.Speed, Info.RotRad );
	Obj.SetObjectInfo( no, Info );
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
void	ShotSet( double x, double y, double rot,
				double speed, ULong atr, short chara, long pat )
{
	CObjectMng	&Obj = CObjectMng::GetInstance();
	long ObjNo = Obj.GetObjectNo();
	if( ObjNo == NOT )
	{
		return;
	}

	//	角度補正
	if( rot > 360 )
	{
		rot -= 360;
	}
	else if( rot < 0 )
	{
		rot += 360;
	}

	Obj.Set( ObjNo, pat, x,y,0.5, g_ObjType.Shot, atr );
	ST_ObjectInfo Info = Obj.GetObjectInfo( ObjNo );
	Info.Atr = atr;
	Info.Speed = speed;
	Info.RotDeg = rot+90;
	Info.RotRad = DegToRad( rot );
	Info.Ct = 0;
	Info.DispCt = 0;
	Obj.SetObjectInfo( ObjNo, Info );

	/////////////////////////////////////////////////
	//	中心座標のセット
	switch( pat - PAT_GAME_SHOT )
	{
		case	0:
			Obj.SetCenter( ObjNo, 7,15 );
			break;
		case	1:
			Obj.SetCenter( ObjNo, 7,15 );
			break;
		case	2:
			Obj.SetCenter( ObjNo, 7,7 );
			break;
		case	3:
			Obj.SetCenter( ObjNo, 7,7 );
			break;
		case	4:
			Obj.SetCenter( ObjNo, 7,7 );
			break;
		case	5:
			Obj.SetCenter( ObjNo, 7,7 );
			break;
	}

	Info = Obj.GetObjectInfo( ObjNo );
	switch( pat - PAT_GAME_SHOT )
	{
		case	0:
			Info.HitRect = 13;
			break;
		case	1:
			Info.HitRect = 13;
			break;
		case	2:
			Info.HitRect = 7;
			break;
		case	3:
			Info.HitRect = 7;
			break;
		case	4:
			Info.HitRect = 7;
			break;
		case	5:
			Info.HitRect = 7;
			break;
	}

	Obj.SetObjectInfo( ObjNo, Info );
}

/******************************************************************************/
//	ノーマルショットセット
//		戻り値：
//				なし
//		引数：
//				obj1		自分の番号
//				speed		弾速
//				atr			アトリビュート
//				chr			キャラクター番号
//				pat			パターン番号
/******************************************************************************/
void	ShotSetNormalDef( long obj1, double speed,
						 ULong atr, double rev, short chr, long pat )
{
	//	ショット発生
	ST_ObjectInfo Info = CObjectMng::GetInstance().GetObjectInfo(obj1);
	double x = Info.PosX;
	double y = Info.PosY;
	ShotSet( x,y, -90+rev, speed, atr, chr, pat );
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
void	ShotSetNormal( long obj1, long obj2, double speed,
					  ULong atr, double rev, short chr, long pat )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo Info1 = Obj.GetObjectInfo(obj1);
	ST_ObjectInfo Info2 = Obj.GetObjectInfo(obj2);

	//	角度
	double rot = GetRot(Info1.PosX,Info1.PosY, Info2.PosX,Info2.PosY );
	rot += DegToRad( rev );

	//	ショット発生
	double x = Info1.PosX + cos( rot );
	double y = Info1.PosY - sin( rot );
	ShotSet( x,y, RadToDeg(rot), speed, atr, chr, pat );
}

/******************************************************************************/
//	拡散ショット
//		戻り値：
//				なし
//		引数：
//				obj1		自分の番号
//				speed		弾速
//				atr			アトリビュート
//				way			拡散数
//				ret_rev		補正角度
//				chr			キャラクター番号
//				pat			パターン番号
/******************************************************************************/
void	ShotSetWideDef( long obj1, double speed, ULong atr,
					   long way, double rot_rev, short chr, long pat )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	double	rot,x,y,way_rot,i_rot,rad;

	ST_ObjectInfo Info = Obj.GetObjectInfo(obj1);

	//	角度
	rot = -90 + DegToRad( rot_rev+11 );

	// 弾発生
	if( (way & 1) == 1 )
	{
		// 奇数ＷＡＹ
		x = Info.PosX + cos( rot );
		y = Info.PosY - sin( rot );
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

		x = Info.PosX + cos( rot - way_rot + i_rot );
		y = Info.PosY - sin( rot - way_rot + i_rot );
		ShotSet( x, y, RadToDeg(rot - way_rot + i_rot), speed, atr, chr, pat );

		x = Info.PosX + cos( rot + way_rot - i_rot );
		y = Info.PosY - sin( rot + way_rot - i_rot );
		ShotSet( x, y, RadToDeg(rot + way_rot - i_rot), speed, atr, chr, pat );
	}
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
void	ShotSetWide( long obj1, long obj2,
					double speed, ULong atr,
					long way, double rot_rev, short chr, long pat )
{
	double	x,y,way_rot,i_rot,rad;
	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo Info1 = Obj.GetObjectInfo(obj1);
	ST_ObjectInfo Info2 = Obj.GetObjectInfo(obj2);

	//	角度
	double rot = GetRot( Info1.PosX,Info1.PosY, Info2.PosX,Info2.PosY );
	rot += DegToRad( rot_rev+11 );

	// 弾発生
	if( (way & 1) == 1 )
	{
		// 奇数ＷＡＹ
		x = Info1.PosX + cos( rot );
		y = Info1.PosY - sin( rot );
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

		x = Info1.PosX + cos( rot - way_rot + i_rot );
		y = Info1.PosY - sin( rot - way_rot + i_rot );
		ShotSet( x, y, RadToDeg(rot - way_rot + i_rot), speed, atr, chr, pat );

		x = Info1.PosX + cos( rot + way_rot - i_rot );
		y = Info1.PosY - sin( rot + way_rot - i_rot );
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
void	ShotSetCreate( long obj1, long obj2,
					  double speed, ULong atr, short chr, long pat )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo Info1 = Obj.GetObjectInfo(obj1);
	ST_ObjectInfo Info2 = Obj.GetObjectInfo(obj2);

	//	角度
	double rot = GetRot( Info1.PosX,Info1.PosY, Info2.PosX,Info2.PosY );

	double x = Obj.GetObjectInfo(UNIT.ObjectNo).PosX;
	double y = Obj.GetObjectInfo(UNIT.ObjectNo).PosY;
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
void	ShotSetSpline( long obj1, long obj2, double speed,
					  ULong atr, double rot_add, double rev, short chr, long pat )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo Info1 = Obj.GetObjectInfo(obj1);
	ST_ObjectInfo Info2 = Obj.GetObjectInfo(obj2);

	//	角度
	double rot = GetRot( Info1.PosX,Info1.PosY, Info2.PosX,Info2.PosY );
	rot += DegToRad( rev );

	double x = Obj.GetObjectInfo(UNIT.ObjectNo).PosX;
	double y = Obj.GetObjectInfo(UNIT.ObjectNo).PosY;
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
void	ShotSetHoming( long obj1, long obj2, double speed,
					  ULong atr, double rev, short chr, long pat )
{
	CObjectMng &Obj = CObjectMng::GetInstance();
	ST_ObjectInfo Info1 = Obj.GetObjectInfo(obj1);
	ST_ObjectInfo Info2 = Obj.GetObjectInfo(obj2);

	//	角度
	double rot = GetRot( Info1.PosX,Info1.PosY, Info2.PosX,Info2.PosY );
	rot += DegToRad( rev );

	double x = Obj.GetObjectInfo(UNIT.ObjectNo).PosX;
	double y = Obj.GetObjectInfo(UNIT.ObjectNo).PosY;
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
	CObjectMng &Obj = CObjectMng::GetInstance();
	short	unit_no;

	if( Obj.GetObjectInfo(no).DispCt > 60 )
	{
		if( ( Obj.GetObjectInfo(no).Atr & OBJ_ATR_UNIT ) == OBJ_ATR_UNIT )
		{
			unit_no = 1;
		}

		ShotSetWide( no, UNIT.ObjectNo, 7, OBJ_ATR_SHOT_SPECIAL1, 32, 0, 1-unit_no, PAT_GAME_SHOT );
		ShotSetWide( no, UNIT.ObjectNo, 7, OBJ_ATR_SHOT_SPECIAL1, 32, 16, 1-unit_no, PAT_GAME_SHOT );
		ShotSetWide( no, UNIT.ObjectNo, 6, OBJ_ATR_SHOT_SPECIAL1, 32, 8, 1-unit_no, PAT_GAME_SHOT );
		ShotSetWide( no, UNIT.ObjectNo, 6, OBJ_ATR_SHOT_SPECIAL1, 32, 24, 1-unit_no, PAT_GAME_SHOT );
		Obj.Remove( no );
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
	long	ObjNo;
	double	rot_add[5]={4,90,10,20,20};
	CObjectMng	&Obj = CObjectMng::GetInstance();

	ObjNo = UNIT.ObjectNo;
	ST_ObjectInfo Info1,Info2;
	Info1 = Obj.GetObjectInfo(no);
	Info2 = Obj.GetObjectInfo(ObjNo);

	//	ショットと対象の角度を取得
	double rot = GetRot( Info1.PosX,Info1.PosY, Info2.PosX,Info2.PosY );
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
	if( ( Info1.Atr & OBJ_ATR_SHOT_HOMING1 ) == OBJ_ATR_SHOT_HOMING1 )
	{
		type = 0;
	}
	else if( ( Info1.Atr & OBJ_ATR_SHOT_HOMING2 ) == OBJ_ATR_SHOT_HOMING2 )
	{
		type = 1;
	}
	else if( ( Info1.Atr & OBJ_ATR_SHOT_HOMING3 ) == OBJ_ATR_SHOT_HOMING3 )
	{
		type = 2;
	}
	else if( ( Info1.Atr & OBJ_ATR_SHOT_HOMING4 ) == OBJ_ATR_SHOT_HOMING4 )
	{
		type = 3;
	}
	else if( ( Info1.Atr & OBJ_ATR_SHOT_HOMING5 ) == OBJ_ATR_SHOT_HOMING5 )
	{
		type = 4;
	}

	//	実処理
	if( Info1.DispCt >= ct[ type ] )
	{
		rot -= Info1.RotDeg;

		if( (rot>0 && rot<=180)|| (rot<0 && rot<=-180) )
		{
			Info1.RotDeg += rot_add[ type ];
		}
		else if( (rot>0 && rot>180) || (rot<0 && rot>-180) )
		{
			Info1.RotDeg -= rot_add[ type ];
		}

		if( Info1.RotDeg >= 360)
		{
			Info1.RotDeg -= 360;
		}
		else if( Info1.RotDeg <= 0)
		{
			Info1.RotDeg += 360;
		}

		g_LibPol.SetRotZ( Info1.PolNo, Info1.RotDeg + 90 );
		Info1.RotRad = DegToRad( Info1.RotDeg );

		Info1.DispCt = 0;

		//	徐々に加速
		Info1.Speed += 0.1;
	}

	//	90フレーム後ホーミングしなくなる
	double Dis = GetDistance(
		Info1.PosX, Info1.PosY,
		Info2.PosX, Info2.PosY ); 
	if( ( Info1.Ct >= 90 ) || ( Dis <= 32 ) )
	{
		if( type == 1 )
		{
			Info1.Atr &= ~OBJ_ATR_SHOT_HOMING2;
		}
		else if( type == 2 )
		{
			Info1.Atr &= ~OBJ_ATR_SHOT_HOMING3;
		}
		else if( type == 3 )
		{
			Info1.Atr &= ~OBJ_ATR_SHOT_HOMING4;
		}
		else if( type == 4 )
		{
			Info1.Atr &= ~OBJ_ATR_SHOT_HOMING5;
		}

		Info1.Atr &= ~OBJ_ATR_SHOT_HOMING;

		Info1.Ct = 0;
	}
	else
	{
		Info1.Ct ++;
	}

	Obj.SetObjectInfo( no, Info1 );
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
//short	ShotSearch( u_long atr )
//{
//	short	ret = 0;
//
//	for( long i=OBJ_GAME_SHOT; i<OBJ_MAX; i++ )
//	{
//		if( ( OBJ[ i ].Atr & atr ) == atr )
//		{
//			ret = ON;
//		}
//	}
//
//	return ret;
//}

/******************************************************************************/
//	ノーマルショット
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	NormarlShot( void )
{
	ULong Atr = OBJ_ATR_UNIT | OBJ_ATR_SHOT | OBJ_ATR_SHOT_NORMAL;

	if( UNIT.ButNo )
	{
		switch( UNIT.ShotLv )
		{
			case	1:
				//ShotSetNormalDef( UNIT.ObjectNo, 7, Atr, 0, 0, PAT_GAME_SHOT );
				//ShotSetNormalDef( UNIT.ObjectNo, 7, Atr, 5, 0, PAT_GAME_SHOT );
				//ShotSetNormalDef( UNIT.ObjectNo, 7, Atr, -5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 0, 0, PAT_GAME_SHOT );
				//ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 5, 0, PAT_GAME_SHOT );
				//ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -5, 0, PAT_GAME_SHOT );
				break;

			case	2:
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 0, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 10, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -10, 0, PAT_GAME_SHOT );
				break;

			case	3:
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 0, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 10, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -10, 0, PAT_GAME_SHOT );

				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 30, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 25, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -30, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -25, 0, PAT_GAME_SHOT );
				break;
		}

		UNIT.ButNo = OFF;
	}
}

/******************************************************************************/
//	ノーマルショットプッシュ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	NormarlShotPush( void )
{
	ULong Atr = OBJ_ATR_UNIT | OBJ_ATR_SHOT | OBJ_ATR_SHOT_NORMAL;

	if( UNIT.ButNo )
	{
		switch( UNIT.ShotLv )
		{
			case	1:
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 0, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -5, 0, PAT_GAME_SHOT );
				break;

			case	2:
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 0, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -5, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, 10, 0, PAT_GAME_SHOT );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr, -10, 0, PAT_GAME_SHOT );
				break;
		}

		UNIT.ButNo = OFF;
	}
}

/******************************************************************************/
//	特殊ショット
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	SpecialShot( void )
{
	ULong Atr = OBJ_ATR_UNIT | OBJ_ATR_SHOT | OBJ_ATR_SHOT_SPECIAL1;

	if( UNIT.ButNo )
	{
		UNIT.ShotFlag = 0;

		UNIT.ButNo = OFF;
	}
}
void	SpecialShotLoop( void )
{
	ULong Atr = OBJ_ATR_UNIT | OBJ_ATR_SHOT | OBJ_ATR_SHOT_SPECIAL1;
	if( UNIT.ShotFlag <= 16 )
	{
		ShotSetWideDef( UNIT.ObjectNo,6.0, Atr, 16, UNIT.ShotFlag*4, 0, PAT_GAME_SHOT );
		UNIT.ShotFlag ++;
	}
}

/******************************************************************************/
//	特殊ショットプッシュ
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	SpecialShotPush( void )
{
	ULong Atr = OBJ_ATR_UNIT | OBJ_ATR_SHOT | OBJ_ATR_SHOT_SPECIAL2;

	if( UNIT.ButNo )
	{
		//ShotSetWideDef( UNIT.ObjectNo, 8, Atr, 16, 0, 0, PAT_GAME_SHOT+3 );
		//ShotSetWideDef( UNIT.ObjectNo, 6, Atr, 16, 16, 0, PAT_GAME_SHOT+3 );
		//ShotSetWideDef( UNIT.ObjectNo, 6, Atr, 16, -16, 0, PAT_GAME_SHOT+3 );

		for( long j=0; j<25; j++ )
		{
			double	speed = 9.0 - ( j * 0.15 );
			ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, speed, Atr, 0, 0, PAT_GAME_SHOT+3 );
			ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, speed, Atr, 8, 0, PAT_GAME_SHOT+3 );
			ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, speed, Atr, 16, 0, PAT_GAME_SHOT+3 );
			ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, speed, Atr, -8, 0, PAT_GAME_SHOT+3 );
			ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, speed, Atr, -16, 0, PAT_GAME_SHOT+3 );
		}

		UNIT.ButNo = OFF;
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
	ULong Atr = OBJ_ATR_UNIT | OBJ_ATR_SHOT | OBJ_ATR_SHOT_EXTRA1;

	if( UNIT.ButNo )
	{
		switch( UNIT.Shot[3] )
		{
			case	0:
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7.0, Atr, 32, 0, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.8, Atr, 32, 8, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.6, Atr, 32, 16, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.4, Atr, 32, 0, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.2, Atr, 32, 8, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.0, Atr, 32, 16, 0, PAT_GAME_SHOT+2 );

				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.8, Atr, 32, 16, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.6, Atr, 32, 8, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.4, Atr, 32, 0, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.2, Atr, 32, 16, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.0, Atr, 32, 8, 0, PAT_GAME_SHOT+2 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 4.8, Atr, 32, 0, 0, PAT_GAME_SHOT+2 );
				break;

			case	1:
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7.0, Atr, 32, 0, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.8, Atr, 32, 0, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.6, Atr, 32, 0, 0, PAT_GAME_SHOT+4 );

				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.2, Atr, 32, 8, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.0, Atr, 32, 8, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.8, Atr, 32, 8, 0, PAT_GAME_SHOT+4 );

				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.4, Atr, 32, 16, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.2, Atr, 32, 16, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.0, Atr, 32, 16, 0, PAT_GAME_SHOT+4 );
				break;

			case	2:
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 10, Atr, 0, 0, PAT_GAME_SHOT+3 );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 10, Atr, 10, 0, PAT_GAME_SHOT+3 );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 10, Atr, -10, 0, PAT_GAME_SHOT+3 );

				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 9.5, Atr, 0, 0, PAT_GAME_SHOT+3 );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 9.5, Atr, 10, 0, PAT_GAME_SHOT+3 );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 9.5, Atr, -10, 0, PAT_GAME_SHOT+3 );

				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 9, Atr, 0, 0, PAT_GAME_SHOT+3 );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 9, Atr, 10, 0, PAT_GAME_SHOT+3 );
				ShotSetNormal( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 9, Atr, -10, 0, PAT_GAME_SHOT+3 );
				break;

			case	3:
				ShotSetHoming( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 8, Atr|OBJ_ATR_SHOT_HOMING4, 0, 0, PAT_GAME_SHOT );
				ShotSetHoming( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr|OBJ_ATR_SHOT_HOMING4, -30, 0, PAT_GAME_SHOT );
				ShotSetHoming( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, Atr|OBJ_ATR_SHOT_HOMING4, 30, 0, PAT_GAME_SHOT );
				break;
		}

		UNIT.ButNo = OFF;
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
	if( UNIT.ButNo )
	{
		switch( UNIT.Shot[4] )
		{
			case	0:
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7.0, OBJ_ATR_SHOT_EXTRA1, 32, 0, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.8, OBJ_ATR_SHOT_EXTRA1, 32, 0, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.6, OBJ_ATR_SHOT_EXTRA1, 32, 0, 0, PAT_GAME_SHOT+4 );

				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.2, OBJ_ATR_SHOT_EXTRA1, 32, 8, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 6.0, OBJ_ATR_SHOT_EXTRA1, 32, 8, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.8, OBJ_ATR_SHOT_EXTRA1, 32, 8, 0, PAT_GAME_SHOT+4 );

				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.4, OBJ_ATR_SHOT_EXTRA1, 32, 16, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.2, OBJ_ATR_SHOT_EXTRA1, 32, 16, 0, PAT_GAME_SHOT+4 );
				ShotSetWide( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 5.0, OBJ_ATR_SHOT_EXTRA1, 32, 16, 0, PAT_GAME_SHOT+4 );
				break;
			case	1:
				break;
			case	2:
				break;
			case	3:
				ShotSetHoming( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 8, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING5, 0, 0, PAT_GAME_SHOT );
				ShotSetHoming( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING5, -30, 0, PAT_GAME_SHOT );
				ShotSetHoming( UNIT.ObjectNo, OBJ_GAME_ENEMY + UNIT.LockEnemy, 7, OBJ_ATR_SHOT_EXTRA1|OBJ_ATR_SHOT_HOMING5, 30, 0, PAT_GAME_SHOT );
				break;
		}

		UNIT.ButNo = OFF;
	}
}
