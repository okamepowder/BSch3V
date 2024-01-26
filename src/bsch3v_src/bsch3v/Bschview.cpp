/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSchView.cpp : CBSchView クラスの動作の定義を行います。

#include "stdafx.h"

#include <assert.h>
#include <string>
#include <list>
#include <imm.h>
using namespace std;

#include "atlpath.h"


#include "BSch.h"

#include "SetLib.h"

#include "BSchDoc.h"
#include "coord2.h"

#include "BSchView.h"
#include "MainFrm.h"

//#include "PartLib.h"
#include "BSchFont.h"
#include "Global.h"

//#include "BSchPart.h"
#include "complib.h"
#include "SetLayer.h"	//  レイヤー設定ダイアログクラス 1998/06/28

#include "CmntAtrb.h"
#include "LblAtrb.h"
#include "TagAtrb.h"
#include "PartAtrb.h"

#include "drawobj.h"
#include "xbschglobal.h"

#include "setfsize.h"
#include ".\bschview.h"

#include "setfont.h"
//#include "setcolor.h"
#include "setcolor2.h"

#include "finddlg.h"

#include "MarkerLineAtrb.h"
#include "SetDecorationLine.h"

#include "QuickProp.h"
#include "SetQuickProp.h"

#include "SetTagFrame.h"

#include "Preview.h"
#include "SelPart.h"

#include "AtribImage.h"

#include "BulkHidingNumVal.h"

#include "ObjFilter.h"
#include "DlgObjFilter.h"

#include "DispBlockNum.h"

#include "Find2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER_AUTOSCROLL	2
//#define ID_TIMER_TMPDRAW	1


/////////////////////////////////////////////////////////////////////////////
// CBSchView

IMPLEMENT_DYNCREATE(CBSchView, CScrollView)

BEGIN_MESSAGE_MAP(CBSchView, CScrollView)
	//{{AFX_MSG_MAP(CBSchView)
	ON_COMMAND(ID_TOOL_SELECTOR, OnToolSelector)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SELECTOR, OnUpdateToolSelector)
	ON_COMMAND(ID_TOOL_DRAG, OnToolDrag)
	ON_UPDATE_COMMAND_UI(ID_TOOL_DRAG, OnUpdateToolDrag)
	ON_COMMAND(ID_TOOL_PART, OnToolPart)
	ON_UPDATE_COMMAND_UI(ID_TOOL_PART, OnUpdateToolPart)
	ON_COMMAND(ID_TOOL_WIRE, OnToolWire)
	ON_UPDATE_COMMAND_UI(ID_TOOL_WIRE, OnUpdateToolWire)
	ON_COMMAND(ID_TOOL_BUS, OnToolBus)
	ON_UPDATE_COMMAND_UI(ID_TOOL_BUS, OnUpdateToolBus)
	ON_COMMAND(ID_TOOL_DASH, OnToolDash)
	ON_UPDATE_COMMAND_UI(ID_TOOL_DASH, OnUpdateToolDash)
	ON_COMMAND(ID_TOOL_JUNCTION, OnToolJunction)
	ON_UPDATE_COMMAND_UI(ID_TOOL_JUNCTION, OnUpdateToolJunction)
	ON_COMMAND(ID_TOOL_ENTRY_WL, OnToolEntryWl)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ENTRY_WL, OnUpdateToolEntryWl)
	ON_COMMAND(ID_TOOL_ENTRY_BL, OnToolEntryBl)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ENTRY_BL, OnUpdateToolEntryBl)
	ON_COMMAND(ID_TOOL_TAG, OnToolTag)
	ON_UPDATE_COMMAND_UI(ID_TOOL_TAG, OnUpdateToolTag)
	ON_COMMAND(ID_TOOL_LABEL, OnToolLabel)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LABEL, OnUpdateToolLabel)
	ON_COMMAND(ID_TOOL_COMMENT, OnToolComment)
	ON_UPDATE_COMMAND_UI(ID_TOOL_COMMENT, OnUpdateToolComment)
	ON_COMMAND(ID_SET_LIBRARY, OnSetLibrary)
	ON_COMMAND(ID_SET_COMMENT_FONT, OnSetCommentFont)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_SHEET_640, OnSheet640)
	ON_COMMAND(ID_SHEET_1000, OnSheet1000)
	ON_COMMAND(ID_SHEET_1600, OnSheet1600)
	ON_COMMAND(ID_SHEET_2000, OnSheet2000)
	ON_COMMAND(ID_SHEET_3000, OnSheet3000)
	ON_UPDATE_COMMAND_UI(ID_DISP_GRID, OnUpdateDispGrid)
	ON_COMMAND(ID_DISP_GRID, OnDispGrid)
	ON_UPDATE_COMMAND_UI(ID_PART_FRAME, OnUpdatePartFrame)
	ON_COMMAND(ID_PART_FRAME, OnPartFrame)
	ON_UPDATE_COMMAND_UI(ID_SNAP_GRID, OnUpdateSnapGrid)
	ON_COMMAND(ID_SNAP_GRID, OnSnapGrid)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_WM_TIMER()
//	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_UPDATE_COMMAND_UI(ID_ROTATE, OnUpdateRotate)
	ON_COMMAND(ID_ROTATE, OnRotate)
	ON_UPDATE_COMMAND_UI(ID_MIRROR, OnUpdateMirror)
	ON_COMMAND(ID_MIRROR, OnMirror)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ATTRIBUTE, OnUpdateEditAttribute)
	ON_COMMAND(ID_EDIT_ATTRIBUTE, OnEditAttribute)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_EXPO_BMP, OnFileExpoBmp)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_PRINT_OPTION, OnFilePrintOption)
//	ON_COMMAND(ID_USE_PREVIEW, OnUsePreview)
//	ON_UPDATE_COMMAND_UI(ID_USE_PREVIEW, OnUpdateUsePreview)
	ON_COMMAND(ID_EDIT_SETUPLAYER, OnEditSetuplayer)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER0, OnUpdateEditMovetolayer0)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER1, OnUpdateEditMovetolayer1)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER2, OnUpdateEditMovetolayer2)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER3, OnUpdateEditMovetolayer3)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER4, OnUpdateEditMovetolayer4)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER5, OnUpdateEditMovetolayer5)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER6, OnUpdateEditMovetolayer6)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOLAYER7, OnUpdateEditMovetolayer7)
	ON_COMMAND(ID_EDIT_MOVETOLAYER0, OnEditMovetolayer0)
	ON_COMMAND(ID_EDIT_MOVETOLAYER1, OnEditMovetolayer1)
	ON_COMMAND(ID_EDIT_MOVETOLAYER2, OnEditMovetolayer2)
	ON_COMMAND(ID_EDIT_MOVETOLAYER3, OnEditMovetolayer3)
	ON_COMMAND(ID_EDIT_MOVETOLAYER4, OnEditMovetolayer4)
	ON_COMMAND(ID_EDIT_MOVETOLAYER5, OnEditMovetolayer5)
	ON_COMMAND(ID_EDIT_MOVETOLAYER6, OnEditMovetolayer6)
	ON_COMMAND(ID_EDIT_MOVETOLAYER7, OnEditMovetolayer7)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_SHEET_FREE, OnSheetFree)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
//	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
ON_COMMAND(ID_FILE_EXPORTEMFDIRECT, OnFileEmfDirect)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
ON_UPDATE_COMMAND_UI(ID_MENU_CROSS_CURSOR, OnUpdateMenuCrossCursor)
ON_COMMAND(IDM_EXTMENU_0, OnExtMenu_0)
ON_COMMAND(IDM_EXTMENU_1, OnExtMenu_1)
ON_COMMAND(IDM_EXTMENU_2, OnExtMenu_2)
ON_COMMAND(IDM_EXTMENU_3, OnExtMenu_3)
ON_COMMAND(IDM_EXTMENU_4, OnExtMenu_4)
ON_COMMAND(IDM_EXTMENU_5, OnExtMenu_5)
ON_COMMAND(IDM_EXTMENU_6, OnExtMenu_6)
ON_COMMAND(IDM_EXTMENU_7, OnExtMenu_7)
ON_COMMAND(IDM_EXTMENU_8, OnExtMenu_8)
ON_COMMAND(IDM_EXTMENU_9, OnExtMenu_9)
ON_COMMAND(ID_MENU_CROSS_CURSOR, OnMenuCrossCursor)
ON_COMMAND(ID_FILE_EXPO_EMF, OnFileExpoEmf)
ON_COMMAND(ID_SET_COLOR, OnSetColor)
ON_COMMAND(ID_EDIT_FIND, OnEditFind)
//ON_COMMAND(ID_EDIT_FINDNEXT, OnEditFindnext)
//ON_UPDATE_COMMAND_UI(ID_EDIT_FINDNEXT, OnUpdateEditFindnext)
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_COMMAND(ID_EDIT_EMF_COPY, OnEditEmfCopy)
ON_UPDATE_COMMAND_UI(ID_EDIT_EMF_COPY, OnUpdateEditEmfCopy)
ON_WM_CREATE()
//ON_COMMAND(ID_SET_COARSE_STEP_ZOOM, OnSetCoarseStepZoom)
//ON_UPDATE_COMMAND_UI(ID_SET_COARSE_STEP_ZOOM, OnUpdateSetCoarseStepZoom)
ON_COMMAND(ID_DISP_NCMARK, OnDispNcmark)
ON_UPDATE_COMMAND_UI(ID_DISP_NCMARK, OnUpdateDispNcmark)
ON_UPDATE_COMMAND_UI(IDC_D0, OnUpdateDisplayLayer0)
ON_UPDATE_COMMAND_UI(IDC_D1, OnUpdateDisplayLayer1)
ON_UPDATE_COMMAND_UI(IDC_D2, OnUpdateDisplayLayer2)
ON_UPDATE_COMMAND_UI(IDC_D3, OnUpdateDisplayLayer3)
ON_UPDATE_COMMAND_UI(IDC_D4, OnUpdateDisplayLayer4)
ON_UPDATE_COMMAND_UI(IDC_D5, OnUpdateDisplayLayer5)
ON_UPDATE_COMMAND_UI(IDC_D6, OnUpdateDisplayLayer6)
ON_UPDATE_COMMAND_UI(IDC_D7, OnUpdateDisplayLayer7)
ON_BN_CLICKED(IDC_D0, OnClickDisplayLayer0)
ON_BN_CLICKED(IDC_D1, OnClickDisplayLayer1)
ON_BN_CLICKED(IDC_D2, OnClickDisplayLayer2)
ON_BN_CLICKED(IDC_D3, OnClickDisplayLayer3)
ON_BN_CLICKED(IDC_D4, OnClickDisplayLayer4)
ON_BN_CLICKED(IDC_D5, OnClickDisplayLayer5)
ON_BN_CLICKED(IDC_D6, OnClickDisplayLayer6)
ON_BN_CLICKED(IDC_D7, OnClickDisplayLayer7)
ON_UPDATE_COMMAND_UI(IDC_E0, OnUpdateEditLayer0)
ON_UPDATE_COMMAND_UI(IDC_E1, OnUpdateEditLayer1)
ON_UPDATE_COMMAND_UI(IDC_E2, OnUpdateEditLayer2)
ON_UPDATE_COMMAND_UI(IDC_E3, OnUpdateEditLayer3)
ON_UPDATE_COMMAND_UI(IDC_E4, OnUpdateEditLayer4)
ON_UPDATE_COMMAND_UI(IDC_E5, OnUpdateEditLayer5)
ON_UPDATE_COMMAND_UI(IDC_E6, OnUpdateEditLayer6)
ON_UPDATE_COMMAND_UI(IDC_E7, OnUpdateEditLayer7)
ON_BN_CLICKED(IDC_E0, OnClickEditLayer0)
ON_BN_CLICKED(IDC_E1, OnClickEditLayer1)
ON_BN_CLICKED(IDC_E2, OnClickEditLayer2)
ON_BN_CLICKED(IDC_E3, OnClickEditLayer3)
ON_BN_CLICKED(IDC_E4, OnClickEditLayer4)
ON_BN_CLICKED(IDC_E5, OnClickEditLayer5)
ON_BN_CLICKED(IDC_E6, OnClickEditLayer6)
ON_BN_CLICKED(IDC_E7, OnClickEditLayer7)
ON_UPDATE_COMMAND_UI(ID_SHEET_FREE, OnUpdateSheetFree)
ON_UPDATE_COMMAND_UI(ID_SHEET_640, OnUpdateSheet640)
ON_UPDATE_COMMAND_UI(ID_SHEET_1000, OnUpdateSheet1000)
ON_UPDATE_COMMAND_UI(ID_SHEET_1600, OnUpdateSheet1600)
ON_UPDATE_COMMAND_UI(ID_SHEET_2000, OnUpdateSheet2000)
ON_UPDATE_COMMAND_UI(ID_SHEET_3000, OnUpdateSheet3000)
ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
ON_COMMAND(ID_TOOL_MARKER, OnToolMarker)
ON_UPDATE_COMMAND_UI(ID_TOOL_MARKER, OnUpdateToolMarker)
ON_COMMAND(ID_DISP_HIDDENPINNUM, OnDispHiddenpinnum)
ON_UPDATE_COMMAND_UI(ID_DISP_HIDDENPINNUM, OnUpdateDispHiddenpinnum)
ON_COMMAND(ID_SETUP_QUICK, &CBSchView::OnSetupQuick)
ON_WM_MOUSEHOVER()
ON_COMMAND(ID_SETUP_ALLOWAPPENDSELECT, &CBSchView::OnSetupAllowappendselect)
ON_UPDATE_COMMAND_UI(ID_SETUP_ALLOWAPPENDSELECT, &CBSchView::OnUpdateSetupAllowappendselect)
ON_COMMAND(ID_CONVERT_C2L, &CBSchView::OnConvertC2l)
ON_UPDATE_COMMAND_UI(ID_CONVERT_C2L, &CBSchView::OnUpdateConvertC2l)
ON_COMMAND(ID_CONVERT_C2T, &CBSchView::OnConvertC2t)
ON_UPDATE_COMMAND_UI(ID_CONVERT_C2T, &CBSchView::OnUpdateConvertC2t)
ON_COMMAND(ID_CONVERT_L2T, &CBSchView::OnConvertL2t)
ON_UPDATE_COMMAND_UI(ID_CONVERT_L2T, &CBSchView::OnUpdateConvertL2t)
ON_COMMAND(ID_CONVERT_L2C, &CBSchView::OnConvertL2c)
ON_UPDATE_COMMAND_UI(ID_CONVERT_L2C, &CBSchView::OnUpdateConvertL2c)
ON_COMMAND(ID_CONVERT_T2L, &CBSchView::OnConvertT2l)
ON_UPDATE_COMMAND_UI(ID_CONVERT_T2L, &CBSchView::OnUpdateConvertT2l)
ON_COMMAND(ID_CONVERT_T2C, &CBSchView::OnConvertT2c)
ON_UPDATE_COMMAND_UI(ID_CONVERT_T2C, &CBSchView::OnUpdateConvertT2c)
ON_COMMAND(ID_BULKATTRIBUTE_TAGFRAME, &CBSchView::OnBulkattributeTagframe)
ON_UPDATE_COMMAND_UI(ID_BULKATTRIBUTE_TAGFRAME, &CBSchView::OnUpdateBulkattributeTagframe)
ON_COMMAND(ID_BULKATTRIBUTE_MARKERLINE, &CBSchView::OnBulkattributeMarkerline)
ON_UPDATE_COMMAND_UI(ID_BULKATTRIBUTE_MARKERLINE, &CBSchView::OnUpdateBulkattributeMarkerline)
ON_COMMAND(ID_BULKATTRIBUTE_COMMENTFONT, &CBSchView::OnBulkattributeCommentfont)
ON_UPDATE_COMMAND_UI(ID_BULKATTRIBUTE_COMMENTFONT, &CBSchView::OnUpdateBulkattributeCommentfont)
ON_COMMAND(ID_BULKATTRIBUTE_REPLACECOMPONENTINFO, &CBSchView::OnBulkattributeReplacecomponentinfo)
ON_UPDATE_COMMAND_UI(ID_BULKATTRIBUTE_REPLACECOMPONENTINFO, &CBSchView::OnUpdateBulkattributeReplacecomponentinfo)
ON_COMMAND(ID_INSERT_BITMAP, &CBSchView::OnInsertBitmap)
ON_UPDATE_COMMAND_UI(ID_BULKATTRIBUTE_HIDENUMBERS, &CBSchView::OnUpdateBulkattributeHidenumbers)
ON_COMMAND(ID_BULKATTRIBUTE_HIDENUMBERS, &CBSchView::OnBulkattributeHidenumbers)
ON_COMMAND(ID_TOOL_USEOBJECTFILTER, &CBSchView::OnToolUseobjectfilter)
ON_UPDATE_COMMAND_UI(ID_TOOL_USEOBJECTFILTER, &CBSchView::OnUpdateToolUseobjectfilter)
ON_UPDATE_COMMAND_UI(ID_EDIT_POINTS, &CBSchView::OnUpdateEditPoints)
ON_COMMAND(ID_EDIT_POINTS, &CBSchView::OnEditPoints)
ON_COMMAND(ID_SETUP_PLACELINEBYCLICK, &CBSchView::OnSetupPlacelinebyclick)
ON_UPDATE_COMMAND_UI(ID_SETUP_PLACELINEBYCLICK, &CBSchView::OnUpdateSetupPlacelinebyclick)
ON_UPDATE_COMMAND_UI(ID_ORDER_BRINGTOFRONT, &CBSchView::OnUpdateOrderBringtofront)
ON_UPDATE_COMMAND_UI(ID_ORDER_BRINGTOBACK, &CBSchView::OnUpdateOrderBringtofront)
ON_COMMAND(ID_ORDER_BRINGTOFRONT, &CBSchView::OnOrderBringtofront)
ON_COMMAND(ID_ORDER_BRINGTOBACK, &CBSchView::OnOrderBringtoback)
ON_UPDATE_COMMAND_UI(ID_RELOAD_RELOAD, &CBSchView::OnUpdateReloadReload)
ON_COMMAND(ID_RELOAD_RELOAD, &CBSchView::OnReloadReload)
ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, &CBSchView::OnUpdateEditFind)
ON_MESSAGE(SCROLL_TO_MSG,OnScrollToMsg)
ON_COMMAND(ID_BULKATTRIBUTE_DECORATIONLINE, &CBSchView::OnBulkattributeDecorationline)
ON_UPDATE_COMMAND_UI(ID_BULKATTRIBUTE_DECORATIONLINE, &CBSchView::OnUpdateBulkattributeDecorationline)
ON_UPDATE_COMMAND_UI(ID_FLOAT_EDIT_WITH_LCOV, &CBSchView::OnUpdateFloatEditWithLcov)
ON_COMMAND(ID_FLOAT_EDIT_WITH_LCOV, &CBSchView::OnFloatEditWithLcov)
ON_COMMAND(ID_SETUP_DISPBLOCK, &CBSchView::OnSetupDispblock)
ON_COMMAND(ID_TOOL_RUN_LCOV, &CBSchView::OnToolRunLcov)
ON_COMMAND(ID_TOOL_RUN_PL3W, &CBSchView::OnToolRunPl3w)
ON_COMMAND(ID_TOOL_RUN_NUT3W, &CBSchView::OnToolRunNut3w)
ON_COMMAND(ID_TOOL_RUN_NL3W, &CBSchView::OnToolRunNl3w)
ON_COMMAND(ID_TOOL_RUN_CE3SEARCH, &CBSchView::OnToolRunCe3search)
ON_COMMAND(ID_FILE_EXPO_CLIPBORD, &CBSchView::OnFileExpoClipbord)
END_MESSAGE_MAP()

SBSchDrawColor CBSchView::m_COL;

//COLORREF CBSchView::m_COL.colBG			=	COL_BG;
//COLORREF CBSchView::m_COL.colJUNC			=	COL_JUNC;
//COLORREF CBSchView::m_COL.colWIRE			=	COL_WIRE;
//COLORREF CBSchView::m_COL.colBUS			=	COL_BUS;
//COLORREF CBSchView::m_COL.colDASH			=	COL_DASH;
//COLORREF CBSchView::m_COL.colTAG			=	COL_TAG;
//COLORREF CBSchView::m_COL.colTAG_NAME		=	COL_TAG_NAME;
//COLORREF CBSchView::m_COL.colCOMMENT		=	COL_COMMENT;
//COLORREF CBSchView::m_COL.colLABEL			=	COL_LABEL;
//COLORREF CBSchView::m_COL.colXOR			=	COL_XOR;
//COLORREF CBSchView::m_COL.colPART			=	COL_PART;
//COLORREF CBSchView::m_COL.colPART_PIN		=	COL_PART_PIN;
//COLORREF CBSchView::m_COL.colPART_PINNAME	=	COL_PART_PINNAME;	
//COLORREF CBSchView::m_COL.colPART_PINNUM	=	COL_PART_PINNUM;
//COLORREF CBSchView::m_COL.colPART_NAME		=	COL_PART_NAME;	
//COLORREF CBSchView::m_COL.colPART_NUM		=	COL_PART_NUM;
//COLORREF CBSchView::m_COL.colPART_FRAME	=	COL_PART_FRAME;
//COLORREF CBSchView::m_COL.colSHEET_FRAME	=	COL_SHEET_FRAME;
//COLORREF CBSchView::m_COL.colSHEET_GRID	=	COL_SHEET_GRID;
//COLORREF CBSchView::m_COL.colDARK			=	COL_DARK;
//COLORREF CBSchView::m_COL.colZLPIN			=	COL_ZLPIN;	//ZeroLengthPin
//COLORREF CBSchView::m_COL.colNC_MARK		=	COL_NC_MARK;
//COLORREF CBSchView::m_COL.colQUICKPROPVIEW =	COL_QUICKPROPVIEW;
//COLORREF CBSchView::m_COL.colTEMPFRAME_FOCUSED		= COL_TEMPFRAME_FOCUSED;
//COLORREF CBSchView::m_COL.colTEMPFRAME_NOT_FOCUSED	= COL_TEMPFRAME_NOT_FOCUSED;



//{
//public:
//	enum CAPTUREBUTTON
//	{
//		MCI_LEFT = 1,
//		MCI_RIGHT = 2
//	};
//protected:
//	unsigned m_captureValue;
//public:
//	MouseCaptureInfo(){
//		m_captureValue = 0;
//	}

//左右ボタンによるマウスのキャプチャ状態を管理するクラス

unsigned MouseCaptureInfo::SetCapture(CAPTUREBUTTON button)
{
	if((m_captureValue & (unsigned)button)==0){
		if(m_captureValue ==0){
			m_pWnd->SetCapture();
		}
		m_captureValue |= (unsigned)button;
	}
	return m_captureValue;
}

unsigned MouseCaptureInfo::ReleaseCapture(CAPTUREBUTTON button)
{
	if((m_captureValue & (unsigned)button)!=0){
		m_captureValue &= ~((unsigned)button);
		if(m_captureValue == 0){
			::ReleaseCapture();
		}
	}
	return m_captureValue;
}
	


/////////////////////////////////////////////////////////////////////////////
// CBSchView クラスの構築/消滅

