#pragma once


// CSetTagFrame �_�C�A���O

class CSetTagFrame : public CDialog
{
	DECLARE_DYNAMIC(CSetTagFrame)

public:
	CSetTagFrame(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSetTagFrame();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TAG_FRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int m_nType;
};
