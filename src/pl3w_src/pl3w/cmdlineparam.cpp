/****************************************************************************
    NL3W netlist utility for BSch3V
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
	m_dstType = 0;
	m_forceExecute	= FALSE;
//	m_flag3sAvoidZeroSupOnExcel = 0;
	m_flag3sMfr = 0;
	m_flag3sMfrPn = 0;
	m_flag3sPackage = 0;
	m_flag3sNote = 0;
	m_flag3sInfo = 0;
	m_flag = 's';
}

void SCmdLineParam::ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast){
	if(bFlag){
		m_flag = pszParam[0];
		switch(m_flag){
		case 'T':
		case 't':
			if(pszParam[1]=='S' || pszParam[1]=='s'){
				m_dstType = 2;
			}else if(pszParam[1]=='C' || pszParam[1]=='c'){
				m_dstType = 1;
			}
			break;

		case 'F':
			m_forceExecute=TRUE;
			m_flag = 's';
			break;
		case 'M':
			if(pszParam[1] == 'N' || pszParam[1] == 'n'){	//2ï∂éöñ⁄Ç™NÇ‹ÇΩÇÕnÇÃÇ∆Ç´ÇÕêªë¢é“å^éÆ
				m_flag3sMfrPn = 1;
			}else{
				m_flag3sMfr = 1;
			}
			m_flag = 's';
			break;
		case 'm':
			if(pszParam[1] == 'N' || pszParam[1] == 'n'){	//2ï∂éöñ⁄Ç™NÇ‹ÇΩÇÕnÇÃÇ∆Ç´ÇÕêªë¢é“å^éÆ
				m_flag3sMfrPn = -1;
			}else{
				m_flag3sMfr = -1;
			}
			m_flag = 's';
			break;
		case 'P':
			m_flag3sPackage = 1;
			m_flag = 's';
			break;
		case 'p':
			m_flag3sPackage = -1;
			m_flag = 's';
			break;
		case 'N':
			m_flag3sNote = 1;
			m_flag = 's';
			break;
		case 'n':
			m_flag3sNote = -1;
			m_flag = 's';
			break;
		//case 'Z':
		//	m_flag3sAvoidZeroSupOnExcel = 1;
		//	m_flag = 's';
		//	break;
		//case 'z':
		//	m_flag3sAvoidZeroSupOnExcel = -1;
		//	m_flag = 's';
		//	break;
		case 'I':
			m_flag3sInfo = 1;
			m_flag = 's';
			break;
		case 'i':
			m_flag3sInfo = -1;
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
			m_strList = pszParam;



			m_flag = 's';
			break;
		default:
			m_flag = 's';
			break;
		}
	}
}
