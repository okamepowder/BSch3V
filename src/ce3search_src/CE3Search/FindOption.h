#pragma once

#include "resource.h"

// CFindOption �_�C�A���O

class CFindOption : public CDialog
{
	DECLARE_DYNAMIC(CFindOption)

public:
	CFindOption(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CFindOption();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FIND2_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:

	BOOL m_bFindComment;
	BOOL m_bFindLabel;
	BOOL m_bFindName;
	BOOL m_bFindNameLib;
	BOOL m_bFindManufacture;
	BOOL m_bFindManufacturePartNumber;
	BOOL m_bFindPackage;
	BOOL m_bFindNote;

	BOOL m_bFindNum;
	BOOL m_bFindTag;
	BOOL m_bMatchWhole;
	BOOL m_bCaseSensitive;
	BOOL m_bIncludeSubfolders;
};
