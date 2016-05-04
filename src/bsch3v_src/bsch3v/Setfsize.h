/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#if !defined(AFX_SETFSIZE_H__A7B3A981_814D_11D2_9287_0040053965F5__INCLUDED_)
#define AFX_SETFSIZE_H__A7B3A981_814D_11D2_9287_0040053965F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SetFSize.h : ヘッダー ファイル
//

#define MAX_STDSIZE 10

/////////////////////////////////////////////////////////////////////////////
// CSetFreeSize ダイアログ

class CSetFreeSize : public CDialog
{

	enum SIZEMAXMIN{
		XSIZE_MIN = SHEETSIZE_X_MIN,
		XSIZE_MAX = SHEETSIZE_X_MAX,
		YSIZE_MIN = SHEETSIZE_Y_MIN,
		YSIZE_MAX = SHEETSIZE_Y_MAX
	};


// コンストラクション
public:
	CSetFreeSize(CWnd* pParent = NULL);   // 標準のコンストラクタ

	int m_nX;
	int m_nY;
	

// ダイアログ データ
	//{{AFX_DATA(CSetFreeSize)
	enum { IDD = IDD_SETSHEETSIZE };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CSetFreeSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSetFreeSize)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnSelchangeMystdsheetsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL GetXY(int* x,int* y);
	BOOL SetXY(int x,int y);
	BOOL SizeStr2Size(LPCTSTR strSize,int* x,int* y);
	void SaveMyStdSheetInfo();
	void SetAddDelButtonState();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETFSIZE_H__A7B3A981_814D_11D2_9287_0040053965F5__INCLUDED_)
