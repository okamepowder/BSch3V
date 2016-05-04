/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// CompEditView.cpp : CCompEditView クラスの実装
//

#include "stdafx.h"
#include "LCoV.h"
#include "CompEditView.h"
#include "draw.h"
#include "pattern.h"
#include ".\compeditview.h"
#include "LinePropertyDlg.h"
#include "PinProperty.h"
#include "PtnProperty.h"
#include "PtnTextDlg.h"
#include "PinProps.h"
#include "coord2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCompEditView

CCompEditView::CCompEditView()
{
	m_pageMargin = PAGE_MARGIN;
	m_tool = TOOL_SELECT;
	m_placeState = 0;
	m_pBmpBuff = NULL;
	m_offScrnBuffSize = CSize(0,0);
	m_offScrnBuffOrg = CPoint(INT_MIN,INT_MIN);
	m_zoomNum = 0;
	m_magV = 1;			//表示倍率の分子
	m_magW = 1;			//表示倍率の分母
	m_block = 0;
	m_pCompIndex = NULL;
	m_pageSize = CSize(10,10);	//適当な値
	m_windowOrg = CPoint(0,0);	//適当な値
	m_pCursorPin = NULL;
	m_dragOpStatus = DRAGOP_NOT;
	m_startSelect = false;
	m_editNodeMode = false;
	m_movingGrip = -1;
	m_nWHEEL_DELTA_z = 0;
	m_nWHEEL_DELTA_s = 0;
	m_tempAreaRectValid = false;
	m_enableContextMenu = false;
	m_captureFlag = 0;
	m_pPinPropsDlg = new CPinProps(this);
	m_displayPinProps = false;
	m_bDrawAddNodeIcon = false;	// 2009/07/20追加
	m_swapPinAndName = false;

	m_bIsCursorOnActivePolygon = false;

	{
		::g_bIsWin9x = false;
//		OSVERSIONINFO versionInfo;
//		versionInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
//		if(GetVersionEx(&versionInfo)){
//			::g_bIsWin9x = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
//		}
//		g_bIsWin9x=true;	//test
	}

}

CCompEditView::~CCompEditView()
{
	delete m_pBmpBuff;
	delete 	m_pCursorPin;
	delete m_pPinPropsDlg;
}

void  CCompEditView::setEditNodeMode(bool editnode)
{
	if(editnode != m_editNodeMode){
		m_editNodeMode = editnode;
		if(m_doc.m_listTemp.size()>0){
			Invalidate(FALSE);
		}
	}
}

void CCompEditView::setTool(int tool)
{
	if(m_tool == tool) return;

	delete 	m_pCursorPin;
	m_pCursorPin = NULL;

	//if(m_doc.releaseTemp()){
	if(releaseTemp()){
		//drawToBuff(true);
	}
	m_placeState = 0;
	m_tool = tool;
	switch(m_tool){
		case TOOL_PIN:
			m_pCursorPin = new SPin;
			m_pCursorPin->setBlock(1);
			m_pCursorPin->setPinName(_T(""));
			m_pCursorPin->setPinNum(0,_T(""));
			break;
	}
	m_movingGrip = -1;

	Invalidate(FALSE);
}




bool CCompEditView::canDelete()
{
	if(m_doc.m_listTemp.size()>0) return true;
	else return false;
}

bool CCompEditView::doDelete()
{
	bool pinSelected = m_doc.isPinSelected();
	if(!m_doc.delTemp()){
		return false;
	}
	Invalidate(FALSE);
	if(pinSelected){
		m_pPinPropsDlg->update();
	}
	return true;
}

bool CCompEditView::canCopy()
{
	if(m_doc.m_listTemp.size()>0) return true;
	else return false;
}

bool CCompEditView::canMirrorRotate()
{
	if(m_doc.m_listTemp.size()==0) return false;
	SCompEditElmListIterator ite;
	ite = m_doc.m_listTemp.begin();
	if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN) return false;
	return true;
}


bool CCompEditView::doMirror()
{
	if(!m_doc.mirror()) return false;
	Invalidate(FALSE);
	return true;
}

bool CCompEditView::doRotate()
{
	if(!m_doc.rotate()) return false;
	Invalidate(FALSE);
	return true;
}


bool CCompEditView::doCopy()
{
	if(!m_doc.copy()) return false;
	Invalidate(FALSE);
	return true;
}

bool CCompEditView::canUndo()
{
	return m_doc.canUndo();
}

bool CCompEditView::doUndo()
{
	m_doc.undo();
	drawToBuff(true);
	Invalidate(FALSE);
	m_pPinPropsDlg->update();
	return true;
}


bool CCompEditView::canPaste()
{
	static const TCHAR* pcszIDPIN= _T("+LCOV_PIN_V.1.0");
	static const TCHAR* pcszIDPTN= _T("+LCOV_PTN_V.1.0");

	int nIdLengthPIN = _tcslen(pcszIDPIN);
	int nIdLengthPTN = _tcslen(pcszIDPTN);

	bool retVal = false;
	if(::IsClipboardFormatAvailable(CF_UNICODETEXT)){
		::OpenClipboard(AfxGetMainWnd()->m_hWnd);		//Open clip board.
		HGLOBAL hGMem=::GetClipboardData(CF_UNICODETEXT);		//Get memory handle.
		if(hGMem!=NULL){
			int nLength=GlobalSize(hGMem);				//Size of clip board data.
			if(nLength>nIdLengthPIN*2){
				TCHAR* pgBuff=(TCHAR*)::GlobalLock(hGMem);	//Lock global memory.
				if(_tcsncmp(pcszIDPIN,pgBuff,nIdLengthPIN)==0){
					retVal = true;
				}
				if(_tcsncmp(pcszIDPTN,pgBuff,nIdLengthPTN)==0){
					retVal = true;
				}
				::GlobalUnlock(hGMem);				//Unock global memory.
			}
		}
		::CloseClipboard();
	}
	return retVal;
}






bool CCompEditView::doPaste()
{
	if(!canPaste())return false;
	setTool(TOOL_SELECT);
	//if(m_doc.releaseTemp()){
	if(releaseTemp()){
		//drawToBuff(true);
	}

	::OpenClipboard(AfxGetMainWnd()->m_hWnd);		//Open clip board.
	HGLOBAL hGMem=::GetClipboardData(CF_UNICODETEXT);		//Get memory handle.
	if(hGMem==NULL){
		::CloseClipboard();						//Close clip board.
		return false;
	}
	TCHAR* pgBuff=(TCHAR*)::GlobalLock(hGMem);	//Lock global memory.
	wstring cbbuff=wstring(pgBuff);				//Initialize string with global memory.
	::GlobalUnlock(hGMem);						//Unock global memory.
	::CloseClipboard();							//Close clip board.


	SReadCE3 rce3;
	rce3.Attach(cbbuff.c_str());

	wstring str;

	rce3.ReadRecord(str);
	if(str == _T("+LCOV_PIN_V.1.0")){
		return doPastePin(rce3);
	}else if(str == _T("+LCOV_PTN_V.1.0")){
		return doPastePtn(rce3);
	}
	return false;
}


bool CCompEditView::doPastePin(SReadCE3& rce3)
{
	delete m_pCursorPin;
	m_pCursorPin = NULL;
	bool retv = false;
	wstring str;
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			break;
		}else if(str[0] == '+'){
			if(str==_T("+PIN")){
				m_pCursorPin = new SPin;
				if(m_pCursorPin->readCe3(rce3,m_pCompIndex->block())){
					retv = true;
				}else{
					delete m_pCursorPin;
					m_pCursorPin = NULL;
				}
				break;
			}else{
				str[0]='-';
				if(rce3.SkipTo(str) == WEOF){
					break;
				}
			}
		}
	}
	m_pPinPropsDlg->update();
	m_tempAreaRectValid=false;
	Invalidate(FALSE);
	return retv;
}

bool CCompEditView::doPastePtn(SReadCE3& rce3)
{
	CPoint ptOffset = m_windowOrg;
	LPtoCompEditP(ptOffset);
	if(ptOffset.x<0) ptOffset.x = 0;
	if(ptOffset.y<0) ptOffset.y = 0;


	bool retv = false;
	wstring str;
	m_doc.incrementOpCount();
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			break;
		}else if(str[0] == '+'){
			SPtnObj* pPtnObj=NULL;
			if(str==_T("+L")){
				pPtnObj = new SPtnObjLine;
			}else if(str==_T("+C")){
				pPtnObj = new SPtnObjCircle;
			}else if(str==_T("+PG")){
				pPtnObj = new SPtnObjPolygon;
			}else if(str==_T("+AR")){
				pPtnObj = new SPtnObjArc;
			}else if(str==_T("+TX")){
				pPtnObj = new SPtnObjText;
			}else{
				str[0]='-';
				if(rce3.SkipTo(str) == WEOF){
					retv= false;
					break;
				}
			}
			if(pPtnObj!=NULL){
				if(pPtnObj->readCe3(rce3)){
					pPtnObj->move(ptOffset.x,ptOffset.y);
					m_doc.addPtnToTemp(pPtnObj);
				}
				delete pPtnObj;
			}
		}
	}
//	m_doc.setupTempArea();
	m_tempAreaRectValid=false;
	Invalidate(FALSE);
	return false;
}


bool CCompEditView::canProperty()
{
	if(m_doc.m_listTemp.size()==1) return true;
	else return false;
}

bool CCompEditView::nextPinNumber()
{
	if(!canNextPinNumber()) return false;

	SCompEditElmListIterator ite;
	ite = m_doc.m_listTemp.begin();
	SPin* pPin =(SPin*)ite->m_pData;

	wstring str = SCompEditDoc::nextString(m_doc.m_lastPinNumber);

	pPin->setPinNum(0,str.c_str());
	m_doc.m_lastPinNumber = str;

	m_tempAreaRectValid = false;
	m_doc.m_tempDirtyFlag = true;

	m_pPinPropsDlg->update();

	Invalidate(FALSE);

	return true;
}

bool CCompEditView::nextPinName()
{
	if(!canNextPinName()) return false;

	SCompEditElmListIterator ite;
	ite = m_doc.m_listTemp.begin();
	SPin* pPin =(SPin*)ite->m_pData;

	wstring str = SCompEditDoc::nextString(m_doc.m_lastPinName);

	pPin->setPinName(str.c_str());
	m_doc.m_lastPinName = str;

	m_tempAreaRectValid = false;
	m_doc.m_tempDirtyFlag = true;

	m_pPinPropsDlg->update();

	Invalidate(FALSE);


	return true;
}



bool CCompEditView::canNextPinNumber()
{
	if(m_doc.m_listTemp.size()==1){
		SCompEditElmListIterator ite;
		ite = m_doc.m_listTemp.begin();
		if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
			SPin* pPin =(SPin*)ite->m_pData;
			if(pPin->block()==1){
				return true;
			}
		}
	}
	return false;
}


bool CCompEditView::canNextPinName()
{
	if(m_doc.m_listTemp.size()==1){
		SCompEditElmListIterator ite;
		ite = m_doc.m_listTemp.begin();
		if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
			return true;
		}
	}
	return false;
}

void CCompEditView::pinModified()
{
	m_tempAreaRectValid = false;
	m_doc.m_tempDirtyFlag = true;
	Invalidate(FALSE);
	m_pPinPropsDlg->update();
}


