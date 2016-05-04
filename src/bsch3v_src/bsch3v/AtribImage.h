#pragma once


// CAtribImage ダイアログ

class CAtribImage : public CDialog
{
	DECLARE_DYNAMIC(CAtribImage)

public:
	CAtribImage(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAtribImage();

// ダイアログ データ
	enum { IDD = IDD_IMAGE_ATRB };

	int m_magnification;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
