// lcov_lang.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include "lcov_lang.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: この DLL が MFC DLL に対して動的にリンクされる場合、
//		MFC 内で呼び出されるこの DLL からエクスポートされたどの関数も
//		関数の最初に追加される AFX_MANAGE_STATE マクロを
//		持たなければなりません。
//
//		例:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 通常関数の本体はこの位置にあります
//		}
//
//		このマクロが各関数に含まれていること、MFC 内の
//		どの呼び出しより優先することは非常に重要です。
//		これは関数内の最初のステートメントでなければな 
//		らないことを意味します、コンストラクタが MFC
//		DLL 内への呼び出しを行う可能性があるので、オブ
//		ジェクト変数の宣言よりも前でなければなりません。
//
//		詳細については MFC テクニカル ノート 33 および
//		58 を参照してください。
//

// Clcov_langApp

BEGIN_MESSAGE_MAP(Clcov_langApp, CWinApp)
END_MESSAGE_MAP()


// Clcov_langApp コンストラクション

Clcov_langApp::Clcov_langApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の Clcov_langApp オブジェクトです。

Clcov_langApp theApp;


// Clcov_langApp 初期化

BOOL Clcov_langApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