CBSchView::CBSchView()
{
	// TODO: この場所に構築用のコードを追加してください。
	m_allowAppendSelect = true;

	m_nFocusedTo = FOCUSED_TO_BODY;

	m_nCurrentTool=ID_TOOL_SELECTOR;
	SetZoom(4);

	m_mouseCaptureInfo.Init(this);
//20100521	m_bCaptureMouse=FALSE;
//20100521	m_bRBCaptureMouse=FALSE;

	m_dispBN = DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM;

	m_bOnSelect=FALSE;
	m_nOnMove=0;
	m_bSnaptoGrid=TRUE;
	m_bTimerActive=FALSE;
	m_bTimer2Active=FALSE;
	m_pBmpBuff = NULL;		//2004/06/05
//	m_pBmpTmpCache=NULL;
	m_nLastSelectLibrary=0;
	m_nLastSelectPart=0;
	m_bZoomUpdate=FALSE;
	//m_bUsePreview=FALSE;
	//m_fShiftKeyDown=FALSE;	//1998/07/18
	m_fEditHighLight=FALSE;

	m_nWHEEL_DELTA = 0;

	m_bBitmapPrint = FALSE;

	m_oldContentsX	=-1;
	m_oldContentsY	=-1;
	m_oldOpCount	=-1;
	m_oldViewScale	=-1;

	m_bPrintColor = true;

	m_bSetColor2Active = false;

	m_nRButtonMode = 0;

	m_bClickClickLineMode = false;	//0.69
	m_nPlaceSequence = 0;			//0.69

	//{
	//	::g_bIsWin9x = false;
	//	OSVERSIONINFO versionInfo;
	//	versionInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	//	if(GetVersionEx(&versionInfo)){
	//		::g_bIsWin9x = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
	//	}
	//}

	::g_bFillEndOfPtnLine = (AfxGetApp()->GetProfileInt(_T("Option"),_T("FillEndOfPtnLine"),0)!=0);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("FillEndOfPtnLine"),g_bFillEndOfPtnLine);

	m_displayQuickPropComp = false;
	m_pQuickPropComp = new CQuickProp;	//090429	
	m_ptDisplayQuickPropAt = CPoint(-100,-100);

	m_ObjectFileterValue = 0;//091010
	m_pObjFilterDlg = NULL;

	//一時データの装飾線は長さを持たない。アトリビュートの保持だけに使う。
	m_tempDecoLine.setP1(SPoint(0,0));
	m_tempDecoLine.setP2(SPoint(0,0));

	m_bEditPointsMode = false;
	m_FindMarkActive = false;

	//bool m_clearTempRect;
}

CBSchView::~CBSchView()
{
//	delete m_pBmpTmpCache;

	//FILE *fp;
	//if((fp = _tfopen(g_logPath,_T("wt, ccs=UTF-8")))!=NULL){
	//	fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.	
	//	fputws(g_log.c_str(),fp);
	//	//fputws(_T("test"),fp);
	//	fclose(fp);
	//}



	delete m_pBmpBuff;
	delete m_pQuickPropComp;	//090429	
	delete m_pObjFilterDlg;
	delete m_pFind2Dlg;
}

BOOL CBSchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//       修正してください。
	
	/////////////////////////////////////////////////////////////////////////////
	// 1996/10/31
	//サイズ変更時に再描画を行わないようにするために、ここを設定する。
	//デフォルトでは、CS_HREDRAW|CS_VREDRAW が有効になっているため、再描画を行う。
	//CMainFrame の PreCreateWindow()も同様に設定しないと有効にならない。
	if(cs.lpszClass==NULL){
		LPCTSTR lpszClassName=AfxRegisterWndClass(	//ウィンドウクラスの新規作成
			CS_DBLCLKS|CS_BYTEALIGNCLIENT,//|CS_HREDRAW|CS_VREDRAW, 
			0,										//カーソルはNULL
			0);
			//(HBRUSH)GetStockObject(WHITE_BRUSH));	//背景は白
		cs.lpszClass=lpszClassName;
	}
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBSchView クラスの描画

void CBSchView::updateBuff(bool bForce)
{
	CBSchDoc* pDoc = GetDocument();

	bool update=bForce;

	CRect rcClient;
	GetClientRect(&rcClient);
	int w=rcClient.right;
	int h=rcClient.bottom;

	CPoint ptLeftTop = GetDeviceScrollPosition();
	int x=ptLeftTop.x;
	int y=ptLeftTop.y;

//	TRACE("*** updateBuff ScrollPosition %d,%d ***\n",x,y);

	
	int opCount = pDoc->opCount();

	if(!m_pBmpBuff) update = true;

	if(m_oldContentsX != x || m_oldContentsY != y){
		update=true;
		m_oldContentsX = x;
		m_oldContentsY = y;
	}

	if(m_oldOpCount	!= opCount){
		update=true;
		m_oldOpCount = opCount;
		TRACE("UPDATE m_oldOpCount %d\n",m_oldOpCount);
	}

	if(m_oldViewScale != m_nZoom){
		update=true;
		m_oldViewScale = m_nZoom;
	}

	if(update)drawBuffer(x,y,w,h,x,y,w,h);

}

void CBSchView::drawBuffer(int x,int y,int w,int h,int clipx,int clipy, int clipw, int cliph )
{

	CDC* pDC = GetDC();

	if(!m_pBmpBuff){
		m_pBmpBuff = new CBitmap;		//ビットマップオブジェクトを new して
		if(!m_pBmpBuff) return;
		if(!m_pBmpBuff->CreateCompatibleBitmap(pDC,w,h)){
			delete m_pBmpBuff;
			m_pBmpBuff = NULL;
			ReleaseDC(pDC);
			return;
		}
	}
	CBSchDoc* pDoc = GetDocument();

	//CRect rcClip = m_rcClip;
	CRect rcClip = CRect(CPoint(clipx,clipy),CSize(clipw,cliph));
	LPtoBSchP(&rcClip,m_nVExt,m_nWExt);

	CBitmap* pOldBmp;
	CDC dcMem;							//メモリデバイスコンテキストオブジェクトの作成
	dcMem.CreateCompatibleDC(pDC);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
	pOldBmp=dcMem.SelectObject(m_pBmpBuff);
//	dcMem.SetViewportOrg(-x,-y);
	dcMem.SetWindowOrg(x,y);

	//背景色で初期化
	if(m_COL.colBG == RGB(255,255,255)){
		dcMem.PatBlt(clipx,clipy,clipw,cliph,WHITENESS);
	}else{
		dcMem.FillSolidRect(clipx,clipy,clipw,cliph,m_COL.colBG);
	}

	CSize size=pDoc->GetSheetSize();				//図面サイズを得る
	BSchPtoLP(&size,m_nVExt,m_nWExt);								//論理論理サイズに変換
	dcMem.IntersectClipRect(0,0,size.cx,size.cy);	//図面範囲外をクリップ指定
	
	DrawSheetGrid(&dcMem,rcClip);
	DrawSheetFrame(&dcMem,DRAW_ON,m_nVExt,m_nWExt,rcClip);
	DWORD dwDrawOption=0;
	if(g_bDisplayPartFrame)dwDrawOption|=DRAW_PARTFRAME;
	if(g_bDisplayNcPinMark)dwDrawOption|=DRAW_NC_MARK;

	DrawMainData(&dcMem,pDoc,DRAW_ON|dwDrawOption,m_nVExt,m_nWExt,rcClip);//通常データの再描画
	if(m_nOnMove & (ON_NAME | ON_NUM)){
		if(m_nOnMove & ON_NAME){
			dwDrawOption=DRAW_INH_PARTNAME;
		}else if(m_nOnMove & ON_NUM){
			dwDrawOption=DRAW_INH_PARTNUM;
		}
		DrawTmpData(&dcMem,pDoc,DRAW_TEMP|dwDrawOption,m_nVExt,m_nWExt,rcClip,TRUE); //一時データの再描画
	}

	
	dcMem.SelectObject(pOldBmp);
//	TRACE("CBSchView::drawBuffer()2\n %d,%d  %d,%d  %d,%d  %d,%d", x,y, w,h, clipx,clipy, clipw,cliph );

	ReleaseDC(pDC);
}


void CBSchView::OnDraw(CDC* pDC)
{
//TRACE("OnDraw Start\n");
	if(m_bSetColor2Active){
		updateBuff(true);
	}else{
		updateBuff(false);
	}
	CBSchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。

	if(!m_pBmpBuff){
		DrawSheetGrid(pDC,m_rcClip);
		DrawSheetFrame(pDC,DRAW_ON,m_nVExt,m_nWExt,m_rcClip);
		DWORD dwDrawOption=0;
		if(g_bDisplayPartFrame)dwDrawOption|=DRAW_PARTFRAME;
		if(g_bDisplayNcPinMark)dwDrawOption|=DRAW_NC_MARK;
		DrawMainData(pDC,pDoc,DRAW_ON|dwDrawOption,m_nVExt,m_nWExt,m_rcClip);//通常データの再描画
	}else{
		CPoint ptLeftTop = GetDeviceScrollPosition();
		int x=ptLeftTop.x;
		int y=ptLeftTop.y;
		
		CRect rcClient;
		GetClientRect(&rcClient);
		int w=rcClient.right;
		int h=rcClient.bottom;
		CBitmap* pOldBmp;
		CDC dcMem; //メモリデバイスコンテキストオブジェクトの作成
		dcMem.CreateCompatibleDC(pDC);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
		pOldBmp=dcMem.SelectObject(m_pBmpBuff);
		dcMem.SetViewportOrg(-x,-y);
		pDC->BitBlt(x,y,w,h,&dcMem,x,y,SRCCOPY);
		dcMem.SelectObject(pOldBmp);
	}

	DWORD dwDrawOption=0;
	if(m_nOnMove & (ON_NAME | ON_NUM)){
		if(m_nOnMove & ON_NAME){
			dwDrawOption=DRAW_INH_PARTNUM | DRAW_INH_PARTBODY;
		}else if(m_nOnMove & ON_NUM){
			dwDrawOption=DRAW_INH_PARTNAME | DRAW_INH_PARTBODY;
		}
	}
	DrawTmpData(pDC,pDoc,DRAW_TEMP|dwDrawOption,m_nVExt,m_nWExt,m_rcClip,TRUE); //一時データの再描画
	pDoc->setupRcTempArea();				//原点の座標を設定
	if(dwDrawOption==0 && pDoc->GetTmpDataCount()>0){
		DrawTmpDataFrame(pDC,pDoc,TRUE);//一時データの枠の再描画
	}
	if(m_bDrawSelectFrame)	
		DrawSelectFrame(pDC,TRUE);		//選択中の枠の再描画
	if(m_bDrawOccasional) 
		DrawOccasional(pDC,TRUE);	//臨時データの描画 

	DrawFindMark(pDC);
	DrawCrossCursor(pDC);
}

void CBSchView::IniReadMarkerLineInitValue()
{
	m_tempMarkerLine.m_width = AfxGetApp()->GetProfileInt(_T("InitValue"),_T("MarkerLineWidth"),SXBSchMarkerLine::DEFAULT_MARKER_WIDTH);
	m_tempMarkerLine.m_nColor = AfxGetApp()->GetProfileInt(_T("InitValue"),_T("MarkerLineColor"),SXBSchMarkerLine::DEFAULT_COLOR);
}

void CBSchView::IniWriteMarkerLineInitValue()
{
	AfxGetApp()->WriteProfileInt(_T("InitValue"),_T("MarkerLineWidth"),m_tempMarkerLine.m_width);
	AfxGetApp()->WriteProfileInt(_T("InitValue"),_T("MarkerLineColor"),m_tempMarkerLine.m_nColor);
}

void CBSchView::IniReadMarkerLineCustomColors(COLORREF* lpColors)
{
	int i;
	CString strKey;
	for(i=0;i<16;i++){
		strKey.Format(_T("Col%d"),i);
		lpColors[i] = AfxGetApp()->GetProfileInt(_T("MarkerCustomValue"),strKey,RGB(255,255,255));
	}
}

void CBSchView::IniWriteMarkerLineCustomColors(COLORREF* lpColors)
{
	int i;
	CString strKey;
	for(i=0;i<16;i++){
		strKey.Format(_T("Col%d"),i);
		AfxGetApp()->WriteProfileInt(_T("MarkerCustomValue"),strKey,lpColors[i]);
	}
}




void CBSchView::OnInitialUpdate()
{
TRACE(_T("CBSchView::OnInitialUpdate()\n"));
	IniReadColor();
	IniReadQuickPropSetting();
	IniReadMarkerLineInitValue();
	m_bCoarseStepZoom = IniReadCoarseStepZoom();
	IniWriteCoarseStepZoom(m_bCoarseStepZoom);
	m_bUseCrossCursor= IniReadUseCrossCursor();
	m_allowAppendSelect = IniReadAllowAppendSelect();
	m_dispBN = IniReadDisplayBlockNum();//ブロック番号の表示情報をレジストリから得る
	m_bDisplayGrid=IniReadDisplayGrid();//グリッドの表示情報をレジストリから得る
	g_bDisplayPartFrame=IniReadDisplayPartFrame();//部品枠の表示情報をレジストリから得る
	g_bDisplayNcPinMark=IniReadDisplayNcPinMark();//NCピンマークの表示情報をレジストリから得る
	g_bDisplayHiddenPinNum=IniReadDisplayHiddenPinNum();//非表示ピン番号の表示情報をレジストリから得る 0.63.01
	m_bClickClickLineMode = IniReadClickClickLineMode();//レジストリからクリッククリックラインモードの情報を得る
	g_JunctionDrawSize = IniReadJunctionSize();	//レジストリからジャンクションの描画サイズを得る
	IniWriteJunctionSize(g_JunctionDrawSize); 	//UIで設定しない項目なので、すぐに書き戻す。新しいBSCH3.INIに項目ができる。

	g_backupLevel= IniReadBackupLevel();		//レジストリからバックアップのレベルを得る
	IniWriteBackupLevel(g_backupLevel);			//すぐに書き戻す

	m_pointUpdateCenter=CPoint(0,0);	//1997/01/26
	CBSchDoc* pDoc = GetDocument();
	if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//コマンドラインから読み込んだファイルに対する処理
	m_fEditHighLight = AfxGetApp()->GetProfileInt(_T("Option"),_T("HighLightEditLayer"),TRUE);

	m_bDrawSelectFrame=FALSE;//XORで描画する選択枠の描画フラグ。
	m_bDrawTmpData=FALSE;//XORで描画する一時データの描画フラグ。
	m_bDrawOccasional =FALSE;//XORで描画する臨時データの描画フラグ。
	m_bDrawTmpDataFrame=FALSE;//XORで描画する一時データの枠の描画フラグ。

//	ScrollToPosition(CPoint(0,0));
	CScrollView::OnInitialUpdate();		// 1997/02/03 に上から移動
	ScrollToPosition(CPoint(0,0));

	//CSize sizeTotal;
	// TODO: このビューのサイズの合計を計算します。
	//sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
}


/////////////////////////////////////////////////////////////////////////////
// CBSchView クラスの診断

#ifdef _DEBUG
void CBSchView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CBSchView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CBSchDoc* CBSchView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBSchDoc)));
	return (CBSchDoc*)m_pDocument;
}
#endif //_DEBUG


//選択の解除
void CBSchView::FixSelection(int option)
{
	if(m_displayQuickPropComp){
		HideQuickPropComp();
	}
	CBSchDoc* pDoc = GetDocument();
	pDoc->FixSelection(option);		//一時データを解除
	m_bEditPointsMode = false;
}
/////////////////////////////////////////////////////////////////////////////
// CBSchView クラスのメッセージ ハンドラ

//ツールを変えたときの共通処理
//・一時データの解放
void CBSchView::ChangeTool(int nOldTool,int nNewTool)
{
	CBSchDoc* pDoc = GetDocument();
	if(pDoc->GetTmpDataCount()){	//一時データが存在していたら...
		//pDoc->FixSelection();		//一時データを解除
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//一時データを解除
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
		updateBuff(true);	// 2005/01/30追加
	}
	
	if(nNewTool==ID_TOOL_SELECTOR){
		if(m_ObjectFileterValue & OBJMASK_USEFILTER){
			if(m_pObjFilterDlg->GetSafeHwnd() == NULL){
				m_pObjFilterDlg->ShowDialog();
			}
		}
	}else{
		if(m_pObjFilterDlg->GetSafeHwnd() != NULL){
			m_pObjFilterDlg->DestroyWindow();
		}
	}






	HideQuickPropComp();
	m_bDrawOccasional = FALSE;
	m_nPlaceSequence = 0;
	Invalidate(FALSE);
}



//選択ツール（ID_TOOL_SELECTOR）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolSelector() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
//	if(m_nCurrentTool==ID_TOOL_SELECTOR) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_SELECTOR);//ツールを変えたときの共通処理
	if(m_nCurrentTool==ID_TOOL_SELECTOR) PostMessage(WM_COMMAND,ID_REFRESH);
	m_nCurrentTool=ID_TOOL_SELECTOR;
}

void CBSchView::OnUpdateToolSelector(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_SELECTOR);
}

//ドラッグツール（ID_TOOL_DRAG）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolDrag() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_DRAG) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_DRAG);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_DRAG;
}

void CBSchView::OnUpdateToolDrag(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_DRAG);
}

//部品配置ツール（ID_TOOL_PART）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolPart() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_PART) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_PART);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_PART;
}

void CBSchView::OnUpdateToolPart(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_PART);
}

//ワイヤーツール（ID_TOOL_WIRE）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolWire() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_WIRE) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_WIRE);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_WIRE;
}

void CBSchView::OnUpdateToolWire(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_WIRE);
}

//バスツール（ID_TOOL_BUS）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolBus() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_BUS) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_BUS);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_BUS;
}

void CBSchView::OnUpdateToolBus(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_BUS);
}

//破線ツール（ID_TOOL_DASH）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolDash() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_DASH) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_DASH);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_DASH;
}

void CBSchView::OnUpdateToolDash(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_DASH);
}

//ジャンクションツール（ID_TOOL_JUNCTION）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolJunction() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_JUNCTION) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_JUNCTION);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_JUNCTION;
	m_pointOccasional=CPoint(-100,-100);
}

void CBSchView::OnUpdateToolJunction(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_JUNCTION);
}

//エントリー ワイヤー 左 配置ツール（ID_TOOL_ENTRY_WL）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolEntryWl() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_ENTRY_WL) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_ENTRY_WL);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_ENTRY_WL;
	m_pointOccasional=CPoint(-100,-100);
}

void CBSchView::OnUpdateToolEntryWl(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(  m_nCurrentTool==ID_TOOL_ENTRY_WL
		             ||m_nCurrentTool==ID_TOOL_ENTRY_WR);
}



//エントリー バス 左 配置ツール（ID_TOOL_ENTRY_BL）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolEntryBl() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_ENTRY_BL) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_ENTRY_BL);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_ENTRY_BL;
	m_pointOccasional=CPoint(-100,-100);
}

void CBSchView::OnUpdateToolEntryBl(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck( m_nCurrentTool==ID_TOOL_ENTRY_BL
		            ||m_nCurrentTool==ID_TOOL_ENTRY_BR);
}



//タグツール（ID_TOOL_TAG）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolTag() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_TAG) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_TAG);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_TAG;
}

void CBSchView::OnUpdateToolTag(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_TAG);
}

//ラベルツール（ID_TOOL_LABEL）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolLabel() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_LABEL) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_LABEL);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_LABEL;
}

void CBSchView::OnUpdateToolLabel(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_LABEL);
}

//コメントツール（ID_TOOL_COMMENT）へのモード切り替え／ボタン状態設定
void CBSchView::OnToolComment() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool==ID_TOOL_COMMENT) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_COMMENT);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_COMMENT;
}

void CBSchView::OnUpdateToolComment(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_COMMENT);
}

//ライブラリの設定の実行
void CBSchView::OnSetLibrary() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSetLib dlg;
	if(dlg.DoModal()==IDOK){
		g_FreeLibrary();
		g_ReadLibrary();
	}
}

//コメントのフォントの選択
void CBSchView::OnSetCommentFont() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSetFont dlg;

	LOGFONT lfComment;
	LOGFONT lfLabel;
	g_IniReadFontInfo(&lfComment);		//レジストリからコメントのフォント情報を得る
	dlg.m_lfComment = lfComment;
	g_IniReadLabelFontInfo(&lfLabel);	//レジストリからラベルのフォント情報を得る
	dlg.m_lfLabel= lfLabel;
	g_IniReadNameFontInfo(&lfLabel);	//レジストリから部品名のフォント情報を得る
	dlg.m_lfName= lfLabel;

	g_IniReadQuickPropFontInfo(&lfLabel);	//レジストリからアトリビュートプレビューのフォント情報を得る
	dlg.m_lfQuickProp= lfLabel;


	if(dlg.DoModal()==IDOK){
		bool updateView = false;
		if(dlg.m_bChangedCommentFont){
			g_IniWriteFontInfo(&dlg.m_lfComment);
			SetDefaultFontSettingsToCommentClass();
		}
		if(dlg.m_bChangedLabelFont){
			g_IniWriteLabelFontInfo(&dlg.m_lfLabel);			
			g_pFont->setLogFont(dlg.m_lfLabel);
			updateView = true;
		}
		if(dlg.m_bChangedNameFont){
			g_IniWriteNameFontInfo(&dlg.m_lfName);			
			g_pFontName->setLogFont(dlg.m_lfName);
			updateView = true;
		}
		if(dlg.m_bChangedQuickPropFont){
			g_IniWriteQuickPropFontInfo(&dlg.m_lfQuickProp);
			m_lfQuickPropFont = dlg.m_lfQuickProp;
			//updateView = true;
		}
		if(updateView){
			CBSchDoc* pDoc = GetDocument();
			pDoc->UpdateAllViews(NULL);
		}

	}
}

//指定した点を中央とするスクロールポジションを得る
CPoint CBSchView::GetSheetLTforScroll(CPoint* ppointCenter)
{
	CBSchDoc* pDoc = GetDocument();
	CSize sizeSheet;
	sizeSheet=pDoc->GetSheetSize();	//図面サイズを得る
	CPoint pointCenter=*ppointCenter;
	CRect rcClient;
	GetClientRect(&rcClient);		//クライアントエリアのサイズを得る
	BSchPtoLP(&pointCenter,m_nVExt,m_nWExt);//論理座標に変換
	BSchPtoLP(&sizeSheet,m_nVExt,m_nWExt);  //論理座標に変換
	CPoint offset;
	offset.x=pointCenter.x-(rcClient.right -rcClient.left)/2;
	offset.y=pointCenter.y-(rcClient.bottom-rcClient.top)/2;
	rcClient.OffsetRect(offset);

	if(rcClient.right>sizeSheet.cx){
		rcClient.left=sizeSheet.cx-(rcClient.right -rcClient.left);
	}
	if(rcClient.left<0)rcClient.left=0;

	if(rcClient.bottom>sizeSheet.cy){
		rcClient.top=sizeSheet.cy-(rcClient.bottom -rcClient.top);
	}
	if(rcClient.top<0)rcClient.top=0;
	return CPoint(rcClient.left,rcClient.top);
}

//クライアントエリア全面がクリアされて呼ばれる
void CBSchView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CBSchDoc* pDoc = GetDocument();
	CSize size;
	CSize sizePage=CSize(200,200);
	CSize sizeLine=CSize(50,50);
TRACE("*****CBSchView::OnUpdate*****\n");
	m_oldOpCount = -1;

	size=pDoc->GetSheetSize();	//図面サイズを得る
	BSchPtoLP(&size,m_nVExt,m_nWExt);			//現状の表示倍率での実際の表示サイズに変換
	SetScrollSizes(MM_TEXT,size,sizePage,sizeLine);
	if(m_bZoomUpdate){
		CPoint pointScrollTo=GetSheetLTforScroll(&m_pointUpdateCenter);
		ScrollToPosition(pointScrollTo);
	}
	CScrollView::OnUpdate(pSender,lHint,pHint);
}


////////////////////////////////////////////////////
//指定位置図面座標へのスクロール
void CBSchView::ScrollToFoundPos(CPoint pt)
{
	CPoint pointScrollTo=GetSheetLTforScroll(&pt);
	ScrollToPosition(pointScrollTo);
	m_ptFindMark = pt;
	m_FindMarkActive = true;
	CBSchDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL);
}

void CBSchView::OnCloseFind2Dialog()
{
	//m_FindMarkActive = false;
	Invalidate();
}


void CBSchView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(m_nZoom>0);
}

void CBSchView::OnZoomIn() 
{
	OnZoomInOutProc(TRUE,FALSE);
}

void CBSchView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(m_nZoom<MAX_ZOOM_STEP_NUM);
}


void CBSchView::OnZoomOut() 
{
	OnZoomInOutProc(FALSE,FALSE);
}


