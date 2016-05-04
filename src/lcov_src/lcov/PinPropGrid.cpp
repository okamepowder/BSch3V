// PinPropGrid.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "PinPropGrid.h"
#include ".\pinpropgrid.h"
#include "LCoVFunc.h"

#include "CompEditView.h"
//#include "EditDetEnter.h"

PinPropGridDrawDim::PinPropGridDrawDim()
{
	SetScrollAreaWidth(100);
	SetNonScrollAreaWidth(300);
}

void PinPropGridDrawDim::SetNonScrollAreaWidth(int n)
{
	int base =(n<288 ? n/36 : 8);
	m_posWidth		= base * 4;		//位置の表示幅
	m_typeWidth		= 80;			//ピンタイプの表示幅
	m_dispChkWidth	= base * 4;		//ピン番号表示チェックボックス
	m_nameWidth		= n - (m_posWidth + m_typeWidth + m_dispChkWidth);
}

int  PinPropGridDrawDim::SetHeight(int n)
{	
	m_rowHeight = 20;
	m_row = n/m_rowHeight;
	m_rowHeight = n/m_row;
	return m_row*m_rowHeight+1;
}
	

//スクロールするエリアのパラメータ設定
//カラム単位でちょうどになる幅を返す
//上位は戻り値で、スクロールバーなどのサイズを再調整する
int PinPropGridDrawDim::SetScrollAreaWidth(int n)
{
	if(n<64){
		m_numWidth = n;
		m_column = 1;
	}else{
		m_column = n/64;
		m_numWidth = n/m_column;
	}
	return m_column*m_numWidth+1;
}


// CPinPropGrid

//int CPinPropGrid::GRID_TEXT_HEIGHT = 12;

IMPLEMENT_DYNAMIC(CPinPropGrid, CWnd)
CPinPropGrid::CPinPropGrid()
{
	m_pBmpBuff = NULL;
	m_pView = NULL;
	m_pinInfoArray = NULL;
	m_selectedIndex = -1;
	//m_blockCount = 1;
	m_displayBlockLeft = 0;
	//m_pEditCtrl = NULL;
	m_displayTop = 0;
	m_activeColumn = -1;
	
	//::FillMemory(&m_logfont,sizeof(LOGFONT),0);

	//m_logfont.lfHeight=-GRID_TEXT_HEIGHT;
	//lstrcpy(m_logfont.lfFaceName,"Tahoma");
	//CFont* pFont;
	//font.CreateFontIndirect(&m_logfont);

}

CPinPropGrid::~CPinPropGrid()
{
	delete m_pBmpBuff;
	delete[] m_pinInfoArray;
	//delete m_pEditCtrl;
}


BEGIN_MESSAGE_MAP(CPinPropGrid, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_FIX_TEXT, OnFixText)
//	ON_WM_CLOSE()
//	ON_WM_DESTROY()
END_MESSAGE_MAP()




// CPinPropGrid メッセージ ハンドラ


LRESULT CPinPropGrid::OnFixText(WPARAM wParam, LPARAM lParam)
{
	TRACE("CPinProps::OnFixText()\n");
	if(m_activeColumn == COLUMN_PINNAME || m_activeColumn >= COLUMN_PINNUMBASE){
		int index = SelectedIndex();
		if(wParam == VK_UP){
			if(index>0){
				index--;
			}
		}else if(wParam==VK_RETURN || wParam==VK_DOWN){
			if(index<(PinCount()-1)){
				index++;
			}
		}
		int col = m_activeColumn;

		m_pView->selectPin(m_pinInfoArray[index].m_pPin->ltrb(),m_pinInfoArray[index].m_pPin->offset());
		activateColumn(col);
		return 1;
	}else{
		return 0;
	}
}


//選択オブジェクトを解放する前に呼ばれる。
void CPinPropGrid::PreReleaseTemp()
{
	restoreColumnData();
}

bool CPinPropGrid::restoreActivePinName(int index)
{
	CString rstr;
	m_edit.GetWindowText(rstr);
	if(!m_pinInfoArray[index].m_pPin->pinName() || _tcscmp(rstr,SPin::pinNameToString(m_pinInfoArray[index].m_pPin->pinName()).c_str())){
		m_pinInfoArray[index].m_pPin->setPinName(rstr);
		m_pView->m_doc.m_lastPinName = rstr;
		return true;
	}
	return false;
}


