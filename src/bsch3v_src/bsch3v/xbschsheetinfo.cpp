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
	m_strProject = _T("");
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
	wstring str(_T(""));
	bool bInitW = false;
	bool bInitH = false;
	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if((str != _T("-SHEETINFO")) || !bInitW || !bInitH){
				return false;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==WEOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<l){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				if(var==_T("PROJ")){
					m_strProject = str.substr(n+1);
				}else{
					int nParam = _tstoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
	//#ifdef _MFCBSCH	//Qt-BSch doesn't support layer.
					if(var==_T("EL")){
						if(isValidLayer(nParam)){
							m_Layer	= nParam;			
						}
					}else
					if(var==_T("VL")){
						nParam &=0xFF;
						if(nParam==0) nParam = 0xFF;
						m_Selection = nParam;
					}else
	//#endif				
					if(var==_T("W")){
						if(nParam>=SHEETSIZE_X_MIN && nParam <=SHEETSIZE_X_MAX) m_p1.setX(nParam);
						bInitW = true;
					}else
					if(var==_T("H")){
						if(nParam>=SHEETSIZE_Y_MIN && nParam <=SHEETSIZE_Y_MAX) m_p1.setY(nParam);
						bInitH = true;
					}else
					if(var==_T("PAGES")){
						m_totalPages=nParam;
					}else
					if(var==_T("PAGE")){
						m_page=nParam;
					}else
					if(var==_T("VER")){
						m_version=nParam;
					}else
					if(var==_T("INITPOS")){
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
	wce3.WriteRecord(_T("+SHEETINFO"));
	wce3.WriteRecordInt(_T("EL"),m_Layer);
	wce3.WriteRecordInt(_T("VL"),m_Selection);
	wce3.WriteRecordInt(_T("W"),m_p1.x());
	//sprintf(sz,"W:%d",m_p1.x());
	//wce3.WriteRecord(sz);
	wce3.WriteRecordInt(_T("H"),m_p1.y());
	//sprintf(sz,"H:%d",m_p1.y());
	//wce3.WriteRecord(sz);
	wce3.WriteRecordString(_T("PROJ"),m_strProject);
	wce3.WriteRecordInt(_T("PAGES"),m_totalPages);
	wce3.WriteRecordInt(_T("PAGE"),m_page);
	wce3.WriteRecordInt(_T("VER"),m_nConstDataVersion);
	wce3.WriteRecordInt(_T("INITPOS"),m_initPos);
	wce3.WriteRecord(_T("-SHEETINFO"));
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