//ズームインアウトの処理
void CBSchView::OnZoomInOutProc(BOOL bZoomIn,BOOL bKey/* =FALSE */)
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(bZoomIn){
		if(m_nZoom<=0) return;
	}else{
		if(m_nZoom>=MAX_ZOOM_STEP_NUM) return;
	}
	CBSchDoc* pDoc = GetDocument();

	//カーソル位置でのズームインアウトのサポート 1999/09/23
	CPoint point;
	CRect  rect;
	CSize size;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	GetClientRect(&rect);
	BOOL fInside= (point.x>=rect.left && point.x<=rect.right
			     && point.y>=rect.top  && point.y<=rect.bottom);
	if(fInside){
		CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
		OnPrepareDC(&dc);	//スクロールビューの反映
		dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
		LPtoBSchP(&point,m_nVExt,m_nWExt);	//マウスの座標を図面座標に変換
		size = pDoc->GetSheetSize();
		fInside =(point.x>=0 && point.x<=size.cx && point.y>=0 && point.y<=size.cy);
	}

	m_bZoomUpdate=TRUE;

	if(fInside && bKey){
		m_pointUpdateCenter=point;	//1999/09/23
	}else{
		m_pointUpdateCenter=GetClientCenter();//クライアントエリアの中央の図面座標
	}
	int zoomstep =(m_bCoarseStepZoom ? 2 : 1);
	if(bZoomIn){
		SetZoom(m_nZoom-zoomstep);
	}else{
		SetZoom(m_nZoom+zoomstep);
	}

	
	pDoc->UpdateAllViews(NULL);
	m_bZoomUpdate=FALSE;

	//ズームインアウトしたときに、カーソルの図面座標を維持する。
	if(fInside && bKey){
		point=m_pointUpdateCenter;
		CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
		OnPrepareDC(&dc);	//スクロールビューの反映
		BSchPtoLP(&point,m_nVExt,m_nWExt);	//マウスの移動先座標を論理座標に変換
		dc.LPtoDP(&point);	//マウスの移動先座標をデバイス座標に変換
		ClientToScreen(&point);	//マウスの移動先座標をスクリーン座標に変換
	}else{
		::GetCursorPos(&point);
	}
	::SetCursorPos(point.x,point.y);//マウスの座標を移動	
}



////////////////////////////////////////////////////////////////////
//図面の更新メニューのハンドラ
//void CBSchView::OnRefresh() 
//{
//	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
//	CBSchDoc* pDoc = GetDocument();
//	pDoc->UpdateAllViews(NULL);
//}


////////////////////////////////////////////////////////////////////
//図面サイズ設定メニューのハンドラ

//イベントハンドラOnSheet640～3000()から呼ばれるプライベート関数
void CBSchView::OnSheetXXX(int n)
{
	CBSchDoc* pDoc = GetDocument();
//	if(pDoc->GetSheetSize()==n) return;
	pDoc->SetSheetSize(n,0,0);		//図面サイズの設定
	pDoc->SetDirty();
	CSize sz = pDoc->GetSheetSize();

	pDoc->IniWriteSheetSize(&sz);		//図面サイズをレジストリに書き込む
	pDoc->UpdateAllViews(NULL);		//Viewを更新する
}


//void CBSchView::OnUpdateSheet640(CCmdUI* pCmdUI) 
//{
//	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
//	CBSchDoc* pDoc = GetDocument();
//	pCmdUI->SetCheck(pDoc->GetSheetSize()==0);
//}

void CBSchView::OnSheet640() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnSheetXXX(0);
}

//void CBSchView::OnUpdateSheet1000(CCmdUI* pCmdUI) 
//{
//	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
//	CBSchDoc* pDoc = GetDocument();
//	pCmdUI->SetCheck(pDoc->GetSheetSize()==1);
//}


void CBSchView::OnSheet1000() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnSheetXXX(1);
}

//void CBSchView::OnUpdateSheet1600(CCmdUI* pCmdUI) 
//{
//	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
//	CBSchDoc* pDoc = GetDocument();
//	pCmdUI->SetCheck(pDoc->GetSheetSize()==2);
//}

void CBSchView::OnSheet1600() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
		OnSheetXXX(2);
}



//void CBSchView::OnUpdateSheet2000(CCmdUI* pCmdUI) 
//{
//	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
//	CBSchDoc* pDoc = GetDocument();
//	pCmdUI->SetCheck(pDoc->GetSheetSize()==3);
//}

void CBSchView::OnSheet2000() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnSheetXXX(3);
}

//void CBSchView::OnUpdateSheet3000(CCmdUI* pCmdUI) 
//{
//	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
//	CBSchDoc* pDoc = GetDocument();
//	pCmdUI->SetCheck(pDoc->GetSheetSize()==4);
//}

void CBSchView::OnSheet3000() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnSheetXXX(4);
}


///////////////////////////////////////////////////////////
//ズーム表示に関わるもの

//ズーム値をセットする
void CBSchView::SetZoom(int  n)
{
//	static int ntableV[5]={5,3,1,1,1};
//	static int ntableW[5]={1,2,1,2,5};
	static int ntableV[9]={5,3,5,3,1,2,2,1,1};
	static int ntableW[9]={1,1,2,2,1,3,5,3,5};
	if(n<0) n=0;
	else if(n>MAX_ZOOM_STEP_NUM) n=MAX_ZOOM_STEP_NUM;
	m_nZoom=n;
	m_nWExt=ntableW[n];
	m_nVExt=ntableV[n];
}

//図面座標を論理座標に変換する(LPPOINT版)
void CBSchView::BSchPtoLP(LPPOINT pp,int nVExt, int nWExt)
{
	pp->x=(pp->x*nVExt)/nWExt;
	pp->y=(pp->y*nVExt)/nWExt;
}
//図面座標を論理座標に変換する(LPSIZE版)
void CBSchView::BSchPtoLP(LPSIZE psize,int nVExt, int nWExt)
{
	psize->cx=(psize->cx*nVExt)/nWExt;
	psize->cy=(psize->cy*nVExt)/nWExt;
}
//図面座標を論理座標に変換する(LPRECT版)
void CBSchView::BSchPtoLP(LPRECT prc,int nVExt, int nWExt)
{
	prc->left   =(prc->left   *nVExt)/nWExt;
	prc->top    =(prc->top    *nVExt)/nWExt;
	prc->right  =(prc->right  *nVExt)/nWExt;
	prc->bottom =(prc->bottom *nVExt)/nWExt;
}

//論理座標を図面座標に変換する(LPPOINT版)
void CBSchView::LPtoBSchP(LPPOINT pp,int nVExt, int nWExt)
{
	pp->x=(pp->x*nWExt)/nVExt;
	pp->y=(pp->y*nWExt)/nVExt;
}

//論理座標を図面座標に変換する(LPSIZE版)
void CBSchView::LPtoBSchP(LPSIZE psize,int nVExt, int nWExt)
{
	psize->cx=(psize->cx*nWExt)/nVExt;
	psize->cy=(psize->cy*nWExt)/nVExt;
}

//論理座標を図面座標に変換する(LPRECT版)
void CBSchView::LPtoBSchP(LPRECT prc,int nVExt, int nWExt)
{
	prc->left   =(prc->left   *nWExt)/nVExt;
	prc->top    =(prc->top    *nWExt)/nVExt;
	prc->right  =(prc->right  *nWExt)/nVExt;
	prc->bottom =(prc->bottom *nWExt)/nVExt;
}

//グリッドへのスナップ
void CBSchView::SnapToGrid(LPPOINT pp)
{
	if(m_bSnaptoGrid){
		pp->x=((pp->x+4)/10)*10;
		pp->y=((pp->y+4)/10)*10;
	}
}

///////////////////////////////////////////////////////
//レジストリ関連
void  CBSchView::IniReadFindInfo(FindInfo& fi)
{
	fi.m_str			= AfxGetApp()->GetProfileString(_T("Find"),_T("String"));
	fi.m_targetFlag		= AfxGetApp()->GetProfileInt(_T("Find"),_T("TargetFlag"),0xffffffff);
	fi.m_bMatchWhole	= AfxGetApp()->GetProfileInt(_T("Find"),_T("MatchWhole"),1);
	fi.m_bCaseSensitive = AfxGetApp()->GetProfileInt(_T("Find"),_T("CaseSensitive"),1);
}

void  CBSchView::IniWriteFindInfo(FindInfo& fi)
{
	AfxGetApp()->WriteProfileString(_T("Find"),_T("String"),fi.m_str);
	AfxGetApp()->WriteProfileInt(_T("Find"),_T("TargetFlag"),fi.m_targetFlag);
	AfxGetApp()->WriteProfileInt(_T("Find"),_T("MatchWhole"),fi.m_bMatchWhole);
	AfxGetApp()->WriteProfileInt(_T("Find"),_T("CaseSensitive"),fi.m_bCaseSensitive);
}


void  CBSchView::IniReadColor()
{
	m_COL.colBG			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("BG"),COL_BG));
	m_COL.colJUNC			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Junction"),COL_JUNC));
	m_COL.colWIRE			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Wire"),COL_WIRE));
	m_COL.colBUS			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Bus"),COL_BUS));
	m_COL.colDASH			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Dash"),COL_DASH));
	m_COL.colTAG			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Tag"),COL_TAG));
	m_COL.colTAG_NAME		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("TagName"),COL_TAG_NAME));
	m_COL.colCOMMENT		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Comment"),COL_COMMENT));
	m_COL.colLABEL			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Label"),COL_LABEL));
	m_COL.colXOR			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Select"),COL_XOR));
	m_COL.colPART			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Component"),COL_PART));
	m_COL.colPART_PIN		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("ComponentPin"),COL_PART_PIN));
	m_COL.colPART_PINNAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("PinName"),COL_PART_PINNAME));
	m_COL.colPART_PINNUM	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("PinNum"),COL_PART_PINNUM));
	m_COL.colPART_NAME		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("ComponentName"),COL_PART_NAME));
	m_COL.colPART_NUM		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("ComponentNum"),COL_PART_NUM));
	m_COL.colPART_FRAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("ComponentFrame"),COL_PART_FRAME));
	m_COL.colSHEET_FRAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("SheetFrame"),COL_SHEET_FRAME));
	m_COL.colSHEET_GRID	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("Grid"),COL_SHEET_GRID));
	m_COL.colDARK			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("InactiveLayer"),COL_DARK));
	m_COL.colZLPIN			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("ZeroLengthPin"),COL_ZLPIN));
	m_COL.colNC_MARK		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("NcMark"),COL_NC_MARK));
	m_COL.colQUICKPROPVIEW =(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("QuickPropView"),COL_QUICKPROPVIEW));
	m_COL.colTEMPFRAME_FOCUSED		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("TempFrameFocused"),COL_TEMPFRAME_FOCUSED));
	m_COL.colTEMPFRAME_NOT_FOCUSED	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("TempFrameNotFocused"),COL_TEMPFRAME_NOT_FOCUSED));
	m_COL.colFIND_MARK =	(COLORREF)(AfxGetApp()->GetProfileInt(_T("Color"),_T("FindMark"),COL_FIND_MARK));
}
void  CBSchView::IniWriteColor()
{
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("BG"),m_COL.colBG);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Junction"),m_COL.colJUNC);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Wire"),m_COL.colWIRE);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Bus"),m_COL.colBUS);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Dash"),m_COL.colDASH);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Tag"),m_COL.colTAG);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("TagName"),m_COL.colTAG_NAME);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Comment"),m_COL.colCOMMENT);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Label"),m_COL.colLABEL);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Select"),m_COL.colXOR);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Component"),m_COL.colPART);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("ComponentPin"),m_COL.colPART_PIN);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("PinName"),m_COL.colPART_PINNAME);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("PinNum"),m_COL.colPART_PINNUM);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("ComponentName"),m_COL.colPART_NAME);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("ComponentNum"),m_COL.colPART_NUM);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("ComponentFrame"),m_COL.colPART_FRAME);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("SheetFrame"),m_COL.colSHEET_FRAME);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("Grid"),m_COL.colSHEET_GRID);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("InactiveLayer"),m_COL.colDARK);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("ZeroLengthPin"),m_COL.colZLPIN);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("NcMark"),m_COL.colNC_MARK);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("QuickPropView"),m_COL.colQUICKPROPVIEW);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("TempFrameFocused"),m_COL.colTEMPFRAME_FOCUSED);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("TempFrameNotFocused"),m_COL.colTEMPFRAME_NOT_FOCUSED);
	AfxGetApp()->WriteProfileInt(_T("Color"),_T("FindMark"),m_COL.colFIND_MARK);

}

//レジストリから粗いステップのズームを使うかどうかの設定を読み込む
BOOL CBSchView::IniReadCoarseStepZoom()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("CoarseStepZoom"),FALSE);
}

//レジストリに粗いステップのズームを使うかどうかの設定を書き込む
void CBSchView::IniWriteCoarseStepZoom(BOOL bCoarse)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("CoarseStepZoom"),bCoarse);
}


//レジストリからクロスカーソル表示の情報を得る
BOOL CBSchView::IniReadUseCrossCursor()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("UseCrossCursor"),FALSE);
}

//レジストリにクロスカーソル表示の情報を書き込む
void CBSchView::IniWriteUseCrossCursor(BOOL bDisplay)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("UseCrossCursor"),bDisplay);
}

//0.82.00 2014/04/27
//レジストリからブロック番号表示の情報を得る
DISP_BLOCKNUM_TYPE CBSchView:: IniReadDisplayBlockNum()
{
	DISP_BLOCKNUM_TYPE dbnt;
	CString str = AfxGetApp()->GetProfileString(_T("Option"),_T("DisplayBlockNum"),_T("NOT"));
	if(_tcsncmp(str,_T("NUM"),3)==0){
		dbnt = DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_NUM;
	}else if(_tcsncmp(str,_T("ALP"),3)==0){
		dbnt = DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_ALPHA;
	}else{
		dbnt = DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM;
	}

	return dbnt;
}

//レジストリにブロック番号表示の情報を書き込む
void CBSchView::IniWriteDisplayBlockNum(DISP_BLOCKNUM_TYPE dbnt)
{
	CString str=_T("NOT");
	if(dbnt ==  DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_NUM){
		str=_T("NUM");
	}else if(dbnt ==  DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_ALPHA){
		str=_T("ALP");
	}
	AfxGetApp()->WriteProfileString(_T("Option"),_T("DisplayBlockNum"),str);	
}



//レジストリからグリッド表示の情報を得る
BOOL CBSchView::IniReadDisplayGrid()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("DisplayGrid"),TRUE);
}

//レジストリにグリッド表示の情報を書き込む
void CBSchView::IniWriteDisplayGrid(BOOL bDisplay)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("DisplayGrid"),bDisplay);
}

//レジストリから部品枠表示の情報を得る
BOOL CBSchView::IniReadDisplayPartFrame()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("DisplayPartFrame"),FALSE);
}

//レジストリに部品枠表示の情報を書き込む
void CBSchView::IniWriteDisplayPartFrame(BOOL bDisplay)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("DisplayPartFrame"),bDisplay);
}

//レジストリからNCピンマーク表示の情報を得る
BOOL CBSchView::IniReadDisplayNcPinMark()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("DisplayNcPinMark"),FALSE);
}

//レジストリにNCピンマーク表示の情報を書き込む
void CBSchView::IniWriteDisplayNcPinMark(BOOL bDisplay)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("DisplayNcPinMark"),bDisplay);
}


//レジストリから非表示ピン番号表示の情報を得る
BOOL CBSchView::IniReadDisplayHiddenPinNum()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("DisplayHiddenPinNum"),FALSE);
}

//レジストリに非表示ピン番号表示の情報を書き込む
void CBSchView::IniWriteDisplayHiddenPinNum(BOOL bDisplay)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("DisplayHiddenPinNum"),bDisplay);
}



//レジストリから部品プレビューの情報を得る
BOOL CBSchView::IniReadUsePreview()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("UsePreview"),TRUE);
}

//レジストリに部品プレビューの情報を書き込む
void CBSchView::IniWriteUsePreview(BOOL bUsePreview)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("UsePreview"),bUsePreview);
}

//レジストリから印刷ページフィット情報の読み込み
BOOL  CBSchView::IniReadPrintPageFit()
{
	return (AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPageFit"),0) != 0);
}

//レジストリに印刷ページフィット情報の書き込み
void CBSchView::IniWritePrintPageFit(BOOL pagefit)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("PrintPageFit"),(pagefit ? 1 : 0));
}

//レジストリから画像出力倍率の読み込み 2016/05/03
int  CBSchView::IniReadExpImageMag()
{
	int res= AfxGetApp()->GetProfileInt(_T("Option"),_T("ExpImageMag"),100);
	if(res<100) return 100;
	if(res>200) return 200;
	return res;
}

//レジストリに画像出力倍率の書き込み 2016/05/03
void CBSchView::IniWriteExpImageMag(int nMag)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("ExpImageMag"),nMag);
}


//レジストリから印刷解像度の読み込み
int  CBSchView::IniReadPrintRes()
{
	int res= AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintRes"),100);
	if(res<30) return 30;
	if(res>300) return 300;
	return res;
}

//レジストリに印刷解像度の書き込み
void CBSchView::IniWritePrintRes(int nRes)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("PrintRes"),nRes);
}

//レジストリから印刷マージンの読み込み
void CBSchView::IniReadPrintMargin(int& left,int& upper,int& right,int& lower)
{
	left=AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintMarginLeft"),10);
	upper=AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintMarginUpper"),10);
	right=AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintMarginRight"),10);
	lower=AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintMarginLower"),10);
}

//レジストリに印刷マージンの書き込み
void CBSchView::IniWritePrintMargin(int left,int upper,int right,int lower)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("PrintMarginLeft"),left);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("PrintMarginUpper"),upper);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("PrintMarginRight"),right);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("PrintMarginLower"),lower);
}

//レジストリからクリッククリックラインモードの情報を得る
BOOL CBSchView::IniReadClickClickLineMode()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("ClickClickLineMode"),FALSE);
}

//レジストリにクリッククリックラインモードの情報を書き込む
void CBSchView::IniWriteClickClickLineMode(BOOL bClickClick)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("ClickClickLineMode"),bClickClick);
}

//Get drawsize of JUNCTION from the registry
unsigned CBSchView::IniReadJunctionSize()
{
	unsigned n = AfxGetApp()->GetProfileInt(_T("Option"), _T("JunctionSize"), 2);
	if (n < 1)n = 1;
	else if (n > 4)n = 4;
	return n;
}

//Set drawsize of JUNCTION to the registry
void CBSchView::IniWriteJunctionSize(unsigned junctionSize)
{
	AfxGetApp()->WriteProfileInt(_T("Option"), _T("JunctionSize"), junctionSize);
}


//Get backup level from the registry
unsigned CBSchView::IniReadBackupLevel()
{
	unsigned n = AfxGetApp()->GetProfileInt(_T("Option"), _T("BackupLevel"), 0);
	if (n > MAX_BACKUP_LEVEL) n = MAX_BACKUP_LEVEL;
	return n;
}

//Set backup level to the registry
void CBSchView::IniWriteBackupLevel(unsigned n)
{
	AfxGetApp()->WriteProfileInt(_T("Option"), _T("BackupLevel"), n
	);
}

//////////////////////////////////////////////////////////////////////
//グリッド表示に関わるもの
void CBSchView::OnUpdateDispGrid(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_bDisplayGrid);
}
void CBSchView::OnDispGrid() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_bDisplayGrid=(m_bDisplayGrid==FALSE);	//グリッド表示フラグをトグル
	IniWriteDisplayGrid(m_bDisplayGrid);	//レジストリにグリッド表示フラグを書き込む
	CBSchDoc* pDoc = GetDocument();			//ドキュメントのポインタを得て
	pDoc->UpdateAllViews(NULL);				// 画面を再描画
}

//////////////////////////////////////////////////////////////////////
//部品枠表示の設定に関わるもの
void CBSchView::OnUpdatePartFrame(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(g_bDisplayPartFrame);
}

void CBSchView::OnPartFrame() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	g_bDisplayPartFrame=(g_bDisplayPartFrame==FALSE);	//部品枠の表示フラグをトグル
	IniWriteDisplayPartFrame(g_bDisplayPartFrame);		//レジストリに部品枠の表示フラグを書き込む
	CBSchDoc* pDoc = GetDocument();			//ドキュメントのポインタを得て
	pDoc->UpdateAllViews(NULL);				// 画面を再描画
}

//デバイスコンテキストの準備
//・スクロールビューの OnPrepareDC()を呼び出してマッピングモードとスクロール位置の設定
//・図面サイズをもとに図面範囲外を描画禁止にする。
//・プライベート変数(m_rcClip)に描画可能な範囲を設定
void CBSchView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CBSchDoc* pDoc = GetDocument();			//ドキュメントのポインタを得て
	CSize size=pDoc->GetSheetSize();				//図面サイズを得る
	if(pDC->IsPrinting()){
		pDC->SetMapMode(MM_TEXT);
	}else{
		CScrollView::OnPrepareDC(pDC, pInfo);
		pDC->GetClipBox(&m_rcClip);				//もともとのクリップ領域と重なった領域を得る
		LPtoBSchP(&m_rcClip,m_nVExt,m_nWExt);					//図面座標に変換
		if(m_rcClip.Height()<=0) m_rcClip.bottom = m_rcClip.top+1;
		if(m_rcClip.Width()<=0) m_rcClip.right = m_rcClip.left+1;
	}
}

////////////////////////////////////////////////////////////////
//部品選択時のプレビューを使うかどうかの設定
//void CBSchView::OnUpdateUsePreview(CCmdUI* pCmdUI) 
//{
//	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
//	pCmdUI->SetCheck(m_bUsePreview);
//	
//}
//void CBSchView::OnUsePreview() 
//{
//	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
//	m_bUsePreview=(m_bUsePreview==FALSE);
//	IniWriteUsePreview(m_bUsePreview);
//}


	

////////////////////////////////////////////////////////////////
//グリッドへのスナップの設定
void CBSchView::OnUpdateSnapGrid(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_bSnaptoGrid);
}

void CBSchView::OnSnapGrid() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_bSnaptoGrid=(m_bSnaptoGrid==FALSE);
	
}


//////////////////////////////////////////////////
//自動スクロールにタイマーを使う。
void CBSchView::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CPoint point;
	KillTimer(nIDEvent);
	switch(nIDEvent){
	case ID_TIMER_AUTOSCROLL:
		{
			m_bTimer2Active=FALSE;
			GetCursorPos(&point);		//スクリーン座標系でのマウスの位置を知る
			ScreenToClient(&point);		//それをクライアント座標系に変換する
			PostMessage(WM_MOUSEMOVE,0,MAKELONG(point.x,point.y));
			break;
		}
	case ID_TIMER_RBUTTONDOWN:
		{
			m_nRButtonMode = 1;
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CUR_SCRL));
			break;
		}
	default:
		{
			TRACE(_T("\nUnknown timer ID  CBschView::OnTimer()"));
		}
	}
	//CScrollView::OnTimer(nIDEvent);
}



