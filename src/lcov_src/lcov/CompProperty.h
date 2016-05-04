/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once


// CCompProperty ダイアログ

class CCompProperty : public CDialog
{
	DECLARE_DYNAMIC(CCompProperty)

public:
	CCompProperty(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCompProperty();

// ダイアログ データ
	enum { IDD = IDD_DLG_PROP_COMP };

	CString m_name;
	CString m_num;
	int m_block;
	int m_x;
	int m_y;
	BOOL m_usePtn;
	BOOL m_noBitPtn;
	BOOL m_useFixedBlockNum;
	int m_fixedBlockNum;
	BOOL m_sharedBlock;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	BOOL isValidPartName(const TCHAR* psz);
	void setBlockDlgItems();
	BOOL storeFixedblocknumState();
	BOOL storeCheckSharedBlock();


public:
	CString m_note;
	CString m_mfr;
	CString m_mfrpn;
	CString m_pkg;

//	afx_msg void OnEnChangeEditNote();
	afx_msg void OnBnClickedCheckUseptn();
	afx_msg void OnClickedCheckFixedblocknum();
	afx_msg void OnClickedCheckSharedBlock();
};
