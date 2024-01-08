// PinProps.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "PinProps.h"
#include ".\pinprops.h"
#include "CompEditView.h"


// CPinProps ダイアログ

IMPLEMENT_DYNAMIC(CPinProps, CDialog)
CPinProps::CPinProps(CWnd* pParent /*=NULL*/)
	: CDialog(CPinProps::IDD, pParent)
{
	//CDialog::Create(CPinProps::IDD,pParent);
	m_pParent = (CCompEditView*)pParent;
	m_pGrid = NULL;
	m_pGridTitle = NULL;
	
}

CPinProps::~CPinProps()
{
	delete m_pGrid;
	delete m_pGridTitle;
}

void CPinProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CPinProps::Create()
{
	BOOL retval = CDialog::Create(CPinProps::IDD,m_pParent);
	
	return retval;
}

//選択オブジェクトを解放する前に呼ばれる。
//グリッドで編集中の項目をデータに書き戻す
void CPinProps::PreReleaseTemp()
{
	m_pGrid->PreReleaseTemp();
}

void CPinProps::initBlock()
{
	m_pGrid->initBlock();
	m_pGridTitle->Invalidate();
}

void CPinProps::update()
{
	TRACE("CPinProps::update()\n");
	m_pGrid->SetupPinInfoArray();

	int selIndex = m_pGrid->SelectedIndex();
	if(selIndex >= 0){
		if(m_pGrid->DisplayTop() > selIndex){
			m_pGrid->SetDisplayTop(selIndex);
		}else if((m_pGrid->DisplayTop() + m_pGrid->m_drawDim.Row())<=selIndex){
			m_pGrid->SetDisplayTop(selIndex-m_pGrid->m_drawDim.Row()+1);
		}
	}
	if((m_pGrid->DisplayTop() + m_pGrid->m_drawDim.Row())>=m_pGrid->PinCount()){
		m_pGrid->SetDisplayTop(m_pGrid->PinCount()-m_pGrid->m_drawDim.Row());
	}
	if(m_pGrid->DisplayTop()<0){
		m_pGrid->SetDisplayTop(0);
	}

	setVScroll();
	setHScroll();

	m_pGrid->Invalidate();
}


void CPinProps::setVScroll()
{
	SCROLLINFO scrlInfo;
	scrlInfo.cbSize = sizeof(SCROLLINFO);
	scrlInfo.fMask = SIF_ALL;
	scrlInfo.nMin = 0;
	scrlInfo.nMax = m_pGrid->PinCount()-1;
	scrlInfo.nPage = m_pGrid->m_drawDim.Row();
	scrlInfo.nPos = m_pGrid->DisplayTop();

	CScrollBar* pScroll = (CScrollBar*)GetDlgItem(IDC_VSCROLL);
	pScroll->SetScrollInfo(&scrlInfo);
}


void CPinProps::setHScroll()
{
	SCROLLINFO scrlInfo;
	scrlInfo.cbSize = sizeof(SCROLLINFO);
	scrlInfo.fMask = SIF_ALL;
	scrlInfo.nMin = 0;
	scrlInfo.nMax = m_pGrid->BlockCount()-1;
	scrlInfo.nPage = m_pGrid->m_drawDim.Column();
	scrlInfo.nPos = m_pGrid->DisplayBlockLeft();

	CScrollBar* pScroll = (CScrollBar*)GetDlgItem(IDC_HSCROLL);
	pScroll->SetScrollInfo(&scrlInfo);
}



BEGIN_MESSAGE_MAP(CPinProps, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPinProps メッセージ ハンドラ

void CPinProps::OnBnClickedOk()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	//TRACE("CPinProps::OnBnClickedOk()\n");
	m_pGrid->OnFixText(VK_RETURN,0);
	//OnOK();
}

void CPinProps::OnBnClickedCancel()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	TRACE("CPinProps::OnBnClickedCancel()\n");
	//CCompEditView *pwnd = (CCompEditView*)m_pParent;
	m_pGrid->restoreColumnData();
	m_pParent->OnViewPinprops();

	//OnCancel();
}






BOOL CPinProps::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	CenterWindow(m_pParent);
	m_pGrid = new CPinPropGrid();
	m_pGridTitle = new CPinPropGridTitle();

	CString strClassName=AfxRegisterWndClass(
		CS_VREDRAW|CS_HREDRAW,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
	NULL);







	CRect initRc = CRect(0,0,0,0);

	m_pGrid->Create(
			strClassName,
			_T("PinPropsGrid"),	
			WS_CHILD | WS_VISIBLE ,
			initRc,
			this,
			ID_PINPROPGRID);

	m_pGrid->m_pView = m_pParent;

	m_pGridTitle->Create(
			strClassName,
			_T("PinPropsGridTitle"),	
			WS_CHILD | WS_VISIBLE ,
			initRc,
			this,
			ID_PINPROPGRIDTITLE);

	m_pGridTitle->m_pGrid = m_pGrid;

	SetCtrlDim();

	setVScroll();
	setHScroll();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//コントロールの位置の調整