bool CCompEditView::editProperty()
{
	if(!canProperty()) return false;
	bool result = false;
	SCompEditElmListIterator ite;
	ite = m_doc.m_listTemp.begin();
	if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
		SPin* pPin =(SPin*)ite->m_pData;
		result = editPinProperty(pPin);
	}else{
		SPtnObj* pPtn =(SPtnObj*)ite->m_pData;
		result = editPtnProperty(pPtn);
	}

	if(result){
		m_tempAreaRectValid = false;
		m_doc.m_tempDirtyFlag = true;
		Invalidate(FALSE);
	}
	return result;
}


bool CCompEditView::editPinProperty(SPin* pPin)
{
	if(m_displayPinProps)return false;
	CPinProperty dlg;
//	dlg.m_name = pPin->pinName();
	dlg.setName(pPin->pinName());
	dlg.m_pinNameDispFlag = pPin->GetPinNameDisplayFlag();
	dlg.setBlock(pPin->block());
	int i;
	for(i=0;i<pPin->block();i++){
		if(pPin->pinNum(i)){
			dlg.m_arrayNum[i]=pPin->pinNum(i);
		}else{
			dlg.m_arrayNum[i]="";
		}
	}
	dlg.m_hideNum = (pPin->type() & PIN_TYPE_NUMHIDE);
	dlg.m_pinType = pPin->type() & 0xf;
	if(dlg.DoModal()==IDOK){
		pPin->setPinName(dlg.m_name);
		pPin->SetPinNameDisplayFlag(dlg.m_pinNameDispFlag);
		m_doc.m_lastPinName = dlg.m_name;
		for(i=0;i<pPin->block();i++){
			pPin->setPinNum(i,dlg.m_arrayNum[i]);
		}
		m_doc.m_lastPinNumber = dlg.m_arrayNum[0];
		pPin->setType(dlg.m_pinType | (dlg.m_hideNum ? 	PIN_TYPE_NUMHIDE : 0));	
		return true;
	}
	return false;
}

bool CCompEditView::editPtnProperty(SPtnObj* pPtn)
{
	if(pPtn->id() == VECTPTN_TXT){
		CPtnTextDlg dlg;
		dlg.m_align = ((SPtnObjText*)pPtn)->m_align;
		dlg.m_strText = ((SPtnObjText*)pPtn)->m_strText.c_str();

		_tcsncpy(dlg.m_logfont.lfFaceName,((SPtnObjText*)pPtn)->m_fontName.c_str(),LF_FACESIZE);
		dlg.m_logfont.lfFaceName[LF_FACESIZE-1]='\0';
		dlg.m_logfont.lfHeight=((SPtnObjText*)pPtn)->m_fontSize;
		dlg.m_logfont.lfWeight=(((SPtnObjText*)pPtn)->m_fontBold ?  FW_BOLD : FW_REGULAR);
		dlg.m_logfont.lfItalic=((SPtnObjText*)pPtn)->m_fontItalic;
		dlg.m_logfont.lfCharSet=DEFAULT_CHARSET;

		if(dlg.DoModal()==IDOK){
			((SPtnObjText*)pPtn)->m_align = dlg.m_align;
			((SPtnObjText*)pPtn)->setText(dlg.m_strText);
			((SPtnObjText*)pPtn)->setFontSize(abs(dlg.m_logfont.lfHeight));
			((SPtnObjText*)pPtn)->setFontBold(dlg.m_logfont.lfWeight>=FW_BOLD);
			((SPtnObjText*)pPtn)->setFontItalic(dlg.m_logfont.lfItalic!=0);
			((SPtnObjText*)pPtn)->setFontName(dlg.m_logfont.lfFaceName);
			return true;
		}
	}else{
		if(pPtn->id()==VECTPTN_L){
			CLinePropertyDlg dlg;
			SPtnObjLine *pLine = (SPtnObjLine*)pPtn;
			dlg.m_dash = pLine->m_style;
			dlg.m_width = pLine->m_width;
			dlg.m_curveline = pLine->getCurve();
			if(dlg.DoModal()==IDOK){
				pLine->m_style = ( dlg.m_dash ? 1 : 0);
				pLine->m_width = dlg.m_width;
				pLine->setCurve(dlg.m_curveline);
				return true;
			}

		}else{
			CPtnProperty dlg;
			dlg.m_dash = pPtn->m_style;
			dlg.m_width = pPtn->m_width;
			dlg.m_fill = (pPtn->m_nFill!=-1);
			dlg.m_enableFill =( pPtn->id() == VECTPTN_PG || pPtn->id() == VECTPTN_C);
			if(dlg.DoModal()==IDOK){
				pPtn->m_style = ( dlg.m_dash ? 1 : 0);
				pPtn->m_nFill = ( dlg.m_fill ? 1 : -1);
				pPtn->m_width = dlg.m_width;
				return true;
			}
		}
	}
	return false;
}



void CCompEditView::setZoomValue(int n)
{
	if(n<MIN_ZOOMNUM) n=MIN_ZOOMNUM;
	if(n>MAX_ZOOMNUM) n=MAX_ZOOMNUM;
	static int aMagV[MAX_ZOOMNUM+1] = {1,2,5,10};
	m_magV = aMagV[n];
	m_magW = 1;
	m_zoomNum = n;
	setPageSize();
	
	CPoint pt;
	GetCursorPos(&pt);
	this->ScreenToClient(&pt);
	CRect rc;
	this->GetClientRect(&rc);
	if((pt.x>=0) && (pt.y>=0) && pt.x<=rc.right && pt.y<=rc.bottom){
		scrollTo(pt,m_pointCurrentPos);
	}else{
		Invalidate();
	}

	if(m_captureFlag == RBUTTON_CAPTURE){
		m_pointRButtonDownDevice = pt;
		CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
		PrepareDC(&dc);		//デバイスコンテキストにスクロール値の反映
		dc.DPtoLP(&pt);	//マウスの座標を論理座標に変換
		CPoint ptCompEdit = pt;
		LPtoCompEditP(ptCompEdit);//マウスの座標を編集座標に変換
		m_pointRButtonDown = ptCompEdit;
	}



}
	



bool CCompEditView::releaseTemp()
{
	m_bIsCursorOnActivePolygon = false;

	bool pinSelected = m_doc.isPinSelected();
	if(pinSelected){
		m_pPinPropsDlg->PreReleaseTemp();
	}

	if(m_doc.releaseTemp()){
		drawToBuff(true);
		Invalidate(FALSE);

		if(pinSelected){
			m_pPinPropsDlg->update(); //ピンの選択状態、数、位置が変わったことを通知する。
		}

		return true;
	}else{
		return 	false;
	}
}

void CCompEditView::setCompIndex(SCompIndex* pCompIndex)
{
	m_pCompIndex = pCompIndex;
	if(m_pCompIndex){
		m_doc.loadCompInfo(m_pCompIndex->m_pCompInfo);
	}else{
		m_doc.init();
	}
	setPageSize();
	m_block =0;
	delete m_pCursorPin;

	m_pCursorPin = NULL;
	m_bIsCursorOnActivePolygon = false;

	setTool(TOOL_SELECT);
//	if(m_pCursorPin != NULL){
//		m_pCursorPin->setLtrb(0);
//		m_pCursorPin->setOffset(0);
//	}
	m_pPinPropsDlg->update(); //ピンの選択状態、数、位置が変わったことを通知する。
	m_pPinPropsDlg->initBlock();

	Invalidate(FALSE);
}

void CCompEditView::saveCompInfo()
{
	if(m_pCompIndex){
		m_doc.saveCompInfo(m_pCompIndex->m_pCompInfo);
	}
}

BEGIN_MESSAGE_MAP(CCompEditView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_PINPROPS_WND, OnViewPinprops)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PINPROPS_WND, OnUpdatePinprops)
	ON_WM_CREATE()
	ON_COMMAND(ID_ADD_NODE, &CCompEditView::OnAddNode)
	ON_UPDATE_COMMAND_UI(ID_ADD_NODE, &CCompEditView::OnUpdateAddNode)
	ON_COMMAND(ID_VIEW_SWAPNAMEANDNUM, &CCompEditView::OnViewSwapnameandnum)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SWAPNAMEANDNUM, &CCompEditView::OnUpdateViewSwapnameandnum)
END_MESSAGE_MAP()



// CChildView メッセージ ハンドラ

BOOL CCompEditView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |=  WS_VSCROLL |WS_HSCROLL;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


//座標変換 論理座標と部品編集座標の相互変換
void CCompEditView::LPtoCompEditP(CPoint& point)
{
	point.x = point.x*m_magW/m_magV;
	point.y = point.y*m_magW/m_magV;
}


void CCompEditView::LPtoCompEditP(CRect& rc)
{
	rc.left   = rc.left*m_magW/m_magV;
	rc.top	  = rc.top*m_magW/m_magV;
	rc.right  = rc.right*m_magW/m_magV;
	rc.bottom = rc.bottom*m_magW/m_magV;
}

void CCompEditView::CompEditPtoLP(CPoint& point)
{
	point.x = point.x*m_magV/m_magW;
	point.y = point.y*m_magV/m_magW;
}

void CCompEditView::CompEditPtoLP(CRect& rc)
{
	rc.left   = rc.left*m_magV/m_magW;
	rc.top	  = rc.top*m_magV/m_magW;
	rc.right  = rc.right*m_magV/m_magW;
	rc.bottom = rc.bottom*m_magV/m_magW;
}


//ピンの配置。既存ピンがなければm_pCursorPinをTempDataに配置する。
//これを呼ぶ前にTempデータが確定されていること
bool CCompEditView::placePin()
{
	if(m_pCompIndex == NULL) return false;
	if(m_pCursorPin == NULL) return false;
	if(m_doc.pinConflict(m_pCursorPin)) return false;
//	if(m_doc.releaseTemp()){
//		this->drawToBuff(true);
//	}
	//ピンの根っこがクライアントエリア内でないと配置しない
	SPoint sptPin =m_doc.pinPosition(m_pCursorPin->ltrb(),m_pCursorPin->offset());
	CPoint ptPin(sptPin.x(),sptPin.y());
	CRect rcClient = CRect(m_windowOrg,m_clientSize);
	LPtoCompEditP(rcClient);
	if(!rcClient.PtInRect(ptPin)) return false;
	//操作カウントをインクリメントして、一時データに配置
	m_doc.incrementOpCount();
	m_pCursorPin->setBlock(m_pCompIndex->block());
	m_doc.addPinToTemp(m_pCursorPin);
	m_doc.setupTempArea();
	m_doc.m_tempDirtyFlag = true;
	if(m_tool == TOOL_SELECT){
		delete m_pCursorPin;
		m_pCursorPin = NULL;
	}
	m_pPinPropsDlg->update();	//ピンが追加されたら、ピン編集ダイアログを更新する。
	return true;
}

		
void CCompEditView::drawCompFrame(CDC* pDC)
{
	if(!m_pCompIndex) return;

	bool usePtn = false;
	if(m_pCompIndex->m_pCompInfo->m_pPtn){
		SPtn *pPtn = m_pCompIndex->m_pCompInfo->m_pPtn;
		usePtn = true;
		if(pPtn->m_pBuff){
			g_Pattern.PutPtn(pDC,0,0,pPtn->m_pBuff,pPtn->m_size.w(),pPtn->m_size.h(),
				RGB(128,255,255),DRAW_ON,0,FALSE,m_magV,m_magW);
		}
	}


	SSize sz = m_pCompIndex->size();
	CPoint ptCompRightBottom = CPoint(sz.w()*10,sz.h()*10);
	CompEditPtoLP(ptCompRightBottom);
	CPen newPen;
	CPen* pOldPen;

	int width;
	COLORREF col;

	if(usePtn){
		width = 0;
		col = RGB(192,192,192);
	}else{
		width = 1*m_magV/m_magW;
		col = COL_PART;
	}

	newPen.CreatePen(PS_SOLID,width,col);
	pOldPen = pDC->SelectObject(&newPen);
	pDC->MoveTo(0,0);
	pDC->LineTo(ptCompRightBottom.x,0);
	pDC->LineTo(ptCompRightBottom.x,ptCompRightBottom.y);
	pDC->LineTo(0,ptCompRightBottom.y);
	pDC->LineTo(0,0);
	pDC->SelectObject(pOldPen);

}

