/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


#if !defined(AFX_SRCLB_H__CE0BE66B_125E_4EC2_90E4_A056F69D5BF8__INCLUDED_)
#define AFX_SRCLB_H__CE0BE66B_125E_4EC2_90E4_A056F69D5BF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrcLB.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CSrcLB �E�B���h�E

class CSrcLB : public CListBox
{
// �R���X�g���N�V����
public:
	CSrcLB();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	void SetListboxWidth();
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSrcLB)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CSrcLB();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSrcLB)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SRCLB_H__CE0BE66B_125E_4EC2_90E4_A056F69D5BF8__INCLUDED_)