//////////////////////////////////////////////////////////
//マウスが移動したときの処理
//引数 nFlags は使わない。プライベート関数のAutoScrollが発生する WM_MOUSEMOVE
//の nFlags は常に０。1996/12/01
//
void CBSchView::OnMouseMove(UINT nFlags, CPoint point) 
{

	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(m_quickPropSetting.isEnabled() && m_quickPropSetting.m_showOnMouseHover && !m_displayQuickPropComp){
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 250;
		TrackMouseEvent(&tme);
	}



	bool bUpdateScreen = false;

	UINT fScroll;		//自動スクロールフラグ
	CSize sizeMouseOffset;

	if(m_bTimer2Active) return;//自動スクロールのタイマー待ちのときは何もしない(1997/01/02)


	m_ptMouseDP=point;	//マウスのデバイス座標の保存
	
	if(m_displayQuickPropComp){
		if(abs(m_ptMouseDP.x-m_ptDisplayQuickPropAt.x)>10 || abs(m_ptMouseDP.y-m_ptDisplayQuickPropAt.y)>10){
			HideQuickPropComp();
		}
	}


	//デバイスコンテキストの準備
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	OnPrepareDC(&dc);	//スクロールビューの反映
	dc.DPtoLP(&point);	//マウスの座標を論理座標に変換

	DrawCrossCursor(&dc);

	
	m_ptMouse=point;
	LPtoBSchP(&m_ptMouse,m_nVExt,m_nWExt);		//マウスの座標を図面座標に変換
	DisplayCurrentXY();

	CBSchDoc* pDoc = GetDocument();

	/////////////////////////////////////////////////////////////////////////
	//マウスカーソルの形状の決定
	//IDC_CUR_DRAG,IDC_CUR_NORMAL_DRAG,IDC_CUR_PLACEを追加 1997/01/27
	HCURSOR hCursor;
	int nIdCursor;


	//カーソルが一時データの上か判定するためのフラグの準備
	int tempDataFlags;
	int tempDataOption;
	if(m_bEditPointsMode){
		tempDataFlags = ON_NODE | ON_CTRLPOINT;
		tempDataOption = SXBSchDoc::EDITPOINTS_MODE;
	}else{
		tempDataFlags = ON_OBJ | ON_NAME | ON_NUM;
		tempDataOption =  0;
	}


	if(m_nRButtonMode==1){
		nIdCursor=IDC_CUR_SCRL;
	}else if(pDoc->IsOnTmpData(&m_ptMouse,tempDataOption,NULL,NULL)&tempDataFlags || m_nOnMove){	//一時データの上か移動中
		if(m_bUseCrossCursor){
			nIdCursor=IDC_CUR_CC_MOVE;		//クロスカーソル用[MOVE]マーク付き矢印
		}else{
			nIdCursor=IDC_CUR_MOVE;			//[MOVE]マーク付き矢印
		}
	}else{			//それ以外
		if(m_bUseCrossCursor){
			//nIdCursor=NULL;				//カーソルなし NULL指定するとDebugモードでassertする。
			nIdCursor=IDC_CURSOR_NULL;		//カーソルなし

		}else{
			nIdCursor=IDC_CUR_NORMAL;		//普通の矢印
		}
	}
	hCursor=AfxGetApp()->LoadCursor(nIdCursor);
	::SetCursor(hCursor);
	///////////////////////////////////////////////////////////////////////////


	if(m_nRButtonMode==1){
		//m_ptRBScrlStartPosition
		CPoint currentScrlPosition = GetDeviceScrollPosition();
		CSize moveSize = (m_ptRBScrlStartMouse-m_ptMouseDP)-(currentScrlPosition-m_ptRBScrlStartPosition);
		OnScrollBy(moveSize, TRUE);
		//return;
	}
	if(m_nOnMove & (ON_NODE|ON_CTRLPOINT)){	//制御点の移動中

		SXBSchObj* pMovePointObj = pDoc->GetTmp();
		if(!pMovePointObj) return;
		if(pMovePointObj->id()==SXBSchObj::ID_DASH){

			sizeMouseOffset=m_ptMouse-m_ptMoveFrom;							//移動量の計算
			SPoint sptOldPos = pMovePointObj->pointIndex(m_nRetOptionOnTempData);
			
			CPoint ptOldPos = CPoint(sptOldPos.x(),sptOldPos.y());
			CPoint ptNewPos=ptOldPos+sizeMouseOffset;						//新しい端点位置の候補
			if(m_nOnMove & ON_NODE){
				SnapToGrid(&ptNewPos);											//新しい端点位置の候補を必要な場合にグリッドへスナップ
			}
			if(ptOldPos!=ptNewPos){
				CSize sizeRealOffset=ptNewPos-ptOldPos;
				SPoint sptRealNewPos = sptOldPos;
				sptRealNewPos+=SPoint(sizeRealOffset.cx,sizeRealOffset.cy);
				pMovePointObj->setPointIndex(m_nRetOptionOnTempData,sptRealNewPos);
				pDoc->SetTempDirty();										//一時データのダーティフラグを立てる
				bUpdateScreen = true;
				m_bTimerActive=FALSE;										//タイマー不使用のダメ押し設定
				m_ptMoveFrom.Offset(sizeRealOffset);						//移動元の移動
			}

		}
	}else if(m_nOnMove & ON_NAME){	//部品名の移動中
		SXBSchComponent* pPart=(SXBSchComponent*)pDoc->GetTmp();		//対象部品のポインタを得る
		if(!pPart->GetHideVal()){
			sizeMouseOffset=m_ptMouse-m_ptMoveFrom;							//移動量の計算
			if(pPart->moveNamePos(sizeMouseOffset.cx,sizeMouseOffset.cy)){		//移動
				pDoc->SetTempDirty();											//一時データのダーティフラグを立てる
				m_ptMoveFrom=m_ptMouse;											//移動元の移動
				bUpdateScreen = true;
			}
		}
		//		InvalidateRect(NULL);
	}else if(m_nOnMove & ON_NUM){//部品番号の移動中
		SXBSchComponent* pPart=(SXBSchComponent*)pDoc->GetTmp();		//対象部品のポインタを得る
		if(!pPart->GetHideNum()){
			sizeMouseOffset=m_ptMouse-m_ptMoveFrom;							//移動量の計算
			if(pPart->moveRefnumPos(sizeMouseOffset.cx,sizeMouseOffset.cy)){	//移動
				pDoc->SetTempDirty();											//一時データのダーティフラグを立てる
				m_ptMoveFrom=m_ptMouse;											//移動元の移動
				bUpdateScreen = true;
			}
		}
		//		InvalidateRect(NULL);
	}else if(m_nOnMove & ON_OBJ){
		CPoint ptOldOrg=pDoc->GetTmpOrigin();							//一時データの原点
		sizeMouseOffset=m_ptMouse-m_ptMoveFrom;							//図面座標上でのマウスの移動量
		CPoint ptNewOrg=ptOldOrg+sizeMouseOffset;						//新しい一時データの原点の候補
		SnapToGrid(&ptNewOrg);											//新しい一時データの原点の候補を必要な場合にグリッドへスナップ
		if(ptOldOrg!=ptNewOrg){											//有効な移動があったものと見做す
			CSize sizeRealOffset=ptNewOrg-ptOldOrg;						//実際の移動量の計算
			pDoc->Shift(&sizeRealOffset);								//一時データの座標の移動
			m_bTimerActive=FALSE;							//タイマー不使用のダメ押し設定
			m_ptMoveFrom.Offset(sizeRealOffset);						//移動元の移動
			bUpdateScreen = true;
//			InvalidateRect(NULL);
		}
	//選択操作中
	}else if(m_bOnSelect){
		m_ptMoveTo=m_ptMouse;
		m_bDrawSelectFrame = TRUE;
		bUpdateScreen = true;
//		InvalidateRect(NULL);

	}else if(m_nCurrentTool==ID_TOOL_WIRE
		|| m_nCurrentTool==ID_TOOL_BUS
		|| m_nCurrentTool==ID_TOOL_DASH
		|| m_nCurrentTool==ID_TOOL_MARKER){

		if(m_nPlaceSequence!=0){
		//if(m_bCaptureMouse!=FALSE){
			//ワイヤー・バス配置作業中
			if(m_nCurrentTool==ID_TOOL_WIRE || m_nCurrentTool==ID_TOOL_BUS){
				CPoint point=m_ptMouse;
				SnapToGrid(&point);
				if(point != m_ptMoveTo){
					m_ptMoveTo = point;
					int cx=abs(m_ptMoveTo.x-m_ptMoveFrom.x);
					int cy=abs(m_ptMoveTo.y-m_ptMoveFrom.y);
					if(m_bSetWireDirection){
						if(cx<10 && cy<10)m_bSetWireDirection=FALSE;
					}
					if(!m_bSetWireDirection){
						if(cx>cy)m_bWireHorizon1st=TRUE; else m_bWireHorizon1st=FALSE;
						if(cx>=10 || cy>=10)m_bSetWireDirection=TRUE;
					}
					m_bDrawOccasional=TRUE;
					bUpdateScreen = true;
				}
			//破線・マーカー配置作業中
			}else if(m_nCurrentTool==ID_TOOL_DASH || m_nCurrentTool==ID_TOOL_MARKER){
				CPoint point=m_ptMouse;
				SnapToGrid(&point);
				if(point != m_ptMoveTo){
					m_ptMoveTo = point;
					m_bDrawOccasional=TRUE;
					bUpdateScreen = true;
				}
			
			}
		}
	//エントリー配置作業中
	}else if(   m_nCurrentTool==ID_TOOL_ENTRY_BL 
             || m_nCurrentTool==ID_TOOL_ENTRY_BR
			 || m_nCurrentTool==ID_TOOL_ENTRY_WL
			 || m_nCurrentTool==ID_TOOL_ENTRY_WR ){
		CPoint point=m_ptMouse;
		SnapToGrid(&point);
		if(point != m_pointOccasional){
			m_pointOccasional = point;
			m_bDrawOccasional = TRUE;
			bUpdateScreen = true;
//			InvalidateRect(NULL);
		}
	//ジャンクション配置作業中
	}else if(m_nCurrentTool==ID_TOOL_JUNCTION){
		CPoint point=m_ptMouse;
		SnapToGrid(&point);
		if(point != m_pointOccasional){
			m_pointOccasional = point;
			m_bDrawOccasional = TRUE;
			bUpdateScreen = true;
//			InvalidateRect(NULL);
		}
	}

	if(bUpdateScreen){
		InvalidateRect(NULL);
	}else{
		DrawCrossCursor(&dc);
	}

//	InvalidateRect(NULL);
	//キャプチャ中は自動スクロールを行う可能性あり。
//20100521	if(m_bCaptureMouse){			//キャプチャ中または線データ配置中
	if(m_mouseCaptureInfo.isLButtonCapture()){			//キャプチャ中または線データ配置中
		TRACE(_T("OnMouseMove()-1\n"));
		int nVKey;
		if(::GetSystemMetrics(SM_SWAPBUTTON))		//左右ボタンの入れ換えをチェック
			nVKey=VK_RBUTTON; else nVKey=VK_LBUTTON;

		if(::GetAsyncKeyState(nVKey)>=0 ) return;	//ボタンが押されていなければ自動スクロールしない

		if(!m_bClickClickLineMode){
			//if(::GetAsyncKeyState(nVKey)>=0 && ::GetAsyncKeyState(VK_SHIFT)>=0) return;	//ボタンが押されていなければ自動スクロールしない 
			//if(::GetAsyncKeyState(nVKey)>=0 ) return;	//ボタンが押されていなければ自動スクロールしない 
			if((fScroll=QueryAutoScroll(&dc))!=0) AutoScroll(&dc,fScroll);//必要な場合に自動スクロールする
		}else{
			if(   m_nCurrentTool==ID_TOOL_SELECTOR 
				|| m_nCurrentTool==ID_TOOL_DRAG ){
				if((fScroll=QueryAutoScroll(&dc))!=0){
					AutoScroll(&dc,fScroll);//必要な場合に自動スクロールする
				}
			}
		}
	}
	//CScrollView::OnMouseMove(nFlags, point);
}

//m_ptMouseの値をツールバーに反映する。
//Inside VisualC++ 4.0 （アスキー）参照
void CBSchView::DisplayCurrentXY()
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SetStatusBarXY(&m_ptMouse);
}


//////////////////////////////////////////////////////////
//左ボタン押下時の処理
//m_nCurrentToolが 
//	ID_TOOL_WIRE
//	ID_TOOL_BUS
//	ID_TOOL_DASH
//	ID_TOOL_SELECTOR
//	ID_TOOL_DRAG
// のときにはマウスをキャプチャする

void CBSchView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_FindMarkActive){
		if(!m_pFind2Dlg || !m_pFind2Dlg->GetSafeHwnd()){
			m_FindMarkActive = false;
			Invalidate();
		}
	}


	if(!m_bClickClickLineMode){
//20100521		if(m_bCaptureMouse||m_bRBCaptureMouse) return;
		if(m_mouseCaptureInfo.isLButtonCapture()||m_mouseCaptureInfo.isRButtonCapture()) return;
	}else{
//20100521		if(m_bRBCaptureMouse) return;
		if(m_mouseCaptureInfo.isRButtonCapture()) return;
	}

	TRACE(_T("CBSchView::OnLButtonDown()\n"));

	//if(m_fShiftKeyDown) return;
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	BOOL bFixTmp=FALSE;//一時データをFIXしたらTRUE
	m_ptMouseDP=point;	//マウスのデバイス座標の保存
	//デバイスコンテキストの準備
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	OnPrepareDC(&dc);	//スクロールビューの反映
	dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
	m_ptMouse=point;
	LPtoBSchP(&m_ptMouse,m_nVExt,m_nWExt);		//マウスの座標を図面座標に変換
	DisplayCurrentXY();

	CBSchDoc* pDoc = GetDocument();
	CSize sizeSheet;
	sizeSheet=pDoc->GetSheetSize();		//図面サイズを得る

	
	m_nOnMove=0;
	if(pDoc->GetTmpDataCount()){	//一時データが存在していたら...

		//カーソルが一時データの上か判定するためのフラグの準備
		int tempDataOption;
		if(m_bEditPointsMode){
			tempDataOption = SXBSchDoc::EDITPOINTS_MODE;
		}else{
			tempDataOption =  0;
		}

		if((m_nCurrentTool==ID_TOOL_SELECTOR || m_nCurrentTool==ID_TOOL_DRAG) && (nFlags & MK_CONTROL) && m_allowAppendSelect){

		}else{

			//カーソルの位置が一時データの上かどうかをチェックして
			m_nOnMove=pDoc->IsOnTmpData(&m_ptMouse,tempDataOption,&m_pObjDetectedOnTempData,&m_nRetOptionOnTempData);
			if(m_bEditPointsMode){
				m_nOnMove &= (ON_NODE|ON_CTRLPOINT);
			}
			if(m_nOnMove==0 ){//上でなければ確定を行う
				//確定の手続き
				//pDoc->FixSelection();		//一時データを解除
				FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//一時データを解除
				if(g_bDisplayNcPinMark) pDoc->setNcPinFlag(); //NCピンのフラグの設定
				updateBuff(true);			// 2005/01/30追加
				m_bDrawTmpDataFrame=FALSE;
				InvalidateRect(NULL);
				bFixTmp=TRUE;	//ボタン押下時に一時データをFIXしたフラグを立てる
			}else{

				if(m_nOnMove & (ON_NAME | ON_NUM)){
					if(m_nOnMove & ON_NAME){
						m_nFocusedTo = FOCUSED_TO_VAL;
					}else if(m_nOnMove & ON_NUM){
						m_nFocusedTo = FOCUSED_TO_NUM;
					}
					updateBuff(true);
				}else{
					m_nFocusedTo = FOCUSED_TO_BODY;
					if((nFlags & MK_CONTROL) 	//2005/04/09
						 && !m_allowAppendSelect){	//2009/05/10 Ctrlキーによる追加選択を許容するときは、Ctrlキー押下で複製しない。
						pDoc->Copy();
						OnEditPaste();
						m_nOnMove = ON_OBJ;
					}
					updateBuff(true);
				}

			}
		}
	}
	
	if(	   m_nCurrentTool==ID_TOOL_WIRE
		|| m_nCurrentTool==ID_TOOL_BUS
		|| m_nCurrentTool==ID_TOOL_DASH
		|| m_nCurrentTool==ID_TOOL_MARKER){
		
		if(!m_bClickClickLineMode){
//20100521			SetCapture();				//CWndのメンバー関数(API関数ではない)
//20100521			m_bCaptureMouse=TRUE;		//キャプチャしていることを示すフラグを立てる
			m_mouseCaptureInfo.SetCapture(MouseCaptureInfo::MCI_LEFT);

			m_ptMoveFrom=m_ptMouse;		//キャプチャの始点をセット
			m_ptMoveFrom=m_ptMoveTo=m_ptMouse;

			SnapToGrid(&m_ptMoveFrom);
			SnapToGrid(&m_ptMoveTo);
			m_bSetWireDirection=FALSE;
			m_nPlaceSequence = 1;
		}else{
			//クリック-クリックで線データ配置
			if(m_nPlaceSequence==0){
				//2010/05/19 SetCapture();				//CWndのメンバー関数(API関数ではない)
				m_ptMoveFrom=m_ptMouse;		//始点をセット
				//2010/05/19 m_bCaptureMouse=TRUE;		//キャプチャしていることを示すフラグを立てる
				m_ptMoveFrom=m_ptMoveTo=m_ptMouse;

				SnapToGrid(&m_ptMoveFrom);
				SnapToGrid(&m_ptMoveTo);
				m_bSetWireDirection=FALSE;
				m_nPlaceSequence = 1;
			}else{
				if(m_bTimer2Active){
					KillTimer(ID_TIMER_AUTOSCROLL);
					m_bTimer2Active=FALSE;
				}
				//2010/05/19 ::ReleaseCapture();		//キャプチャを解放
				//2010/05/19 m_bCaptureMouse=FALSE;	//キャプチャしていることを示すフラグをクリアする

				if(m_nCurrentTool==ID_TOOL_WIRE){
					PlaceWire(pDoc);
				}else if(m_nCurrentTool==ID_TOOL_BUS){
					PlaceBus(pDoc);
				}else if(m_nCurrentTool==ID_TOOL_DASH){
					PlaceDash(pDoc);
				}else if(m_nCurrentTool==ID_TOOL_MARKER){
					PlaceMarkerLine(pDoc);
				}
				m_nPlaceSequence = 0;
				m_bSetWireDirection=FALSE;
				m_bDrawOccasional=FALSE;
				InvalidateRect(NULL);
			}
		}
		InvalidateRect(NULL);
	//必要な場合にキャプチャを行う
	}else if(	   
		   m_nCurrentTool==ID_TOOL_SELECTOR
		|| m_nCurrentTool==ID_TOOL_DRAG
		|| m_nOnMove	 !=0){
//20100521		SetCapture();				//CWndのメンバー関数(API関数ではない)
//20100521		m_bCaptureMouse=TRUE;		//キャプチャしていることを示すフラグを立てる
		m_mouseCaptureInfo.SetCapture(MouseCaptureInfo::MCI_LEFT);

		m_ptMoveFrom=m_ptMouse;		//キャプチャの始点をセット
		m_ptMoveFrom=m_ptMoveTo=m_ptMouse;
		if(m_nOnMove){
			TRACE(_T("Button down on temporary data area.m_nOnMove=%X\n"),m_nOnMove);
		}else if(m_nCurrentTool==ID_TOOL_SELECTOR ||m_nCurrentTool==ID_TOOL_DRAG ){
			m_bOnSelect=TRUE;
		}
		InvalidateRect(NULL);
	/////////////////////////////////////////
	//キャプチャを行わないツールでの配置動作
	}else if(m_ptMouse.x>=10 && m_ptMouse.x<=sizeSheet.cx-10 &&
			 m_ptMouse.y>=10 && m_ptMouse.y<=sizeSheet.cy-10){
		//ジャンクション
		if(m_nCurrentTool==ID_TOOL_JUNCTION){
			PlaceJunction(pDoc);
			InvalidateRect(NULL);
		//タグ
		}else if(m_nCurrentTool==ID_TOOL_TAG){
			if(!bFixTmp){
				PlaceTag(pDoc,&m_ptMouse);
				m_bDrawTmpData=TRUE;
				InvalidateRect(NULL);
			}
		//ラベル
		}else if(m_nCurrentTool==ID_TOOL_LABEL){
			if(!bFixTmp){
				PlaceLabel(pDoc,&m_ptMouse);
				m_bDrawTmpData=TRUE;
				InvalidateRect(NULL);
			}
		//コメント
		}else if(m_nCurrentTool==ID_TOOL_COMMENT){
			if(!bFixTmp){
				PlaceComment(pDoc,&m_ptMouse);
				m_bDrawTmpData=TRUE;
				InvalidateRect(NULL);
			}
		//パーツ
		}else if(m_nCurrentTool==ID_TOOL_PART){
			if(!bFixTmp){
				PlacePart(pDoc,&m_ptMouse);
				m_bDrawTmpData=TRUE;
				InvalidateRect(NULL);
			}

		//エントリー
		}else if(  m_nCurrentTool==ID_TOOL_ENTRY_BL
				|| m_nCurrentTool==ID_TOOL_ENTRY_BR
				|| m_nCurrentTool==ID_TOOL_ENTRY_WL
				|| m_nCurrentTool==ID_TOOL_ENTRY_WR){
			PlaceEntry(pDoc);
			InvalidateRect(NULL);
		}
	}	//CScrollView::OnLButtonDown(nFlags, point);
}


//////////////////////////////////////////////////////////
//左ボタン解放時の処理
void CBSchView::OnLButtonUp(UINT nFlags, CPoint point) 
{

//	if(m_fShiftKeyDown) return;

	bool shiftKeyDown = (nFlags&MK_SHIFT);

	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CRect rc;
	m_ptMouseDP=point;	//マウスのデバイス座標の保存
	//デバイスコンテキストの準備
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	OnPrepareDC(&dc);	//スクロールビューの反映
	dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
	m_ptMouse=point;
	LPtoBSchP(&m_ptMouse,m_nVExt,m_nWExt);		//マウスの座標を図面座標に変換
	DisplayCurrentXY();
	CBSchDoc* pDoc = GetDocument();

	if(	   m_nCurrentTool==ID_TOOL_WIRE
		|| m_nCurrentTool==ID_TOOL_BUS
		|| m_nCurrentTool==ID_TOOL_DASH
		|| m_nCurrentTool==ID_TOOL_MARKER){

		if(!m_bClickClickLineMode){
//20100521			if(m_bCaptureMouse){
			if(m_mouseCaptureInfo.isLButtonCapture()){
				if(m_bTimer2Active){
					KillTimer(ID_TIMER_AUTOSCROLL);
					m_bTimer2Active=FALSE;
				}

//20100521				::ReleaseCapture();		//キャプチャを解放
//20100521				m_bCaptureMouse=FALSE;	//キャプチャしていることを示すフラグをクリアする
				m_mouseCaptureInfo.ReleaseCapture(MouseCaptureInfo::MCI_LEFT);

				if(m_nCurrentTool==ID_TOOL_WIRE){
					PlaceWire(pDoc);
				}else if(m_nCurrentTool==ID_TOOL_BUS){
					PlaceBus(pDoc);
				}else if(m_nCurrentTool==ID_TOOL_DASH){
					PlaceDash(pDoc);
				}else if(m_nCurrentTool==ID_TOOL_MARKER){
					PlaceMarkerLine(pDoc);
				}
				m_nPlaceSequence = 0;
				m_bDrawOccasional=FALSE;
				InvalidateRect(NULL);
			}
		}else{
		}
	//他のツールでキャプチャ中であればキャプチャを解放
//20100521	}else if(m_bCaptureMouse){
	}else if(m_mouseCaptureInfo.isLButtonCapture()){
		if(m_bTimer2Active){
			KillTimer(ID_TIMER_AUTOSCROLL);
			m_bTimer2Active=FALSE;
		}
//20100521		::ReleaseCapture();		//キャプチャを解放
//20100521		m_bCaptureMouse=FALSE;	//キャプチャしていることを示すフラグをクリアする
		m_mouseCaptureInfo.ReleaseCapture(MouseCaptureInfo::MCI_LEFT);


		if(m_nOnMove){
			int old_m_nOnMove = m_nOnMove; 
			m_nOnMove=0;
			if(old_m_nOnMove & (ON_NAME | ON_NUM)){
				updateBuff(true);
				InvalidateRect(NULL);
			}

			if(m_nCurrentTool==ID_TOOL_SELECTOR && !m_displayQuickPropComp && m_quickPropSetting.isEnabled() && !m_quickPropSetting.m_showOnMouseHover){
				const SXBSchComponent* pObj = pDoc->isComponentSelectedSingly();
				if(pObj){
					ShowQuickPropComp(pObj);
				}
			}

		}else if(m_nCurrentTool==ID_TOOL_SELECTOR ||m_nCurrentTool==ID_TOOL_DRAG ){
			//選択ツールの場合の処理
			//選択中ならば	枠線を（消去のために）描画
			//				ドキュメントの選択関数を呼び出す
			if(m_bOnSelect){
				int nCount;
				//DrawSelectFrame(&dc,FALSE);	//選択中の枠線の消去のために描画
				m_bDrawSelectFrame = FALSE;
				m_bOnSelect=FALSE;
				m_nFocusedTo = FOCUSED_TO_BODY;
				
				HCURSOR hCursor;
				hCursor=AfxGetApp()->LoadStandardCursor(IDC_WAIT);//砂時計カーソル
				hCursor=::SetCursor(hCursor);
				CSize size=m_ptMoveFrom-m_ptMoveTo;
				
				SelectOption selOption;
				selOption.m_mask = m_ObjectFileterValue;

				if(abs(size.cx)>5 || abs(size.cy)>5){//選択に当たってマウスを移動した
					TRACE(_T("\nArea select")); 
					rc=CRect(m_ptMoveFrom,m_ptMoveTo);
					rc.NormalizeRect();
					//g_MakeRECT(&m_ptMoveFrom,&m_ptMoveTo,&rc);	//選択範囲の矩形の設定
					if(m_nCurrentTool==ID_TOOL_SELECTOR)nCount=pDoc->SetSel(&rc,selOption);	//範囲選択の呼び出し
					else	nCount=pDoc->SetDrag(&rc,shiftKeyDown);	//ドラッグ用範囲選択の呼び出し
				}else{//選択に当たってマウスを大きく動かしていない
					TRACE(_T("\nPoint select")); 
					if(m_nCurrentTool==ID_TOOL_SELECTOR){
						nCount=pDoc->SetSel(&m_ptMoveTo,selOption);//点指定選択の呼び出し
						if(nCount==1 && pDoc->isComponentSelectedSingly()){
							int flags = pDoc->IsOnTmpData(&m_ptMoveTo,0,NULL,NULL);
							if(flags & ON_NUM){
								m_nFocusedTo = FOCUSED_TO_NUM;
							}else if(flags & ON_NAME){
								m_nFocusedTo = FOCUSED_TO_VAL;
							}
						}
					}else{
						nCount=pDoc->SetDrag(&m_ptMoveTo,shiftKeyDown);//ドラッグ用点指定選択の呼び出し
					}
				}
				if(nCount>0){		//図面オブジェクトが選択範囲に存在した...						
					updateBuff(true);
				}
				if(nCount==1 && m_nCurrentTool==ID_TOOL_SELECTOR && !m_displayQuickPropComp  && m_quickPropSetting.isEnabled() && !m_quickPropSetting.m_showOnMouseHover){
					const SXBSchComponent* pObj = pDoc->isComponentSelectedSingly();
					if(pObj){
						ShowQuickPropComp(pObj);
					}
				}
				//m_nFocusedTo = FOCUSED_TO_BODY;
				InvalidateRect(NULL);
				::SetCursor(hCursor);
			}
		}	
	}
}

