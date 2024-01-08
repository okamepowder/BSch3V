/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


// nlistDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "nlist.h"
#include "nlistDlg.h"
#include "plist.h"

#include "atlpath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNlistDlg �_�C�A���O

CNlistDlg::CNlistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNlistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNlistDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_noMessageBox = FALSE;
}

void CNlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNlistDlg)
	DDX_Control(pDX, IDC_LIST_SRC, m_listsrc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNlistDlg, CDialog)
	//{{AFX_MSG_MAP(CNlistDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REFSRC, OnRefsrc)
	ON_BN_CLICKED(IDC_CLR, OnClr)
	ON_LBN_SELCHANGE(IDC_LIST_SRC, OnSelchangeListSrc)
	ON_BN_CLICKED(IDC_REFDST, OnRefdst)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_CSVOUT, &CNlistDlg::OnBnClickedRadioCsvout)
	ON_BN_CLICKED(IDC_RADIO_SYLKOUT, &CNlistDlg::OnBnClickedRadioSylkout)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNlistDlg ���b�Z�[�W �n���h��

BOOL CNlistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_SMALL);
	SetIcon(hIcon, FALSE);		// �������A�C�R����ݒ�
//	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	CString title;
	GetWindowText(title);

	CString strVersion;
	strVersion.LoadString(IDS_VERSION);

	title = title + _T(" Ver. ") + strVersion;
	SetWindowText(title);


	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	const CStringList& strListSrcFiles=((CNlistApp*)AfxGetApp())->m_cmdInfo.m_strListSrcFiles;
	POSITION pos = strListSrcFiles.GetHeadPosition();
	while(pos){
		CString fileName = strListSrcFiles.GetNext(pos);
		pLB->AddString(fileName);
	}






	int check;

	//�I�v�V�����̐ݒ�
	//�R�}���h���C���̐ݒ���m�F���āA0�Ȃ�R�}���h���C���̎w�肪�Ȃ��̂�INI�t�@�C���ݒ���g��
	//check��1�Ȃ�`�F�b�N�A0�܂���-1�Ȃ�I�t

	//check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sAvoidZeroSupOnExcel;
	//if(check == 0){
	//	check = AfxGetApp()->GetProfileInt("OPTION","AvoidZeroSupOnExcel",0);
	//}
	//((CButton*)GetDlgItem(IDC_CHECK_AVOID_ZERO_SUPPRESS))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));


	//�o�̓t�@�C���`���̏����ݒ�
	//�R�}���h���C���Őݒ肳��Ă�����AINI�t�@�C���ݒ���D�悷��B
	//�o�̓t�@�C�����̊g���q��CSV�܂���SLK�Ȃ炻��ŃI�[�o�[���C�h����B

	int dstType = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_dstType;
	if(dstType == 0){
		dstType = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("DstType"),0);
	}

	CString name;
	name= ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_strList;
	if(!name.IsEmpty()){
		SetDlgItemText(IDC_EDIT_DST,name);

		TCHAR szExt[_MAX_EXT];
		_tsplitpath(name,NULL,NULL,NULL,szExt);
		if(_tcsicmp(szExt,_T(".xml"))==0){
			dstType = PListParam::PL_FILETYPE_XML;
		}else if(_tcsicmp(szExt,_T(".csv"))==0){
			dstType = PListParam::PL_FILETYPE_CSV;
		}
	}

	((CButton*)GetDlgItem(IDC_RADIO_CSVOUT))->SetCheck((dstType<=1 ? BST_CHECKED : BST_UNCHECKED));
	((CButton*)GetDlgItem(IDC_RADIO_SYLKOUT))->SetCheck((dstType==2 ? BST_CHECKED : BST_UNCHECKED));

	if(name.IsEmpty()){
		SetDstFileName();	//���̊֐��̓��W�I�{�^���̏�Ԃ��`�F�b�N���Ă���̂ŁA���W�I�{�^���̐ݒ��Ɏ��s����K�v������
	}

	
	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sMfr;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Mfr"),0);
	}
	((CButton*)GetDlgItem(IDC_CHECK_MFR))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));


	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sMfrPn;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("MfrPn"),0);
	}
	((CButton*)GetDlgItem(IDC_CHECK_MFRPN))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));
	
	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sPackage;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Package"),0);
	}
	((CButton*)GetDlgItem(IDC_CHECK_PKG))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));
	
	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sNote;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Note"),1);	//�m�[�g�͌݊����̂��߂�1���f�t�H���g�Ƃ���
	}
	((CButton*)GetDlgItem(IDC_CHECK_NOTE))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));

	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sInfo;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Info"),0);	//�m�[�g�͌݊����̂��߂�1���f�t�H���g�Ƃ���
	}
	((CButton*)GetDlgItem(IDC_CHECK_COMPINFO))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));



	if(((CNlistApp*)AfxGetApp())->m_cmdInfo.m_forceExecute){
		m_noMessageBox=TRUE;
		OnOK();
		EndDialog(IDOK);
	}

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CNlistDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CNlistDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CNlistDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CNlistDlg::OnRefsrc() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nResult;
	//TCHAR szFileNameBuffer[FILE_NAME_BUFFER_SIZE];
	_tcscpy(m_szFileNameBuffer,_T("*.CE3"));

	CFileDialog dlg(TRUE,				//�I�[�v���_�C�A���O
					NULL,				//�g���q���t�����Ȃ������ꍇ�̃f�t�H���g
					NULL,			//�����t�@�C����
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("BSch3 data(*.ce3)|*.CE3||"),	//�t�B���^������
					this);				//�e�E�C���h�E
	
	dlg.m_ofn.lpstrFile = m_szFileNameBuffer;	//�t�@�C�������i�[����o�b�t�@��ݒ肷��B
	dlg.m_ofn.nMaxFile = FILE_NAME_BUFFER_SIZE;		//�t�@�C�������i�[����o�b�t�@�̃T�C�Y��ݒ肷��B
	
	nResult=dlg.DoModal();			//�t�@�C���_�C�A���O�̎��s
	if(nResult==IDOK){		//����I���Ȃ�c
		CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);	//���X�g�{�b�N�X���擾���āc
		//pLB->ResetContent();
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			pLB->AddString(strPath);	//���X�g�{�b�N�X�ւ̃Z�b�g
		}
		pLB->SetListboxWidth();
		SetDstFileName();
	}

}

