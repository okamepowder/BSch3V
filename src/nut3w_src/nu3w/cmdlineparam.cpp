/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "cmdlineparam.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SCmdLineParam::SCmdLineParam(){
	m_flag3sAutoNum = -1;
	m_initNum = 1;
	m_stepNum = 1;
	m_flag = 's';
	m_forceExecute=FALSE;
}

void SCmdLineParam::ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast){
	if(bFlag){
		m_flag = pszParam[0];
		switch(m_flag){
		case 'A':
			m_flag3sAutoNum=1;
			m_flag = 's';
			break;
		case 'a':
			m_flag3sAutoNum=-1;
			m_flag = 's';
			break;
		case 'F':
			m_forceExecute=TRUE;
			m_flag = 's';
			break;
		default:
			break;
		}

	}else if(pszParam[0]){
		switch(m_flag){
		case 's':
			m_strListSrcFiles.AddTail(pszParam);
			break;
		case 'I':
			m_initNum = _tstoi(pszParam);
			m_flag = 's';
			break;
		case 'S':
			m_stepNum = _tstoi(pszParam);
			m_flag = 's';
			break;
		default:
			m_flag = 's';
			break;
		}
	}
}