bool CPinPropGrid::restoreActivePinNum(int index,int col)
{
	int block = col-COLUMN_PINNUMBASE+m_displayBlockLeft;
	CString rstr;
	m_edit.GetWindowText(rstr);
	if(!m_pinInfoArray[index].m_pPin->pinNum(block) || _tcscmp(rstr,m_pinInfoArray[index].m_pPin->pinNum(block))){
		m_pinInfoArray[index].m_pPin->setPinNum(block,rstr);
		m_pView->m_doc.m_lastPinNumber = rstr;

		return true;
	}
	return false;
}


bool CPinPropGrid::restoreActivePintype(int index)
{
	int sel = m_cmbbox.GetCurSel();
	int oldtype = m_pinInfoArray[index].m_pPin->type();
	int typeval = oldtype & PIN_TYPE_NUMHIDE;
	switch(sel){
		case 1:
			typeval |= PIN_TYPE_NEGATIVE;
			break;
		case 2:
			typeval |= PIN_TYPE_CLOCK;
			break;
		case 3:
			typeval |= (PIN_TYPE_CLOCK|PIN_TYPE_NEGATIVE);
			break;
		case 4:
			typeval |= PIN_TYPE_SMALL;
			break;
		case 5:
			typeval |= PIN_TYPE_ZLENG;
			break;
		default:
			typeval |= PIN_TYPE_NOMAL;
			break;
	}
	if(oldtype != typeval){
		m_pinInfoArray[index].m_pPin->setType(typeval);
		return true;
	}
	return false;
}


void CPinPropGrid::restoreColumnData()
{
	bool pinModified = false;
	int index = SelectedIndex();
	switch(m_activeColumn){
		case COLUMN_PINTYPE:
			if(index>=0){
				if(restoreActivePintype(index)){
					pinModified = true;
				}
			}
			m_cmbbox.ShowWindow(SW_HIDE);
			break;
		case COLUMN_PINNAME:
			if(index>=0){
				if(restoreActivePinName(index)){
					pinModified = true;
				}
			}
			m_edit.ShowWindow(SW_HIDE);
			break;
		default:
			if(m_activeColumn>=COLUMN_PINNUMBASE){
				if(index>=0){
					if(restoreActivePinNum(index,m_activeColumn)){
						pinModified = true;
					}
				}
				m_edit.ShowWindow(SW_HIDE);
			}
			break;
	}
	m_activeColumn = -1;
	if(pinModified)m_pView->pinModified();
}

void CPinPropGrid::initBlock()
{
	m_displayBlockLeft = 0;
	Invalidate();
}

int CPinPropGrid::activeBlock()
{
	if(m_activeColumn<COLUMN_PINNUMBASE) return 0;
	return m_activeColumn-COLUMN_PINNUMBASE + m_displayBlockLeft;
}


void CPinPropGrid::SetDisplayBlockLeft(int n)
{
	restoreColumnData();

	if(n<=0){
		m_displayBlockLeft = 0;
		return;
	}
	int blockCount = m_pView->m_doc.m_block;
	if((n+m_drawDim.Column())>=blockCount){
		m_displayBlockLeft = blockCount - m_drawDim.Column();
	}else{
		m_displayBlockLeft = n;
	}
	if(m_displayBlockLeft<0){
		m_displayBlockLeft=0;
	}
}



int CPinPropGrid::SetupPinInfoArray()
{
	delete[]m_pinInfoArray;

	SCompEditElmListIterator ite;

	//ピン数のカウント
	m_pinCount = 0;
	ite = m_pView->m_doc.m_listMain.begin();
	while(ite != m_pView->m_doc.m_listMain.end()){
		if(ite->m_deleteOpCount<0){
			if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
				m_pinCount++;
				//SPin* pPin =(SPin*)ite->m_pData;
				//drawPin(pDC,pPin,m_block,dwMode);
			}
		}
		ite++;
	}

	ite = m_pView->m_doc.m_listTemp.begin();
	while(ite != m_pView->m_doc.m_listTemp.end()){
		if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
			m_pinCount++;
			break;
		}
		ite++;
	}

	TRACE("CPinPropGrid::SetupPinInfoArray() count:%d\n",m_pinCount);

	int index = 0;
	m_pinInfoArray = new GridPinInfo[m_pinCount];

	ite = m_pView->m_doc.m_listMain.begin();
	while(ite != m_pView->m_doc.m_listMain.end()){
		if(ite->m_deleteOpCount<0){
			if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
				m_pinInfoArray[index].m_select = false;
				m_pinInfoArray[index].m_pPin = (SPin*)ite->m_pData;
				index++;
			}
		}
		ite++;
	}

	ite = m_pView->m_doc.m_listTemp.begin();
	while(ite != m_pView->m_doc.m_listTemp.end()){
		if(ite->m_dataType==SCompEditDoc::DATATYPE_PIN){
			m_pinInfoArray[index].m_select = true;
			m_pinInfoArray[index].m_pPin = (SPin*)ite->m_pData;
			index++;
		}
		ite++;
	}

	m_selectedIndex = -1;
	
	::qsort(m_pinInfoArray,m_pinCount,sizeof(GridPinInfo),GridPinInfo::compGridPinInfo);
	//TRACE("--------------\n");
	for(index = 0;index<m_pinCount; index++){
		if(m_pinInfoArray[index].m_select){
			m_selectedIndex = index;
			break;
		}
		//TRACE("%d,%d\n",m_pinInfoArray[index].m_pPin->ltrb(),m_pinInfoArray[index].m_pPin->offset());
	}
	//TRACE("--------------\n");


	//Invalidate();

	return 0;
}


