/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#include "stdafx.h"
#include "snut.h"
#include "xbschsheetinfo.h"

SNutDataFile::~SNutDataFile()
{
	SNutDataElementListIterator ite;

	ite = m_recordList.begin();

	while(ite != m_recordList.end()){
		delete (*ite);
		ite++;
	}
	m_recordList.clear();

	//delete m_pDoc;
}

//参照番号をセットする
void SNutComponent::SetRef(SNutDataElement* pElement,const TCHAR* pcszRefnum)
//bool SNutComponent::loadRef()
{
	m_refNumElement = pElement;
	wstring refnum = pcszRefnum;
	int len = refnum.length();
	int pos = len-1;
	while(pos>=0){
		if(!isdigit(refnum[pos])) break;
		pos--;
	}
	pos++;	//プレフィックスの文字数が得られる
	m_prefix = refnum.substr(0,pos);
	if((len-pos)>0){
		m_suffix = _tstoi(refnum.substr(pos,len-pos).c_str());
		m_validSuffix = true;
	}else{
		m_suffix = 0;
		m_validSuffix = false;
	}
}

//ブロック番号をセットする
void SNutComponent::SetBlock(SNutDataElement* pElement,const TCHAR* block)
{
	m_blockElement = pElement;
	m_block = _tstoi(block);
}



void SNutComponent::update()
{
	if(m_ignore) return;
	TCHAR strNumVal[32];
	if(m_refNumElement!=NULL){
		_stprintf(strNumVal,_T("%d"),m_suffix);
		wstring refnum=_T("R:");
		refnum += m_prefix+strNumVal;
		m_refNumElement->record = refnum;
	}
	if(m_blockElement!=NULL){
		_stprintf(strNumVal,_T("%d"),m_block);
		wstring block=_T("B:");
		block+=strNumVal;
		m_blockElement->record = block;
	}
}


//bool SNutComponent::operator<(const SNutComponent &a)const
//{
//	if(m_index != a.m_index){
//		return m_index < a.m_index;
//	}
//	SPoint pa = m_pObj->p1();
//	SPoint pb = a.m_pObj->p1();
//	if(pa.x() != pb.x()){
//		return pa.x() < pb.x();
//	}
//	return pa.y() < pb.y();
//}


wstring createBackUpPath(const TCHAR* strPath)
{
	wstring str=strPath;
	return str+_T("$");
}

SNut::SNut()
{
	m_dataFileCount=0;
	m_compArray = NULL;
//	m_powcompArray = NULL;
	m_compArrayCount = 0;
//	m_powcompArrayCount = 0;
	m_enablePrefixFilter = false;
	m_prefixFilterDontMode = false;
//	m_newlyOnly = false;
	m_prefixFilterArray = NULL;
	m_prefixFilterCount = 0;
}

SNut::~SNut()
{
//	clearCompArray();
//	clearSchFiles();

	TRACE(_T("SNut::~SNut()  m_complist.size() %d\n"), m_complist.size());

	NutCompListIterator ite = m_complist.begin();
	while(ite != m_complist.end()){
		delete (*ite);
		ite++;
	}
	m_complist.clear();

	delete[]m_compArray;
	delete[]m_prefixFilterArray;
}

//void SNut::clearSchFiles()
//{
//	int n;
//	for(n = 0;n<m_dataFileCount;n++){
//        m_schFiles[n].m_filename =_T("");
//		//SXBSchDoc* pDoc = m_schFiles[n].m_pDoc;
//		//delete pDoc;
//		m_schFiles[n].m_pDoc = NULL;
//	}
//	m_dataFileCount=0;
//}


//void SNut::clearCompArray()
//{
//	int n;
//	SNutComponent* pNutComp;
//	if(m_compArrayCount>0){
//		for(n=0;n<m_compArrayCount;n++){
//			pNutComp = m_compArray[n];
//			delete pNutComp;
//		}
//	}
//	delete[]m_compArray;
//	m_compArray = NULL;
//	m_compArrayCount = 0;
//
////	if(m_powcompArrayCount>0){
////		for(n=0;n<m_powcompArrayCount;n++){
////			pNutComp = m_powcompArray[n];
////			delete pNutComp;
////		}
////	}
////	delete[]m_powcompArray;
////	m_powcompArray = NULL;
////	m_powcompArrayCount = 0;
//}


//部品のリファレンス、ブロック情報を、レコードに書き戻す
void SNut::restore()
{
	for(int i=0;i<m_compArrayCount;i++){
		TCHAR sz[16];
		_stprintf(sz,_T("%d"),m_compArray[i]->m_block);
		wstring rec = _T("BLK:");
		rec += sz;
		m_compArray[i]->m_blockElement->record = rec;

		_stprintf(sz,_T("%d"),m_compArray[i]->m_suffix);
		rec = _T("R:")+m_compArray[i]->m_prefix;
		rec += sz;
		m_compArray[i]->m_refNumElement->record = rec;
	}
}

