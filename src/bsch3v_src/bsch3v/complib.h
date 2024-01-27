/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef COMPLIB_H
#define COMPLIB_H

#include "coord.h"

#include "SPtnObj.h"
#include "SPtnObjArc.h"
#include "SPtnObjCircle.h"
#include "SPtnObjLine.h"
#include "SPtnObjPolygon.h"
#include "SPtnObjText.h"


class SReadCE3;
class SWriteCE3;

#ifdef _BSCH
	extern bool g_notSaveBitPtn;
#endif

#define PIN_LENGTH 10


////////////////////////////////////////////////////////////////////
//���i���C�u�����Ɋ֌W�����`


enum PIN_TYPE
{
	PIN_TYPE_NOMAL		=0x00,
	PIN_TYPE_SMALL		=0x01,
	PIN_TYPE_ZLENG		=0x02,
	PIN_TYPE_CLOCK		=0x04,
	PIN_TYPE_NEGATIVE	=0x08,
	PIN_TYPE_NUMHIDE	=0x10
};

enum PIN_LOC
{
	PIN_LOC_L			=0x0,
	PIN_LOC_T			=0x1,
	PIN_LOC_R			=0x2,
	PIN_LOC_B			=0x3
};
/*
enum VECTPTN
{
	VECTPTN_L			=1,
	VECTPTN_C			=2,
	VECTPTN_PG			=3,
	VECTPTN_AR			=4
};
*/



typedef SPtnObj* PSPtnObj;
typedef list<SPtnObj*> SPtnObjList;
typedef list<SPtnObj*>::iterator SPtnObjListIterator;

//LB3���C�u�����̓ǂݍ��݂Ŏg���p�^�[���N���X
class SPtn
{
protected:
	unsigned char* readBmp(SReadCE3& rce3);//,int x,int y,int nBuffSize);
	//bool readLine(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readCircle(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readCircleFill(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readArc(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readPolygon(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);

	//bool writeLine(SWriteCE3& wce3,int* buff,int* count);
	//bool writeCircle(SWriteCE3& wce3,int* buff,int* count);
	//bool writeCircleFill(SWriteCE3& wce3,int* buff,int* count);
	//bool writeArc(SWriteCE3& wce3,int* buff,int* count);
	//bool writePolygon(SWriteCE3& wce3,int* buff,int* count);


public:
	TCHAR*			m_pszName;		//�p�^�[����
	SSize			m_size;			//�T�C�Y
	int				m_nBuffsize;	//�o�b�t�@�̃T�C�Y
	PSPtnObj*		m_ppvPtn;		//�x�N�g���p�^�[���I�u�W�F�N�g�̃|�C���^�z��
	int				m_vPtnCount;
	unsigned char*	m_pBuff;		//�o�b�t�@�ւ̃|�C���^
//	int*			m_pvBuff;		//�x�N�g���`���ւ̃|�C���^
//	int				m_nvBuffsize;	//�x�N�g���`���̃o�b�t�@�̃T�C�Y

	SPtn(){							//�R���X�g���N�^
		m_pszName=NULL;
		m_nBuffsize=0;
		m_pBuff=NULL;
//		m_nvBuffsize=0;
//		m_pvBuff=NULL;
		m_size=SSize(0,0);
		m_vPtnCount=0;
		m_ppvPtn=NULL;
	}

	SPtn(const SPtn&);				//�R�s�[�R���X�g���N�^
	SPtn& operator=(const SPtn&);	//������Z�q	

	virtual ~SPtn();				//�f�X�g���N�^
	bool readCe3(SReadCE3& rce3);
	bool writeCe3(SWriteCE3& wce3,LPCTSTR pszName);
	void setSize(const SSize& sizePixel);
};



////////////////////////////////////////////////////////////////////
//�s���N���X�̒�`
class SPin
{
protected:
	int			m_nBlock;	//m_nBlock�̐������s���ԍ�������z����Ǘ����Ă���
	TCHAR**		m_ppszNum;	//�s���ԍ�������z��ւ̃|�C���^
	TCHAR*		m_pszName;	//�s�����ւ̃|�C���^
	int			m_nType;	//�s���̃^�C�v
	int			m_nLTRB;	//�ʒu LTRB
	int			m_nOffset;	//�ʒu �I�t�Z�b�g
	unsigned long m_pinNameDisplayFlag; //�s�����ʕ\���t���O
	unsigned	m_flags;	//�t�@�C���ɕۑ�����Ȃ����쎞�̃t���O

	enum{
		FLAGS_NC = 0x0001
	};

	//bool m_pinNameDrawCache;
	TCHAR*	m_pszPinNameDraw;



protected:
	void deleteBuff();

public:
	SPin();					//�R���X�g���N�^
	~SPin();
	SPin(const SPin&);				//�R�s�[�R���X�g���N�^
	SPin& operator=(const SPin&);	//������Z�q

