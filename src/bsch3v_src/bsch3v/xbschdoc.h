/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHDOC_H
#define XBSCHDOC_H


#include <list>
#include <stack>
using namespace std;

#include "SelectOption.h"
#include "ObjFilter.h"

#include "xbschobj.h"
#include "xbschjunc.h"
#include "xbschlabel.h"
#include "xbschcomment.h"
#include "xbschtag.h"
#include "xbschcomponent.h"

#include "xbschline.h"
#include "xbschdashline.h"
#include "xbschentry.h"
#include "xbschdelobj.h"
#include "xbschsheetinfo.h"
#include "xbschimageobj.h"
#include "ce3io.h"


//typedef list<string> stringList;
//typedef list<string>::iterator stringListIterator;

typedef list<SXBSchObj*> XBSchObjList;
typedef list<SXBSchObj*>::iterator XBSchObjListIterator;
typedef list<SXBSchObj*>::reverse_iterator XBSchObjListRevIterator;




typedef list<SPoint> SPointList;
typedef list<SPoint>::iterator SPointListIterator;

class SXBSchSheetInfo;
class SXBSchWire;

enum FIND_TARGET_FLAGS
{
	FINDTGT_TAG				=0x00000001,
	FINDTGT_LABEL			=0x00000002,
	FINDTGT_COMMENT			=0x00000004,
	FINDTGT_NAME			=0x00000008,
	FINDTGT_NAMELIB			=0x00000010,
	FINDTGT_NUM				=0x00000020,
	FINDTGT_MANUFACTURE		=0x00000040,
	FINDTGT_MANUFACTUREPN	=0x00000080,
	FINDTGT_PACKAGE			=0x00000100,
	FINDTGT_NOTE			=0x00000200
};




class SRedoInfo
{
public:
	enum UndoType
	{
		UndoAddMoveToStack = 0,
		UndoDeleteCopyToStack
	};

	SXBSchObj* m_pObj;
	int m_undoCount;
	UndoType m_undoType;

	SRedoInfo(){
		m_pObj = NULL;
		m_undoCount = -1;
		m_undoType = UndoAddMoveToStack;
	}
};

typedef stack<SRedoInfo> SRedoInfoStack;


class SPointEmum
{
public:
	SPointEmum();
	virtual ~SPointEmum();
	bool addSPoint(SPoint point);

	SPointList m_list;
};

class SLayer
{
public:
		static unsigned  wLayerTable[];
		//={	0x0001,0x0002,0x0004,0x0008,
		//							0x0010,0x0020,0x0040,0x0080,
		//							0x0100,0x0200,0x0400,0x0800,
		//						0x1000,0x2000,0x4000,0x8000};
};

class SXBSchDoc
{
public:
	SXBSchDoc();
	virtual ~SXBSchDoc();
	bool m_bDoTidyUp;

public:
	enum RESET_SELECT_OPTION
	{
		RESET_SELECT_NORMAL	 = 0,
		RESET_SELECT_DELETE	 = 1,
		RESET_SELECT_BRING_TO_FRONT = 2,
		RESET_SELECT_BRING_TO_BACK = 3
	};



protected:


	bool m_bDirty;			//�h�L�������g�ɑ΂��ĕύX�����������Ƃ�true�ɂȂ�B
	bool m_bTempDirty;		//�ꎞ�f�[�^�ɑ΂��ĕύX�����������Ƃ�true�ɂȂ�B
	bool m_bTempNewPlace;	//�ꎞ�f�[�^���V�K�z�u�������̂��y�[�X�g�f�[�^�̂Ƃ�true

	//int m_resetSelectOption;



	SSize m_sizeSheet;	//�V�[�g�̃T�C�Y��ۑ�����
	int m_nOpCount;				//����ԍ�
	int m_nOldestOpCount;		//�����Ƃ��Â�����ԍ�

	int m_nRedoActive;			// Redo���L���ȂƂ��AUndo���n�܂����Ƃ���opCount��ێ�����B

	SRect m_rcTempArea;			//	
	SRect m_rcRotateRect;		//��]�┽�]�Œ��S�ʒu�����߂邽�߂̃G���A
	SPoint m_ptTempSnapPoint;	//�ꎞ�I�u�W�F�N�g���X�i�b�v������ꍇ�͂��̓_���O���b�h�ɏ��悤�ɂ���
	bool m_bSelectForDrag;
	unsigned m_visibleLayer;
	int m_editLayer;
	wstring m_strLayerName[SHEET_LAYERS];