//保存に失敗したら0以外を返す
int SNut::saveFiles()
{
	int dataIndex;
	int rval;
	for(int i=0;i<m_dataFileCount;i++){
		FILE* fp;
		if((fp = _tfopen(m_schFiles[i].m_filename.c_str(),_T("wt, ccs=UTF-8")))==NULL) return 1;
		fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.
		SWriteCE3 wce3;
		wce3.Attach(fp);

		SNutDataElementListIterator ite;

		ite = m_schFiles[i].m_recordList.begin();

		while(ite != m_schFiles[i].m_recordList.end()){
			if((*ite)->newline){
				wce3.WriteEOL();
			}
			wce3.WriteRecord((*ite)->record);
			ite++;
		}
		wce3.WriteEOL();
		fclose(fp);
	}
	return 0;
}
		

//バージョン判定
int SNut::ce3FileCheck(const TCHAR* fname)
{
	FILE* fp;
	char charBuff[1024];
	wchar_t wcharBuff[1024];
	size_t readSize;

	if((fp = _tfopen(fname,_T("rb")))==NULL) return CE3FCHK_ERROR;
	readSize = fread(charBuff,1,1023,fp);
	charBuff[readSize] = 0;
	fclose(fp);
	for(int i=0;i<=readSize;i++){
		wcharBuff[i] = charBuff[i];
	}
	SReadCE3 rce3;
	rce3.Attach(wcharBuff);

	wstring str(_T(""));
	if(rce3.ReadRecord(str)==WEOF){	//空のファイルのときの処理
			return CE3FCHK_ERROR;
	}

	if(str != _T("+BSCH3_DATA_V.1.0")){
		return CE3FCHK_ERROR;
	}
	if(rce3.SkipTo(_T("+SHEETINFO"))==WEOF){
		return CE3FCHK_ERROR;
	}
	SXBSchSheetInfo sheetInfo;
	if(! sheetInfo.readCe3(rce3,NULL,NULL)){
		return CE3FCHK_V080_OR_L;	//SheetInfoが長すぎるかもしれない。長いSheetInfoは070または以前にはない。
	}
	//Data Version 0.80未満はローカルエンコードと見做す
	if(sheetInfo.dataVersion()<80){
		return CE3FCHK_V070_OR_E;
	}else{
		return CE3FCHK_V080_OR_L;
	}
}

