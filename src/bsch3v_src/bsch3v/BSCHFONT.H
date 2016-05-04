/****************************************************************************
     BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

///////////////////////////////////////////////////////////////////
// BSchのフォントクラス
// ＣＥのフォントと同一のものを使う
// さしあたり Windows のフォントを使わない理由
//      ・小さいドット数での認識のしやすさ
//      ・上線のサポート
// このクラスはBSchの起動時に１つだけ作成して、終了時に破棄する。
// 実際には、グローバル変数で確保する

#ifndef BSCHFONT_H
#define BSCHFONT_H



class CBSchFont
{
private:
//	static unsigned char Font88h[95*8];
//	unsigned char* pFont88v;
	//int 			m_nBuffXByte;	//バッファの横サイズをバイト数で表現したもの
	//int				m_nBuffXDot;	//バッファの横サイズをドット数で表現したもの
	//int 			m_nBuffYDot;	//バッファの縦サイズをドット数で表現したもの
	//char*			m_pString;		//文字列へのポインタ
	int				m_nStrLen;		//文字列の長さ
	//unsigned char* 	m_pBmpBit;		//展開用バッファへのポインタ。コンストラクタで4096バイト確保
	LOGFONT         m_logfont;	
	CFont*			m_pFontH;
	CFont*			m_pFontV;
	int				m_VExt;
	int				m_WExt;
	int				m_vOffset;

	
private:
//	void Init88Font();			//垂直用のフォントの作成
//	void InitWinFont();			//メンバ変数のLOGFONTの設定
//	void SetToBuffH();			//バッファへのフォントの展開
//	void SetToBuffV();			//バッファへのフォントの展開
	

public:
	CBSchFont();		//コンストラクタ
	~CBSchFont();		//デストラクタ。

	LOGFONT logfont();
	void setLogFont(LOGFONT lf);

	CSize PutFont(		//ラベルフォントの描画
		CDC* pDC,				//出力先のデバイスコンテキスト
		int x,int y,			//出力位置
		LPCTSTR lpstr,			//出力文字列
		int nStrLen=-1,			//文字列長
		COLORREF col=RGB(255,255,255),	//出力色
		UINT nMode=DRAW_ON,		//描画モード
		BOOL bVert=FALSE,		//垂直フラグ
		int  nPointPos=0,		//指定した出力位置が 0:文字列の前 1:文字列の後ろ 2:文字列の中央
		int  nVExt=1,			//表示倍率(View)
		int  nWExt=1,			//表示倍率(Window)
		BOOL bUseUpperLine=TRUE);	//上線の使用

	static CSize PutWinFont(	//Windowsフォント文字列の描画
		CDC* pDC,				//出力先のデバイスコンテキスト
		int x,int y,			//出力位置
		LPCTSTR lpstr,			//出力文字列
		int nStrLen,			//文字列長
		LOGFONT* plf,
		COLORREF col,			//出力色
		UINT nMode,				//描画モード
		BOOL bVert,
		int  nPointPos,			//指定した出力位置が 0:文字列の前 1:文字列の後ろ 2:文字列の中央
								//					 0:文字列の下 4:文字列の上   8:文字列の中
		int  nVExt,				//表示倍率(View)
		int  nWExt);			//表示倍率(Window)

	static CSize PutWinFontML(	//コメント用のWindowsフォント文字列の描画
		CDC* pDC,				//出力先のデバイスコンテキスト
		int x,int y,			//出力位置
		LPCTSTR lpstr,			//出力文字列
		int nStrLen,			//文字列長
		LOGFONT* plf,
		int width,
		CSize& size1Line,
		COLORREF col,			//出力色
		UINT nMode,				//描画モード
		int  nVExt,				//表示倍率(View)
		int  nWExt);			//表示倍率(Window)


	static BOOL IniReadFontInfo(const TCHAR* sectionName, LPLOGFONT lplf,const TCHAR* defFont,int defHeight);
	static BOOL IniWriteFontInfo(const TCHAR* sectionName, LPLOGFONT lplf);

};

//////////////////////////////////////////////////////////////////////
//フォント関連 グローバル変数・関数		Global.hから移動
extern CBSchFont* g_pFont;		//実体は BSchFont.cpp
extern CBSchFont* g_pFontName;	//実体は BSchFont.cpp


BOOL g_IniReadFontInfo(LPLOGFONT lplf);	//レジストリからコメントのデフォルトのフォント情報を得る
BOOL g_IniWriteFontInfo(LPLOGFONT lplf);//レジストリにコメントのデフォルトのフォント情報を書き込む

BOOL g_IniReadLabelFontInfo(LPLOGFONT lplf);//レジストリからラベルのフォント情報を得る
BOOL g_IniWriteLabelFontInfo(LPLOGFONT lplf);//レジストリにラベルのフォント情報を書き込む

BOOL g_IniReadNameFontInfo(LPLOGFONT lplf);//レジストリから部品名番号のフォント情報を得る
BOOL g_IniWriteNameFontInfo(LPLOGFONT lplf);//レジストリに部品名番号のフォント情報を書き込む

BOOL g_IniReadQuickPropFontInfo(LPLOGFONT lplf);//レジストリからアトリビュートプレビューのフォント情報を得る
BOOL g_IniWriteQuickPropFontInfo(LPLOGFONT lplf);//レジストリにアトリビュートプレビューのフォント情報を書き込む

//#ifdef _DEBUG
//void TEST_BSchFont(CDC* pDC,int nVExt,int nWExt);
//#endif


#endif