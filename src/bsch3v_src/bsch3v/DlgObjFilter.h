#pragma once
#include "resource.h"
#include "ImageButton.h"



// CDlgObjFilter ダイアログ

class CDlgObjFilter : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjFilter)

public:
	CDlgObjFilter(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgObjFilter();

// ダイアログ データ
	enum { IDD = IDD_OBJECTFILTER };

	unsigned* m_pObjectFilterValue;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	CImageList m_imageListButton;

	int m_xpos;
	int m_ypos;


	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	void ShowDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnDestroy();
	//CImageButton m_buttonCheck1;
	CImageButton m_buttonComponent;
	CImageButton m_buttonBus;
	CImageButton m_buttonWire;
	CImageButton m_buttonDash;
	CImageButton m_buttonMarker;
	CImageButton m_buttonJunction;
	CImageButton m_buttonBEntry;
	CImageButton m_buttonWEntry;
	CImageButton m_buttonTag;
	CImageButton m_buttonLabel;
	CImageButton m_buttonComment;
	CImageButton m_buttonImage;

	afx_msg void OnBnClickedComponent();
	afx_msg void OnBnClickedBus();
	afx_msg void OnBnClickedWire();
	afx_msg void OnBnClickedDash();
	afx_msg void OnBnClickedMarker();
	afx_msg void OnBnClickedJunction();
	afx_msg void OnBnClickedBEntry();
	afx_msg void OnBnClickedWEntry();
	afx_msg void OnBnClickedTag();
	afx_msg void OnBnClickedLabel();
	afx_msg void OnBnClickedComment();
	afx_msg void OnBnClickedImage();
};