//読み込みに失敗したら0以外を返す
//1:すでに同じファイルを読み込みずみ
//2:読み込みエラー
//3:エラーじゃないけど新しいBSch3Vで作られたデータだ
int SNut::readFile(const TCHAR* path)
{
	//int m_dataFileCount;
	//SNutDataFile m_schFiles[MAX_SCH_FILES];
	int retv = 2;	
	WCHAR* wcBuff = NULL;
	stringList stackTag;
	SXBSchSheetInfo sheetInfo;
	SNutComponent* psnutComp= NULL;

	int dataIndex;
	//既にオープンしたデータに同名のファイルがないかのチェック
	for(dataIndex=0;dataIndex<m_dataFileCount;dataIndex++){
		if(path == m_schFiles[dataIndex].m_filename) return 1;
	}

	FILE* fp;
	SReadCE3 rce3;
	wstring str;

	int filecheck = ce3FileCheck(path);
	if(filecheck == CE3FCHK_ERROR){
		return 2;
	}else if(filecheck == CE3FCHK_V070_OR_E){
		 fp=_tfopen(path,_T("rb"));
		//seek and get file size;
		fseek(fp,0,SEEK_END);
		fpos_t filelength;			
		fgetpos(fp,&filelength); 
		fseek(fp,0,SEEK_SET);
		char* buff = new char[filelength+1];
		size_t readSize = fread(buff,1,filelength,fp);
		if(readSize<filelength) goto ReadEnd;
		buff[filelength]=0;

		int wcBuffSize = ::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, NULL,0);
		wcBuff = new WCHAR[wcBuffSize];
		::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, wcBuff, wcBuffSize);
		delete[]buff;

		rce3.Attach(wcBuff);
	}else{
		if((fp=_tfopen(path,_T("rt, ccs=UTF-8")))==NULL) return 2;
		rce3.Attach(fp);
	}







	//
	//if((fp=_tfopen(path,_T("rt, ccs=UTF-8")))==NULL) return 2;

	//SReadCE3 rce3;
	//rce3.Attach(fp);
	//wstring str;
	//if(rce3.ReadRecord(str)==WEOF){
	//	goto ReadEnd;
	//}
	//if(str !=_T("+BSCH3_DATA_V.1.0")){
	//	goto ReadEnd;
	//}
	//if(rce3.SkipTo(_T("+SHEETINFO"))==WEOF){
	//	goto ReadEnd;
	//}else{
	//	if(! sheetInfo.readCe3(rce3,NULL,NULL)){
	//		goto ReadEnd;
	//	}
	//	//Data Version 0.70以下はローカルエンコードと見做す
	//	if(sheetInfo.dataVersion()<=70){
	//		fclose(fp);
	//		//バイナリモードで開きなおす。
	//		FILE* fp=_tfopen(path,_T("rb"));
	//		//seek and get file size;
	//		fseek(fp,0,SEEK_END);
	//		fpos_t filelength;
	//		fgetpos(fp,&filelength); 
	//		fseek(fp,0,SEEK_SET);
	//		char* buff = new char[filelength+1];
	//		size_t readSize = fread(buff,1,filelength,fp);
	//		if(readSize<filelength) goto ReadEnd;
	//		buff[filelength]=0;

	//		int wcBuffSize = ::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, NULL,0);
	//		wcBuff = new WCHAR[wcBuffSize];
	//		::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, wcBuff, wcBuffSize);
	//		delete[]buff;

	//		rce3.Attach(wcBuff);
	//	}else{
	//		fseek(fp,0,SEEK_SET);
	//		rce3.Attach(fp);
	//	}
	//}


	m_schFiles[dataIndex].m_filename = path;
	m_dataFileCount++;



	enum DATA_FLAGS
	{
		COMPONENT = 1,
		COMPONENT_INFO = 2,
		SHEETINFO = 4
	};

	int linenum = rce3.Line();
	unsigned int flags = 0; 

	int recordCount =0;

	str = _T("");

	while(1){
		if(rce3.ReadRecord(str)==EOF){
			goto ReadEnd;
		}

		SNutDataElement* pRecord = new  SNutDataElement();
		pRecord->record = str;
		int line = rce3.Line();
		if(linenum!=line){
			pRecord->newline = true;
			linenum = line;
		}

		m_schFiles[dataIndex].m_recordList.push_back(pRecord);
		recordCount++;

		if(str[0]=='+'){
			wstring tag = str.substr(1);
			if(stackTag.size() == 1 && tag == _T("SHEETINFO")){
				flags = SHEETINFO;
			}else if(stackTag.size() == 1 && tag == _T("COMPONENT")){
				flags = COMPONENT;
				delete psnutComp;
				psnutComp = new SNutComponent();
				psnutComp->m_index = dataIndex;
			}else if(flags == COMPONENT && stackTag.size() == 3 && tag == _T("COMP")){
				flags |= COMPONENT_INFO;
			}
			stackTag.push_back(tag);
		}else if(str[0]=='-'){
			wstring tag = stackTag.back();
			if(tag != str.substr(1)){
				goto ReadEnd;
			}
			stackTag.pop_back();

			TRACE(_T("flags %02X, str %s,stackTag.size() %d\n"),flags,str.c_str(),stackTag.size());

			if(stackTag.size() == 0 && tag == _T("BSCH3_DATA_V.1.0")){
				break;
			}else if(stackTag.size()==1){
				if(flags == COMPONENT){
					if(psnutComp!=NULL){
						m_complist.push_back(psnutComp);
						psnutComp = NULL;
					}
				}
				flags = 0;
			}else if(flags == (COMPONENT|COMPONENT_INFO) && stackTag.size() == 3 && tag == _T("COMP")){
				flags &= ~COMPONENT_INFO;
			}

		}else{
				
			wstring var;
			wstring val;
			int nval;

			SReadCE3::ParseRecord(str,var,val);
			if(flags == SHEETINFO && stackTag.size() ==2){
				if(var == _T("VER")){
					int ver = _tstoi(val.c_str());
					if(ver<80){
						pRecord->record = _T("VER:80");
					}
				}
			}else if(flags == COMPONENT && stackTag.size() == 2 && psnutComp!=NULL){
				if(var == _T("L")){
					psnutComp->m_layer = _tstoi(val.c_str());
				}else if(var == _T("X")){
					psnutComp->m_p.setX(_tstoi(val.c_str()));
				}else if(var == _T("Y")){
					psnutComp->m_p.setY(_tstoi(val.c_str()));
				}else if(var == _T("BLK")){
					psnutComp->SetBlock(pRecord,val.c_str());
				}else if(var == _T("LIB")){
					psnutComp->m_orgname = val;
				}else if(var == _T("R")){
					psnutComp->SetRef(pRecord, val.c_str());
				}else if(var == _T("N")){
					psnutComp->m_name = val;
				}
			}else if(flags == (COMPONENT|COMPONENT_INFO) && stackTag.size() == 4  && psnutComp!=NULL){
				if(var == _T("B")){
					psnutComp->m_blocks = _tstoi(val.c_str());
				}else if(var == _T("TYPE")){
					if(val == _T("FXB")){
						psnutComp->m_useFixedBlock=true;
					}else if(val == _T("SHB")){
						psnutComp->m_powComp = true;
					}
				}else if(var == _T("FB")){
					nval = _tstoi(val.c_str());
					if(nval<0) nval=0;
					else if(nval>254) nval=254;
					psnutComp->m_fixedblock =nval;
				}
			}
		}
	}


ReadEnd:
	delete[]wcBuff;
	fclose(fp);
	return retv;
}

const TCHAR* SNut::filename(int n)
{
	if(n<0 || n>=m_dataFileCount)return NULL;
	return m_schFiles[n].m_filename.c_str();
}

int SNut::renameToBackup()
{
	int i;
	for(i=0;i<m_dataFileCount;i++){
		const TCHAR* path = m_schFiles[i].m_filename.c_str();
		wstring strBak=createBackUpPath(path);//バックアップファイル名を作成する。
		_tremove(strBak.c_str());							//すでに同名のバックアップファイルがある場合に、それを消去
		_trename(path,strBak.c_str());					//元のファイルをバックアップファイルに改名
	}
	return 1;
}

//Anumでの処理のためのソートのための比較ルーチン
// プレフィックス
// keepされている(newOnlyのときだけ
//    電源部品、SharedBlockの部品が前
// keepされていないもの
//    非電源部品、非SharedBlockの部品が前
//    ファイル番号が若いものが前
//    X座標
//    Y座標

int ComparePSNutCompForAnum( const void *p1, const void *p2 )
{
	PSNutComponent pElem1 = *(PSNutComponent*)(p1);
	PSNutComponent pElem2 = *(PSNutComponent*)(p2);

	int compValue;

	compValue= _tcscmp(pElem1->m_prefix.c_str(),pElem2->m_prefix.c_str());
	if(compValue !=0) return compValue;

	//keepされているものが前
	if( pElem1->m_keep  &&  !pElem2->m_keep ) return -1;
	if(!pElem1->m_keep  &&   pElem2->m_keep ) return  1;

	if(pElem1->m_keep){	//keep部品同志の比較
		//サフィックスで比較
		compValue = pElem1->m_suffix - pElem2->m_suffix;
		if(compValue !=0) return compValue;
		//電源部品が前
		if(  pElem1->m_powComp && !pElem2->m_powComp) return -1;
		if( !pElem1->m_powComp &&  pElem2->m_powComp) return  1;
		////blocksが大きいものが前
		//compValue = pElem1->m_blocks - pElem2->m_blocks;
		//if(compValue !=0) return compValue;
	}else{	//keepされていないもの同士の比較
		//非電源部品が前
		if( !pElem1->m_powComp &&  pElem2->m_powComp) return -1;
		if(  pElem1->m_powComp && !pElem2->m_powComp) return  1;
	}
	if(pElem1->m_index != pElem2->m_index){
		return pElem1->m_index - pElem2->m_index;
	}
	if(pElem1->m_layer != pElem2->m_layer){
		return pElem1->m_layer - pElem2->m_layer;
	}
	SPoint pa = pElem1->m_p;
	SPoint pb = pElem2->m_p;
	if(pa.x() != pb.x()){
		return pa.x()-pb.x();
	}
	return pa.y()-pb.y();
}



////Prefix -> keepされているものが前 -> powCompでない -> ページ -> X ->Yで比較する。
//int ComparePSNutCompPos( const void *p1, const void *p2 )
//{
//	PSNutComponent pElem1 = *(PSNutComponent*)(p1);
//	PSNutComponent pElem2 = *(PSNutComponent*)(p2);
//
//	int n= _tcscmp(pElem1->m_prefix.c_str(),pElem2->m_prefix.c_str());
//	if(n!=0) return n;
//
//
//	if((pElem1->m_keep ==false) && (pElem2->m_keep != false)) return 1;
//	if((pElem1->m_keep !=false) && (pElem2->m_keep == false)) return -1;
//
//	//keepフラグが設定されているもの同士ならサフィックスが小さいものが前に来る
//	if((pElem1->m_keep !=false) && (pElem2->m_keep != false)){
//		if(pElem1->m_suffix != pElem2->m_suffix){
//			return pElem1->m_suffix - pElem2->m_suffix;
//		}
//	}
//
//	if((pElem1->m_powComp ==false) && (pElem2->m_powComp != false)) return -1;
//	if((pElem1->m_powComp !=false) && (pElem2->m_powComp == false)) return 1;
//
//
//	//サフィックスが設定されていないもの同士なら、ページ、位置で比較する
//	if(pElem1->m_index != pElem2->m_index){
//		return pElem1->m_index - pElem2->m_index;
//	}
//	SPoint pa = pElem1->m_p;
//	SPoint pb = pElem2->m_p;
//	if(pa.x() != pb.x()){
//		return pa.x()-pb.x();
//	}
//	return pa.y()-pb.y();
//}
//

