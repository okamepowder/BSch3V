/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Preview.h : �w�b�_�[ �t�@�C��
//
#ifndef PREVIEW_H
#define PREVIEW_H

/////////////////////////////////////////////////////////////////////////////
// CPreview �E�B���h�E

class CPreview : public CWnd
{
// �R���X�g���N�V����
public:
	CPreview();

// �A�g���r���[�g
private:
	const SCompIndex* m_pPartIndex;
	CSize		m_sizePart;

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CPreview)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CPreview();

	void SetPartIndex(const SCompIndex* pPartIndex);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CPreview)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif