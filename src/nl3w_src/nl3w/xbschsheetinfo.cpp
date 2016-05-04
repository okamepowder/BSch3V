/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面情報クラスの実装
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <string>
using namespace std;
#include "ce3io.h"

#include "xbschobj.h"
#include "SheetSizeInfo.h"
#include "xbschsheetinfo.h"



SXBSchSheetInfo::SXBSchSheetInfo()
{
	m_p1.setX(1500);
	m_p1.setY(1000);
	m_Selection=0xFF;
	m_Layer=0;

	m_totalPages = 1;
	m_page = 1;
	m_strProject = "";
	m_version = 0;
	m_initPos = INITPOS_LT;
}
	
void SXBSchSheetInfo::setInitPos(int n)
{
	switch(n){
		case INITPOS_LB:
		case INITPOS_RT:
		case INITPOS_RB:
			m_initPos = n;
			break;
		default:
			m_initPos = 0;
	};
}



bool SXBSchSheetInfo::readCe3(SReadCE3& rce3,const SPoint*,SXBSchDoc*)
{
	string str("");
	bool bInitW = false;
	bool bInitH = false;
	while(1){
		if(rce3.ReadRecord(str)==EOF) return false;
		if(str[0] == '-'){
			if((str != "-SHEETINFO") || !bInitW || !bInitH){
				return false;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==EOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<l){	// : で分けられたレコードである
				string var = str.substr(0,n);				//先頭から:の手前まで
				if(var=="PROJ"){
					m_strProject = str.substr(n+1);
				}else{
					int nParam = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
	//#ifdef _MFCBSCH	//Qt-BSch doesn't support layer.
					if(var=="EL"){
						if(isValidLayer(nParam)){
							m_Layer	= nParam;			
						}
					}else
					if(var=="VL"){
						nParam &=0xFF;
						if(nParam==0) nParam = 0xFF;
						m_Selection = nParam;
					}else
	//#endif				
					if(var=="W"){
						if(nParam>=SHEETSIZE_X_MIN && nParam <=SHEETSIZE_X_MAX) m_p1.setX(nParam);
						bInitW = true;
					}else
					if(var=="H"){
						if(nParam>=SHEETSIZE_Y_MIN && nParam <=SHEETSIZE_Y_MAX) m_p1.setY(nParam);
						bInitH = true;
					}else
					if(var=="PAGES"){
						m_totalPages=nParam;
					}else
					if(var=="PAGE"){
						m_page=nParam;
					}else
					if(var=="VER"){
						m_version=nParam;
					}else
					if(var=="INITPOS"){
						setInitPos(nParam);
					}
						
				}
			}
		}
	}
	return true;
}
/*
//ストリームからの読み込み ～ -SHEETINFO
bool SXBSchSheetInfo::readStream(SCsvIStream* pStrm,const SPoint*)
{
	if(!pStrm) return false;
	QString str;
	int nResult;
	int nParam;
	bool bInitW = false;
	bool bInitH = false;
	while(1){
		nResult = pStrm->getRecord(str);
		if(nResult == SCsvIStream::GETRECORD_EOF || nResult == SCsvIStream::GETRECORD_ERROR){
			return false;
		}
		//str = deleteLeftSpace(str);
		if(str[0] == '-'){
			if((str != "-SHEETINFO") || !bInitW || !bInitH){
				return false;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			if(pStrm->skipTo(str.replace(1,1,"-")) != SCsvIStream::GETRECORD_NORMAL)return false;
		}else{
			QChar cc = str[0];
			switch(cc){
			case 'W':
				if(str[1] != ':') break;
				nParam = (str.mid(2)).toInt();
				m_p1.setX(nParam);
				bInitW = true;
				break;
			case 'H':
				if(str[1] != ':') break;
				nParam = (str.mid(2)).toInt();
				m_p1.setX(nParam);
				bInitH = true;
				break;
			default:
				break;
			}
		}
	}
	return true;
}			
*/

bool SXBSchSheetInfo::writeCe3(SWriteCE3& wce3,const SPoint* )
{
	//char sz[32];
	wce3.WriteRecord("+SHEETINFO");
	wce3.WriteRecordInt("EL",m_Layer);
	wce3.WriteRecordInt("VL",m_Selection);
	wce3.WriteRecordInt("W",m_p1.x());
	//sprintf(sz,"W:%d",m_p1.x());
	//wce3.WriteRecord(sz);
	wce3.WriteRecordInt("H",m_p1.y());
	//sprintf(sz,"H:%d",m_p1.y());
	//wce3.WriteRecord(sz);
	wce3.WriteRecordString("PROJ",m_strProject);
	wce3.WriteRecordInt("PAGES",m_totalPages);
	wce3.WriteRecordInt("PAGE",m_page);
	wce3.WriteRecordInt("VER",m_nConstDataVersion);
	wce3.WriteRecordInt("INITPOS",m_initPos);
	wce3.WriteRecord("-SHEETINFO");
	wce3.WriteEOL();
	return true;
}

/*
//ストリームへの書き込み　+SHEETINFO ～ -SHEETINFO
bool SXBSchSheetInfo::writeStream(SCsvOStream* pStrm,const SPoint*)
{
	if(!pStrm) return false;
	char sz[32];
	pStrm->writeRecord("+SHEETINFO");
	sprintf(sz,"W:%d",m_p1.x());
	pStrm->writeRecord(sz);
	sprintf(sz,"H:%d",m_p1.y());
	pStrm->writeRecord(sz);
	pStrm->writeRecord("-SHEETINFO");
	pStrm->writeEOL();
	return true;
}
*/