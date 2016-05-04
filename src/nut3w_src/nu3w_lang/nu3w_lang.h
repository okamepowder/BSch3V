// nu3w_lang.h : nu3w_lang.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// Cnu3w_langApp
// このクラスの実装に関しては nu3w_lang.cpp を参照してください。
//

class Cnu3w_langApp : public CWinApp
{
public:
	Cnu3w_langApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
