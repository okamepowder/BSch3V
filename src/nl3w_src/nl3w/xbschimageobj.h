/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素イメージオブジェクト
****************************************************************************/
#ifndef XBSCHIMAGEOBJ_H
#define XBSCHIMAGEOBJ_H

#include "xbsch.h"
#include "xbschobj.h"

class SImageData;

#define IMAGEOBJ_MIN_MAGNIFICATION  5
#define IMAGEOBJ_MAX_MAGNIFICATION 200


class SXBSchImageObj :	public SXBSchObj
{
public:
	SXBSchImageObj(void);
	virtual ~SXBSchImageObj(void);

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc* );
	bool writeCe3(SWriteCE3& ,const SPoint* );


protected:
	SImageData*	m_pImageData;
	int m_drawMagnification;

protected:
	void DeleteImageData();


public:
	int		ImageWidth();
	int		ImageHeight();
	int		DrawMagnification(){return m_drawMagnification;}
	void	SetDrawMagnification(int n);
	const SImageData* ImageData(){return m_pImageData;}

	bool LoadDIB(const char* fname);

public:
	bool keepOrder(){return true;}

public:
	//IDの取得
	unsigned id(){return ID_IMAGEOBJECT;}
	unsigned objectFilterValue(){return OBJMASK_IMAGE;} 
	//複製を行う
	SXBSchObj* duplicate();

	//座標の設定  m_p2への設定を無効にしている
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//左右反転を行う
	void mirrorH(){}	//何もしない
	//回転を行う
	void rotate(){}		//何もしない

	//占有範囲を返す
	SRect area();
	SRect rotateRect();


	//点を指定して選択重要度を返す
	int qSelectWeight(const SPoint& p);
	//範囲を指定して選択を行う
	unsigned testSelection(const SRect& rc);
	//範囲を選択してドラッグ選択を行う
	unsigned testSelectionForDrag(const SRect& rc);
	//描画が必要かどうかを返す
	bool qRedraw(const SRect& rc);

	//単独選択のときにミラー可能かどうかを返す
	bool canMirrorSingleSelected() {return false;}
	//単独選択のときに回転可能かどうかを返す
	bool canRotateSingleSelected() {return false;}	
	//アトリビュートの変更が可能な場合はtrueを返す。
	bool canAttribute(){return true;}

};
#endif
