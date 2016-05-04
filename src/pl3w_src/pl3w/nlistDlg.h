/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


// nlistDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_NLISTDLG_H__B8E59C02_9A8D_4DE7_A228_7DB454FAE122__INCLUDED_)
#define AFX_NLISTDLG_H__B8E59C02_9A8D_4DE7_A228_7DB454FAE122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SrcLB.h"


/////////////////////////////////////////////////////////////////////////////
// CNlistDlg �_�C�A���O

#define FILE_NAME_BUFFER_SIZE (_MAX_PATH * 64  + 1)

class CNlistDlg : public CDialog
{
// �\�z
public:
	CNlistDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^
protected:
	TCHAR m_szFileNameBuffer[FILE_NAME_BUFFER_SIZE];

	BOOL m_noMessageBox;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CNlistDlg)
	enum { IDD = IDD_NLIST_DIALOG };
	CSrcLB	m_listsrc;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CNlistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	void SetDstFileName();

	int DstFileSetting();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CNlistDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRefsrc();
	afx_msg void OnClr();
	afx_msg void OnSelchangeListSrc();
	afx_msg void OnRefdst();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckMfr();
//	afx_msg void OnCbnSelchangeComboDstType();
	afx_msg void OnBnClickedRadioCsvout();
	afx_msg void OnBnClickedRadioSylkout();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_NLISTDLG_H__B8E59C02_9A8D_4DE7_A228_7DB454FAE122__INCLUDED_)