void CNlistDlg::OnClr() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	pLB->ResetContent();
	SetDstFileName();

}

void CNlistDlg::OnSelchangeListSrc() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	//TRACE("CNlistDlg::OnSelchangeListSrc()\n");
	SetDstFileName();
}


int CNlistDlg::DstFileSetting()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_CSVOUT))->GetCheck() == BST_CHECKED){
		return PListParam::PL_FILETYPE_CSV;	//CSV
	}else{
		return PListParam::PL_FILETYPE_XML;	//XML
	}
}


void CNlistDlg::SetDstFileName()
{
	int fileType = DstFileSetting();

	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
	CButton* pBtn=(CButton*)GetDlgItem(IDOK);
	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	int n = pLB->GetCount();
	if(n>0){
		CString strSrc;
		int index = pLB->GetCurSel();
		if(index == LB_ERR){
			index = 0;
		}
		pLB->GetText(index,strSrc);
		TCHAR szPath[_MAX_PATH];
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szName[_MAX_FNAME];
		_tsplitpath(strSrc,szDrive,szDir,szName,NULL);
		if(fileType == 0){
			_tmakepath(szPath,szDrive,szDir,szName,_T(".csv"));
		}else{
			_tmakepath(szPath,szDrive,szDir,szName,_T(".xml"));
		}
		pEdit->SetWindowText(szPath);
		pBtn->EnableWindow(TRUE);
	}else{
		pEdit->SetWindowText(_T(""));
		pBtn->EnableWindow(FALSE);
	}
}

