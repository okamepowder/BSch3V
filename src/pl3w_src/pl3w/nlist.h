/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


// nlist.h : NLIST アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_NLIST_H__4D8E6AF1_916A_40CF_856D_3E12A75CF47D__INCLUDED_)
#define AFX_NLIST_H__4D8E6AF1_916A_40CF_856D_3E12A75CF47D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

#include "cmdlineparam.h"

/////////////////////////////////////////////////////////////////////////////
// CNlistApp:
// このクラスの動作の定義に関しては nlist.cpp ファイルを参照してください。
//

class CNlistApp : public CWinApp
{
public:
	CNlistApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNlistApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CNlistApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	SCmdLineParam m_cmdInfo;
private:
	TCHAR *m_pszIniFileName;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NLIST_H__4D8E6AF1_916A_40CF_856D_3E12A75CF47D__INCLUDED_)
