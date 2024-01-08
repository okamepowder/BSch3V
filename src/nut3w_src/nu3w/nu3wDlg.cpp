/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
// nu3wDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Global.h"
#include "nu3w.h"
#include "nu3wDlg.h"
#include "SNut.h"
#include "DdLb.h"
#include "cmdlineparam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FILE_NAME_BUFFER_SIZE (_MAX_PATH * 64  + 1)

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cnu3wDlg ダイアログ



Cnu3wDlg::Cnu3wDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cnu3wDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_noMessageBox = FALSE;
}

void Cnu3wDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SRC, m_lbSrc);
}

BEGIN_MESSAGE_MAP(Cnu3wDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(ID_REF, OnBnClickedRef)
	ON_BN_CLICKED(ID_CLR, OnBnClickedClr)
	ON_BN_CLICKED(ID_REF, OnBnClickedRef)
	ON_LBN_SELCHANGE(IDC_LIST_SRC, OnLbnSelchangeListSrc)
	ON_BN_CLICKED(ID_DOWN, OnBnClickedDown)
	ON_BN_CLICKED(ID_UP, OnBnClickedUp)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_PREFIXFILTER, &Cnu3wDlg::OnBnClickedCheckEnablePrefixfilter)
//	ON_BN_CLICKED(IDC_RADIO_DONT, &Cnu3wDlg::OnBnClickedRadioDont)
ON_BN_CLICKED(IDC_NEWLYONLY, &Cnu3wDlg::OnBnClickedNewlyonly)
ON_BN_CLICKED(IDC_EN_LAYER, &Cnu3wDlg::OnClickedEnLayer)
END_MESSAGE_MAP()


// Cnu3wDlg メッセージ ハンドラ

BOOL Cnu3wDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_SMALL);
	SetIcon(hIcon, FALSE);		// 小さいアイコンを設定

	//SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定



	// TODO: 初期化をここに追加します。

	CString title;
	GetWindowText(title);

	CString strVersion;
	strVersion.LoadString(IDS_VERSION);

	title = title + _T(" Ver. ") + strVersion;
	SetWindowText(title);


	

	CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);
	const CStringList& strListSrcFiles=((Cnu3wApp*)AfxGetApp())->m_cmdInfo.m_strListSrcFiles;
	POSITION pos = strListSrcFiles.GetHeadPosition();
	while(pos){
		CString fileName = strListSrcFiles.GetNext(pos);
		pLB->AddString(fileName);
	}

	CStatic* pStatic;
	TCHAR szNumVal[64]; 

	pStatic  = (CStatic*)GetDlgItem(IDC_STATIC_INIT_VAL);
	CString strStatic; 
	pStatic->GetWindowText(strStatic);
	_stprintf(szNumVal,_T("%d"),g_maxInitValue);
	strStatic.Append(szNumVal);
	pStatic->SetWindowText(strStatic);

	pStatic  = (CStatic*)GetDlgItem(IDC_STATIC_STEP_VAL);
	pStatic->GetWindowText(strStatic);
	_stprintf(szNumVal,_T("%d"),g_maxStepValue);
	strStatic.Append(szNumVal);
	pStatic->SetWindowText(strStatic);




	setButtonEnable();
	int n;
	CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_INIT);
	//pSpin->SetRange(1,PARTNUM_INIT_MAX);
	pSpin->SetRange(1,g_maxInitValue);
	n=((Cnu3wApp*)AfxGetApp())->m_cmdInfo.m_initNum;
	if(n<0)n=1;
	//else if(n>PARTNUM_INIT_MAX) n=PARTNUM_INIT_MAX;
	else if(n>g_maxInitValue) n=g_maxInitValue;
	pSpin->SetPos(n);

	pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_STEP);
	//pSpin->SetRange(1,PARTNUM_STEP_MAX);
	pSpin->SetRange(1,g_maxStepValue);
	n=((Cnu3wApp*)AfxGetApp())->m_cmdInfo.m_stepNum;
	if(n<0)n=1;
	//else if(n>PARTNUM_STEP_MAX) n=PARTNUM_STEP_MAX;
	else if(n>g_maxStepValue) n=g_maxStepValue;
	pSpin->SetPos(n);

	/////////////////////////////////////////////////////
	//  チェックボックスの設定
	n=((Cnu3wApp*)AfxGetApp())->m_cmdInfo.m_flag3sAutoNum;
	CButton* pCheck=(CButton*)GetDlgItem(IDC_ANUM);
	pCheck->SetCheck(n>0);

	if(((Cnu3wApp*)AfxGetApp())->m_cmdInfo.m_forceExecute){
		m_noMessageBox=TRUE;
		OnOK();
		EndDialog(IDOK);
	}

	((CButton*)GetDlgItem(IDC_EN_LAYER))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK_ENABLE_PREFIXFILTER))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_DONT))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_NEWLYONLY))->SetCheck(0);

	setPrefixFilterControlEnable();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void Cnu3wDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void Cnu3wDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR Cnu3wDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//ソースのリストボックスの状態でボタンのイネーブル状態を設定