void CCompEditView::drawPin(CDC* pDC,SPin* pPin,int block,DWORD dwMode)
{
	if(!pPin)return;
	SPoint pinPos;
	if(pPin->block()<=block) block = pPin->block()-1;
	SSize sz = m_pCompIndex->size();
	int offset = pPin->offset()*10;
	int ltrb = pPin->ltrb();

	switch(ltrb&3){
		case 0:
			pinPos.setX(0);
			pinPos.setY(offset);
			break;
		case 1:
			pinPos.setX(offset);
			pinPos.setY(0);
			break;
		case 2:
			pinPos.setX(sz.w()*10);
			pinPos.setY(offset);
			break;
		case 3:
			pinPos.setX(offset);
			pinPos.setY(sz.h()*10);
			break;
	}

	if(m_swapPinAndName){
		dwMode |= DRAW_SWAP_PINNAME_PINNUM;
	}

	g_drawComponentPin(
		pDC,						//描画先
		pinPos,
		pPin->type(),
		ltrb,
		pPin->pinName(),			//ピン名
		pPin->GetPinNameDisplayFlag(),
		pPin->pinNum(block),		//ピン番号
		dwMode,						//描画モード
		m_magV,						//表示倍率(View)
		m_magW,						//表示倍率(Window)
		NULL);						//クリップ情報
}


void CCompEditView::drawPins(CDC* pDC,SCompEditElmList& list,DWORD dwMode)
{
	//int block = activeBlock();
	SCompEditElmListIterator ite;
	ite = list.begin();
	while(ite != list.end()){
		if(ite->m_deleteOpCount<0){
			if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
				SPin* pPin =(SPin*)ite->m_pData;
				drawPin(pDC,pPin,m_block,dwMode);
				//drawPin(pDC,pPin,block,dwMode);
			}
		}
		ite++;
	}
}

void CCompEditView::drawXCursor(CDC* pDC)
{
	if(!m_drawXcursor) return;
	CPoint ptXcursor = m_pointCurrentPos;
	CompEditPtoLP(ptXcursor);

	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	penNew.CreatePen(PS_SOLID,0,RGB(192,192,192));
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_XORPEN);	//描画モードの設定

	pDC->MoveTo(ptXcursor.x-10,ptXcursor.y);
	pDC->LineTo(ptXcursor.x+10,ptXcursor.y);
	pDC->MoveTo(ptXcursor.x,ptXcursor.y-10);
	pDC->LineTo(ptXcursor.x,ptXcursor.y+10);


	pDC->SetROP2(nOldDrawMode);
	pDC->SelectObject(ppenOld);
}


void CCompEditView::drawPlaceLine(CDC* pDC)
{
	CPen newPen;
	CPen* pPenOld;
	int x0,y0,x1,y1;

	int width = 0;
	newPen.CreatePen(PS_SOLID,width,RGB(255,0,0));
	pPenOld = pDC->SelectObject(&newPen);

	x0=m_arrayTempPoint[0].x();
	y0=m_arrayTempPoint[0].y();

	x0 = (x0*m_magV)/m_magW;
	y0 = (y0*m_magV)/m_magW;
	pDC->MoveTo(x0,y0);

	for(int n=1;n<=m_placeState;n++){
		x1=m_arrayTempPoint[n].x();
		y1=m_arrayTempPoint[n].y();

		x1 = (x1*m_magV)/m_magW;
		y1 = (y1*m_magV)/m_magW;
		pDC->LineTo(x1,y1);
	}
	if(m_placeState>1){
		pDC->LineTo(x0,y0);
	}
	//TRACE("drawPlaceLine %d,%d,%d,%d\n",x0,y0,x1,y1);


	pDC->SelectObject(pPenOld);
}

void CCompEditView::drawPlaceCircle(CDC* pDC)
{
	CPen newPen;
	CPen* pPenOld;
	int x0,y0,x1,y1;

	int width = 0;
	newPen.CreatePen(PS_SOLID,width,RGB(255,0,0));
	pPenOld = pDC->SelectObject(&newPen);

	CBrush* pBrushOld=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	x0=m_arrayTempPoint[0].x();
	y0=m_arrayTempPoint[0].y();

	x0 = (x0*m_magV)/m_magW;
	y0 = (y0*m_magV)/m_magW;

	x1=m_arrayTempPoint[1].x();
	y1=m_arrayTempPoint[1].y();

	x1 = (x1*m_magV)/m_magW;
	y1 = (y1*m_magV)/m_magW;

	CRect rc(x0,y0,x1,y1);
	rc.NormalizeRect();
	rc.right++;
	rc.bottom++;

	pDC->Ellipse(&rc);
	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);

}


void CCompEditView::drawPlaceArc(CDC* pDC)
{
	CPen newPen;
	CPen* pPenOld;

	int xc,yc,r,x0,y0,x1,y1,x2,y2,x3,y3;

	int width = 0;


	xc=m_arrayTempPoint[0].x();
	yc=m_arrayTempPoint[0].y();
	r =abs(m_arrayTempPoint[0].x()-m_arrayTempPoint[1].x());

	x0 = xc-r;
	y0 = yc-r;
	x1 = xc+r;
	y1 = yc+r;

	x0 = (x0*m_magV)/m_magW;
	y0 = (y0*m_magV)/m_magW;
	x1 = (x1*m_magV)/m_magW;
	y1 = (y1*m_magV)/m_magW;

	CRect rc(x0,y0,x1,y1);
	rc.NormalizeRect();
	rc.right++;
	rc.bottom++;

	newPen.CreatePen(PS_SOLID,width,RGB(220,220,220));
	pPenOld = pDC->SelectObject(&newPen);
	CBrush* pBrushOld=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Ellipse(&rc);
	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);
	newPen.DeleteObject();

	if(m_placeState>=2){
		x2 = m_arrayTempPoint[2].x();
		y2 = m_arrayTempPoint[2].y();
		if(m_placeState==2){
			x3 = xc-(x2-xc);
			y3 = yc-(y2-yc);
		}else{
			x3 = m_arrayTempPoint[3].x();
			y3 = m_arrayTempPoint[3].y();
		}
		x2 = (x2*m_magV)/m_magW;
		y2 = (y2*m_magV)/m_magW;
		x3 = (x3*m_magV)/m_magW;
		y3 = (y3*m_magV)/m_magW;

		CPoint ptBegin = CPoint(x2,y2);
		CPoint ptEnd  = CPoint(x3,y3);


		newPen.CreatePen(PS_SOLID,width,RGB(255,0,0));
		pPenOld = pDC->SelectObject(&newPen);
		pDC->Arc(&rc,ptBegin,ptEnd);
		pDC->SelectObject(pPenOld);
		newPen.DeleteObject();
	}
}

void CCompEditView::drawPtn(CDC* pDC,SPtnObj* pPtn,int cx,int cy,COLORREF col,int magV,int magW)
{
	switch(pPtn->id()){
		case VECTPTN_L:
			g_DrawPtnLine(
				pDC,				// CDC* pDC,				//描画
				(SPtnObjLine*)pPtn,	// pObj,		//描画オブジェクト
				col,				// COLORREF col,			//描画色
				0,					//int	 dir,
				cx,					//int	 cx,
				cy,					//int  cy,
				0,					//int	 x0,
				0,					//int  y0,
				magV,				//int  nVExt,				//表示倍率(View)
				magW);			//int  nWExt)				//表示倍率(Window)
				break;
		case VECTPTN_C:
			g_DrawPtnCircle(
				pDC,				//描画
				(SPtnObjCircle*)pPtn,	// pObj,	//描画オブジェクト
				col,					//COLORREF col,			//描画色
				0,						//int	 dir,
				cx,						//int	 cx,
				cy,						//int  cy,
				0,						//int	 x0,
				0,						//int  y0,
				magV,					//int  nVExt,				//表示倍率(View)
				magW);				//int  nWExt)				//表示倍率(Window)
			break;
		case VECTPTN_PG:
			g_DrawPtnPolygon(
				pDC,				//描画
				(SPtnObjPolygon*)pPtn,	// pObj,	//描画オブジェクト
				col,					//COLORREF col,			//描画色
				0,						//int	 dir,
				cx,						//int	 cx,
				cy,						//int  cy,
				0,						//int	 x0,
				0,						//int  y0,
				magV,					//int  nVExt,				//表示倍率(View)
				magW);				//int  nWExt)				//表示倍率(Window)
			break;
		case VECTPTN_AR:
			g_DrawPtnArc(
				pDC,				//描画
				(SPtnObjArc*)pPtn,		// pObj,		//描画オブジェクト
				col,					//COLORREF col,			//描画色
				0,						//int	 dir,
				cx,						//int	 cx,
				cy,						//int  cy,
				0,						//int	 x0,
				0,						//int  y0,
				magV,					//int  nVExt,				//表示倍率(View)
				magW);				//int  nWExt)				//表示倍率(Window)
				break;
		case VECTPTN_TXT:
			g_DrawPtnText(
				pDC,				//描画
				(SPtnObjText*)pPtn,		// pObj,		//描画オブジェクト
				col,					//COLORREF col,			//描画色
				0,						//int	 dir,
				cx,						//int	 cx,
				cy,						//int  cy,
				0,						//int	 x0,
				0,						//int  y0,
				magV,					//int  nVExt,				//表示倍率(View)
				magW);				//int  nWExt)				//表示倍率(Window)
				break;
	}
}


void CCompEditView::drawPtns(CDC* pDC,SCompEditElmList& list,DWORD dwMode)
{
	if(list.size()<=0)return;
	if(!m_pCompIndex)return;

	COLORREF col;

	if(dwMode & DRAW_ON)		col = COL_EDITPTN;
	else						col = COL_XOR;


	SSize sz = m_pCompIndex->size();
	int cx=sz.w()*10;
	int cy=sz.h()*10;

	SCompEditElmListIterator ite;
	ite = list.begin();
	while(ite != list.end()){
		if(ite->m_deleteOpCount<0){
			if(ite->m_dataType==SCompEditDoc::DATATYPE_PTN){
				SPtnObj* pPtn =(SPtnObj*)ite->m_pData;
				drawPtn(pDC,pPtn,cx,cy,col,m_magV,m_magW);
			}
		}
		ite++;
	}
}

