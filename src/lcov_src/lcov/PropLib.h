/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once


// CPropLib �_�C�A���O

class CPropLib : public CDialog
{
	DECLARE_DYNAMIC(CPropLib)

public:
	CPropLib(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPropLib();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_PROP_LIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString m_propString;
};
