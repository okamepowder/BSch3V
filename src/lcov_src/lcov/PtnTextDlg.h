#pragma once


// CPtnTextDlg ダイアログ

class CPtnTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CPtnTextDlg)

public:
	CPtnTextDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPtnTextDlg();

// ダイアログ データ
	enum { IDD = IDD_PTNTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	int m_align;
	LOGFONT m_logfont;



	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCheckComfong();
	afx_msg void OnBnClickedButtonFont();
};