void CBSchView::ShowQuickPropComp(const SXBSchComponent* pObj)
{
	if(!m_pQuickPropComp)return;
	if(!m_quickPropSetting.isEnabled()){
		return;
	}


	if(m_displayQuickPropComp){
		m_pQuickPropComp->ShowWindow(SW_HIDE);							//090429
		m_displayQuickPropComp = false;
	}
	

	CString str;
	CString strLine;
	bool firstline = true;
	if(m_quickPropSetting.m_num){
		strLine.Format(_T("Num: %s"),pObj->refnum());
		str.Append(strLine);
		firstline = false;
	}
	if(m_quickPropSetting.m_val){
		if(!firstline)str.Append(_T("\n"));
		strLine.Format(_T("Val: %s"),pObj->name());
		str.Append(strLine);
		firstline = false;
	}
	if(m_quickPropSetting.m_mfr){
		if(!firstline)str.Append(_T("\n"));
		strLine.Format(_T("Mfr: %s"),pObj->mfr());
		str.Append(strLine);
		firstline = false;
	}
	if(m_quickPropSetting.m_mfrpn){
		if(!firstline)str.Append(_T("\n"));
		strLine.Format(_T("MfrPn: %s"),pObj->mfrpnum());
		str.Append(strLine);
		firstline = false;
	}
	if(m_quickPropSetting.m_package){
		if(!firstline)str.Append(_T("\n"));
		strLine.Format(_T("Pkg: %s"),pObj->packagename());
		str.Append(strLine);
		firstline = false;
	}
	if(m_quickPropSetting.m_nameinlib){
		if(!firstline)str.Append(_T("\n"));
		strLine.Format(_T("NameInLib: %s"),pObj->orgname());
		str.Append(strLine);
		firstline = false;
	}
	if(m_quickPropSetting.m_note){
		if(!firstline)str.Append(_T("\n"));
		strLine.Format(_T("Note: %s"),pObj->note());
		str.Append(strLine);
		firstline = false;
	}
	m_pQuickPropComp->m_lf = m_lfQuickPropFont;//090429	
	
	m_pQuickPropComp->m_str = str;
	CSize sizeContent = m_pQuickPropComp->GetDrawSize();	//内容のサイズを取得(5ピクセルずつのマージン含む)//090429	
	m_ptDisplayQuickPropAt = m_ptMouseDP;	//保存しているのは表示場所ではなく、表示トリガのマウスの位置
	
	CPoint ptTemp = m_ptMouseDP;
	this->ClientToScreen(&ptTemp);

	int x = ptTemp.x - 5 - sizeContent.cx;
	int y = ptTemp.y - 5 - sizeContent.cy;
	if(x<0) x=0;
	if(y<0) y=0;

	//m_pQuickPropComp->MoveWindow(ptTemp.x+5,ptTemp.y,100,50);
	m_pQuickPropComp->MoveWindow(x,y,sizeContent.cx,sizeContent.cy);//090429
	m_pQuickPropComp->m_COL_BG = m_COL.colQUICKPROPVIEW;				//090429

	m_pQuickPropComp->ShowWindow(SW_SHOWNOACTIVATE);
	this->SetFocus();
	m_displayQuickPropComp = true;
}
void CBSchView::HideQuickPropComp()
{
	if(!m_pQuickPropComp)return;
	m_pQuickPropComp->ShowWindow(SW_HIDE);	//090429
	m_displayQuickPropComp = false;
}

void CBSchView::IniReadQuickPropSetting()
{
	m_quickPropSetting.m_enable				= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("ENABLE"),0)!=0);
	m_quickPropSetting.m_showOnMouseHover	= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("MOUSEHOVER"),0)!=0);
	m_quickPropSetting.m_num				= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("NUM"),0)!=0);
	m_quickPropSetting.m_val				= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("VAL"),0)!=0);
	m_quickPropSetting.m_mfr				= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("MFR"),0)!=0);
	m_quickPropSetting.m_mfrpn				= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("MFRPN"),0)!=0);
	m_quickPropSetting.m_package			= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("PKG"),0)!=0);
	m_quickPropSetting.m_note				= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("NOTE"),0)!=0);
	m_quickPropSetting.m_nameinlib			= (AfxGetApp()->GetProfileInt(_T("QuickProp"),_T("NAMEINLIB"),0)!=0);
}

void CBSchView::IniWriteQuickPropSetting()
{
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("ENABLE"),(m_quickPropSetting.m_enable		? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("MOUSEHOVER"),(m_quickPropSetting.m_showOnMouseHover? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("NUM"),(m_quickPropSetting.m_num			? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("VAL"),(m_quickPropSetting.m_val			? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("MFR"),(m_quickPropSetting.m_mfr			? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("MFRPN"),(m_quickPropSetting.m_mfrpn		? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("PKG"),(m_quickPropSetting.m_package		? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("NOTE"),(m_quickPropSetting.m_note			? 1:0));
	AfxGetApp()->WriteProfileInt(_T("QuickProp"),_T("NAMEINLIB"),(m_quickPropSetting.m_nameinlib? 1:0));
}



//自動スクロールの必要性を問う
//次の条件を全て満足するときにスクロールが必要と判断する
//部品名、部品番号の移動中は禁止
//・左ボタンが押下状態 (この条件を除外 1998/07/18)
//・カーソルがクライアントエリアの内側(AUTOSCROLL_FRAME)ドットの枠線より外にある
//・ScrollViewがスクロールしきっていない
UINT CBSchView::QueryAutoScroll(CDC* pDC)
{
	if(m_nOnMove &(	ON_NAME | ON_NUM)) return 0;//部品名、部品番号の移動中は禁止

	CBSchDoc* pDoc = GetDocument();
	UINT dwResult=0;
	RECT rcDP,rcBSch;		//クライアントエリアの四辺の座標
	CSize sizeSheet;		//図面サイズ
	sizeSheet=pDoc->GetSheetSize();	//図面サイズを得る
	GetClientRect(&rcDP);	//クライアントエリアのデバイス座標を得る
	rcBSch=rcDP;			//
	pDC->DPtoLP(&rcBSch);	//クライアントエリアの論理座標を得る
	LPtoBSchP(&rcBSch,m_nVExt,m_nWExt);		//クライアントエリアの図面座標に変換

	//クライアントエリアのサイズが小さいときは自動スクロールしない
	if(rcDP.right <AUTOSCROLL_FRAME*20 ||
	   rcDP.bottom<AUTOSCROLL_FRAME*20) return 0;
	//左検査
	if(m_ptMouseDP.x<AUTOSCROLL_FRAME &&
	   rcBSch.left>0){
		dwResult|=AUTOSCROLL_LEFT;
	}
	//上検査
	if(m_ptMouseDP.y<AUTOSCROLL_FRAME &&
	   rcBSch.top>0){
		dwResult|=AUTOSCROLL_TOP;
	}
	//右検査
	if(m_ptMouseDP.x>(rcDP.right-AUTOSCROLL_FRAME) &&
		rcBSch.right<(sizeSheet.cx-1)){
		dwResult|=AUTOSCROLL_RIGHT;
	}
	//下検査
	if(m_ptMouseDP.y>(rcDP.bottom-AUTOSCROLL_FRAME) &&
		rcBSch.bottom<(sizeSheet.cy-1)){
		dwResult|=AUTOSCROLL_BOTTOM;
	}
	return dwResult;
}	

//自動スクロールの発生
//(1)スクロールを発生させる。
//(2)一時データの再描画のためにマウスの移動メッセージをポストする。
void  CBSchView::AutoScroll(CDC* pDC,UINT fScroll)
{
	POINT point;
	UINT nIDTimer=SetTimer(ID_TIMER_AUTOSCROLL,100,NULL);

	if(fScroll & AUTOSCROLL_LEFT){
		SendMessage(WM_HSCROLL,SB_LINELEFT);
//		SendMessage(WM_HSCROLL,SB_PAGELEFT);
	}else if(fScroll & AUTOSCROLL_RIGHT){
		SendMessage(WM_HSCROLL,SB_LINERIGHT);
//		SendMessage(WM_HSCROLL,SB_PAGERIGHT);
	}else if(fScroll & AUTOSCROLL_TOP){
		SendMessage(WM_VSCROLL,SB_LINELEFT);
//		SendMessage(WM_VSCROLL,SB_PAGELEFT);
	}else if(fScroll & AUTOSCROLL_BOTTOM){
		SendMessage(WM_VSCROLL,SB_LINERIGHT);
//		SendMessage(WM_VSCROLL,SB_PAGERIGHT);
	}
	if(nIDTimer==0){//タイマーが使えなかったとき...
					//ここはタイマーが使えればOnTimer()で行うもの
		GetCursorPos(&point);		//スクリーン座標系でのマウスの位置を知る
		ScreenToClient(&point);		//それをクライアント座標系に変換する
		PostMessage(WM_MOUSEMOVE,0,MAKELONG(point.x,point.y));
	}else{
		m_bTimer2Active=TRUE;
	}
}

//あらゆるスクロールで呼ばれる OnScrollBy()のオーバーライド
BOOL CBSchView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	BOOL bResult;
	InvalidateRect(NULL);
	bResult=CScrollView::OnScrollBy(sizeScroll, bDoScroll);
	return bResult;
}

//////////////////////////////////////////////////////////////////////
//編集関連のメッセージハンドラ

//////////   Undo
BOOL CBSchView::MenuItemTest_Undo()		//1997/01/27 追加
{
	//OnUpdateEditUndo()から移動
	CBSchDoc* pDoc = GetDocument();
	return pDoc->CanUndo();
}

BOOL CBSchView::MenuItemTest_Redo()
{
	CBSchDoc* pDoc = GetDocument();
	return pDoc->CanRedo();
}

void CBSchView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_Undo());
}

void CBSchView::OnEditUndo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	pDoc->Undo();								//Undo()呼び出し
	if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
	pDoc->UpdateAllViews(NULL);					//描画の更新
}

//////////   Delete
void CBSchView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->CanEdit());
}

void CBSchView::OnEditDelete() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	//デバイスコンテキストの準備
	pDoc->Clear();							//データそのものの消去
	if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
	m_oldOpCount = -1;
	InvalidateRect(NULL);
}

//////////   Copy
BOOL CBSchView::MenuItemTest_Copy()
{
	//OnUpdateEditCopy()から移動
	CBSchDoc* pDoc = GetDocument();
	return pDoc->CanEdit();
}

void CBSchView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_Copy());
}	


void CBSchView::OnEditCopy() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	pDoc->Copy();	//データをクリップボードに転送
}

//////////   Paste
BOOL CBSchView::MenuItemTest_Paste()
{
	//OnUpdateEditPaste()から移動
	CBSchDoc* pDoc = GetDocument();
	return pDoc->CanPaste();
}

void CBSchView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_Paste());
}

//ペーストは原点を指定しなければならない
//	カーソルがクライアントエリア内に存在するときはカーソル位置を原点とする
//	カーソルがクライアントエリア外に存在するときはクライントエリアの左上隅を原点とする
//	ただし、原点のXYともに図面座標で10以上であること
void CBSchView::OnEditPaste() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(m_nCurrentTool!=ID_TOOL_SELECTOR){
		//ChangeTool(m_nCurrentTool,ID_TOOL_SELECTOR);//ツールを変えたときの共通処理
		m_nCurrentTool=ID_TOOL_SELECTOR;
	}

	CBSchDoc* pDoc = GetDocument();
	CPoint ptCursor;					//カーソル座標
	CRect  rc;							//クライアントエリアの座標
	CPoint point=GetScrollPosition();	//配置座標。まずクライアントエリア左上隅の論理座標を入れる。
	
	GetCursorPos(&ptCursor);			//スクリーン座標系でのマウスの位置を知る
	ScreenToClient(&ptCursor);			//それをクライアント座標系に変換する
	GetClientRect(&rc);					//クライアントエリアの座標を得る(left,topは常に０)

	CSize sizeSheet;
	sizeSheet=pDoc->GetSheetSize();	//図面サイズを得る

	//クライアントエリア左上の座標
	CPoint pointLT=point;	//
	LPtoBSchP(&pointLT,m_nVExt,m_nWExt);	//図面座標に変換
	
	//クライアントエリア中央の論理座標を得る	1997/01/26
	CPoint pointCenter;
	pointCenter.x=rc.right;		//右下クライアント座標を入れて
	pointCenter.y=rc.bottom;
	pointCenter+=point;			//スクロールを考慮した論理座標に変換
	LPtoBSchP(&pointCenter,m_nVExt,m_nWExt);	//図面座標に変換
	if(pointCenter.x>sizeSheet.cx) pointCenter.x=sizeSheet.cx;	//図面サイズから出ないことを保証
	if(pointCenter.y>sizeSheet.cy) pointCenter.y=sizeSheet.cy;	
	pointCenter.x=pointLT.x+(pointCenter.x-pointLT.x)/2;	//論理座標中央に変換
	pointCenter.y=pointLT.y+(pointCenter.y-pointLT.y)/2;

	if(ptCursor.x>=rc.left && ptCursor.x<=rc.right &&	//カーソルの位置が
	   ptCursor.y>=rc.top  && ptCursor.y<=rc.bottom){	//クライアントエリア内ならば...
		ptCursor+=point;				//カーソル位置に原点を加算して論理座標とする。
		LPtoBSchP(&ptCursor,m_nVExt,m_nWExt);			//さらに図面座標に変換
		if(ptCursor.x>sizeSheet.cx-20 || ptCursor.y>sizeSheet.cy-20){	//カーソルが図面範囲外ならば...
			point=pointCenter;			//クライアントエリア中央に配置 1997/01/26
		}else{
			point=ptCursor;				//カーソル位置にペースト
		}
	}else{
		point=pointCenter;			//クライアントエリア中央に配置 1997/01/26
	}
	if(point.x<10) point.x=10;			//左・上10ドット以内なら10ドットの位置に補正
	if(point.y<10) point.y=10;

	SnapToGrid(&point);					//必要ならグリッドへのスナップ

//	TRACE("\n Paste at %d,%d",point.x,point.y);

	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	OnPrepareDC(&dc);	//スクロールビューの反映

	if(pDoc->GetTmpDataCount()){		//一時データが存在するときは...
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);					//一時データを解除
		//pDoc->FixSelection();			//一時データを解除
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
		updateBuff(true);				// 2005/01/30追加
	}
	pDoc->Paste(&point);				//データのペースト
	if(pDoc->GetTmpDataCount()){		//ペーストに成功した結果、一時データが存在
		pDoc->setupRcTempArea();
	}
	TRACE("OnEditPaste()\n");
	m_oldOpCount = -1;
	InvalidateRect(NULL);
}

//////////   Cut
BOOL CBSchView::MenuItemTest_Cut()
{
	CBSchDoc* pDoc = GetDocument();
	return pDoc->CanEdit();
}

void CBSchView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_Cut());
}

void CBSchView::OnEditCut() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	//デバイスコンテキストの準備
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	pDoc->Cut();							//データをクリップボードに転送してから削除
	if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
	m_oldOpCount = -1;
	InvalidateRect(NULL);
}


//////////   Rotate
BOOL CBSchView::MenuItemTest_Rotate()
{
	//OnUpdateRotate()から移動
	CBSchDoc* pDoc = GetDocument();
	//TRACE("MenuItemTest_Rotate:%d\n",pDoc->CanRotate());
	return (pDoc->CanRotate()
		        || m_nCurrentTool==ID_TOOL_ENTRY_BL
				|| m_nCurrentTool==ID_TOOL_ENTRY_BR
				|| m_nCurrentTool==ID_TOOL_ENTRY_WL
				|| m_nCurrentTool==ID_TOOL_ENTRY_WR);
}

void CBSchView::OnUpdateRotate(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_Rotate());
	//pCmdUI->Enable(FALSE);

}

void CBSchView::OnRotate() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	OnPrepareDC(&dc);	//スクロールビューの反映
	if(    m_nCurrentTool==ID_TOOL_ENTRY_BL
		|| m_nCurrentTool==ID_TOOL_ENTRY_BR
		|| m_nCurrentTool==ID_TOOL_ENTRY_WL
		|| m_nCurrentTool==ID_TOOL_ENTRY_WR){
		//DrawOccasional(&dc,FALSE);
		switch(m_nCurrentTool){
		case ID_TOOL_ENTRY_BL:
			m_nCurrentTool=ID_TOOL_ENTRY_BR;
			m_pointOccasional.x+=10;
			break;
		case ID_TOOL_ENTRY_BR:
			m_nCurrentTool=ID_TOOL_ENTRY_BL;
			m_pointOccasional.x-=10;
			break;
		case ID_TOOL_ENTRY_WL:
			m_nCurrentTool=ID_TOOL_ENTRY_WR;
			m_pointOccasional.x+=10;
			break;
		case ID_TOOL_ENTRY_WR:
			m_nCurrentTool=ID_TOOL_ENTRY_WL;
			m_pointOccasional.x-=10;
			break;
		}
		InvalidateRect(NULL);
	}else{	
		CBSchDoc* pDoc = GetDocument();

		SXBSchComponent* pComp = NULL; 
		if(pDoc->isComponentSelectedSingly()){
			pComp = (SXBSchComponent*)pDoc->GetTmp();
		}
		if(pComp && (m_nCurrentTool==ID_TOOL_SELECTOR ||m_nCurrentTool==ID_TOOL_PART )){
			switch(m_nFocusedTo){
				case FOCUSED_TO_NUM:
					pComp->setRefnumHorizontal(!pComp->refnumHorizontal());
					break;
				case FOCUSED_TO_VAL:
					pComp->setNameHorizontal(!pComp->nameHorizontal());
					break;
				default:
					pDoc->Rotate();		//回転
			}
			pDoc->SetTempDirty();

		}else{
			pDoc->Rotate();							//回転
		}
		InvalidateRect(NULL);
	}
}

//////////   Mirror
BOOL CBSchView::MenuItemTest_Mirror()
{
	//OnUpdateMirror()から移動
	CBSchDoc* pDoc = GetDocument();
	return pDoc->CanMirror();
}

void CBSchView::OnUpdateMirror(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_Mirror());
}

void CBSchView::OnMirror() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	pDoc->Mirror();							//左右反転
	InvalidateRect(NULL);
}


//////////   Attribute
BOOL CBSchView::MenuItemTest_Attribute()
{
	//OnUpdateEditAttribute()から移動
	CBSchDoc* pDoc = GetDocument();
	return (pDoc->CanAttribute());
}

void CBSchView::OnUpdateEditAttribute(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_Attribute());
}


void CBSchView::OnEditAttribute() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	CRect rc=pDoc->GetTmpRect();


	SXBSchObj* pObj = pDoc->GetTmp();							//この処理は要らないのでは?  要検討2013/08/14
	SXBSchObj* pObjOld =pObj->duplicate();//バックアップの作成	//この処理は要らないのでは?  要検討2013/08/14

	if(DoAttributeDialog()){
		CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
		OnPrepareDC(&dc);	//スクロールビューの反映
		//旧一時データの枠を消去
		BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
		rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
		updateBuff(true);
		InvalidateRect(NULL);
	}
	delete pObjOld;	//バックアップの消去						//この処理は要らないのでは?  要検討2013/08/14
}

//右ボタンメニュー追加 1997/01/23
void CBSchView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
//	if(m_bCaptureMouse) return; // 1997/03/06 キャプチャ中は右ボタンメニューを禁止
	if(m_mouseCaptureInfo.isLButtonCapture()) return; // 1997/03/06 キャプチャ中は右ボタンメニューを禁止
	CMenu menu;
	menu.LoadMenu(IDR_FLOATINGMENU);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
		                               point.x,point.y,AfxGetMainWnd()); //1997/04/22
/******************************************************************************************************************
	TrackPopupMenu()の4番目の引数 CWnd* に this（ここでは CBSchViewのウインドウ）を渡すと
	Enable/Disableや、チェックマークがメインメニューと同期しない。最初、これがわからずに
	下記のように自前で操作していた  1997/04/22

	if(!MenuItemTest_Undo())		menu.EnableMenuItem(ID_EDIT_UNDO,	  MF_GRAYED|MF_BYCOMMAND);//1997/01/27 追加
	if(!MenuItemTest_Cut())			menu.EnableMenuItem(ID_EDIT_CUT,	  MF_GRAYED|MF_BYCOMMAND);
	if(!MenuItemTest_Copy())		menu.EnableMenuItem(ID_EDIT_COPY,	  MF_GRAYED|MF_BYCOMMAND);
	if(!MenuItemTest_Paste())		menu.EnableMenuItem(ID_EDIT_PASTE,	  MF_GRAYED|MF_BYCOMMAND);
	if(!MenuItemTest_Attribute())	menu.EnableMenuItem(ID_EDIT_ATTRIBUTE,MF_GRAYED|MF_BYCOMMAND);
	if(!MenuItemTest_Mirror())		menu.EnableMenuItem(ID_MIRROR,		  MF_GRAYED|MF_BYCOMMAND);
	if(!MenuItemTest_Rotate())		menu.EnableMenuItem(ID_ROTATE,		  MF_GRAYED|MF_BYCOMMAND);
	int nTool=m_nCurrentTool;
	if(nTool==ID_TOOL_ENTRY_BR) nTool=ID_TOOL_ENTRY_BL;
	else if(nTool==ID_TOOL_ENTRY_WR) nTool=ID_TOOL_ENTRY_WL;
	menu.CheckMenuItem(nTool,MF_CHECKED|MF_BYCOMMAND);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
		                               point.x,point.y,this);
******************************************************************************************************************/
}

