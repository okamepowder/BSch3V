/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// PartAtrb.h : �w�b�_�[ �t�@�C��
//
#ifndef PARTATRB_H
#define PARTATRB_H

#include "complib.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPartAtrb �_�C�A���O

class CPartAtrb : public CDialog
{
// �R���X�g���N�V����
public:
	CPartAtrb(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPartAtrb)
	enum { IDD = IDD_ATRIB_PART };
	CString	m_rPartName;
	CString	m_rPartNum;
	CString m_rNote;
	//}}AFX_DATA
	int		m_nBlock;
	int		m_nBlockMin;       //�R���X�g���N�^�łP�ɏ�����
	int		m_nBlockMax;
	CString m_rOrgName;
	const SCompIndex* m_pPartIndex;
	int m_nLastSelectPart;
	int m_nLastSelectLibrary;
	BOOL m_modifiedCompInfo;	   //�R���X�g���N�^��FALSE�ɏ�����

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CPartAtrb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPartAtrb)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditPartblock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//IDC_PIN_NUM �X�^�e�B�b�N�R���g���[���Ƀs���ԍ����Z�b�g
	void SetBlockPins(int nBlock);

	TCHAR m_szLcoPath[_MAX_PATH];
	BOOL m_enableEditComp;
	//SCompIndex* m_pTempPartIndex;
	SCompLib m_tempLib;		//LCoV�ŕҏW����Ƃ��Ɏg���B���̃_�C�A���O�N���X�����ł���܂Ŏ��̂�����Ȃ��悤�ɃN���X�ϐ��Ƃ��Đ錾�����B

	UINT m_timerID;

public:
	afx_msg void OnBnClickedReplaceCompindex();
	afx_msg void OnBnClickedEditCompindex();

	BOOL m_bNameVertical;
	BOOL m_bNumVertical;
	
	CString m_mfr;
	CString m_mfrpnum;
	CString m_package;

	int m_nFocusedTo;

	

	BOOL m_bHideNum;
	BOOL m_bHideVal;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#endif