// bsch3v_lang.h : bsch3v_lang.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// Cbsch3v_langApp
// このクラスの実装に関しては bsch3v_lang.cpp を参照してください。
//

class Cbsch3v_langApp : public CWinApp
{
public:
	Cbsch3v_langApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
