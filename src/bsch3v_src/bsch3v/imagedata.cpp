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
	TCHAR str[128];
	wce3.WriteRecord(_T("+IMAGE_DIB"));	wce3.WriteEOL();
	while(dataRemain>0){
		int writeSize=(dataRemain<57 ? dataRemain : 57);
		EncodeBase64(pBuff,writeSize,str,100);
		dataRemain-=writeSize;
		pBuff+=writeSize;
		wce3.WriteRecord(str);	wce3.WriteEOL();
	}
	wce3.WriteRecord(_T("-IMAGE_DIB"));	wce3.WriteEOL();
	return true;
}

#define IMAGETYPE_JPEG 1
#define IMAGETYPE_PNG  2



bool SImageDIB::LoadWithGdiPlus(const TCHAR* fname,int imagetype)
{
	FILE* fp = _tfopen(fname,_T("rb"));
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


bool SImageDIB::LoadWindowsBmp(const TCHAR* fname)
{
	FILE* fp = _tfopen(fname,_T("rb"));
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

//クリップボードからのロード
bool  SImageDIB::LoadWindowsClipboardDIB(byte* buff, int clipBoardDataSize)
{
	BITMAPINFOHEADER* pBitmapInfoHeader = (BITMAPINFOHEADER*)buff;	//クリップボードのCF_DIBの先頭は、BITMAPINFOHEADER
	int headerSize =sizeof(BITMAPINFOHEADER);
	int bytesPerLine;
	int imageSize;
	int buffSize;

	//ヘッダー部分のチェック
	if (pBitmapInfoHeader->biSize != sizeof(BITMAPINFOHEADER)) return false;
	if (pBitmapInfoHeader->biPlanes != 1) return false;
	if (pBitmapInfoHeader->biBitCount <16) return false;	//16ビットカラー以上をサポート
	if (pBitmapInfoHeader->biWidth < 1) return false;		//幅、高さゼロ以下は不正
	if (pBitmapInfoHeader->biHeight < 1) return false;
	switch (pBitmapInfoHeader->biCompression) {
	case BI_RGB:
		break;
	case BI_BITFIELDS:
		headerSize += 4;	//ヘッダー末尾にRGBQUAD領域がくっついて4バイト増える。
		break;
	default:
		return false;		//とりあえず非圧縮だけサポート
	}

	{
		int bytesPerPixel = (pBitmapInfoHeader->biBitCount + 7) / 8;	//1ピクセルあたりのバイト数
		bytesPerLine = ((bytesPerPixel * pBitmapInfoHeader->biWidth + 3) / 4) * 4;	//1ラインのバイト数は4バイトアライン
	}
	imageSize = bytesPerLine * pBitmapInfoHeader->biHeight;
	
	int dibDataSize = headerSize + imageSize;
	if (dibDataSize > clipBoardDataSize)return false;	//ヘッダーとイメージデータの合計サイズがクリップボードのデータサイズより大きいのはおかしい。
	buffSize = dibDataSize + sizeof(BITMAPFILEHEADER);
	if (buffSize > MAX_IMAGEBUFF_SIZE) return false;

	m_pbuff = new byte[(unsigned)buffSize];

	for (int i = 0; i < sizeof(BITMAPFILEHEADER); i++) m_pbuff[i] = 0;

	m_pBmpFH = (BITMAPFILEHEADER*)m_pbuff;
	m_pBmpIH = (BITMAPINFOHEADER*)(m_pbuff + sizeof(BITMAPFILEHEADER));
	m_pBmpInfo = (BITMAPINFO*)m_pBmpIH;
	m_pBitData = m_pbuff + sizeof(BITMAPFILEHEADER) + headerSize;

	m_pBmpFH->bfSize = buffSize;
	m_pBmpFH->bfType = 0x4D42;
	m_pBmpFH->bfOffBits = sizeof(BITMAPFILEHEADER) + headerSize;

	byte* pSrc = buff;
	byte* pDst = (byte*)m_pBmpIH;
	for (int i = 0; i < dibDataSize; i++) {
		*pDst++ = *pSrc++;
	}
	return true;

}


//DIBファイルからの読み込み
bool SImageDIB::Load(const TCHAR* fname)
{
	delete[]m_pbuff;
	m_pbuff = NULL;
#ifdef _BSCHCAPTURE
	TCHAR szExtension[_MAX_EXT];
	::_tsplitpath(fname,NULL,NULL,NULL,szExtension);
	if(_tcsicmp(szExtension,_T(".bmp"))==0){
		return LoadWindowsBmp(fname);
	}else{
		if(g_bAvailableGDIplus){
			if((_tcsicmp(szExtension,_T(".jpg"))==0 || _tcsicmp(szExtension,_T(".jpeg"))==0)){
				return LoadWithGdiPlus(fname,IMAGETYPE_JPEG);
			}else if(_tcsicmp(szExtension,_T(".png"))==0){
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

	wstring str(_T(""));
	while(1){
		//if(readDataSize==30096){
		//	TRACE(_T("readDataSize==30096\n"));
		//}
		if(rce3.ReadRecord(str)==WEOF){
			return false;
		}
		if(str[0] == '-'){
			if(str != _T("-IMAGE_DIB")){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==WEOF){
				return false;
			}
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
					if(rce3.SkipTo(_T("-IMAGE_DIB"))==WEOF){
						return false;
					}
					break;
				}
				dibDataSize = ((BITMAPFILEHEADER*)(tempBuff))->bfSize;
				if(dibDataSize>MAX_IMAGEBUFF_SIZE){
					if(rce3.SkipTo(_T("-IMAGE_DIB"))==WEOF){
						return false;
					}
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
				if(rce3.SkipTo(_T("-IMAGE_DIB"))==WEOF){
					return false;
				}
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