	void SetPinNameDisplayFlag(unsigned long val){m_pinNameDisplayFlag = val;}
	unsigned long GetPinNameDisplayFlag(void ){return m_pinNameDisplayFlag;}

	
	void setBlock(int  nBlock);	//�u���b�N�����Z�b�g����B
	void setPinName(const TCHAR* name);				//�s�������Z�b�g����B
	void setPinNum(int nBlock,const TCHAR* name);	//�s���ԍ����Z�b�g����B
	void setType(int type){ m_nType=type;}
	void setLtrb(int ltrb){ m_nLTRB=ltrb;}
	void setOffset(int offset){m_nOffset=offset;}
	const TCHAR* pinName() const {return m_pszName;}
	const TCHAR* pinNameDraw() const {return m_pszPinNameDraw;}
	const TCHAR* pinNum(int nBlock) const;
	int type() const {return m_nType;}
	int ltrb() const {return m_nLTRB;}
	int offset() const {return m_nOffset;}
	int block() const { return m_nBlock;}

	bool nc() const { return m_flags &  FLAGS_NC;}
	void set_nc(bool itisNC);

	void setupPinNameDraw();

	bool readCe3(SReadCE3& rce3,int block);
	bool writeCe3(SWriteCE3& rce3);
};

typedef SPin* PSPin;

typedef list<SPin*> SPinList;
typedef list<SPin*>::iterator SPinListIterator;


////////////////////////////////////////////////////////////////////
//���i���N���X�̒�`
class SCompInfo
{
protected:
	void deleteBuff();

public:
	enum CITYPE{
		GENERAL			= 0,
		SHARED_BLOCK	= 1,	//��ɕ����u���b�N�̕��i�ƈꏏ�Ɏg���鋤�ʕ����A�I�y�A���v�A74���W�b�N�̓d���A�����[�̃R�C���A�X�C�b�`�̏ƌ�
		FIXED_BLOCK		= 2		//
	};

public:
	TCHAR*	m_pszRef;	//�f�t�H���g���i�ԍ�
	SSize	m_size;		//���i�̃T�C�Y
	int		m_nBlock;	//�p�b�P�[�W������̃u���b�N��
	int		m_nFixedBlock; //�Œ�u���b�N
	CITYPE	m_ciType;	
	SPtn*	m_pPtn;		//LB3����ǂݍ��񂾃p�^�[���ւ̃|�C���^
	int		m_nPinCount;//�s����
	PSPin*	m_ppPin;	//LB3����ǂݍ��񂾃s���N���X�̔z��ւ̃|�C���^
	wstring	m_note;		//�m�[�g 2005/04/30
	wstring	m_mfr;		//�����ҋL���� 2009/02/01
	wstring	m_mfrpn;	//�����Ҍ^���L���� 2009/02/01
	wstring	m_pkg;		//�p�b�P�[�W�L���� 2009/02/01

	SCompInfo(){		//�R���X�g���N�^
		m_pszRef	=NULL;
		m_nBlock	=1;
		m_pPtn		=NULL;
		m_nPinCount	=0;
		m_ppPin		=NULL;
		m_note		=_T("");
		m_mfr		=_T("");
		m_mfrpn		=_T("");
		m_pkg		=_T("");
		m_nFixedBlock = 0;
		m_ciType	= CITYPE::GENERAL;
	}
	~SCompInfo();			//�f�X�g���N�^

	SCompInfo(const SCompInfo&);			//�R�s�[�R���X�g���N�^
	SCompInfo& operator=(const SCompInfo&);	//������Z�q

	bool readCe3(SReadCE3& rce3,wstring& name,wstring& ptnname);	//�t�@�C������̓ǂݍ���
	bool writeCe3(SWriteCE3& wce3,const TCHAR* pszName);

	void setCiType(CITYPE citype);
	void setRef(const TCHAR* ref);		//�f�t�H���g���i�ԍ��̐ݒ�
	void setSize(const SSize& size,bool createPtn);	//�T�C�Y�̐ݒ� �p�^�[����L����ꍇ�̓p�^�[���̃T�C�Y���ύX
	void setUsePattern(bool usepattern);
	void setBlock(int block);
	void setFixedBlock(int fb);
	void addPin(SPin* ppin);
	void removePin(int ltrb,int offset,bool move);
	void insertNullPin(int ltrb,int offset);
	SPin* pinInfo(int ltrb,int offset);	//�w�肵���ʒu�̃s�����𓾂�
};

////////////////////////////////////////////////////////////////////
//���i�C���f�b�N�X�N���X�̒�`
class SCompIndex
{
	TCHAR*				m_lpszName;	//���i��
public:
	SCompInfo*			m_pCompInfo;//�Ή����镔�i���N���X�ւ̃|�C���^

	SCompIndex(){			//�R���X�g���N�^
		m_lpszName	=NULL;
		m_pCompInfo =NULL;
	}
	~SCompIndex(){			//�f�X�g���N�^
		delete[]m_lpszName;
	}
	void setName(const TCHAR* name);
	void setCompInfo(SCompInfo* pcinfo){m_pCompInfo = pcinfo;}

