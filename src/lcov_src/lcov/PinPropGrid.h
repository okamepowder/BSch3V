#pragma once


#include "EditForGrid.h"
class CCompEditView;
//class CEditDetEnter;

class PinPropGridDrawDim
{
public:
	PinPropGridDrawDim();

	void SetNonScrollAreaWidth(int n);
	int SetScrollAreaWidth(int n);
	int SetHeight(int n);

protected:
	int m_posWidth;		//位置の表示幅
	int m_nameWidth;	//ピン名の表示幅
	int m_typeWidth;	//ピンタイプの表示幅
	int m_dispChkWidth;	//ピン番号表示チェックボックス
	int m_numWidth;		//ひとつの番号の表示幅
	int m_column;		//番号表示のカラム数
	int m_rowHeight;	//行の高さ
	int m_row;			//行数

public:
	int PosWidth(){return m_posWidth;}		//位置の表示幅
	int NameWidth(){return m_nameWidth;}	//ピン名の表示幅
	int TypeWidth(){return m_typeWidth;}	//ピンタイプの表示幅
	int DispChkWidth(){return m_dispChkWidth;}	//ピン番号表示チェックボックス
	int NumWidth(){return m_numWidth;}		//ひとつの番号の表示幅
	int Column(){return m_column;}			//番号表示のカラム数
	int RowHeight(){return m_rowHeight;}	//行の高さ
	int Row(){return m_row;}				//行数
	int NumLeft(){return m_posWidth+m_nameWidth+m_typeWidth+m_dispChkWidth;}
	int NumHideLeft(){return m_posWidth+m_nameWidth+m_typeWidth;}
	int PintypeLeft(){return m_posWidth+m_nameWidth;}
	int PinNameLeft(){return m_posWidth;}
	

};


class SPin;


class GridPinInfo
{
public:
	bool m_select;
	SPin* m_pPin;

	GridPinInfo(){
		m_select = false;
		m_pPin = NULL;
	}

	//ソートのための静的関数
	static int compGridPinInfo(const void *arg1, const void *arg2 );
	

};




class CPinPropGrid : public CWnd
{
	DECLARE_DYNAMIC(CPinPropGrid)

public:
	PinPropGridDrawDim m_drawDim;
	CCompEditView* m_pView;

	GridPinInfo* m_pinInfoArray;

	CComboBoxEx m_cmbbox;

protected:
	int m_pinCount;				//ピン数
	//int m_blockCount;			//ブロック数
	int m_selectedIndex;		//選択されているインデックス
	int m_displayTop;			//一番上の列
	int m_displayBlockLeft;		//左端のカラム
	int m_activeColumn;

	static const int GRID_TEXT_HEIGHT = 12;

	CEditForGrid m_edit;


	CImageList m_imagelist;
	void initEditCtrl();
	void initPintypeCombobox();

	enum{
		COLUMN_PINNAME = 1,
		COLUMN_PINTYPE = 2,
		COLUMN_PINNUMHIDE = 3,
		COLUMN_PINNUMBASE = 4
	};

public:
	int BlockCount();

	int PinCount(){
		if(!m_pinInfoArray) return 0;
		return m_pinCount;
	}

	int SelectedIndex(){
		if(!m_pinInfoArray) return -1;
		return m_selectedIndex;
	}

	int DisplayBlockLeft(){
		return m_displayBlockLeft;
	}

	void SetDisplayBlockLeft(int n);

	int DisplayTop(){
		return m_displayTop;
	}

	void SetDisplayTop(int n){
		restoreColumnData();
		if(n<0){
			m_displayTop = 0;
			return;
		}
		if(!m_pinInfoArray) return;
		if(n+ m_drawDim.Row()>=PinCount()){
			m_displayTop = 	PinCount() - m_drawDim.Row();
		}else{
			m_displayTop = n;
		}
		if(m_displayTop<0){
			m_displayTop = 0;
		}
	}




public:
	void PreReleaseTemp();	//選択オブジェクトを解放する前に呼ばれる。
	int SetupPinInfoArray();	
	void restoreColumnData();
	bool restoreActivePintype(int index);
	bool restoreActivePinName(int index);
	bool restoreActivePinNum(int index,int col);
	void initBlock();
	int activeBlock();

public:
	CPinPropGrid();
	virtual ~CPinPropGrid();
	CFont m_font;

protected:
	DECLARE_MESSAGE_MAP()

	//オフスクリーンバッファ
	CBitmap* m_pBmpBuff;
	CSize	m_offScrnBuffSize;

	void drawGrid(CDC* pDC);
	void draw(CDC* pDC);
	void drawPinInfo(CDC* pDC,int row,GridPinInfo* info);
	void drawBG(CDC* pDC);

	//CEditDetEnter* m_pEditCtrl;

	void PointToCell(CPoint pt, int& row, int& column);

	//LOGFONT m_logfont;

	void activateColumn(int col);
	void toggleHideNum();
	void startEditPinType();
	void startEditPinName();
	void startEditPinNum(int col);

public:
	afx_msg void OnPaint();

	afx_msg LRESULT OnFixText(WPARAM wParam, LPARAM lParam);


	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnClose();
//	afx_msg void OnDestroy();
protected:
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

class CPinPropGridTitle : public CWnd
{
	DECLARE_DYNAMIC(CPinPropGridTitle)

public:
	CPinPropGrid* m_pGrid;

public:
	CPinPropGridTitle();
	virtual ~CPinPropGridTitle();

protected:
	//オフスクリーンバッファ
	CBitmap* m_pBmpBuff;
	//CSize	m_offScrnBuffSize;
	void draw(CDC* pDC);


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};