/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
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
	virtual bool Load(const TCHAR* fname)=0;
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
	bool Load(const TCHAR* fname);

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

	bool LoadWindowsBmp(const TCHAR* fname);
	bool LoadWithGdiPlus(const TCHAR* fname,int imagetype);


};

#endif

