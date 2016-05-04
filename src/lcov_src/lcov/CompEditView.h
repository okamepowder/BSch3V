/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// CompEditView.h : CCompEditView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "CompEditDoc.h"
#include "PinProps.h"




// SCompEditView �E�B���h�E

class CCompEditView : public CWnd
{
	enum{
		PAGE_MARGIN = 50,
		MIN_ZOOMNUM = 0,
		MAX_ZOOMNUM = 3
	};

	enum{
		DRAGOP_NOT	= 0,
		DRAGOP_PTN  = 1,
		DRAGOP_PIN	= 2
	};

public:
	enum{
		TOOL_SELECT = 1,
		TOOL_PIN	= 2,
		TOOL_LINE	= 3,
		TOOL_CIRCLE	= 4,
		TOOL_POLYGON = 5,
		TOOL_ARC	= 6,
		TOOL_TEXT	= 7
	};

// �R���X�g���N�V����
public:
	CCompEditView();

// ����
public:
	SCompEditDoc m_doc;
	SCompIndex* m_pCompIndex;


protected:
	int m_zoomNum;
	int m_magV;			//�\���{���̕��q
	int m_magW;			//�\���{���̕���
	int m_block;

	CSize m_pageSize;	//���݂̕\���{���ł̃y�[�W�T�C�Y
	CSize m_clientSize;
	CPoint m_windowOrg;

	int m_pageMargin;

	int m_tool;
	int m_placeState;
	SPoint m_arrayTempPoint[64];

	int m_dragOpStatus;		//�I�u�W�F�N�g�̃h���b�O��;

	bool m_enableContextMenu;
	//bool m_enableRButtonDrag;
	bool m_startSelect;

	unsigned m_captureFlag;
	enum MOUSE_CAPTURE_FLAG
	{
		LBUTTON_CAPTURE = 1,
		RBUTTON_CAPTURE = 2
	};


	CPoint m_pointRButtonDownDevice;
	CPoint m_pointRButtonDown;

	CPoint m_pointLButtonDown;
	CPoint m_pointCurrentPos;
	CPoint m_pointLButtonUp;
	CSize m_prevOffset;		//�h���b�O���ɂ��łɈړ����m�肵����
	int m_pinPosLtrbStart;		//�s���̃h���b�O���͂��߂�O�̈ʒu
	int m_pinPosOffsetStart;	//�s���̃h���b�O���͂��߂�O�̈ʒu


	//�|���S�������Ɋւ���ϐ�
	bool m_bIsCursorOnActivePolygon;				//�����\�Ȉʒu�ɃJ�[�\�������邩
	class SPtnObjPolygon* m_pPolygonToDivide;		//�����\�ł��邱�Ƃ����o�����|���S���̃|�C���^
	int m_nLineNumOfPolygonToDivide;				//�����\�ȃ|���S���̉��Ԗڂ̃��C�������L�^����
	CPoint m_ptDividePolygonAt;						//��������ꏊ���L�^����
	HICON m_hDivideMarkIcon;


	//�I�t�X�N���[���o�b�t�@
	CBitmap* m_pBmpBuff;
	CSize	m_offScrnBuffSize;
	CPoint  m_offScrnBuffOrg;

	//�J�[�\���ɕt���]���I�u�W�F�N�g
	SPin* m_pCursorPin;

	//���p�`�̃m�[�h�ǉ��}�[�N�`���� 2009/07/20
	bool m_bDrawAddNodeIcon;
	CPoint m_ptDrawAddNodeIcon;


	bool m_editNodeMode;

	int m_movingGrip;

	bool m_tempAreaRectValid;

	bool m_drawXcursor;

	CPinProps *m_pPinPropsDlg;
	bool m_displayPinProps;

	bool m_swapPinAndName;	//0.81.01


// ����
public:
	int tool(){return m_tool;}
	void setTool(int tool);

	bool canDelete();
	bool doDelete();

	bool canCreateBitPtn();
	bool doCreateBitPtn();

	bool canCopy();
	bool doCopy();

	bool canUndo();
	bool doUndo();

	bool canMirrorRotate();
	bool doMirror();
	bool doRotate();

	void  setEditNodeMode(bool editnode);
	bool  editNodeMode(){return m_editNodeMode;}

	
	bool canPaste();
	bool doPaste();
	bool doPastePin(SReadCE3& rce3);
	bool doPastePtn(SReadCE3& rce3);

	bool selectPin(int ltrb,int offset);


	bool canProperty();
	bool editProperty();
	bool editPinProperty(SPin* pPin);
	bool editPtnProperty(SPtnObj* pPtn);
	bool save();

	bool canNextPinNumber();
	bool canNextPinName();

	bool nextPinNumber();
	bool nextPinName();



	bool releaseTemp();

	
	void pinModified();


	void setDirty(){m_doc.m_tempDirtyFlag = true;}

	//�w�肵���f�o�C�X���W���A�w�肵���ҏW���W�ɂȂ�悤�ɃX�N���[������B
	void scrollTo(CPoint ptDevice,CPoint ptEdit);


// �I�[�o�[���C�h
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CCompEditView();

protected:
	void PrepareDC(CDC* pDC);
	void drawToBuff(bool force);


	void drawPin(CDC* pDC,SPin* pPin,int block,DWORD dwMode);
	void drawPins(CDC* pDC,SCompEditElmList& list,DWORD dwMode);
	void drawPtns(CDC* pDC,SCompEditElmList& list,DWORD dwMode);
	void drawPtn(CDC* pDC,SPtnObj* pPtn,int cx,int cy,COLORREF col,int magV,int magW);
	void drawCompFrame(CDC* pDC);
	void drawGrid(CDC* pDC);
	void drawGrip(CDC* pDC);

	int doSelect(SPoint sptEdit,CPoint lpt);

	void setZoomValue(int n);
	//���W�ϊ� �_�����W�ƕ��i�ҏW���W�̑��ݕϊ�
	void LPtoCompEditP(CPoint& point);
	void LPtoCompEditP(CRect& rc);
	void CompEditPtoLP(CPoint& point);
	void CompEditPtoLP(CRect& rc);

	//�s���̔z�u�B�����s�����Ȃ����m_pCursorPin��TempData�ɔz�u����B
	bool placePin();

	void drawXCursor(CDC* pDC);
	void drawPlaceLine(CDC* pDC);
	void drawPlaceCircle(CDC* pDC);
	void drawPlaceArc(CDC* pDC);

	bool editNodeActive();

	int isOnGrip(const CPoint& logicalPoint);
	//�J�[�\�����I�����ꂽ�|���S���̐��ォ�ǂ���������
	bool TestIsCursorOnPolygon(const CPoint& editPoint, int limit);//, SPtnObjPolygon** ppPolygon, int& lineNum, CPoint& ptOnPolygon);
	//bool TestIsCursorOnPolygon(const CPoint& logicalPoint, int limit, SPtnObjPolygon** ppPolygon, int& lineNum, CPoint& ptOnPolygon);

	int m_nWHEEL_DELTA_z;
	int m_nWHEEL_DELTA_s;

	int activeBlock();

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void setCompIndex(SCompIndex* pCompIndex);
	void saveCompInfo();
	void setPageMargin(int n, bool updateWindow);
	void setPageSize();
	bool onKeyEscape();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewZoomin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewZoomout(CCmdUI *pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewPinprops();
	afx_msg void OnUpdatePinprops(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAddNode();
	afx_msg void OnUpdateAddNode(CCmdUI *pCmdUI);
	afx_msg void OnViewSwapnameandnum();
	afx_msg void OnUpdateViewSwapnameandnum(CCmdUI *pCmdUI);
};

