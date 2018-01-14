/************************************************************************************************/
//	ショットメイン
//		ヘッダファイル
/************************************************************************************************/


#ifndef		__SHOT_H__
#define		__SHOT_H__



//----------------------------------------------------------------------------------------------//
//	デフェイン
//----------------------------------------------------------------------------------------------//

#define	SHOT_MAX	256

enum
{
	SHOT_ATR_NORMAL = 0,
	SHOT_ATR_HOMING,
};



//----------------------------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////
//	ショット構造体
typedef	struct
{
	//	フラグ
	u_char	use;		//	使用フラグ
	u_char	disp;		//	表示フラグ

	//	番号
	long	pol_no;		//	ポリゴン番号

	//	座標系
	double	px,py,pz;	//	表示座標
	double	bx,by,bz;	//	表示座標（バックアップ）
	double	mx,my;		//	移動量
	short	w,h;		//	サイズ

	//	属性系
	u_long	atr;		//	アトリビュート(自機、自ショット

	//	その他
	double	rot_r,rot_d;	//	向き(ラジアン、デフリー)
	double	speed;		//	移動速度

	short	disp_ct;	//	カウンタ（useフラグがONになってからの時間）
	long	ct;			//	汎用カウンタ
}SHOT_TBL;


/////////////////////////////////////////////////
//	カラーテーブル
typedef	struct
{
	u_char	a;
	u_char	r,g,b;
}COL_TBL;


/////////////////////////////////////////////////
//	ショットデータ
typedef	struct
{
	short	w,h;
	short	col;
}SHOT_INIT_TBL;


#endif


//----------------------------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------------------------//
extern	void	ShotInitAll( void );			//	ショット初期化（全体）
extern	void	ShotInit( SHOT_TBL *shot );		//	ショット初期化（単体）
extern	void	ShotLoop( void );				//	ショットループ
extern	void	ShotMain( SHOT_TBL *shot );		//	ショットメイン
extern	long	GetShotNo( void );				//	ショット空き番号取得
extern	void	ShotRotSet( SHOT_TBL *shot );	//	ショット角度セット
extern	long	ShotHitChk( long no );			//	ショットの当たり判定
extern	long	HitCheck( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );

extern	void	ShotNormalSet( double speed, double rev );
extern	void	ShotWideSet( double speed, long way, double rot_rev );
extern	void	ShotCreateSet( double speed );
extern	void	ShotSplineSet( double speed, double rot_add, double rev );
extern	void	ShotHomingSet( double speed, double rev );


//	ショット構造体
extern	SHOT_TBL	SHOT[SHOT_MAX];
