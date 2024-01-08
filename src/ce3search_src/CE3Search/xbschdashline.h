/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素装飾線クラス (旧称は破線クラス)
****************************************************************************/
#ifndef XBSCHDASHLINE_H
#define XBSCHDASHLINE_H

#include "xbschline.h"

//破線クラス Version 0.68で装飾ラインに拡張
class SXBSchDash:public SXBSchLine
{
public:
	static const int MIN_WIDTH = 1;
	static const int MAX_WIDTH = 5;
	static const int MIN_ENDMARKSIZE = 3;
	static const int MAX_ENDMARKSIZE = 20;

	enum LINESTYLE{
		LINESTYLE_NORMAL		=0,
		LINESTYLE_DASH			=1,
		LINESTYLE_LINEDOT		=2,
		LINESTYLE_LINEDOTDOT	=3
	};

	enum ENDPOINTSTYLE{
		ENDPOINTSTYLE_NORMAL	=0,
		ENDPOINTSTYLE_ARROW		=1,
		ENDPOINTSTYLE_CIRCLE	=2
	};




	//コンストラクタ
	SXBSchDash();
	//デストラクタ
	~SXBSchDash();


	////点を指定して選択重要度を返す
	//int qSelectWeight(const SPoint&);

	//範囲を指定して選択を行う
	unsigned testSelection(const SRect& );

	//描画が必要かどうかを返す
	bool qRedraw(const SRect& );

	SRect area();
	SRect rotateRect();



	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	
	//IDの取得
	unsigned id(){return ID_DASH;}
	unsigned objectFilterValue(){return OBJMASK_DASH;} 
	//複製を行う
	SXBSchObj* duplicate();

	//アトリビュートの変更が可能な場合はtrueを返す。
	bool canAttribute() {return true;}



protected:
	bool m_nodeCacheValid;

	bool m_bCurve;
	SPoint m_ptCt1;
	SPoint m_ptCt2;
	int m_lineStyle;
	int m_lineWidth;
	int m_startPointStyle;
	int m_endPointStyle;
	int m_endMarkSize;

	//int m_nodeCount;
	SPoint* m_arrayNode;
	SRect m_rcCurve;

	//static const int m_nBezierNodes = 32;
	//static const int m_nBezierNodeCache
	void createNodes();	//曲線の選択テスト用のノードを生成する。描画はWindowsのAPIを使う。
	//SPoint intermediatePoint(SPoint p0,SPoint p1,int n);	//0 < n < m_nBezierNodes


	void setDefaultCtrlPos();	//曲線の制御点をデフォルト位置に設定する。

public:
	bool keepOrder(){return true;}

public:
	//座標の設定
	void setX1(int x1){m_nodeCacheValid=false; m_p1.setX(x1);}
	void setY1(int y1){m_nodeCacheValid=false; m_p1.setY(y1);}
	void setX2(int x2){m_nodeCacheValid=false; m_p2.setX(x2);}
	void setY2(int y2){m_nodeCacheValid=false; m_p2.setY(y2);}
	void setP1(const SPoint& p){m_nodeCacheValid=false; m_p1 = p;}
	void setP2(const SPoint& p){m_nodeCacheValid=false; m_p2 = p;}

	//曲線フラグの入出力
	void setCurve(bool curve);
	bool curve(){return m_bCurve;}

	//線幅の入出力
	void setLineWidth(int width){
		if(width < SXBSchDash::MIN_WIDTH){
			m_lineWidth = SXBSchDash::MIN_WIDTH;
		}else if(width > SXBSchDash::MAX_WIDTH){
			m_lineWidth = SXBSchDash::MAX_WIDTH;
		}else{
			m_lineWidth = width;
		}
	}

	int lineWidth(){return m_lineWidth;}


	void setEndMarkSize(int markSize){
		if(markSize < SXBSchDash::MIN_ENDMARKSIZE){
			m_endMarkSize = SXBSchDash::MIN_ENDMARKSIZE;
		}else if(markSize > SXBSchDash::MAX_ENDMARKSIZE){
			m_endMarkSize = SXBSchDash::MAX_ENDMARKSIZE;
		}else{
			m_endMarkSize = markSize;
		}
	}

	int endMarkSize(){return m_endMarkSize;}

	//指定点が操作可能なものの上にあるかどうか
	int isEditablePoint(const SPoint& pt,int& info);


	//制御点の入出力
	void setCt1(const SPoint& p){m_nodeCacheValid=false; m_ptCt1 = p;}
	void setCt2(const SPoint& p){m_nodeCacheValid=false; m_ptCt2 = p;}
	SPoint Ct1(){return  m_ptCt1;}
	SPoint Ct2(){return  m_ptCt2;}

	//インデックスを使った座標の設定
	void setPointIndex(int index,SPoint pt);

	//インデックスを使った座標の取得
	SPoint pointIndex(int index){
		switch(index){
			case EDITPOINT_P1:
				return m_p1;
			case EDITPOINT_P2:
				return m_p2;
			case EDITPOINT_C1:
				return m_ptCt1;
			default:
			//case EDITPOINT_C2:
				return m_ptCt2;
		}
	}



	//スタイルの入出力
	void setLineStyle(int style){
		if(style == LINESTYLE_NORMAL || style == LINESTYLE_LINEDOT || style == LINESTYLE_LINEDOTDOT){
			m_lineStyle = style;
		}else{
			m_lineStyle = LINESTYLE_DASH;
		}
	}

	void setStartPointStyle(int style){
		if(style == ENDPOINTSTYLE_ARROW || style == ENDPOINTSTYLE_CIRCLE){
			m_startPointStyle = style;
		}else{
			m_startPointStyle = ENDPOINTSTYLE_NORMAL;
		}
	}

	void setEndPointStyle(int style){
		if(style == ENDPOINTSTYLE_ARROW || style == ENDPOINTSTYLE_CIRCLE){
			m_endPointStyle = style;
		}else{
			m_endPointStyle = ENDPOINTSTYLE_NORMAL;
		}
	}
	int lineStyle(){return m_lineStyle;}
	int startPointStyle(){return m_startPointStyle;}
	int endPointStyle(){return m_endPointStyle;}
	
	bool qValid();
	//{return(m_p1 != m_p2);}
	bool isHorizontal(){return (m_p1.y() == m_p2.y())&& !m_bCurve;}
	bool isVertical(){return (m_p1.x() == m_p2.x())&& !m_bCurve;}

	//位置を動かす
	void move(int offsetx,int offsety);

	//X座標を指定して左右反転を行う
	void mirrorH(int x);
	//左右反転を行う
	void mirrorH();
	//XY中心座標を指定して、回転を行う
	void rotate(const SPoint& p);
	//回転を行う
	void rotate();
};

#endif
