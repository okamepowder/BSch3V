/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// LCoV.h : LCoV アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif




// CLCoVApp:
// このクラスの実装については、LCoV.cpp を参照してください。
//

class CLCoVApp : public CWinApp
{
public:
	CLCoVApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	TCHAR m_szIniFileDir[_MAX_PATH];

private:
	TCHAR *m_pszIniFileName;
	TCHAR m_moduleFileName[_MAX_PATH];


// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	bool m_bTempMode;
};

extern CLCoVApp theApp;
