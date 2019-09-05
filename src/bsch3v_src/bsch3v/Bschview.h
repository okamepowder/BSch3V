/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSchView.h : CBSchView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BSCHVIEW_H
#define BSCHVIEW_H

#include <atlimage.h>


#define AUTOSCROLL_FRAME	2	//�N���C�A���g�G���A�̓���(AUTOSCROLL_FRAME)�h�b�g
                                //���O���Ȃ玩���X�N���[������
//�ȉ��̂S�̓v���C�x�[�g�֐���QueryAutoScroll()���Ԃ��l
#define AUTOSCROLL_LEFT		1
#define AUTOSCROLL_TOP		2
#define AUTOSCROLL_RIGHT	4
#define AUTOSCROLL_BOTTOM	8

// DrawListData()�� ����dwOption�Ŏg���l


//#define EXCEPT_DRAG_LINE	512
//#define DRAG_LINE_ONLY		1024
#define DRAW_ALL_LAYER		2048
#define DRAW_TEMP_DATA		1024

#define MAX_ZOOM_STEP_NUM 8


#include "BSchDrawColor.h"

class CBSchDoc;

class CQuickProp;

class CDlgObjFilter;

class CFind2;

class CDispBlockNum;

class FindInfo
{
public:
	CString m_str;
	BOOL	m_bMatchWhole;
	BOOL	m_bCaseSensitive;
	unsigned int m_targetFlag;
};

class QuickPropSetting
{
public:
	bool m_enable;
	bool m_showOnMouseHover;
	bool m_num;
	bool m_val;
	bool m_mfr;
	bool m_mfrpn;
	bool m_package;
	bool m_note;
	bool m_nameinlib;
	QuickPropSetting(){
		m_num = false;
		m_val = false;
		m_enable = false;
		m_showOnMouseHover = false;
		m_mfr = false;
		m_mfrpn = false;
		m_package = false;
		m_note = false;
		m_nameinlib = false;
	};
	bool isEnabled(){
		return m_enable && (m_num || m_val || m_mfr || m_mfrpn || m_package || m_note || m_nameinlib);
	}

};

//���E�{�^���ɂ��}�E�X�̃L���v�`����Ԃ��Ǘ�����N���X
class MouseCaptureInfo
{
public:
	enum CAPTUREBUTTON
	{
		MCI_LEFT = 1,
		MCI_RIGHT = 2
	};
protected:
	CWnd* m_pWnd;
	unsigned m_captureValue;
public:
	MouseCaptureInfo(){
		m_captureValue = 0;
		m_pWnd = NULL;
	}
	void Init(CWnd* pWnd){
		m_captureValue = 0;
		m_pWnd = pWnd;
	}
	unsigned SetCapture(CAPTUREBUTTON button);
	unsigned ReleaseCapture(CAPTUREBUTTON button);
	bool isLButtonCapture(){return (m_captureValue & (unsigned)MCI_LEFT);}
	bool isRButtonCapture(){return (m_captureValue & (unsigned)MCI_RIGHT);}
};



class CBSchView : public CScrollView
{
public:
	static SBSchDrawColor m_COL;

	//static COLORREF m_COL_BG;
	//static COLORREF m_COL_JUNC;
	//static COLORREF m_COL_WIRE;
	//static COLORREF m_COL_BUS;
	//static COLORREF m_COL_DASH;
	//static COLORREF m_COL_TAG;
	//static COLORREF m_COL_TAG_NAME;
	//static COLORREF m_COL_COMMENT;
	//static COLORREF m_COL_LABEL;
	//static COLORREF m_COL_XOR;
	//static COLORREF m_COL_PART;
	//static COLORREF m_COL_PART_PIN;
	//static COLORREF m_COL_PART_PINNAME;
	//static COLORREF m_COL_PART_PINNUM;
	//static COLORREF m_COL_PART_NAME;
	//static COLORREF m_COL_PART_NUM;
	//static COLORREF m_COL_PART_FRAME;
	//static COLORREF m_COL_SHEET_FRAME;
	//static COLORREF m_COL_SHEET_GRID;
	//static COLORREF m_COL_DARK;
	//static COLORREF m_COL_ZLPIN;
	//static COLORREF m_COL_NC_MARK;
	//static COLORREF m_COL_QUICKPROPVIEW;
	//static COLORREF m_COL_TEMPFRAME_FOCUSED;
	//static COLORREF m_COL_TEMPFRAME_NOT_FOCUSED;


