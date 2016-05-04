/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
// nu3wDlg.cpp : �����t�@�C��
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

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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


// Cnu3wDlg �_�C�A���O



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


// Cnu3wDlg ���b�Z�[�W �n���h��

BOOL Cnu3wDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_SMALL);
	SetIcon(hIcon, FALSE);		// �������A�C�R����ݒ�

	//SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�



	// TODO: �������������ɒǉ����܂��B

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
	//  �`�F�b�N�{�b�N�X�̐ݒ�
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

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void Cnu3wDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR Cnu3wDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�\�[�X�̃��X�g�{�b�N�X�̏�ԂŃ{�^���̃C�l�[�u����Ԃ�ݒ�
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
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

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
			case 1://���łɓ����t�@�C����ǂݍ��݂���
				break;
			case 2://�ǂݍ��݃G���[
				break;
			case 3://�G���[����Ȃ����ǐV����BSch3V�ō��ꂽ�f�[�^��
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
	if(nInit<1 || nInit>g_maxInitValue){							//�w��͈͊O�Ȃ�
		SetDlgItemInt(IDC_INIT,1);									//�f�t�H���g�l��1��ݒ肵��
		CString msg;
		msg.Format(IDS_INIT_RANGE_ERR,g_maxInitValue);
		AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);		//���b�Z�[�W�{�b�N�X�\��
		CEdit* pEdit=(CEdit*)GetDlgItem(IDC_INIT);
		pEdit->SetSel(0,-1);
		return;
	}
	int nStep=GetDlgItemInt(IDC_STEP);
	if(nStep<1 || nStep>g_maxStepValue){							//�w��͈͊O�Ȃ�
		SetDlgItemInt(IDC_STEP,1);									//�f�t�H���g�l��1��ݒ肵��
		CString msg;
		msg.Format(IDS_STEP_RANGE_ERR,g_maxStepValue);
		AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);		//���b�Z�[�W�{�b�N�X�\��
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
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
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
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int nResult;
	TCHAR szNames[FILE_NAME_BUFFER_SIZE];
	_tcscpy(szNames,_T("*.CE3"));

	CFileDialog dlg(TRUE,				//�I�[�v���_�C�A���O
					NULL,				//�g���q���t�����Ȃ������ꍇ�̃f�t�H���g
					NULL,			//�����t�@�C����
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("BSch3 data(*.ce3)|*.CE3||"),	//�t�B���^������
					this);				//�e�E�C���h�E
	
	dlg.m_ofn.lpstrFile = szNames;	//�t�@�C�������i�[����o�b�t�@��ݒ肷��B
	dlg.m_ofn.nMaxFile = FILE_NAME_BUFFER_SIZE;		//�t�@�C�������i�[����o�b�t�@�̃T�C�Y��ݒ肷��B
	
	nResult=dlg.DoModal();			//�t�@�C���_�C�A���O�̎��s
	if(nResult==IDOK){		//����I���Ȃ�c
		CDdLb* pLB=(CDdLb*)GetDlgItem(IDC_LIST_SRC);	//���X�g�{�b�N�X���擾���āc
		//pLB->ResetContent();
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		int nSelect = pLB->GetCurSel();
		int nCount  = pLB->GetCount();
		if(nSelect==LB_ERR) nSelect=-1;
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			nSelect = pLB->InsertString(nSelect,strPath);	//���X�g�{�b�N�X�ւ̃Z�b�g
			nSelect++;
		}
		pLB->SetListboxWidth();
	}
	setButtonEnable();

}

void Cnu3wDlg::OnLbnSelchangeListSrc()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	setButtonEnable();
}

void Cnu3wDlg::OnBnClickedDown()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
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
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
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

	// TODO:  �����ɏ�������ǉ����Ă�������

	CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = "NUT3W Version ";
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void Cnu3wDlg::OnBnClickedCheckEnablePrefixfilter()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	setPrefixFilterControlEnable();
}

void Cnu3wDlg::setPrefixFilterControlEnable()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	bool enable = (((CButton*)GetDlgItem(IDC_CHECK_ENABLE_PREFIXFILTER))->GetCheck()!=0);
	GetDlgItem(IDC_EDIT_PREFIXFILTER)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_DONT)->EnableWindow(enable);
	GetDlgItem(IDC_RADIO_ONLY)->EnableWindow(enable);
}



void Cnu3wDlg::OnBnClickedNewlyonly()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
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
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	bool enable = true;
	if(((CButton*)GetDlgItem(IDC_EN_LAYER))->GetCheck()){
		((CButton*)GetDlgItem(IDC_ANUM))->SetCheck(1);
		enable = false;
	}
	GetDlgItem(IDC_ANUM)->EnableWindow(enable);
}