//ダブルクリックでアトリビュート変更  1997/01/25
void CBSchView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください	
	//CScrollView::OnLButtonDblClk(nFlags, point);
	if(m_nCurrentTool==ID_TOOL_WIRE){
		if(m_nPlaceSequence>0 && m_bClickClickLineMode){
			m_nPlaceSequence = 0;
//20100521			m_bCaptureMouse=FALSE;
//20100521			::ReleaseCapture();
			m_mouseCaptureInfo.ReleaseCapture(MouseCaptureInfo::MCI_LEFT);

			m_bSetWireDirection=FALSE;
			m_bDrawOccasional=FALSE;
		}

		CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
		OnPrepareDC(&dc);	//スクロールビューの反映
		dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
		m_pointOccasional=point;
		LPtoBSchP(&m_pointOccasional,m_nVExt,m_nWExt);		//マウスの座標を図面座標に変換
		SnapToGrid(&m_pointOccasional);
		CBSchDoc* pDoc = GetDocument();
		PlaceJunction(pDoc);
		InvalidateRect(NULL);
	}else{
		if(MenuItemTest_Attribute()) SendMessage(WM_COMMAND,ID_EDIT_ATTRIBUTE);
	}
}


//クライアントエリアの中央の図面座標 1997/01/25
//OnEditPaste()から抜粋して作成
CPoint CBSchView::GetClientCenter()
{
	CBSchDoc* pDoc = GetDocument();

	CPoint pointLT=GetScrollPosition();	//クライアントエリア左上隅の論理座標
	LPtoBSchP(&pointLT,m_nVExt,m_nWExt);				//図面座標に変換
	CRect  rc;							//クライアントエリアの座標
	GetClientRect(&rc);					//クライアントエリアの座標を得る(left,topは常に０)
	CSize sizeSheet;
	sizeSheet=pDoc->GetSheetSize();		//図面サイズを得る
	CPoint pointCenter;
	pointCenter.x=rc.right;		//右下クライアント座標を入れて
	pointCenter.y=rc.bottom;
	pointCenter+=pointLT;			//スクロールを考慮した論理座標に変換
	LPtoBSchP(&pointCenter,m_nVExt,m_nWExt);	//図面座標に変換
	if(pointCenter.x>sizeSheet.cx) pointCenter.x=sizeSheet.cx;	//図面サイズから出ないことを保証
	if(pointCenter.y>sizeSheet.cy) pointCenter.y=sizeSheet.cy;	
	pointCenter.x=pointLT.x+(pointCenter.x-pointLT.x)/2;	//論理座標中央に変換
	pointCenter.y=pointLT.y+(pointCenter.y-pointLT.y)/2;
	return pointCenter;
}


void CBSchView::OnEditSetuplayer() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	if(pDoc->GetTmpDataCount()){	//一時データが存在していたら...
		//pDoc->FixSelection();		//一時データを解除
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//一時データを解除
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
		updateBuff(true);			// 2005/01/30追加
		Invalidate(FALSE);
	}

	CSetLayer dlg;
	dlg.m_nEdit = pDoc->editLayer();
	dlg.m_wShow	= pDoc->visibleLayer();
	dlg.m_fEditHigh = m_fEditHighLight;
	dlg.m_fDisplayNameOnDlgBar = (AfxGetApp()->GetProfileInt(_T("Option"), _T("DisplayLayerNameOnDlgBar"), TRUE) ? TRUE : FALSE);

	for (int i = 0; i < 8; i++) {
		dlg.m_strName[i] = pDoc->layerName(i);
	}

	if(dlg.DoModal()==IDOK){
		pDoc->setEditLayer(dlg.m_nEdit);
		pDoc->setVisibleLayer(dlg.m_wShow);
		m_fEditHighLight = dlg.m_fEditHigh;
		for (int i = 0; i < 8; i++) {
			pDoc->setLayerName(i, dlg.m_strName[i]);
		}
		SetDBarLayer_LayerName();
		AfxGetApp()->WriteProfileInt(_T("Option"),_T("HighLightEditLayer"),m_fEditHighLight);

		AfxGetApp()->WriteProfileInt(_T("Option"), _T("DisplayLayerNameOnDlgBar"), dlg.m_fDisplayNameOnDlgBar);

		pDoc->UpdateAllViews(NULL);
	}
}


//レイヤーの移動 1998/06/28
BOOL CBSchView::MenuItemTest_MoveToLayer(int nLayer)
{
	CBSchDoc* pDoc = GetDocument();
	WORD wBit=(0x0001 << nLayer);
	return (   pDoc->CanEdit() 
			&& (m_nCurrentTool==ID_TOOL_SELECTOR)
			&& (nLayer != pDoc->editLayer()) 
			&& (wBit & pDoc->visibleLayer()) );
}

void CBSchView::OnUpdateEditMovetolayer0(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(0));
}

void CBSchView::OnUpdateEditMovetolayer1(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(1));
}

void CBSchView::OnUpdateEditMovetolayer2(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(2));
}

void CBSchView::OnUpdateEditMovetolayer3(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(3));
}

void CBSchView::OnUpdateEditMovetolayer4(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(4));
}

void CBSchView::OnUpdateEditMovetolayer5(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(5));
}

void CBSchView::OnUpdateEditMovetolayer6(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(6));
}

void CBSchView::OnUpdateEditMovetolayer7(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(MenuItemTest_MoveToLayer(7));
}

//1998/06/30  選択範囲を指定レイヤーに移動
void CBSchView::MoveLayer(int n)
{
	DWORD dwDark=0;
	CBSchDoc* pDoc = GetDocument();
	if(m_fEditHighLight) dwDark=DRAW_DARK;
	pDoc->MoveLayer(n);
	if(pDoc->GetTmpDataCount()){	//一時データが存在していたら...
		//pDoc->FixSelection();		//一時データを解除
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//一時データを解除
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
		updateBuff(true);			// 2005/01/30追加
		InvalidateRect(NULL);

	}
}

void CBSchView::OnExtMenu_0()
{
	OnExtMenuN(0);
}

void CBSchView::OnExtMenu_1()
{
	OnExtMenuN(1);
}

void CBSchView::OnExtMenu_2()
{
	OnExtMenuN(2);
}

void CBSchView::OnExtMenu_3()
{
	OnExtMenuN(3);
}

void CBSchView::OnExtMenu_4()
{
	OnExtMenuN(4);
}

void CBSchView::OnExtMenu_5()
{
	OnExtMenuN(5);
}

void CBSchView::OnExtMenu_6()
{
	OnExtMenuN(6);
}

void CBSchView::OnExtMenu_7()
{
	OnExtMenuN(7);
}

void CBSchView::OnExtMenu_8()
{
	OnExtMenuN(8);
}

void CBSchView::OnExtMenu_9()
{
	OnExtMenuN(9);
}

void CBSchView::OnExtMenuN(int n)
{
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_extensionMenuCount <= n) return;

	CBSchDoc* pDoc = GetDocument();
	if(!pDoc->SaveFile()) return;

	CString dataPath=pDoc->GetPathName();

	CString strCmdline = pMainFrame->m_aryExtensionMenu[n].formatCmdline(dataPath);
	BOOL bReload = pMainFrame->m_aryExtensionMenu[n].isReload();

	TCHAR* cmdline = new TCHAR[_tcslen(strCmdline)+1];
	_tcscpy(cmdline,strCmdline);


	PROCESS_INFORMATION pi;
	STARTUPINFO si;

    ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);


	CPath iniPath(((CBSchApp*)AfxGetApp())->m_szIniFileDir);
	TCHAR szCurDir[_MAX_PATH];
	BOOL validIniPath = iniPath.IsDirectory();
	
	if(validIniPath){
		::GetCurrentDirectory(_MAX_PATH,szCurDir);
		::SetCurrentDirectory(iniPath.m_strPath);
	}

    BOOL resProc = CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);


	if(bReload && resProc){
		while(WAIT_TIMEOUT==WaitForSingleObject(pi.hProcess,1)){
			MSG msg;
			if(GetMessage(&msg,NULL,0,0)){
				if(msg.message == WM_PAINT){				//	メッセージが WM_PAINT なら
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		pDoc->ReloadFile();
	}

	if(validIniPath){
		::SetCurrentDirectory(szCurDir);
	}	

	CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

	delete[]cmdline;

	//LRESULT lres = pMainFrame->SendMessage(WM_COMMAND,ID_FILE_SAVE);


}

void CBSchView::OnEditMovetolayer0() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(0);
}

void CBSchView::OnEditMovetolayer1() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(1);
}

void CBSchView::OnEditMovetolayer2() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(2);
}

void CBSchView::OnEditMovetolayer3() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(3);
}

void CBSchView::OnEditMovetolayer4() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(4);
}

void CBSchView::OnEditMovetolayer5() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(5);
}

void CBSchView::OnEditMovetolayer6() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(6);
}

void CBSchView::OnEditMovetolayer7() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	MoveLayer(7);
}



void CBSchView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch(nChar){
		case VK_PRIOR:	OnZoomInOutProc(TRUE,TRUE);return;
		case VK_NEXT:	OnZoomInOutProc(FALSE,TRUE);return;
	}

	short keyState = ::GetKeyState(VK_SHIFT);
	TRACE(_T("CBSchView::OnKeyDown %04X\n"),keyState);
	bool bShiftKeyDown = keyState&0x8000;

	if(nChar == VK_ESCAPE){
//20100521		if(!m_bCaptureMouse && !m_nPlaceSequence){
		if(!m_mouseCaptureInfo.isLButtonCapture() && !m_nPlaceSequence){
			SendMessage(WM_COMMAND,ID_TOOL_SELECTOR);
			return;
		}else if(m_nPlaceSequence>0 && m_bClickClickLineMode){
			m_nPlaceSequence--;
			if(m_nPlaceSequence==0){
//20100521				::ReleaseCapture();
//20100521				m_bCaptureMouse=FALSE;
				m_mouseCaptureInfo.ReleaseCapture(MouseCaptureInfo::MCI_LEFT);

				m_bSetWireDirection=FALSE;
				m_bDrawOccasional=FALSE;
			}
			Invalidate();
			return;
		}
	}
	if(nChar == VK_SPACE){
		if(m_nCurrentTool==ID_TOOL_WIRE || m_nCurrentTool==ID_TOOL_BUS){
			if(m_bSetWireDirection){
				m_bWireHorizon1st=!m_bWireHorizon1st;
				Invalidate();
			}
		}
		return;
	}


//20100521	if(!m_bCaptureMouse){
	if(!m_mouseCaptureInfo.isLButtonCapture()){
		switch(nChar){
		case 'S':SendMessage(WM_COMMAND,ID_TOOL_SELECTOR);return;
		case 'R':SendMessage(WM_COMMAND,ID_TOOL_DRAG);return;
		case 'P':SendMessage(WM_COMMAND,ID_TOOL_PART);return;
		case 'B':SendMessage(WM_COMMAND,ID_TOOL_BUS);return;
		case 'W':SendMessage(WM_COMMAND,ID_TOOL_WIRE);return;
		case 'D':SendMessage(WM_COMMAND,ID_TOOL_DASH);return;
		case 'M':SendMessage(WM_COMMAND,ID_TOOL_MARKER);return;
		case 'J':SendMessage(WM_COMMAND,ID_TOOL_JUNCTION);return;
		case 'N':SendMessage(WM_COMMAND,ID_TOOL_ENTRY_BL);return;
		case 'E':SendMessage(WM_COMMAND,ID_TOOL_ENTRY_WL);return;
		case 'T':SendMessage(WM_COMMAND,ID_TOOL_TAG);return;
		case 'L':SendMessage(WM_COMMAND,ID_TOOL_LABEL);return;
		case 'C':SendMessage(WM_COMMAND,ID_TOOL_COMMENT);return;
		}
		
		CBSchDoc* pDoc = GetDocument();

		//int moveStepBase = (m_bSnaptoGrid ? 10 : 1);

		SXBSchComponent* pComp = NULL; 
		if(pDoc->isComponentSelectedSingly()){
			pComp = (SXBSchComponent*)pDoc->GetTmp();
		}
		if(pComp && (m_nCurrentTool==ID_TOOL_SELECTOR ||m_nCurrentTool==ID_TOOL_DRAG ||m_nCurrentTool==ID_TOOL_PART )){
			if(nChar == VK_TAB){

				if(!bShiftKeyDown){
					switch(m_nFocusedTo){
						case  FOCUSED_TO_BODY:
							if(!pComp->GetHideNum()){
								m_nFocusedTo = FOCUSED_TO_NUM;
								break;
							}
						case  FOCUSED_TO_NUM:
							if(!pComp->GetHideVal()){
								m_nFocusedTo = FOCUSED_TO_VAL;
								break;
							}
						default:
							m_nFocusedTo = FOCUSED_TO_BODY;
					}
				}else{
					switch(m_nFocusedTo){
						case  FOCUSED_TO_BODY:
							if(!pComp->GetHideVal()){
								m_nFocusedTo = FOCUSED_TO_VAL;
								break;
							}
						case  FOCUSED_TO_VAL:
							if(!pComp->GetHideNum()){
								m_nFocusedTo = FOCUSED_TO_NUM;
								break;
							}
						default:
							m_nFocusedTo = FOCUSED_TO_BODY;
					}
				}
				updateBuff();
				Invalidate();
				return;
			}
		}

		if(pComp && (m_nCurrentTool==ID_TOOL_SELECTOR ||m_nCurrentTool==ID_TOOL_PART )){
			int x = 0;
			int y = 0;
			int moveStep = ((m_nFocusedTo == FOCUSED_TO_BODY)&& m_bSnaptoGrid ? 10 : 1);
			if(bShiftKeyDown){
				moveStep*=5;
			}
			switch(nChar){
				case VK_DOWN:
					y=moveStep;
					break;
				case VK_UP:
					y=-moveStep;
					break;
				case VK_LEFT:
					x=-moveStep;
					break;
				case VK_RIGHT:
					x=moveStep;
					break;
			}
			if(x||y){
				switch(m_nFocusedTo){
					case FOCUSED_TO_BODY:
						{
							CSize sizeOffset = CSize(x,y);
							pDoc->Shift(&sizeOffset);
						}
						break;
					case FOCUSED_TO_NUM:
						pComp->moveRefnumPos(x,y);
						pDoc->SetTempDirty();
						break;

					case FOCUSED_TO_VAL:
						pComp->moveNamePos(x,y);
						pDoc->SetTempDirty();
						break;
				}
				updateBuff();
				Invalidate();
				return;
			}
		}

		if(pDoc->GetTmpDataCount()){
			int x = 0;
			int y = 0;
			int moveStep = (m_bSnaptoGrid ? 10 : 1);
			if(bShiftKeyDown){
				moveStep*=5;
			}
			switch(nChar){
				case VK_DOWN:
					y=moveStep;
					break;
				case VK_UP:
					y=-moveStep;
					break;
				case VK_LEFT:
					x=-moveStep;
					break;
				case VK_RIGHT:
					x=moveStep;
					break;
			}
			if(x||y){
				CSize sizeOffset = CSize(x,y);
				pDoc->Shift(&sizeOffset);
				updateBuff();
				Invalidate();
				return;
			}
		}
	}

	//CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);


}

void CBSchView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(nChar == VK_SHIFT){
		TRACE(_T("SHIFT KEYUP\n"));		
//		if(m_fShiftKeyDown){
//			TRACE("CBSchView::OnKeyUp()\n");
//			//::ReleaseCapture();
//			//m_bCaptureMouse=FALSE;
//			m_fShiftKeyDown=FALSE;
//			CPoint point;
//			::GetCursorPos(&point);
//			ScreenToClient(&point);
//			SendMessage(WM_LBUTTONUP,0,MAKELONG(point.x,point.y));
//		}
	}
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CBSchView::OnKillFocus(CWnd* pNewWnd) 
{
	CScrollView::OnKillFocus(pNewWnd);
//	if(m_fShiftKeyDown){
//		PostMessage(WM_KEYUP,VK_SHIFT);
//	}
	//PostMessage(WM_KEYUP,VK_SHIFT);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	TRACE(_T("\nKILLFOCUS\n"));
}



//コメントのアトリビュート変更用のダイアログを起動
BOOL CBSchView::DoAttributeDialogComment(SXBSchComment* pObj)
{
	CCmntAtrb dlg;
	dlg.m_rText=pObj->text();

	_tcsncpy(dlg.m_logfont.lfFaceName,pObj->fontName(),LF_FACESIZE);
	dlg.m_logfont.lfFaceName[LF_FACESIZE-1]='\0';
	dlg.m_logfont.lfHeight=pObj->fontSize();
	dlg.m_logfont.lfWeight=(pObj->fontBold() ?  FW_BOLD : FW_REGULAR);
	dlg.m_logfont.lfItalic=pObj->fontItalic();
	dlg.m_logfont.lfCharSet=DEFAULT_CHARSET;

//	dlg.m_logfont.lfPitchAndFamily=AfxGetApp()->GetProfileInt(lpszCommentFont,"PitchAndFamily",0);	//フォントのピッチ＆ファミリ情報を得る。デフォルトは0
	dlg.m_enableTag = pObj->enableTag();
	int width =pObj->width();
	if(width<0){
		dlg.m_enableWidthSetting = false;
		dlg.m_width = 10000;
	}else{
		dlg.m_enableWidthSetting = true;
		dlg.m_width = width;
	}


	if(dlg.DoModal()==IDCANCEL) return FALSE;



//	if(dlg.m_bUseSmallFont)pObj->SetType(COMMENT_SMALL);
//	else                   pObj->SetType(COMMENT_LARGE);
	pObj->setText(dlg.m_rText);
	pObj->setFontSize(abs(dlg.m_logfont.lfHeight));
	pObj->setFontBold(dlg.m_logfont.lfWeight>=FW_BOLD);
	pObj->setFontItalic(dlg.m_logfont.lfItalic!=0);
	pObj->setFontName(dlg.m_logfont.lfFaceName);
	
	pObj->deleteMarkupLineList();
	pObj->setEnableTag(dlg.m_enableTag);
	//pObj->setupMarkupLineList();

	if(dlg.m_enableWidthSetting){
		pObj->setWidth(dlg.m_width);
	}else{
		pObj->setWidth(-1);
	}
	//dlg.m_logfont.lfCharSet=DEFAULT_CHARSET;
	//dlg.m_logfont.lfPitchAndFamily=AfxGetApp()->GetProfileInt(lpszCommentFont,"PitchAndFamily",0);	//フォントのピッチ＆ファミリ情報を得る。デフォルトは0
	return TRUE;
}

//ラベルのアトリビュート変更用のダイアログを起動
BOOL CBSchView::DoAttributeDialogLabel(SXBSchLabel* pObj)
{
	CLblAtrb dlg;
	dlg.m_rText=pObj->text();
	if(dlg.DoModal()==IDCANCEL) return FALSE;
	pObj->setText(dlg.m_rText);
	return TRUE;
}

//タグのアトリビュート変更用のダイアログを起動
BOOL CBSchView::DoAttributeDialogTag(SXBSchTag* pObj)
{
	CTagAtrb dlg;
	dlg.m_rName=pObj->text();
	dlg.m_nType=pObj->tagType();
	if(dlg.DoModal()==IDCANCEL) return FALSE;
	pObj->setTagType(dlg.m_nType);
	pObj->setText(dlg.m_rName);
	return TRUE;
}

//アトリビュート変更用のダイアログを起動
BOOL CBSchView::DoAttributeDialogComponent(SXBSchComponent* pObj)
{
	CPartAtrb dlg(this);	//0.83.05
	if(pObj->compInfoIndex()==NULL) return FALSE;
	dlg.m_nBlock   =pObj->block()+1;
//    dlg.m_nBlockMax=m_pPartIndex->GetBlock();
	dlg.m_pPartIndex=pObj->compInfoIndex();
	dlg.m_rPartNum=pObj->refnum();
	dlg.m_bNumVertical = !pObj->refnumHorizontal();
	dlg.m_rPartName=pObj->name();
	dlg.m_bNameVertical = !pObj->nameHorizontal();
	dlg.m_rOrgName=pObj->orgname();
	dlg.m_rNote=pObj->note();
	dlg.m_nLastSelectLibrary = m_nLastSelectLibrary;
	dlg.m_nLastSelectPart = m_nLastSelectPart; 
	dlg.m_mfr = pObj->mfr();
	dlg.m_mfrpnum = pObj->mfrpnum();
	dlg.m_package = pObj->packagename();
	dlg.m_nFocusedTo = m_nFocusedTo;
	dlg.m_bHideNum = pObj->GetHideNum();
	dlg.m_bHideVal = pObj->GetHideVal();

	if(dlg.DoModal()==IDCANCEL) return FALSE;
	if(dlg.m_pPartIndex!=pObj->compInfoIndex()){
		pObj->setCompInfoIndex(dlg.m_pPartIndex);


		m_nLastSelectLibrary = dlg.m_nLastSelectLibrary;
		m_nLastSelectPart = dlg.m_nLastSelectPart; 
	}
	//else if(dlg.m_modifiedCompInfo){
	//	pObj->setPinLtrb();
	//}
	//ブロックの値の再設定
	pObj->setBlock(dlg.m_nBlock-1);
	//部品番号の再設定
	pObj->setRefnum(dlg.m_rPartNum);
	pObj->setRefnumHorizontal(!dlg.m_bNumVertical);//2005/04/09
	//部品名の再設定
	pObj->setName(dlg.m_rPartName);
	pObj->setNameHorizontal(!dlg.m_bNameVertical);//2005/04/09
	pObj->setNote(dlg.m_rNote);
	pObj->setMfr(dlg.m_mfr);
	pObj->setMFrPNum(dlg.m_mfrpnum);
	pObj->setPackageName(dlg.m_package);
	pObj->SetHideNum(dlg.m_bHideNum);
	pObj->SetHideVal(dlg.m_bHideVal);


	if(m_nCurrentTool==ID_TOOL_SELECTOR ||m_nCurrentTool==ID_TOOL_PART){
		if(	((m_nFocusedTo == FOCUSED_TO_NUM) && pObj->GetHideNum())||
			((m_nFocusedTo == FOCUSED_TO_VAL) && pObj->GetHideVal()) ){
				m_nFocusedTo = FOCUSED_TO_BODY;
		}
	}




	return TRUE;
}

//イメージオブジェクトアトリビュート変更用のダイアログを起動
BOOL CBSchView::DoAttributeDialogImageObj(SXBSchImageObj* pObj)
{
	CAtribImage dlg;
	dlg.m_magnification = pObj->DrawMagnification();
	if(dlg.DoModal()==IDOK){
		pObj->SetDrawMagnification(dlg.m_magnification);
		return TRUE;
	}
	return FALSE;
}


//マーカーラインアトリビュート変更用のダイアログを起動
BOOL CBSchView::DoAttributeDialogMarkerLine(SXBSchMarkerLine* pObj)
{
	CMarkerLineAtrb dlg;
	dlg.m_nColor = pObj->m_nColor;
	dlg.m_nWidth = pObj->m_width;
	IniReadMarkerLineCustomColors(dlg.m_arrayColor);
	if(dlg.DoModal()==IDOK){
		pObj->m_nColor = dlg.m_nColor;
		pObj->m_width = dlg.m_nWidth;
		IniWriteMarkerLineCustomColors(dlg.m_arrayColor);
		return TRUE;
	}
	return FALSE;
}

//装飾線アトリビュート変更用のダイアログを起動
BOOL  CBSchView::DoAttributeDialogDecoLine(SXBSchDash* pObj)
{
	CSetDecorationLine dlg;
	dlg.m_bCurve = pObj->curve();
	dlg.m_lineStyle = pObj->lineStyle();
	dlg.m_lineWidth = pObj->lineWidth();
	dlg.m_startPointStyle = pObj->startPointStyle();
	dlg.m_endPointStyle = pObj->endPointStyle();
	dlg.m_endMarkSize = pObj->endMarkSize();
	if(dlg.DoModal()==IDOK){
		pObj->setCurve(dlg.m_bCurve);
		pObj->setLineStyle(dlg.m_lineStyle);
		pObj->setLineWidth(dlg.m_lineWidth);
		pObj->setStartPointStyle(dlg.m_startPointStyle);
		pObj->setEndPointStyle(dlg.m_endPointStyle);
		pObj->setEndMarkSize(dlg.m_endMarkSize);
		return TRUE;
	}
	return FALSE;
}



