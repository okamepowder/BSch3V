#ifndef CMDLINEPARAM_H
#define CMDLINEPARAM_H



////////////////////////////////////////////////////////////////////
//���i���C�u�����N���X
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
