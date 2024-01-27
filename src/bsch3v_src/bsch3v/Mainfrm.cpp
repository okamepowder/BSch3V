/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// MainFrm.cpp : CMainFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "BSch.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define COPYDATA_CHECKOPENFILE 1


class CBSchView;




/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_COPYDATA()
	ON_COMMAND(ID_VIEW_LAYERBOX, OnViewLayerbox)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LAYERBOX, OnUpdateViewLayerbox)
	ON_WM_SIZE()
	ON_MESSAGE(SCROLL_TO_MSG,OnScrollToMsg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 0 �X�e�[�^�X ���C�� �C���W�P�[�^
//	ID_INDICATOR_KANA,		// �폜
	ID_INDICATOR_CAPS,		// 1
	ID_INDICATOR_NUM,		// 2
//	ID_INDICATOR_SCRL,
    ID_SEPARATOR,			// 3   X���W�̂��߂̒ǉ���
    ID_SEPARATOR,			// 4   Y���W�̂��߂̒ǉ���
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̍\�z/����

CMainFrame::CMainFrame()
{
	// TODO: ���̈ʒu�Ƀ����o�̏����������R�[�h��ǉ����Ă��������B
	m_extensionMenuCount=0;
}

CMainFrame::~CMainFrame()
{
}


struct EnumWindowsProcCheckOpenedParam{
	const TCHAR* pcszPathName;
	HWND hMyWnd;
    HWND hFindWnd;
};


BOOL CALLBACK CMainFrame::EnumWindowsProcCheckOpened(HWND hwnd, LPARAM lParam)
{
	EnumWindowsProcCheckOpenedParam* pParam = (EnumWindowsProcCheckOpenedParam*)lParam;

	//���������E�B���h�E���������g�Ȃ牽�����Ȃ�
	if(pParam->hMyWnd == hwnd) return TRUE;

	//���������E�B���h�E��BSch3V�łȂ���Ή������Ȃ��B
	if(!::GetProp(hwnd, _T("BSch3V-Suigyodo"))) return TRUE;


	COPYDATASTRUCT cd;
	cd.dwData = COPYDATA_CHECKOPENFILE;
	cd.lpData = (PVOID)pParam->pcszPathName;
	cd.cbData = (_tcslen(pParam->pcszPathName)+1)*sizeof(wchar_t); 

	if (::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM)&cd)){
		//�Y���E�B���h�E������������A�E�B���h�E�n���h����ۑ����ė񋓂𒆒f
		pParam->hFindWnd = hwnd;
		return FALSE;
	}
    return TRUE;
}


CWnd* CMainFrame::CheckOpened(const TCHAR* pcszPathName)
{
	EnumWindowsProcCheckOpenedParam coParam;
	coParam.pcszPathName = pcszPathName;
	coParam.hMyWnd = GetSafeHwnd();
	coParam.hFindWnd = NULL;
	EnumWindows(EnumWindowsProcCheckOpened,(LPARAM)&coParam);
    return CWnd::FromHandle(coParam.hFindWnd);
}


BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if(pCopyDataStruct->dwData == COPYDATA_CHECKOPENFILE)
	{
		CDocument* pDoc=((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		if (!pDoc)return FALSE;
		CString strPathName = pDoc->GetPathName();
		CString strCheckOpenPath = (LPCTSTR)pCopyDataStruct->lpData;
		return (strPathName==strCheckOpenPath);
	}
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}




void CMainFrame::OnDestroy()
{
	
	WINDOWPLACEMENT wp;
	this->GetWindowPlacement(&wp);
	int cx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	int cy = wp.rcNormalPosition.bottom- wp.rcNormalPosition.top;
	IniWriteWindowPosition(wp.showCmd,cx,cy);


	CFrameWnd::OnDestroy();
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	RemoveProp(GetSafeHwnd(), _T("BSch3V-Suigyodo"));
}


int CMainFrame::CreateDBarLayer(UINT idd)
{
	if (!m_dbarLayer.Create(this, idd,
		WS_CHILD | CBRS_RIGHT, idd))
	{
		TRACE0("Failed to create layer dialogbar\n");
		return -1;      // �쐬�Ɏ��s
	}

	m_dbarLayer.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	CString strTitle;
	strTitle.LoadString(IDS_LAYER_DBAR_TITLE);
	m_dbarLayer.SetWindowText(strTitle);

	HBITMAP hBmp;
	CStatic* pStatic;
	hBmp = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_EDIT), IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS);
	pStatic = (CStatic*)m_dbarLayer.GetDlgItem(IDC_STATIC_EDIT);
	pStatic->SetBitmap(hBmp);

	hBmp = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_VIEW), IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS);
	pStatic = (CStatic*)m_dbarLayer.GetDlgItem(IDC_STATIC_VIEW);
	pStatic->SetBitmap(hBmp);
	return 0;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;



	SetProp(GetSafeHwnd(), _T("BSch3V-Suigyodo"), (HANDLE)1);
	AfxGetApp()->m_pMainWnd = this;


	//DWORD dwVisible;

	UINT idd;
	if (AfxGetApp()->GetProfileInt(_T("Option"), _T("DisplayLayerNameOnDlgBar"), TRUE)) {
		idd = IDD_DBAR_LAYER_S;	//�����񗓕t���_�C�A���O�o�[
	}
	else {
		idd = IDD_DBAR_LAYER;
	}

	CreateDBarLayer(idd);
	//if(!m_dbarLayer.Create(	this,IDD_DBAR_LAYER,
	//						WS_CHILD|CBRS_RIGHT,IDD_DBAR_LAYER))
	//{
	//	TRACE0("Failed to create layer dialogbar\n");
	//	return -1;      // �쐬�Ɏ��s
	//}

	//m_dbarLayer.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	//CString strTitle;
	//strTitle.LoadString(IDS_LAYER_DBAR_TITLE);
	//m_dbarLayer.SetWindowText(strTitle);

	//HBITMAP hBmp;
	//CStatic *pStatic;
	//hBmp=(HBITMAP)::LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE (IDB_EDIT),IMAGE_BITMAP,0,0,LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);
	//pStatic = (CStatic*)m_dbarLayer.GetDlgItem(IDC_STATIC_EDIT);
	//pStatic->SetBitmap(hBmp);

	//hBmp=(HBITMAP)::LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE (IDB_VIEW),IMAGE_BITMAP,0,0,LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);
	//pStatic = (CStatic*)m_dbarLayer.GetDlgItem(IDC_STATIC_VIEW);
	//pStatic->SetBitmap(hBmp);



	BOOL bVisible;
	//���C���[�_�C�A���O�o�[�̕\���E��\���̏������W�X�g�����瓾�� 2007/03/27
	if (AfxGetApp()->GetProfileInt(_T("Option"), _T("DisplayLayerDlgBar"), FALSE)) {
		bVisible = TRUE;//WS_VISIBLE;
	} else {
		bVisible = FALSE;
	}
	ShowControlBar(&m_dbarLayer, bVisible, FALSE);



	DWORD dwVisible;
	//�c�[���o�[�̕\���E��\���̏������W�X�g�����瓾�� 1997/01/25
	if(AfxGetApp()->GetProfileInt(_T("Option"),_T("DisplayToolBar"),TRUE)){
		dwVisible=WS_VISIBLE;
	}else{
		dwVisible=0;
	}

	if (!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | dwVisible | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))


	//if (!m_wndToolBar.Create(this,WS_CHILD|dwVisible|CBRS_TOP) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // �쐬�Ɏ��s
	}

	//�X�e�[�^�X�o�[�̕\���E��\���̏������W�X�g�����瓾�� 1997/01/25
	if(AfxGetApp()->GetProfileInt(_T("Option"),_T("DisplayStatusBar"),TRUE)) dwVisible=WS_VISIBLE;
	else dwVisible=0;
	if (!m_wndStatusBar.Create(	this,
								WS_CHILD|dwVisible|CBRS_BOTTOM,
								ID_BSCH_STATUS_BAR)
		||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // �쐬�Ɏ��s
	}

	//�X�e�[�^�X�o�[�̍��W�\���g�̊m��
	int nStusBarWidth;	//�X�e�[�^�X�o�[�ɍ��W�������\�����邽�߂̕�
	TEXTMETRIC tm;	//�e�L�X�g���g���b�N
	CDC* pDC=m_wndStatusBar.GetDC();
	pDC->GetTextMetrics(&tm);
	m_wndStatusBar.ReleaseDC(pDC);
	nStusBarWidth=tm.tmAveCharWidth*(4+1);	//�����͂S�����{�}�[�W��1����
	m_wndStatusBar.SetPaneInfo(3,ID_SEPARATOR,SBPS_NORMAL,nStusBarWidth);
	m_wndStatusBar.SetPaneInfo(4,ID_SEPARATOR,SBPS_NORMAL,nStusBarWidth);


	// TODO: �����c�[�� �`�b�v�X���K�v�Ȃ��ꍇ�A�������폜���Ă��������B
//	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: �c�[�� �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�͈ȉ��̂R�s���폜
	//       ���Ă��������B
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_dbarLayer);
	DockControlBar(&m_wndToolBar);



	addExtensionMenu();

	return 0;
}

void CMainFrame::addExtensionMenu()
{
	TCHAR szKey[32];
	CString menuTitle;
	CString option;
	CString cmd;
	int n;
	
	CMenu menuPopup;
	menuPopup.CreatePopupMenu();
	n=0;
	while(n<10){
		_stprintf_s(szKey,sizeof(szKey)/sizeof(szKey[0]),_T("Menu%d"),n);
		CString strMenuInfo=AfxGetApp()->GetProfileString(_T("ExtensionMenu"),szKey);
		if(strMenuInfo.IsEmpty()) break;
		int top,end;
		top=end=0;
		end++;
		while(strMenuInfo[end]!=',' && strMenuInfo[end]) end++;
		if(!strMenuInfo[end])break;
		menuTitle=strMenuInfo.Mid(top,end-top);
		
		top=end+1;
		end=top;
		if(!strMenuInfo[top])break;
		while(strMenuInfo[end]!=',' && strMenuInfo[end]) end++;
		if(!strMenuInfo[end])break;
		option=strMenuInfo.Mid(top,end-top);

		top=end+1;
		if(!strMenuInfo[top])break;
		cmd=strMenuInfo.Mid(top);

		m_aryExtensionMenu[n].setOption(option);
		m_aryExtensionMenu[n].m_cmd=cmd;
		menuPopup.AppendMenu(MF_STRING, IDM_EXTMENU_0+n,menuTitle);
		n++;
	}
	m_extensionMenuCount=n;
	if(n>0){
		CString str;
		str.LoadString(IDS_EXTMENU_TITLE);
		CMenu* pMenu=GetMenu();
		pMenu->InsertMenu(6,MF_POPUP|MF_BYPOSITION,(UINT)menuPopup.Detach(),str);
	}

}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C�����āAWindow �N���X��X�^�C����
	//       �C�����Ă��������B
	/////////////////////////////////////////////////////////////////////////////
	if(cs.lpszClass==NULL){
		LPCTSTR lpszClassName=AfxRegisterWndClass(	//�E�B���h�E�N���X�̐V�K�쐬
			CS_DBLCLKS, 
			AfxGetApp()->LoadStandardCursor(IDC_ARROW),										//�J�[�\����NULL
			(HBRUSH)GetStockObject(WHITE_BRUSH),
			AfxGetApp()->LoadIcon(IDR_MAINFRAME));	//�A�v���P�[�V�����W���A�C�R��
		cs.lpszClass=lpszClassName;
	}
	cs.cx = m_sizeInitialWindowSize.cx;
	cs.cy = m_sizeInitialWindowSize.cy;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̐f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���b�Z�[�W �n���h��



////////////////////////////////////////////////////////////
// �X�e�[�^�X�o�[�̕\���^��\���؂�ւ�
// AppWizard�����������X�e�[�^�X�o�[��BSch�Ǝ���ID�������̂ɒu������������
// ���̃n���h�����K�v�ɂȂ�B
// �Q�l Inside Visual C++ VERSION 4(�A�X�L�[�j13��
void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck((m_wndStatusBar.GetStyle()&WS_VISIBLE)!=0);	
}

