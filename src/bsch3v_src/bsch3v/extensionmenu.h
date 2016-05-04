/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef EXTENSIONMENU_H
#define EXTENSIONMENU_H

class CExtensionMenu
{
public:
	CExtensionMenu();
	CString m_cmd;
	CString formatCmdline(const TCHAR* datapath);
	BOOL isReload();

	void setOption(const TCHAR* option);
protected:
	TCHAR m_option[16]; 
};
#endif