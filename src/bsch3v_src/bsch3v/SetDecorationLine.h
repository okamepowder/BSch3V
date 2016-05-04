#pragma once


// CSetDecorationLine ダイアログ

class CSetDecorationLine : public CDialog
{
	DECLARE_DYNAMIC(CSetDecorationLine)

public:
	CSetDecorationLine(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSetDecorationLine();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SETDECOLINE };

protected:
	CImageList m_imagelistLineStyle;
	CImageList m_imagelistEndpointStyle;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	int addItem(CComboBoxEx* pCbox,int index,int imageIndex);

public:
	bool m_bCurve;
	int m_lineStyle;
	int m_startPointStyle;
	int m_endPointStyle;
	int m_lineWidth;
	int m_endMarkSize;

protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCancel();
};
