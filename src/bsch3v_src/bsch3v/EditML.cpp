// EditML.cpp : �����t�@�C��
//

#include "stdafx.h"
//#include "BSch3V.h"
#include "EditML.h"
#include ".\editml.h"


// CEditML

IMPLEMENT_DYNAMIC(CEditML, CEdit)
CEditML::CEditML()
{
}

CEditML::~CEditML()
{
}


BEGIN_MESSAGE_MAP(CEditML, CEdit)
	ON_WM_CHAR()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CEditML ���b�Z�[�W �n���h��


void CEditML::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	int n = ::GetKeyState(VK_SHIFT);
	TRACE("OnChar %04X, %d, %04X %04x\n",nChar, nRepCnt, nFlags, n);

	if(nChar == 0x0D && n & 0x8000){
		GetParent()->SendMessage(WM_COMMAND,IDOK,MAKELONG(0,BN_CLICKED));
	}else{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	

}


