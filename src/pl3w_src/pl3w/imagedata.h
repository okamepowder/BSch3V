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

/****************************************************************************
** 図面に挿入されるイメージデータの実体のクラス
** BSch3Vの中では、挿入されたイメージデータそのものを改変することはない。
** そのため、イメージデータの実体はUNDO、REDOを含めたBSch3Vのイメージデータ
** オブジェクトから共通で参照される。
** 参照カウンタを持ち、残り1のときにDeleteされたらこの実体もdeleteする。
****************************************************************************/
#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "xbsch.h"

class SImageData
{
public:
	enum{
		IMAGEOBJECT_DIB=0
	};

	enum{
		MAX_IMAGEBUFF_SIZE=4000000
	};


	SImageData(void);
	virtual ~SImageData(void);

	int m_refCount;

//protected:
//	int m_drawWidth;
//	int m_drawHeight;

public:
//	int DrawWidth(){return m_drawWidth;}
//	int DrawHeight(){return m_drawHeight;}
//	virtual void SetDrawWidth(int n){if(n<1)n=1; m_drawWidth=n;}
//	virtual void SetDrawHeight(int n){if(n<1)n=1; m_drawHeight=n;}
	
	virtual int ImageWidth()=0;
	virtual int ImageHeight()=0;
	
	virtual unsigned ImageType()=0;
	virtual bool Load(const char* fname)=0;
	virtual bool Load(SReadCE3& rce3)=0;
	virtual bool Save(SWriteCE3& wce3)=0;
};


class SImageDIB:public SImageData
{
public:
	SImageDIB(void);
	virtual ~SImageDIB(void);

	int ImageWidth();
	int ImageHeight();
	
	unsigned ImageType(){return IMAGEOBJECT_DIB;}

	//DIBのファイルからの読み込み
	bool Load(const char* fname);

	bool Load(SReadCE3& rce3);
	bool Save(SWriteCE3& wce3);

public:	
	byte* m_pbuff;
	BITMAPFILEHEADER* 	m_pBmpFH;
	BITMAPINFOHEADER* 	m_pBmpIH;
	BITMAPINFO*			m_pBmpInfo;
	byte*				m_pBitData;

protected:
	bool checkDIB();

	bool LoadWindowsBmp(const char* fname);
	bool LoadWithGdiPlus(const char* fname,int imagetype);


};

#endif

