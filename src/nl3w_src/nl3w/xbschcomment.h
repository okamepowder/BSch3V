/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素ラベルクラス
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
	//コンストラクタ
	SXBSchComment();
	//デストラクタ
	virtual ~SXBSchComment();

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

    //IDの取得
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
	//複製を行う
	SXBSchObj* duplicate();
	//座標の設定  m_p2への設定を無効にしている
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//左右反転を行う
	void mirrorH(){}	//何もしない
	//回転を行う
	void rotate(){}
	//点を指定して選択重要度を返す
	int qSelectWeight(const SPoint& p);
	//範囲を指定して選択を行う
	unsigned testSelection(const SRect& rc);
	//範囲を選択してドラッグ選択を行う
	unsigned testSelectionForDrag(const SRect& rc);
	//描画が必要かどうかを返す
	bool qRedraw(const SRect& rc);
	//占有範囲を返す
	SRect area();
	SRect rotateRect();



public:
	//有効なオブジェクトならtrueを返す
	bool qValid(){return (m_strText.length()>0);}
	void setText(const char* psz);
	const char* text(){return m_strText.c_str();}
	//アトリビュートの変更が可能な場合はtrueを返す。
	bool canAttribute(){return true;}
	bool drawSizeIsValid(){return m_drawSizeIsValid;}
//	void setDrawSize(int vOffset,const SSize& size);	//描画サイズの設定。原点より、vOffset上からのサイズ
//	void setDrawSize(const SSize& size);		//描画サイズの設定
	void setDrawSize(const SSize& size,const SSize& size1Line);		//描画サイズの設定
//	void set1LineDrawSize(const SSize& size);	//1ライン描画時のサイズの設定

//	int maxWidth();
	void setWidth(int w);
	int width();

	void setFontName(const char* psz);	//フォント名の設定		
	const char* fontName()const {return m_fontName.c_str();}	//フォント名の取得
	void setFontSize(int fontSize);	//フォントサイズの設定
	int	fontSize()const{return m_fontSize;}//フォントサイズの取得
	void setFontBold(bool istrue);	//ボールド設定
	bool fontBold()const{return m_fontBold;} //ボールド設定取得
	void setFontItalic(bool istrue);//イタリック設定
	bool fontItalic()const{return m_fontItalic;}//イタリック設定取得
	void setFontUnderline(bool istrue);//アンダーライン設定
	bool fontUnderline()const{return m_fontUnderline;}//アンダーライン設定取得
	void setFontStrikeOut(bool istrue);//取り消し線設定
	bool fontStrikeOut()const{return m_fontStrikeOut;}//取り消し線設定取得
	void setEnableTag(bool istrue);//タグ有効設定
	bool enableTag()const{return m_enableTag;}//タグ設定取得
	void deleteMarkupLineList();
	void setupMarkupLineList();

	//単独選択のときにミラー可能かどうかを返す
	bool canMirrorSingleSelected() {return false;}//2008/06/26
	//単独選択のときに回転可能かどうかを返す
	bool canRotateSingleSelected() {return false;}//2008/06/26


};

#endif
