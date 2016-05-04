/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#if !defined(AFX_SETFSIZE_H__A7B3A981_814D_11D2_9287_0040053965F5__INCLUDED_)
#define AFX_SETFSIZE_H__A7B3A981_814D_11D2_9287_0040053965F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SetFSize.h : �w�b�_�[ �t�@�C��
//

#define MAX_STDSIZE 10

/////////////////////////////////////////////////////////////////////////////
// CSetFreeSize �_�C�A���O

class CSetFreeSize : public CDialog
{

	enum SIZEMAXMIN{
		XSIZE_MIN = SHEETSIZE_X_MIN,
		XSIZE_MAX = SHEETSIZE_X_MAX,
		YSIZE_MIN = SHEETSIZE_Y_MIN,
		YSIZE_MAX = SHEETSIZE_Y_MAX
	};


// �R���X�g���N�V����
public:
	CSetFreeSize(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	int m_nX;
	int m_nY;
	

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSetFreeSize)
	enum { IDD = IDD_SETSHEETSIZE };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CSetFreeSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSetFreeSize)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnSelchangeMystdsheetsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL GetXY(int* x,int* y);
	BOOL SetXY(int x,int y);
	BOOL SizeStr2Size(LPCTSTR strSize,int* x,int* y);
	void SaveMyStdSheetInfo();
	void SetAddDelButtonState();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETFSIZE_H__A7B3A981_814D_11D2_9287_0040053965F5__INCLUDED_)
