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
//CBSchDocのうち、ドラッグのための選択に関わるプライベート関数の動作を定義
//


#include "stdafx.h"
//#include <memory.h>
#include "BSch.h"



#include "BSchObj.h"	//図面オブジェクト基底クラス
#include "BSchJunc.h"	//	図面オブジェクト	接合点
#include "BSchEntr.h"	//	図面オブジェクト	エントリー
#include "BSchLine.h"	//	図面オブジェクト	線
#include "BSchWire.h"	//	図面オブジェクト	ワイヤー
#include "BSchBus.h"	//	図面オブジェクト	バス線
#include "BSchDash.h"	//	図面オブジェクト	破線
#include "BSchTag.h"	//	図面オブジェクト	タグ
#include "BSchLbl.h"	//	図面オブジェクト	ラベル
#include "BSchCmnt.h"	//	図面オブジェクト	コメント

#include "BSchDoc.h"

#include "PartLib.h"
#include "BSchFont.h"
#include "Global.h"

#include "ReadERC.h"
#include "BSchPart.h"	//	図面オブジェクト	部品


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


