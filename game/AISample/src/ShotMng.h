/******************************************************************************/
//	�V���b�g���C��
//		�w�b�_�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	void	ShotInitAll( void );			//	�V���b�g�������i�S�́j
extern	void	ShotInit( ST_ObjectInfo *obj );		//	�V���b�g�������i�P�́j
extern	void	ShotLoop( void );				//	�V���b�g���[�v
extern	void	ShotMain( ST_ObjectInfo *obj );		//	�V���b�g���C��
extern	long	GetShotNo( void );				//	�V���b�g�󂫔ԍ��擾
extern	void	ShotRotSet( ST_ObjectInfo *obj, long chara );	//	�V���b�g�p�x�Z�b�g
extern	void	ShotHitChk( void );				//	�V���b�g�̓����蔻��
extern	void	NormarlShot( void );
extern	void	NormarlShotPush( void );
extern	void	SpecialShot( void );
extern	void	SpecialShotPush( void );
extern	void	ExtraShot1( void );
extern	void	ExtraShot2( void );
extern	void	ShotSetNormal( long obj1, long obj2, double speed, ULong atr, double rev, short chr, long pat );
extern	void	ShotSetWide( long obj1, long obj2, double speed, ULong atr, long way, double rot_rev, short chr, long pat );
