#pragma once


// CBulkHidingNumVal �_�C�A���O

class CBulkHidingNumVal : public CDialog
{
	DECLARE_DYNAMIC(CBulkHidingNumVal)

public:
	CBulkHidingNumVal(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CBulkHidingNumVal();

	int m_bHideNumCheckState;
	int m_bHideValCheckState;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_HIDING_NUM_VAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedCheckHidenum();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
