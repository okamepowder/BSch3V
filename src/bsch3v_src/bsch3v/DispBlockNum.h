#pragma once

#include "bsch.h"

// CDispBlockNum �_�C�A���O

class CDispBlockNum : public CDialog
{
	DECLARE_DYNAMIC(CDispBlockNum)

public:
	CDispBlockNum(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CDispBlockNum();

public:
	DISP_BLOCKNUM_TYPE m_dispBN;



// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_DISPBLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};