void CCompEditView::drawGrip(CDC* pDC)
{
	SCompEditElmListIterator ite;
	if(m_doc.m_listTemp.size()!=1 || !m_editNodeMode) return;
	ite = m_doc.m_listTemp.begin();
	if(ite->m_dataType!=SCompEditDoc::DATATYPE_PTN) return;
	SPtnObj* pObj =(SPtnObj*)ite->m_pData;
	int nodes = pObj->gripCount();
	int i;

	CBrush* pBrushOld=(CBrush*)pDC->SelectStockObject(BLACK_BRUSH);
	CPen* pPenOld =(CPen*)pDC->SelectStockObject(BLACK_PEN);

	CPoint ptArray[4];

	for(i=0;i<nodes;i++){
		SPoint spt=pObj->gripPos(i);
		CPoint pt(spt.x(),spt.y());
		CompEditPtoLP(pt);
		if(pObj->id() == VECTPTN_L && ((SPtnObjLine*)pObj)->getCurve()){
			ptArray[i]=pt;
		}
		CRect rc(pt.x-3,pt.y-3,pt.x+3,pt.y+3);
		pDC->Rectangle(&rc);
		if(i==0){
			CBrush* pBrushOld=(CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
		}
	}
	if(pObj->id() == VECTPTN_L && ((SPtnObjLine*)pObj)->getCurve()){
		pDC->MoveTo(ptArray[0]);
		pDC->LineTo(ptArray[2]);
		pDC->MoveTo(ptArray[1]);
		pDC->LineTo(ptArray[3]);

	}

	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);
}

void CCompEditView::drawGrid(CDC* pDC)
{
	CPoint ptLeftTop = m_windowOrg;
	CPoint ptRightBottom = ptLeftTop + m_clientSize;
	LPtoCompEditP(ptLeftTop);
	LPtoCompEditP(ptRightBottom);
	int x,y;
	bool grid10only = (m_magV/m_magW < 5);
	bool yGrid10;
	for(y=ptLeftTop.y;y<ptRightBottom.y;y++){
		yGrid10 =  (y%10 == 0);
		if(!yGrid10 && grid10only) continue;
		for(x=ptLeftTop.x;x<ptRightBottom.x;x++){
			if((yGrid10 && (x%10==0))||(!grid10only &&(yGrid10 || (x%10==0)))){
				CPoint pt=CPoint(x,y);
				CompEditPtoLP(pt);
				pDC->SetPixel(pt,RGB(0,0,255));
			}else if(!grid10only){
				CPoint pt=CPoint(x,y);
				CompEditPtoLP(pt);
				pDC->SetPixel(pt,RGB(192,192,192));
			}
		}
	}

}
	

void CCompEditView::PrepareDC(CDC* pDC)
{
	pDC->SetWindowOrg(m_windowOrg);
}


bool CCompEditView::editNodeActive()
{
	SCompEditElmListIterator ite;
	if(m_doc.m_listTemp.size()==1 && m_editNodeMode){
		ite = m_doc.m_listTemp.begin();
		if(ite->m_dataType==SCompEditDoc::DATATYPE_PTN){
			return true;
		}
	}
	return false;
}

void CCompEditView::OnPaint() 
{


	CPaintDC dc(this); // 描画のデバイス コンテキスト
	
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	
	// メッセージの描画のために CWnd::OnPaint() を呼び出さないでください。

	bool bForceUpdateBuff = false;

	if(m_block != activeBlock()){
		m_block = activeBlock();
		bForceUpdateBuff = true;
	}

	drawToBuff(bForceUpdateBuff);
	PrepareDC(&dc);


	if(!m_pBmpBuff){
		drawCompFrame(&dc);
		drawGrid(&dc);
		drawPins(&dc,m_doc.m_listMain,DRAW_ON);
		drawPtns(&dc,m_doc.m_listMain,DRAW_ON);
	}else{
		CDC dcMem;						//メモリデバイスコンテキストオブジェクトの作成
		dcMem.CreateCompatibleDC(&dc);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
		CBitmap* pOldBmp=dcMem.SelectObject(m_pBmpBuff);
		dc.BitBlt(m_windowOrg.x,m_windowOrg.y,m_clientSize.cx,m_clientSize.cy,&dcMem,0,0,SRCCOPY);
		dcMem.SelectObject(pOldBmp);
	}

	drawPins(&dc,m_doc.m_listTemp,DRAW_TEMP);
	drawPtns(&dc,m_doc.m_listTemp,DRAW_TEMP);

	if(m_doc.m_listTemp.size()>0){
		//CRect rcTemp = CRect(
		//		m_doc.m_tempArea.l(),
		//		m_doc.m_tempArea.t(),
		//		m_doc.m_tempArea.r(),
		//		m_doc.m_tempArea.b()
		//	);
		//CompEditPtoLP(rcTemp);

		if(editNodeActive()){
			drawGrip(&dc);
		}else{
			if(!m_tempAreaRectValid){
				m_doc.setupTempArea();
				m_tempAreaRectValid = true;
			}
			CRect rcTemp = CRect(
					m_doc.m_tempArea.l(),
					m_doc.m_tempArea.t(),
					m_doc.m_tempArea.r(),
					m_doc.m_tempArea.b()
				);
			CompEditPtoLP(rcTemp);

			dc.DrawFocusRect(&rcTemp);	//点線枠の描画
		}
	}

	if(m_tool == TOOL_SELECT && m_startSelect){
		CRect rcTemp = CRect(m_pointLButtonDown,m_pointCurrentPos);
		rcTemp.NormalizeRect();
		CompEditPtoLP(rcTemp);
		//TRACE("%d,%d,%d,%d\n",rcTemp.left,rcTemp.top,rcTemp.right,rcTemp.bottom);
		dc.DrawFocusRect(&rcTemp);	//点線枠の描画
	}


	if(m_pCompIndex!=NULL){
//		if(m_pCursorPin != NULL && m_doc.m_listTemp.size()==0){
//			drawPin(&dc,m_pCursorPin,0,DRAW_TEMP);
//		}
		if(m_pCursorPin != NULL && m_dragOpStatus == DRAGOP_NOT){// && m_doc.m_listTemp.size()==0){
			drawPin(&dc,m_pCursorPin,0,DRAW_TEMP);
		}

		if(m_tool == TOOL_LINE || m_tool == TOOL_POLYGON ){
			if(m_placeState>0){
				drawPlaceLine(&dc);
			}
		}else if(m_tool == TOOL_CIRCLE){
			if(m_placeState>0){
				drawPlaceCircle(&dc);
			}
		}else if(m_tool == TOOL_ARC){
			if(m_placeState>0){
				drawPlaceArc(&dc);
			}
		}
	}

	if(m_bIsCursorOnActivePolygon){
		int x = (m_ptDividePolygonAt.x * m_magV)/m_magW - 5;
		int y = (m_ptDividePolygonAt.y * m_magV)/m_magW - 5;
		dc.DrawIcon(x,y,m_hDivideMarkIcon);
	}


	drawXCursor(&dc);

}


void CCompEditView::setPageMargin(int n, bool updateWindow)
{
	if(n<50) n=50;
	else if(n>200) n=200;
	m_pageMargin = n;
	if(updateWindow){
		setPageSize();
	}
}


void CCompEditView::setPageSize()
{
	int cx,cy;
	if(!m_pCompIndex){
		cx = 10;
		cy = 10;
	}else{
		cx = m_pCompIndex->size().w()*10;
		cy = m_pCompIndex->size().h()*10;
	}
	cx = (cx+m_pageMargin*2)*m_magV/m_magW;
	cy = (cy+m_pageMargin*2)*m_magV/m_magW;
	m_pageSize = CSize(cx,cy);

	SendMessage(WM_SIZE,0,MAKELONG(m_clientSize.cx,m_clientSize.cy));
	drawToBuff(true);
}


void CCompEditView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO : ここにメッセージ ハンドラ コードを追加します。

	m_clientSize = CSize(cx,cy);

	SCROLLINFO si;

	si.fMask=SIF_RANGE|SIF_POS|SIF_PAGE;

	//スクロールバーPOSITIONのデフォルト値の設定
	si.nPos = m_windowOrg.x+(m_pageMargin*m_magV)/m_magW;
	
	if(m_pageSize.cx<=cx){//クライアントエリアがページより大きい場合
	//------------|---------------------------|--------------
	//----------------*-------------------*------------------
		si.nPos = 0;
		m_windowOrg.x = -((cx - m_pageSize.cx)/2+(m_pageMargin*m_magV)/m_magW);
	}else if(si.nPos>(m_pageSize.cx-cx)){
		si.nPos = m_pageSize.cx-cx;
		m_windowOrg.x = si.nPos - (m_pageMargin*m_magV)/m_magW;
	}
	si.nMin=0;
	si.nMax=m_pageSize.cx;
	si.nPage=cx;
	SetScrollInfo(SB_HORZ,&si,TRUE);


	//スクロールバーPOSITIONのデフォルト値の設定
	si.nPos = m_windowOrg.y+(m_pageMargin*m_magV)/m_magW;
	
	if(m_pageSize.cy<=cy){//クライアントエリアがページより大きい場合
		si.nPos = 0;
		m_windowOrg.y = -((cy - m_pageSize.cy)/2+(m_pageMargin*m_magV)/m_magW);
	}else if(si.nPos>(m_pageSize.cy-cy)){
		si.nPos = m_pageSize.cy-cy;
		m_windowOrg.y = si.nPos - (m_pageMargin*m_magV)/m_magW;
	}
	si.nMin=0;
	si.nMax=m_pageSize.cy;
	si.nPage=cy;
	SetScrollInfo(SB_VERT,&si,TRUE);

	delete m_pBmpBuff;

	CDC* pDC = GetDC();
	m_pBmpBuff = new CBitmap;		//ビットマップオブジェクトを new して
	if(!m_pBmpBuff) return;
	if(!m_pBmpBuff->CreateCompatibleBitmap(pDC,cx,cy)){
		delete m_pBmpBuff;
		m_pBmpBuff = NULL;
		ReleaseDC(pDC);
		return;
	}
	ReleaseDC(pDC);

	//drawToBuff();
}

bool CCompEditView::save()
{
	if(!m_pCompIndex) return false;
	//bool updateFlag = m_doc.releaseTemp();
	bool updateFlag = releaseTemp();		//20081112
	if(m_doc.m_dirtyFlag){
		saveCompInfo();
	}
	//if(updateFlag){
	//	drawToBuff(true);
	//	Invalidate(false);
	//}
	return true;
}




void CCompEditView::drawToBuff(bool force)
{
	CDC* pDC = GetDC();

	if(!m_pBmpBuff || m_offScrnBuffSize!=m_clientSize){
		delete m_pBmpBuff;
		m_pBmpBuff = new CBitmap;		//ビットマップオブジェクトを new して
		if(!m_pBmpBuff || !m_pBmpBuff->CreateCompatibleBitmap(pDC,m_clientSize.cx,m_clientSize.cy)){
			delete m_pBmpBuff;
			m_pBmpBuff = NULL;
			ReleaseDC(pDC);
			return;
		}
		m_offScrnBuffSize=m_clientSize;
		force = true;
	}

	if(force || m_offScrnBuffOrg != m_windowOrg){

		CBitmap* pOldBmp;
		CDC dcMem;						//メモリデバイスコンテキストオブジェクトの作成
		dcMem.CreateCompatibleDC(pDC);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
		pOldBmp=dcMem.SelectObject(m_pBmpBuff);
		CRect rc=CRect(0,0,m_clientSize.cx,m_clientSize.cy);
		dcMem.FillRect(&rc,(CBrush*)dcMem.SelectStockObject(WHITE_BRUSH));	

		PrepareDC(&dcMem);

		drawCompFrame(&dcMem);
		drawGrid(&dcMem);
		drawPins(&dcMem,m_doc.m_listMain,DRAW_ON);
		drawPtns(&dcMem,m_doc.m_listMain,DRAW_ON);

		dcMem.SelectObject(pOldBmp);

		m_offScrnBuffOrg = m_windowOrg;
	}
	ReleaseDC(pDC);
}

