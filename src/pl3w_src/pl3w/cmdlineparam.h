/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef CMDLINEPARAM_H
#define CMDLINEPARAM_H



////////////////////////////////////////////////////////////////////
//�R�}���h���C�����N���X
class SCmdLineParam : public CCommandLineInfo
{

public:
	SCmdLineParam();
	virtual ~SCmdLineParam(){};

	CStringList m_strListSrcFiles;
	CString		m_strList;
	int			m_dstType;	//0:�w��Ȃ�  1:CSV   2:SYLK
	//int			m_flag3sAvoidZeroSupOnExcel; //0:�w��Ȃ� 1:�[���T�v���X�������  -1:���ɉ������Ȃ�
	int			m_flag3sMfr;		//0:�w��Ȃ� 1:�����҂��o��     -1:�o�͂��Ȃ�
	int			m_flag3sMfrPn;		//0:�w��Ȃ� 1:�����Ҍ^�����o�� -1:�o�͂��Ȃ�
	int			m_flag3sPackage;	//0:�w��Ȃ� 1:�p�b�P�[�W���o�� -1:�o�͂��Ȃ�
	int			m_flag3sNote;		//0:�w��Ȃ� 1:�m�[�g���o��     -1:�o�͂��Ȃ�
	int			m_flag3sInfo;		//0:�w��Ȃ� 1:���i�����o��    -1:�o�͂��Ȃ�
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
