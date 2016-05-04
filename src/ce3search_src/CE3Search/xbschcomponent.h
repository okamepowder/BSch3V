/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f���i�N���X
****************************************************************************/
#ifndef XBSCHCOMPONENT_H
#define XBSCHCOMPONENT_H

#include "xbsch.h"
#include "xbschobj.h"
#include "complib.h"



//    (0) 0      (1) 90     (2) 180    (3) 270
//     ��         ��         ��         ��
//    ���s��     ���k��     ���a��     ���q��     
//  ���k���q�� ���a���s�� ���q���k�� ���s���a��
//    ���a��     ���q��     ���s��     ���k��     
//     ��         ��         ��         ��
//
//    (4) M0     (5) M90    (6) M180   (7) M270
//     ��         ��         ��         ��
//    ���s��     ���k��     ���a��     ���q��     
//  ���q���k�� ���s���a�� ���k���q�� ���a���s��
//    ���a��     ���q��     ���s��     ���k��     
//     ��         ��         ��         ��


class SXBSchComponent:public SXBSchObj
{
public:
	enum BSCHCOMP_DIR{
		COMPONENT_DIR_0		=0,
		COMPONENT_DIR_90	=1,
		COMPONENT_DIR_180	=2,
		COMPONENT_DIR_270	=3,
		COMPONENT_DIR_M0	=4,
		COMPONENT_DIR_M90	=5,
		COMPONENT_DIR_M180	=6,
		COMPONENT_DIR_M270	=7
	};

	enum BSCHCOMP_TEXTPOS{
		COMPONENT_DEFAULT_REFNUM_X	=2,
		COMPONENT_DEFAULT_REFNUM_Y	=10,
		COMPONENT_DEFAULT_NAME_X	=2,
		COMPONENT_DEFAULT_NAME_Y	=20
	};

	enum BSCHCOMP_PINLTRB{
		COMPONENT_HASLPIN	=1,
		COMPONENT_HASTPIN	=2,
		COMPONENT_HASRPIN	=4,
		COMPONENT_HASBPIN	=8,
	};

	//�R���X�g���N�^
	SXBSchComponent();
	//�f�X�g���N�^
	virtual ~SXBSchComponent();
	SXBSchComponent(const SXBSchComponent&);				//�R�s�[�R���X�g���N�^
	SXBSchComponent& operator=(const SXBSchComponent&);	//������Z�q
	bool readEmbLibCe3(SReadCE3&, SCompInfo*,wstring& );
	bool writeEmbLibCe3(SWriteCE3& wce3);

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

    //ID�̎擾
	unsigned id(){return ID_COMPONENT;}
	unsigned objectFilterValue(){return OBJMASK_COMPONENT;} 
protected:
	wstring	m_name;			//���O
	SPoint	m_name_pos;		//���O�̌��_����̃I�t�Z�b�g
	bool	m_name_hide;	//���O��\��
	bool	m_name_drawSizeIsValid;
	SSize	m_name_drawSize;
	bool	m_name_horiz;
	wstring	m_refnum;		//�Q�Ɣԍ�
	SPoint	m_refnum_pos;	//�Q�Ɣԍ��̌��_����̃I�t�Z�b�g
	bool	m_refnum_hide;	//�ԍ���\��
	bool	m_refnum_drawSizeIsValid;
	SSize	m_refnum_drawSize;
	bool	m_refnum_horiz;
	int		m_block;		//�u���b�N�ԍ�
	int		m_dir;			//����
	int		m_pinltrb;		//LTRB��10�s�N�Z�����s�������݂��Ă��邩�ǂ����̃t���O
	SCompIndex* m_compInfoIndex;		//���i�̏��ւ̃|�C���^
//	SCompIndex* m_embeddedCompInfoIndex;	//���ߍ��񂾕��i�̏��ւ̃|�C���^
	bool	m_useEmbeddedLib;
	wstring	m_orgname;		//�璷�����ǁA���C�u��������ǂݑ��˂����i���c�����߂ɕK�v
	wstring	m_packagename;	//2009/01/06 �p�b�P�[�W��
	wstring	m_mfrpnum;		//2009/01/07 �����Ҍ^��
	wstring	m_mfr;			//2009/01/07 ������
	wstring	m_note;

protected:
	void	mirrorDir();		//m_dir�ϐ������E���]����
	void	rotateDir();		//m_dir�ϐ����E��]����
	void	resetRefnumPos();	//�Q�Ɣԍ��̈ʒu���f�t�H���g�̈ʒu�ɕύX
	void	resetNamePos();		//���i���̈ʒu���f�t�H���g�̈ʒu�ɕύX
//	void	setPinLtrb();		//LTRB�Ƀs�������݂��Ă��邩�ǂ����̃t���O�̐ݒ�



public:
	//bool keepOrder(){return true;}
	void	setPinLtrb();		//LTRB�Ƀs�������݂��Ă��邩�ǂ����̃t���O�̐ݒ�
	
