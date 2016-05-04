
// CE3Search.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "cmdlineparam.h"
// CCE3SearchApp:
// See CE3Search.cpp for the implementation of this class
//

//class CCE3SearchApp : public CWinAppEx
class CCE3SearchApp : public CWinApp
{
public:
	CCE3SearchApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	SCmdLineParam m_cmdInfo;

private:
	TCHAR *m_pszIniFileName;
};

extern CCE3SearchApp theApp;