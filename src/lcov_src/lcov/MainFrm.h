/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "LCoVDoc.h"
//#include "ChildView.h"
#include "CompEditView.h"
//#include "CompListDlgBar.h"

#include "lbcompname.h"


class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CDialogBar  m_wndDlgBar;
	//SCompListDlgBar m_wndDlgBar;
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CCompEditView    m_wndView;
	SLBCompName m_LBCompName;
	SCompInfo*	m_OldSelectedCompInfo;




// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnButtonUp();
	afx_msg void OnUpdateButtonUp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonDown(CCmdUI *pCmdUI);
	afx_msg void OnButtonDown();
	afx_msg void OnSelchangeNameList();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	SLCoVDoc m_LCoVDoc;
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

public:
	BOOL DoOpen(LPCTSTR pszFileName = NULL);

protected:
	BOOL DoSaveChanged();
	
	void setCompIndexToListBox();			//���i�C���f�b�N�X�����X�g�{�b�N�X�ɐݒ�
	void setEnableUpDownButton();
	BOOL DoOnFileSave();
	BOOL DoSave(LPCTSTR filename);			//�t�@�C���ۑ��̎���
	BOOL DoSaveAs(LPCTSTR defaultFilename);	//���O��t���ăt�@�C���ۑ��̎���
	bool canPaste();						//�y�[�X�g�ł����Ԃ��ǂ����̔���
	void doCopyComp();
	void doDeleteComp();

	void setAppCaption(LPCTSTR filename);

	void sortComponent();
	static int sortComponentCompare(SCompIndex** p1,SCompIndex ** p2);

	
public:
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnEditCopyComp();
	afx_msg void OnUpdateEditCopyComp(CCmdUI *pCmdUI);
	afx_msg void OnEditCutComp();
	afx_msg void OnUpdateEditCutComp(CCmdUI *pCmdUI);
	afx_msg void OnEditPasteComp();
	afx_msg void OnUpdateEditPasteComp(CCmdUI *pCmdUI);
//	afx_msg void OnViewToolbar();
	afx_msg void OnToolSelect();
	afx_msg void OnUpdateToolSelect(CCmdUI *pCmdUI);
	afx_msg void OnToolPin();
	afx_msg void OnUpdateToolPin(CCmdUI *pCmdUI);
	afx_msg void OnToolLine();
	afx_msg void OnUpdateToolLine(CCmdUI *pCmdUI);
	afx_msg void OnToolCircle();
	afx_msg void OnUpdateToolCircle(CCmdUI *pCmdUI);
	afx_msg void OnToolPolygon();
	afx_msg void OnUpdateToolPolygon(CCmdUI *pCmdUI);
	afx_msg void OnToolArc();
	afx_msg void OnUpdateToolArc(CCmdUI *pCmdUI);
	afx_msg void OnEditProperty();
	afx_msg void OnUpdateEditProperty(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditNewComp();
	afx_msg void OnEditPropertyComp();
	afx_msg void OnUpdateEditPropertyComp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditMirror(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRotate(CCmdUI *pCmdUI);
	afx_msg void OnEditRotate();
	afx_msg void OnEditMirror();
	afx_msg void OnEditNodeMode();
	afx_msg void OnUpdateEditNodeMode(CCmdUI *pCmdUI);
	afx_msg void OnToolCreateBptn();
	afx_msg void OnUpdateToolCreateBptn(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnEditPropLib();
	afx_msg void OnEditAddAlias();
	afx_msg void OnUpdateEditAddAlias(CCmdUI *pCmdUI);
	afx_msg void OnEditSortComponent();
	afx_msg void OnUpdateEditSortComponent(CCmdUI *pCmdUI);
	afx_msg void OnToolText();
	afx_msg void OnUpdateToolText(CCmdUI *pCmdUI);
	afx_msg void OnEditNextPinName();
	afx_msg void OnUpdateEditNextPinName(CCmdUI *pCmdUI);
	afx_msg void OnEditNextPinNum();
	afx_msg void OnUpdateNextPinNum(CCmdUI *pCmdUI);


//>>>�t���[���E�B���h�E�T�C�Y�̕��� BSch3V����ڐA 2008/02/09
protected:
	static CSize m_sizeInitialWindowSize;

public:
	static void SetInitialWindowSize(int cx,int cy);
	static void IniWriteWindowPosition(int cmdShow,int cx,int cy);
	static void IniReadWindowPosition(int& cmdShow,int& cx,int& cy);
//<<<�t���[���E�B���h�E�T�C�Y�̕��� BSch3V����ڐA 2008/02/09

};