void CNlistDlg::OnRefdst() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nResult;
//	TCHAR szFileNameBuffer[_MAX_PATH];
//	strcpy(szFileNameBuffer,"*.CE3");

	static const TCHAR* CSV_FILTER = _T("CSV file(*.csv)|*.CSV||");
	static const TCHAR* XML_FILTER = _T("Excel 2007 XML file(*.xml)|*.xml||");


	const TCHAR* pcszFilter = (DstFileSetting()<=1 ? CSV_FILTER : XML_FILTER);

	CFileDialog dlg(TRUE,				//�I�[�v���_�C�A���O
					NULL,				//�g���q���t�����Ȃ������ꍇ�̃f�t�H���g
					NULL,			//�����t�@�C����
					OFN_HIDEREADONLY,
					pcszFilter,	//�t�B���^������
					this);				//�e�E�C���h�E
	
	
	nResult=dlg.DoModal();			//�t�@�C���_�C�A���O�̎��s
	if(nResult==IDOK){				//����I���Ȃ�c
		CString strPath = dlg.GetPathName();
		//CPath strPath = dlg.GetPathName();

		//CString strExt = dlg.GetFileExt();
		//strExt.MakeLower();
		//if(strExt!="csv" && strExt!="slk"){
		//	int extIndex = dlg.m_ofn.nFilterIndex;
		//	TRACE("%d\n",extIndex);
		//	if(extIndex == 2){
		//		strPath.RenameExtension(".slk");
		//	}else{
		//		strPath.RenameExtension(".csv");
		//	}

		//}

		CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
		pEdit->SetWindowText(strPath);
	}	
}

void CNlistDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	SPlist plist;

	PListParam param;
	
	bool setflag;
	
	//setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_AVOID_ZERO_SUPPRESS))->GetCheck() == BST_CHECKED) ? true : false );
	//param.plAvoidZeroSuppressingOnMsExcel = setflag;
	//AfxGetApp()->WriteProfileInt("OPTION","AvoidZeroSupOnExcel",(setflag? 1: 0));

	//param.plAvoidZeroSuppressingOnMsExcel = (AfxGetApp()->GetProfileInt("OPTION","AvoidZeroSupOnExcelCSV",0)!=0);

	int dstType = DstFileSetting();
	plist.m_param.listtype = dstType;	//CSVOUT�łȂ���΁ASYLK
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("DstType"),dstType);

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_MFR))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagMfr = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Mfr"),(setflag? 1: 0));

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_MFRPN))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagMfrPn = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("MfrPn"),(setflag? 1: 0));

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_PKG))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagPackage = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Package"),(setflag? 1: 0));

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_NOTE))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagNote = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Note"),(setflag? 1: 0));


	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_COMPINFO))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagInfo = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Info"),(setflag? 1: 0));


	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	if(nCount==0){
		return;
	}


	CString str;
	int readCount=0;
	for(int n=0;n<nCount;n++){
		pLB->GetText(n,str);
		int retval=0;
		retval=plist.readFile(str);
		switch(retval){
			case 1://���łɓ����t�@�C����ǂݍ��݂���
				break;
			case 2://�ǂݍ��݃G���[
				break;
			case 3:
				break;
			default:
				break;
		}
	}

	if(plist.fileCount()<nCount){
		if(!m_noMessageBox)	AfxMessageBox(IDS_SRCOPENERR);
		return;
	}


	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
	CString strDstPath;
	pEdit->GetWindowText(strDstPath);


//	FILE* fp =fopen(strDstPath,"wt");
//	FILE* fp =_tfopen(strDstPath,_T("wb"));

	//���i�\���쐬����B
	int result = plist.writePartsList(strDstPath);

	if(result == -2){
		if(!m_noMessageBox)	AfxMessageBox(IDS_DSTOPENERR);
		return;
	//}else if(result == -1){

	}else{
		if(!m_noMessageBox)	AfxMessageBox(IDS_FINISHED, MB_OK | MB_ICONINFORMATION);
	}

	//CDialog::OnOK();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
		CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = _T("PL3W Version ");
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CNlistDlg::OnBnClickedRadioCsvout()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	TRACE("CNlistDlg::OnBnClickedRadioCsvout()\n");
	SetDstFileName();
}

void CNlistDlg::OnBnClickedRadioSylkout()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	TRACE("CNlistDlg::OnBnClickedRadioSylkout()\n");
	SetDstFileName();
}
