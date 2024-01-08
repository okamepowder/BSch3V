/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#include "stdafx.h"
#include "plist.h"
#include "xbschsheetinfo.h"

SNutDataFile::~SNutDataFile()
{
}

//参照番号をセットする
void SNutComponent::SetRef(/*SNutDataElement* pElement,*/const TCHAR* pcszRefnum)
//bool SNutComponent::loadRef()
{
	//m_refNumElement = pElement;
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


SPlist::SPlist()
{
	m_dataFileCount=0;
	m_compArray = NULL;
	m_compArrayCount = 0;
}

SPlist::~SPlist()
{
	TRACE(_T("SPlist::~SNut()  m_complist.size() %d\n"), m_complist.size());

	NutCompListIterator ite = m_complist.begin();
	while(ite != m_complist.end()){
		delete (*ite);
		ite++;
	}
	m_complist.clear();

	delete[]m_compArray;
}





//読み込みに失敗したら0以外を返す
//1:すでに同じファイルを読み込みずみ
//2:読み込みエラー
//3:エラーじゃないけど新しいBSch3Vで作られたデータだ
int SPlist::readFile(const TCHAR* path)
{
	//int m_dataFileCount;
	//SNutDataFile m_schFiles[MAX_SCH_FILES];
	int retv = 2;	
	char* buff = NULL;
	WCHAR* wcBuff = NULL;
	stringList stackTag;
	SXBSchSheetInfo sheetInfo;
	SNutComponent* psnutComp= NULL;
	SReadCE3 rce3;
	wstring str;



	int dataIndex;
	//既にオープンしたデータに同名のファイルがないかのチェック
	for(dataIndex=0;dataIndex<m_dataFileCount;dataIndex++){
		if(path == m_schFiles[dataIndex].m_filename) return 1;
	}

	FILE* fp=_tfopen(path,_T("rb"));
	//seek and get file size;
	fseek(fp,0,SEEK_END);
	fpos_t filelength;
	fgetpos(fp,&filelength); 
	fseek(fp,0,SEEK_SET);
	buff = new char[filelength+1];
	size_t readSize = fread(buff,1,filelength,fp);
	fclose(fp);
	if(readSize<filelength) goto ReadEnd;
	buff[filelength]=0;


	//ここからバージョン判定
	wcBuff = new wchar_t[filelength+1];
	for(int i=0;i<(filelength+1);i++){
		wcBuff[i]=buff[i];
	}
	rce3.Attach(wcBuff);

	if(rce3.ReadRecord(str)==WEOF){
		goto ReadEnd;
	}
	if(str !=_T("+BSCH3_DATA_V.1.0")){
		goto ReadEnd;
	}
	if(rce3.SkipTo(_T("+SHEETINFO"))==WEOF){
		goto ReadEnd;
	}else{
		if(! sheetInfo.readCe3(rce3,NULL,NULL)){
			goto ReadEnd;
		}
	}

	delete[]wcBuff;
	wcBuff = NULL;
	//ここまでバージョン判定

	//Data Version 0.70以下はローカルエンコードと見做す
	if(sheetInfo.dataVersion()<=70){
		int wcBuffSize = ::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, NULL,0);
		wcBuff = new WCHAR[wcBuffSize];
		::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, wcBuff, wcBuffSize);
	}else{
		int wcBuffSize = ::MultiByteToWideChar(CP_UTF8, 0, buff, -1, NULL,0);
		wcBuff = new WCHAR[wcBuffSize];
		::MultiByteToWideChar(CP_UTF8, 0, buff, -1, wcBuff, wcBuffSize);
	}
	delete[]buff;
	buff = NULL;

	rce3.Attach(wcBuff);





	//FILE* fp;
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

			SReadCE3::ParseRecord(str,var,val);
			if(flags == SHEETINFO && stackTag.size() ==2){
			}else if(flags == COMPONENT && stackTag.size() == 2 && psnutComp!=NULL){
				if(var == _T("X")){
					psnutComp->m_p.setX(_tstoi(val.c_str()));
				}else if(var == _T("Y")){
					psnutComp->m_p.setY(_tstoi(val.c_str()));
				//}else if(var == _T("BLK")){
				//	psnutComp->SetBlock(pRecord,val.c_str());
				}else if(var == _T("LIB")){
					psnutComp->m_orgname = val;
				}else if(var == _T("MFR")){
					psnutComp->m_mfr = val;
				}else if(var == _T("MFRPN")){
					psnutComp->m_mfrpn = val;
				}else if(var == _T("NOTE")){
					psnutComp->m_note = val;
				}else if(var == _T("PKG")){
					psnutComp->m_package = val;
				}else if(var == _T("R")){
					psnutComp->SetRef(/*pRecord,*/ val.c_str());
				}else if(var == _T("N")){
					psnutComp->m_name = val;
				}
			}else if(flags == (COMPONENT|COMPONENT_INFO) && stackTag.size() == 4  && psnutComp!=NULL){
				if(var == _T("NOTE")){
					psnutComp->m_componentNote = val;
				}
			}
		}
	}

