/******************************************************************************/
//	オブジェクト情報ファイル
//		ヘッダファイル
/******************************************************************************/

#ifndef	__ObjectInfo_H__
#define	__ObjectInfo_H__

//----------------------------------------------------------------------------//
//	定義
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//
class	CObjectInfo
{
protected:
	ST_ObjectInfo	m_Info;

public:
	CObjectInfo();
	virtual ~CObjectInfo();

public:
	void	Set( long PatNo, double PosX, double PosY, double PosZ, ULong Type, ULong Atr );
	void	Set( long Width, long Height, double PosX, double PosY, double PosZ, ULong Type, ULong Atr );
	void	SetPos( double PosX, double PosY, double PosZ );
	void	SetCenter( short CenterX, short CenterY );
	void	SetScale( double Scale );
	void	SetTransfer( double MoveX, double MoveY );
	void	SetAnm( long AnmNo, long LoopFlag );
	void	Move( void );
	void	Disp( void );
	long	Clipping( RECT Rect );
	void	PosBackup( void );
	void	PosRestore( void );
	ST_ObjectInfo	GetObectInfo( void );
	void	SetObjectInfo( ST_ObjectInfo Info );
};


#endif	//	__ObjectInfo_H__
