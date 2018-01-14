/******************************************************************************/
//	オブジェクト管理ファイル
//		ヘッダファイル
/******************************************************************************/

#ifndef	__ObjectMng_H__
#define	__ObjectMng_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//
#define	OBJ_MAX	5000

enum E_ObjectType
{
	_ObjTypeNone,	//	なし（初期値）
	_ObjTypeBG,		//	背景
	_ObjTypeSystem,	//	システム
	_ObjTypeRect,	//	領域
	_ObjTypePlayer,	//	プレイヤー
	_ObjTypeEnemy,	//	敵
	_ObjTypeShot,	//	ショット
};

//----------------------------------------------------------------------------//
//	構造体
//----------------------------------------------------------------------------//

//	オブジェクト管理構造体
typedef	struct
{
	//	フラグ
	UChar	Use;					//	使用フラグ
	UChar	Disp;					//	表示フラグ
	long	LoopFlag;				//	アニメループフラグ

	//	番号
	long	PatNo;					//	パターン番号
	long	PolNo;					//	ポリゴン番号
	long	AnmNo;					//	アニメ管理番号

	//	座標系
	double	PosX,PosY,PosZ;			//	表示座標
	double	PosXBk,PosYBk,PosZBk;	//	バックアップ座標
	double	MoveX,MoveY;			//	移動量
	short	CenterX,CenterY;		//	中心座標

	//	属性系
	ULong	Type;					//	タイプフラグ(機体、ショット、その他)
	ULong	Atr;					//	アトリビュート(自機、自ショット)

	short	HitRect;				//	当たり判定

	//	その他
	double	Scale;					//	拡大率
	double	RotRad,RotDeg;			//	向き(ラジアン、デグリー)
	double	Speed;					//	移動速度
	short	Dead;					//	消滅フラグ(0:生存、1:消滅、2:点滅)

	short	DispCt;					//	カウンタ（useフラグがONになってからの時間）
	short	DeadCt;					//	消滅カウンタ
	long	Ct;						//	汎用カウンタ
}ST_ObjectInfo;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CObjectMng
{
protected:
	ST_ObjectInfo	m_List[OBJ_MAX];

public:
	CObjectMng();
	~CObjectMng();
	static	CObjectMng	&GetInstance( void );
	void	Clear( void );
	void	Remove( long ObjNo );
	void	RemoveType( E_ObjectType Type );
	void	Set( long ObjNo, long PatNo, double PosX, double PosY, double PosZ, ULong Type, ULong Atr );
	void	Set(long ObjNo, long Width, long Height, double PosX, double PosY, double PosZ, ULong Type, ULong Atr );
	void	SetPos( long ObjNo, double PosX, double PosY, double PosZ );
	void	SetCenter( long ObjNo, short CenterX, short CenterY );
	void	SetSize( long ObjNo, short Width, short Height );
	void	SetScale( long ObjNo, double Scale );
	void	SetColor( long ObjNo, UChar A, UChar R, UChar G, UChar B );
	void	SetTransfer( long ObjNo, double MoveX, double MoveY );
	void	SetAnm( long ObjNo, long AnmNo, long LoopFlag );
	ST_ObjectInfo	GetObjectInfo( long ObjNo );
	ST_ObjectInfo	*GetObjectInfo( void );
	void	SetObjectInfo( long ObjNo, ST_ObjectInfo Info );
	void	Loop( void );
	long	GetCount( ST_ObjectInfo Info );
	long	GetObjectNo( void );
	void	AddMovePos( long ObjNo, double AddX, double AddY );
	void	DispChange( long ObjNo, UChar Flag );

protected:
	void	Disp( long ObjNo );
	void	Move( long ObjNo );
	long	Clipping( long ObjNo, RECT Rect );
	void	PosBackup( long ObjNo );
	void	PosRestore( long ObjNo );
};

class CObjectType
{
public:
	static const E_ObjectType None = _ObjTypeNone;
	static const E_ObjectType BG = _ObjTypeBG;
	static const E_ObjectType System = _ObjTypeSystem;
	static const E_ObjectType Rect = _ObjTypeRect;
	static const E_ObjectType Player = _ObjTypePlayer;
	static const E_ObjectType Enemy = _ObjTypeEnemy;
	static const E_ObjectType Shot = _ObjTypeShot;
};

#endif	//	__ObjectMng_H__

extern class	CObjectType g_ObjType;