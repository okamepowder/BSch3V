// SetTagFrame.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "BSch.h"
#include "SetTagFrame.h"


// CSetTagFrame �_�C�A���O

IMPLEMENT_DYNAMIC(CSetTagFrame, CDialog)

CSetTagFrame::CSetTagFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTagFrame::IDD, pParent)
	, m_nType(0)
{

}

CSetTagFrame::~CSetTagFrame()
{
}

void CSetTagFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TAG_SQ, m_nType);
}


BEGIN_MESSAGE_MAP(CSetTagFrame, CDialog)
END_MESSAGE_MAP()


// CSetTagFrame ���b�Z�[�W �n���h��
