// SetAddIn.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "nlist.h"
#include "SetAddIn.h"
#include "SAddInFiles.h"

// CSetAddIn �_�C�A���O

IMPLEMENT_DYNAMIC(CSetAddIn, CDialog)

CSetAddIn::CSetAddIn(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAddIn::IDD, pParent)
{
	m_nAddinCount = 0;
}

CSetAddIn::~CSetAddIn()
{
}

void CSetAddIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CSetAddIn::SetListboxWidth()
{
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	CDC* pDC;
	CSize textsize;
	int maxwidth=0;
	int i,n;
	pDC = pLB->GetDC();

	n = pLB->GetCount();
	for(i=0;i<n;i++){
		pLB->GetText(i,str);
		textsize = pDC->GetTextExtent(str);
		if(textsize.cx>maxwidth) maxwidth = textsize.cx;
	}
	pLB->SetHorizontalExtent(maxwidth);
	pLB->ReleaseDC(pDC);
}

//�_�C�A���O�\���{�^���̃C�l�[�u��
void CSetAddIn::EnableButton(UINT id,BOOL bEnable)
{
	CButton*  pBtn=(CButton*)GetDlgItem(id);
	// �f�B�Z�[�u�����悤�Ƃ���{�^���Ƀt�H�[�J�X������ꍇ��
	// �n�j�{�^���Ƀt�H�[�J�X���Z�b�g����
	if(!bEnable){
		CWnd* pWnd=GetFocus();
		if(pWnd==pBtn){
			CButton* pDefaultBtn=(CButton*)GetDlgItem(IDOK);
			pDefaultBtn->SetFocus();
		}
	}
	pBtn->EnableWindow(bEnable);
}


BEGIN_MESSAGE_MAP(CSetAddIn, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSetAddIn::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CSetAddIn::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CSetAddIn::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CSetAddIn::OnBnClickedButtonUp)
	ON_LBN_SELCHANGE(IDC_LIST_FILES, &CSetAddIn::OnLbnSelchangeListFiles)
END_MESSAGE_MAP()


// CSetAddIn ���b�Z�[�W �n���h��

BOOL CSetAddIn::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	CSize textsize;
	TCHAR szEntry[32];
	int i;


	//���W�X�g���ɓo�^���ꂽ���C�u�����t�@�C�������X�g�{�b�N�X�֐ݒ�
	for(i=0;i<MAX_ADDIN;i++){
		wsprintf(szEntry,_T("ADDIN%d"),i);			//�G���g��������̍쐬
		str=AfxGetApp()->GetProfileString(_T("ADDINLIB"),szEntry);
		if(str.IsEmpty()) break;		//������i�A�h�C���t�@�C�����j���l���ł��Ȃ�������break
		pLB->InsertString(-1,str);		//���X�g�{�b�N�X�̍Ō�ɒǉ�
	}
	SetListboxWidth();


	m_nAddinCount = pLB->GetCount();			//�o�^���̐ݒ�
	EnableButton(IDC_BUTTON_ADD,m_nAddinCount<MAX_ADDIN);	//�܂��o�^������łȂ���Βǉ��\�Ȃ悤�ɁuADD�v���C�l�[�u��
		
	EnableButton(IDC_BUTTON_DOWN,FALSE);		//�u���ֈړ��v�{�^���̓f�B�Z�[�u��
	if(m_nAddinCount!=0){						//�o�^�����O�łȂ����
		pLB->SetCurSel(m_nAddinCount-1);		//���X�g�{�b�N�X�̈�ԉ��̍s�ɃJ�[�\���ݒ�
	}else{										//�o�^�����O�Ȃ��
		EnableButton(IDC_BUTTON_UP,  FALSE);	//�u��ֈړ��v���u�폜�v���f�B�Z�[�u��
		EnableButton(IDC_BUTTON_REMOVE, FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CSetAddIn::OnBnClickedButtonAdd()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
		int nResult;
		if(m_nAddinCount>=MAX_ADDIN) return;	//���łɓo�^����Ȃ�Ȃɂ����Ȃ�

	TCHAR szFileNameBuffer[_MAX_PATH *MAX_ADDIN+256];
	_tcscpy(szFileNameBuffer,_T("*.DLL"));

	CFileDialog dlg(TRUE,				//�I�[�v���_�C�A���O
					_T("dll"),				//�g���q���t�����Ȃ������ꍇ�̃f�t�H���g
					NULL,			//�����t�@�C����
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("Add in (*.dll)|*.DLL||"),	//�t�B���^������
					this);				//�e�E�C���h�E
	
	CString strCaption;
	strCaption.LoadString(IDS_ADD_ADDIN);//�_�C�A���O�{�b�N�X�̃L���v�V���������\�[�X���烍�[�h
	dlg.m_ofn.lpstrTitle=strCaption;		//�t�@�C���_�C�A���O�N���X�ɃL���v�V�����̐ݒ�
	dlg.m_ofn.lpstrFile = szFileNameBuffer;	//�t�@�C�������i�[����o�b�t�@��ݒ肷��B
	dlg.m_ofn.nMaxFile = _MAX_PATH *MAX_ADDIN+256;		//�t�@�C�������i�[����o�b�t�@�̃T�C�Y��ݒ肷��B
	
	nResult=dlg.DoModal();			//�t�@�C���_�C�A���O�̎��s
	if(nResult==IDOK){		//����I���Ȃ�c
//		CString rString=CString(dlg.m_ofn.lpstrFile);		//�t�@�C���_�C�A���O����t�@�C�������擾����
		CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);	//���X�g�{�b�N�X���擾���āc
		int n;
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			n=pLB->GetCurSel();				//���ݑI������Ă���Ƃ���ɑ}�����邽�߂Ɍ��ݒl�擾
			if(n==LB_ERR) n=0;				//���ݑI������Ă�����̂��Ȃ���ΐ擪�ɓ����
			pLB->InsertString(n,strPath);	//���X�g�{�b�N�X�ւ̃Z�b�g
			pLB->SetCurSel(n);				//�I���s�̍Đݒ�
			m_nAddinCount=pLB->GetCount();		//�����o�[�ϐ��ւ̓o�^���̐ݒ�
			if(m_nAddinCount>=MAX_ADDIN) break;
		}
		SetListboxWidth();
		EnableButton(IDC_BUTTON_ADD ,m_nAddinCount<MAX_ADDIN);//�uADD�v�{�^���̐ݒ�
		EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//�u���ցv�{�^���̐ݒ�
		EnableButton(IDC_BUTTON_UP  ,n>0);				//�u��ցv�{�^���̐ݒ�
		EnableButton(IDC_BUTTON_REMOVE ,TRUE);			//�uDEL�v�{�^���̐ݒ�
	}
}

