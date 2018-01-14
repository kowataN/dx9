/******************************************************************************/
//	タイトルメイン
//		ヘッダファイル
/******************************************************************************/

#ifndef	__Title_H__
#define	__Title_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//
enum
{
	//	テクスチャ番号
	TEX_TITLE_BG = 0,	//	背景
	TEX_TITLE_SYS,		//	その他

	//	パターン番号
	PAT_TITLE_BG = 0,	//	背景
	PAT_TITLE_TEXT1,	//	「CAN YOU」
	PAT_TITLE_TEXT2,	//	「ESCAPA2」
	PAT_TITLE_TEXT3,	//	「PUSH START」
	PAT_TITLE_TEXT4,	//	「GAME_START」
	PAT_TITLE_TEXT5,	//	「OPTION」

	//	ポリゴン番号
	POL_TITLE_BG = 0,	//	背景
	POL_TITLE_TEXT1,	//	「CAN YOU」
	POL_TITLE_TEXT2,	//	「ESCAPA2」
	POL_TITLE_TEXT3,	//	「PUSH START」
	POL_TITLE_TEXT4,	//	「GAME_START」
	POL_TITLE_TEXT5,	//	「OPTION」

	//	オブジェクト番号
	OBJ_TITLE_BG = 0,	//	背景
	OBJ_TITLE_TEXT1,	//	「CAN YOU」
	OBJ_TITLE_TEXT2,	//	「ESCAPA2」
	OBJ_TITLE_TEXT3,	//	「PUSH START」
	OBJ_TITLE_TEXT4,	//	「GAME_START」
	OBJ_TITLE_TEXT5,	//	「OPTION」

	//	その他
	TITLE_TIME = 60,
};

#endif	//	__Title_H__

//----------------------------------------------------------------------------//
//	プロトタイプ宣言
//----------------------------------------------------------------------------//
extern	void	TitleInit( void );	//	タイトル初期化
extern	short	TitleMain( void );	//	タイトルメイン
