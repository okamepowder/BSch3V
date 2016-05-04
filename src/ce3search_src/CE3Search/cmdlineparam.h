#ifndef CMDLINEPARAM_H
#define CMDLINEPARAM_H



////////////////////////////////////////////////////////////////////
//部品ライブラリクラス
class SCmdLineParam : public CCommandLineInfo
{

public:
	SCmdLineParam();
	virtual ~SCmdLineParam(){};

	CString		m_strPath;
	bool		m_setPath;

protected:
	void ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast
	);
};


#endif