void CCompEditView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	int nOffset = 0;
	switch(nSBCode){
	case SB_LINEUP:
		nOffset=-20;
		break;
	case SB_LINEDOWN:
		nOffset=+20;
		break;
	case SB_PAGEUP:
		nOffset=-100;
		break;
	case SB_PAGEDOWN:
		nOffset=+100;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		break;
	default:
		return;
	}
	SCROLLINFO si;

	si.fMask=SIF_RANGE|SIF_POS|SIF_PAGE;

	//スクロールバーPOSITIONのデフォルト値の設定
	if(nOffset!=0){ //SB_THUMBPOSITION, SB_THUMBTRACK以外のときの処理
		m_windowOrg.x += nOffset;
		si.nPos = m_windowOrg.x+(m_pageMargin*m_magV)/m_magW;
		if(si.nPos>(m_pageSize.cx-m_clientSize.cx)){
			si.nPos = m_pageSize.cx-m_clientSize.cx;
		}else if(si.nPos<0){
			si.nPos = 0;
		}
	}else{
		si.nPos = nPos;
	}
	if(m_pageSize.cx<=m_clientSize.cx){//クライアントエリアがページより大きい場合
		si.nPos = 0;
		m_windowOrg.x = -((m_clientSize.cx - m_pageSize.cx)/2+(m_pageMargin*m_magV)/m_magW);
	}else{
		m_windowOrg.x = si.nPos - (m_pageMargin*m_magV)/m_magW;
	}
	si.nMin=0;
	si.nMax=m_pageSize.cx;
	si.nPage=m_clientSize.cx;
	SetScrollInfo(SB_HORZ,&si,TRUE);
	//TRACE("OnHScroll m_windowOrg = (%d,%d)\n",m_windowOrg.x,m_windowOrg.y);
	//drawToBuff();
	Invalidate(FALSE);
}

void CCompEditView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	//CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	int nOffset = 0;
	switch(nSBCode){
	case SB_LINEUP:
		nOffset=-20;
		break;
	case SB_LINEDOWN:
		nOffset=+20;
		break;
	case SB_PAGEUP:
		nOffset=-100;
		break;
	case SB_PAGEDOWN:
		nOffset=+100;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		break;
	default:
		return;
	}
	SCROLLINFO si;

	si.fMask=SIF_RANGE|SIF_POS|SIF_PAGE;

	//スクロールバーPOSITIONのデフォルト値の設定
	if(nOffset!=0){ //SB_THUMBPOSITION, SB_THUMBTRACK以外のときの処理
		m_windowOrg.y += nOffset;
		si.nPos = m_windowOrg.y+(m_pageMargin*m_magV)/m_magW;
		if(si.nPos>(m_pageSize.cy-m_clientSize.cy)){
			si.nPos = m_pageSize.cy-m_clientSize.cy;
		}else if(si.nPos<0){
			si.nPos = 0;
		}
	}else{
		si.nPos = nPos;
	}
	if(m_pageSize.cy<=m_clientSize.cy){//クライアントエリアがページより大きい場合
		si.nPos = 0;
		m_windowOrg.y = -((m_clientSize.cy - m_pageSize.cy)/2+(m_pageMargin*m_magV)/m_magW);
	}else{
		m_windowOrg.y = si.nPos - (m_pageMargin*m_magV)/m_magW;
	}
	si.nMin=0;
	si.nMax=m_pageSize.cy;
	si.nPage=m_clientSize.cy;
	SetScrollInfo(SB_VERT,&si,TRUE);
	//TRACE("OnHScroll m_windowOrg = (%d,%d)\n",m_windowOrg.x,m_windowOrg.y);
	//drawToBuff();
	Invalidate(FALSE);
}

//指定したデバイス座標が、指定した編集座標になるようにスクロールする。
void CCompEditView::scrollTo(CPoint ptDevice,CPoint ptEdit)
{
	int xEdit = (ptEdit.x*m_magV/m_magW);
	int yEdit = (ptEdit.y*m_magV/m_magW);
	m_windowOrg.x = xEdit - ptDevice.x;
	m_windowOrg.y = yEdit - ptDevice.y;

	SCROLLINFO si;

	si.nPos = m_windowOrg.x+(m_pageMargin*m_magV)/m_magW;
	if(si.nPos>(m_pageSize.cx-m_clientSize.cx)){
		si.nPos = m_pageSize.cx-m_clientSize.cx;
	}else if(si.nPos<0){
		si.nPos = 0;
	}

	if(m_pageSize.cx<=m_clientSize.cx){//クライアントエリアがページより大きい場合
		si.nPos = 0;
		m_windowOrg.x = -((m_clientSize.cx - m_pageSize.cx)/2+(m_pageMargin*m_magV)/m_magW);
	}else{
		m_windowOrg.x = si.nPos - (m_pageMargin*m_magV)/m_magW;
	}
	//else{
	//	m_windowOrg.x = si.nPos - (m_pageMargin*m_magV)/m_magW;
	//}
	si.nMin=0;
	si.nMax=m_pageSize.cx;
	si.nPage=m_clientSize.cx;
	SetScrollInfo(SB_HORZ,&si,TRUE);




	si.nPos = m_windowOrg.y+(m_pageMargin*m_magV)/m_magW;
	if(si.nPos>(m_pageSize.cy-m_clientSize.cy)){
		si.nPos = m_pageSize.cy-m_clientSize.cy;
	}else if(si.nPos<0){
		si.nPos = 0;
	}
	if(m_pageSize.cy<=m_clientSize.cy){//クライアントエリアがページより大きい場合
		si.nPos = 0;
		m_windowOrg.y = -((m_clientSize.cy - m_pageSize.cy)/2+(m_pageMargin*m_magV)/m_magW);
	}else{
		m_windowOrg.y = si.nPos - (m_pageMargin*m_magV)/m_magW;
	}
	//else{
	//	m_windowOrg.y = si.nPos - (m_pageMargin*m_magV)/m_magW;
	//}
	si.nMin=0;
	si.nMax=m_pageSize.cy;
	si.nPage=m_clientSize.cy;
	SetScrollInfo(SB_VERT,&si,TRUE);


	Invalidate(FALSE);


}





void CCompEditView::OnViewZoomin()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	//TRACE("ZoomIN\n");
	setZoomValue(m_zoomNum+1);
}

void CCompEditView::OnViewZoomout()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	//TRACE("ZoomOUT\n");
	setZoomValue(m_zoomNum-1);
}

void CCompEditView::OnUpdateViewZoomin(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_zoomNum<MAX_ZOOMNUM);
}

void CCompEditView::OnUpdateViewZoomout(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_zoomNum>MIN_ZOOMNUM);
}



void CCompEditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CWnd::OnLButtonDown(nFlags, point);
	SetFocus();
	m_captureFlag |= LBUTTON_CAPTURE;
	SetCapture();		//マウスのキャプチャー
	bool bUpdateView = false;
	m_drawXcursor = false;
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	PrepareDC(&dc);		//デバイスコンテキストにスクロール値の反映
	dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
	CPoint ptCompEdit = point;
	LPtoCompEditP(ptCompEdit);//マウスの座標を編集座標に変換
	m_pointLButtonDown = ptCompEdit;
	SPoint sptCompEdit=SPoint(ptCompEdit.x,ptCompEdit.y);

	m_movingGrip = -1;

	TRACE("ButtonDown at (%d,%d)\n",ptCompEdit.x,ptCompEdit.y);

	if(m_pCompIndex==NULL)return;

	//選択されているオブジェクトが存在しているときには、
	//ボタン押下ポイントが選択オブジェクトから外れている場合に
	//選択状態から解放する。
	if(      (m_doc.m_listTemp.size()!=0)
	   &&  ! (editNodeActive() && isOnGrip(point)>=0)
	   &&  ! (!editNodeActive() && m_doc.isOnTmpData(sptCompEdit)) ){
		//if(m_doc.releaseTemp()){
		if(releaseTemp()){
			bUpdateView = true;
			//drawToBuff(true);
			//Invalidate(FALSE);
			//パターンツールでは解放直後に継続して配置をスタートしない。
			if(	m_tool == TOOL_LINE || 
				m_tool == TOOL_POLYGON || 
				m_tool == TOOL_CIRCLE || 
				m_tool == TOOL_ARC ||
				m_tool == TOOL_TEXT){
				return;
			}
		}
	}

	if(m_doc.m_listTemp.size() == 0){
		if(m_pCursorPin != NULL){
		//ピンカーソルが存在するときは、確定を行う
			int pinLtrb;
			int pinOffset;
			m_doc.pinPlacePoint(sptCompEdit,pinLtrb,pinOffset);
			m_pCursorPin->setLtrb(pinLtrb);
			m_pCursorPin->setOffset(pinOffset);
			if(m_tool == TOOL_PIN){
				m_pCursorPin->setPinName(_T("NAME"));
				m_pCursorPin->setPinNum(0,_T("NUM"));
			}
			if(placePin()){
				Invalidate(FALSE);
			}
			if(m_tool == TOOL_PIN){
				m_pCursorPin->setPinName(_T(""));
				m_pCursorPin->setPinNum(0,_T(""));
				m_pCursorPin->setBlock(1);
			}
			return;
		}else{
			if(m_tool == TOOL_SELECT){
				m_startSelect = true;
				return;
			}else{
				if(m_pCompIndex->pattern()!=NULL){ //部品がパターンを持つとき
					SPtnObj* pPtnObj = NULL;
					if(m_tool == TOOL_TEXT){
						CPtnTextDlg dlg;
						if(dlg.DoModal()==IDOK){
							if(!dlg.m_strText.IsEmpty()){
								pPtnObj = new SPtnObjText;
								((SPtnObjText*)pPtnObj)->m_p0 = sptCompEdit;
								((SPtnObjText*)pPtnObj)->m_align = dlg.m_align;
								((SPtnObjText*)pPtnObj)->setText(dlg.m_strText);
								((SPtnObjText*)pPtnObj)->setFontSize(abs(dlg.m_logfont.lfHeight));
								((SPtnObjText*)pPtnObj)->setFontBold(dlg.m_logfont.lfWeight>=FW_BOLD);
								((SPtnObjText*)pPtnObj)->setFontItalic(dlg.m_logfont.lfItalic!=0);
								((SPtnObjText*)pPtnObj)->setFontName(dlg.m_logfont.lfFaceName);
								Invalidate(FALSE);
							}
							m_placeState = 0;
						}
					}else{
						if(m_placeState==0){
							if(m_tool == TOOL_LINE || 
								m_tool == TOOL_POLYGON || 
								m_tool == TOOL_CIRCLE || 
								m_tool == TOOL_ARC){
								m_arrayTempPoint[0] = sptCompEdit;
								m_arrayTempPoint[1] = sptCompEdit;
								m_placeState = 1;
//								Invalidate(FALSE);
							}
						}else{
							//SPtnObj* pPtnObj = NULL;
							if(m_tool == TOOL_LINE){
								m_arrayTempPoint[1] = sptCompEdit;
								pPtnObj = new SPtnObjLine;
								((SPtnObjLine*)pPtnObj)->SetP0(m_arrayTempPoint[0]);
								((SPtnObjLine*)pPtnObj)->SetP1(m_arrayTempPoint[1]);
								m_placeState = 0;
//								Invalidate(FALSE);
							}else if(m_tool == TOOL_POLYGON){
								if(m_placeState < VECTPLOYGON_MAXNODE-2){
									m_arrayTempPoint[m_placeState++] = sptCompEdit;
									m_arrayTempPoint[m_placeState] = sptCompEdit;
									Invalidate(FALSE);
								}
							}else if(m_tool == TOOL_CIRCLE){
								m_arrayTempPoint[1] = sptCompEdit;
								pPtnObj = new SPtnObjCircle;
								((SPtnObjCircle*)pPtnObj)->m_p0=m_arrayTempPoint[0];
								((SPtnObjCircle*)pPtnObj)->m_p1=m_arrayTempPoint[1];
								m_placeState = 0;
//								Invalidate(FALSE);
							}else if(m_tool == TOOL_ARC){
								if(m_placeState < 3){
									m_arrayTempPoint[m_placeState++] = sptCompEdit;
									m_arrayTempPoint[m_placeState] = sptCompEdit;
									Invalidate(FALSE);
								}else{
									m_arrayTempPoint[3] = sptCompEdit;
									pPtnObj = new SPtnObjArc;
									((SPtnObjArc*)pPtnObj)->m_ptC=m_arrayTempPoint[0];
									((SPtnObjArc*)pPtnObj)->m_nR=abs(m_arrayTempPoint[0].x()-m_arrayTempPoint[1].x());
									((SPtnObjArc*)pPtnObj)->setBeginPoint(m_arrayTempPoint[2].x(),m_arrayTempPoint[2].y());
									((SPtnObjArc*)pPtnObj)->setEndPoint(m_arrayTempPoint[3].x(),m_arrayTempPoint[3].y());
									m_placeState = 0;
//									Invalidate(FALSE);
								}
							}
							//if(pPtnObj){
							//	m_doc.incrementOpCount();
							//	m_doc.addPtnToTemp(pPtnObj);
							//	m_doc.setupTempArea();
							//	delete pPtnObj;
							//	pPtnObj=NULL;
							//}
						}
					}
					if(pPtnObj){
						m_doc.incrementOpCount();
						m_doc.addPtnToTemp(pPtnObj);
						delete pPtnObj;
						pPtnObj=NULL;
						m_tempAreaRectValid = false;
						//m_doc.setupTempArea();
						Invalidate(FALSE);
					}
				}
			}
		}
	}else if(editNodeActive()){
		m_movingGrip = isOnGrip(point);
	}else if(!editNodeActive() && m_doc.isOnTmpData(sptCompEdit)){
	//非ノード編集モードにおいて、ボタン押下ポイントが、一時データの位置であるとき。
		m_prevOffset = CSize(0,0);		//ドラッグ中にすでに移動を確定した量
		if(m_doc.m_listTemp.size()==1){	//単独選択
			SCompEditElmListIterator ite;
			ite = m_doc.m_listTemp.begin();
			if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
				SPin* pPin =(SPin*)ite->m_pData;
				//ピンのドラッグをはじめる前の位置
				m_pinPosLtrbStart	= pPin->ltrb();
				m_pinPosOffsetStart	= pPin->offset();
				m_dragOpStatus = DRAGOP_PIN;
			}else{
				m_dragOpStatus = DRAGOP_PTN;
			}
		}else{//複数同時選択
			m_dragOpStatus = DRAGOP_PTN;
		}
	}
}