ReadEnd:
	delete[]buff;
	delete[]wcBuff;
	return retv;
}


//Prefix -> Suffixで比較する。
int ComparePSNutCompRef( const void *p1, const void *p2 )
{
	PSNutComponent pElem1 = *(PSNutComponent*)(p1);
	PSNutComponent pElem2 = *(PSNutComponent*)(p2);

	int n= _tcscmp(pElem1->m_prefix.c_str(),pElem2->m_prefix.c_str());
	if(n!=0) return n;
	return pElem1->m_suffix - pElem2->m_suffix;
}



//m_complist の部品から、処理対象をm_compArrayにまとめる。
void SPlist::pickupComponent()
{

	NutCompList listCompP;

	NutCompListIterator ite = m_complist.begin();
	while(ite != m_complist.end()){
		SNutComponent* pComp = *ite;
		ite++;
		if(pComp->m_prefix.size()==0) continue;	//部品番号プレフィクスのないものは無視
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



void SPlist::convertToCsvRecord(const wstring& src,wstring& dest)
{
	dest = _T("");

	int len= src.size();
	dest+=_T("\"");
	for(int i=0;i<len;i++){
		if(src[i]=='\"'){
			dest+=_T("\"\"");
		}else if(src[i]=='\r'){		//Windows環境とExcelのCSVに依存する
		}else{
			dest+=src[i];
		}
	}
	dest+=_T("\"");
}	


void SPlist::encodeXMLstring(const wstring& src,wstring& dest)
{
	dest = _T("");

	int len= src.size();
	for(int i=0;i<len;i++){
		switch(src[i]){
			case '<':
				dest+=_T("&lt;");		break;
			case '>':
				dest+=_T("&gt;");		break;
			case '&':
				dest+=_T("&amp;");		break;
			case '\"':
				dest+=_T("&quot;");		break;
			default:
				if(src[i]<0x20){
					TCHAR str[64];
					_stprintf(str,_T("&#%d;"),(int)(src[i]));
					dest+=str;
				}else{
					dest+=src[i];
				}
		}
	}
}


int SPlist::writePartsList(const TCHAR* filename)
{
	if(m_param.listtype == PListParam::PL_FILETYPE_CSV){
		return writePartsList_csv(filename);
	}else{
		return writePartsList_xml(filename);
	}
}



//CSV形式の部品リスト出力
int SPlist::writePartsList_csv(const TCHAR* filename)
{
	pickupComponent();
	if(m_compArrayCount==0) return -1;

	qsort(m_compArray,m_compArrayCount, sizeof(PSNutComponent), ComparePSNutCompRef );

	FILE* fp;
	if((fp = _tfopen(filename,_T("wt, ccs=UTF-8")))==NULL) return -2;
	//fseek( fp, 0, SEEK_SET );
	

	wstring name=_T("");
	wstring prefix=_T("");
	int suffix;
	wstring outString;

	for(int i=0;i<m_compArrayCount;i++){
		if(i>0 && (prefix==m_compArray[i]->m_prefix && suffix==m_compArray[i]->m_suffix)){
			if(name == m_compArray[i]->m_name){
				outString = _T("*");
			}else{
				outString = _T("?");
			}
		}else{
			outString = _T("");
		}

		_ftprintf(fp,_T("%s"),outString.c_str());

		convertToCsvRecord(m_compArray[i]->m_prefix,outString);
		_ftprintf(fp,_T(",%s,%d"),outString.c_str(),m_compArray[i]->m_suffix);

		convertToCsvRecord(m_compArray[i]->m_name,outString);
		_ftprintf(fp,_T(",%s"),outString.c_str());

		if(m_param.plFlagMfr){
			convertToCsvRecord(m_compArray[i]->m_mfr,outString);
			_ftprintf(fp,_T(",%s"),outString.c_str());
		}

		if(m_param.plFlagMfrPn){
			convertToCsvRecord(m_compArray[i]->m_mfrpn,outString);
			_ftprintf(fp,_T(",%s"),outString.c_str());
		}

		if(m_param.plFlagPackage){
			convertToCsvRecord(m_compArray[i]->m_package,outString);
			_ftprintf(fp,_T(",%s"),outString.c_str());
		}

		if(m_param.plFlagNote){
			convertToCsvRecord(m_compArray[i]->m_note,outString);
			_ftprintf(fp,_T(",%s"),outString.c_str());
		}

		if(m_param.plFlagInfo){
			convertToCsvRecord(m_compArray[i]->m_componentNote,outString);
			_ftprintf(fp,_T(",%s"),outString.c_str());
		}

		_ftprintf(fp,_T("\n"));

		prefix	= m_compArray[i]->m_prefix;
		suffix	= m_compArray[i]->m_suffix;
		name	= m_compArray[i]->m_name;
	}
	fclose(fp);
	return 0;
}

const TCHAR* SPlist::xmlHeader =
_T("<?xml version=\"1.0\"?>\n\
<?mso-application progid=\"Excel.Sheet\"?>\n\
<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
 xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n\
 xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n\
 xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
 xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n\
 <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n\
 </DocumentProperties>\n\
 <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n\
 </ExcelWorkbook>\n\
 <Styles>\n\
   <Style ss:ID=\"s70\">\n\
    <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Top\"/>\n\
    <Borders/>\n\
    <Font ss:FontName=\"Arial Unicode MS\" x:CharSet=\"128\" x:Family=\"Modern\"\n\
     ss:Size=\"9\"/>\n\
    <Interior/>\n\
   </Style>\n\
   <Style ss:ID=\"s71\">\n\
    <Alignment ss:Horizontal=\"Right\" ss:Vertical=\"Top\"/>\n\
    <Borders/>\n\
    <Font ss:FontName=\"Arial Unicode MS\" x:CharSet=\"128\" x:Family=\"Modern\"\n\
     ss:Size=\"9\"/>\n\
    <Interior/>\n\
   </Style>\n\
   <Style ss:ID=\"s72\">\n\
    <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Top\" ss:WrapText=\"1\"/>\n\
    <Borders/>\n\
    <Font ss:FontName=\"Arial Unicode MS\" x:CharSet=\"128\" x:Family=\"Modern\"\n\
     ss:Size=\"9\"/>\n\
    <Interior/>\n\
   </Style>\n\
 </Styles>\n");





//xml形式の部品リスト出力
int SPlist::writePartsList_xml(const TCHAR* filename)
{
	pickupComponent();
	if(m_compArrayCount==0) return -1;

	qsort(m_compArray,m_compArrayCount, sizeof(PSNutComponent), ComparePSNutCompRef );

	int rowCount = m_compArrayCount;

	int columnWidth[16];
	columnWidth[0]=SPlist::COLUMN_WIDTH_STATUS;
	columnWidth[1]=SPlist::COLUMN_WIDTH_PREFIX;
	columnWidth[2]=SPlist::COLUMN_WIDTH_SUFFIX;
	columnWidth[3]=SPlist::COLUMN_WIDTH_NAME;
	int columnCount = 4;	//ステータス、プリフィックス、サフィックス、部品名

	if(m_param.plFlagMfr){
		columnWidth[columnCount] = SPlist::COLUMN_WIDTH_MFR;
		columnCount++;
	}
	if(m_param.plFlagMfrPn){
		columnWidth[columnCount] = SPlist::COLUMN_WIDTH_MFRPN;
		columnCount++;
	}
	if(m_param.plFlagPackage){
		columnWidth[columnCount] = SPlist::COLUMN_WIDTH_PKG;
		columnCount++;
	}
	if(m_param.plFlagNote){
		columnWidth[columnCount] = SPlist::COLUMN_WIDTH_NOTE;
		columnCount++;
	}
	if(m_param.plFlagInfo){
		columnWidth[columnCount] = SPlist::COLUMN_WIDTH_COMP_NOTE;
		columnCount++;
	}




	FILE* fp;
	if((fp = _tfopen(filename,_T("wt, ccs=UTF-8")))==NULL) return -2;
	fseek( fp, 0, SEEK_SET );
	
	_ftprintf(fp,_T("%s"),xmlHeader);
	_ftprintf(fp,_T("<Worksheet ss:Name=\"Sheet1\">\n"));
	_ftprintf(fp,_T(" <Table ss:ExpandedColumnCount=\"%d\" ss:ExpandedRowCount=\"%d\" x:FullColumns=\"1\"\n\
  x:FullRows=\"1\" ss:StyleID=\"s70\" ss:DefaultColumnWidth=\"60\"\n\
  ss:DefaultRowHeight=\"16.5\">\n"),columnCount,rowCount);

	for(int i=0;i<columnCount;i++){
		_ftprintf(fp,_T("  <Column ss:StyleID=\"s70\" ss:AutoFitWidth=\"0\" ss:Width=\"%d\"/>\n"),columnWidth[i]);
	}

	wstring name=_T("");
	wstring prefix=_T("");
	int suffix;
	wstring outString;
	for(int i=0;i<rowCount;i++){
		if(i>0 && (prefix==m_compArray[i]->m_prefix && suffix==m_compArray[i]->m_suffix)){
			if(name == m_compArray[i]->m_name){
				outString = _T("*");
			}else{
				outString = _T("?");
			}
		}else{
			outString = _T("");
		}
		_ftprintf(fp,_T("  <Row ss:AutoFitHeight=\"1\">\n"));
		_ftprintf(fp,_T("   <Cell ss:StyleID=\"s70\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		encodeXMLstring(m_compArray[i]->m_prefix,outString);
		_ftprintf(fp,_T("   <Cell ss:StyleID=\"s71\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		_ftprintf(fp,_T("   <Cell ss:StyleID=\"s70\"><Data ss:Type=\"Number\">%d</Data></Cell>\n"),m_compArray[i]->m_suffix);
		encodeXMLstring(m_compArray[i]->m_name,outString);
		_ftprintf(fp,_T("   <Cell ss:StyleID=\"s70\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		if(m_param.plFlagMfr){
			encodeXMLstring(m_compArray[i]->m_mfr,outString);
			_ftprintf(fp,_T("   <Cell ss:StyleID=\"s70\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		}
		if(m_param.plFlagMfrPn){
			encodeXMLstring(m_compArray[i]->m_mfrpn,outString);
			_ftprintf(fp,_T("   <Cell ss:StyleID=\"s70\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		}
		if(m_param.plFlagPackage){
			encodeXMLstring(m_compArray[i]->m_package,outString);
			_ftprintf(fp,_T("   <Cell ss:StyleID=\"s70\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		}
		if(m_param.plFlagNote){
			encodeXMLstring(m_compArray[i]->m_note,outString);
			_ftprintf(fp,_T("   <Cell ss:StyleID=\"s72\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		}
		if(m_param.plFlagInfo){
			encodeXMLstring(m_compArray[i]->m_componentNote,outString);
			_ftprintf(fp,_T("   <Cell ss:StyleID=\"s72\"><Data ss:Type=\"String\">%s</Data></Cell>\n"),outString.c_str());
		}
		_ftprintf(fp,_T("  </Row>\n"));
		prefix	= m_compArray[i]->m_prefix;
		suffix	= m_compArray[i]->m_suffix;
		name	= m_compArray[i]->m_name;
	}
	_ftprintf(fp,_T(" </Table>\n"));
	_ftprintf(fp,_T("</Worksheet>\n"));
	_ftprintf(fp,_T("</Workbook>\n"));

	fclose(fp);
	return 0;
}
	
