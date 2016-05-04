// nl3w_lang.h : nl3w_lang.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// Cnl3w_langApp
// このクラスの実装に関しては nl3w_lang.cpp を参照してください。
//

class Cnl3w_langApp : public CWinApp
{
public:
	Cnl3w_langApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
