/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef CMDLINEPARAM_H
#define CMDLINEPARAM_H



////////////////////////////////////////////////////////////////////
//部品ライブラリクラス
class SCmdLineParam : public CCommandLineInfo
{

public:
	SCmdLineParam();
	virtual ~SCmdLineParam(){};

	CStringList m_strListSrcFiles;
	int			m_flag3sAutoNum;
	int			m_initNum;
	int			m_stepNum;
	BOOL		m_forceExecute;

protected:
	TCHAR		m_flag;
	void ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast
	);
};


#endif