//アトリビュート変更ダイアログの起動
BOOL CBSchView::DoAttributeDialog()
{
	if(m_displayQuickPropComp){
		HideQuickPropComp();
	}

	CBSchDoc* pDoc = GetDocument();
	SXBSchObj* pObj = pDoc->GetTmp();
	BOOL dlgResult;
	switch(pObj->id()){
	case SXBSchObj::ID_COMMENT:
		dlgResult = DoAttributeDialogComment((SXBSchComment*)pObj);
		break;
	case SXBSchObj::ID_LABEL:
		dlgResult = DoAttributeDialogLabel((SXBSchLabel*)pObj);
		break;
	case SXBSchObj::ID_TAG:
		dlgResult = DoAttributeDialogTag((SXBSchTag*)pObj);
		break;
	case SXBSchObj::ID_COMPONENT:
		dlgResult = DoAttributeDialogComponent((SXBSchComponent*)pObj);
		break;
	case SXBSchObj::ID_DASH:
		dlgResult = DoAttributeDialogDecoLine((SXBSchDash*)pObj);
		break;
	case SXBSchObj::ID_MARKER:
		dlgResult = DoAttributeDialogMarkerLine((SXBSchMarkerLine*)pObj);
		break;
	case SXBSchObj::ID_IMAGEOBJECT:
		dlgResult = DoAttributeDialogImageObj((SXBSchImageObj*)pObj);
		break;
	default:
		dlgResult = FALSE;
	}

	if(dlgResult){
		//pDoc->setupRcTempArea();	//原点の座標を設定
	//	m_rcTmpData=pObj->GetRect();		//占有矩形範囲を設定
	////////////////////////////////////////////////////////////////////
	//                    1997/01/12                                  //
	//  占有矩形範囲の設定はCBSchView::OnEditAttribute()でおこなう    //
	//  目的：CBSchCommentは一度描画しないと占有範囲が設定できない!!  //
	////////////////////////////////////////////////////////////////////
		pDoc->SetTempDirty();			//一時データの変更フラグを立てる
		return TRUE;
	}else{
		return FALSE;
	}
}



void CBSchView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	TRACE("CBSchView::OnSize() %d,%d",cx,cy);
	delete m_pBmpBuff;
	m_pBmpBuff = NULL;
}

void CBSchView::OnSheetFree() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
		// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CBSchDoc* pDoc = GetDocument();
	CSize size;
	size = pDoc->GetSheetSize();
	CSetFreeSize dlg;
	dlg.m_nX=size.cx;
	dlg.m_nY=size.cy;
	if(dlg.DoModal()==IDOK){
		size=CSize(dlg.m_nX,dlg.m_nY);
		pDoc->SetSheetSize(-1,size.cx,size.cy);			//図面サイズの設定
		pDoc->IniWriteSheetSize(&size);			//図面サイズをレジストリに書き込む
		//pDoc->SetModifiedFlag(TRUE);
		//pDoc->SetDirty();
		pDoc->UpdateAllViews(NULL);				//Viewを更新する
	}

}

BOOL CBSchView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	BOOL retv = FALSE;
	if((MK_CONTROL|MK_RBUTTON ) & nFlags){
		if(MK_RBUTTON & nFlags){
			if(m_nRButtonMode==0){
				KillTimer(ID_TIMER_RBUTTONDOWN);
			}
			m_nRButtonMode = 2;
		}
		m_nWHEEL_DELTA+=zDelta;
		if(m_nWHEEL_DELTA >= WHEEL_DELTA){
			while(m_nWHEEL_DELTA >= WHEEL_DELTA){
				m_nWHEEL_DELTA-=WHEEL_DELTA;
				SendMessage(WM_KEYDOWN,VK_PRIOR);
			}
			TRACE("CBSchView::OnMouseWheel() UP\n");
			retv = TRUE;
		}else if(m_nWHEEL_DELTA <= -WHEEL_DELTA){
			while(m_nWHEEL_DELTA <= -WHEEL_DELTA){
				m_nWHEEL_DELTA+=WHEEL_DELTA;
				SendMessage(WM_KEYDOWN,VK_NEXT);
			}
			TRACE("CBSchView::OnMouseWheel() DOWN\n");
			retv = TRUE;
		}
		if(m_nRButtonMode==2){
			POINT pt;
			::GetCursorPos(&pt);
			ScreenToClient(&pt);
			UINT nIDTimer=SetTimer(ID_TIMER_RBUTTONDOWN,300,NULL);
			m_ptRBScrlStartMouse = pt;
			m_ptRBScrlStartPosition =GetDeviceScrollPosition();
		}
		return retv;
	}else{
		return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	}
}

void CBSchView::OnUpdateMenuCrossCursor(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(m_bUseCrossCursor);
}

void CBSchView::OnMenuCrossCursor()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_bUseCrossCursor=(m_bUseCrossCursor==FALSE);	//グリッド表示フラグをトグル
	IniWriteUseCrossCursor(m_bUseCrossCursor);	//レジストリにグリッド表示フラグを書き込む
	InvalidateRect(NULL);
}


//void CBSchView::OnSetCoarseStepZoom()
//{
//	// TODO : ここにコマンド ハンドラ コードを追加します。
//	m_bCoarseStepZoom=(m_bCoarseStepZoom==FALSE);
//	IniWriteCoarseStepZoom(m_bCoarseStepZoom);
//}
//
//void CBSchView::OnUpdateSetCoarseStepZoom(CCmdUI *pCmdUI)
//{
//	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
//	pCmdUI->SetCheck(m_bCoarseStepZoom);
//}


void CBSchView::OnSetColor()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	//CSetColor dlg;
	CSetColor2 dlg(this);
	dlg.m_pColor = &m_COL;

	//dlg.m_COL_BG			= m_COL.colBG;
	//dlg.m_COL_JUNC			= m_COL.colJUNC;
	//dlg.m_COL_WIRE			= m_COL.colWIRE;
	//dlg.m_COL_BUS			= m_COL.colBUS;
	//dlg.m_COL_DASH			= m_COL.colDASH;
	//dlg.m_COL_TAG			= m_COL.colTAG;
	//dlg.m_COL_TAG_NAME		= m_COL.colTAG_NAME;
	//dlg.m_COL_COMMENT		= m_COL.colCOMMENT;		
	//dlg.m_COL_LABEL			= m_COL.colLABEL;
	//dlg.m_COL_XOR			= m_COL.colXOR;
	//dlg.m_COL_PART			= m_COL.colPART;
	//dlg.m_COL_PART_PIN		= m_COL.colPART_PIN;
	//dlg.m_COL_PART_PINNAME	= m_COL.colPART_PINNAME;
	//dlg.m_COL_PART_PINNUM	= m_COL.colPART_PINNUM;
	//dlg.m_COL_PART_NAME		= m_COL.colPART_NAME;
	//dlg.m_COL_PART_NUM		= m_COL.colPART_NUM;
	//dlg.m_COL_PART_FRAME	= m_COL.colPART_FRAME;
	//dlg.m_COL_SHEET_FRAME	= m_COL.colSHEET_FRAME;
	//dlg.m_COL_SHEET_GRID	= m_COL.colSHEET_GRID;
	//dlg.m_COL_DARK			= m_COL.colDARK;
	//dlg.m_COL_ZLPIN			= m_COL.colZLPIN;
	//dlg.m_COL_NC_MARK		= m_COL.colNC_MARK;
	//dlg.m_COL_QUICKPROPVIEW	= m_COL.colQUICKPROPVIEW;
	//dlg.m_COL_TEMPFRAME_FOCUSED		 = m_COL.colTEMPFRAME_FOCUSED;
	//dlg.m_COL_TEMPFRAME_NOT_FOCUSED	 = m_COL.colTEMPFRAME_NOT_FOCUSED;
	dlg.m_PRINTBG_WHITE		= AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintBgWhite"),1);
	m_bSetColor2Active = true;
	if(dlg.DoModal()==IDOK){
		//m_COL.colBG			= dlg.m_COL_BG;
		//m_COL.colJUNC			= dlg.m_COL_JUNC;
		//m_COL.colWIRE			= dlg.m_COL_WIRE;
		//m_COL.colBUS			= dlg.m_COL_BUS;
		//m_COL.colDASH			= dlg.m_COL_DASH;
		//m_COL.colTAG			= dlg.m_COL_TAG;
		//m_COL.colTAG_NAME		= dlg.m_COL_TAG_NAME;
		//m_COL.colCOMMENT		= dlg.m_COL_COMMENT;		
		//m_COL.colLABEL			= dlg.m_COL_LABEL;
		//m_COL.colXOR			= dlg.m_COL_XOR;
		//m_COL.colPART			= dlg.m_COL_PART;
		//m_COL.colPART_PIN		= dlg.m_COL_PART_PIN;
		//m_COL.colPART_PINNAME	= dlg.m_COL_PART_PINNAME;
		//m_COL.colPART_PINNUM	= dlg.m_COL_PART_PINNUM;
		//m_COL.colPART_NAME		= dlg.m_COL_PART_NAME;
		//m_COL.colPART_NUM		= dlg.m_COL_PART_NUM;
		//m_COL.colPART_FRAME	= dlg.m_COL_PART_FRAME;
		//m_COL.colSHEET_FRAME	= dlg.m_COL_SHEET_FRAME;
		//m_COL.colSHEET_GRID	= dlg.m_COL_SHEET_GRID;
		//m_COL.colDARK			= dlg.m_COL_DARK;
		//m_COL.colZLPIN			= dlg.m_COL_ZLPIN;
		//m_COL.colNC_MARK		= dlg.m_COL_NC_MARK;
		//m_COL.colQUICKPROPVIEW	= dlg.m_COL_QUICKPROPVIEW;
		//m_COL.colTEMPFRAME_FOCUSED		= dlg.m_COL_TEMPFRAME_FOCUSED;
		//m_COL.colTEMPFRAME_NOT_FOCUSED	= dlg.m_COL_TEMPFRAME_NOT_FOCUSED;

		IniWriteColor();
		AfxGetApp()->WriteProfileInt(_T("Option"),_T("PrintBgWhite"),(dlg.m_PRINTBG_WHITE ? 1 : 0));

		//updateBuff(true);
		//InvalidateRect(NULL);
	}
	m_bSetColor2Active = false;
	//IniWriteColor();
	//AfxGetApp()->WriteProfileInt("Option","PrintBgWhite",(dlg.m_PRINTBG_WHITE ? 1 : 0));

	updateBuff(true);
	InvalidateRect(NULL);
}

void CBSchView::OnUpdateEditFind(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(m_pFind2Dlg!=NULL && m_pFind2Dlg->GetSafeHwnd()!=NULL);

}


void CBSchView::OnEditFind()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。


	if(!m_pFind2Dlg){
		m_pFind2Dlg = new CFind2(this);
		m_pFind2Dlg->m_pView = this;
	}
	if(m_pFind2Dlg->GetSafeHwnd()){
		m_pFind2Dlg->DestroyWindow();
	}else{
		m_FindMarkActive = false;
		m_pFind2Dlg->ShowDialog();
	}

	//if(m_nCurrentTool!=ID_TOOL_SELECTOR){
	//	m_nCurrentTool=ID_TOOL_SELECTOR;
	//}
	//CFindDlg dlg;
	//FindInfo fi;
	//IniReadFindInfo(fi);
	//dlg.m_strFind=fi.m_str;
	//dlg.m_bFindTag			= ((fi.m_targetFlag & FINDTGT_TAG)!=0);
	//dlg.m_bFindLabel		= ((fi.m_targetFlag & FINDTGT_LABEL)!=0);
	//dlg.m_bFindComment		= ((fi.m_targetFlag & FINDTGT_COMMENT)!=0);
	//dlg.m_bFindName			= ((fi.m_targetFlag & FINDTGT_NAME)!=0);
	//dlg.m_bFindNameLib		= ((fi.m_targetFlag & FINDTGT_NAMELIB)!=0);
	//dlg.m_bFindNum			= ((fi.m_targetFlag & FINDTGT_NUM)!=0);
	//dlg.m_bFindManufacture	= ((fi.m_targetFlag & FINDTGT_MANUFACTURE)!=0);
	//dlg.m_bFindManufacturePartNumber	= ((fi.m_targetFlag & FINDTGT_MANUFACTUREPN)!=0);
	//dlg.m_bFindPackage		= ((fi.m_targetFlag & FINDTGT_PACKAGE)!=0);
	//dlg.m_bFindNote			= ((fi.m_targetFlag & FINDTGT_NOTE)!=0);
	//dlg.m_bMatchWhole		= fi.m_bMatchWhole;
	//dlg.m_bCaseSensitive	= fi.m_bCaseSensitive;
	//if(dlg.DoModal()==IDOK){
	//	fi.m_targetFlag = 0;
	//	if(dlg.m_bFindTag)		fi.m_targetFlag |=	FINDTGT_TAG;
 //       if(dlg.m_bFindLabel)	fi.m_targetFlag |=	FINDTGT_LABEL;
	//	if(dlg.m_bFindComment)	fi.m_targetFlag |=	FINDTGT_COMMENT;
	//	if(dlg.m_bFindName)		fi.m_targetFlag |=	FINDTGT_NAME;
 //       if(dlg.m_bFindNameLib)	fi.m_targetFlag |=	FINDTGT_NAMELIB;
	//	if(dlg.m_bFindNum)		fi.m_targetFlag |=	FINDTGT_NUM;

	//	if(dlg.m_bFindManufacture)			fi.m_targetFlag |=	FINDTGT_MANUFACTURE;
	//	if(dlg.m_bFindManufacturePartNumber)fi.m_targetFlag |=	FINDTGT_MANUFACTUREPN;
	//	if(dlg.m_bFindPackage)				fi.m_targetFlag |=	FINDTGT_PACKAGE;
	//	if(dlg.m_bFindNote)					fi.m_targetFlag |=  FINDTGT_NOTE;

	//	fi.m_str = dlg.m_strFind;
	//	fi.m_bMatchWhole = dlg.m_bMatchWhole;
	//	fi.m_bCaseSensitive = dlg.m_bCaseSensitive;

	//	if(	!fi.m_str.IsEmpty() && fi.m_targetFlag==0) return;
	//	IniWriteFindInfo(fi);
	//	CBSchDoc* pDoc = GetDocument();
	//	if(pDoc->FindStart(fi.m_str,fi.m_bMatchWhole,fi.m_bCaseSensitive,fi.m_targetFlag,true)){
	//		SXBSchObj* pObj = pDoc->GetTmp();
	//		if(pObj!=NULL){
	//			SPoint pt = pObj->p1();
	//			m_bZoomUpdate=TRUE;
	//			m_pointUpdateCenter=CPoint(pt.x(),pt.y());
	//			pDoc->UpdateAllViews(NULL);
	//			m_bZoomUpdate=FALSE;
	//			return;
	//		}
	//	}else{
	//		AfxMessageBox(IDS_FINISH_FIND,MB_ICONINFORMATION|MB_OK);
	//	}
	//	updateBuff(true);
	//	InvalidateRect(NULL);
	//}

}

bool CBSchView::isFindMarkActive()
{
	if(!m_FindMarkActive) return false;
	//if(!m_pFind2Dlg){
	//	m_FindMarkActive = false;
	//	return false;
	//}
	//if(!m_pFind2Dlg->GetSafeHwnd()){
	//	m_FindMarkActive = false;
	//	return false;
	//}
	return true;
}


//void CBSchView::OnEditFindnext()
//{
//	// TODO : ここにコマンド ハンドラ コードを追加します。
//	if(m_nCurrentTool!=ID_TOOL_SELECTOR){
//		m_nCurrentTool=ID_TOOL_SELECTOR;
//	}
//	CBSchDoc* pDoc = GetDocument();
//	if(!pDoc->CanFindNext()) return;
//	unsigned int dummy;
//	if(pDoc->Find()){
//		SXBSchObj* pObj = pDoc->GetTmp();
//		if(pObj!=NULL){
//			SPoint pt = pObj->p1();
//			m_bZoomUpdate=TRUE;
//			m_pointUpdateCenter=CPoint(pt.x(),pt.y());
//			pDoc->UpdateAllViews(NULL);
//			m_bZoomUpdate=FALSE;
//			return;
//		}
//	}else{
//		AfxMessageBox(IDS_FINISH_FIND,MB_ICONINFORMATION|MB_OK);
//	}
//	updateBuff(true);
//	InvalidateRect(NULL);
//}

//void CBSchView::OnUpdateEditFindnext(CCmdUI *pCmdUI)
//{
//	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
//	CBSchDoc* pDoc = GetDocument();
//	pCmdUI->Enable(pDoc->CanFindNext());
//}



void CBSchView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CScrollView::OnRButtonDown(nFlags, point);
//20100521	if(m_bCaptureMouse) return;
	if(m_mouseCaptureInfo.isLButtonCapture()) return;

	if(m_nRButtonMode) return;

	UINT nIDTimer=SetTimer(ID_TIMER_RBUTTONDOWN,300,NULL);

//20100521	SetCapture();				//CWndのメンバー関数(API関数ではない)
//20100521	m_bRBCaptureMouse = TRUE;
	m_mouseCaptureInfo.SetCapture(MouseCaptureInfo::MCI_RIGHT);

	m_ptRBScrlStartMouse = point;
	m_ptRBScrlStartPosition =GetDeviceScrollPosition();
}

void CBSchView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//20100521	if(m_bRBCaptureMouse){
	if(m_mouseCaptureInfo.isRButtonCapture()){
//20100521			m_bRBCaptureMouse = FALSE;
//20100521			::ReleaseCapture();		//キャプチャを解放
		m_mouseCaptureInfo.ReleaseCapture(MouseCaptureInfo::MCI_RIGHT);
		if(m_nRButtonMode==0){
			KillTimer(ID_TIMER_RBUTTONDOWN);
		}
	}


	if(m_nRButtonMode){
		m_nRButtonMode = 0;
	}else{
//20100521		if(!m_bCaptureMouse){
		if(!m_mouseCaptureInfo.isLButtonCapture()){
			if(m_nCurrentTool==ID_TOOL_MARKER){
				if(DoAttributeDialogMarkerLine(&m_tempMarkerLine)){
					IniWriteMarkerLineInitValue();
				}
				return;


			}else if(m_nCurrentTool==ID_TOOL_DASH){
				DoAttributeDialogDecoLine(&m_tempDecoLine);
				return;
				//if(DoAttributeDialogDecoLine(&m_tempDecoLine)){
				//	return;
				//}
			}
		}
		CScrollView::OnRButtonUp(nFlags, point);
	}
}

void CBSchView::OnEditEmfCopy()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	CopyEmf();
}

void CBSchView::OnUpdateEditEmfCopy(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(MenuItemTest_Copy());
}


void CBSchView::SetDBarLayer_LayerName(void)
{
	CMainFrame* pfrm = (CMainFrame*)AfxGetMainWnd();


	CBSchDoc* pDoc = GetDocument();

	for (int i = 0; i < 8; i++) {
		CEdit* pEdit = (CEdit*)(pfrm->m_dbarLayer.GetDlgItem(IDC_N0 + i));
		if(pEdit!=NULL){
			pEdit->SetWindowText(pDoc->layerName(i));
		}
		else {
			break;
		}
	}
}


void g_SetDBarLayer_LayerName(void)
{
	if (!g_pViewWindow)return;
	((CBSchView*)g_pViewWindow)->SetDBarLayer_LayerName();
}


int CBSchView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  ここに特定な作成コードを追加してください。
	g_pViewWindow = this;

	HWND hwnd=GetSafeHwnd();
	ImmAssociateContext(hwnd, NULL);

	if(m_pQuickPropComp){
		CString strClassName=AfxRegisterWndClass(
				CS_VREDRAW|CS_HREDRAW,
				AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
				NULL);
		CRect rcInit = CRect(0,0,10,10);
		//m_pQuickPropComp->Create(strClassName,_T("QuickPropComponent"),WS_CHILD|WS_BORDER,rcInit,this,ID_QUICKPROPCOMPONENT);
		m_pQuickPropComp->CreateEx(0,strClassName,NULL,WS_POPUP,rcInit,this,NULL,NULL); //090429	
		m_pQuickPropComp->m_pView = this;	//090429	
		g_IniReadQuickPropFontInfo(&m_lfQuickPropFont);
	}

	SetDefaultFontSettingsToCommentClass();

	 
	m_pObjFilterDlg = new CDlgObjFilter(this);
	m_pObjFilterDlg->m_pObjectFilterValue = &m_ObjectFileterValue;

	//m_pFind2Dlg = new CFind2(this);
	m_pFind2Dlg = NULL;
	
	return 0;
}


//コメントのクラスにデフォルトのフォントを設定する
void CBSchView::SetDefaultFontSettingsToCommentClass()
{
	LOGFONT lf;
	g_IniReadFontInfo(&lf);

	SXBSchComment::m_defaultFontName = lf.lfFaceName;
	SXBSchComment::m_defaultFontSize = lf.lfHeight;
	SXBSchComment::m_defaultFontBold = (lf.lfWeight>=FW_BOLD);
	SXBSchComment::m_defaultFontItalic= (lf.lfItalic != 0);
	SXBSchComment::m_defaultFontUnderline= false;
	SXBSchComment::m_defaultFontStrikeOut= false;
}



void CBSchView::OnDispNcmark()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	g_bDisplayNcPinMark = !g_bDisplayNcPinMark;
	IniWriteDisplayNcPinMark(g_bDisplayNcPinMark);		//レジストリに部品枠の表示フラグを書き込む
	CBSchDoc* pDoc = GetDocument();			//ドキュメントのポインタを得て
	if(g_bDisplayNcPinMark)pDoc->setNcPinFlag();
	pDoc->UpdateAllViews(NULL);				// 画面を再描画

}

void CBSchView::OnUpdateDispNcmark(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(g_bDisplayNcPinMark);

}

void CBSchView::OnUpdateDisplayLayer(CCmdUI *pCmdUI,int layer)
{
	CBSchDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->visibleLayer()&(1<<layer));
}

void CBSchView::OnClickDisplayLayer(int layer)
{
	CBSchDoc* pDoc = GetDocument();
	if(pDoc->GetTmpDataCount()){	//一時データが存在していたら...
		//pDoc->FixSelection();		//一時データを解除
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//一時データを解除
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
	}
	unsigned int visibleLayer = pDoc->visibleLayer();
	if(pDoc->editLayer()==layer){
		visibleLayer |= (1<<layer);
	}else{
		visibleLayer ^= (1<<layer);
	}
	pDoc->setVisibleLayer(visibleLayer);
	updateBuff(true);
	Invalidate(FALSE);
}

void CBSchView::OnClickDisplayLayer0()
{
	OnClickDisplayLayer(0);
}
void CBSchView::OnClickDisplayLayer1()
{
	OnClickDisplayLayer(1);
}
void CBSchView::OnClickDisplayLayer2()
{
	OnClickDisplayLayer(2);
}
void CBSchView::OnClickDisplayLayer3()
{
	OnClickDisplayLayer(3);
}
void CBSchView::OnClickDisplayLayer4()
{
	OnClickDisplayLayer(4);
}
void CBSchView::OnClickDisplayLayer5()
{
	OnClickDisplayLayer(5);
}
void CBSchView::OnClickDisplayLayer6()
{
	OnClickDisplayLayer(6);
}
void CBSchView::OnClickDisplayLayer7()
{
	OnClickDisplayLayer(7);
}

void CBSchView::OnUpdateDisplayLayer0(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,0);
}
void CBSchView::OnUpdateDisplayLayer1(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,1);
}
void CBSchView::OnUpdateDisplayLayer2(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,2);
}
void CBSchView::OnUpdateDisplayLayer3(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,3);
}
void CBSchView::OnUpdateDisplayLayer4(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,4);
}
void CBSchView::OnUpdateDisplayLayer5(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,5);
}
void CBSchView::OnUpdateDisplayLayer6(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,6);
}
void CBSchView::OnUpdateDisplayLayer7(CCmdUI *pCmdUI)
{
	OnUpdateDisplayLayer(pCmdUI,7);
}

void CBSchView::OnUpdateEditLayer(CCmdUI *pCmdUI,int layer)
{
	CBSchDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->editLayer()==layer);
}