	SXBSchObj* duplicate();	//�������s��
	//���W�̐ݒ�  m_p2�ւ̐ݒ�𖳌��ɂ��Ă���
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}

	void move(int offsetx,int offsety);	//�ʒu�𓮂���
	void mirrorH(int x); //X���W���w�肵�č��E���]���s��
	virtual void mirrorH(); //���E���]���s��
	void rotate(const SPoint& p); //XY���S���W���w�肵�āA��]���s��
	void rotate();	//��]���s��
	int qSelectWeight(const SPoint& p);				//�_���w�肵�đI���d�v�x��Ԃ�

	int isEditablePoint(const SPoint& pt,int& info);//�w��_������\�Ȃ��̂̏�ɂ��邩�ǂ���


	unsigned testSelection(const SRect& rc);		//�͈͂��w�肵�đI�����s��
	unsigned testSelectionForDrag(const SRect& rc); //�͈͂�I�����ăh���b�O�I�����s��
	bool qRedraw(const SRect&){return true;}		//�`�悪�K�v���ǂ�����Ԃ� qRedraw()�͏�ɕ`�����������B
	
	SRect area();	//��L�͈͂�Ԃ�
	SRect bodyArea();	//��L�͈͂�Ԃ�

	SRect rotateRect();
	SSize size() const;
	SRect nameArea() const;
	SRect refnumArea() const;

	void setNameDrawSize(const SSize& size);	//�`��T�C�Y�̐ݒ�
	void setRefnumDrawSize(const SSize& size);	//�`��T�C�Y�̐ݒ�

	//���i�ԍ��A�l�̔�\��
//#define XBSCHCOMPONET_NUMVAL_HIDE_VAL 1000000
	void SetHideVal(bool hide);
	void SetHideNum(bool hide);
	bool GetHideVal() const;
	bool GetHideNum() const;


public:
	void setName(const TCHAR* psz);
	const TCHAR* name() const {return m_name.c_str();}
	void setNameHorizontal(bool bHoriz);	//2005/04/09
	bool nameHorizontal() const {return m_name_horiz;}//2005/04/09

	void setNote(const TCHAR* psz);
	const TCHAR* note() const {return m_note.c_str();}
	void setNamePos(const SPoint& p){m_name_pos = p;}
	bool moveNamePos(int offsetx,int offsety);
	SPoint namePos(){return m_name_pos;}

	void setRefnum(const TCHAR* psz);
	const TCHAR* refnum() const {return m_refnum.c_str();}
	void setRefnumHorizontal(bool bHoriz);//2005/04/09
	bool refnumHorizontal() const {return m_refnum_horiz;}//2005/04/09
	void setRefnumPos(const SPoint& p){m_refnum_pos = p;}
	bool moveRefnumPos(int offsetx,int offsety);
	SPoint refnumPos(){return m_refnum_pos;}

	void setOrgName(const TCHAR* psz);
	const TCHAR* orgname() const {return m_orgname.c_str();}


	const SCompIndex* compInfoIndex() const {return m_compInfoIndex;}
	void setCompInfoIndex(const SCompIndex* pIndex);

	//>>>> Version 0.63
	void setPackageName(const TCHAR* psz);
	const TCHAR* packagename() const {return m_packagename.c_str();}

	void setMfr(const TCHAR* psz);
	const TCHAR* mfr() const {return m_mfr.c_str();}

	void setMFrPNum(const TCHAR* psz);
	const TCHAR* mfrpnum() const {return m_mfrpnum.c_str();}
	//<<<< Version 0.63

	
	const SPin*  pinLoc(int nIndex,int& nLtrb,SPoint& ptLoc) const; //�s���̍������̈ʒu�����擾����
	const SPin*  pinEnd(int nIndex,int& nLtrb,SPoint& ptEnd) const; //�s���̐�[�̈ʒu�����擾����
	SPoint pinPosition(int)	const;//n�Ԗڂ̃s���̐�[�̈ʒu�𓾂�BpinEnd()�̃��[�e�B���C�Y

	SPin* pinInfo(int nIndex);

	int pinCount() const; //�s���̐��𓾂�

	void setPinNC(int n,bool bSet);	//n�Ԗڂ̃s����NC�t���O��bSet�ɂ���
	bool pinNC(int n) const ;		//n�Ԗڂ̃s����NC�t���O�𓾂�
	void presetPinNC();				//NC�t���O���v���Z�b�g����


	void setBlock(int n);
	int block(){return m_block;}

	void setDir(int n);
	int dir(){return m_dir;}
	
	bool canAttribute(){return true;}	//�A�g���r���[�g�̕ύX���\�ȏꍇ��true��Ԃ��B
};

#endif
