/******************************************************************************/
//	DirectX9���C�u����
//		���O�o��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�C���N���[�h
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"
#include	<vector>
using namespace std;



/******************************************************************************/
//	���O	�F	�R���X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibLog::CDx9LibLog()
{
	sprintf_s( m_FileName, MAX_PATH, "Result.log" );
}
CDx9LibLog::CDx9LibLog( char *FileName  )
{
	strcpy_s( m_FileName, MAX_PATH, FileName );
}

/******************************************************************************/
//	���O	�F	�f�X�g���N�^
//	����	�F	�Ȃ�
//	�߂�l	�F	�Ȃ�
//	����	�F	�Ȃ�
//	���l	�F	�Ȃ�
/******************************************************************************/
CDx9LibLog::~CDx9LibLog()
{
}

//void	CDx9LibLog::Write( E_LogType Type, char *Msg )
//{
//	ofstream OutFile( m_FileName, ios::out|ios::app|ios::ate );
//	if( OutFile.fail() )
//	{
//		return;
//	}
//	OutFile << Type << "    " << Msg << endl;
//}

void	CDx9LibLog::Write( E_LogType Type, char *Fromat, ... )
{
	char	strLogType[ LogTypeMax ][128] =
		{ "DebugLog", "ErrLog  " };
	va_list	Argp;
	char	pszBuf[MAX_PATH];
	long	Count = 0;

	va_start( Argp, Fromat );
	vsprintf_s( pszBuf, 256, Fromat, Argp );
	va_end( Argp );

	vector<string>	SplitStr = SplitText(pszBuf, "^");
	char Name[MAX_PATH];
	PathToName( (char*)SplitStr[0].c_str(), Name );

#ifdef	_DEBUG
	ofstream OutFile( m_FileName, ios::out|ios::app|ios::ate );
	if( OutFile.fail() )
	{
		return;
	}
	sprintf_s( pszBuf, MAX_PATH, "%-24s %5s�s�� %s", Name, SplitStr[1].c_str(), SplitStr[2].c_str() );
	OutFile << strLogType[Type] << " " << pszBuf << endl;
	OutFile.close();
#else
	if( Type == TypeErrLog )
	{
		ofstream OutFile( m_FileName, ios::out|ios::app|ios::ate );
		if( OutFile.fail() )
		{
			return;
		}
		OutFile << strLogType[Type] << "    " << pszBuf << endl;
	}
#endif	//	_DEBUG
}
