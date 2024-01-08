/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 2004-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "cmdlineparam.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SCmdLineParam::SCmdLineParam(){
	m_flag3sRemoveNOCONNECTION = 0;
	m_flag3sSetPinInfoToNoNameNet = 0;
	m_listType = 0;
	m_forceExecute	= FALSE;
	m_flag = 's';
}

void SCmdLineParam::ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast){
	if(bFlag){
		m_flag = pszParam[0];
		switch(m_flag){
		case 'N':
			m_flag3sRemoveNOCONNECTION=1;
			m_flag = 's';
			break;
		case 'n':
			m_flag3sRemoveNOCONNECTION=-1;
			m_flag = 's';
			break;
		case 'P':
			m_flag3sSetPinInfoToNoNameNet=1;
			m_flag = 's';
			break;
		case 'p':
			m_flag3sSetPinInfoToNoNameNet=-1;
			m_flag = 's';
			break;
		case 'T':
			if(pszParam[1] == 'T'){
				m_listType = TYPE_TELESIS;
			}else if(pszParam[1] == 'R'){
				m_listType = TYPE_PROTEL;
			}
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
		case 'o':
			m_strNetList = pszParam;
			m_flag = 's';
			break;
		case 'r':
			m_strReportFile = pszParam;
			m_flag = 's';
			break;
		default:
			m_flag = 's';
			break;
		}
	}
}
