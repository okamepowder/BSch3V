// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//


#pragma once

#define _BIND_TO_CURRENT_VCLIBS_VERSION 1




// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER                  // Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501           // これを Windows XP またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT            // Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINNT 0x0501     // これを Windows XP またはそれ以降のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS          // Windows ME 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0500   // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE               // IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0600        // これを IE 6.0  またはそれ以降のバージョン向けに適切な値に変更してください。
#endif


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

// 一般的で無視しても安全な MFC の警告メッセージの一部の非表示を解除します。
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC オートメーション クラス

#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT




//#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

//#if !defined(AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_)
//#define AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。
//
//#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
//#include <afxext.h>         // MFC の拡張部分
//#include <afxdisp.h>        // MFC のオートメーション クラス
//#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
//#endif // _AFX_NO_AFXCMN_SUPPORT
//
//
////{{AFX_INSERT_LOCATION}}
//// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
//
//#endif // !defined(AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_)