void CPinProps::SetCtrlDim()
{
	CRect rcClient;
	CRect rcVScroll;
	CRect rcHScroll;
	CRect rcGrid;
	CRect rcGridTitle;


	GetClientRect(&rcClient);
	GetDlgItem(IDC_VSCROLL)->GetWindowRect(&rcVScroll);
	ScreenToClient(&rcVScroll);
	int height = rcVScroll.Height();

	//縦スクロールバーの上端にグリッドの上端を合わせる
	//グリッドの縦サイズは、縦スクロールバーの初期長さ以下で、行高さの整数倍になるようにする
	//グリッドの縦サイズを決めたら、縦スクロールバーの下端をそれに合わせる。
	//グリッドの左端は、縦スクロールバーの右端とダイアログの右端の間隔に合わせて調整する

	height = m_pGrid->m_drawDim.SetHeight(height);

	//m_pGrid->m_cmbbox.SetItemHeight(-1,m_pGrid->m_drawDim.RowHeight());

	rcVScroll.bottom = rcVScroll.top+height;
	GetDlgItem(IDC_VSCROLL)->MoveWindow(&rcVScroll);
	
	rcGrid.top = rcVScroll.top;
	rcGrid.bottom = rcVScroll.bottom;
	rcGrid.left = rcClient.left + (rcClient.right - rcVScroll.right);
	rcGrid.right = rcVScroll.left;
	m_pGrid->MoveWindow(&rcGrid);

	rcGridTitle.left = rcGrid.left;
	rcGridTitle.right = rcGrid.right;
	rcGridTitle.bottom = rcGrid.top;
	rcGridTitle.top = 3;
	



	GetDlgItem(IDC_HSCROLL)->GetWindowRect(&rcHScroll);
	ScreenToClient(&rcHScroll);
	int scrollAreaWidth = rcHScroll.Width();
	scrollAreaWidth =  m_pGrid->m_drawDim.SetScrollAreaWidth(scrollAreaWidth);
	m_pGrid->m_drawDim.SetNonScrollAreaWidth(rcGrid.Width() - scrollAreaWidth);

	rcHScroll.left = rcHScroll.right-scrollAreaWidth;
	height = rcHScroll.Height();
	rcHScroll.top = rcGrid.bottom;
	rcHScroll.bottom = rcHScroll.top+height;
	GetDlgItem(IDC_HSCROLL)->MoveWindow(&rcHScroll);

	m_pGridTitle->MoveWindow(rcGridTitle);


}
void CPinProps::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	//CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar->GetDlgCtrlID() != IDC_VSCROLL){
		return;
	}

	int currentPos = m_pGrid->DisplayTop();

	switch(nSBCode){
		case SB_BOTTOM:
			currentPos = 10000;
			break;
		case SB_LINEDOWN:
			currentPos += 1;
			break;
		case SB_LINEUP:
			currentPos -= 1;
			break;
		case SB_PAGEDOWN:
			currentPos += 4;
			break;
		case SB_PAGEUP:
			currentPos -= 4;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			currentPos = nPos;
			break;
		case SB_TOP:
			currentPos = 0;
		default:
			return;
	}
	m_pGrid->SetDisplayTop(currentPos);
	m_pGrid->Invalidate();

	setVScroll();
				
}

void CPinProps::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	//CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar->GetDlgCtrlID() != IDC_HSCROLL){
		return;
	}

	int currentPos = m_pGrid->DisplayBlockLeft();

	switch(nSBCode){
		case SB_BOTTOM:
			currentPos = 10000;
			break;
		case SB_LINEDOWN:
			currentPos += 1;
			break;
		case SB_LINEUP:
			currentPos -= 1;
			break;
		case SB_PAGEDOWN:
			currentPos += 2;
			break;
		case SB_PAGEUP:
			currentPos -= 2;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			currentPos = nPos;
			break;
		case SB_TOP:
			currentPos = 0;
		default:
			return;
	}
	m_pGrid->SetDisplayBlockLeft(currentPos);
	m_pGrid->Invalidate();
	m_pGridTitle->Invalidate();

	setHScroll();

}

//void CPinProps::OnClose()
//{
//	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//
//	CDialog::OnClose();
//}
