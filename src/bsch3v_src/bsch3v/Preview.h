/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Preview.h : ヘッダー ファイル
//
#ifndef PREVIEW_H
#define PREVIEW_H

/////////////////////////////////////////////////////////////////////////////
// CPreview ウィンドウ

class CPreview : public CWnd
{
// コンストラクション
public:
	CPreview();

// アトリビュート
private:
	const SCompIndex* m_pPartIndex;
	CSize		m_sizePart;

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CPreview)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CPreview();

	void SetPartIndex(const SCompIndex* pPartIndex);

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CPreview)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif