/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2005 H.Okada

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
	char*			m_pszName;		//�p�^�[����
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
	bool writeCe3(SWriteCE3& wce3,const char* pszName);
	void setSize(const SSize& sizePixel);
};



////////////////////////////////////////////////////////////////////
//�s���N���X�̒�`
class SPin
{
protected:
	int			m_nBlock;	//m_nBlock�̐������s���ԍ�������z����Ǘ����Ă���
	char**		m_ppszNum;	//�s���ԍ�������z��ւ̃|�C���^
	char*		m_pszName;	//�s�����ւ̃|�C���^
	int			m_nType;	//�s���̃^�C�v
	int			m_nLTRB;	//�ʒu LTRB
	int			m_nOffset;	//�ʒu �I�t�Z�b�g
	unsigned	m_flags;	//�t�@�C���ɕۑ�����Ȃ����쎞�̃t���O

	enum{
		FLAGS_NC = 0x0001
	};

protected:
	void deleteBuff();

public:
	SPin();					//�R���X�g���N�^
	~SPin();
	SPin(const SPin&);				//�R�s�[�R���X�g���N�^
	SPin& operator=(const SPin&);	//������Z�q

	
	void setBlock(int  nBlock);	//�u���b�N�����Z�b�g����B
	void setPinName(const char* name);				//�s�������Z�b�g����B
	void setPinNum(int nBlock,const char* name);	//�s���ԍ����Z�b�g����B
	void setType(int type){ m_nType=type;}
	void setLtrb(int ltrb){ m_nLTRB=ltrb;}
	void setOffset(int offset){m_nOffset=offset;}
	const char* pinName() const {return m_pszName;}
	const char* pinNum(int nBlock) const;
	int type() const {return m_nType;}
	int ltrb() const {return m_nLTRB;}
	int offset() const {return m_nOffset;}
	int block() const { return m_nBlock;}

	bool nc() const { return m_flags &  FLAGS_NC;}
	void set_nc(bool itisNC);

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
	char*	m_pszRef;	//�f�t�H���g���i�ԍ�
	SSize	m_size;		//���i�̃T�C�Y
	int		m_nBlock;	//�p�b�P�[�W������̃u���b�N��
	SPtn*	m_pPtn;		//LB3����ǂݍ��񂾃p�^�[���ւ̃|�C���^
	int		m_nPinCount;//�s����
	PSPin*	m_ppPin;	//LB3����ǂݍ��񂾃s���N���X�̔z��ւ̃|�C���^
	string	m_note;		//�m�[�g 2005/04/30
	string	m_mfr;		//�����ҋL���� 2009/02/01
	string	m_mfrpn;	//�����Ҍ^���L���� 2009/02/01
	string	m_pkg;		//�p�b�P�[�W�L���� 2009/02/01

	SCompInfo(){		//�R���X�g���N�^
		m_pszRef	=NULL;
		m_nBlock	=1;
		m_pPtn		=NULL;
		m_nPinCount	=0;
		m_ppPin		=NULL;
		m_note		="";
		m_mfr		="";
		m_mfrpn		="";
		m_pkg		="";
	}
	~SCompInfo();			//�f�X�g���N�^

	SCompInfo(const SCompInfo&);			//�R�s�[�R���X�g���N�^
	SCompInfo& operator=(const SCompInfo&);	//������Z�q

	bool readCe3(SReadCE3& rce3,string& name,string& ptnname);	//�t�@�C������̓ǂݍ���
	bool writeCe3(SWriteCE3& wce3,const char* pszName);

	void setRef(const char* ref);		//�f�t�H���g���i�ԍ��̐ݒ�
	void setSize(const SSize& size,bool createPtn);	//�T�C�Y�̐ݒ� �p�^�[����L����ꍇ�̓p�^�[���̃T�C�Y���ύX
	void setUsePattern(bool usepattern);
	void setBlock(int block);
	void addPin(SPin* ppin);
	void removePin(int ltrb,int offset,bool move);
	void insertNullPin(int ltrb,int offset);
	SPin* pinInfo(int ltrb,int offset);	//�w�肵���ʒu�̃s�����𓾂�
};

