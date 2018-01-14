/******************************************************************************/
//	ショットメイン
//		ヘッダファイル
/******************************************************************************/

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	ShotInitAll( void );			//	ショット初期化（全体）
extern	void	ShotInit( OBJ_TBL *obj );		//	ショット初期化（単体）
extern	void	ShotLoop( void );				//	ショットループ
extern	void	ShotMain( OBJ_TBL *obj );		//	ショットメイン
extern	long	GetShotNo( void );				//	ショット空き番号取得
extern	void	ShotRotSet( OBJ_TBL *obj, long chara );	//	ショット角度セット
extern	void	ShotHitChk( void );				//	ショットの当たり判定
extern	void	*ShotTbl[];
