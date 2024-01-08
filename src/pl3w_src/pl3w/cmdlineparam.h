/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef CMDLINEPARAM_H
#define CMDLINEPARAM_H



////////////////////////////////////////////////////////////////////
//コマンドライン情報クラス
class SCmdLineParam : public CCommandLineInfo
{

public:
	SCmdLineParam();
	virtual ~SCmdLineParam(){};

	CStringList m_strListSrcFiles;
	CString		m_strList;
	int			m_dstType;	//0:指定なし  1:CSV   2:SYLK
	//int			m_flag3sAvoidZeroSupOnExcel; //0:指定なし 1:ゼロサプレスを避ける  -1:特に何もしない
	int			m_flag3sMfr;		//0:指定なし 1:製造者を出力     -1:出力しない
	int			m_flag3sMfrPn;		//0:指定なし 1:製造者型式を出力 -1:出力しない
	int			m_flag3sPackage;	//0:指定なし 1:パッケージを出力 -1:出力しない
	int			m_flag3sNote;		//0:指定なし 1:ノートを出力     -1:出力しない
	int			m_flag3sInfo;		//0:指定なし 1:部品情報を出力    -1:出力しない
	BOOL		m_forceExecute;

protected:
	char		m_flag;
	void ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast
	);
};


#endif