int CCompEditView::doSelect(SPoint sptEdit,CPoint lpt)
{
	if(m_doc.m_listMain.size()<=0)return 0;
	if(!m_pCompIndex)return 0;
	int selCount = m_doc.doSelect(sptEdit);
	if(selCount>0) return selCount;


	//以下はパターンの選択
	//パターンはメモリデバイスコンテキストに実際に描画してみて、
	//指定範囲を描画が横切るかどうかをチェックしている。
	CDC* pDC = GetDC();
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC,32,32);
	CBitmap* pOldBmp;
	CDC dcMem;						//メモリデバイスコンテキストオブジェクトの作成
	dcMem.CreateCompatibleDC(pDC);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
	pOldBmp=dcMem.SelectObject(&bmp);
	dcMem.FillSolidRect(0,0,32,32,RGB(0,0,0));

	dcMem.SetWindowOrg(lpt.x-16,lpt.y-16);
//	dcMem.SetPixel(lpt,RGB(0,0,0));

	COLORREF col = COL_EDITPTN;

	SSize sz = m_pCompIndex->size();
	int cx=sz.w()*10;
	int cy=sz.h()*10;

	SCompEditElm elm;
	SCompEditElmListRevIterator rite;
	rite = m_doc.m_listMain.rbegin();
	while(rite != m_doc.m_listMain.rend()){
		if(rite->m_deleteOpCount<0 && rite->m_dataType==SCompEditDoc::DATATYPE_PTN){
			SPtnObj* pPtn =(SPtnObj*)rite->m_pData;
			drawPtn(&dcMem,pPtn,cx,cy,col,m_magV,m_magW);
			COLORREF colpt=RGB(0,0,0);
			int nx,ny;
			int tx,ty;
			ty = lpt.y-1;
			for(ny=0;ny<3;ny++){
				tx = lpt.x-1;
				for(nx=0;nx<3;nx++){
					colpt |= dcMem.GetPixel(tx,ty);
					tx++;
				}
				ty++;
			}
			if(colpt!=RGB(0,0,0)){
				int opCount = m_doc.opCount();
				rite->m_deleteOpCount = opCount;	//m_nOpCount;
				SPtnObj* pNewPtn = pPtn->duplicate();
				elm.m_dataType = SCompEditDoc::DATATYPE_PTN;
				elm.m_placeOpCount = opCount;	//m_nOpCount;
				elm.m_deleteOpCount = -1;
				elm.m_pOrigin = &(*rite);
				elm.m_pData = pNewPtn;
				m_doc.m_tempDirtyFlag = false;
				m_doc.m_listTemp.push_back(elm);
				selCount = 1;
				break;
			}
		}
		rite++;
	}
	dcMem.SelectObject(pOldBmp);
	ReleaseDC(pDC);
	return selCount;
}

//ピン編集グリッドダイアログから呼ばれる
bool CCompEditView::selectPin(int ltrb,int offset)
{
	bool bUpdate = false;
	bool retv = false;
	if(m_tool != TOOL_SELECT){
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_SELECT);
		bUpdate = true;
	}

	if(m_doc.m_listTemp.size()!=0){
		releaseTemp();
		bUpdate = true;
	}

	m_startSelect = false;
	m_movingGrip = -1;
	m_dragOpStatus = DRAGOP_NOT;

	if(m_doc.m_listMain.size()>0 && m_pCompIndex!=NULL){
		m_doc.incrementOpCount();
		if(m_doc.doSelect(ltrb,offset)==1){
			m_doc.setupTempArea();
			drawToBuff(true);
			m_pPinPropsDlg->update();
			TRACE("selectPin(int ltrb,int offset) opCount %d\n",m_doc.opCount());

			retv = true;
			bUpdate = true;
		}
	}
	if(bUpdate){
		Invalidate();
	}
	return retv;
}

void CCompEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	TRACE("CCompEditView::OnLButtonUp  %d\n",m_captureFlag);
	CWnd::OnLButtonUp(nFlags, point);
	m_captureFlag &= ~LBUTTON_CAPTURE;
	if(m_captureFlag==0){
		TRACE("CCompEditView::OnLButtonUp  Release Caputure\n");
		::ReleaseCapture();	//マウスの解放
	}
	bool bUpdateView = false;
	m_drawXcursor = false;
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	PrepareDC(&dc);		//デバイスコンテキストにスクロール値の反映
	dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
	CPoint ptCompEdit = point;
	LPtoCompEditP(ptCompEdit);//マウスの座標を編集座標に変換
	m_pointLButtonUp = ptCompEdit;
	SPoint sptCompEdit=SPoint(ptCompEdit.x,ptCompEdit.y);

	int movX = m_pointLButtonUp.x - m_pointLButtonDown.x;
	int movY = m_pointLButtonUp.y - m_pointLButtonDown.y;


	//if(m_dragOpStatus == DRAGOP_PIN){
	if(m_doc.m_listTemp.size()==1){	//単独選択
		SCompEditElmListIterator ite;
		ite = m_doc.m_listTemp.begin();
		if(m_dragOpStatus == DRAGOP_PIN){
			SPin* pPin =(SPin*)ite->m_pData;
			if(m_doc.pinConflict(pPin)){
				pPin->setLtrb(m_pinPosLtrbStart);
				pPin->setOffset(m_pinPosOffsetStart);
				m_doc.setupTempArea();
				bUpdateView = true;
			}else{
				m_pPinPropsDlg->update();	//ピンが移動されたら、ピン編集ダイアログを更新する。
			}
		}				
	}else if(m_tool == TOOL_SELECT && m_startSelect){
		if(!m_dragOpStatus){
			m_doc.incrementOpCount();
			int selCount = 0;
			if(abs(movX)<3 && abs(movY)<3){
				//selCount = m_doc.doSelect(sptCompEdit);
				selCount = doSelect(sptCompEdit,point);
			}else{
				SPoint ptStart(m_pointLButtonDown.x,m_pointLButtonDown.y);
				SRect rcSelArea(ptStart,sptCompEdit);
				rcSelArea.normalize();//Version 0.21
				selCount = m_doc.doSelect(rcSelArea);
			}
			if(selCount){
				m_doc.setupTempArea();
				drawToBuff(true);	
				if(m_doc.isPinSelected()){	//ピンが選択されていたら、ピン編集ダイアログを更新する。
					m_pPinPropsDlg->update();
				}

			}else{
				m_doc.decrementOpCount();
			}
		}
		bUpdateView = true;
	}

	m_startSelect = false;
	m_movingGrip = -1;

	m_dragOpStatus = DRAGOP_NOT;
	if(bUpdateView){
		Invalidate(FALSE);
	}
}

void CCompEditView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CWnd::OnLButtonDblClk(nFlags, point);
	bool bUpdateView = false;
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	PrepareDC(&dc);		//デバイスコンテキストにスクロール値の反映
	dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
	CPoint ptCompEdit = point;
	LPtoCompEditP(ptCompEdit);//マウスの座標を編集座標に変換
	SPoint sptCompEdit=SPoint(ptCompEdit.x,ptCompEdit.y);

	if(canProperty()){
		if(editProperty()){
			bUpdateView = true;
		}
	}

	if(m_tool == TOOL_POLYGON && m_placeState >= 2){
		int count = m_placeState;
		if(count>VECTPLOYGON_MAXNODE) count = VECTPLOYGON_MAXNODE;
		SPtnObj* pPtnObj = new SPtnObjPolygon;
		((SPtnObjPolygon*)pPtnObj)->m_nCount = count;
		((SPtnObjPolygon*)pPtnObj)->m_pPt = new SPoint[count];
		int n;
		for(n=0;n<count;n++){
			((SPtnObjPolygon*)pPtnObj)->m_pPt[n] = m_arrayTempPoint[n];
		}
		if(pPtnObj){
			m_doc.incrementOpCount();
			m_doc.addPtnToTemp(pPtnObj);
			m_doc.setupTempArea();
			delete pPtnObj;
		}
		m_placeState =0;
		bUpdateView = true;
	}

	if(bUpdateView){
		Invalidate(FALSE);
	}
}

