#pragma once


// CPtnTextDlg �_�C�A���O

class CPtnTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CPtnTextDlg)

public:
	CPtnTextDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPtnTextDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PTNTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
