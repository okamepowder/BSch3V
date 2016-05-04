#pragma once


// CLinePropertyDlg ダイアログ

class CLinePropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinePropertyDlg)

public:
	//BOOL m_dash;
	int m_width;
	//BOOL m_curveline;

public:
	CLinePropertyDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CLinePropertyDlg();

// ダイアログ データ
	enum { IDD = IDD_LINE_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	BOOL m_curveline;
	BOOL m_dash;
};
