/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// CmntAtrb.h : �w�b�_�[ �t�@�C��
//

#ifndef CMNTATRB_070318_H
#define CMNTATRB_070318_H

#include "EditML.h"

/////////////////////////////////////////////////////////////////////////////
// CCmntAtrb �_�C�A���O

class CCmntAtrb : public CDialog
{
// �R���X�g���N�V����
public:
	CCmntAtrb(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CCmntAtrb)
	enum { IDD = IDD_ATRIB_COMMENT };
	CEditML m_editml;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CCmntAtrb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

public:
	CString	m_rText;
	LOGFONT m_logfont;
	bool m_enableWidthSetting;
	int m_width;
	bool m_enableTag;

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CCmntAtrb)
	afx_msg void OnButtonFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEditComment();
	afx_msg void OnBnClickedCheckEnableWidth();
	virtual BOOL OnInitDialog();
protected:

	CSize m_sizeMin;
	CSize m_sizePrev;

	bool initFlag;

	void OffsetCtrl(int dx, int dy, int id);

	virtual void OnOK();
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnCancel();
};

#endif