void CSetAddIn::OnBnClickedButtonRemove()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int n;
	if(m_nAddinCount==0) return;		//�o�^�����O�Ȃ牽�����Ȃ�
    CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	n=pLB->GetCurSel();			//�I������Ă�����̂̔ԍ����擾
	if(n==LB_ERR) return;		//�I������Ă�����̂��Ȃ���Ή������Ȃ�
	pLB->DeleteString(n); 		//�I������Ă�����̂��폜
    m_nAddinCount=pLB->GetCount(); 	//�o�^���̐ݒ�
	if(n>=m_nAddinCount) n=m_nAddinCount-1;	//�폜�����̂��ŉ��s�̂��̂Ȃ玟�ɑI������͈̂��
	if(n>=0)pLB->SetCurSel(n);
	SetListboxWidth();
	EnableButton(IDC_BUTTON_ADD, TRUE);
   	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1 && m_nAddinCount>0);
   	EnableButton(IDC_BUTTON_UP,  n>0);
	EnableButton(IDC_BUTTON_REMOVE, m_nAddinCount>0);
}

void CSetAddIn::OnBnClickedButtonDown()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	int n=pLB->GetCurSel();						//�I������Ă���ʒu�̎擾
	if(n==LB_ERR || n>=m_nAddinCount-1) return;	//�I������Ă�����̂��Ȃ������ɍŌ���̏ꍇ�͉������Ȃ�
	pLB->GetText(n,str);						//�I������Ă��镶������擾���āc
	pLB->DeleteString(n);						//			���X�g�{�b�N�X����폜
	n++;										//�����ԍ��������Ă��āc
	pLB->InsertString(n,str);					//			�ǉ����āc
	pLB->SetCurSel(n);							//			�J�[�\����ݒ肵�đI����Ԃɂ���
	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//�u���ցv�{�^���̐ݒ�
	EnableButton(IDC_BUTTON_UP  ,n>0);				//�u��ցv�{�^���̐ݒ�
}

void CSetAddIn::OnBnClickedButtonUp()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	int n=pLB->GetCurSel();			//�I������Ă���ʒu�̎擾
	if(n==LB_ERR || n==0) return;	//�I������Ă�����̂��Ȃ������ɐ擪�̏ꍇ�͉������Ȃ�
	pLB->GetText(n,str);		//�I������Ă��镶������擾���āc
	pLB->DeleteString(n);			//			���X�g�{�b�N�X����폜
	n--;							//�����ԍ����グ�āc
	pLB->InsertString(n,str);	//			�ǉ����āc
	pLB->SetCurSel(n);				//			�J�[�\����ݒ肵�đI����Ԃɂ���
	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//�u���ցv�{�^���̐ݒ�
	EnableButton(IDC_BUTTON_UP  ,n>0);				//�u��ցv�{�^���̐ݒ�
}

void CSetAddIn::OnLbnSelchangeListFiles()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	int n=pLB->GetCurSel();									//�I������Ă���ʒu�̎擾
	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//�u���ցv�{�^���̐ݒ�
	EnableButton(IDC_BUTTON_UP  ,n>0);						//�u��ցv�{�^���̐ݒ�
}

void CSetAddIn::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);	//���X�g�{�b�N�X���擾���āc
	int count = pLB->GetCount();
	for(int index = 0;index<MAX_ADDIN;index++){
		CString addinPath;
		TCHAR szEntry[32];
		wsprintf(szEntry,_T("ADDIN%d"),index);			//�G���g��������̍쐬

		if(index<count){
			pLB->GetText(index,addinPath);
		}else{
			addinPath.Empty();
		}
		AfxGetApp()->WriteProfileString(_T("ADDINLIB"),szEntry,addinPath);
	}

	CDialog::OnOK();
}

void CSetAddIn::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CDialog::OnCancel();
}