void Cnu3wDlg::setButtonEnable()
{
	CButton* pButton;
	CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	int nSelect = pLB->GetCurSel();
	pButton=(CButton*)GetDlgItem(IDOK);
	pButton->EnableWindow(nCount>0);
	pButton=(CButton*)GetDlgItem(ID_CLR);
	pButton->EnableWindow(nCount>0);
	pButton=(CButton*)GetDlgItem(ID_UP);
	pButton->EnableWindow(nCount>0 && nSelect!= LB_ERR && nSelect>0);
	pButton=(CButton*)GetDlgItem(ID_DOWN);
	pButton->EnableWindow(nCount>0 && nSelect!= LB_ERR && nSelect<nCount-1);
}

void Cnu3wDlg::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	SNut snut;

	CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	if(nCount==0){
		return;
	}
	CString str;
	int readCount=0;
	for(int n=0;n<nCount;n++){
		CString msg;
		CString ldmsg;
		const TCHAR* pcsz;

		pLB->GetText(n,str);
		int retval=0;
		retval=snut.readFile(str);
		switch(retval){
			case 1://すでに同じファイルを読み込みずみ
				break;
			case 2://読み込みエラー
				break;
			case 3://エラーじゃないけど新しいBSch3Vで作られたデータだ
				/*pcsz = snut.filename(snut.fileCount()-1);
				if(pcsz){
					msg = pcsz;
					msg += "\n";
					ldmsg.LoadString(IDS_LATERVERSION);
					msg += ldmsg;
					if(AfxMessageBox(msg,MB_OKCANCEL)==IDCANCEL){
						return;
					}
				}*/
				break;
			default:
				break;
		}
	}
	if(snut.fileCount()==0)return;


	int nInit=GetDlgItemInt(IDC_INIT);
	if(nInit<1 || nInit>g_maxInitValue){							//指定範囲外なら
		SetDlgItemInt(IDC_INIT,1);									//デフォルト値の1を設定して
		CString msg;
		msg.Format(IDS_INIT_RANGE_ERR,g_maxInitValue);
		AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);		//メッセージボックス表示
		CEdit* pEdit=(CEdit*)GetDlgItem(IDC_INIT);
		pEdit->SetSel(0,-1);
		return;
	}
	int nStep=GetDlgItemInt(IDC_STEP);
	if(nStep<1 || nStep>g_maxStepValue){							//指定範囲外なら
		SetDlgItemInt(IDC_STEP,1);									//デフォルト値の1を設定して
		CString msg;
		msg.Format(IDS_STEP_RANGE_ERR,g_maxStepValue);
		AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);		//メッセージボックス表示
		CEdit* pEdit=(CEdit*)GetDlgItem(IDC_STEP);
		pEdit->SetSel(0,-1);
		return;
	}

	snut.renameToBackup();

	snut.m_enablePrefixFilter = (((CButton*)GetDlgItem(IDC_CHECK_ENABLE_PREFIXFILTER))->GetCheck() == 1);
	snut.m_prefixFilterDontMode = (((CButton*)GetDlgItem(IDC_RADIO_DONT))->GetCheck()== 1);
	bool newlyOnly = (((CButton*)GetDlgItem(IDC_NEWLYONLY))->GetCheck()== 1);
	bool anumMode = newlyOnly || (((CButton*)GetDlgItem(IDC_ANUM))->GetCheck() == 1);
	bool en_layer = (((CButton*)GetDlgItem(IDC_EN_LAYER))->GetCheck()== 1);

	CString filterString;
	GetDlgItem(IDC_EDIT_PREFIXFILTER)->GetWindowText(filterString);
	snut.setPrefixFilter(filterString);

	if(anumMode){
		unsigned int option=0;
		if(newlyOnly){
			option |= SNut::NUTOPTION_NEWONLY;
		}
		if(en_layer){
			option |= SNut::NUTOPTION_EN_LAYER;
		}
		snut.anum(nInit,nStep,option);
	}else{
		snut.renum(nInit,nStep);
	}


	snut.saveFiles();
	if(!m_noMessageBox){
		AfxMessageBox(IDS_FINISHED);
	}
	//CDialog::OnOK();
}


