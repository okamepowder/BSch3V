// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//
#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include "targetver.h"

//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#define _CRT_SECURE_NO_WARNINGS


#include <afxwin.h>         // MFC の標準コンポーネント
#include <afxext.h>         // MFC の拡張部分

#include "resource.h"       // メイン シンボル



#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows 95 コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <Shlwapi.h>