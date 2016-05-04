#pragma once

#include "bsch.h"

// CDispBlockNum ダイアログ

class CDispBlockNum : public CDialog
{
	DECLARE_DYNAMIC(CDispBlockNum)

public:
	CDispBlockNum(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDispBlockNum();

public:
	DISP_BLOCKNUM_TYPE m_dispBN;



// ダイアログ データ
	enum { IDD = IDD_DIALOG_DISPBLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};
