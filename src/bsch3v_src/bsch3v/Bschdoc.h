/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSchDoc.h : CBSchDoc �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BSCHDOC_H
#define BSCHDOC_H

#include "xbschdoc.h"
#include "xbschdocsch.h"

#define	MAX_MAINDATA	1	//���C���f�[�^�̐�


//m_fTmpData�̒l
//TMPDATA_SQAREA �� TMPDATA_SINGLE	�͔r���I
//TMPDATA_MOVED  �� TMPDATA_NEW		�͔r���I	
#define	TMPDATA_SQAREA	1	//�ꎞ�f�[�^�͋�`�͈͂�����
#define	TMPDATA_SINGLE	2	//�ꎞ�f�[�^�͒P��}�ʃI�u�W�F�N�g
#define	TMPDATA_MOVED	4	//�ꎞ�f�[�^��m_listMainData����ړ���������
#define	TMPDATA_NEW		8	//�ꎞ�f�[�^�͐V�K�z�u�f�[�^
#define TMPDATA_DIRTY	16	//�ꎞ�f�[�^��ݒ��A�ړ����]�Ȃǂ̑�����s����
#define TMPDATA_FOR_DRAG 32 //�ꎞ�f�[�^�̓h���b�O�̂��߂̂���
#define TMPDATA_NO_FRAME 64 //�ꎞ�f�[�^�̕`��̍ۂɓ_���g��\�����Ȃ�
			
//IsOnTmpData()�̕Ԓl
//#define ONTMPDATA		1	//�w�肵���_�͈ꎞ�f�[�^�̏�
//#define ONTMPDATA_NAME	2	//�w�肵���_�͕��i���̏�
//#define ONTMPDATA_NUM	4	//�w�肵���_�͕��i�ԍ��̏�



#include "xbschdocsch.h"

class CBSchDoc : public CDocument
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CBSchDoc();
	DECLARE_DYNCREATE(CBSchDoc)

//////////////////////////////////////////////////////////////////
// �v���C�x�[�g�ϐ�
private:

	SXBSchDocSch xbschdocsch;
	static SIZE m_tablesizeSheet[MAX_SHEETSIZE];

public:
	//���C���[�Ɋւ�����
	//WORD		m_wShowLayer;
	//int			m_nEditLayer;
	//BOOL		m_fEditHighLight;

	unsigned int visibleLayer(){return xbschdocsch.visibleLayer();}
	void setVisibleLayer(unsigned int n){xbschdocsch.setVisibleLayer(n);}
	int editLayer(){return xbschdocsch.editLayer();}
	void setEditLayer(int n){xbschdocsch.setEditLayer(n);}
	void setNcPinFlag(){xbschdocsch.setNcPinFlag();}
	const TCHAR* layerName(int i) { return xbschdocsch.layerName(i); }
	void setLayerName(int i, const TCHAR* lpcsz) { xbschdocsch.setLayerName(i, lpcsz); }


public:
	void setTotalPages(int n){xbschdocsch.setTotalPages(n);}
	void setPage(int n){xbschdocsch.setPage(n);}
	void setProjectName(const TCHAR* psz){xbschdocsch.setProjectName(psz);}
	void setInitPos(int n){xbschdocsch.setInitPos(n);}
	
	int totalPages(){return xbschdocsch.totalPages();}
	int page(){return xbschdocsch.page();}
	const TCHAR* projectName(){return xbschdocsch.projectName();}
	int dataVersion(){return xbschdocsch.dataVersion();}
	int initPos(){return xbschdocsch.initPos();}




private:


// �I�y���[�V����
public:
//	void	IncrementDataDepth();	//�L����m_listMainData�̐��̃C���N�������g
//									//�ő吔�ɂȂ�����Â��f�[�^�̓����������ōs��
	void	FixSelection(int option);			//�I���̉���
	int		SetSel(CRect* prc,SelectOption& refSelOption);		//��`�͈͂ł̑I��
	int		SetSel(CPoint* pp,SelectOption& refSelOption);		//�_�w��ł̑I��
	int		SetDrag(CRect* prc,bool bCutOffPinWire);	//��`�͈͂ł̃h���b�O���[�h�I��
	int		SetDrag(CPoint* pp,bool bCutOffPinWire);	//�_�w��ł̃h���b�O���[�h�I��
	bool	isSelectedForDtag(){return xbschdocsch.isSelectedForDtag();}
	int		GetTmpDataCount(){return xbschdocsch.m_listTemp.size();} //�ꎞ�f�[�^�̐���Ԃ��B

