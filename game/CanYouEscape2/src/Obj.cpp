/******************************************************************************/
//	オブジェクト用ファイル
//		ソースファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib/Dx9Lib.h"
#include	"./Dx9LibAnm.h"
#include	"./Dx9LibPad.h"

#include	"./Obj.h"
#include	"./GameMain.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
OBJ_TBL	OBJ[OBJ_MAX];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
void	ObjInitAll( void );				//	オブジェクト全初期化
void	ObjInit( long no );				//	オブジェクト初期化
void	ObjSet( long obj_no, long pol_no, long pat_no, double x, double y, double z, u_long type, u_long atr );
void	ObjPosSet( long obj_no, double x, double y, double z );		//	表示座標セット
void	ObjCenterSet( long obj_no, short cx, short cy );	//	中心座標セット
void	ObjScaleSet( long obj_no, double scale );			//	拡大率セット
void	ObjMovePosSet( long obj_no, double mx, double my );	//	移動量セット
void	ObjAnmSet( long obj_no, long anm_no, long flag );	//	オブジェクトアニメセット
void	ObjMove( void );				//	オブジェクト全移動
void	ObjMoveLoop( long no );			//	オブジェクト移動
void	ObjDispAll( void );				//	オブジェクト全出現
void	ObjDisp( OBJ_TBL *obj );		//	オブジェクト出現
long	ObjClipping( long no );			//	オブジェクトクリッピング
void	ObjPosBackUp( OBJ_TBL *obj );	//	座標バックアップ
void	ObjPosRet( OBJ_TBL *obj );		//	座標を元に戻す
long	ObjGetNo( long no );			//	空き番号取得
void	ObjDelete( short no, long time );	//	オブジェクト消滅



/******************************************************************************/
//	オブジェクト全初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ObjInitAll( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		ObjInit( i );
	}
}

/******************************************************************************/
//	オブジェクト初期化
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ObjInit( long no )
{
	POL[ no ].Disp = OFF;

	//	フラグ
	OBJ[ no ].use = OFF;				//	使用
	OBJ[ no ].disp = OFF;				//	表示
	OBJ[ no ].buzz = OFF;				//	バズり

	//	番号
	OBJ[ no ].pol_no = NOT;
	OBJ[ no ].pat_no = NOT;
	OBJ[ no ].anm_no = NOT;

	//	座標
	ObjPosSet( no, 0,0,0 )	;	//	表示座標
	ObjPosBackUp( &OBJ[ no ] );	//	バックアップ座標
	ObjMovePosSet( no, 0,0 );	//	移動量
	ObjCenterSet( no, 0,0 );	//	中心座標

	//	属性
	OBJ[ no ].type = 0;			//	タイプ
	OBJ[ no ].atr = OBJ_ATR_NONE;	//	アトリビュート

	//	当たり判定
	OBJ[ no ].hit_rect = 0;
	OBJ[ no ].buzz_rect = 0;

	//	その他
	OBJ[ no ].scale = 1.0;	//	拡大率
	OBJ[ no ].rot_r = OBJ[ no ].rot_d = 0;	//	向き
	OBJ[ no ].rot = 0;		//	角度
	OBJ[ no ].speed = 0;		//	スピード
	OBJ[ no ].dead = 0;		//	死亡フラグ
	OBJ[ no ].dead_ct = 0;	//	死亡カウンタ
	OBJ[ no ].ct = 0;		//	汎用カウンタ
}

/******************************************************************************/
//	オブジェクト設定
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				pol_no		ポリゴン番号
//				pat_no		パターン番号
//				x,y,z		座標
//				type		タイプ
//				atr			アトリビュート
/******************************************************************************/
void	ObjSet( long obj_no, long pol_no, long pat_no, double x, double y, double z, u_long type, u_long atr )
{
	//	フラグ
	OBJ[ obj_no ].use = ON;
	OBJ[ obj_no ].disp = ON;

	//	番号
	OBJ[ obj_no ].pol_no = pol_no;
	OBJ[ obj_no ].pat_no = pat_no;

	//	座標
	OBJ[ obj_no ].px = x;
	OBJ[ obj_no ].py = y;
	OBJ[ obj_no ].pz = z;

	//	属性
	OBJ[ obj_no ].type = type;
	OBJ[ obj_no ].atr = atr;

	//	ポリゴンセット
	SetPolygonPat( pol_no, pat_no, x,y,z, ATR_NONE );
}

/******************************************************************************/
//	オブジェクトの表示座標セット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				x,y,z		表示座標
/******************************************************************************/
void	ObjPosSet( long obj_no, double x, double y,double z )
{
	OBJ[ obj_no ].px = x;
	OBJ[ obj_no ].py = y;
	OBJ[ obj_no ].pz = z;

	long pol = OBJ[ obj_no ].pol_no;
	POL[ pol ].PosX = x;
	POL[ pol ].PosY = y;
	POL[ pol ].PosZ = z;
}

/******************************************************************************/
//	中心座標のセット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				cx,cy		中心座標
/******************************************************************************/
void	ObjCenterSet( long obj_no, short cx, short cy )
{
	OBJ[ obj_no ].cx = cx;
	OBJ[ obj_no ].cy = cy;

	long pol = OBJ[ obj_no ].pol_no;
	POL[ pol ].CenterX = cx;
	POL[ pol ].CenterY = cy;
}

