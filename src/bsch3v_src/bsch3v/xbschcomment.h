/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f���x���N���X
****************************************************************************/
#ifndef XBSCHCOMMENT_H
#define XBSCHCOMMENT_H

#include "xbsch.h"
#include "xbschobj.h"
#include "SMarkupTextLine.h"

#define COMMENT_Y_OFFSET 2


class SXBSchComment:public SXBSchObj
{
public:
	enum BSCHCOMMENT_DIR{
		COMMENT_DIR_0		=0,
		COMMENT_DIR_1		=1,
		COMMENT_DIR_2		=2,
		COMMENT_DIR_3		=3
	};



public:
	static wstring	m_defaultFontName;
	static int	m_defaultFontSize;
	static bool	m_defaultFontBold;
	static bool	m_defaultFontItalic;
	static bool	m_defaultFontUnderline;
	static bool	m_defaultFontStrikeOut;



public:
	//�R���X�g���N�^
	SXBSchComment();
	//�f�X�g���N�^
	virtual ~SXBSchComment();

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

    //ID�̎擾
	unsigned id(){return ID_COMMENT;}
	unsigned objectFilterValue(){return OBJMASK_COMMENT;} 

	MarkupTextLineList* m_pMarkupTextLineList;


protected:
	wstring	m_strText;
	bool	m_drawSizeIsValid;
//	int		m_drawAreaVOffset;
	SSize	m_drawSize;
//	SSize	m_drawRealSize;
	wstring	m_fontName;
	int		m_fontSize;
	bool	m_fontBold;
	bool	m_fontItalic;
	bool	m_fontUnderline;
	bool	m_fontStrikeOut;

	SSize	m_1lineDrawSize;
	int		m_width;
	int		m_lineHeight;
	bool	m_enableTag;

	bool m_bHoriz;	//2012/11/12
	int  m_dir;	//2014/12/14

public:
	bool keepOrder(){return true;}


public:
	//�������s��
	SXBSchObj* duplicate();
	//���W�̐ݒ�  m_p2�ւ̐ݒ�𖳌��ɂ��Ă���
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//���E���]���s��
	void mirrorH(){}	//�������Ȃ�

	//XY���S���W���w�肵�āA��]���s��
	void rotate(const SPoint& p);
	//��]���s��
	void rotate();

	//�_���w�肵�đI���d�v�x��Ԃ�
	int qSelectWeight(const SPoint& p);
	//�͈͂��w�肵�đI�����s��
	unsigned testSelection(const SRect& rc);
	//�͈͂�I�����ăh���b�O�I�����s��
	unsigned testSelectionForDrag(const SRect& rc);
	//�`�悪�K�v���ǂ�����Ԃ�
	bool qRedraw(const SRect& rc);
	//��L�͈͂�Ԃ�
	SRect area();
	SRect rotateRect();



public:
	//�L���ȃI�u�W�F�N�g�Ȃ�true��Ԃ�
	bool qValid(){return (m_strText.length()>0);}
	void setText(const TCHAR* psz);
	const TCHAR* text(){return m_strText.c_str();}
	//�A�g���r���[�g�̕ύX���\�ȏꍇ��true��Ԃ��B
	bool canAttribute(){return true;}


	bool horizontal(){ return m_bHoriz; }
	int dir(){return m_dir;}	// 2014/12/14 


	bool drawSizeIsValid(){return m_drawSizeIsValid;}
//	void setDrawSize(int vOffset,const SSize& size);	//�`��T�C�Y�̐ݒ�B���_���AvOffset�ォ��̃T�C�Y
//	void setDrawSize(const SSize& size);		//�`��T�C�Y�̐ݒ�
	void setDrawSize(const SSize& size,const SSize& size1Line);		//�`��T�C�Y�̐ݒ�
//	void set1LineDrawSize(const SSize& size);	//1���C���`�掞�̃T�C�Y�̐ݒ�

	SSize SizeDraw() {return(m_drawSize);}
	SSize Size1Line() {return(m_1lineDrawSize);}

//	void setDrawRealSize(const SSize& size) {m_drawRealSize = size;}
//	SSize SizeDrawReal() {return m_drawRealSize;}



//	int maxWidth();
	void setWidth(int w);
	int width();

	void setFontName(const TCHAR* psz);	//�t�H���g���̐ݒ�		
	const TCHAR* fontName()const {return m_fontName.c_str();}	//�t�H���g���̎擾
	void setFontSize(int fontSize);	//�t�H���g�T�C�Y�̐ݒ�
	int	fontSize()const{return m_fontSize;}//�t�H���g�T�C�Y�̎擾
	void setFontBold(bool istrue);	//�{�[���h�ݒ�
	bool fontBold()const{return m_fontBold;} //�{�[���h�ݒ�擾
	void setFontItalic(bool istrue);//�C�^���b�N�ݒ�
	bool fontItalic()const{return m_fontItalic;}//�C�^���b�N�ݒ�擾
	void setFontUnderline(bool istrue);//�A���_�[���C���ݒ�
	bool fontUnderline()const{return m_fontUnderline;}//�A���_�[���C���ݒ�擾
	void setFontStrikeOut(bool istrue);//���������ݒ�
	bool fontStrikeOut()const{return m_fontStrikeOut;}//���������ݒ�擾
	void setEnableTag(bool istrue);//�^�O�L���ݒ�
	bool enableTag()const{return m_enableTag;}//�^�O�ݒ�擾
	void deleteMarkupLineList();
	void setupMarkupLineList(bool printMode);

	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() {return false;}//2008/06/26
	//�P�ƑI���̂Ƃ��ɉ�]�\���ǂ�����Ԃ�
	bool canRotateSingleSelected() {return true;}//2008/06/26, 2012/11/11 true�ɏC��


};

#endif
