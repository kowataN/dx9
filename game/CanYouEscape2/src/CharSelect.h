/******************************************************************************/
//	キャラクターセレクト
//		ヘッダファイル
/******************************************************************************/

#ifndef	__CharSelect_H__
#define	__CharSelect_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//
enum
{
	//	テクスチャ番号
	TEX_CHAR_SELECT_BG = 0,
	TEX_CHAR_SELECT_CHAR,
	TEX_CHAR_SELECT_SHOT,

	//	パターン番号
	PAT_CHAR_SELECT_BG = 0,
	PAT_CHAR_SELECT_1P,
	PAT_CHAR_SELECT_2P,
	PAT_CHAR_SELECT_SHOT,

	//	ポリゴン番号
	POL_CHAR_SELECT_BG = 0,
	POL_CHAR_SELECT_CUR_1P,
	POL_CHAR_SELECT_CUR_2P,
	POL_CHAR_SELECT_1P,
	POL_CHAR_SELECT_2P,
	POL_CHAR_SELECT_SHOT,

	//	オブジェクト番号
	OBJ_CHAR_SELECT_BG = 0,
	OBJ_CHAR_SELECT_ICON,
	OBJ_CHAR_SELECT_CUR_1P,
	OBJ_CHAR_SELECT_CUR_2P,
	OBJ_CHAR_SELECT_1P,
	OBJ_CHAR_SELECT_2P,
	OBJ_CHAR_SELECT_SHOT,
};

#endif	//	__CharSelect_H__

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
extern	short	SelectFlag[2];

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	CharSelectInit( void );	//	キャラセレクト初期化
extern	void	CharSelectMain( void );	//	キャラセレクトメイン
