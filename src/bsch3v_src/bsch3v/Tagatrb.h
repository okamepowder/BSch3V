/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// TagAtrb.h : �w�b�_�[ �t�@�C��
//

#ifndef TAGATRB_H
#define TAGATRB_H

/////////////////////////////////////////////////////////////////////////////
// CTagAtrb �_�C�A���O

class CTagAtrb : public CDialog
{
// �R���X�g���N�V����
public:
	CTagAtrb(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTagAtrb)
	enum { IDD = IDD_ATRIB_TAG };
	CString	m_rName;
	int		m_nType;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CTagAtrb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTagAtrb)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif