#pragma once


// CAtribImage �_�C�A���O

class CAtribImage : public CDialog
{
	DECLARE_DYNAMIC(CAtribImage)

public:
	CAtribImage(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CAtribImage();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_IMAGE_ATRB };

	int m_magnification;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
