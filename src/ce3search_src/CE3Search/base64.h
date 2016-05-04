/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#ifndef BASE64_H
#define BASE64_H


// Base64のエンコードを行う。
bool EncodeBase64(const byte* src,int srcsize,TCHAR* dstbuff,int buffsize);
// Base64のデコードを行う。
void DecodeBase64(const TCHAR* src, byte* dstbuff,int buffsize,int* pDataSize);

#endif
