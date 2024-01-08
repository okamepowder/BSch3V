#pragma once


// CMarkerLineAtrb ダイアログ

class CMarkerLineAtrb : public CDialog
{
	DECLARE_DYNAMIC(CMarkerLineAtrb)

public:
	CMarkerLineAtrb(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMarkerLineAtrb();

	int m_nWidth;
	unsigned int m_nColor;

	CBrush m_brush;

	COLORREF m_arrayColor[16];



// ダイアログ データ
	enum { IDD = IDD_ATRIB_MARKER_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonSetcolor();
};

