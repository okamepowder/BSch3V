// lcov_lang.h : lcov_lang.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// Clcov_langApp
// このクラスの実装に関しては lcov_lang.cpp を参照してください。
//

class Clcov_langApp : public CWinApp
{
public:
	Clcov_langApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