////////////////////////////////////////////////////////////////////
//���i�C���f�b�N�X�N���X�̒�`
class SCompIndex
{
	char*				m_lpszName;	//���i��
public:
	SCompInfo*			m_pCompInfo;//�Ή����镔�i���N���X�ւ̃|�C���^

	SCompIndex(){			//�R���X�g���N�^
		m_lpszName	=NULL;
		m_pCompInfo =NULL;
	}
	~SCompIndex(){			//�f�X�g���N�^
		delete[]m_lpszName;
	}
	void setName(const char* name);
	void setCompInfo(SCompInfo* pcinfo){m_pCompInfo = pcinfo;}

	SCompInfo* compInfo() const{return m_pCompInfo;}
	const char*	name() const{return m_lpszName;}			//���i���𓾂�
	const char* number() const;			//���i�ԍ��𓾂�

	const char* Mfr() const;			//�����҂𓾂�		20090203
	const char* MfrPn() const;			//�����Ҍ^���𓾂�		20090203
	const char* Pkg() const;			//�p�b�P�[�W�𓾂�		20090203

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
	char*			m_lpszLibraryName;		//���C�u������
	string			m_prop_string;
	SPtnList		m_listPtn;				//�ǂݍ��񂾃p�^�[���̃��X�g
	SCompInfoList	m_listCompInfo;			//�ǂݍ��ݍ��񂾕��i�N���X�̃��X�g
	SCompIndexList	m_listCompIndex;		//�ǂݍ��ݍ��񂾕��i�C���f�b�N�X�̃��X�g
	PSCompIndex*	m_ppCompIndex;			//�ǂݍ��ݍ��񂾕��i�C���f�b�N�X�̃|�C���^�z��
	int				m_aux1;
	int				m_aux2;


public:
	SCompLib(){										//�R���X�g���N�^
		m_lpszLibraryName=NULL;
		m_ppCompIndex=NULL;
		m_prop_string="";
		m_aux1 = 0;
		m_aux2 = 0;
		//m_nCount=0;
	}
	~SCompLib();										//�f�X�g���N�^
	bool ReadLb3(SReadCE3& rce3,bool singleComponent = false);//LB3���C�u�����̓ǂݍ���
	void deleteContent();//���e�̔j��

	bool writeLibraryFile(const char* lpszFileName);	//�t�@�C����������
	bool readLibraryFile(const char* lpszFileName);		//�t�@�C���ǂݍ���

	void setLibraryName(const char* name);				//���C�u�������̐ݒ�
	const char* libraryName() const ;					//���C�u�������𓾂�
	int count() const{return m_listCompIndex.size();}	//�Ǘ����i���𓾂�
	SCompIndex* compIndex(int nIndexNum) const;			//n�Ԗڂ̕��i�̃C���f�b�N�X�𓾂�

	static bool WriteAlias(SWriteCE3& wce3,const char*  alias,const char*  name);
	string uniqueName(const string& name);	//�����̕��i���Əd�Ȃ�Ȃ����O�����

private:
	bool ReadLb3Ptn(SReadCE3& rce3);//�p�^�[���̓ǂݍ���
	bool ReadLb3Comp(SReadCE3& rce3);
	bool ReadLb3Alias(SReadCE3& rce3);

	void clearPtnList();	//�����o�[�ϐ��p�^�[�����X�g�̃N���A


	bool isUniqueName(const string& name);	//�����̕��i���Əd�����Ă��Ȃ����ǂ���

	//bool WriteLb3Pin(SWriteCE3& wce3,const SPin& pin);							//�s���̏�������
	//bool WriteLb3Ptn(SWriteCE3& wce3,const SPtn& ptn,const char*  name);			//�p�^�[���̏�������
	//bool WriteLb3Comp(SWriteCE3& wce3,const SCompInfo& compinfo,SWriteCE3& wce3);	//���i�̏�������
	
};


#endif
