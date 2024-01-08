#include "stdafx.h"
#include "cmdlineparam.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SCmdLineParam::SCmdLineParam(){
	m_strPath = "";
	m_setPath = false;
}

void SCmdLineParam::ParseParam(
				const TCHAR* pszParam,
				BOOL bFlag,
				BOOL bLast){
	if(!bFlag){
		m_strPath = pszParam;
		m_setPath = true;
	}
}