	static void IniReadColor();
	static void IniWriteColor();

public:



protected:
	void IniReadMarkerLineInitValue();
	void IniWriteMarkerLineInitValue();
	void IniReadMarkerLineCustomColors(COLORREF* lpColors);
	void IniWriteMarkerLineCustomColors(COLORREF* lpColors);


//�N�C�b�N�v���p�e�B
protected:
	DISP_BLOCKNUM_TYPE m_dispBN;
	bool m_displayQuickPropComp;
	CQuickProp *m_pQuickPropComp;
	void ShowQuickPropComp(const SXBSchComponent* pObj);
	void HideQuickPropComp();
	CPoint m_ptDisplayQuickPropAt;
	QuickPropSetting m_quickPropSetting;
	void IniReadQuickPropSetting();
	void IniWriteQuickPropSetting();
	LOGFONT m_lfQuickPropFont;


//�ǉ��I��
protected:
	bool m_allowAppendSelect;
	BOOL IniReadAllowAppendSelect();
	void IniWriteAllowAppendSelect();




protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CBSchView();
	DECLARE_DYNCREATE(CBSchView)

// �A�g���r���[�g
public:
	CBSchDoc* GetDocument();

	void IniReadFindInfo(FindInfo& fi);
	void IniWriteFindInfo(FindInfo& fi);

//////////////////////////////////////////////////////////////////
// �v���C�x�[�g�ϐ�
private:

	////////////////////////////////////////////////////
	//���W�֘A
	CPoint m_ptMouseDP;		//�f�o�C�X���W�ŕ\���}�E�X�̍��W(OnMouseMove(),OnLButtonDown(),OnLButtonUp()�ŕύX)
	CPoint m_ptMouse;		//�}�ʍ��W�ŕ\���}�E�X�̍��W(OnMouseMove(),OnLButtonDown(),OnLButtonUp()�ŕύX)
	CPoint m_ptMoveFrom;	//�L���v�`�����n�߂����W
	CPoint m_ptMoveTo;		//�O��̃}�E�X�̐}�ʍ��W
	CRect m_rcClip;			//�}�ʍ��W�ŕ\�����N���b�s���O�̈�i�}�ʃI�u�W�F�N�g�̕`��̑O�ɕK���ݒ�)
							//OnPrepareDC()�̒��Őݒ肵�Ă���
	//CSize m_sizeClientArea;	//�N���C�A���g�G���A�̃T�C�Y







	
	////////////////////////////////////////////////////
	//���i�̑I���̏�ԂɊւ�����
	int m_nLastSelectLibrary;	//�Ō�ɑI�񂾃��C�u�����̃��X�g�{�b�N�X�ł̏����ԍ�
	int m_nLastSelectPart;		//�Ō�ɑI�񂾕��i�̃��X�g�{�b�N�X�ł̏����ԍ�



	////////////////////////////////////////////////////
	// �I�����̃t�B���^�[
	unsigned m_ObjectFileterValue;
	CDlgObjFilter* m_pObjFilterDlg;

	////////////////////////////////////////////////////
	// ����
	//�����_�C�A���O
	CFind2* m_pFind2Dlg;
	//�����֌W�ϐ�
	bool m_FindMarkActive;
	CPoint m_ptFindMark;
	
	bool isFindMarkActive(); 



	//BOOL m_bUsePreview;

	BOOL m_fEditHighLight;
	////////////////////////////////////////////////////
	//�c�[���I���E����E�\���̏�ԂɊւ�����
	int m_nCurrentTool;		//���ݑI�����Ă���c�[��

	MouseCaptureInfo m_mouseCaptureInfo;

//20100521		BOOL m_bCaptureMouse;	//�}�E�X���L���v�`�����Ă�����TRUE
//20100521		BOOL m_bRBCaptureMouse;	//�E�{�^������Ń}�E�X���L���v�`�����Ă�����TRUE


	BOOL m_bOnSelect;		//�͈͑I�𒆂Ȃ�TRUE
	UINT m_nOnMove;			//��O�̂Ƃ��A�������ړ����B
							//�l�� CBSchDoc::IsOnTmpData()�̕Ԓl�����
							// ONTMPDATA      (1)	�w�肵���_�͈ꎞ�f�[�^�̏�
							// ONTMPDATA_NAME (2)	�w�肵���_�͕��i���̏�
							// ONTMPDATA_NUM  (4)	�w�肵���_�͕��i�ԍ��̏�
	
	BOOL m_bSetWireDirection;	//���ꂪTRUE�̂Ƃ��̓��C���[�ʒu�̂P�{�ڂ̕������Z�b�g�ς�
	BOOL m_bWireHorizon1st;		//���ꂪTRUE�̂Ƃ��̓��C���[�ʒu�̂P�{�ڂ�����

	CPoint m_pointOccasional;	//�Վ��f�[�^�̍��W
	//m_bDrawXXXXXX��XOR�ŕ`�悷��I�u�W�F�N�g�̕`��t���O
	//���ꂪTURE�̂Ƃ��AOnDraw()��XOR�̃I�u�W�F�N�g��`�悷��
	//��������`�悵���ꏊ�ɍĕ`�悵�ď�������ꍇ�́A�Ӑ}�I��FALSE��ݒ肷�邱��
	//�u�`��̓x�Ƀg�O������v�Ƃ����̂́A�����ĕ`����L�蓾��̂����炤�܂������Ȃ�
	//������OnInitialUpdate()�ŃN���A����B
	BOOL m_bDrawSelectFrame;	//XOR�ŕ`�悷��I��g			�`������TRUE
	BOOL m_bDrawTmpData;		//XOR�ŕ`�悷��ꎞ�f�[�^		�`������TRUE
	BOOL m_bDrawOccasional;		//XOR�ŕ`�悷��Վ��f�[�^		�`������TRUE
	BOOL m_bDrawTmpDataFrame;	//XOR�ŕ`�悷��ꎞ�f�[�^�̘g	�`������TRUE
	
	CBitmap* m_pBmpBuff;	//�I�t�X�N���[���o�b�t�@
//	CSize   m_sizeTmpCacheOffset;//�L���b�V���r�b�g�}�b�v�̃I�t�Z�b�g

	BOOL m_bSnaptoGrid;			//�O���b�h�ւ̃X�i�b�v�B�R���X�g���N�^��TRUE�ɏ������B
	BOOL m_bTimerActive;		//�ړ��������Ƀ^�C�}�[���L���ȂƂ���TRUE
	BOOL m_bTimer2Active;		//�����X�N���[���̃^�C�}�[���L���ȂƂ���TRUE

	int m_nWHEEL_DELTA;


	int m_oldContentsX;
	int m_oldContentsY;
	int m_oldOpCount;
	int m_oldViewScale;
	bool m_clearTempRect;

	SXBSchMarkerLine m_tempMarkerLine;
	SXBSchDash m_tempDecoLine;

	bool m_bEditPointsMode;
	SXBSchObj* m_pObjDetectedOnTempData;
	int m_nRetOptionOnTempData;


	bool m_bClickClickLineMode;	//0.69 �N���b�N�N���b�N�Ń��C����z�u���郂�[�h
	int m_nPlaceSequence;		//0.69 �N���b�N�N���b�N�Ŕz�u����Ƃ��̃V�[�P���X�ԍ�
	bool placeLineNow(){ return(m_bClickClickLineMode && m_nPlaceSequence>0); }


	////////////////////////////////////////////////////
	//�I���I�u�W�F�N�g�̃t�H�[�J�X�Ɋւ�����
private:
	int m_nFocusedTo;
//public:
//	enum{
//		FOCUSED_TO_BODY = 0,
//		FOCUSED_TO_NUM = 1,
//		FOCUSED_TO_VAL = 2
//	};

	
public:
	////////////////////////////////////////////////////
	//�������ʂ̎w��ʒu�}�ʍ��W�ւ̃X�N���[��
	void ScrollToFoundPos(CPoint pt);
	void OnCloseFind2Dialog();


	////////////////////////////////////////////////////
	//�E�{�^���X�N���[���Ɋւ�����
private:
	int m_nRButtonMode;
	CPoint m_ptRBScrlStartMouse;
	CPoint m_ptRBScrlStartPosition;
	

	////////////////////////////////////////////////////
	//�Y�[���Ɋւ�����
	int m_nVExt;			//�\���{��(View)
	int m_nWExt;			//�\���{��(Window)
	int m_nZoom;			//0�`2�̃Y�[���l �����o�֐�SetZoom�̈���
	CPoint m_pointUpdateCenter;
	BOOL m_bZoomUpdate;		//OnUpDate()�ŃY�[�����̂Ƃ�TRUE 1997/01/26
				//���ꂪTRUE�̂Ƃ��AOnUpDate()�Ŏw��_�ւ̃X�N���[�����s��



	////////////////////////////////////////////////////
	//�\���I�v�V�����Ɋւ�����
	BOOL m_bDisplayGrid;		//�O���b�h�̕\��
	BOOL m_bUseCrossCursor;		//�N���X�J�[�\���̎g�p
	BOOL m_bCoarseStepZoom;		//�e���X�e�b�v�̃Y�[��
//	BOOL m_bDisplayPartFrame;	//���i�g�̕\��   1997/01/01 �O���[�o���ϐ��Ɉړ�
	
	/////////////////////////////////////////////////////
	//����Ɋւ�����
//	int m_nPrintMagX;			//���������{���i���q�j
//	int m_nPrintMagY;			//���������{���i���q�j
//	int m_nPrintMagX2;			//���������{���i����j
//	int m_nPrintMagY2;			//���������{���i����j
	int m_nPrintVExt;
	int m_nPrintWExt;
	bool m_bPrintColor;
	bool m_bPrintPageFit;


	CPoint m_pointPageLT;			//�y�[�W������̍���̗p����̃s�N�Z��
	CSize m_sizePageDevicePixel;	//�y�[�W������̗p����̃s�N�Z���T�C�Y
	CPoint m_pointPrintOrigin;	//�y�[�W������̍���̍��W
	CSize m_sizePrintPixel;		//�P�y�[�W�ɏo�͉\�Ȑ}�ʏ�ł̃T�C�Y
	int m_nPageX;				//���������̃y�[�W��
	int m_nPageY;				//���������̃y�[�W��

	int m_bPrintFrame;			//������̐}�ʘg�o��

	SXBSchComment m_cmntFooter;
	bool m_bPrintFooter;

	void preparePrintVars();


	//����𑜓x�ݒ� 1998/07/04
	//int	 m_nPrintRes;					//�蓮�ݒ�̈���𑜓x 0�Ȃ玩���ݒ�
	BOOL m_bBitmapPrint;
	int GetDefaultPrintRes(CDC* pDC);	//�W���̉𑜓x�𓾂�

	bool m_bSetColor2Active;


	///////////////////////////////////////
	//�L�[����Ɋւ�����
//	BOOL m_fShiftKeyDown;	//�V�t�g�L�[�̉����ŃZ�b�g�����
	int  m_nRepeatArrowKey;	//���L�[�̃��s�[�g��

public:
	void updateBuff(bool bForce = true);

//////////////////////////////////////////////////////////////////
// �v���C�x�[�g�֐�
private:
	void SetZoom(int n);			//�Y�[���l�̐ݒ�
	void BSchPtoLP(LPPOINT pp,int nVExt,int nWExt);		//�}�ʍ��W��_�����W�ɕϊ�����(LPPOINT��)
	void BSchPtoLP(LPSIZE psize,int nVExt,int nWExt);	//�}�ʍ��W��_�����W�ɕϊ�����(LPSIZE��)
	void BSchPtoLP(LPRECT prc,int nVExt,int nWExt);		//�}�ʍ��W��_�����W�ɕϊ�����(LPRECT��)
	void LPtoBSchP(LPPOINT pp,int nVExt,int nWExt);		//�_�����W��}�ʍ��W�ɕϊ�����(LPPOINT��)
	void LPtoBSchP(LPSIZE psize,int nVExt,int nWExt);	//�_�����W��}�ʍ��W�ɕϊ�����(LPSIZE��)
	void LPtoBSchP(LPRECT prc,int nVExt,int nWExt);		//�_�����W��}�ʍ��W�ɕϊ�����(LPRECT��)
	void SnapToGrid(LPPOINT pp);	//�O���b�h�ւ̃X�i�b�v
	CPoint GetClientCenter();		//�N���C�A���g�G���A�̒����̐}�ʍ��W
	CPoint GetSheetLTforScroll(CPoint* ppointCenter);//�w�肵���_�𒆉��Ƃ���X�N���[���|�W�V�����𓾂�

	BOOL IniReadCoarseStepZoom(); //���W�X�g������e���X�e�b�v�̃Y�[�����g�����ǂ����̐ݒ��ǂݍ���
	void IniWriteCoarseStepZoom(BOOL bCoarse);//���W�X�g���ɑe���X�e�b�v�̃Y�[�����g�����ǂ����̐ݒ����������

	BOOL IniReadUseCrossCursor();//���W�X�g������N���X�J�[�\���\���̏��𓾂�
	void IniWriteUseCrossCursor(BOOL bDisplay);//���W�X�g���ɃN���X�J�[�\���\���̏�����������

	DISP_BLOCKNUM_TYPE IniReadDisplayBlockNum();				//���W�X�g������u���b�N�ԍ��\���̏��𓾂�
	void IniWriteDisplayBlockNum(DISP_BLOCKNUM_TYPE dbnt);		//���W�X�g���Ƀu���b�N�ԍ��\���̏�����������


	BOOL IniReadDisplayGrid();		//���W�X�g������O���b�h�\���̏��𓾂�
	void IniWriteDisplayGrid(BOOL bDisplay);//���W�X�g���ɃO���b�h�\���̏�����������

	BOOL IniReadDisplayPartFrame();	//���W�X�g�����畔�i�g�\���̏��𓾂�
	void IniWriteDisplayPartFrame(BOOL bDisplay);//���W�X�g���ɕ��i�g�\���̏�����������

	BOOL IniReadDisplayNcPinMark();	//���W�X�g������NC�s���}�[�N�\���̏��𓾂�
	void IniWriteDisplayNcPinMark(BOOL bDisplay);//���W�X�g����NC�s���}�[�N�\���̏�����������

	BOOL IniReadDisplayHiddenPinNum();//���W�X�g�������\���s���ԍ��\���̏��𓾂�
	void IniWriteDisplayHiddenPinNum(BOOL bDisplay);//���W�X�g���ɔ�\���s���ԍ��\���̏�����������

	BOOL IniReadUsePreview();//���W�X�g�����畔�i�v���r���[�̏��𓾂�
	void IniWriteUsePreview(BOOL bUsePreview);//���W�X�g���ɕ��i�v���r���[�̏�����������

	BOOL IniReadPrintPageFit();//���W�X�g���������y�[�W�t�B�b�g���̓ǂݍ���
	void IniWritePrintPageFit(BOOL pagefit);//���W�X�g���Ɉ���y�[�W�t�B�b�g���̏�������


	int  IniReadPrintRes();			//���W�X�g���������𑜓x�̓ǂݍ���
	void IniWritePrintRes(int nRes);//���W�X�g���Ɉ���𑜓x�̏�������
	int  IniReadExpImageMag();		//���W�X�g������摜�o�͔{���̓ǂݍ��� 2016/05/03
	void IniWriteExpImageMag(int nMag);	//���W�X�g���ɉ摜�o�͔{���̏������� 2016/05/03


	void IniReadPrintMargin(int& left,int& upper,int& right,int& lower);//���W�X�g���������}�[�W���̓ǂݍ���
	void IniWritePrintMargin(int left,int upper,int right,int lower);//���W�X�g���Ɉ���}�[�W���̏�������

	BOOL CBSchView::IniReadClickClickLineMode();	//���W�X�g������N���b�N�N���b�N���C�����[�h�̏��𓾂�
	void CBSchView::IniWriteClickClickLineMode(BOOL bClickClick);	//���W�X�g���ɃN���b�N�N���b�N���C�����[�h�̏�����������


	//�R�����g�̃N���X�Ƀf�t�H���g�̃t�H���g��ݒ肷��
	void SetDefaultFontSettingsToCommentClass();


	void OnSheetXXX(int n);			//�C�x���g�n���h��OnSheet640�`3000()����Ă΂��
	
	void DrawSheetGrid(CDC* pDC,const CRect& rcClip);	//�O���b�h�̕`�� "BScView2.CPP"

	void DrawSheetFrameCharH(CDC* pDC,COLORREF col,int ny,int nWidth,UINT nMode,int nVExt,int nWExt);//�}�ʘg�̐������W�����̕`��
	void DrawSheetFrameCharV(CDC* pDC,COLORREF col,int nx,int nWidth,UINT nMode,int nVExt,int nWExt);//�}�ʘg�̐������W�����̕`��
	void DrawSheetFrame(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect& rcClip);	//�}�ʘg�̕`�� "BScView2.CPP"

	void DrawPrintBg(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect* prc);	//������̔w�i�F�̕`�� "BScView2.CPP"


	//void updateBuff(bool bForce = true);
	void drawBuffer(int x,int y,int w,int h,int clipx,int clipy, int clipw, int cliph );

	void DrawObj(							//�}�ʗv�f��`�悷��
		CDC* pDC,							//�`���
		SXBSchObj* pObj,						//�`��I�u�W�F�N�g
		DWORD dwMode,						//�`�惂�[�h
		int  nVExt,							//�\���{��(View)
		int  nWExt,							//�\���{��(Window)
		LPCRECT lprcClip=NULL);				//�N���b�v���


	void DrawListData(CDC* pDC,XBSchObjList* pList,DWORD dwMode,
		int  nVExt,int  nWExt,
		const CRect& rcClip,DWORD dwOption,DWORD layerMask=0xFFFF);//�w�胊�X�g�f�[�^�̕`�� "BScView2.CPP"
	void DrawMainData(CDC* pDC,CBSchDoc* pDoc,DWORD dwMode,
		int  nVExt,int  nWExt,
		const CRect& rcClip);//���C���f�[�^�̕`�� "BScView2.CPP"
	void DrawTmpData(CDC* pDC,CBSchDoc* pDoc,DWORD dwMode,
		int  nVExt,int  nWExt,
		const CRect& rcClip,BOOL bDraw=TRUE);	//�ꎞ�f�[�^�̕`�� "BScView2.CPP"
	void DrawTmpDataFrame(CDC* pDC,CBSchDoc* pDoc,BOOL bDraw=TRUE);//�ꎞ�f�[�^�̘g�̕`�� "BScView2.CPP"
	void DrawSelectFrame(CDC* pDC,BOOL bDraw=TRUE);			//�I�𒆂̓_���g�̕`�� "BScView2.CPP"
//	BOOL PrepareBmpTmpCache();	//�ꎞ�f�[�^�`��̂��߂̃L���b�V���r�b�g�}�b�v�̏��� "BScView2.CPP"
	void DrawOccasional(CDC* pDC,BOOL bDraw=TRUE);	//�Վ��f�[�^�̕`�� "BScView2.CPP"

	void DrawCrossCursor(CDC* pDC);
	void DrawFindMark(CDC* pDC);

	void DisplayCurrentXY();		//m_ptMouse�̒l���c�[���o�[�ɔ��f����B
	UINT QueryAutoScroll(CDC* pDC);	//�����X�N���[���̕K�v����₤
	void AutoScroll(CDC* pDC,UINT fScroll);	//�����X�N���[���̔���
	void ChangeTool(int nOldTool,int nNewTool);//�c�[����ς����Ƃ��̋��ʏ���

