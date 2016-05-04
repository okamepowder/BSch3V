/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2005 H.Okada

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

//  Drag.cpp
//CBSchDoc�̂����A�h���b�O�̂��߂̑I���Ɋւ��v���C�x�[�g�֐��̓�����`
//


#include "stdafx.h"
//#include <memory.h>
#include "BSch.h"



#include "BSchObj.h"	//�}�ʃI�u�W�F�N�g���N���X
#include "BSchJunc.h"	//	�}�ʃI�u�W�F�N�g	�ڍ��_
#include "BSchEntr.h"	//	�}�ʃI�u�W�F�N�g	�G���g���[
#include "BSchLine.h"	//	�}�ʃI�u�W�F�N�g	��
#include "BSchWire.h"	//	�}�ʃI�u�W�F�N�g	���C���[
#include "BSchBus.h"	//	�}�ʃI�u�W�F�N�g	�o�X��
#include "BSchDash.h"	//	�}�ʃI�u�W�F�N�g	�j��
#include "BSchTag.h"	//	�}�ʃI�u�W�F�N�g	�^�O
#include "BSchLbl.h"	//	�}�ʃI�u�W�F�N�g	���x��
#include "BSchCmnt.h"	//	�}�ʃI�u�W�F�N�g	�R�����g

#include "BSchDoc.h"

#include "PartLib.h"
#include "BSchFont.h"
#include "Global.h"

#include "ReadERC.h"
#include "BSchPart.h"	//	�}�ʃI�u�W�F�N�g	���i


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


