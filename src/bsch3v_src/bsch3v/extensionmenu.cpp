/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#include "stdafx.h"
#include "extensionmenu.h"

CExtensionMenu::CExtensionMenu()
{
	m_option[0]=0;
}

void CExtensionMenu::setOption(const TCHAR* option)
{
	int n=0;
	while(option[n]&&(n<15)){
		m_option[n]=option[n];
		n++;
	}
	m_option[n]=0;
}

CString CExtensionMenu::formatCmdline(const TCHAR* datapath)
{
	CString result=_T("");
	CString path;
	path.Format(_T("\"%s\""),datapath);

	if(m_cmd.IsEmpty()) return result;

	result = m_cmd;
	result.Replace(_T("%f"),path);
	return result;
}

BOOL CExtensionMenu::isReload()
{
	int n=0;
	while(m_option[n]){
		if(m_option[n]=='R') return TRUE;
		n++;
	}
	return FALSE;
}

