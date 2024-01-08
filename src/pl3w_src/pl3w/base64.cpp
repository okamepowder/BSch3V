#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string>
#include <ctype.h>
#include "base64.h"



char EncodeBase64_char(unsigned data)
{
	data &= 63;

	if(data<=25){
		return 'A'+data;
	}else if(26<=data && data<=51){
		return 'a'+data-26;
	}else if(52<=data && data<=61){
		return '0'+data-52;
	}else if(data==62){
		return '!';
	}else{// if(data==63){
		return '/';
	}
}


// 3バイト単位のBase64のエンコードを行う。
// dstbuffは4バイト以上のサイズを持つこと
void EncodeBase64_block(const byte* srcbuff,int blocksize,char* dstbuff)
{

	byte src[3];
	int i;

	for(i=0;i<3&&i<blocksize;i++){
		src[i]=*srcbuff++;
	}
	while(i<3){
		src[i]=0;
		i++;
	}

	unsigned data;

	data = (((unsigned)(src[0]))>>2)&0x3F;
	dstbuff[0]= EncodeBase64_char(data);
	data = (((((unsigned)(src[0]))<<8) +((unsigned)(src[1])))>>4)&0x3F;
	dstbuff[1]= EncodeBase64_char(data);
	data = (((((unsigned)(src[1]))<<8) +((unsigned)(src[2])))>>6)&0x3F;
	dstbuff[2]= EncodeBase64_char(data);
	data = ((unsigned)(src[2]))&0x3F;
	dstbuff[3]= EncodeBase64_char(data);

	if(blocksize>=3){
		return;
	}else if(blocksize==2){
		dstbuff[3]= '=';
	}else if(blocksize==1){
		dstbuff[2]= '=';
		dstbuff[3]= '=';
	}else{
		dstbuff[0]= '=';
		dstbuff[1]= '=';
		dstbuff[2]= '=';
		dstbuff[3]= '=';
	}
}


// Base64のエンコードを行う。
bool EncodeBase64(const byte* src,int srcsize,char* dstbuff,int buffsize)
{
	char* dst = dstbuff;
	int buffRemain = buffsize;
	while(srcsize>0 && buffRemain>4){
		EncodeBase64_block(src,srcsize,dst);
		src+=3;
		srcsize-=3;
		dst+=4;
		buffRemain-=4;
	}
	*dst = '\0';
	return (srcsize<=0);
}



unsigned DecodeBase64_char(char c)
{
	if('A'<=c && c<='Z'){
		return (unsigned)c-'A';
	}else if('a'<=c && c<='z'){
		return (unsigned)c-'a'+26;
	}else if('0'<=c && c<='9'){
		return (unsigned)c-'0'+52;
	}else if(c=='!'){
		return 62;
	}else if(c=='/'){
		return 63;
	}else{
		return 64;
	}
}
	

// 4文字単位のBase64のデコードを行う。
// dstbuffは3バイト以上のサイズを持つこと
void DecodeBase64_block(const char* srcbuff,byte* dstbuff,int* pDataSize)
{
	*pDataSize = 0;
	unsigned data1,data2;
	if(srcbuff[0]=='\0'||srcbuff[1]=='\0'){
		return;
	}else{
		data1 = DecodeBase64_char(srcbuff[0]);
		data2 = DecodeBase64_char(srcbuff[1]);
		if(data1>63 || data2>63)return; 
		*dstbuff = (data1<<2)|(data2>>4);
		(*pDataSize)++;
		dstbuff++;
	}
	if(srcbuff[2]=='\0'){
		return;
	}else{
		data1 = DecodeBase64_char(srcbuff[2]);
		if(data1>63)return; 
		*dstbuff = ((data2&0xF)<<4)|(data1>>2);
		(*pDataSize)++;
		dstbuff++;
	}
	if(srcbuff[3]=='\0'){
		return;
	}else{
		data2 = DecodeBase64_char(srcbuff[3]);
		if(data2>63)return; 
		*dstbuff = ((data1&0x3)<<6)|(data2);
		(*pDataSize)++;
		dstbuff++;
	}
	return;
}


// Base64のデコードを行う。
void DecodeBase64(const char* src, byte* dstbuff,int buffsize,int* pDataSize)
{
	*pDataSize = 0;
	int blocksize;
	int buffremain = buffsize;
	while(buffremain>=3){
		DecodeBase64_block(src,dstbuff,&blocksize);
		*pDataSize+=blocksize;
		if(blocksize<3) return;
		src+=4;
		dstbuff+=3;
		buffremain-=3;
	}
}