//Prefix -> Suffixで比較する。
int ComparePSNutCompRef( const void *p1, const void *p2 )
{
	PSNutComponent pElem1 = *(PSNutComponent*)(p1);
	PSNutComponent pElem2 = *(PSNutComponent*)(p2);

	int n= _tcscmp(pElem1->m_prefix.c_str(),pElem2->m_prefix.c_str());
	if(n!=0) return n;
	return pElem1->m_suffix - pElem2->m_suffix;
}


//bool SNut::DoPrefixFilter(const char* pcszPrefix)
//{
//	return true;
//}

void SNut::setPrefixFilter(const TCHAR* filterPrefixes)
{
	stringList tempStringList;
	wstring s;

	delete[]m_prefixFilterArray;
	m_prefixFilterCount = 0;


	if(!filterPrefixes)return;

	int state = 0;	//0:start  1:prefix  2:end



	while(*filterPrefixes){
		if(state == 0){
			if(!isspace(*filterPrefixes)){
				state = 1;
				s.clear();
			}else{
				filterPrefixes++;
				continue;
			}
		}

		if(state == 1){
			if(!isspace(*filterPrefixes) && (*filterPrefixes  != ',')){
				s+=*filterPrefixes;
				filterPrefixes++;
				continue;
			}else{
				state = 2;
				if(s.length()>0){
					tempStringList.push_back(s);
				}
			}
		}

		if(state == 2){
			if(*filterPrefixes  == ','){
				state = 0;
			}
			filterPrefixes++;
		}
	}

	if(state==1){
		if(s.length()>0){
			tempStringList.push_back(s);
		}
	}

	if(tempStringList.size()>0){
		m_prefixFilterCount = tempStringList.size();
		m_prefixFilterArray = new wstring[m_prefixFilterCount];
		for(int i=0;i<m_prefixFilterCount;i++){
			m_prefixFilterArray[i] = tempStringList.front();
			tempStringList.pop_front();
		}
	}


	for(int i=0;i<m_prefixFilterCount;i++){
		TRACE("%s\n",m_prefixFilterArray[i].c_str());
	}

}


//m_complist の部品から、処理対象をm_compArrayにまとめる。
void SNut::pickupComponent(unsigned int option)
{

	bool newlyOnly = ((option & NUTOPTION_NEWONLY)!=0);
	bool ignore_layer = ((option & NUTOPTION_EN_LAYER)==0);

	NutCompList listCompP;

	NutCompListIterator ite = m_complist.begin();
	while(ite != m_complist.end()){
		SNutComponent* pComp = *ite;
		ite++;
		if(pComp->m_prefix.size()==0) continue;	//部品番号プレフィクスのないものは無視
		if(pComp->m_validSuffix && newlyOnly) pComp->m_keep = true;
		if(ignore_layer) pComp->m_layer = 0;
		if(m_enablePrefixFilter){
			bool prefixHit = false;
			for(int i=0;i<m_prefixFilterCount;i++){
				if(_tcsicmp(pComp->m_prefix.c_str(),m_prefixFilterArray[i].c_str())==0){
					prefixHit = true;
					break;
				}
			}
			if(prefixHit && m_prefixFilterDontMode) continue;	//除外モードでプレフィックスがヒットしたら無視
		    if(!prefixHit && !m_prefixFilterDontMode) continue;	//限定モードでプレフィックスがヒットしないときは無視
		}


		//電源部品かどうかの判断
		//orgnameが4文字以上で、先頭が"VCC","VDD","GND","VEE","VSS"のとき電源部品とする。
		if((pComp->m_orgname.size()>3)
			&&(
			  (_tcsncmp(pComp->m_orgname.c_str(),_T("VCC"),3)==0)
			||(_tcsncmp(pComp->m_orgname.c_str(),_T("VDD"),3)==0)
			||(_tcsncmp(pComp->m_orgname.c_str(),_T("GND"),3)==0)
			||(_tcsncmp(pComp->m_orgname.c_str(),_T("VEE"),3)==0)
			||(_tcsncmp(pComp->m_orgname.c_str(),_T("VSS"),3)==0)
			)
			){
				pComp->m_powComp |= true;
		}
		listCompP.push_back(pComp);
	}
	m_compArrayCount = listCompP.size();
	m_compArray = new PSNutComponent[m_compArrayCount];
	ite = listCompP.begin();
	int n=0;
	while(ite != listCompP.end()){
		m_compArray[n] = (*ite);
		n++;
		ite++;
	}
}