void CCompEditView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CWnd::OnMouseMove(nFlags, point);


	bool bUpdateView = false;
//	m_pointCurrentPosDevice = point;
	m_drawXcursor = false;
	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	PrepareDC(&dc);		//デバイスコンテキストにスクロール値の反映
	CPoint ptLogical = point;
	dc.DPtoLP(&ptLogical);	//マウスの座標を論理座標に変換
	CPoint ptCompEdit = ptLogical;
	LPtoCompEditP(ptCompEdit);//マウスの座標を編集座標に変換
	SPoint sptCompEdit=SPoint(ptCompEdit.x,ptCompEdit.y);
	m_pointCurrentPos = ptCompEdit;


	if(m_enableContextMenu){
		int xd = abs(m_pointRButtonDownDevice.x - point.x);
		int yd = abs(m_pointRButtonDownDevice.y - point.y);
		if(xd>5 || yd>5){
			m_enableContextMenu = false;	//ボタン押下後xyいずれかを5ピクセル以上移動したらコンテキストメニューを禁止する。
			//m_enableRButtonDrag = true;
		}
	}else if(nFlags & MK_RBUTTON){
		if(m_captureFlag == RBUTTON_CAPTURE){
			scrollTo(point,m_pointRButtonDown);
			return;
		}
	}





	bool enableDivideLine = editNodeActive();
	int pinLtrb;
	int pinOffset;
	//マウスカーソルの形状の決定
	if(editNodeActive()){
		//int nOnGripValue = isOnGrip(point);
		int nOnGripValue = isOnGrip(ptLogical);	//2009/07/20 ノードに近付けたらカーソルにmoveマークが付くように修正
		TRACE("nOnGripValue %d\n",nOnGripValue);
		if(nOnGripValue >=0 || m_movingGrip>=0){
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE));
			enableDivideLine = false;
		}else{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
	}else if(m_doc.isOnTmpData(sptCompEdit)){	//一時データの上か移動中
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE));
	}else{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}

	if(m_movingGrip >=0){
		enableDivideLine = false;
		SCompEditElmListIterator ite;
		ite = m_doc.m_listTemp.begin();
		if(ite->m_dataType!=SCompEditDoc::DATATYPE_PTN){
			m_movingGrip = -1;
		}else{
			SPtnObj* pObj =(SPtnObj*)ite->m_pData;
			pObj->setGripPos(m_movingGrip,sptCompEdit.x(),sptCompEdit.y());
			bUpdateView = true;
			m_doc.m_tempDirtyFlag = true;
		}
	}
	



	if(m_dragOpStatus == DRAGOP_PIN){	//ピンの移動中
		enableDivideLine = false;
		//	if(m_doc.m_listTemp.size()==1){	//単独選択
		SCompEditElmListIterator ite;
		ite = m_doc.m_listTemp.begin();
		if(m_doc.m_listTemp.size()==1){	//単独選択
//		if(m_dragOpStatus == DRAGOP_PIN){
			SPin* pPin =(SPin*)ite->m_pData;
			m_doc.pinPlacePoint(sptCompEdit,pinLtrb,pinOffset);
			if(pinLtrb != pPin->ltrb()){
				bUpdateView = true;
				m_doc.m_tempDirtyFlag = true;
				pPin->setLtrb(pinLtrb);
			}
			if(pinOffset != pPin->offset()){
				bUpdateView = true;
				m_doc.m_tempDirtyFlag = true;
				pPin->setOffset(pinOffset);
			}
		}
	}else if(m_dragOpStatus == DRAGOP_PTN){	//パターンの移動中
		enableDivideLine = false;
		CSize totalMoveSize = m_pointCurrentPos - m_pointLButtonDown;
		CSize moveSize = totalMoveSize - m_prevOffset;
		m_prevOffset = totalMoveSize;
		m_doc.move(moveSize.cx,moveSize.cy);
		bUpdateView = true;

	}else if(m_pCursorPin != NULL){		//if(m_tool == TOOL_PIN){  //ピンの配置中
		enableDivideLine = false;

		//		if(m_doc.m_listTemp.size()==0){
			m_doc.pinPlacePoint(sptCompEdit,pinLtrb,pinOffset);
			if(pinLtrb != m_pCursorPin->ltrb()){
				bUpdateView = true;
				m_pCursorPin->setLtrb(pinLtrb);
			}
			if(pinOffset != m_pCursorPin->offset()){
				bUpdateView = true;
				m_pCursorPin->setOffset(pinOffset);
			}
//		}
	}

	//カーソルが選択ポリゴンを分割できる位置にあるかどうかのチェック
	//この状態が変化したときは、再描画を実行する。
	bool old_bIsCursorOnActivePolygon = m_bIsCursorOnActivePolygon;
	if(enableDivideLine){
		TestIsCursorOnPolygon(ptCompEdit,1);
		//TRACE("OnMouseMove() ptCompEdit (x,y):(%d,%d)\n",ptCompEdit.x,ptCompEdit.y);
		//TRACE("OnMouseMove() m_bIsCursorOnActivePolygon:%d  (x,y):(%d,%d)\n",(m_bIsCursorOnActivePolygon ? 1 : 0),m_ptDividePolygonAt.x,m_ptDividePolygonAt.y);
	}else{
		m_bIsCursorOnActivePolygon = false;
	}
	if(m_bIsCursorOnActivePolygon || old_bIsCursorOnActivePolygon != m_bIsCursorOnActivePolygon){
		bUpdateView = true;
	}




	if(m_tool == TOOL_SELECT && m_startSelect){
		bUpdateView = true;
	}

	if(m_tool == TOOL_LINE || m_tool == TOOL_POLYGON || m_tool == TOOL_CIRCLE || m_tool == TOOL_ARC){
		if(m_placeState>0){
			m_arrayTempPoint[m_placeState] = sptCompEdit;
			bUpdateView = true;
		}else{
			if(m_doc.m_listTemp.size()==0){
				m_drawXcursor = true;
				bUpdateView = true;
			}
		}
	}


	if(bUpdateView){
		TRACE("Upadate View\n");
		m_doc.setupTempArea();
		Invalidate(FALSE);
	}
}

bool CCompEditView::onKeyEscape()
{
	bool bUpdateView = false;

	//if(m_doc.releaseTemp()){
	if(releaseTemp()){
		bUpdateView = true;
		//drawToBuff(true);
	}
	if(m_tool == TOOL_SELECT){
		if(m_pCursorPin != NULL){
			delete m_pCursorPin;
			m_pCursorPin = NULL;
			bUpdateView = true;
		}
	}
	if(m_placeState>0){
		m_arrayTempPoint[m_placeState-1]=m_arrayTempPoint[m_placeState];
		m_placeState --;
		bUpdateView = true;
	}

	m_movingGrip = -1;

	if(bUpdateView){
		Invalidate(FALSE);
	}
	return bUpdateView;
}


void CCompEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	switch(nChar){
	case VK_ESCAPE:
		if(!onKeyEscape()){
			AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_SELECT);
		}
		break;
	case 'S':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_SELECT);
		break;
	case 'P':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_PIN);
		break;
	case 'L':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_LINE);
		break;
	case 'C':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_CIRCLE);
		break;
	case 'G':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_POLYGON);
		break;
	case 'A':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_ARC);
		break;
	case 'T':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_TOOL_TEXT);
		break;
	case 'N':
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_EDIT_NODE_MODE);
		//OnEditNodeMode();
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CCompEditView::isOnGrip(const CPoint& logicalPoint)
{
	int x = logicalPoint.x;
	int y = logicalPoint.y;

	if(m_doc.m_listTemp.size()!=1) return -1;
	SCompEditElmListIterator ite;
	ite = m_doc.m_listTemp.begin();
	if(ite->m_dataType!=SCompEditDoc::DATATYPE_PTN) return -1;
	SPtnObj* pObj =(SPtnObj*)ite->m_pData;
	int nodes = pObj->gripCount();
	int i;
	for(i=0;i<nodes;i++){
		SPoint sptGrip=pObj->gripPos(i);
		CPoint ptGrip(sptGrip.x(),sptGrip.y());
		CompEditPtoLP(ptGrip);
		if(abs(x-ptGrip.x)<=3 && abs(y-ptGrip.y)<=3) return i;
	}
	return -1;
}

//カーソルが選択されたポリゴンの線上かどうかを検査
bool CCompEditView::TestIsCursorOnPolygon(const CPoint& editPoint, int limit)//, SPtnObjPolygon** ppPolygon, int& lineNum, CPoint& ptOnPolygon)
{
	//検査結果を格納するメンバー変数のクリア
	m_bIsCursorOnActivePolygon = false;				//分割可能な位置にカーソルがあるか
	m_pPolygonToDivide = NULL;						//分割可能であることを検出したポリゴンのポインタ
	m_nLineNumOfPolygonToDivide = -1;				//分割可能なポリゴンの何番目のラインかを記録する
	//CPoint m_ptDividePolygonAt;					//分割する場所を記録する

	int x = editPoint.x;
	int y = editPoint.y;

	SPoint sptDividePolygonAt;

	SPtnObjPolygon* pPolygon = NULL;
	SPoint sptEdit = CPoint2SPoint(editPoint);

	if(m_doc.m_listTemp.size()<1) return false;
	SCompEditElmListIterator ite;
	ite = m_doc.m_listTemp.begin();
	while(ite != m_doc.m_listTemp.end()){
		if(ite->m_dataType!=SCompEditDoc::DATATYPE_PTN){
			ite++;
			continue;
		}
		SPtnObj* pObj =(SPtnObj*)ite->m_pData;
		if(pObj->id()!=VECTPTN_PG){
			ite++;
			continue;
		}

		pPolygon = (SPtnObjPolygon*) pObj;
		
		int n;
		SPoint ptBegin;
		SPoint ptEnd;

		for(n=0; n< pPolygon->m_nCount; n++){
			ptBegin = pPolygon->m_pPt[n];
			int m=n+1;
			if(m==pPolygon->m_nCount) m= 0;
			ptEnd = pPolygon->m_pPt[m];

			//TRACE("TestIsCursorOnPolygon() %d,%d-%d,%d\n",ptBegin.x(),ptBegin.y(),ptEnd.x(),ptEnd.y());

			if(PointIsCloseToLineSide(sptEdit,ptBegin,ptEnd,limit,sptDividePolygonAt)){
				//カーソルが選択ポリゴンに近いところにあれば、判定値をメンバー変数に書き戻す
				m_bIsCursorOnActivePolygon = true;
				m_pPolygonToDivide = pPolygon;
				m_nLineNumOfPolygonToDivide = n;
				m_ptDividePolygonAt = SPoint2CPoint(sptDividePolygonAt); 
				return true;
			}
		}
		ite++;
	}
	return false;
}