void CBSchView::OnClickEditLayer(int layer)
{
	CBSchDoc* pDoc = GetDocument();
	if(pDoc->editLayer()==layer)return;

	if(pDoc->GetTmpDataCount()){	//一時データが存在していたら...
		//pDoc->FixSelection();		//一時データを解除
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//一時データを解除
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
	}
	pDoc->setEditLayer(layer);
	unsigned int visibleLayer = pDoc->visibleLayer();
	unsigned laylerBit = 1<<layer;
	if(!(visibleLayer & laylerBit)){
		visibleLayer |= (1<<layer);
		pDoc->setVisibleLayer(visibleLayer);
	}
	updateBuff(true);
	Invalidate(FALSE);
}



void CBSchView::OnUpdateEditLayer0(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,0);
}
void CBSchView::OnUpdateEditLayer1(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,1);
}
void CBSchView::OnUpdateEditLayer2(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,2);
}
void CBSchView::OnUpdateEditLayer3(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,3);
}
void CBSchView::OnUpdateEditLayer4(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,4);
}
void CBSchView::OnUpdateEditLayer5(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,5);
}
void CBSchView::OnUpdateEditLayer6(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,6);
}
void CBSchView::OnUpdateEditLayer7(CCmdUI *pCmdUI)
{
	OnUpdateEditLayer(pCmdUI,7);
}
void CBSchView::OnClickEditLayer0()
{
	OnClickEditLayer(0);
}
void CBSchView::OnClickEditLayer1()
{
	OnClickEditLayer(1);
}
void CBSchView::OnClickEditLayer2()
{
	OnClickEditLayer(2);
}
void CBSchView::OnClickEditLayer3()
{
	OnClickEditLayer(3);
}
void CBSchView::OnClickEditLayer4()
{
	OnClickEditLayer(4);
}
void CBSchView::OnClickEditLayer5()
{
	OnClickEditLayer(5);
}
void CBSchView::OnClickEditLayer6()
{
	OnClickEditLayer(6);
}
void CBSchView::OnClickEditLayer7()
{
	OnClickEditLayer(7);
}


void CBSchView::OnUpdateSheetFree(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	int index = pDoc->GetSheetSizeIndex();
	pCmdUI->SetCheck(index==-1);
}

void CBSchView::OnUpdateSheet640(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	int index = pDoc->GetSheetSizeIndex();
	pCmdUI->SetCheck(index==0);
}

void CBSchView::OnUpdateSheet1000(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	int index = pDoc->GetSheetSizeIndex();
	pCmdUI->SetCheck(index==1);

}

void CBSchView::OnUpdateSheet1600(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	int index = pDoc->GetSheetSizeIndex();
	pCmdUI->SetCheck(index==2);
}

void CBSchView::OnUpdateSheet2000(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	int index = pDoc->GetSheetSizeIndex();
	pCmdUI->SetCheck(index==3);
}

void CBSchView::OnUpdateSheet3000(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	int index = pDoc->GetSheetSizeIndex();
	pCmdUI->SetCheck(index==4);
}

void CBSchView::OnEditRedo()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	if(pDoc->Redo()){								//Undo()呼び出し
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
		pDoc->UpdateAllViews(NULL);					//描画の更新
	}
}

void CBSchView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(MenuItemTest_Redo());
	
}

void CBSchView::OnToolMarker()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(m_nCurrentTool==ID_TOOL_MARKER) return;
	ChangeTool(m_nCurrentTool,ID_TOOL_MARKER);//ツールを変えたときの共通処理
	m_nCurrentTool=ID_TOOL_MARKER;
}

void CBSchView::OnUpdateToolMarker(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(m_nCurrentTool==ID_TOOL_MARKER);
}

void CBSchView::OnDispHiddenpinnum()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	g_bDisplayHiddenPinNum = !g_bDisplayHiddenPinNum;
	IniWriteDisplayHiddenPinNum(g_bDisplayHiddenPinNum);		//レジストリに
	CBSchDoc* pDoc = GetDocument();			//ドキュメントのポインタを得て
	pDoc->UpdateAllViews(NULL);				// 画面を再描画
}

void CBSchView::OnUpdateDispHiddenpinnum(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(g_bDisplayHiddenPinNum);
}

void CBSchView::OnSetupQuick()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CSetQuickProp dlg;
	dlg.m_checkOnMouseHover = m_quickPropSetting.m_showOnMouseHover;
	dlg.m_checkUseQuickProp = m_quickPropSetting.m_enable;
	dlg.m_checkNum = m_quickPropSetting.m_num;
	dlg.m_checkVal = m_quickPropSetting.m_val;
	dlg.m_checkMFR = m_quickPropSetting.m_mfr;
	dlg.m_checkMFRPN = m_quickPropSetting.m_mfrpn;
	dlg.m_checkPackage = m_quickPropSetting.m_package;
	dlg.m_checkNote = m_quickPropSetting.m_note;
	dlg.m_checkNameinlib = m_quickPropSetting.m_nameinlib;
	if(dlg.DoModal()==IDOK){
		m_quickPropSetting.m_enable = dlg.m_checkUseQuickProp;
		m_quickPropSetting.m_showOnMouseHover = dlg.m_checkOnMouseHover;
		m_quickPropSetting.m_num = dlg.m_checkNum;
		m_quickPropSetting.m_val = dlg.m_checkVal;
		m_quickPropSetting.m_mfr = dlg.m_checkMFR;
		m_quickPropSetting.m_mfrpn = dlg.m_checkMFRPN;
		m_quickPropSetting.m_package = dlg.m_checkPackage;
		m_quickPropSetting.m_note = dlg.m_checkNote;
		m_quickPropSetting.m_nameinlib = dlg.m_checkNameinlib;
		IniWriteQuickPropSetting();
	}

}

void CBSchView::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CScrollView::OnMouseHover(nFlags, point);
	if(m_quickPropSetting.isEnabled() && m_quickPropSetting.m_showOnMouseHover){
		CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
		OnPrepareDC(&dc);	//スクロールビューの反映
		dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
		LPtoBSchP(&point,m_nVExt,m_nWExt);		//マウスの座標を図面座標に変換
		CBSchDoc* pDoc = GetDocument();

		const SXBSchComponent* pComp = pDoc->isOnComponent(&point);
		if(pComp){
			ShowQuickPropComp(pComp);
		}
	}
	TRACE("OnMouseHover()\n");
}

void CBSchView::OnSetupAllowappendselect()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	m_allowAppendSelect = !m_allowAppendSelect;
	IniWriteAllowAppendSelect();
}

void CBSchView::OnUpdateSetupAllowappendselect(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(m_allowAppendSelect);
}

//レジストリから追加選択許可情報を得る
BOOL CBSchView::IniReadAllowAppendSelect()
{
	return AfxGetApp()->GetProfileInt(_T("Option"),_T("UseAllowAppendSelect"),FALSE);
}

//レジストリに追加選択許可情報を書き込む
void CBSchView::IniWriteAllowAppendSelect()
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("UseAllowAppendSelect"),m_allowAppendSelect);
}

void CBSchView::OnConvertC2l()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	//コメントをラベルに変換
	CBSchDoc* pDoc = GetDocument();
	pDoc->bulkConvertComment2Label();
	InvalidateRect(NULL);
}

void CBSchView::OnUpdateConvertC2l(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isCommentSelected());
}


void CBSchView::OnConvertC2t()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	pDoc->bulkConvertComment2Tag();
	InvalidateRect(NULL);
}

void CBSchView::OnUpdateConvertC2t(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isCommentSelected());
}


void CBSchView::OnConvertL2c()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	pDoc->bulkConvertLabel2Comment();
	InvalidateRect(NULL);
}

void CBSchView::OnUpdateConvertL2c(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isLabelSelected());
}


void CBSchView::OnConvertL2t()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	pDoc->bulkConvertLabel2Tag();
	InvalidateRect(NULL);
}

void CBSchView::OnUpdateConvertL2t(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isLabelSelected());
}


void CBSchView::OnConvertT2l()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	pDoc->bulkConvertTag2Label();
	InvalidateRect(NULL);
}

void CBSchView::OnUpdateConvertT2l(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isTagSelected());
}


void CBSchView::OnConvertT2c()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	pDoc->bulkConvertTag2Comment();
	InvalidateRect(NULL);
}

void CBSchView::OnUpdateConvertT2c(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isTagSelected());
}


void CBSchView::OnBulkattributeTagframe()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CSetTagFrame dlg;
	if(dlg.DoModal()==IDOK){
		int nType = dlg.m_nType;
		CBSchDoc* pDoc = GetDocument();
		pDoc->bulkAttributeChangeTagFrameTo(nType);
		InvalidateRect(NULL);
	}
}

void CBSchView::OnUpdateBulkattributeTagframe(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isTagSelected());
}

void CBSchView::OnBulkattributeMarkerline()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if(DoAttributeDialogMarkerLine(&m_tempMarkerLine)){
		IniWriteMarkerLineInitValue();
		CBSchDoc* pDoc = GetDocument();
		pDoc->bulkAttributeMarkerLine(m_tempMarkerLine.m_width,m_tempMarkerLine.m_nColor);
		InvalidateRect(NULL);
	}
}

void CBSchView::OnUpdateBulkattributeMarkerline(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isMarkerlineSelected());
}

void CBSchView::OnBulkattributeCommentfont()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	LOGFONT logfont;
	g_IniReadFontInfo(&logfont);
	CFontDialog dlg(&logfont,CF_TTONLY | CF_SCREENFONTS);
	
	if(dlg.DoModal()==IDOK){
		SXBSchComment cmnt;
		cmnt.setFontName(logfont.lfFaceName);
		cmnt.setFontSize(abs(logfont.lfHeight));
		cmnt.setFontBold(logfont.lfWeight>=FW_BOLD);
		cmnt.setFontItalic(logfont.lfItalic!=0);

		CBSchDoc* pDoc = GetDocument();
		pDoc->bulkAttributeCommentFont(cmnt);
		InvalidateRect(NULL);
	}
}

void CBSchView::OnUpdateBulkattributeCommentfont(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isCommentSelected());
}


void CBSchView::OnBulkattributeReplacecomponentinfo()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CSelPart dlg;
	dlg.m_bUsePreview=TRUE;		//プレビューを使うかどうかの設定
	if(g_LibraryInfo(m_nLastSelectLibrary)){//NULLでなければ...
		dlg.m_nLastSelectLibrary=m_nLastSelectLibrary;
		dlg.m_nLastSelectPart=m_nLastSelectPart;
	}else{
		dlg.m_nLastSelectLibrary=0;
		dlg.m_nLastSelectPart=0;
	}
	if(dlg.DoModal()==IDOK){
		const SCompIndex* pPartIndex = dlg.m_pCompIndex;
		if(pPartIndex==NULL){
			CString rString;
			//メッセージボックス用のメッセージの作成
			AfxFormatString1(rString,IDS_NOT_FOUND_PART,dlg.m_rStrPartName); 
			AfxMessageBox(rString,MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		m_nLastSelectPart=    dlg.m_nLastSelectPart;
		m_nLastSelectLibrary= dlg.m_nLastSelectLibrary;

		CBSchDoc* pDoc = GetDocument();
		pDoc->bulkAttributeReplaceComponentInfo(pPartIndex);
		InvalidateRect(NULL);

	}
}


void CBSchView::OnUpdateBulkattributeReplacecomponentinfo(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isComponentSelected());
}

void CBSchView::OnInsertBitmap()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。

	CString strFilter;
	strFilter = _T("Bitmap(*.bmp)|*.BMP|JPEG(*.jpg;*.jpeg)|*.JPG;*.JPEG|PNG(*.png)|*.PNG||");


	CFileDialog dlg(TRUE,				//オープンダイアログ
					_T("bmp"),				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
					strFilter,	//フィルタ文字列
					this);				//親ウインドウ
	
	CString rCaption;
	rCaption.LoadString(IDS_IMAGE_IMPORT);//ダイアログボックスのキャプションをリソースからロード
	dlg.m_ofn.lpstrTitle=rCaption;		//ファイルダイアログクラスにキャプションの設定
	
	if(dlg.DoModal()==IDOK){
		CString path = dlg.GetPathName();

		CBSchDoc* pDoc = GetDocument();

		CRect  rc;							//クライアントエリアの座標
		CPoint point=GetScrollPosition();	//配置座標。まずクライアントエリア左上隅の論理座標を入れる。
		GetClientRect(&rc);					//クライアントエリアの座標を得る(left,topは常に０)
		CSize sizeSheet;
		sizeSheet=pDoc->GetSheetSize();	//図面サイズを得る

		//クライアントエリア左上の座標
		CPoint pointLT=point;	//
		LPtoBSchP(&pointLT,m_nVExt,m_nWExt);	//図面座標に変換
	
		//クライアントエリア中央の論理座標を得る
		CPoint pointCenter;
		pointCenter.x=rc.right;		//右下クライアント座標を入れて
		pointCenter.y=rc.bottom;
		pointCenter+=point;			//スクロールを考慮した論理座標に変換
		LPtoBSchP(&pointCenter,m_nVExt,m_nWExt);	//図面座標に変換
		if(pointCenter.x>sizeSheet.cx) pointCenter.x=sizeSheet.cx;	//図面サイズから出ないことを保証
		if(pointCenter.y>sizeSheet.cy) pointCenter.y=sizeSheet.cy;	
		pointCenter.x=pointLT.x+(pointCenter.x-pointLT.x)/2;	//論理座標中央に変換
		pointCenter.y=pointLT.y+(pointCenter.y-pointLT.y)/2;
		point=pointCenter;
		if(point.x<10) point.x=10;			//左・上10ドット以内なら10ドットの位置に補正
		if(point.y<10) point.y=10;
		SnapToGrid(&point);					//必要ならグリッドへのスナップ


		SXBSchImageObj* pImageObj = new SXBSchImageObj;
		pImageObj->setLayer(pDoc->editLayer());
		pImageObj->setP1(SPoint(point.x,point.y));
		if(!pImageObj->LoadDIB(path)){
			delete pImageObj;
			return;
		}
		pDoc->AddData(pImageObj);
		InvalidateRect(NULL);
	}
}

void CBSchView::OnUpdateBulkattributeHidenumbers(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isComponentSelected());
}

void CBSchView::OnBulkattributeHidenumbers()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBulkHidingNumVal dlg;
	if(dlg.DoModal()==IDOK){
		CBSchDoc* pDoc = GetDocument();
		if(dlg.m_bHideNumCheckState!=BST_INDETERMINATE){
			pDoc->bulkAttributeHideComponentNum(dlg.m_bHideNumCheckState==BST_CHECKED);
		}

		if(dlg.m_bHideValCheckState!=BST_INDETERMINATE){
			pDoc->bulkAttributeHideComponentVal(dlg.m_bHideValCheckState==BST_CHECKED);
		}

		//pDoc->bulkAttributeReplaceComponentInfo(pPartIndex);
		InvalidateRect(NULL);

	}
}

void CBSchView::OnToolUseobjectfilter()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if(m_nCurrentTool==ID_TOOL_SELECTOR){
		if(m_pObjFilterDlg->GetSafeHwnd() == NULL){
			m_pObjFilterDlg->ShowDialog();
			m_ObjectFileterValue |= OBJMASK_USEFILTER;
		}else{
			m_pObjFilterDlg->DestroyWindow();
			m_ObjectFileterValue &= ~OBJMASK_USEFILTER;
		}
	}
}

void CBSchView::OnUpdateToolUseobjectfilter(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(m_ObjectFileterValue & OBJMASK_USEFILTER);
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR);
}

void CBSchView::OnUpdateEditPoints(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	BOOL enable = pDoc->CanEditPoints();
	if(!enable){
		m_bEditPointsMode = false;
	}
	pCmdUI->Enable(enable);
	pCmdUI->SetCheck(m_bEditPointsMode);
}

void CBSchView::OnEditPoints()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	BOOL enable = pDoc->CanEditPoints();
	if(!enable){
		m_bEditPointsMode = false;
	}else{
		m_bEditPointsMode = !m_bEditPointsMode;
	}
	InvalidateRect(NULL);
}

void CBSchView::OnSetupPlacelinebyclick()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	m_bClickClickLineMode = !m_bClickClickLineMode;
	IniWriteClickClickLineMode(m_bClickClickLineMode);
}

void CBSchView::OnUpdateSetupPlacelinebyclick(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(m_bClickClickLineMode);
}

void CBSchView::OnUpdateOrderBringtofront(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->CanChangeOrder());
}

void CBSchView::OnOrderBringtofront()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	FixSelection(SXBSchDoc::RESET_SELECT_BRING_TO_FRONT);		//一時データを解除
	if(g_bDisplayNcPinMark){
		CBSchDoc* pDoc = GetDocument();
		pDoc->setNcPinFlag();		//NCピンのフラグの設定
	}
	updateBuff(true);
	m_bDrawOccasional = FALSE;
	m_nPlaceSequence = 0;
	Invalidate(FALSE);
}

void CBSchView::OnOrderBringtoback()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	FixSelection(SXBSchDoc::RESET_SELECT_BRING_TO_BACK);		//一時データを解除
	if(g_bDisplayNcPinMark){
		CBSchDoc* pDoc = GetDocument();
		pDoc->setNcPinFlag();		//NCピンのフラグの設定
	}
	updateBuff(true);
	m_bDrawOccasional = FALSE;
	m_nPlaceSequence = 0;
	Invalidate(FALSE);
}

void CBSchView::OnUpdateReloadReload(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	CString strPath = pDoc->GetPathName();
	TRACE("Pathname = %s\n",strPath);
	pCmdUI->Enable(!strPath.IsEmpty());// && pDoc->IsModified());
}

void CBSchView::OnReloadReload()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();
	if(pDoc->IsModified()){
		if(AfxMessageBox(IDS_DISCARD_AND_RELOAD,MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2)==IDCANCEL){
			return;
		}
	}
	pDoc->ReloadFile();
}

void CBSchView::DisplayMessageLine()
{
	CString strMsg;
	CString strLoad;
	if(m_nCurrentTool==ID_TOOL_WIRE || m_nCurrentTool==ID_TOOL_BUS){
		if(m_nPlaceSequence>0 && m_bClickClickLineMode){
			strLoad.LoadString(IDS_MSGLINE_WIRE_ESC);
			strMsg+=strLoad;
		}
		if(m_bSetWireDirection){
			strLoad.LoadString(IDS_MSGLINE_SPACE_CHANGE_DIR);
			strMsg+=strLoad;
		}
	}
	if(m_nCurrentTool==ID_TOOL_DASH || m_nCurrentTool==ID_TOOL_MARKER){
		if(m_nPlaceSequence>0 && m_bClickClickLineMode){
			strLoad.LoadString(IDS_MSGLINE_WIRE_ESC);
			strMsg+=strLoad;
		}
	}

	if(m_strMessageLine!=strMsg){
		CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
		m_strMessageLine=strMsg;
		pFrame->SetMessageLine(m_strMessageLine);
	}
}
	


LRESULT CBSchView::OnScrollToMsg(UINT wParam,LONG lParam)
{
	int x = (int)LOWORD(lParam);
	int y = (int)HIWORD(lParam);

	CPoint pt = CPoint(x,y);
	ScrollToFoundPos(pt);

	//CString msg;


	//msg.Format("OnScrollToMsg %d:%d",x,y);
	//AfxMessageBox(msg);
	return 0;
}


void CBSchView::OnBulkattributeDecorationline()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if(DoAttributeDialogDecoLine(&m_tempDecoLine)){
		CBSchDoc* pDoc = GetDocument();
		pDoc->bulkAttributeDecorationline(m_tempDecoLine);
		InvalidateRect(NULL);
	}
}

void CBSchView::OnUpdateBulkattributeDecorationline(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	CBSchDoc* pDoc = GetDocument();	
	pCmdUI->Enable(m_nCurrentTool==ID_TOOL_SELECTOR && pDoc->isDecorationlineSelected());
}

//LCoVによる部品情報の編集が可能なとき、有効な部品オブジェクトを返す
SXBSchComponent* CBSchView::CanEditWithLCoV()
{
	CBSchDoc* pDoc = GetDocument();
	SXBSchObj* pObj = NULL;
	bool enable=false;
	if(pDoc->CanAttribute()){
		pObj = pDoc->GetTmp();
		if(pObj->id()==SXBSchObj::ID_COMPONENT){ 
			LPCTSTR pLCoVPath;
			pLCoVPath = ((CBSchApp*)AfxGetApp())->LCoVPath();
			FILE* fp;
			fp = _tfopen(pLCoVPath,_T("r"));
			enable = (fp != NULL);
			if(fp != NULL) fclose(fp);
		}
	}
	if(enable) return (SXBSchComponent*)pObj;
	else return NULL;
}

void CBSchView::OnUpdateFloatEditWithLcov(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラー コードを追加します。


	pCmdUI->Enable(CanEditWithLCoV()!=NULL);
}


void CBSchView::OnFloatEditWithLcov()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。

	
	SXBSchComponent* pObj =CanEditWithLCoV();
	if(pObj==NULL) return;

	CBSchDoc* pDoc = GetDocument();

	CString orgName = pObj->orgname();

	SCompInfo* pInfo =pObj->compInfoIndex()->compInfo();

	SCompLib tempLib;	

	CBSchApp::editWidthLCoV(orgName, pInfo, tempLib);

	
	if(tempLib.count()<1) return;

	SCompIndex* pPartIndex = tempLib.compIndex(0);
	if(pPartIndex==NULL) return;

	int block = pObj->block();
	int blocks = pPartIndex->block();
	if(block>=blocks){
		pObj->setBlock(0);
	}

	pObj->setCompInfoIndex(pPartIndex);
	pDoc->SetTempDirty();
	updateBuff(true);
	InvalidateRect(NULL);
}


void CBSchView::OnSetupDispblock()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。

	CDispBlockNum dlg;
	dlg.m_dispBN = m_dispBN;
	if(dlg.DoModal() == IDOK){
		m_dispBN = dlg.m_dispBN;
	}

	IniWriteDisplayBlockNum(m_dispBN);
	CBSchDoc* pDoc = GetDocument();			//ドキュメントのポインタを得て
	pDoc->UpdateAllViews(NULL);				// 画面を再描画
}

std::wstring CBSchView::GetAppPath()
{
	std:wstring res;
	TCHAR fname[MAX_PATH];
	fname[0] = _T('\0');
	if (::GetModuleFileName(NULL, fname, MAX_PATH))
	{
		std::wstring wText(fname);
		size_t pos = wText.find_last_of(L"\\/");
		if (std::wstring::npos != pos)
		{
			res = wText.substr(0, pos+1);
		}
	}
	return res;
}

void CBSchView::ExecExternalTool(const std::wstring& FileName)
{
	std::wstring s(FileName);
	size_t pos = FileName.find_last_of(L"\\/");
	if (!FileName.empty() && (std::wstring::npos == pos))
	{ // パスがないので実行ファイルと同じパスとみなす。
		s = GetAppPath() + s;
	}
	if (!s.empty())
	{
		wchar_t* args[] = { NULL };
		STARTUPINFOW sInfo = {};
		PROCESS_INFORMATION pInfo = {};
		wchar_t* cmd = (wchar_t*) s.c_str();
		if (!CreateProcess(
			s.c_str() // ApplicationName
			, NULL // CommandLine arguments
			, NULL
			, NULL
			, FALSE
			, 0
			, NULL
			, NULL
			, &sInfo
			, &pInfo
		))
		{
			s = L"起動できませんでした\r\n" + s;
			MessageBox(s.c_str(), L"");
		}
	}
}

void CBSchView::OnToolRunLcov()
{
	ExecExternalTool(L"LCoV.exe");
}


void CBSchView::OnToolRunPl3w()
{
	ExecExternalTool(L"pl3w.exe");
}


void CBSchView::OnToolRunNut3w()
{
	ExecExternalTool(L"nut3w.exe");
}


void CBSchView::OnToolRunNl3w()
{
	ExecExternalTool(L"nl3w.exe");
}


void CBSchView::OnToolRunCe3search()
{
	ExecExternalTool(L"CE3Search.exe");
}


void CBSchView::OnFileExpoClipbord()
{
	// Todo: EMF
	ExportToClipbord();
}