void Cnu3wDlg::OnBnClickedClr()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	if(nCount){
		int nSelect = pLB->GetCurSel();
		if(nSelect == LB_ERR) nSelect = 0;
		pLB->DeleteString(nSelect);
		nCount --;
		if(nCount>0){
			if(nSelect>=nCount)nSelect=nCount-1;
			if(nSelect>=0)pLB->SetCurSel(nSelect);
		}
	}	
	setButtonEnable();
}

void Cnu3wDlg::OnBnClickedRef()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	int nResult;
	TCHAR szNames[FILE_NAME_BUFFER_SIZE];
	_tcscpy(szNames,_T("*.CE3"));

	CFileDialog dlg(TRUE,				//オープンダイアログ
					NULL,				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("BSch3 data(*.ce3)|*.CE3||"),	//フィルタ文字列
					this);				//親ウインドウ
	
	dlg.m_ofn.lpstrFile = szNames;	//ファイル名を格納するバッファを設定する。
	dlg.m_ofn.nMaxFile = FILE_NAME_BUFFER_SIZE;		//ファイル名を格納するバッファのサイズを設定する。
	
	nResult=dlg.DoModal();			//ファイルダイアログの実行
	if(nResult==IDOK){		//正常終了なら…
		CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);	//リストボックスを取得して…
		//pLB->ResetContent();
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		int nSelect = pLB->GetCurSel();
		int nCount  = pLB->GetCount();
		if(nSelect==LB_ERR) nSelect=-1;
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			nSelect = pLB->InsertString(nSelect,strPath);	//リストボックスへのセット
			nSelect++;
		}
		pLB->SetListboxWidth();
	}
	setButtonEnable();

}

void Cnu3wDlg::OnLbnSelchangeListSrc()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	setButtonEnable();
}

void Cnu3wDlg::OnBnClickedDown()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CString string;
	CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	if(nCount>=2){
		int nSelect = pLB->GetCurSel();
		if(nSelect == LB_ERR || nSelect>=nCount -1) return;
		pLB->GetText(nSelect,string);
		pLB->DeleteString(nSelect);
		nCount--;
		nSelect++;
		if(nSelect>=nCount)nSelect=-1;
		nSelect=pLB->InsertString(nSelect,string);
		pLB->SetCurSel(nSelect);
	}	
	setButtonEnable();
}

void Cnu3wDlg::OnBnClickedUp()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CString string;
	CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	if(nCount>=2){
		int nSelect = pLB->GetCurSel();
		if(nSelect == LB_ERR || nSelect < 1) return;
		pLB->GetText(nSelect,string);
		pLB->DeleteString(nSelect);
		//nCount--;
		nSelect--;
		//if(nSelect>=nCount)nSelect=-1;
		nSelect=pLB->InsertString(nSelect,string);
		pLB->SetCurSel(nSelect);
	}	
	setButtonEnable();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = "NUT3W Version ";
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void Cnu3wDlg::OnBnClickedCheckEnablePrefixfilter()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	setPrefixFilterControlEnable();
}

void Cnu3wDlg::setPrefixFilterControlEnable()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	bool enable = (((CButton*)GetDlgItem(IDC_CHECK_ENABLE_PREFIXFILTER))->GetCheck()!=0);
	GetDlgItem(IDC_EDIT_PREFIXFILTER)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_DONT)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_ONLY)->EnableWindow(enable);
}



void Cnu3wDlg::OnBnClickedNewlyonly()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	bool enable = true;
	if(((CButton*)GetDlgItem(IDC_NEWLYONLY))->GetCheck()){
		SetDlgItemInt(IDC_INIT,1);
		SetDlgItemInt(IDC_STEP,1);
		((CButton*)GetDlgItem(IDC_ANUM))->SetCheck(1);
		enable = false;
	}
	GetDlgItem(IDC_ANUM)->EnableWindow(enable);
	GetDlgItem(IDC_INIT)->EnableWindow(enable);
	GetDlgItem(IDC_STEP)->EnableWindow(enable);

}


void Cnu3wDlg::OnClickedEnLayer()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	bool enable = true;
	if(((CButton*)GetDlgItem(IDC_EN_LAYER))->GetCheck()){
		((CButton*)GetDlgItem(IDC_ANUM))->SetCheck(1);
		enable = false;
	}
	GetDlgItem(IDC_ANUM)->EnableWindow(enable);
}
