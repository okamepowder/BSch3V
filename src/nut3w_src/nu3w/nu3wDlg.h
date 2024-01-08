/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// nu3wDlg.h : ヘッダー ファイル
//

#pragma once
#include "ddlb.h"

//#define PARTNUM_INIT_MAX (1000)		//部品番号初期値の最大値
//#define PARTNUM_STEP_MAX (20)		//部品番号ステップ値の最大値

// Cnu3wDlg ダイアログ
class Cnu3wDlg : public CDialog
{
// コンストラクション
public:
	Cnu3wDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_NU3W_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	BOOL m_noMessageBox;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CDdLb m_lbSrc;
protected:
	virtual void OnOK();
	void setButtonEnable();  //ソースのリストボックスの状態でOKボタンのイネーブル状態を設定

	void setPrefixFilterControlEnable();

public:
//	afx_msg void OnBnClickedRef();
	afx_msg void OnBnClickedClr();
	afx_msg void OnBnClickedRef();
	afx_msg void OnLbnSelchangeListSrc();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedCheckEnablePrefixfilter();

	afx_msg void OnBnClickedNewlyonly();
	afx_msg void OnClickedEnLayer();
};
