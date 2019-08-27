
#pragma once

// 以下のマクロは、最低限必要なプラットフォームを定義します。最低限必要なプラットフォームとは、
// アプリケーションを実行するために必要な機能を備えた最も古いバージョンの Windows や Internet Explorer など
// をいいます。これらのマクロは、指定したバージョンと、それ以前のバージョンのプラットフォーム上で利用できるすべての機能を有効にすることによって 
// 動作します。

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