	SCompInfo* compInfo() const{return m_pCompInfo;}
	const TCHAR*	name() const{return m_lpszName;}			//���i���𓾂�
	const TCHAR* number() const;			//���i�ԍ��𓾂�

	const TCHAR* Mfr() const;			//�����҂𓾂�		20090203
	const TCHAR* MfrPn() const;			//�����Ҍ^���𓾂�		20090203
	const TCHAR* Pkg() const;			//�p�b�P�[�W�𓾂�		20090203

	SSize size() const;					//�T�C�Y�𓾂�
	SSize sizePlace(int dir) const;		//�z�u���̐}�ʏ�̃T�C�Y�𓾂�
	int	  block() const;				//�p�b�P�[�W������̃u���b�N���𓾂�
	SPtn*	pattern() const;			//�p�^�[���ւ̃|�C���^�𓾂�
	int	pinCount() const;				//�s���̐��𓾂�
	SPin* pin(int index) const;			//�s���̏��𓾂�
	//�s���̏��𓾂�
	//�����̏�񂩂�A�s���̈ʒu���(LTRB�ƁA���_����̃I�t�Z�b�g)��Ԃ�
	SPin* pin(int index,int dir,int& nLtrb,SPoint& ptLoc) const;

};



typedef SCompIndex* PSCompIndex;

typedef list<SPtn*> SPtnList;
typedef list<SPtn*>::iterator SPtnListIterator;

typedef list<SCompInfo*> SCompInfoList;
typedef list<SCompInfo*>::iterator SCompInfoListIterator;

typedef list<SCompIndex*> SCompIndexList;
typedef list<SCompIndex*>::iterator SCompIndexListIterator;


////////////////////////////////////////////////////////////////////
//���i���C�u�����N���X
class SCompLib
{
public:
	LPTSTR			m_lpszLibraryName;		//���C�u������
	wstring			m_prop_string;
	SPtnList		m_listPtn;				//�ǂݍ��񂾃p�^�[���̃��X�g
	SCompInfoList	m_listCompInfo;			//�ǂݍ��ݍ��񂾕��i�N���X�̃��X�g
	SCompIndexList	m_listCompIndex;		//�ǂݍ��ݍ��񂾕��i�C���f�b�N�X�̃��X�g
	PSCompIndex*	m_ppCompIndex;			//�ǂݍ��ݍ��񂾕��i�C���f�b�N�X�̃|�C���^�z��
	time_t			m_aux1;
	time_t			m_aux2;

	int				m_dataVersion;			//Version 0.80
static const int m_nConstDataVersion = 82;	//Version 0.80 ���C�u�����f�[�^�̃o�[�W����



public:
	SCompLib(){										//�R���X�g���N�^
		m_lpszLibraryName=NULL;
		m_ppCompIndex=NULL;
		m_prop_string=_T("");
		m_aux1 = 0;
		m_aux2 = 0;
		m_dataVersion = m_nConstDataVersion;
		//m_nCount=0;
	}
	~SCompLib();										//�f�X�g���N�^
	bool ReadLb3(SReadCE3& rce3,bool singleComponent = false);//LB3���C�u�����̓ǂݍ���
	void deleteContent();//���e�̔j��

	bool writeLibraryFile(LPCTSTR lpszFileName);	//�t�@�C����������
	bool readLibraryFile(LPCTSTR lpszFileName);		//�t�@�C���ǂݍ���

	void setLibraryName(LPCTSTR name);				//���C�u�������̐ݒ�
	LPCTSTR libraryName() const ;					//���C�u�������𓾂�
	int count() const{return m_listCompIndex.size();}	//�Ǘ����i���𓾂�
	SCompIndex* compIndex(int nIndexNum) const;			//n�Ԗڂ̕��i�̃C���f�b�N�X�𓾂�

	static bool WriteAlias(SWriteCE3& wce3,LPCTSTR  alias,LPCTSTR  name);
	wstring uniqueName(const wstring& name);	//�����̕��i���Əd�Ȃ�Ȃ����O�����

private:
	bool ReadLb3Ptn(SReadCE3& rce3);//�p�^�[���̓ǂݍ���
	bool ReadLb3Comp(SReadCE3& rce3);
	bool ReadLb3Alias(SReadCE3& rce3);

	void clearPtnList();	//�����o�[�ϐ��p�^�[�����X�g�̃N���A


	bool isUniqueName(const wstring& name);	//�����̕��i���Əd�����Ă��Ȃ����ǂ���

	//bool WriteLb3Pin(SWriteCE3& wce3,const SPin& pin);							//�s���̏�������
	//bool WriteLb3Ptn(SWriteCE3& wce3,const SPtn& ptn,const char*  name);			//�p�^�[���̏�������
	//bool WriteLb3Comp(SWriteCE3& wce3,const SCompInfo& compinfo,SWriteCE3& wce3);	//���i�̏�������
	
};


#endif