//	UINT	GetTmpDataFlag(){return m_fTmpData;}//���݂̈ꎞ�f�[�^�̎��ʃt���O��Ԃ�
	XBSchObjList* GetTmpList();			//�ꎞ�f�[�^�̃��X�g�𓾂�
	SXBSchObj* GetTmp();				//�ꎞ�f�[�^���P��}�ʃI�u�W�F�N�g�̂Ƃ��A�����Ԃ�
	CPoint  GetTmpOrigin();				//�ꎞ�f�[�^�̌��_��Ԃ�
	CRect	GetTmpRect();				//�ꎞ�f�[�^�̐�L�͈͂�Ԃ�
	//void 	SetTmpRect(CRect* prc){m_rcTmpData=*prc;}	//�ꎞ�f�[�^�̐�L�͈͊O������ݒ肷��
	int		IsOnTmpData(CPoint* pp,int nOption,SXBSchObj** ppObj,int* pRetOption);//�w�肵���_���ꎞ�f�[�^�̏ォ�ǂ���

	BOOL	IsEmpty();				//�f�[�^���󂩂ǂ�����Ԃ�
	void	Paste(CPoint* pOrigin); //�y�[�X�g
	void	Cut();					//�J�b�g
	void	Copy();					//�R�s�[
	void	Clear();				//�N���A
	void	Undo();					//�A���h�D
	bool	Redo();					//�A���h�D
	void	Rotate();				//��]����
	void	Mirror();				//���E���]����
	void	Shift(CPoint* pOrigin);	//�ړ�����
	void	Shift(CSize*  psizeOffset);	//�ړ�����
//	BOOL	DoAttributeDialog();	//�A�g���r���[�g�ύX�_�C�A���O�̋N��
	void	SetDirty();				//�_�[�e�B�t���O���Z�b�g����
	void	SetTempDirty();				//�ꎞ�f�[�^�̃_�[�e�B�t���O���Z�b�g����
	BOOL	CanEdit();				//�I�����ꂽ���̂����݂��邩�ǂ���
	BOOL	CanPaste();				//�y�[�X�g�\���ǂ���
	BOOL	CanUndo();				//�A���h�D�\���ǂ���
	BOOL	CanRedo();				//�A���h�D�\���ǂ���
	BOOL	CanAttribute();			//�A�g���r���[�g�̕ύX���\���ǂ���
	BOOL	CanRotate();			//��]���\���ǂ���
	BOOL	CanMirror();			//���E���]���\���ǂ���
	BOOL	CanEditPoints();		//�_�̕ҏW���\��
	BOOL	CanChangeOrder();		//�����̕ύX���\���H

	//���i��1�����P�ƂőI������Ă��邩
	const SXBSchComponent* isComponentSelectedSingly();

	//�J�[�\�������i�̏�ɂ��邩�ǂ���
	const SXBSchComponent* isOnComponent(CPoint *pp);


	//�p�[�c���I������Ă��邩
	bool isComponentSelected(){return xbschdocsch.isComponentSelected();}
	//���������I������Ă��邩
	bool isDecorationlineSelected(){return xbschdocsch.isDecorationlineSelected();}
	//�}�[�J�[���C�����I������Ă��邩
	bool isMarkerlineSelected(){return xbschdocsch.isMarkerlineSelected();}
	//�^�O���I������Ă��邩
	bool isTagSelected(){return xbschdocsch.isTagSelected();}
	//���x�����I������Ă��邩
	bool isLabelSelected(){return xbschdocsch.isLabelSelected();}
	//�R�����g���I������Ă��邩
	bool isCommentSelected(){return xbschdocsch.isCommentSelected();}




	//�����܂��͂��͂����炯�B�������̋@�\�Q�𗬗p���������B
	BOOL	CanFindNext();			//�u���̌����v���\���ǂ���
	const SXBSchObj* FindStart(const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive,unsigned int targetFlags,bool selectWhenFound);
	const SXBSchObj* Find();
	void GetFoundText(CString& str);
	unsigned int GetFountType(){ return xbschdocsch.m_foundType;}
	SPoint GetFoundPoint(){ return xbschdocsch.m_foundPoint;}



	void setTidyUpEnable(bool en){xbschdocsch.m_bDoTidyUp = en;}
	bool tidyUp(){return xbschdocsch.m_bDoTidyUp;}

	//�o���N�A�g���r���[�g ���i���̒u��
	void bulkAttributeReplaceComponentInfo(const SCompIndex* pPartIndex){xbschdocsch.bulkAttributeReplaceComponentInfo(pPartIndex);}

	//�o���N�A�g���r���[�g ���i���̔�\���ݒ�
	void bulkAttributeHideComponentVal(bool hide){xbschdocsch.bulkAttributeHideComponentVal(hide);}

	//�o���N�A�g���r���[�g ���i�ԍ��̔�\���ݒ�
	void bulkAttributeHideComponentNum(bool hide){xbschdocsch.bulkAttributeHideComponentNum(hide);}

	//�o���N�A�g���r���[�g �R�����g�t�H���g�̕ύX
	void bulkAttributeCommentFont(SXBSchComment& refComment){xbschdocsch.bulkAttributeCommentFont(refComment);}

	//�o���N�A�g���r���[�g �}�[�J�[���C���̕ύX
	void bulkAttributeMarkerLine(int width,unsigned nColor){xbschdocsch.bulkAttributeMarkerLine(width,nColor);}

	//�o���N�A�g���r���[�g �������̕ύX
	void bulkAttributeDecorationline(SXBSchDash& refDecoline){xbschdocsch.bulkAttributeDecorationLine(refDecoline);}


	//�o���N�A�g���r���[�g �^�O�g�̕ύX
	void bulkAttributeChangeTagFrameTo(int n){xbschdocsch.bulkAttributeChangeTagFrameTo(n);}

	//�R�����g�����x���ɕϊ�
	void bulkConvertComment2Label(){xbschdocsch.bulkConvertComment2Label();}

	//�R�����g�����x���ɕϊ�
	void bulkConvertComment2Tag(){xbschdocsch.bulkConvertComment2Tag();}

	//���x�����^�O�ɕϊ�
	void bulkConvertLabel2Tag(){xbschdocsch.bulkConvertLabel2Tag();}

	//���x�����R�����g�ɕϊ�
	void bulkConvertLabel2Comment(){xbschdocsch.bulkConvertLabel2Comment();}

	//�^�O�����x���ɕϊ�
	void bulkConvertTag2Label(){xbschdocsch.bulkConvertTag2Label();}

	//�^�O���R�����g�ɕϊ�
	void bulkConvertTag2Comment(){xbschdocsch.bulkConvertTag2Comment();}







	int		opCount(){return xbschdocsch.opCount();}

	BOOL	AddData(SXBSchObj* pObj);//�}�ʃI�u�W�F�N�g�̉��ǉ�
	XBSchObjList* GetMainData(int n=0);	//�擪����n�ڂ̃f�[�^�̃��X�g�ւ̃|�C���^�𓾂�
	

	void	SetSheetSize(int n,int w,int h);	//�}�ʃT�C�Y��ԍ��ŃZ�b�g����
	CSize	GetSheetSize();//�}�ʃT�C�Y��Ԃ�
	int		GetSheetSizeIndex();//�}�ʃT�C�Y��Ԃ�

	//���W�X�g������}�ʃT�C�Y�𓾂�
	void  IniReadSheetSize(CSize* psize);
	//int		IniReadSheetSize();		//���W�X�g������}�ʃT�C�Y�𓾂�
	//void	IniWriteSheetSize(int nSize);//���W�X�g���ɐ}�ʃT�C�Y����������
	//���W�X�g���ɐ}�ʃT�C�Y����������
	void IniWriteSheetSize(CSize* pSize);

	void	MoveLayer(int nLayer);	//���C���[�̈ړ�
	void	setupRcTempArea(){xbschdocsch.setupRcTempArea();}

	BOOL SaveFile(){return DoFileSave();}	//2005/10/16
	BOOL ReloadFile();

//	void	setPointTmpData(CPoint& point){m_pointTmpData=point;}

//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CBSchDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CBSchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
public:
	BOOL IsModified();

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CBSchDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
};

/////////////////////////////////////////////////////////////////////////////

#endif