/******************************************************************************/
//	拡大率のセット
//		戻り値：
//				なし
//		引数：
//				obj_no	オブジェクト番号
//				scale	拡大率
/******************************************************************************/
void	ObjScaleSet( long obj_no, double scale )
{
	OBJ[ obj_no ].scale = scale;

	long pol = OBJ[ obj_no ].pol_no;
	SetPolygonScale( pol, scale );
}

/******************************************************************************/
//	移動量セット
//		戻り値；
//				なし
//		引数：
//				obj_no	オブジェクト番号
//				mx,my	移動量
/******************************************************************************/
void	ObjMovePosSet( long obj_no, double mx, double my )
{
	OBJ[ obj_no ].mx = mx;
	OBJ[ obj_no ].my = my;
}

/******************************************************************************/
//	オブジェクトアニメセット
//		戻り値：
//				なし
//		引数：
//				obj_no		オブジェクト番号
//				anm_no		アニメ番号
//				flag		フラグ
/******************************************************************************/
void	ObjAnmSet( long obj_no, long anm_no, long flag )
{
}

/******************************************************************************/
//	オブジェクト全移動
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ObjMove( void )
{
	for( long i=0; i<OBJ_MAX; i++ )
	{
		ObjMoveLoop( i );
	}
}

/******************************************************************************/
//	オブジェクト移動
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	ObjMoveLoop( long no )
{
	ObjPosBackUp( &OBJ[ no ] );

	OBJ[ no ].px += OBJ[ no ].mx;
	OBJ[ no ].py += OBJ[ no ].my;

	ObjClipping( no );
}

/******************************************************************************/
//	オブジェクト全出現
//		戻り値：
//				なし
//		引数：
//				なし
/******************************************************************************/
void	ObjDispAll( void )
{
	for( int i=0; i<OBJ_MAX; i++ )
	{
		if( OBJ[ i ].use )
		{
			ObjDisp( &OBJ[ i ] );
		}
	}
}

/******************************************************************************/
//	オブジェクト出現
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	ObjDisp( OBJ_TBL *obj )
{
	//	一時格納用
	long	pol,pat;
	double	x,y,z;

	//	番号
	pat = obj->pat_no;
	pol = obj->pol_no;

	//	座標
	x = obj->px;
	y = obj->py;
	z = obj->pz;

	//	座標のバックアップ
	obj->bx = obj->px;
	obj->by = obj->py;
	obj->bz = obj->pz;

	//	ポリゴン再セット
	SetPolygonPat( pol, pat, x,y,z, ATR_NONE );

	POL[ pol ].Disp = obj->disp;

	//	拡大率
	POL[ pol ].ScaleX = obj->scale;
	POL[ pol ].ScaleY = obj->scale;

	//	角度
	POL[ pol ].Rot = obj->rot;

	//	中心座標
	POL[ pol ].CenterX = obj->cx;
	POL[ pol ].CenterY = obj->cy;
}

/******************************************************************************/
//	オブジェクトクリッピング
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
long	ObjClipping( long no )
{
	long	ret = 0;
	long	w,h,flag;


	w = POL[ OBJ[ no ].pol_no ].Width;
	h = POL[ OBJ[ no ].pol_no ].Height;

	flag = 0;

	switch( OBJ[ no ].type )
	{
		case	OBJ_TYPE_UNIT:
			if( OBJ[ no ].px < 0 )
			{
				OBJ[ no ].px = 0;
			}
			else if( OBJ[ no ].px > WIDTH )
			{
				OBJ[ no ].px = WIDTH;
			}

			if( OBJ[ no ].py < 0 )
			{
				OBJ[ no ].py = 0;
			}
			else if( OBJ[ no ].py > HEIGHT )
			{
				OBJ[ no ].py = HEIGHT;
			}
			break;

		case	OBJ_TYPE_SHOT:
			if( ( OBJ[ no ].px < -50 ) || ( OBJ[ no ].px > 690 )
			|| ( OBJ[ no ].py < -50 ) || ( OBJ[ no ].py > 530 ) )
			{
				ObjInit( no );
			}
			break;
	}

	return ret;
}

/******************************************************************************/
//	座標バックアップ
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	ObjPosBackUp( OBJ_TBL *obj )
{
	obj->bx = obj->px;
	obj->by = obj->py;
	obj->bz = obj->pz;
}

/******************************************************************************/
//	座標を元に戻す
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトのアドレス
/******************************************************************************/
void	ObjPosRet( OBJ_TBL *obj )
{
	obj->px = obj->bx;
	obj->py = obj->by;
}

/******************************************************************************/
//	空き番号取得
//		戻り値：
//				なし
//		引数：
//				*obj	オブジェクトの番号
/******************************************************************************/
long	ObjGetNo( long no )
{
	long	i;

	for( i=no; i<OBJ_MAX; i++ )
	{
		if( OBJ[i].use == OFF )
		{
			return i;
		}
	}

	return NOT;
}

/******************************************************************************/
//	オブジェクト消滅
//		戻り値：
//				なし
//		引数：
//				time		消滅までの時間（フレーム数）
/******************************************************************************/
void	ObjDelete( short no, long time )
{
	if( OBJ[ no ].disp == OFF )
	{
		return;
	}
}
