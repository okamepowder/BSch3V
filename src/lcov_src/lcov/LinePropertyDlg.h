#pragma once


// CLinePropertyDlg �_�C�A���O

class CLinePropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinePropertyDlg)

public:
	//BOOL m_dash;
	int m_width;
	//BOOL m_curveline;

public:
	CLinePropertyDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CLinePropertyDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_LINE_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	BOOL m_curveline;
	BOOL m_dash;
};
