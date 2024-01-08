/****************************************************************************
    BSch3V schematic capture
   Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include <stdio.h>
//#include <qnamespace.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "imagedata.h"
#include "base64.h"
#ifdef _BSCHCAPTURE
	#include "Global.h"
#endif

#include <atlimage.h>	


SImageData::SImageData(void)
{
	m_refCount = 0;
}

SImageData::~SImageData(void)
{
}




SImageDIB::SImageDIB(void)
{
	m_pbuff = NULL;
	m_pBmpFH = NULL;
	m_pBmpIH = NULL;
	m_pBmpInfo = NULL;
	m_pBitData = NULL;
}

SImageDIB::~SImageDIB(void)
{
	delete [] m_pbuff;
}

int SImageDIB::ImageWidth()
{
	if(m_pBmpIH == NULL) return 0;
	return m_pBmpIH->biWidth;
	
}

int SImageDIB::ImageHeight()
{
	if(m_pBmpIH == NULL) return 0;
	return m_pBmpIH->biHeight;
}


//DIBバッファの構造体ポインタの設定と妥当性のチェック
//さしあたり、縦横サイズが0以下でないことを確認  2009/05/30
bool SImageDIB::checkDIB()
{
	m_pBmpFH	= (BITMAPFILEHEADER*)m_pbuff;
	m_pBmpIH	= (BITMAPINFOHEADER*)(m_pbuff + sizeof(BITMAPFILEHEADER));
	m_pBmpInfo	= (BITMAPINFO*)m_pBmpIH;
	m_pBitData	= m_pbuff + m_pBmpFH->bfOffBits;

	if(m_pBmpFH->bfType!=0x4D42 || ImageWidth()<=0 || ImageHeight()<=0){
		m_pBmpFH	= NULL;
		m_pBmpIH	= NULL;
		m_pBmpInfo	= NULL;
		m_pBitData	= NULL;
		return false;
	}
	return true;
}


bool SImageDIB::Save(SWriteCE3& wce3)
{
	if(m_pbuff==NULL) return false;
	int dataSize = m_pBmpFH->bfSize;
	int dataRemain = dataSize;
	byte* pBuff =m_pbuff;
	char str[128];
	wce3.WriteRecord("+IMAGE_DIB");	wce3.WriteEOL();
	while(dataRemain>0){
		int writeSize=(dataRemain<57 ? dataRemain : 57);
		EncodeBase64(pBuff,writeSize,str,100);
		dataRemain-=writeSize;
		pBuff+=writeSize;
		wce3.WriteRecord(str);	wce3.WriteEOL();
	}
	wce3.WriteRecord("-IMAGE_DIB");	wce3.WriteEOL();
	return true;
}

#define IMAGETYPE_JPEG 1
#define IMAGETYPE_PNG  2



bool SImageDIB::LoadWithGdiPlus(const char* fname,int imagetype)
{
	FILE* fp = fopen(fname,"rb");
	if(!fp)return false;
	fseek(fp,0,SEEK_END);
	//
	fpos_t filesize = 0;
	fgetpos(fp,&filesize);
	if(filesize > MAX_IMAGEBUFF_SIZE){
		fclose(fp);
		return false;
	}
	
	fseek(fp,0,SEEK_SET);

	m_pbuff = new byte[(unsigned)filesize+54];
	int readsize = fread(m_pbuff+54,1,(size_t)filesize,fp);
	if(readsize!=filesize){
		delete[]m_pbuff;
		m_pbuff = NULL;
		fclose(fp);
		return false;
	}
	fclose(fp);

	CImage image;

	CComPtr<IStream> pStream;
	pStream = NULL;

	HGLOBAL hMem = ::GlobalAlloc(GHND, filesize); 
	LPVOID pGBuff = ::GlobalLock(hMem);
	memcpy(pGBuff, m_pbuff+54, filesize);
	::GlobalUnlock(hMem);

	CreateStreamOnHGlobal(hMem, TRUE, &pStream);

	HRESULT res = image.Load(pStream);

	if(FAILED(res)){
		image.Destroy();
		::GlobalFree(hMem);
		delete[]m_pbuff;
		m_pbuff = NULL;
		return false;
	}

	for(int i=0;i<54;i++) m_pbuff[i]=0;


	m_pBmpFH	= (BITMAPFILEHEADER*)m_pbuff;
	m_pBmpIH	= (BITMAPINFOHEADER*)(m_pbuff + sizeof(BITMAPFILEHEADER));
	m_pBmpInfo	= (BITMAPINFO*)m_pBmpIH;
	m_pBitData	= m_pbuff + 54;

	m_pBmpFH->bfSize = filesize+54;
	m_pBmpFH->bfType = 0x4D42;
	m_pBmpFH->bfOffBits = 54;

	m_pBmpIH->biSize = 40;
	m_pBmpIH->biBitCount = 0;
	m_pBmpIH->biCompression = (imagetype==IMAGETYPE_JPEG ? BI_JPEG : BI_PNG);
	m_pBmpIH->biPlanes = 1;
	m_pBmpIH->biHeight = image.GetHeight();
	m_pBmpIH->biWidth = image.GetWidth();
	m_pBmpIH->biSizeImage = filesize;

	image.Destroy();
	::GlobalFree(hMem);
	
	return true;
}


bool SImageDIB::LoadWindowsBmp(const char* fname)
{
	FILE* fp = fopen(fname,"rb");
	if(!fp)return false;
	fseek(fp,0,SEEK_END);
	//
	fpos_t filesize = 0;
	fgetpos(fp,&filesize);
	if(filesize > MAX_IMAGEBUFF_SIZE){
		fclose(fp);
		return false;
	}
	
	fseek(fp,0,SEEK_SET);

	m_pbuff = new byte[(unsigned)filesize];
	int readsize = fread(m_pbuff,1,(size_t)filesize,fp);
	if(readsize!=filesize){
		delete[]m_pbuff;
		m_pbuff = NULL;
		fclose(fp);
		return false;
	}
	fclose(fp);
	if(!checkDIB()){
		delete[]m_pbuff;
		m_pbuff = NULL;
	}
	return true;
}




//DIBファイルからの読み込み
bool SImageDIB::Load(const char* fname)
{
	delete[]m_pbuff;
	m_pbuff = NULL;
#ifdef _BSCHCAPTURE
	char szExtension[_MAX_EXT];
	::_splitpath(fname,NULL,NULL,NULL,szExtension);
	if(stricmp(szExtension,".bmp")==0){
		return LoadWindowsBmp(fname);
	}else{
		if(g_bAvailableGDIplus){
			if((stricmp(szExtension,".jpg")==0 || stricmp(szExtension,".jpeg")==0)){
				return LoadWithGdiPlus(fname,IMAGETYPE_JPEG);
			}else if(stricmp(szExtension,".png")==0){
				return LoadWithGdiPlus(fname,IMAGETYPE_PNG);
			}
		}
		return false;
	}
#else
	return false;
#endif
}


bool SImageDIB::Load(SReadCE3& rce3)
{
	string strbuff;

	byte tempBuff[1024];
	int readDataSize = 0;
	byte* pbuff = tempBuff;
	bool useTempBuff=true;
	int buffRemain = 1024;

	int dibDataSize = 1024;
	int dibBuffSize = 0;

	delete[]m_pbuff;
	m_pbuff = NULL;

	string str("");
	while(1){
		if(rce3.ReadRecord(str)==EOF) return false;
		if(str[0] == '-'){
			if(str != "-IMAGE_DIB"){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==EOF) return false;
		}else{
			int l=str.length();
			int recordDatasize;
			DecodeBase64(str.c_str(),pbuff, buffRemain,&recordDatasize);
			readDataSize	+=recordDatasize;
			pbuff			+=recordDatasize;
			buffRemain		-=recordDatasize;
			//最初のヘッダー部分を読み出したら
			//妥当性(先頭のBMとデータサイズ)を確認ののち、データバッファを確保してtempBuffからコピーする
			if(useTempBuff && readDataSize > sizeof(BITMAPFILEHEADER)){
				if(((BITMAPFILEHEADER*)(tempBuff))->bfType != 0x4D42){
					if(rce3.SkipTo("-IMAGE_DIB")==EOF) return false;
					break;
				}
				dibDataSize = ((BITMAPFILEHEADER*)(tempBuff))->bfSize;
				if(dibDataSize>MAX_IMAGEBUFF_SIZE){
					if(rce3.SkipTo("-IMAGE_DIB")==EOF) return false;
					break;
				}
				dibBuffSize = dibDataSize+256;
				m_pbuff = new byte[dibBuffSize];
				pbuff = m_pbuff;
				for(int i=0;i<readDataSize;i++){
					*pbuff++ = tempBuff[i];
				}
				buffRemain = dibBuffSize-readDataSize;
				useTempBuff = false;
			}
			if(readDataSize>=dibDataSize){
				if(rce3.SkipTo("-IMAGE_DIB")==EOF) return false;
				break;
			}
		}
	}
	if(m_pbuff!=NULL){
		if(readDataSize!=dibDataSize){
			delete[]m_pbuff;
			m_pbuff = NULL;
			return false;
		}
		if(!checkDIB()){
			delete[]m_pbuff;
			m_pbuff = NULL;
			return false;
		}
	}
	return true;
}



