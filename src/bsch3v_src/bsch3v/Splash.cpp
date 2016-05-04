/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


#include "stdafx.h"  // e. g. stdafx.h
#include "resource.h"  // e.g. resource.h

#include "Splash.h"  // e.g. splash.h

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   �X�v���b�V�� �X�N���[�� �N���X

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;
CSplashWnd::CSplashWnd()
{
}

CSplashWnd::~CSplashWnd()
{
	// �X�^�e�B�b�N�ȃE�B���h�E �|�C���^�̃N���A
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// �X�v���b�V�� �X�N���[����V�K�ɃA���P�[�g���A�E�B���h�E���쐬���܂��B
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

void CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return;

	// �L�[�{�[�h�܂��̓}�E�X ���b�Z�[�W���󂯎�����ꍇ�ɃX�v���b�V�� �X�N���[�����\���ɂ��܂��B
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
	}
}

BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
	if (!m_bitmap.LoadBitmap(IDB_SPLASH))
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

void CSplashWnd::HideSplashScreen()
{
	// �E�B���h�E��j�����A���C���t���[�����X�V���܂��B
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
}

void CSplashWnd::PostNcDestroy()
{
	// C++ �N���X�� Free
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �E�B���h�E�𒆉��ɔz�u���܂��B
	CenterWindow();

	// �X�v���b�V�� �X�N���[����j������܂ł̃^�C�}��ݒ肵�܂��B
	SetTimer(1,750, NULL);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// �摜��`�悵�܂��B
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// �X�v���b�V�� �X�N���[�� �E�B���h�E��j�����܂��B
	HideSplashScreen();
}
