/******************************************************************************/
//	ショットメイン
//		ヘッダファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	ShotInitAll( void );			//	ショット初期化（全体）
extern	void	ShotInit( ST_ObjectInfo *obj );		//	ショット初期化（単体）
extern	void	ShotLoop( void );				//	ショットループ
extern	void	ShotMain( ST_ObjectInfo *obj );		//	ショットメイン
extern	long	GetShotNo( void );				//	ショット空き番号取得
extern	void	ShotRotSet( ST_ObjectInfo *obj, long chara );	//	ショット角度セット
extern	void	ShotHitChk( void );				//	ショットの当たり判定
extern	void	NormarlShot( void );
extern	void	NormarlShotPush( void );
extern	void	SpecialShot( void );
extern	void	SpecialShotPush( void );
extern	void	ExtraShot1( void );
extern	void	ExtraShot2( void );
extern	void	ShotSetNormal( long obj1, long obj2, double speed, ULong atr, double rev, short chr, long pat );
extern	void	ShotSetWide( long obj1, long obj2, double speed, ULong atr, long way, double rot_rev, short chr, long pat );
