#pragma once


// CSetTagFrame ダイアログ

class CSetTagFrame : public CDialog
{
	DECLARE_DYNAMIC(CSetTagFrame)

public:
	CSetTagFrame(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSetTagFrame();

// ダイアログ データ
	enum { IDD = IDD_TAG_FRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int m_nType;
};