//ソートのための静的関数
int GridPinInfo::compGridPinInfo(const void *arg1, const void *arg2 )
{
	GridPinInfo* info1 = (GridPinInfo*)arg1;
	GridPinInfo* info2 = (GridPinInfo*)arg2;

	static const int ltrbWeightTable[4] = {0,3,2,1};

	int ltrbdif = ltrbWeightTable[info1->m_pPin->ltrb()] - ltrbWeightTable[info2->m_pPin->ltrb()];
	if(ltrbdif != 0) return ltrbdif;
	switch(info1->m_pPin->ltrb()){
		case 0:
		case 3:
			return info1->m_pPin->offset() - info2->m_pPin->offset();
		case 2:
		case 1:
			return info2->m_pPin->offset() - info1->m_pPin->offset();
		default:
			return 0;
	}
}

void CPinPropGrid::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。
	TRACE("CPinPropGrid::OnPaint()\n");

	CRect rcClient;
	GetClientRect(&rcClient);

	if(m_pBmpBuff==NULL){
		m_pBmpBuff = new CBitmap;		//ビットマップオブジェクトを new して
		if(!m_pBmpBuff) return;
		if(!m_pBmpBuff->CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height())){
			delete m_pBmpBuff;
			m_pBmpBuff = NULL;
			return;
		}
	}

	CBitmap* pOldBmp;
	CDC dcMem;						//メモリデバイスコンテキストオブジェクトの作成
	dcMem.CreateCompatibleDC(&dc);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
	pOldBmp=dcMem.SelectObject(m_pBmpBuff);

	draw(&dcMem);

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBmp);

}

