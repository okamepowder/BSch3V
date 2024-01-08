/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Copyright (C) 2002 H.Okada(suigyodo) All rights reserved.
/****************************************************************************
                    XBSch�}�ʏ��N���X

  �V�[�g�f�[�^�̃o�[�W�����͂��̃N���X�Œ�`���Ă���B
  static const int m_nConstDataVersion = xx;



****************************************************************************/
#ifndef XBSCHSHEETINFO_H
#define XBSCHSHEETINFO_H

#include "xbsch.h"
#include "xbschobj.h"

class SXBSchSheetInfo:public SXBSchObj
{
public:
	//�R���X�g���N�^
	SXBSchSheetInfo();
	
	//SXBSchSheetInfo(){m_p1.setX(1500);m_p1.setY(1000);m_Selection=0xFF;m_Layer=0;}
	//SXBSchJunc(const SXBSchJunc& junc);	//�R�s�[�R���X�g���N�^
	//�f�X�g���N�^
	virtual ~SXBSchSheetInfo(){};
/*
	//�X�g���[������̓ǂݍ��� �`-SHEETINFO
	bool readStream(SCsvIStream* pStrm,const QPoint* pptOffset);
	//�X�g���[���ւ̏������݁@+SHEETINFO�`+SHEETINFO
	bool writeStream(SCsvOStream* pStrm,const QPoint* pptOffset);
*/

protected:
	int m_totalPages;
	int m_page;
	string m_strProject;
	int m_version;
	int m_initPos;

public:
	enum {
		INITPOS_LT = 0,
		INITPOS_LB = 1,
		INITPOS_RT = 2,
		INITPOS_RB = 3
	};


public:
	void setTotalPages(int n){m_totalPages = n;}
	void setPage(int n){m_page = n;}
	void setProjectName(const char* psz){m_strProject = psz;}
	void setInitPos(int n);
	
	int totalPages(){return m_totalPages;}
	int page(){return m_page;}
	const char* projectName(){return m_strProject.c_str();}
	int dataVersion(){return m_version;}
	int initPos(){return m_initPos;}


	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//ID�̎擾
	unsigned id(){return ID_SHEETINFO;}
	unsigned objectFilterValue(){return 0;} 
public:
	static const int m_nConstDataVersion = 68;	//�V�[�g�f�[�^�̃o�[�W����

	void setWidth(int w) {m_p1.setX(w);}
	void setHeight(int h){m_p1.setY(h);}
	int width(){return x1();}
	int height(){return y1();}

	void setEditLayer(int n){m_Layer=n;}	
	int editLayer(){return m_Layer;}

	void setVisbleLayer(unsigned n){m_Selection = n;}
	unsigned visbleLayer(){return m_Selection;}
	
	void setX1(int){}
	void setY1(int){}
	void setP1(const SPoint&){}
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}

	//�ʒu�𓮂���
	void move(int,int){};
	//X���W���w�肵�č��E���]���s��
	void mirrorH(int){};
	//���E���]���s��
	void mirrorH(){};
	//XY���S���W���w�肵�āA��]���s��
	void rotate(const SPoint&){};
	//��]���s��
	void rotate(){};
	//�I���t���O���Z�b�g����
	void setSelection(unsigned){};
	//�I������������
	void resetSelection(){};
	//��L�͈͂�Ԃ�
	SRect area(){return SRect(0,0,m_p1.x(),m_p1.y());}

	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() {return false;}//2008/06/26
	//�P�ƑI���̂Ƃ��ɉ�]�\���ǂ�����Ԃ�
	bool canRotateSingleSelected() {return false;}//2008/06/26

};

#endif
