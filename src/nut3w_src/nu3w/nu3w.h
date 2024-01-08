/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// nu3w.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"		// メイン シンボル

#include "cmdlineparam.h"
// Cnu3wApp:
// このクラスの実装については、nu3w.cpp を参照してください。
//

class Cnu3wApp : public CWinApp
{
public:
	Cnu3wApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();
public:
	SCmdLineParam m_cmdInfo;

// 実装

	DECLARE_MESSAGE_MAP()

private:
	TCHAR *m_pszIniFileName;
};

extern Cnu3wApp theApp;