void CPinPropGrid::drawPinInfo(CDC* pDC,int row,GridPinInfo* info)
{
	if(!info) return;
	CRect rc;

	rc.top    = m_drawDim.RowHeight() * row;
	rc.bottom = m_drawDim.RowHeight() * (row+1);

	//CFont font;
	CFont* pFont;
	//font.CreateFontIndirect(&m_logfont);
	pFont = pDC->SelectObject(&m_font);


	CString str;
	static const TCHAR* cszLTRB = _T("LTRB");

	rc.left= 2;
	rc.right = m_drawDim.PosWidth();
	str.Format(_T("%c%d"),cszLTRB[info->m_pPin->ltrb()],info->m_pPin->offset());
	pDC->DrawText(str,&rc,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

	rc.left		= rc.left + m_drawDim.PosWidth();
	rc.right	= rc.left + m_drawDim.NameWidth();

	wstring name = SPin::pinNameToString(info->m_pPin->pinName());
	pDC->DrawText(name.c_str(),name.length(),&rc,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

	rc.left		= rc.right;
	
	LCoVFunc::DrawPinType(pDC,rc.left+2,rc.top+2,info->m_pPin->type());

	rc.left		= rc.left + m_drawDim.TypeWidth();

	if(info->m_pPin->type() & PIN_TYPE_NUMHIDE){
		LCoVFunc::DrawResourceBMP(pDC,rc.left+4,rc.top,15,15,IDB_BMP_CHK,0,0);
	}

	int i,block;
	int blockCount = BlockCount();
	int columnCount = m_drawDim.Column();
	rc.left = m_drawDim.NumLeft()+2;
	rc.right = rc.left + m_drawDim.NumWidth();
	for(i=0,block=m_displayBlockLeft;i<columnCount && block<blockCount;i++,block++){
		pDC->DrawText(info->m_pPin->pinNum(block),&rc,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		rc.left += m_drawDim.NumWidth();
		rc.right = rc.left + m_drawDim.NumWidth();
	}



	
	pDC->SelectObject(pFont);
}

void CPinPropGrid::draw(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,RGB(0xFF,0xFF,0xFF));

	drawBG(pDC);

	pDC->SetBkMode(TRANSPARENT);

	if(m_pinCount>0){
		int i,j;
		for(i=0,j=m_displayTop;(i<m_drawDim.Row())&&(j<m_pinCount);i++,j++){
			drawPinInfo(pDC,i,m_pinInfoArray+j);
		}
	}



	drawGrid(pDC);
}

int CPinPropGrid::BlockCount()
{
	return m_pView->m_doc.m_block;
}

void CPinPropGrid::drawBG(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	
	CRect rc;


	int n;

	n = SelectedIndex();
	if(n>=DisplayTop() && n<(DisplayTop()+m_drawDim.Row())){
		rc.top = rcClient.top + m_drawDim.RowHeight()*(n-DisplayTop());
		rc.left = rcClient.left;
		rc.bottom = rc.top + m_drawDim.RowHeight();
		rc.right = rcClient.right;
		pDC->FillSolidRect(&rc,RGB(0xD0,0xD0,0xFF));
	}

	rc.top = rcClient.top;
	rc.left = rcClient.left;
	rc.bottom = rcClient.bottom;
	rc.right = m_drawDim.PosWidth();
	pDC->FillSolidRect(&rc,RGB(0xD0,0xD0,0xD0));

	n = PinCount() - DisplayTop();
	if(n<m_drawDim.Row()){
		rc.top = rcClient.top + m_drawDim.RowHeight()*n;
		rc.left = rcClient.left;
		rc.bottom = rcClient.bottom;
		rc.right = rcClient.right;
		pDC->FillSolidRect(&rc,RGB(0xD0,0xD0,0xD0));
	}
	
	
	int blockCount = BlockCount();
	n = blockCount - DisplayBlockLeft();
	if(n<m_drawDim.Column()){
		rc.top = rcClient.top;
		rc.left = rcClient.left + m_drawDim.NumLeft() + m_drawDim.NumWidth()*n;
		rc.bottom = rcClient.bottom;
		rc.right = rcClient.right;
		pDC->FillSolidRect(&rc,RGB(0xD0,0xD0,0xD0));
	}


	


	if(DisplayTop()+m_drawDim.Row()>PinCount())

	pDC->SetBkColor(RGB(0xFF,0xFF,0xFF));
}

	

void CPinPropGrid::drawGrid(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CPen penNew;
	CPen* ppenOld;
	penNew.CreatePen(PS_SOLID,1,RGB(0,0,0));
	ppenOld=pDC->SelectObject(&penNew);

	int i,n,x,y;
	
	y = rcClient.Height()-1;
	x = rcClient.Width()-1;

	pDC->MoveTo(0,0);
	pDC->LineTo(x,0);
	pDC->LineTo(x,y);
	pDC->LineTo(0,y);
	pDC->LineTo(0,0);

	x = m_drawDim.PosWidth();
	pDC->MoveTo(x,0);
	pDC->LineTo(x,y);
	x += m_drawDim.NameWidth();
	pDC->MoveTo(x,0);
	pDC->LineTo(x,y);
	x += m_drawDim.TypeWidth();
	pDC->MoveTo(x,0);
	pDC->LineTo(x,y);
	x += m_drawDim.DispChkWidth();
	pDC->MoveTo(x,0);
	pDC->LineTo(x,y);
	
	n = m_drawDim.Column()-1;
	for(i=0;i<n;i++){
		x += m_drawDim.NumWidth();
		pDC->MoveTo(x,0);
		pDC->LineTo(x,y);
	}

	y=0;
	x = rcClient.Width()-1;
	n = m_drawDim.Row()-1;
	for(i=0;i<n;i++){
		y += m_drawDim.RowHeight();
		pDC->MoveTo(0,y);
		pDC->LineTo(x,y);
	}
	

	pDC->SelectObject(ppenOld);

}

void CPinPropGrid::PointToCell(CPoint pt, int& row, int& column)
{
	row = (pt.y / m_drawDim.RowHeight());

	int x = pt.x;
	
	if(x < m_drawDim.PosWidth()){
		column = 0;
		return;
	}
	x -= m_drawDim.PosWidth();
	if(x < m_drawDim.NameWidth()){
		column = 1;
		return;
	}
    x -= m_drawDim.NameWidth();
	if(x < m_drawDim.TypeWidth()){
		column = 2;
		return;
	}
	x -= m_drawDim.TypeWidth();
	if(x < m_drawDim.DispChkWidth()){
		column = 3;
		return;
	}
	x -= m_drawDim.DispChkWidth();
	column = 4+x/m_drawDim.NumWidth();	
}


void CPinPropGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	delete m_pBmpBuff;

	

	// TODO : ここにメッセージ ハンドラ コードを追加します。
}

void CPinPropGrid::initEditCtrl()
{
	m_edit.Create(WS_CHILD|ES_AUTOHSCROLL/* | WS_VISIBLE   | WS_TABSTOP | WS_BORDER*/,CRect(0, 0, 100, 20), this, IDC_EDIT_GRIDTEXT);
	m_edit.SetFont(&m_font,FALSE);
}


void CPinPropGrid::initPintypeCombobox()
{
	//	m_cmbbox.Create(WS_CHILD|WS_VISIBLE | CBS_DROPDOWNLIST,CRect(0,0,72+::GetSystemMetrics(SM_CXVSCROLL)+::GetSystemMetrics(SM_CXEDGE)*2,80),this,IDC_CBOX_PINTYPE);
	m_cmbbox.Create(WS_CHILD| CBS_DROPDOWNLIST,CRect(0,0,72+::GetSystemMetrics(SM_CXVSCROLL)+::GetSystemMetrics(SM_CXEDGE)*2,120),this,IDC_CBOX_PINTYPE);
	m_imagelist.Create(IDB_BITMAP_PINTYPELIST,72,1,RGB(255,255,0));

	//CComboBoxEx* pcmbbox =(CComboBoxEx*)GetDlgItem(IDC_COMBOBOXEX_PINTYPE);
	m_cmbbox.SetImageList(&m_imagelist);

	COMBOBOXEXITEM    item;
	//item.mask = CBEIF_IMAGE;
	item.iItem = -1;
	//CString str = _T("アイテム1");

	//item.mask = CBEIF_IMAGE| CBEIF_SELECTEDIMAGE |CBEIF_TEXT;
    item.mask = CBEIF_IMAGE| CBEIF_SELECTEDIMAGE;
    item.pszText = 0;//const_cast<LPTSTR>(static_cast<LPCTSTR>(str));

	item.iImage = 0;
	item.iSelectedImage = 0;
	m_cmbbox.InsertItem(&item);
	item.iImage = 1;
	item.iSelectedImage = 1;
	m_cmbbox.InsertItem(&item);
	item.iImage = 2;
	item.iSelectedImage = 2;
	m_cmbbox.InsertItem(&item);
	item.iImage = 3;
	item.iSelectedImage = 3;
	m_cmbbox.InsertItem(&item);
	item.iImage = 4;
	item.iSelectedImage = 4;
	m_cmbbox.InsertItem(&item);
	item.iImage = 5;
	item.iSelectedImage = 5;
	m_cmbbox.InsertItem(&item);

	//m_cmbbox.SetItemHeight(-1,m_drawDim.RowHeight()); 

}



int CPinPropGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  ここに特定な作成コードを追加してください。


	LOGFONT logfont;
	::FillMemory(&logfont,sizeof(LOGFONT),0);
	logfont.lfHeight=-GRID_TEXT_HEIGHT;
	logfont.lfCharSet=DEFAULT_CHARSET;

	lstrcpy(logfont.lfFaceName,_T("MS UI Gothic"));
	m_font.CreateFontIndirect(&logfont);


	//::FillMemory(&m_logfont,sizeof(LOGFONT),0);
	//m_logfont.lfHeight=-GRID_TEXT_HEIGHT;
	//lstrcpy(m_logfont.lfFaceName,"Tahoma");
	//m_font.CreateFontIndirect(&m_logfont);


	initPintypeCombobox();
	initEditCtrl();


	return 0;
}

void CPinPropGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	int row;
	int column;

	PointToCell(point,row,column);

	TRACE("CPinPropGrid::OnLButtonUp()  row:%d  col:%d\n",row,column);

	int index = DisplayTop()+row;

	if(index<PinCount()){
		m_pView->selectPin(m_pinInfoArray[index].m_pPin->ltrb(),m_pinInfoArray[index].m_pPin->offset());
		TRACE("CPinPropGrid::OnLButtonUp(UINT nFlags, CPoint point) \n");
		activateColumn(column);
	}


	CWnd::OnLButtonUp(nFlags, point);
}

void CPinPropGrid::activateColumn(int col)
{
	switch(col){
		case COLUMN_PINNAME:
			startEditPinName();
			break;
		case COLUMN_PINNUMHIDE:
			toggleHideNum();
			break;
		case COLUMN_PINTYPE:
			startEditPinType();
			break;
		default:
			if(col>=COLUMN_PINNUMBASE){
				startEditPinNum(col);
			}
			;
	}
}

void CPinPropGrid::toggleHideNum()
{
	int index = SelectedIndex();
	if(index<0) return;

	int pinType = m_pinInfoArray[index].m_pPin->type();

	m_pinInfoArray[index].m_pPin->setType(pinType ^ PIN_TYPE_NUMHIDE);

	m_pView->pinModified();

}