	void OnZoomInOutProc(BOOL bZoomIn,BOOL bKey =FALSE);	//�Y�[���C���A�E�g�̏���

	void FixSelection(int option);	//�I���̉���

	SXBSchComponent* CanEditWithLCoV();	//LCoV�ɂ�镔�i���̕ҏW���\�ȂƂ��A�L���ȕ��i�I�u�W�F�N�g��Ԃ�

	///////////////////////////////////////////////////
	//  ���j���[�A�C�e���̃C�l�[�u���̉ۂ�Ԃ�
	//    ���j���[�A�C�e���̃C�l�[�u���̉ۂ����C�����j���[�ƃR���e�L�X�g���j���[
	//  �ƂŁA����̃��W�b�N�ōs������
	//    1997/01/23 �R���e�L�X�g���j���[�̒ǉ��ƂƂ��ɂ��̊֐��Q��ǉ�����
	BOOL MenuItemTest_Undo();		//���ɖ߂�  1997/01/27 �ǉ�
	BOOL MenuItemTest_Redo();		//���Ȃ��� 2008/01/13 �ǉ�
	BOOL MenuItemTest_Cut();		//�؂���
	BOOL MenuItemTest_Copy();		//�R�s�[
	BOOL MenuItemTest_Paste();		//�\��t��
	BOOL MenuItemTest_Attribute();	//�A�g���r���[�g
	BOOL MenuItemTest_Mirror();		//�~���[
	BOOL MenuItemTest_Rotate();		//��]

	BOOL MenuItemTest_MoveToLayer(int nLayer);	//���C���[�̈ړ� 1998/06/28
	
	//////////////////////////////////////
	//�z�u�֘A
	BOOL PlaceJunction(CBSchDoc* pDoc);//�W�����N�V�����̔z�u "PLACE.CPP"
	BOOL PlacePart(CBSchDoc* pDoc,CPoint* ppoint);//�p�[�c�̔z�u "PLACE.CPP"
	BOOL PlaceEntry(CBSchDoc* pDoc);//�G���g���[�̔z�u "PLACE.CPP"
	BOOL PlaceLabel(CBSchDoc* pDoc,CPoint* ppoint);//���x���̔z�u "PLACE.CPP"
	BOOL PlaceComment(CBSchDoc* pDoc,CPoint* ppoint);//�R�����g�̔z�u "PLACE.CPP"
	BOOL PlaceTag(CBSchDoc* pDoc,CPoint* ppoint);//�^�O�̔z�u "PLACE.CPP"
	BOOL PlaceWire(CBSchDoc* pDoc);//���C���[�̔z�u "PLACE.CPP"
	BOOL PlaceBus(CBSchDoc* pDoc);//�o�X�̔z�u "PLACE.CPP"
	BOOL PlaceDash(CBSchDoc* pDoc);//�j���̔z�u "PLACE.CPP"
	BOOL PlaceMarkerLine(CBSchDoc* pDoc);//�}�[�J�[���C���̔z�u "PLACE.CPP"
	
	//////////////////////////////////////
	//���C���[�֘A
	void MoveLayer(int n);	//1998/06/30  �I��͈͂��w�背�C���[�Ɉړ�
	void OnUpdateDisplayLayer(CCmdUI *pCmdUI,int layer);
	void OnClickDisplayLayer(int layer);
	void OnUpdateEditLayer(CCmdUI *pCmdUI,int layer);
	void OnClickEditLayer(int layer);



	///////////////////////////////////////////////
	//����֘A
	void SetPageSize(CDC* pDC,int nRes);	//������̔{���A�T�C�Y�����߂�
	int  GetCountPage();		//���y�[�W���𓾂�
	void SaveToImage(CImage& image);
	BOOL ExportBitmapFile(LPCTSTR pszFileName);	//�r�b�g�}�b�v�t�@�C���o��
	BOOL ExportToClipbord();
	//BOOL ExportBidFile(LPCSTR pszFileName);		//BID�t�@�C���o��
	BOOL ExportEmf(LPCTSTR pszFileName);

	BOOL CopyEmf();

	
	void PrepareDrawFlagForPrint(CDC* pDC);	//����̂��߂̕`��t���O�̏���
	void PopDrawFlagForPrint();		//����O��PrepareDrawFlagForPrint()�ő��삵��
	                                //�`��t���O�����ɖ߂�
	
	// �I�y���[�V����
	//�A�g���r���[�g�ύX�_�C�A���O�̋N��
	BOOL DoAttributeDialog();

