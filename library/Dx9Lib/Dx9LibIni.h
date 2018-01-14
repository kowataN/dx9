/******************************************************************************/
//	DirectX9ライブラリ
//		構成ファイル管理
/******************************************************************************/

#ifndef	__Dx9LibIni_H__
#define	__Dx9LibIni_H__

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	<string>
using namespace std;

//----------------------------------------------------------------------------//
//	クラス
//----------------------------------------------------------------------------//

//	構成ファイル管理クラス
class CDx9LibIni
{
private:
	char	m_FilePath[MAX_PATH];

public:
	CDx9LibIni();
	CDx9LibIni(char *FilePath);
	virtual ~CDx9LibIni();
	void	Initialize( char *FilePath );
	string	GetValueString( char *Section, char *Key );
	int		GetValueInt( char *Section, char *Key );
};

#endif	//	__Dx9LibIni_H__
