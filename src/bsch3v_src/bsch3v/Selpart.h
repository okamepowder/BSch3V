/****************************************************************************
     BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SelPart.h : �w�b�_�[ �t�@�C��
//
// ���i��I�����邽�߂̃_�C�A���O�{�b�N�X�̃N���X
//   ���̃_�C�A���O�͕��i����Ԃ��B�v���O�����iBSch�j�́A����
// ���i������A���i�C���f�b�N�X�ւ̃|�C���^�𓾂Ȃ���΂Ȃ�
// �Ȃ��B

/////////////////////////////////////////////////////////////////////////////
// CSelPart �_�C�A���O

class CSelPart : public CDialog
{
	int m_nPartCount;
	int m_nLibraryCount;
	CRect m_rcDialog;
	CRect m_rcClient;

	CPreview* m_pwndPreview;

	BOOL m_bInhibitUpdatePartName;
	BOOL m_bInhibitUpdateListbox;

// �R���X�g���N�V����
public:
	CString m_rStrPartName;	//�_�C�A���O�I����A���i��������
	int m_nLastSelectPart;
	int m_nLastSelectLibrary;
	const SCompIndex* m_pCompIndex;
	
	BOOL m_bUsePreview;

	CSelPart(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	~CSelPart()
	{
		delete m_pwndPreview;
	}

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSelPart)
	enum { IDD = IDD_SELECT_PARTS };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CSelPart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSelPart)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListLibrary();
	afx_msg void OnSelchangeListPartname();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkListPartname();
	afx_msg void OnChangeEditPartname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	void SetPartName(int nLib);	//�w�肵�����C�u�����̕��i�����X�g�{�b�N�X�ɐݒ肷��
	void SetLibraryName();		//���C�u�����̖��O�����X�g�{�b�N�X�ɐݒ肷��
	void SetEditPartName(LPCTSTR str);//�G�f�B�b�g�R���g���[���Ƀp�[�c����ݒ�
	void AutoUpdate();
};
