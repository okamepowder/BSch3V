#pragma once


// CImageButton

class CImageButton : public CButton
{
	DECLARE_DYNAMIC(CImageButton)

public:
	CImageButton();
	virtual ~CImageButton();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	int m_nCheck;
	CImageList* m_pImageList;
	int m_nImageIndex;

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetCheck(int nCheck);
	void SetImage(CImageList* pImageList,int index);
protected:
	//virtual void PreSubclassWindow();

};


