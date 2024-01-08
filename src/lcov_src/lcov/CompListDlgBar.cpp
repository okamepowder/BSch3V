/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2005 H.Okada (http://www.suigyodo.com/online)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*****************************************************************************/

// CompListDlgBar.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LCoV.h"
#include "CompListDlgBar.h"



// SCompListDlgBar

IMPLEMENT_DYNAMIC(SCompListDlgBar, CDialogBar)
SCompListDlgBar::SCompListDlgBar()
{
}

SCompListDlgBar::~SCompListDlgBar()
{
}


BEGIN_MESSAGE_MAP(SCompListDlgBar, CDialogBar)
END_MESSAGE_MAP()



// SCompListDlgBar ���b�Z�[�W �n���h��


void SCompListDlgBar::PreSubclassWindow()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	m_LBCompName.SubclassDlgItem(IDC_LIST_NAME,this);
	CDialogBar::PreSubclassWindow();
}
