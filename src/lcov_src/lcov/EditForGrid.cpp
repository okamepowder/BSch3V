// EditForGrid.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LCoV.h"
#include "EditForGrid.h"
#include ".\editforgrid.h"


// CEditForGrid

IMPLEMENT_DYNAMIC(CEditForGrid, CEdit)
CEditForGrid::CEditForGrid()
{
}

CEditForGrid::~CEditForGrid()
{
}


BEGIN_MESSAGE_MAP(CEditForGrid, CEdit)
//	ON_WM_CHAR()
ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CEditForGrid ���b�Z�[�W �n���h��


//void CEditForGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
//
//	CEdit::OnChar(nChar, nRepCnt, nFlags);
//}

void CEditForGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if(nChar == VK_UP || nChar == VK_DOWN){
		GetParent()->SendMessage(WM_FIX_TEXT,(UINT)nChar);
	}else{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}