void CPinPropGrid::startEditPinNum(int col)
{
	int block = col-COLUMN_PINNUMBASE+m_displayBlockLeft;
	int index = SelectedIndex();
	if(index<0) return;
	if(block>=m_pinInfoArray[index].m_pPin->block()) return;
	m_edit.SetLimitText(64);		//0.81.01
	m_edit.SetWindowText(m_pinInfoArray[index].m_pPin->pinNum(block));

	int row = index - m_displayTop;
	int x = m_drawDim.NumLeft()+(block-m_displayBlockLeft)*m_drawDim.NumWidth();
	int y = m_drawDim.RowHeight()*row;
	m_edit.MoveWindow(x+1,y+1,m_drawDim.NumWidth()-2,m_drawDim.RowHeight()-1);
	m_edit.SetFocus();
	m_edit.SetSel(0,-1,0);
	m_edit.ShowWindow(SW_SHOW);
	m_activeColumn = col;
}

void CPinPropGrid::startEditPinName()
{
	int index = SelectedIndex();
	if(index<0) return;
	m_edit.SetLimitText(250);	//0.81.01
	m_edit.SetWindowText(SPin::pinNameToString(m_pinInfoArray[index].m_pPin->pinName()).c_str());

	int row = index - m_displayTop;
	int x = m_drawDim.PinNameLeft();
	int y = m_drawDim.RowHeight()*row;
	m_edit.MoveWindow(x+1,y+1,m_drawDim.NameWidth()-2,m_drawDim.RowHeight()-1);
	m_edit.SetFocus();
	m_edit.SetSel(0,-1,0);
	m_edit.ShowWindow(SW_SHOW);
	m_activeColumn = COLUMN_PINNAME;

}

void CPinPropGrid::startEditPinType()
{
	int index = SelectedIndex();
	if(index<0) return;

	int pintype = m_pinInfoArray[index].m_pPin->type();

	pintype &= 
		(PIN_TYPE_NOMAL |
		PIN_TYPE_SMALL |
		PIN_TYPE_ZLENG |
		PIN_TYPE_CLOCK |
		PIN_TYPE_NEGATIVE);

	int pintypeIndex;

	switch(pintype){
		case PIN_TYPE_NEGATIVE:
			pintypeIndex = 1;
			break;
		case PIN_TYPE_CLOCK:
			pintypeIndex = 2;
			break;
		case PIN_TYPE_NEGATIVE | PIN_TYPE_CLOCK:
			pintypeIndex = 3;
			break;
		case PIN_TYPE_SMALL:
			pintypeIndex = 4;
			break;
		case PIN_TYPE_ZLENG:
			pintypeIndex = 5;
			break;
		default:
			pintypeIndex = 0;
	}	

	int row = index - m_displayTop;
	int x = m_drawDim.PintypeLeft();
	int y = m_drawDim.RowHeight()*row;
	m_cmbbox.SetCurSel(pintypeIndex);
	m_cmbbox.MoveWindow(x,y,72+::GetSystemMetrics(SM_CXVSCROLL)+::GetSystemMetrics(SM_CXEDGE)*2,80);
	m_cmbbox.ShowWindow(SW_SHOW);
	m_activeColumn = COLUMN_PINTYPE;
	


}


IMPLEMENT_DYNAMIC(CPinPropGridTitle, CWnd)

CPinPropGridTitle::CPinPropGridTitle()
{
	m_pGrid = NULL;
	m_pBmpBuff = NULL;
}

CPinPropGridTitle::~CPinPropGridTitle()
{
	delete m_pBmpBuff;
}

//void CPinPropGrid::OnClose()
//{
//	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//
//	CWnd::OnClose();
//}

//void CPinPropGrid::OnDestroy()
//{
//	CWnd::OnDestroy();
//
//	// TODO : ここにメッセージ ハンドラ コードを追加します。
//}

//BOOL CPinPropGrid::OnCommand(WPARAM wParam, LPARAM lParam)
//{
//	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//
//	return CWnd::OnCommand(wParam, lParam);
//}
BEGIN_MESSAGE_MAP(CPinPropGridTitle, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CPinPropGridTitle::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。


	CRect rcClient;
	GetClientRect(&rcClient);

	if(m_pBmpBuff==NULL){
		m_pBmpBuff = new CBitmap;		//ビットマップオブジェクトを new して
		if(!m_pBmpBuff) return;
		if(!m_pBmpBuff->CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height())){
			delete m_pBmpBuff;
			m_pBmpBuff = NULL;
			return;
		}
	}

	CBitmap* pOldBmp;
	CDC dcMem;						//メモリデバイスコンテキストオブジェクトの作成
	dcMem.CreateCompatibleDC(&dc);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
	pOldBmp=dcMem.SelectObject(m_pBmpBuff);

	draw(&dcMem);

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBmp);
}