//void SNut::sortPOS()
//{
//	qsort(m_compArray,m_compArrayCount, sizeof(PSNutComponent), ComparePSNutCompPos );
//}

//void SNut::sortREF()
//{
//	qsort(m_compArray,m_compArrayCount, sizeof(PSNutComponent), ComparePSNutCompRef );
//}


int SNut::anum(int init,int step,unsigned int option)
{
	bool newonly = ((option & NUTOPTION_NEWONLY)!=0);
	pickupComponent(option);

	qsort(m_compArray,m_compArrayCount, sizeof(PSNutComponent), ComparePSNutCompForAnum);
	wstring currentPrefix=_T("");
	int prefixStartPos=0;
	int nextPrefixStartPos=0;
	int notkeepStart;
	while(nextPrefixStartPos<m_compArrayCount){
		prefixStartPos = nextPrefixStartPos;
		currentPrefix = m_compArray[prefixStartPos]->m_prefix;




		//次のプリフィックスの始まりをみつけておく
		nextPrefixStartPos++;
		while(nextPrefixStartPos<m_compArrayCount){
			if(m_compArray[nextPrefixStartPos]->m_prefix != currentPrefix) break;
			nextPrefixStartPos++;
		}
		//keepされていないグループの始まりをみつけておく
		notkeepStart = prefixStartPos;
		while(notkeepStart<nextPrefixStartPos){
			if(!m_compArray[notkeepStart]->m_keep) break;
			notkeepStart++;
		}


		if(currentPrefix==_T("U")){
			TRACE(_T("\n"));
		}

		if(newonly){
			//既存の電源部品に関連付けを行う
			for(int i=prefixStartPos;i<notkeepStart;i++){
				//if(m_compArray[i]->m_set){i++; continue;}
				if(m_compArray[i]->m_powComp){//keepグループに電源部品が見つかったら
					bool setFunctionComponent = false;	//電源部品に機能部品が関連付けられていたらtrueになる。

					//既存の機能部品との関連付けを試みる
					for(int j=i+1;j<notkeepStart;j++){
						if(m_compArray[i]->m_suffix == m_compArray[j]->m_suffix && !m_compArray[j]->m_powComp){
							m_compArray[j]->m_setPowComp = true;
							setFunctionComponent = true;
						}
					}

					//既存の機能部品と関連づけができなかった場合は新規部品との関連づけを行う
					//ここで関連づけた新規部品はkeepフラグを立てて、既存部品と同じ扱いとする。
					if(!setFunctionComponent){
						for(int j=notkeepStart;j<nextPrefixStartPos;j++){
							if(m_compArray[j]->m_keep)continue;
							if(m_compArray[j]->m_powComp)continue;
							if(m_compArray[i]->m_name == m_compArray[j]->m_name){
								m_compArray[j]->m_suffix = m_compArray[i]->m_suffix;
								m_compArray[j]->m_block = 0;
								m_compArray[j]->m_keep = true;	//■■■■■ keepフラグを立てている ■■■■■ 
								m_compArray[j]->m_setPowComp = true;
								setFunctionComponent = true;
								break;
							}
						}
					}
				}
			}

			//既存の複数ブロック部品を埋める
			//前のステップで、既存電源部品と関連づけられた新規部品も対象となるため、
			//ループは現在のプレフィックス全体に及ぶ
			for(int i=prefixStartPos;i<nextPrefixStartPos;i++){
				if(m_compArray[i]->m_checked) continue;
				if(m_compArray[i]->m_powComp) continue;
				if(!m_compArray[i]->m_keep)continue;
				if(m_compArray[i]->m_blocks>1){
					int blocks = m_compArray[i]->m_blocks;
					if(blocks>255) blocks = 255;	//2014/05/06
					int blocksetcount = 1;
					bool blocktabel[256];
					for(int j=0;j<blocks;j++)blocktabel[j] = false;
					int block = m_compArray[i]->m_block;
					if(block>=blocks) block = blocks-1;
					blocktabel[block]=true;
					m_compArray[i]->m_checked = true;

					//既存部品で埋めることを試みる
					//サフィックスが同じならOKとする。
					//名前やブロック番号、ブロック数に矛盾がないことは既存番号の責任
					for(int j=i+1;j<notkeepStart && blocksetcount<blocks; j++){
						if(m_compArray[j]->m_checked) continue;
						if(m_compArray[j]->m_powComp) continue;
						if(m_compArray[i]->m_suffix == m_compArray[j]->m_suffix){
							block = m_compArray[j]->m_block;
							if(block>=blocks) block = blocks-1;
							blocktabel[block]=true;
							blocksetcount++;
							m_compArray[j]->m_checked = true;
							m_compArray[j]->m_setPowComp = m_compArray[i]->m_setPowComp;
						}
					}

					//既存部品で埋められない場合は新規部品で埋める
					//名前が一致すればOKとする。
					//ブロック数に矛盾がないことは作図の責任
					int unusedBlock =0;
					for(int j=notkeepStart; j<nextPrefixStartPos && blocksetcount<blocks; j++){
						if(m_compArray[j]->m_checked) continue;
						if(m_compArray[j]->m_powComp) continue;
						if(m_compArray[i]->m_name == m_compArray[j]->m_name){
							m_compArray[j]->m_suffix = m_compArray[i]->m_suffix;
							//未使用ブロック番号を探す
							while(unusedBlock<blocks && blocktabel[unusedBlock])unusedBlock++;
							if(unusedBlock<blocks){
								blocktabel[unusedBlock]=true;
							}
							m_compArray[j]->m_block = min(unusedBlock,m_compArray[j]->m_blocks-1);
							m_compArray[j]->m_checked = true;
							m_compArray[j]->m_setPowComp = m_compArray[i]->m_setPowComp;
							blocksetcount++;
						}
					}
				}

				else if(m_compArray[i]->m_useFixedBlock){
					bool blocktabel[255];
					for(int j=0;j<255;j++)blocktabel[j] = false;
					int block = m_compArray[i]->m_fixedblock;
					blocktabel[block]=true;
					m_compArray[i]->m_checked = true;

					//既存部品で埋めることを試みる
					//サフィックスが同じならOKとする。
					//名前やブロック番号に矛盾がないことは既存番号の責任
					for(int j=i+1;j<notkeepStart; j++){
						if(m_compArray[j]->m_checked) continue;
						if(m_compArray[j]->m_powComp) continue;
						if(m_compArray[i]->m_suffix == m_compArray[j]->m_suffix){
							block = m_compArray[j]->m_fixedblock;
							blocktabel[block]=true;
							m_compArray[j]->m_checked = true;
							m_compArray[j]->m_setPowComp = m_compArray[i]->m_setPowComp;
						}
					}

					//既存部品で埋められない場合は新規部品で埋める
					//名前が一致すればOKとする。
					//ブロック数に矛盾がないことは作図の責任
					int unusedBlock =0;
					for(int j=notkeepStart; j<nextPrefixStartPos; j++){
						if(m_compArray[j]->m_checked) continue;
						if(m_compArray[j]->m_powComp) continue;
						if(m_compArray[i]->m_name == m_compArray[j]->m_name && m_compArray[j]->m_useFixedBlock){
							block = m_compArray[j]->m_fixedblock;
							if(blocktabel[block]) continue;
							blocktabel[block] = true;
							m_compArray[j]->m_suffix = m_compArray[i]->m_suffix;
							m_compArray[j]->m_checked = true;
							m_compArray[j]->m_setPowComp = m_compArray[i]->m_setPowComp;
						}
					}
				}
			}
		}





		//新規部品の電源部品以外の処理
		//大ループは初期状態の新規部品の範囲だけだが、電源部品との関連づけでkeepフラグを立てている場合がある。
		//そのため、このループでもkeepフラグをチェックして、フラグが立っているものは除外している。
		int unusedSuffix=init;
		for(int i=notkeepStart;i<nextPrefixStartPos;i++){
			if(m_compArray[i]->m_checked) continue;
			if(m_compArray[i]->m_powComp) continue;
			if(m_compArray[i]->m_keep)continue;		//keepフラグが立っているものは処理しない
			TRACE(_T("newly suffix %d\n"),m_compArray[i]->m_suffix);

			if(newonly){
				//既存部品で使われていないサフィックスを得る
				bool used = true;
				while(used){
					used = false;
					for(int j=prefixStartPos;j<notkeepStart;j++){
						if(m_compArray[j]->m_suffix == unusedSuffix){
							used = true;
							unusedSuffix += step;	//使われていたらステップぶん進めて再度チェックに
							break;
						}
					}
				}
			}

			m_compArray[i]->m_checked = true;
			m_compArray[i]->m_block = 0;
			m_compArray[i]->m_suffix = unusedSuffix;

			//複数ブロック部品は埋める
			if(m_compArray[i]->m_blocks>1){
				int blocks = m_compArray[i]->m_blocks;
				int block = 1;
				for(int j=i+1;j<nextPrefixStartPos && block<blocks;j++){
					if(m_compArray[j]->m_checked) continue;
					if(m_compArray[j]->m_powComp) continue;
					if(m_compArray[i]->m_keep)continue;		//keepフラグが立っているものは処理しない
					if(m_compArray[i]->m_name == m_compArray[j]->m_name){
						m_compArray[j]->m_checked = true;
						m_compArray[j]->m_suffix = m_compArray[i]->m_suffix;
						m_compArray[j]->m_block = block;
						block++;
					}
				}
			}else if(m_compArray[i]->m_useFixedBlock){
				bool blocktabel[255];
				for(int j=0;j<255;j++)blocktabel[j] = false;
				int block = m_compArray[i]->m_fixedblock;
				blocktabel[block] = true;
				for(int j=i+1;j<nextPrefixStartPos;j++){
					if(m_compArray[j]->m_checked) continue;
					if(m_compArray[j]->m_powComp) continue;
					if(m_compArray[i]->m_keep)continue;		//keepフラグが立っているものは処理しない
					if(! m_compArray[i]->m_useFixedBlock) continue;
					if(m_compArray[i]->m_name == m_compArray[j]->m_name){
						block = m_compArray[j]->m_fixedblock;
						if(!blocktabel[block]){
							blocktabel[block] = true;
							m_compArray[j]->m_checked = true;
							m_compArray[j]->m_suffix = m_compArray[i]->m_suffix;
							
						}
					}
				}
			}
			unusedSuffix += step;
		}


		//新規部品の電源部品の処理
		for(int i=notkeepStart;i<nextPrefixStartPos;i++){
			if(!m_compArray[i]->m_powComp) continue;
			if(m_compArray[i]->m_keep)continue;
			TRACE(_T("newly powercomp suffix %d\n"),m_compArray[i]->m_suffix);
			int suffix;
			bool setSuffix = false;
			//設定済みの電源部品以外をスキャンして関連付けができるものを探す
			for(int j=prefixStartPos; j<i; j++){
				if(m_compArray[j]->m_powComp) continue;
				if(m_compArray[j]->m_setPowComp) continue; //すでに電源部品と関連付けられているものは除外
				if(m_compArray[j]->m_name == m_compArray[i]->m_name){
					if(!setSuffix){		//最初に関連付けできる機能部品を見つけるまでの処理
						suffix = m_compArray[j]->m_suffix;
						m_compArray[j]->m_setPowComp = true;
						setSuffix = true;		//「見つかった」フラグ
					}else{				//いったん見つかったら、同じ名前で同じ部品番号の機能部品に関連付けフラグを立てておく
						if(suffix == m_compArray[j]->m_suffix){
							m_compArray[j]->m_setPowComp = true;
						}
					}
				}
			}
			if(setSuffix){	//機能部品との関連付けができた場合
				m_compArray[i]->m_suffix = suffix;
			}else{			//機能部品との関連付けができなかった場合
				if(newonly){
					bool used = true;
					while(used){
						used = false;
						//既存部品で使われていないサフィックスを得る
						for(int j=prefixStartPos;j<notkeepStart;j++){
							if(m_compArray[j]->m_suffix == unusedSuffix){
								used = true;
								unusedSuffix += step;
								break;
							}
						}
					}
				}
				m_compArray[i]->m_suffix = unusedSuffix;
				unusedSuffix += step;
			}
		}

	}
	restore();
	return 1;
}




int SNut::renum(int init,int step)
{
	pickupComponent(NUTOPTION_NON);
	qsort(m_compArray,m_compArrayCount, sizeof(PSNutComponent), ComparePSNutCompRef );

	wstring currentPrefix=_T("");
	int currentSuffix;
	int suffix;

	for(int n=0;n<m_compArrayCount;n++){
		if(m_compArray[n]->m_prefix != currentPrefix){	//スキャンしていてプリフィックスが変わったら
			currentPrefix = m_compArray[n]->m_prefix;
			currentSuffix = m_compArray[n]->m_suffix;
			suffix = init;
		}else{
			if(m_compArray[n]->m_suffix != currentSuffix){
				currentSuffix = m_compArray[n]->m_suffix;
				suffix += step;
			}
		}
		m_compArray[n]->m_suffix = suffix;
	}
	restore();
	return 1;
}