void CMainFrame::OnViewStatusBar() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	//�X�e�[�^�X�o�[�̕\���E��\�������W�X�g���ɕۑ� 1997/01/25
	BOOL bDisplay=((m_wndStatusBar.GetStyle()&WS_VISIBLE)==0);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("DisplayStatusBar"),bDisplay);
	CFrameWnd::OnBarCheck(ID_BSCH_STATUS_BAR);	//1997/04/11 �ǉ�
	//m_wndStatusBar.ShowWindow(bDisplay);		//1997/04/11 �폜
	//RecalcLayout();							//1997/04/11 �폜
}

//ppoint�̒l���X�e�[�^�X�o�[�ɕ\��
void CMainFrame::SetStatusBarXY(const POINT* ppoint)
{
	TCHAR str[32];
	wsprintf(str,_T("%5d"),ppoint->x);
	m_wndStatusBar.SetPaneText(3,str);
	wsprintf(str,_T("%5d"),ppoint->y);
	m_wndStatusBar.SetPaneText(4,str);
}

//�X�e�[�^�X�o�[�̃y�C��0�Ƀ��b�Z�[�W���o�͂���
void CMainFrame::SetMessageLine(CString& rStr)
{
	m_wndStatusBar.SetPaneText(0,rStr);
}

void CMainFrame::OnViewToolbar() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	BOOL bDisplay=((m_wndToolBar.GetStyle()&WS_VISIBLE)==0);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("DisplayToolBar"),bDisplay);
	OnBarCheck(AFX_IDW_TOOLBAR);	//1997/04/11 �ǉ�
//	CFrameWnd::OnBarCheck(AFX_IDW_TOOLBAR);	//1997/04/11 �ǉ�
	//m_wndToolBar.ShowWindow(bDisplay);	//1997/04/11 �폜
	//RecalcLayout();						//1997/04/11 �폜
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck((m_wndToolBar.GetStyle()&WS_VISIBLE)!=0);	
}


void CMainFrame::OnViewLayerbox()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	BOOL bDisplay=((m_dbarLayer.GetStyle()&WS_VISIBLE)==0);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("DisplayLayerDlgBar"),bDisplay);

	ShowControlBar(&m_dbarLayer, bDisplay, FALSE);

//	OnBarCheck(IDD_DBAR_LAYER);
}

void CMainFrame::OnUpdateViewLayerbox(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck((m_dbarLayer.GetStyle()&WS_VISIBLE)!=0);	
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	WINDOWPLACEMENT wp;
	this->GetWindowPlacement(&wp);
	int wpcx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	int wpcy = wp.rcNormalPosition.bottom- wp.rcNormalPosition.top;
	TRACE("CMainFrame::OnSize()  %d,%d\n",wpcx,wpcy);
	if(400<=wpcx && 300<=wpcy){
		IniWriteWindowPosition(wp.showCmd,wpcx,wpcy);
	}
	

}

CSize CMainFrame::m_sizeInitialWindowSize = CSize(CW_USEDEFAULT,CW_USEDEFAULT);

void CMainFrame::SetInitialWindowSize(int cx,int cy)
{
	if(cx!=CW_USEDEFAULT && cx<400) cx = 400;
	if(cy!=CW_USEDEFAULT && cy<300) cy = 300;
	m_sizeInitialWindowSize = CSize(cx,cy);
}


#define WINDOW_POS_SECTION _T("WindowPosition")
void CMainFrame::IniWriteWindowPosition(int cmdShow,int cx,int cy)
{
	TRACE(_T("CMainFrame::IniWriteWindowPosition()  %d,%d,%d\n"),cmdShow,cx,cy);
	AfxGetApp()->WriteProfileInt(WINDOW_POS_SECTION,_T("cmdShow"),cmdShow);
	AfxGetApp()->WriteProfileInt(WINDOW_POS_SECTION,_T("Width"),cx);
	AfxGetApp()->WriteProfileInt(WINDOW_POS_SECTION,_T("Height"),cy);
}
void CMainFrame::IniReadWindowPosition(int& cmdShow,int& cx,int& cy)
{
	cmdShow = AfxGetApp()->GetProfileInt(WINDOW_POS_SECTION,_T("cmdShow"),SW_NORMAL);
	cx = AfxGetApp()->GetProfileInt(WINDOW_POS_SECTION,_T("Width"),CW_USEDEFAULT);
	cy = AfxGetApp()->GetProfileInt(WINDOW_POS_SECTION,_T("Height"),CW_USEDEFAULT);
}

LRESULT CMainFrame::OnScrollToMsg(UINT wParam,LONG lParam)
{
	g_pViewWindow->SendMessage(SCROLL_TO_MSG,0,lParam);

	//int x = (int)LOWORD(lParam);
	//int y = (int)HIWORD(lParam);
	//CString msg;
	//msg.Format("OnScrollToMsg %d:%d",x,y);
	//AfxMessageBox(msg);
	return 0;
}