	//�R�����g�̃A�g���r���[�g�ύX�p�̃_�C�A���O���N��
	BOOL DoAttributeDialogComment(SXBSchComment* pObj);
	//���x���̃A�g���r���[�g�ύX�p�̃_�C�A���O���N��
	BOOL DoAttributeDialogLabel(SXBSchLabel* pObj);
	//�^�O�̃A�g���r���[�g�ύX�p�̃_�C�A���O���N��
	BOOL DoAttributeDialogTag(SXBSchTag* pObj);
	//���i�A�g���r���[�g�ύX�p�̃_�C�A���O���N��
	BOOL DoAttributeDialogComponent(SXBSchComponent* pObj);
	//�}�[�J�[���C���A�g���r���[�g�ύX�p�̃_�C�A���O���N��
	BOOL DoAttributeDialogMarkerLine(SXBSchMarkerLine* pObj);
	//�������A�g���r���[�g�ύX�p�̃_�C�A���O���N��
	BOOL DoAttributeDialogDecoLine(SXBSchDash* pObj);
	//�C���[�W�I�u�W�F�N�g�A�g���r���[�g�ύX�p�̃_�C�A���O���N��
	BOOL DoAttributeDialogImageObj(SXBSchImageObj* pObj);

public:
	void DisplayMessageLine();

private:
	CString m_strMessageLine;


public:


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CBSchView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // �\�z��̍ŏ��̂P�x�����Ăяo����܂��B
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CBSchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnExtMenuN(int n);
	void ExecExternalTool(const std::wstring& FileName);
	std::wstring GetAppPath();

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CBSchView)
	afx_msg void OnToolSelector();
	afx_msg void OnUpdateToolSelector(CCmdUI* pCmdUI);
	afx_msg void OnToolDrag();
	afx_msg void OnUpdateToolDrag(CCmdUI* pCmdUI);
	afx_msg void OnToolPart();
	afx_msg void OnUpdateToolPart(CCmdUI* pCmdUI);
	afx_msg void OnToolWire();
	afx_msg void OnUpdateToolWire(CCmdUI* pCmdUI);
	afx_msg void OnToolBus();
	afx_msg void OnUpdateToolBus(CCmdUI* pCmdUI);
	afx_msg void OnToolDash();
	afx_msg void OnUpdateToolDash(CCmdUI* pCmdUI);
	afx_msg void OnToolJunction();
	afx_msg void OnUpdateToolJunction(CCmdUI* pCmdUI);
	afx_msg void OnToolEntryWl();
	afx_msg void OnUpdateToolEntryWl(CCmdUI* pCmdUI);
	afx_msg void OnToolEntryBl();
	afx_msg void OnUpdateToolEntryBl(CCmdUI* pCmdUI);
	afx_msg void OnToolTag();
	afx_msg void OnUpdateToolTag(CCmdUI* pCmdUI);
	afx_msg void OnToolLabel();
	afx_msg void OnUpdateToolLabel(CCmdUI* pCmdUI);
	afx_msg void OnToolComment();
	afx_msg void OnUpdateToolComment(CCmdUI* pCmdUI);
	afx_msg void OnSetLibrary();
	afx_msg void OnSetCommentFont();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnSheet640();
	afx_msg void OnSheet1000();
	afx_msg void OnSheet1600();
	afx_msg void OnSheet2000();
	afx_msg void OnSheet3000();
	afx_msg void OnUpdateDispGrid(CCmdUI* pCmdUI);
	afx_msg void OnDispGrid();
	afx_msg void OnUpdatePartFrame(CCmdUI* pCmdUI);
	afx_msg void OnPartFrame();
	afx_msg void OnUpdateSnapGrid(CCmdUI* pCmdUI);
	afx_msg void OnSnapGrid();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnRefresh();
	afx_msg void OnUpdateRotate(CCmdUI* pCmdUI);
	afx_msg void OnRotate();
	afx_msg void OnUpdateMirror(CCmdUI* pCmdUI);
	afx_msg void OnMirror();
	afx_msg void OnUpdateEditAttribute(CCmdUI* pCmdUI);
	afx_msg void OnEditAttribute();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFileExpoBmp();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFilePrintOption();