	unsigned m_selectedTypeFlag;
	void updateSelectedTypeFlag();
	enum SELECTED_TYPE_FLAGS
	{
		SELECTED_TYPE_TAG			=1,
		SELECTED_TYPE_LABEL			=2,
		SELECTED_TYPE_COMMENT		=4,
		SELECTED_TYPE_COMPONENT		=8,
		SELECTED_TYPE_MARKER		=16,
		SELECTED_TYPE_DECOLINE		=32
	};

	int m_totalPages;
	int m_page;
	wstring m_strProject;
	int m_version;
	int m_initPos;
public:
	void setTotalPages(int n){m_totalPages = n;}
	void setPage(int n){m_page = n;}
	void setProjectName(const TCHAR* psz){m_strProject = psz;}
	void setInitPos(int n){m_initPos = n;}
	
	int totalPages(){return m_totalPages;}
	int page(){return m_page;}
	const TCHAR* projectName(){return m_strProject.c_str();}
	int dataVersion(){return m_version;}
	int initPos(){return m_initPos;}

	bool createdNewerVersion(){return (m_version>SXBSchSheetInfo::m_nConstDataVersion);}



protected:
	//�I���̉��� bDel �� true �̂Ƃ��́A�폜���s��
	//public �� resetSelect() �� del() ����Ă΂��B
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	//bool resetSelect_1(bool bDel);
	bool resetSelect_1(int option);

	//�ꎞ�f�[�^�����C���f�[�^�Ɉڂ��āA�ꎞ�f�[�^���N���A����
	void moveTempDataToMain(bool newPlace,int option);

	void pickupJunction(const SRect& rc);//���ׂĂ̐ڍ��_�� m_listWorkJunc �ɒ��o����B
	void selectForDragJunction(int scanMax,const SRect& rc);	//�͈͓��̐ڍ��_�̑I���B���ׂĂ̐ڍ��_��m_listWorkJunc �ɒ��o����B
	int divideWire(SXBSchWire* swire,const SRect& rc,SXBSchWire** dwire);
	void selectForDragWire(int scanMax,const SRect& rc);		//�h���b�O�̂��߂̃��C���[�I���B
	void selectForDragExceptJuncWire(int scanMax,const SRect& rc);//�h���b�O�̂��߂̐ڍ��_���C���[�ȊO�̑I���B
	void selectForDragPinConnection(int scanMax,const SRect& rc,bool bCutOffPinWire);//�h���b�O�̂��߂̃s�������I��
	bool selectForDragPin(int scanMax,const SPoint pt);//�w�肵���_�Ƀs��������������[�������C���[��z�u����

	void selectForDragReCheckSelectedLine(const SRect& rc);//�����̑I����ɁA�V���Ȕ͈͂�ǉ��I�����ꂽ�^�O�A���i�̃s���ɐڑ����ꂽ���I��[�_���Ȃ����`�F�b�N

	

public:  //�X�g���[��I/O�֘A
	bool writeMinimumLb3(SWriteCE3& wce3,XBSchObjList* pList,bool bOwnOnly);
	bool writeCe3(SWriteCE3& wce3,XBSchObjList* pList,bool bAllObj,const SPoint* pptOrigin);
	bool readCe3(	SReadCE3& rce3,
					XBSchObjList* pList,
					const SPoint* pptOrigin,
					SXBSchSheetInfo* pSheetInfo);

public:
	XBSchObjList m_listMain;
	XBSchObjList m_listTemp;
protected:
	XBSchObjList m_listWorkJunc;			//�ꎞ��Ɨp�̃��[�N�G���A �h���b�O�I���Ŏg�p
	XBSchObjList m_listWorkWire;			//�ꎞ��Ɨp�̃��[�N�G���A �h���b�O�I���Ŏg�p
	XBSchObjList m_listWorkTagComponent;	//�ꎞ��Ɨp�̃��[�N�G���A �h���b�O�I���Ŏg�p

protected:
	SRedoInfoStack m_stackRedoInfo;

//----- find -----
protected:
	virtual bool findStringCompare(const TCHAR* szTarget,const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive);
public:
	const SXBSchObj*  findStart(const TCHAR* sz,bool bMatchWhole,bool bCaseSeinsitive,unsigned int targetFlags,bool selectWhenFound);
	//const SXBSchObj*  find(bool bStart=false);
	const SXBSchObj*  find(bool bStart);
	//�u���̌����v���\��?
	bool canFindNext(){return m_bFindCommandActive;}

