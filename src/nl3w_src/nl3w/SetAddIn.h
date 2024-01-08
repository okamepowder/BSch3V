#pragma once


// CSetAddIn �_�C�A���O

class CSetAddIn : public CDialog
{
	DECLARE_DYNAMIC(CSetAddIn)

public:
	CSetAddIn(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSetAddIn();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SETADDIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	void SetListboxWidth();

	//�_�C�A���O�\���{�^���̃C�l�[�u��
	void EnableButton(UINT id,BOOL bEnable);

	int m_nAddinCount;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnLbnSelchangeListFiles();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
