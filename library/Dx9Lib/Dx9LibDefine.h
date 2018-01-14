/******************************************************************************/
//	DirectX9ライブラリ
//		定数
/******************************************************************************/

#ifndef	__Dx9LibDefine_H__
#define	__Dx9LibDefine_H__

////////////////////////////////////////////////////////////////////////////////
//	定数（変更可）

////////////////////////////////////////
//	ウインドウ関連

//	アプリケーションの名前
#define	APP_NAME			"GameMain"

//	画面のサイズ
#define	WIDTH				640
#define	HEIGHT				480

//	ウインドウモード
//		１：フルスクリーンモード
//		０：ウインドウモード
#define	SCREEN_MODE			0

//	ウインドウモード切り替え
//		ON ：切り替え有り
//		OFF：切り替え無し
#define	CHANGE_SCREEN_MODE	OFF

//	終了確認
//		ON ：あり
//		OFF：なし
#define	WIN_CLOSE			OFF

//	ウインドウクリアカラー
#define	WIN_CLEAR_R			0
#define	WIN_CLEAR_G			0
#define	WIN_CLEAR_B			0

//	多重機動制御フラグ
//		ON ：多重起動防止
//		OFF：多重起動許可
#define	MULTIPLEX_FLAG		OFF

//	読み込み可能最大サイズ（デフォで１０Ｍ）
#define	FREE_WORK_SIZE		(10*1024*1024)

////////////////////////////////////////
//	グラフィック関連

//	テクスチャ最大数
#define	TEX_MAX				128

//	パターン最大数
#define	PAT_MAX				512

//	スプライト最大数
#define	SP_MAX				2048

//	ライン最大数
#define	LINE_MAX			5000

//	ポリゴン最大数
#define	POL_MAX				2048

//	透明色	ARGB
#define	TRANS				0xFF00FF00

////////////////////////////////////////
//	インプット関連

//	マウスカーソル表示切り替え
//		TRUE：表示
//		FALAE：非表示
#define	CURSOR_DISP			TRUE

//	ジョイスティックの使用フラグ
//		ON：使用
//		OFF：未使用
#define	JOY_USE				ON

//	ジョイスティックの最大数
#define	JOY_MAX				4

//	ジョイスティックのボタン最大数
#define	BUTTON_MAX			(JOY_BUT_MAX)

////////////////////////////////////////
//	オーディオ関連

//	オーディオの最大数
#define	AUDIO_MAX			16

//	オーディオファイルの最大数
#define	AUDIO_SIZE_MAX		(2*1024*1024)	//	２Ｍまで

////////////////////////////////////////
//	テキスト関連

//	テキスト最大数
#define	TXT_MAX				1024

//	文字数
#define	TXT_SIZE			1024

//	フォントサイズ
#define	FONT_SIZE			14

//	フォント
#define	FONT_TYPE			"ＭＳゴシック"

////////////////////////////////////////
//	プレイ関連

//	IPアドレス
#define	CONNECT_IP			"114.167.198.222"

//	ポート
#define	CONNECT_PORT		30000

////////////////////////////////////////
//	その他

//	フレーム数
#define	FPS					60

//	FPS表示フラグ
//		ON：表示
//		OFF：非表示
#define	FPS_DISP			ON

////////////////////////////////////////////////////////////////////////////////
//	定数（変更禁止）

////////////////////////////////////////
//	グラフィック関連

//	バッファサイズ
#define	DIDEVICE_BUFFERSIZE	100

//	アトリビュート
#define	ATR_NONE			0	//	何もしない
#define	ATR_REV_W			1	//	左右反転
#define	ATR_REV_H			2	//	上下反転
#define	ATR_ALL				( ATR_REV_W | ATR_REV_H )	//	左右上下反転
#define	ATR_BOX				1

////////////////////////////////////////
//	その他

//	円周率
#define	PI					3.1415926

//	フラグ用
#define	ON					1
#define	OFF					0
#define	NOT					(-1)

////////////////////////////////////////
//	unsigned型
typedef	unsigned char		UChar;
typedef	unsigned short		UShort;
typedef	unsigned long		ULong;

////////////////////////////////////////////////////////////////////////////////
//	マクロ
#define	SAFE_DELETE(x)		if(x){delete x;x=NULL;};
#define	SAFE_DELETE_ARY(x)	if(x){delete [] x;x=NULL;};
#define	SAFE_RELEASE(X)		if(X){X->Release();X=NULL;}
#define	FVF_TLVERTEX		(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

////////////////////////////////////////////////////////////////////////////////
//	定義
enum	E_WindowStyle
{
	WindowMode,
	FullScreenMode
};

#endif	//	__Dx9LibDefine_H__