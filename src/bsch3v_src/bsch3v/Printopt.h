/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef _PRINTOPT_H
#define _PRINTOPT_H
#include "afxwin.h"

// PrintOpt.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPrintOpt �_�C�A���O

class CPrintOpt : public CDialog
{
// �R���X�g���N�V����
public:
	CPrintOpt(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPrintOpt)
	enum { IDD = IDD_SET_PRINT_OPTION };
	BOOL	m_bFrame;
	BOOL	m_bPartNum;
	BOOL	m_bPartName;
	BOOL	m_bPrintBmp;
	BOOL	m_bFooter;
	CString m_strFooter;
	//}}AFX_DATA

	int m_nPrintRes;
	int m_nImageMag;		//2016/05/03 �C���[�W�t�@�C���̊g��w��ǉ�
	int m_nLeftMargin;
	int m_nRightMargin;
	int m_nUpperMargin;
	int m_nLowerMargin;

	//int m_nDefaultRes;

	BOOL m_bPageFit;



// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CPrintOpt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPrintOpt)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPrintColor;
//	afx_msg void OnBnClickedCheckPrintColor();
	afx_msg void OnBnClickedCheckPrintbmp();
	afx_msg void OnBnClickedCheckPagefit();
	afx_msg void OnBnClickedCheckFooter();
};

#endif

