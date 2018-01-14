/******************************************************************************/
//	DirectX9���C�u����
//		�R����
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//
//	�ėp
extern	char	g_FreeWork[FREE_WORK_SIZE];

//----------------------------------------------------------------------------//
//	�v���g�^�C�v�錾
//----------------------------------------------------------------------------//
extern	long	LoadFile( char *FileName, char *LoadBuf );							//	�t�@�C�����[�h
extern	long	SaveFile( char *FileName, char *SaveBuf, long Size );				//	�t�@�C���Z�[�u
extern	void	PathToName( char *Path, char *FileName);							//	�p�X����t�@�C�����擾
extern	void	PathToOpt( char *Path, char *FileName );							//	�p�X����g���q�擾
extern	void	PathToDir( char *Path, char *Dir );									//	�p�X����f�B���N�g���擾
extern	void	PathChange( char *NewPath, char *BakupPath );						//	�p�X�ύX
extern	long	Secret( char *FileName );											//	�Í���
extern	void	SaveSecret( char *FileName, char *SaveFileName );					//	�Í��t�@�C���ۑ�
extern	long	AntiSecret( char *SecretFile, char *FileName );						//	���
extern	void	SetShort( short Dat, char *pt );									//	�V���[�g�^��ݒ�
extern	void	SetLong( long Dat, char *pt );										//	�����O�^��ݒ�
extern	void	SetFloat( float Dat, char *pt );									//	�t���[�g�^��ݒ�
extern	void	SetDouble( double Dat, char *pt );									//	�_�u���^����ݒ�
extern	short	GetShort( char *pt );												//	�V���[�g�^�f�[�^�̎擾
extern	long	GetLong( char *pt );												//	�����O�^�f�[�^�̎擾
extern	float	GetFloat( char *pt );												//	�t���[�g�^�f�[�^�̎擾
extern	double	GetDouble( char *pt );												//	�_�u���^�f�[�^�̎擾
extern	bool	LoadFileDialog( char *Filter, char *Ext, char *Title, char *Path );	//	�t�@�C���I�[�v���_�C�A���O�\��
extern	bool	SaveFileDialog( char *Filter, char *Ext, char *Title, char *Path );	//	�t�@�C���Z�[�u�_�C�A���O�\��
extern	double	GetRot( double x1, double y1, double x2, double y2 );				//	�p�x�擾
extern	double	GetDistance( double x1, double y1, double x2, double y2 );			//	�����擾
extern	void	CorrectRot( double &Rot );
extern	double	RadToDeg( double Rot );												//	���W�A�����f�O���[�ɕϊ�
extern	double	DegToRad( double Rot );												//	�f�O���[�����W�A���ɕϊ�
extern	double	GetMoveX( double Speed, double Rot );								//	�ړ��ʎ擾(X)
extern	double	GetMoveY( double Speed, double Rot );								//	�ړ��ʎ擾(Y)
extern	void	MaxMin( double& Target, double Max, double Min );
extern	void	MaxMin( long& Target, long Max, long Min );
extern	ULong	ToDec( const char *Target );
extern	void	WaitTime( long time );
extern	vector<string>	SplitText( string Str, string delim );
