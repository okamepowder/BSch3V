/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#ifndef BASE64_H
#define BASE64_H


// Base64�̃G���R�[�h���s���B
bool EncodeBase64(const byte* src,int srcsize,char* dstbuff,int buffsize);
// Base64�̃f�R�[�h���s���B
void DecodeBase64(const char* src, byte* dstbuff,int buffsize,int* pDataSize);

#endif