////ポリゴンの線上かどうかのチェック
//bool CCompEditView::TestIsCursorOnPolygon(const CPoint& logicalPoint, int limit, SPtnObjPolygon** ppPolygon, int& lineNum, CPoint& ptOnPolygon)
//{
//	int x = logicalPoint.x;
//	int y = logicalPoint.y;
//	*ppPolygon = NULL;
//	lineNum = -1;
//	SPoint sptPnPolygon;
//
//	SPoint ptLogical = CPoint2SPoint(logicalPoint);
//
//	if(m_doc.m_listTemp.size()<1) return false;
//	SCompEditElmListIterator ite;
//	ite = m_doc.m_listTemp.begin();
//	while(ite != m_doc.m_listTemp.end()){
//		if(ite->m_dataType!=SCompEditDoc::DATATYPE_PTN){
//			ite++;
//			continue;
//		}
//		SPtnObj* pObj =(SPtnObj*)ite->m_pData;
//		if(pObj->id()!=VECTPTN_PG){
//			ite++;
//			continue;
//		}
//
//		*ppPolygon = (SPtnObjPolygon*) pObj;
//		
//		int n;
//		SPoint ptBegin;
//		SPoint ptEnd;
//		//SPoint ptCross;
//
//
//		for(n=0; n<(*ppPolygon)->m_nCount; n++){
//			ptBegin = (*ppPolygon)->m_pPt[n];
//			int m=n+1;
//			if(m==(*ppPolygon)->m_nCount) m= 0;
//			ptEnd = (*ppPolygon)->m_pPt[m];
//
//			if(PointIsCloseToLineSide(ptLogical,ptBegin,ptEnd,limit,sptPnPolygon)){
//				ptOnPolygon = SPoint2CPoint(sptPnPolygon);
//				lineNum = n; 
//				return true;
//			}
//
//		}
//
//		ite++;
//	}
//
//	return false;
//}




bool CCompEditView::canCreateBitPtn()
{
	if(m_pCompIndex==NULL) return false;
	if(m_pCompIndex->m_pCompInfo->m_pPtn){
		SPtn *pPtn = m_pCompIndex->m_pCompInfo->m_pPtn;
		if(pPtn->m_pBuff) return true;
	}
	return false;
}

bool CCompEditView::doCreateBitPtn()
{
	//return false;

	if(!canCreateBitPtn()) return false;
	if(m_doc.m_listMain.size()<=0)return false;

	//m_doc.releaseTemp();
	releaseTemp();

	SPtn *pPtn = m_pCompIndex->m_pCompInfo->m_pPtn;
	int width = pPtn->m_size.w();
	int height = pPtn->m_size.h();

	int nBmpXbyte;	//デバイス依存のビットマップの水平方向バイト数
	int nBmpSize;	//デバイス依存のビットマップのサイズ
	int nDibXbyte;	//デバイス独立のビットマップの水平方向バイト数
	int nDibSize;	//デバイス依存のビットマップのサイズ

	nBmpXbyte=((width+15)/16)*2;
	nBmpSize=nBmpXbyte*height;
	if(pPtn->m_nBuffsize!=nBmpSize) return false;
	nDibXbyte=((width+31)/32)*4;
	nDibSize=nDibXbyte*height;



	CDC* pDC = GetDC();
	CBitmap bmp;
	bmp.CreateBitmap(width,height,1,1,NULL);

	CBitmap* pOldBmp;
	CDC dcMem;						//メモリデバイスコンテキストオブジェクトの作成
	dcMem.CreateCompatibleDC(pDC);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
	pOldBmp=dcMem.SelectObject(&bmp);

	dcMem.SetWindowOrg(0,0);
	//白で初期化
	COLORREF colBk=dcMem.GetBkColor();
	dcMem.FillSolidRect(0,0,width,height,RGB(255,255,255));
	dcMem.SetBkColor(colBk);
	dcMem.SetWindowOrg(0,0);

	COLORREF col = RGB(0,0,0);

	SSize sz = m_pCompIndex->size();
	int cx=sz.w()*10;
	int cy=sz.h()*10;

	SCompEditElm elm;
	SCompEditElmListIterator ite;
	ite = m_doc.m_listMain.begin();
	while(ite != m_doc.m_listMain.end()){
		if(ite->m_deleteOpCount<0 && ite->m_dataType==SCompEditDoc::DATATYPE_PTN){
			SPtnObj* pPtn =(SPtnObj*)ite->m_pData;
			drawPtn(&dcMem,pPtn,cx,cy,col,1,1);
		}
		ite++;
	}


	BYTE* bitbuff=new BYTE[nDibSize];

	/////////////////////////////////////
	//BITMAPINFO
	int headersize = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*2;
	BYTE* pbihBuff = new BYTE[headersize];
	BITMAPINFO* pbih =(BITMAPINFO*) pbihBuff;
	pbih->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	pbih->bmiHeader.biWidth =width;
	pbih->bmiHeader.biHeight=height;
	pbih->bmiHeader.biPlanes=1;
	pbih->bmiHeader.biBitCount=1;
	pbih->bmiHeader.biCompression=0;
	pbih->bmiHeader.biSizeImage=nDibSize;
	pbih->bmiHeader.biXPelsPerMeter=5906;	//150dpi
	pbih->bmiHeader.biYPelsPerMeter=5906;	//150dpi
	pbih->bmiHeader.biClrUsed=0;
	pbih->bmiHeader.biClrImportant=0;

	//ビットの取得
	int nGetLine = ::GetDIBits(dcMem.m_hDC,(HBITMAP)bmp,0,height,bitbuff,pbih,DIB_RGB_COLORS);

	dcMem.SelectObject(pOldBmp);
	ReleaseDC(pDC);

	unsigned char* pDst = pPtn->m_pBuff;
	int copyx,copyy;
	//BYTE* pSrc;
	for(copyy = height-1; copyy>=0; copyy--){
		BYTE* pSrc = bitbuff+copyy*nDibXbyte;
		for(copyx = 0; copyx < nBmpXbyte; copyx++){
			*pDst++ = (*pSrc++)^0xFF;
		}
	}

	delete[]pbihBuff;
	delete[]bitbuff;
	drawToBuff(true);
	Invalidate(FALSE);
	m_doc.m_dirtyFlag = true;
	return true;
}

BOOL CCompEditView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	//return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	if((MK_CONTROL & nFlags) || m_captureFlag==RBUTTON_CAPTURE){
		if(m_enableContextMenu){
			m_enableContextMenu = false;
			//m_enableRButtonDrag = true;
		}

		m_nWHEEL_DELTA_z+=zDelta;
		if(m_nWHEEL_DELTA_z >= WHEEL_DELTA){
			while(m_nWHEEL_DELTA_z >= WHEEL_DELTA){
				m_nWHEEL_DELTA_z-=WHEEL_DELTA;
				SendMessage(WM_COMMAND,ID_VIEW_ZOOMIN);
			}
			return TRUE;
		}else if(m_nWHEEL_DELTA_z <= -WHEEL_DELTA){
			while(m_nWHEEL_DELTA_z <= -WHEEL_DELTA){
				m_nWHEEL_DELTA_z+=WHEEL_DELTA;
				SendMessage(WM_COMMAND,ID_VIEW_ZOOMOUT);
			}
			return TRUE;
		}else{
			return FALSE;
		}
	}else{
		m_nWHEEL_DELTA_s+=zDelta;
		if(m_nWHEEL_DELTA_s >= WHEEL_DELTA){
			while(m_nWHEEL_DELTA_s >= WHEEL_DELTA){
				m_nWHEEL_DELTA_s-=WHEEL_DELTA;
				SendMessage(WM_VSCROLL,SB_LINEUP);
			}
			return TRUE;
		}else if(m_nWHEEL_DELTA_s <= -WHEEL_DELTA){
			while(m_nWHEEL_DELTA_s <= -WHEEL_DELTA){
				m_nWHEEL_DELTA_s+=WHEEL_DELTA;
				SendMessage(WM_VSCROLL,SB_LINEDOWN);
			}
			return TRUE;
		}else{
			return FALSE;
		}
	}
}

void CCompEditView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	if(!m_enableContextMenu)return;
	CMenu menu;

	menu.LoadMenu(IDR_COMPEDITCONTEXT);
	CMenu* contextMenu = menu.GetSubMenu(0);

	//if(!m_bIsCursorOnActivePolygon){
	//	contextMenu->RemoveMenu(ID_ADD_NODE,MF_BYCOMMAND);
	//}

	contextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
		                               point.x,point.y,AfxGetMainWnd());
}

void CCompEditView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CWnd::OnRButtonDown(nFlags, point);
	SetFocus();
	m_captureFlag |= RBUTTON_CAPTURE;
	SetCapture();		//マウスのキャプチャー
	m_enableContextMenu = true;
	m_pointRButtonDownDevice = point;

	CClientDC dc(this);	//クライアントエリアのデバイスコンテキストを得て
	PrepareDC(&dc);		//デバイスコンテキストにスクロール値の反映
	dc.DPtoLP(&point);	//マウスの座標を論理座標に変換
	CPoint ptCompEdit = point;
	LPtoCompEditP(ptCompEdit);//マウスの座標を編集座標に変換
	m_pointRButtonDown = ptCompEdit;

}

void CCompEditView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	TRACE("CCompEditView::OnRButtonUp  %d\n",m_captureFlag);
	m_captureFlag &= ~RBUTTON_CAPTURE;
	if(m_captureFlag==0){
		TRACE("CCompEditView::OnRButtonUp  Release Caputure\n");
		::ReleaseCapture();	//マウスの解放
	}
	//m_enableRButtonDrag = false;
	CWnd::OnRButtonUp(nFlags, point);
}

int CCompEditView::activeBlock()
{
	if(!m_displayPinProps || m_pPinPropsDlg==NULL) return 0;
	return m_pPinPropsDlg->activeBlock();
}


void CCompEditView::OnViewPinprops()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!m_displayPinProps){
		if(m_pPinPropsDlg->GetSafeHwnd()==0){
			m_pPinPropsDlg->Create();
		}
		m_pPinPropsDlg->ShowWindow(SW_SHOW);
		m_displayPinProps = true;
	}else{
		//m_pPinPropsDlg->DestroyWindow();
		m_pPinPropsDlg->ShowWindow(SW_HIDE);
		m_pPinPropsDlg->initBlock();
		m_displayPinProps = false;
		Invalidate();
	}

}

void CCompEditView::OnUpdatePinprops(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->SetCheck(m_displayPinProps);
}

int CCompEditView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  ここに特定な作成コードを追加してください。
	if(m_pPinPropsDlg->GetSafeHwnd()==0){
		m_pPinPropsDlg->Create();
	}

	m_hDivideMarkIcon = AfxGetApp()->LoadIcon(IDI_ICON_DIVIDEMARK);

	return 0;
}

void CCompEditView::OnAddNode()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if(!m_bIsCursorOnActivePolygon) return;	//分割可能な位置にカーソルがあるか
	if(m_pPolygonToDivide == NULL) return;	//分割可能であることを検出したポリゴンのポインタ

	TRACE("%d\n",m_pPolygonToDivide->m_nCount);

	m_pPolygonToDivide->addGrip(m_nLineNumOfPolygonToDivide,m_ptDividePolygonAt.x,m_ptDividePolygonAt.y);

	m_bIsCursorOnActivePolygon = false;
	m_doc.m_tempDirtyFlag = true;

	Invalidate();
}

void CCompEditView::OnUpdateAddNode(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_bIsCursorOnActivePolygon);
}


void CCompEditView::OnViewSwapnameandnum()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	m_swapPinAndName = !m_swapPinAndName;
	drawToBuff(true);
	Invalidate();
}


void CCompEditView::OnUpdateViewSwapnameandnum(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->SetCheck(m_swapPinAndName);
}
