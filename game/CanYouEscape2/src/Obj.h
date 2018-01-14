/******************************************************************************/
//	オブジェクト用ファイル
//		ソースファイル
/******************************************************************************/

#ifndef	__Obj_H__
#define	__Obj_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//
#define	OBJ_MAX		5000	//	オブジェクト最大数

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	オブジェクト管理構造体
typedef	struct
{
	//	フラグ
	u_char	use;			//	使用フラグ
	u_char	disp;			//	表示フラグ
	u_char	buzz;			//	バズリフラグ

	//	番号
	long	pol_no;			//	ポリゴン番号
	long	pat_no;			//	パターン番号
	long	anm_no;			//	アニメ管理番号

	//	座標系
	double	px,py,pz;		//	表示座標
	double	bx,by,bz;		//	表示座標（バックアップ）
	double	mx,my;			//	移動量
	short	cx,cy;			//	中心座標

	//	属性系
	u_long	type;			//	タイプフラグ(機体、ショット、その他)
	u_long	atr;			//	アトリビュート(自機、自ショット)

	//	当たり判定
	short	hit_rect;		//	当たり判定
	short	buzz_rect;		//	バズリ判定

	//	その他
	double	scale;			//	拡大率
	double	rot_r,rot_d;	//	向き(ラジアン、デフリー)
	double	rot;			//	角度
	double	speed;			//	移動速度
	short	dead;			//	消滅フラグ(0:生存、1:消滅、2:点滅)

	short	disp_ct;		//	カウンタ（useフラグがONになってからの時間）
	short	dead_ct;		//	消滅カウンタ
	long	ct;				//	汎用カウンタ
}OBJ_TBL;

#endif	//	__Obj_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	OBJ_TBL	OBJ[OBJ_MAX];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	ObjInitAll( void );				//	オブジェクト全初期化
extern	void	ObjInit( long no );				//	オブジェクト初期化
extern	void	ObjSet( long obj_no, long pol_no, long pat_no, double x, double y, double z, u_long type, u_long atr );	//	オブジェクト設定
extern	void	ObjPosSet( long obj_no, double x, double y, double z );		//	表示座標セット
extern	void	ObjCenterSet( long obj_no, short cx, short cy );	//	中心座標のセット
extern	void	ObjScaleSet( long obj_no, double scall );			//	拡大率のセット
extern	void	ObjAnmSet( long obj_no, long anm_no, long flag );	//	オブジェクトアニメセット
extern	void	ObjMove( void );				//	オブジェクト全移動
extern	void	ObjMoveLoop( OBJ_TBL *obj );	//	オブジェクト移動
extern	void	ObjDispAll( void );				//	オブジェクト全出現
extern	void	ObjDisp( OBJ_TBL *obj );		//	オブジェクト出現
extern	long	ObjClipping( long no );	//	オブジェクトクリッピング
extern	void	ObjPosBackUp( OBJ_TBL *obj );	//	座標バックアップ
extern	void	ObjPosRet( OBJ_TBL *obj );		//	座標を元に戻す
extern	long	ObjGetNo( long no );			//	空き番号取得
