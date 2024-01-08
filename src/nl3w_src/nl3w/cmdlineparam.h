/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 2004-2014 H.Okada
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
	CString		m_strNetList;
	CString		m_strReportFile;
	int			m_flag3sRemoveNOCONNECTION;
	int			m_flag3sSetPinInfoToNoNameNet;
	int			m_listType;
	BOOL		m_forceExecute;

	enum{
		TYPE_TELESIS = 1,
		TYPE_PROTEL
	};

protected:
	char		m_flag;
	void ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast
	);
};


#endif
