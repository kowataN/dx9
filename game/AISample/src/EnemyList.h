/******************************************************************************/
//	�G�l�~�[�ꗗ
//		�w�b�_�t�@�C��
/******************************************************************************/

#ifndef	__EnemyList_H__
#define	__EnemyList_H__

#include	"./EnemyControl.h"

//----------------------------------------------------------------------------//
//	��`
//----------------------------------------------------------------------------//

//	�ő吔
#define	ENEMY_MAX	100

#endif	//	__EnemyList_H__

//----------------------------------------------------------------------------//
//	�N���X
//----------------------------------------------------------------------------//
class	CEnemyList
{
private:
	CEnemy	m_List[ENEMY_MAX];
	UChar	m_Use[ENEMY_MAX];

public:
	CEnemyList();
	~CEnemyList();
	void	Init( long EnemyNo, long OjbjNo, long TargetNo, double PosX, double PosY );
	void	MainLoop( void );
};