void CPinPropGridTitle::draw(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,RGB(0xF0,0xF0,0xFF));
	int vh = rc.bottom/2;

	pDC->SetBkMode(TRANSPARENT);
	CFont* pFont = pDC->SelectObject(&(m_pGrid->m_font));
	CString str;
	CRect rcStr;

	rcStr = CRect(0,0,m_pGrid->m_drawDim.PinNameLeft(),rc.bottom);
	str.LoadString(IDS_GRID_TITLE_POS);
	pDC->DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	rcStr = CRect(m_pGrid->m_drawDim.PinNameLeft(),0,m_pGrid->m_drawDim.PintypeLeft(),rc.bottom);
	str.LoadString(IDS_GRID_TITLE_NAME);
	pDC->DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	rcStr = CRect(m_pGrid->m_drawDim.PintypeLeft(),0,m_pGrid->m_drawDim.NumHideLeft(),rc.bottom);
	str.LoadString(IDS_GRID_TITLE_TYPE);
	pDC->DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	rcStr = CRect(m_pGrid->m_drawDim.NumHideLeft(),0,rc.right,vh);
	str.LoadString(IDS_GRID_TITLE_NUM);
	pDC->DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	int i;
	int x;
	int b;

	x = m_pGrid->m_drawDim.NumLeft();
	b = m_pGrid->DisplayBlockLeft();
	CString strformat;
	strformat.LoadString(IDS_GRID_TITLE_BLOCK);
	for(i=0;i<m_pGrid->m_drawDim.Column();i++){
		rcStr = CRect(x,vh,x+m_pGrid->m_drawDim.NumWidth(),rc.bottom);
		str.Format(strformat,b+1);
		pDC->DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);
		x+=m_pGrid->m_drawDim.NumWidth();
		b++;
	}

	rcStr = CRect(m_pGrid->m_drawDim.NumHideLeft(),vh,m_pGrid->m_drawDim.NumLeft(),rc.bottom);
	str.LoadString(IDS_GRID_TITLE_HIDE);
	pDC->DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(pFont);


	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	CPen* pPenOld = pDC->SelectObject(&pen);
	//dc.Rectangle(&rc);
	pDC->MoveTo(0,0);
	pDC->LineTo(rc.right-1,0);
	pDC->LineTo(rc.right-1,rc.bottom);
	pDC->LineTo(0,rc.bottom);
	pDC->LineTo(0,0);
	pDC->MoveTo(m_pGrid->m_drawDim.PinNameLeft(),rc.top);
	pDC->LineTo(m_pGrid->m_drawDim.PinNameLeft(),rc.bottom);
	pDC->MoveTo(m_pGrid->m_drawDim.PintypeLeft(),rc.top);
	pDC->LineTo(m_pGrid->m_drawDim.PintypeLeft(),rc.bottom);
	pDC->MoveTo(m_pGrid->m_drawDim.NumHideLeft(),rc.top);
	pDC->LineTo(m_pGrid->m_drawDim.NumHideLeft(),rc.bottom);
	pDC->MoveTo(m_pGrid->m_drawDim.NumHideLeft(),vh);
	pDC->LineTo(rc.right-1,vh);
	x = m_pGrid->m_drawDim.NumLeft();
	
	for(i=0;i<m_pGrid->m_drawDim.Column();i++){
        pDC->MoveTo(x,vh);
		pDC->LineTo(x,rc.bottom);
		x+=m_pGrid->m_drawDim.NumWidth();
	}
	pDC->SelectObject(pPenOld);



	//CRect rc;
	//GetClientRect(&rc);
	//dc.FillSolidRect(&rc,RGB(0xF0,0xF0,0xFF));
	//int vh = rc.bottom/2;

	//dc.SetBkMode(TRANSPARENT);
	//CFont* pFont = dc.SelectObject(&(m_pGrid->m_font));
	//CString str;
	//CRect rcStr;

	//rcStr = CRect(0,0,m_pGrid->m_drawDim.PinNameLeft(),rc.bottom);
	//str.LoadString(IDS_GRID_TITLE_POS);
	//dc.DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	//rcStr = CRect(m_pGrid->m_drawDim.PinNameLeft(),0,m_pGrid->m_drawDim.PintypeLeft(),rc.bottom);
	//str.LoadString(IDS_GRID_TITLE_NAME);
	//dc.DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	//rcStr = CRect(m_pGrid->m_drawDim.PintypeLeft(),0,m_pGrid->m_drawDim.NumHideLeft(),rc.bottom);
	//str.LoadString(IDS_GRID_TITLE_TYPE);
	//dc.DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	//rcStr = CRect(m_pGrid->m_drawDim.NumHideLeft(),0,rc.right,vh);
	//str.LoadString(IDS_GRID_TITLE_NUM);
	//dc.DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);

	//int i;
	//int x;
	//int b;

	//x = m_pGrid->m_drawDim.NumLeft();
	//b = m_pGrid->DisplayBlockLeft();
	//CString strformat;
	//strformat.LoadString(IDS_GRID_TITLE_BLOCK);
	//for(i=0;i<m_pGrid->m_drawDim.Column();i++){
	//	rcStr = CRect(x,vh,x+m_pGrid->m_drawDim.NumWidth(),rc.bottom);
	//	str.Format(strformat,b);
	//	dc.DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);
	//	x+=m_pGrid->m_drawDim.NumWidth();
	//	b++;
	//}

	//rcStr = CRect(m_pGrid->m_drawDim.NumHideLeft(),vh,m_pGrid->m_drawDim.NumLeft(),rc.bottom);
	//str.LoadString(IDS_GRID_TITLE_HIDE);
	//dc.DrawText(str,&rcStr,DT_CENTER | DT_VCENTER|DT_SINGLELINE);



	//dc.SelectObject(pFont);
	//CPen pen;
	//pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	//CPen* pPenOld = dc.SelectObject(&pen);
	////dc.Rectangle(&rc);
	//dc.MoveTo(0,0);
	//dc.LineTo(rc.right-1,0);
	//dc.LineTo(rc.right-1,rc.bottom);
	//dc.LineTo(0,rc.bottom);
	//dc.LineTo(0,0);
	//dc.MoveTo(m_pGrid->m_drawDim.PinNameLeft(),rc.top);
	//dc.LineTo(m_pGrid->m_drawDim.PinNameLeft(),rc.bottom);
	//dc.MoveTo(m_pGrid->m_drawDim.PintypeLeft(),rc.top);
	//dc.LineTo(m_pGrid->m_drawDim.PintypeLeft(),rc.bottom);
	//dc.MoveTo(m_pGrid->m_drawDim.NumHideLeft(),rc.top);
	//dc.LineTo(m_pGrid->m_drawDim.NumHideLeft(),rc.bottom);
	//dc.MoveTo(m_pGrid->m_drawDim.NumHideLeft(),vh);
	//dc.LineTo(rc.right-1,vh);
	//x = m_pGrid->m_drawDim.NumLeft();
	//
	//for(i=0;i<m_pGrid->m_drawDim.Column();i++){
 //       dc.MoveTo(x,vh);
	//	dc.LineTo(x,rc.bottom);
	//	x+=m_pGrid->m_drawDim.NumWidth();
	//}
	//dc.SelectObject(pPenOld);

}