	//�����Ō��������I�u�W�F�N�g�̏��
	wstring m_foundString;	//������
	unsigned int m_foundType;
	SPoint m_foundPoint;

protected:
	bool m_bFindCommandActive;
	wstring	m_strFind;//�����Ώە�����
	bool	m_bFindMatchWhole;
	bool	m_bCaseSeinsitive;
	unsigned int m_nTargetFlags;
	bool m_selectWhenFound;
	XBSchObjListRevIterator m_reviteFind;
	int m_nFindStep;
//----- find -----


public:
	void setDirty(bool flag = true){m_bDirty = flag;}
	void setTempDirty(bool flag = true){m_bTempDirty = flag;}

	bool isDirty(){return (m_bDirty || m_bTempDirty || m_bTempNewPlace);}	//�Ō�̕ۑ��̂��ƂɕύX�����������ǂ�����Ԃ�
	bool isEmpty();	//�}�ʃI�u�W�F�N�g����ł������false��Ԃ�
	void SetSheetSize(const SSize& size);
	SSize SheetSize(){return m_sizeSheet;}

public:
	enum ISTEMPDATA_OPTION{
		EDITPOINTS_MODE = 1
	};

public:
//20060603	//�}�ʗv�f�̒ǉ� ����ԍ���Ԃ�
//20060603	int add(SXBSchObj* pObj);
	
	//�}�ʗv�f�����C�����X�g��push_back����B
	void pushBackToMainList(SXBSchObj* pObj,bool doTidyUp);

	//�W�����N�V�����̏d���`�F�b�N���s��
	//bool checkRedundancyJunction(SXBSchJunc* pObj);

	//�W�����N�V�����A�G���g���[�̏d���`�F�b�N���s��
	bool checkRedundancy(SXBSchObj* pObj);



	//���C���[,�o�X�̌�������
	void tidyUpWireBus(SXBSchLine* pObj);

	//���C���[,�W�����N�V�����̓������� �X�e�b�v1
	void tidyUpWireJunction_1(int opCount);
	//���C���[,�W�����N�V�����̓������� �X�e�b�v2
	void tidyUpWireJunction_2(int opCount);
	//���C���[,�W�����N�V�����̓������� �X�e�b�v3
	void tidyUpWireJunction_3(int opCount);


	//�I����ԂŒǉ��}�ʗv�f�̒ǉ� ����ԍ���Ԃ�
	int addSelect(SXBSchObj* pObj);

	//�_���w�肵�Ă̑I��
	int select(const SPoint& pt,SelectOption& refSelOption);
protected:
	bool	m_bValidContinuousSingleSelect;
	XBSchObjListIterator m_singleSelIte;
	SPoint	m_ptBiginSingleSelectAt;


public:
	//�͈͂��w�肵�Ă̑I��
	int select(const SRect& rc,SelectOption& refSelOption);

	int selectForDrag(const SRect& rc,bool bCutOffPinWire);
	bool isSelectedForDtag(){return this->m_bSelectForDrag;}


	void setNcPinFlag();


	//m_rcTempArea �� m_ptTempSnapPoint ���ꎞ�I�u�W�F�N�g�̃��X�g����ݒ肷��B
	void setupRcTempArea();
	void setupRotateRect();

	
	
	//�I���̉���
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	bool resetSelect(int option);
	//�ꎞ�I�u�W�F�N�g�͈̔͂��擾����
	SRect tempArea(){return m_rcTempArea;}
	//�ꎞ�I�u�W�F�N�g�̃X�i�b�v�|�C���g���擾����
	SPoint tempSnapPoint(){return m_ptTempSnapPoint;}
	//������
	void init();

	enum CE3_FILE_CHECK_RESULT{
		CE3FCHK_ERROR		=0,
		CE3FCHK_V070_OR_E	=1,
		CE3FCHK_V080_OR_L	=2,
		CE3FCHK_EMPTY		=3
	};
	

	//�o�[�W��������
	int ce3FileCheck(const TCHAR* fname);

	//�t�@�C������̓ǂݍ���
	int readFile(const TCHAR* fname);
	//int readFile(FILE* fp);
	//�t�@�C���ւ̏�������
	int writeFile(const TCHAR* fname);
	int writeFile(FILE* fp);




	//***** �ҏW *****

	//�R�s�[
	//�R�s�[���s��ꂽ�Ƃ���true��Ԃ�
	virtual bool copy(){return false;}
	//�\��t��
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	virtual bool paste(const SPoint&){return false;}
	//�\��t���\���H
	virtual bool canPaste(){return false;}

	//�폜
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	bool del();

	//�؂���
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	bool cut();


	//Undo
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	bool undo();

	bool redo();
protected:
	void discardRedo();

	bool isOnGrid(SPoint pt);

public:
	//�R�s�[�\���H
	bool canCopy();
	//�폜�\���H
	bool canDel();
	//�؂���\���H
	bool canCut();
	//Undo�\���H
	bool canUndo();
	//Redo�\���H
	bool canRedo();




