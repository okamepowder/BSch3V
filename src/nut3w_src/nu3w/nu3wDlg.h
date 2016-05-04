/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// nu3wDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "ddlb.h"

//#define PARTNUM_INIT_MAX (1000)		//���i�ԍ������l�̍ő�l
//#define PARTNUM_STEP_MAX (20)		//���i�ԍ��X�e�b�v�l�̍ő�l

// Cnu3wDlg �_�C�A���O
class Cnu3wDlg : public CDialog
{
// �R���X�g���N�V����
public:
	Cnu3wDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_NU3W_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;
	BOOL m_noMessageBox;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CDdLb m_lbSrc;
protected:
	virtual void OnOK();
	void setButtonEnable();  //�\�[�X�̃��X�g�{�b�N�X�̏�Ԃ�OK�{�^���̃C�l�[�u����Ԃ�ݒ�

	void setPrefixFilterControlEnable();

public:
//	afx_msg void OnBnClickedRef();
	afx_msg void OnBnClickedClr();
	afx_msg void OnBnClickedRef();
	afx_msg void OnLbnSelchangeListSrc();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedCheckEnablePrefixfilter();

	afx_msg void OnBnClickedNewlyonly();
	afx_msg void OnClickedEnLayer();
};
