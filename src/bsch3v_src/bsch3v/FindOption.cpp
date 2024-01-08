// FindOption.cpp : 実装ファイル
//

#include "stdafx.h"
#include "FindOption.h"


// CFindOption ダイアログ

IMPLEMENT_DYNAMIC(CFindOption, CDialog)

CFindOption::CFindOption(CWnd* pParent /*=NULL*/)
	: CDialog(CFindOption::IDD, pParent)
	, m_bFindComment(FALSE)
	, m_bFindLabel(FALSE)
	, m_bFindName(FALSE)
	, m_bFindNameLib(FALSE)
	, m_bFindManufacture(FALSE)
	, m_bFindManufacturePartNumber(FALSE)
	, m_bFindPackage(FALSE)
	, m_bFindNote(FALSE)
	, m_bFindNum(FALSE)
	, m_bFindTag(FALSE)
	, m_bMatchWhole(FALSE)
	, m_bCaseSensitive(FALSE)
{

}

CFindOption::~CFindOption()
{
}

void CFindOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_FIND_COMMENT, m_bFindComment);
	DDX_Check(pDX, IDC_FIND_LABEL, m_bFindLabel);
	DDX_Check(pDX, IDC_FIND_NAME, m_bFindName);
	DDX_Check(pDX, IDC_FIND_NAME_LIB, m_bFindNameLib);
	DDX_Check(pDX, IDC_FIND_MANUFACTURE, m_bFindManufacture);
	DDX_Check(pDX, IDC_FIND_MANUFACTUREPN, m_bFindManufacturePartNumber);
	DDX_Check(pDX, IDC_FIND_PACKAGE, m_bFindPackage);
	DDX_Check(pDX, IDC_FIND_NOTE, m_bFindNote);
	DDX_Check(pDX, IDC_FIND_NUM, m_bFindNum);
	DDX_Check(pDX, IDC_FIND_TAG, m_bFindTag);
	DDX_Check(pDX, IDC_FIND_MATCHWHOLE, m_bMatchWhole);
	DDX_Check(pDX, IDC_FIND_CASE_SENSITIVE, m_bCaseSensitive);
}


BEGIN_MESSAGE_MAP(CFindOption, CDialog)
END_MESSAGE_MAP()


// CFindOption メッセージ ハンドラ
