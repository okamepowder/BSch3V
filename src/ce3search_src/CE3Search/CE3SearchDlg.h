
// CE3SearchDlg.h : header file
//

#pragma once

#include "coord.h"

class CCE3SearchDlg;

class SFindResult
{
public:
	CString m_docpath;
	CString m_strText;
	CString m_strType;
	SPoint  m_pt;
};

//typedef vector<SFindResult*> FindResultArray;
//typedef vector<SFindResult*>::iterator FindResultArrayIterator;


#define SEARCH_FIND_MSG (WM_APP+1)
#define SEARCH_END_MSG (WM_APP+2)
#define SCROLL_TO_MSG (WM_APP+100)

class FindInfo
{
public:
	CString m_str;
	BOOL	m_bMatchWhole;
	BOOL	m_bCaseSensitive;
	BOOL	m_bIncludeSubfolders;
	unsigned int m_targetFlag;
};

struct SearchThreadParam
{
	LPCTSTR pInitFolder;
	CCE3SearchDlg* pWnd;
};

// CCE3SearchDlg dialog
class CCE3SearchDlg : public CDialog
{
// Construction
public:
	CCE3SearchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CE3SEARCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
	
	CWinThread* m_pSearchThread;
	HANDLE m_hSearchThread;
	SearchThreadParam m_sparam;
	CString m_strSearchFolder;
	CImageList m_imageListSortMark;


	static BOOL CALLBACK EnumWindowsCheckSearchPath(HWND hwnd, LPARAM lParam);
	CWnd* CheckOtherCE3SearchPath(const TCHAR* pcszPathName);
	

	//int m_colWidth[3];



public:
	bool m_abort;
	bool m_searchEnd;

private:
	int m_nSortBy;
	bool m_bSortUp;

	int m_nSelected;


public:
	FindInfo m_fi;
	int m_pathColumnWidth;	//リストコントロールのレポートビューでのパスのカラムの幅

	void IniReadFindInfo(FindInfo& fi);
	void IniWriteFindInfo(FindInfo& fi);

	void setListColumnInfo();
	void initListCtrlFound();

	void setListColumnSortMark();

	//リストコントロールの内容のクリア
	void deleteListItems();


	//リストコントロールに検索結果を追加する。
	void addFindResult(SFindResult* pResult); 

	void doSort();

	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);
	
	static BOOL CALLBACK EnumWindowsProcCheckOpened(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProcID(HWND hwnd, LPARAM lParam);

	CWnd* CheckOpened(const TCHAR* pcszPathName);
	void OpenAndScrollTo(const TCHAR* pcszPathName, CPoint pt);



// Implementation
protected:
	HICON m_hIcon;


	//CString m_strInitialFolder;

	bool searchFolder(const TCHAR* folder);

	void disableControls(bool searching);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonRefFolder();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonOption();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnDestroy();
	afx_msg void OnHdnEndtrackListFound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickListFound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnSearchFind(UINT wParam,LONG lParam);
	afx_msg LRESULT OnSearchEnd(UINT wParam,LONG lParam);


	afx_msg void OnLvnItemchangedListFound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListFound(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};

