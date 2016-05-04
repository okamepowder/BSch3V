/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Global.h グローバル変数の外部参照の宣言をする
//
/////////////////////////////////////////////////////////////////////////////

#include <string>
using namespace std;


//////////////////////////////////////////////////////////////////////
//画面表示関連
extern BOOL g_bDisplayPartFrame;
extern BOOL g_bDisplayNcPinMark;
extern BOOL g_bDisplayHiddenPinNum;	//0.63.01

extern CWnd* g_pViewWindow;

//////////////////////////////////////////////////////////////////////
//環境関連
extern BOOL g_bJapaneseLangEnv;	
extern BOOL g_bAvailableGDIplus;

//////////////////////////////////////////////////////////////////////
//アクティブなドキュメントのパス名のコピー

extern wstring g_docPath;
extern wstring g_docTimeStamp;
extern wstring g_docDate;
extern wstring g_printTime;




//////////////////////////////////////////////////////////////////////
//ログ関連

//extern wstring g_log;
//extern TCHAR g_logPath[];

//////////////////////////////////////////////////////////////////////
//印刷関連
//extern int g_bPrintPartNum;		//印刷時の部品番号出力    1997/01/26
//extern int g_bPrintPartName;	//印刷時の部品名出力    1998/07/22


/*     PartLib.hに移動 1997/01/04
//////////////////////////////////////////////////////////////////////
//ライブラリ関連
extern CPartLib* g_pPartLib[MAX_LIB];	//部品ライブラリ
BOOL g_ReadLibrary();	//レジストリからライブラリの情報を得て読み込む
void g_FreeLibrary();	//ライブラリのメモリの解放を行う
CPartIndex* g_SearchPartIndex(LPCSTR pszName);	//ライブラリから部品インデックスを得る
*/

/*     BSchFont.hに移動 1997/01/01
//////////////////////////////////////////////////////////////////////
//フォント関連
extern CBSchFont* g_pFont;
BOOL g_IniReadFontInfo(LPLOGFONT lplf);	//レジストリからフォント情報を得る
BOOL g_IniWriteFontInfo(LPLOGFONT lplf);//レジストリにフォント情報を書き込む
*/
//////////////////////////////////////////////////////////////////////
//座標関連
//void g_MakeRECT(LPPOINT p1,LPPOINT p2,LPRECT prc);//２点の座標から矩形範囲を設定する
//void g_CorrectRECT(LPRECT prc);		//矩形座標の上下左右が入れ替わって要る場合に正しくする
//BOOL g_IsRect2insideRect1(LPRECT prc1,LPRECT prc2);
//矩形2が矩形1の内側（または同じ）にあるかどうかを確かめる


//////////////////////////////////////////////////////////////////////
//クリップボード関係

//UINT g_GetClipboardFormat();		//クリップボードのフォーマットの値を得る
//UINT g_RegisterClipboardFormat();	//クリップボードのフォーマットの登録をする
