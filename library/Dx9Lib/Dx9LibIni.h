/******************************************************************************/
//	DirectX9���C�u����
//		�\���t�@�C���Ǘ�
/******************************************************************************/

#ifndef	__Dx9LibIni_H__
#define	__Dx9LibIni_H__

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	<string>
using namespace std;

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//

//	�\���t�@�C���Ǘ��N���X
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
