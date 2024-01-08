#pragma once

#include "PinPropGrid.h"

class CCompEditView;

// CPinProps �_�C�A���O

class CPinProps : public CDialog
{
	DECLARE_DYNAMIC(CPinProps)

public:
	CPinProps(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPinProps();
	//BOOL Create(CWnd* pParent = NULL);
	BOOL Create();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PINPROPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	CCompEditView* m_pParent;

	CPinPropGrid* m_pGrid;
	CPinPropGridTitle* m_pGridTitle;

	void PreReleaseTemp();	//�I���I�u�W�F�N�g���������O�ɌĂ΂��B
	void update();
	void initBlock();
	int activeBlock(){
		if(m_pGrid!=NULL){
			return m_pGrid->activeBlock();
		}
		return 0;
	}

	void setVScroll();
	void setHScroll();

protected:


public:
	virtual BOOL OnInitDialog();
	void SetCtrlDim();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnClose();
};
