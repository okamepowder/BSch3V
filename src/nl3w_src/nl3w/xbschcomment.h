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
	static string	m_defaultFontName;
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
	string	m_strText;
	bool	m_drawSizeIsValid;
//	int		m_drawAreaVOffset;
	SSize	m_drawSize;
	string	m_fontName;
	int		m_fontSize;
	bool	m_fontBold;
	bool	m_fontItalic;
	bool	m_fontUnderline;
	bool	m_fontStrikeOut;

	SSize	m_1lineDrawSize;
	int		m_width;
	int		m_lineHeight;
	bool	m_enableTag;

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
	//��]���s��
	void rotate(){}
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
	void setText(const char* psz);
	const char* text(){return m_strText.c_str();}
	//�A�g���r���[�g�̕ύX���\�ȏꍇ��true��Ԃ��B
	bool canAttribute(){return true;}
	bool drawSizeIsValid(){return m_drawSizeIsValid;}
//	void setDrawSize(int vOffset,const SSize& size);	//�`��T�C�Y�̐ݒ�B���_���AvOffset�ォ��̃T�C�Y
//	void setDrawSize(const SSize& size);		//�`��T�C�Y�̐ݒ�
	void setDrawSize(const SSize& size,const SSize& size1Line);		//�`��T�C�Y�̐ݒ�
//	void set1LineDrawSize(const SSize& size);	//1���C���`�掞�̃T�C�Y�̐ݒ�

//	int maxWidth();
	void setWidth(int w);
	int width();

	void setFontName(const char* psz);	//�t�H���g���̐ݒ�		
	const char* fontName()const {return m_fontName.c_str();}	//�t�H���g���̎擾
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
	void setupMarkupLineList();

	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() {return false;}//2008/06/26
	//�P�ƑI���̂Ƃ��ɉ�]�\���ǂ�����Ԃ�
	bool canRotateSingleSelected() {return false;}//2008/06/26


};

#endif
