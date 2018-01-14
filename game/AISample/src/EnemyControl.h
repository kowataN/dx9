/******************************************************************************/
//	エネミー制御
//		ソースファイル
/******************************************************************************/

#ifndef	__EnemyControl_H__
#define	__EnemyControl_H__

#include	<vector>
#include	"./ObjectMng.h"
using namespace std;

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//

//	状態
enum E_State
{
	_StateStop,		//	停止
	_StateMove,		//	移動
	_StateAtack,	//	攻撃
};

//	位置
enum	E_Position
{
	_PosLeftUp,		//	左上
	_PosCenterUp,	//	上
	_PosRightUp,	//	右上
	_PosLeft,		//	左
	_PosCenter,		//	真ん中
	_PosRight,		//	右
	_PosLeftDown,	//	左下
	_PosCenterDown,	//	下
	_PosRightDown,	//	右下
	_PosMax,		//	最大値
};

//	距離
enum E_Lange
{
	_LngShort,
	_LngMiddle,
	_LngLong,
};

//	移動フラグ
enum	E_MoveFlag
{
	_MoveFlagSet,	//	設定
	_MoveFlagLoop,	//	ループ
};

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

////////////////////////////////////////
//	ユニット用
typedef	struct
{
	long	ObjectNo;			//	オブジェクト番号
	long	TargetObjNo;		//	目標オブジェクト番号

	RECT	PatSize;			//	パターンサイズ
	short	CenterX,CenterY;	//	中心座標
	short	HitRect;			//	当たり範囲
	double	Speed;				//	移動速度

	long	Shot[5];			//	ショット番号
	//	ショットフラグ
	//	(0:押してない、1:ノーマル、2:特殊１、3:特殊２、4:ＥＸ１、5:ＥＸ２)
	long	ButNo;
	long	ShotLv;				//	ショットレベル

	long	Life;				//	ライフ
	E_State	State;				//	状態
	double	Distance;			//	距離
	E_Position	Pos;			//	位置
	long	ShotTimer;			//	ショットタイマ
	long	ShotCt;				//	ショットカウンタ
	E_MoveFlag	MoveFlag;			//	移動フラグ
	long	MoveTimer;			//	移動タイマ
	long	MoveCt;				//	移動カウンタ
}ST_Enemy;

#endif	//	__EnemyControl_H__

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CEnemy
{
private:
	ST_Enemy	m_Info;
	UChar		m_InitFlag;

public:
	CEnemy();
	~CEnemy();
	void	Init( long OjbjNo, long TargetNo, double PosX, double PosY );
	void	MainLoop( void );

private:
	void	Stop( void );
	void	Move( void );
	void	Atack( void );
	void	HitChk( void );
	void	Turn( long ObjNo1, long ObjNo2 );
	vector<ST_ObjectInfo> GetShot( void );
	void	SetPostion( void );
	void	SetDistance( void );
	void	SetMove( void );
	void	SetMovePos( ST_ObjectInfo MyInfo, ST_ObjectInfo Player, double &MoveX, double &MoveY );
	void	SetEscape( ST_ObjectInfo MyInfo, vector<ST_ObjectInfo> Shot, double &MoveX, double &MoveY );
	void	LoopMove( void );
	E_Lange	GetLange( void );
	E_Lange	GetLange( double PosX1, double PosY1, double PosX2, double PosY2 );
};