//	afx_msg void OnUsePreview();
//	afx_msg void OnUpdateUsePreview(CCmdUI* pCmdUI);
	afx_msg void OnEditSetuplayer();
	afx_msg void OnUpdateEditMovetolayer0(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer7(CCmdUI* pCmdUI);
	afx_msg void OnEditMovetolayer0();
	afx_msg void OnEditMovetolayer1();
	afx_msg void OnEditMovetolayer2();
	afx_msg void OnEditMovetolayer3();
	afx_msg void OnEditMovetolayer4();
	afx_msg void OnEditMovetolayer5();
	afx_msg void OnEditMovetolayer6();
	afx_msg void OnEditMovetolayer7();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSheetFree();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateMenuCrossCursor(CCmdUI *pCmdUI);
	afx_msg void OnMenuCrossCursor();
	afx_msg void OnFileExpoEmf();
	afx_msg void OnSetColor();
	afx_msg void OnEditFind();
	//afx_msg void OnEditFindnext();
	//afx_msg void OnUpdateEditFindnext(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnExtMenu_0();
	afx_msg void OnExtMenu_1();
	afx_msg void OnExtMenu_2();
	afx_msg void OnExtMenu_3();
	afx_msg void OnExtMenu_4();
	afx_msg void OnExtMenu_5();
	afx_msg void OnExtMenu_6();
	afx_msg void OnExtMenu_7();
	afx_msg void OnExtMenu_8();
	afx_msg void OnExtMenu_9();

	afx_msg void OnEditEmfCopy();
	afx_msg void OnUpdateEditEmfCopy(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnSetCoarseStepZoom();
//	afx_msg void OnUpdateSetCoarseStepZoom(CCmdUI *pCmdUI);
	afx_msg void OnDispNcmark();
	afx_msg void OnUpdateDispNcmark(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer0(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer7(CCmdUI *pCmdUI);
	afx_msg void OnClickDisplayLayer0();
	afx_msg void OnClickDisplayLayer1();
	afx_msg void OnClickDisplayLayer2();
	afx_msg void OnClickDisplayLayer3();
	afx_msg void OnClickDisplayLayer4();
	afx_msg void OnClickDisplayLayer5();
	afx_msg void OnClickDisplayLayer6();
	afx_msg void OnClickDisplayLayer7();
	afx_msg void OnUpdateEditLayer0(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer7(CCmdUI *pCmdUI);
	afx_msg void OnClickEditLayer0();
	afx_msg void OnClickEditLayer1();
	afx_msg void OnClickEditLayer2();
	afx_msg void OnClickEditLayer3();
	afx_msg void OnClickEditLayer4();
	afx_msg void OnClickEditLayer5();
	afx_msg void OnClickEditLayer6();
	afx_msg void OnClickEditLayer7();
	afx_msg void OnUpdateSheetFree(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet640(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet1000(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet1600(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet2000(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet3000(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnToolMarker();
	afx_msg void OnUpdateToolMarker(CCmdUI *pCmdUI);
	afx_msg void OnDispHiddenpinnum();
	afx_msg void OnUpdateDispHiddenpinnum(CCmdUI *pCmdUI);
	afx_msg void OnSetupQuick();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnSetupAllowappendselect();
	afx_msg void OnUpdateSetupAllowappendselect(CCmdUI *pCmdUI);
	afx_msg void OnConvertC2l();
	afx_msg void OnUpdateConvertC2l(CCmdUI *pCmdUI);
	afx_msg void OnConvertC2t();
	afx_msg void OnUpdateConvertC2t(CCmdUI *pCmdUI);
	afx_msg void OnConvertL2t();
	afx_msg void OnUpdateConvertL2t(CCmdUI *pCmdUI);
	afx_msg void OnConvertL2c();
	afx_msg void OnUpdateConvertL2c(CCmdUI *pCmdUI);
	afx_msg void OnConvertT2l();
	afx_msg void OnUpdateConvertT2l(CCmdUI *pCmdUI);
	afx_msg void OnConvertT2c();
	afx_msg void OnUpdateConvertT2c(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeTagframe();
	afx_msg void OnUpdateBulkattributeTagframe(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeMarkerline();
	afx_msg void OnUpdateBulkattributeMarkerline(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeCommentfont();
	afx_msg void OnUpdateBulkattributeCommentfont(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeReplacecomponentinfo();
	afx_msg void OnUpdateBulkattributeReplacecomponentinfo(CCmdUI *pCmdUI);
	afx_msg void OnInsertBitmap();
	afx_msg void OnUpdateBulkattributeHidenumbers(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeHidenumbers();
	afx_msg void OnToolUseobjectfilter();
	afx_msg void OnUpdateToolUseobjectfilter(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPoints(CCmdUI *pCmdUI);
	afx_msg void OnEditPoints();
	afx_msg void OnSetupPlacelinebyclick();
	afx_msg void OnUpdateSetupPlacelinebyclick(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOrderBringtofront(CCmdUI *pCmdUI);
	afx_msg void OnOrderBringtofront();
	afx_msg void OnOrderBringtoback();
	afx_msg void OnUpdateReloadReload(CCmdUI *pCmdUI);
	afx_msg void OnReloadReload();
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);

	afx_msg LRESULT OnScrollToMsg(UINT wParam,LONG lParam);
	afx_msg void OnBulkattributeDecorationline();
	afx_msg void OnUpdateBulkattributeDecorationline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFloatEditWithLcov(CCmdUI *pCmdUI);
	afx_msg void OnFloatEditWithLcov();
	afx_msg void OnSetupDispblock();
	afx_msg void OnToolRunLcov();
	afx_msg void OnToolRunPl3w();
	afx_msg void OnToolRunNut3w();
	afx_msg void OnToolRunNl3w();
	afx_msg void OnToolRunCe3search();
	afx_msg void OnFileExpoClipbord();
};

#ifndef _DEBUG  // BSchView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CBSchDoc* CBSchView::GetDocument()
   { return (CBSchDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

#endif
