#pragma once
#include "resource.h"
#include <assert.h>
#include <string>
#include <list>
#include <vector>
#include <imm.h>
using namespace std;
#include "BSch.h"
#include "BSchDoc.h"
#include "coord2.h"
#include "BSchView.h"


class SFindResult
{
public:
	CString m_strText;
	CString m_strType;
	SPoint  m_pt;
};

typedef vector<SFindResult*> FindResultArray;
typedef vector<SFindResult*>::iterator FindResultArrayIterator;


// CFind2 ダイアログ

class CFind2 : public CDialog
{
	DECLARE_DYNAMIC(CFind2)

public:
	CFind2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CFind2();

	void ShowDialog();

public:
	CBSchView* m_pView;

// ダイアログ データ
	enum { IDD = IDD_FIND2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

protected:
	int m_xpos;
	int m_ypos;
	int m_cx;
	int m_cy;
	int m_mincx;
	int m_mincy;

	int m_colWidth[2];


	FindInfo m_fi;
	CImageList m_imageListSortMark;

	CString m_str_refnum;
	CString m_str_value;
	CString m_str_mfr;
	CString m_str_mfrpn;
	CString m_str_pkg;
	CString m_str_note;
	CString m_str_tag;
	CString m_str_label;
	CString m_str_comment;
	CString m_str_nameinlib;


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void initListCtrlFound();
	void deleteListItems();
	void setListColumnInfo();
	void setListColumnSortMark();
	void doSort();
	void setCtrlSize();
	LPTSTR typeString(unsigned int type);
	int m_nSortBy;
	int m_bSortUp;
	//FindResultArray m_resultArray;
	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);
	int m_nSelected;


protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonOption();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnLvnColumnclickListFound(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnOdstatechangedListFound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListFound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