	//���i��1�����P�ƂőI������Ă��邩
	const SXBSchComponent* isComponentSelectedSingly();

	//�J�[�\�������i�̏�ɂ��邩�ǂ���
	const SXBSchComponent* isOnComponent(const SPoint& pt);

	//�p�[�c���I������Ă��邩
	bool isComponentSelected();
	//���������I������Ă��邩
	bool isDecorationlineSelected();
	//�}�[�J�[���C�����I������Ă��邩
	bool isMarkerlineSelected();
	//�^�O���I������Ă��邩
	bool isTagSelected();
	//���x�����I������Ă��邩
	bool isLabelSelected();
	//�R�����g���I������Ă��邩
	bool isCommentSelected();




	//Attribute�ύX���\��?
	bool canAttribute();

	//Rotate
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	bool rotate();
	//Mirror
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	bool mirror();

	//Rotate�\���H
	bool canRotate();
	//Mirror�\���H
	bool canMirror();

	//EditPoints���\��
	bool canEditPoints();

	//�����̕ύX���\���H
	bool canChangeOrder();


	//�o���N�A�g���r���[�g ���i���̒u��
	void bulkAttributeReplaceComponentInfo(const SCompIndex* pPartIndex);

	//�o���N�A�g���r���[�g ���i���̔�\���ݒ�
	void bulkAttributeHideComponentVal(bool hide);

	//�o���N�A�g���r���[�g ���i�ԍ��̔�\���ݒ�
	void bulkAttributeHideComponentNum(bool hide);


	//�o���N�A�g���r���[�g �R�����g�t�H���g�̕ύX
	void bulkAttributeCommentFont(SXBSchComment& refComment);

	//�o���N�A�g���r���[�g �������̕ύX
	void bulkAttributeDecorationLine(SXBSchDash& refDecoline);

	//�o���N�A�g���r���[�g �}�[�J�[���C���̕ύX
	void bulkAttributeMarkerLine(int width,unsigned nColor);

	//�o���N�A�g���r���[�g �^�O�g�̕ύX
	void bulkAttributeChangeTagFrameTo(int n);

	//���x������^�O�𐶐�
	SXBSchTag* generateTagFromLabel(SXBSchLabel* pObj);
	//���x�����^�O�ɕϊ�
	void bulkConvertLabel2Tag();

	//���x������R�����g�𐶐�
	SXBSchComment* generateCommentFromLabel(SXBSchLabel* pObj);
	//���x�����R�����g�ɕϊ�
	void bulkConvertLabel2Comment();



	//�^�O���烉�x���𐶐�
	SXBSchLabel* generateLabelFromTag(SXBSchTag* pObj);
	//�^�O�����x���ɕϊ�
	void bulkConvertTag2Label();

	//�^�O����R�����g�𐶐�
	SXBSchComment* generateCommentFromTag(SXBSchTag* pObj);
	//�^�O���R�����g�ɕϊ�
	void bulkConvertTag2Comment();



	//�R�����g���烉�x���𐶐�
	//SXBSchLabel* generateLabelFromComment(SXBSchComment* pObj);
	//�R�����g�����x���ɕϊ�
	void bulkConvertComment2Label();

	//�R�����g����^�O�𐶐�
	//SXBSchTag* generateTagFromComment(SXBSchComment* pObj);
	//�R�����g���^�O�ɕϊ�
	void bulkConvertComment2Tag();





	//���C���[�̈ړ�
	bool moveToLayer(int nLayer);
	unsigned int visibleLayer(){return m_visibleLayer;}
	void setVisibleLayer(unsigned int n){m_visibleLayer=n&0xff;}
	int editLayer(){return m_editLayer;}
	void setEditLayer(int n){if(n>=0&&n<8)m_editLayer=n;}
	const TCHAR* layerName(int i) { return m_strLayerName[i].c_str(); }
	void setLayerName(int i, const TCHAR* lpcsz) { m_strLayerName[i] = lpcsz; setDirty(true); }



	//����ԍ���Ԃ�
	int opCount(){return m_nOpCount;}
	//�Â��f�[�^�̍폜
	void delOldData(int nOpCount);
	//�w�肵�����W���ꎞ�f�[�^�̏ォ�ǂ���
	int isOnTempData(const SPoint& pt,int nOption,SXBSchObj** ppObj,int* pOption);
	//�ꎞ�f�[�^���ړ�����
	void moveTempData(int offsetx,int offsety);
};

#endif  //#ifndef XBSCHDOC_H