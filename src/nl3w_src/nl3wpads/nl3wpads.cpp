// nl3wpads.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include <stdio.h>
#include "dlliftype.h"


void __stdcall typeName(TCHAR* buff,int buffsize)
{
	static const TCHAR* fmtname = L"PADS";	
	wcsncpy(buff,fmtname,buffsize-1);
	buff[buffsize-1] = '\0';
}


/////////////////////////////////////////////////////////////////////////////////////
//ERROR CODE
//successful: 0
//file error: -1
//no net data: -2
//user define error: =>1
int __stdcall fnWriteNetList(const TCHAR* filename, const SExportNetlistInfo* pNetInfo)
{
	if(filename == NULL) return -1;
	FILE* fp = _wfopen(filename,L"wb");
	if(fp==NULL) return -1;

	if(pNetInfo==NULL) return -2;


	fprintf(fp,"*PADS-PCB*\r\n");
	fprintf(fp,"*PART*\r\n");

	for(int i=0;i<pNetInfo->nComponentInfoCount;i++){
		fprintf(fp,"%s%d %s\r\n",
			pNetInfo->array_CompInfo[i].pszPrefix,
			pNetInfo->array_CompInfo[i].nSuffix,
			pNetInfo->array_CompInfo[i].pszPackage
		);
	}

	fprintf(fp,"*NET*\r\n");
	
	for(int i=0;i<pNetInfo->nNetValueCount;i++){
		fprintf(fp,"*SIGNAL* %s\r\n",pNetInfo->array_NetValue[i].pszNetName);
		int n=0;
		for(int j=0;j< pNetInfo->array_NetValue[i].nPinCount; j++){
			if(n>0){
				fprintf(fp,"  ");
			}
			fprintf(fp,"%s.%s",
				pNetInfo->array_NetValue[i].pArrayPinValue[j].pszRefNum,
				pNetInfo->array_NetValue[i].pArrayPinValue[j].pszPinNum);
			n++;

			if(n==5){
				fprintf(fp,"\r\n");
				n=0;
			}

		}
		if(n>0){
			fprintf(fp,"\r\n");
		}

	}
	fprintf(fp,"*END*\r\n");
	fclose(fp);
	return 0;
}



