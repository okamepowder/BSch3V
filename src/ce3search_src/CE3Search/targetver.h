
#pragma once

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
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

