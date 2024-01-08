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

//�Q�Ɣԍ����Z�b�g����
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
	pos++;	//�v���t�B�b�N�X�̕�������������
	m_prefix = refnum.substr(0,pos);
	if((len-pos)>0){
		m_suffix = _tstoi(refnum.substr(pos,len-pos).c_str());
		m_validSuffix = true;
	}else{
		m_suffix = 0;
		m_validSuffix = false;
	}
}

//�u���b�N�ԍ����Z�b�g����
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


//���i�̃��t�@�����X�A�u���b�N�����A���R�[�h�ɏ����߂�
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

//�ۑ��Ɏ��s������0�ȊO��Ԃ�
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
		

//�o�[�W��������
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
	if(rce3.ReadRecord(str)==WEOF){	//��̃t�@�C���̂Ƃ��̏���
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
		return CE3FCHK_V080_OR_L;	//SheetInfo���������邩������Ȃ��B����SheetInfo��070�܂��͈ȑO�ɂ͂Ȃ��B
	}
	//Data Version 0.80�����̓��[�J���G���R�[�h�ƌ���
	if(sheetInfo.dataVersion()<80){
		return CE3FCHK_V070_OR_E;
	}else{
		return CE3FCHK_V080_OR_L;
	}
}

//�ǂݍ��݂Ɏ��s������0�ȊO��Ԃ�
//1:���łɓ����t�@�C����ǂݍ��݂���
//2:�ǂݍ��݃G���[
//3:�G���[����Ȃ����ǐV����BSch3V�ō��ꂽ�f�[�^��
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
	//���ɃI�[�v�������f�[�^�ɓ����̃t�@�C�����Ȃ����̃`�F�b�N
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
	//	//Data Version 0.70�ȉ��̓��[�J���G���R�[�h�ƌ���
	//	if(sheetInfo.dataVersion()<=70){
	//		fclose(fp);
	//		//�o�C�i�����[�h�ŊJ���Ȃ����B
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
		wstring strBak=createBackUpPath(path);//�o�b�N�A�b�v�t�@�C�������쐬����B
		_tremove(strBak.c_str());							//���łɓ����̃o�b�N�A�b�v�t�@�C��������ꍇ�ɁA���������
		_trename(path,strBak.c_str());					//���̃t�@�C�����o�b�N�A�b�v�t�@�C���ɉ���
	}
	return 1;
}

//Anum�ł̏����̂��߂̃\�[�g�̂��߂̔�r���[�`��
// �v���t�B�b�N�X
// keep����Ă���(newOnly�̂Ƃ�����
//    �d�����i�ASharedBlock�̕��i���O
// keep����Ă��Ȃ�����
//    ��d�����i�A��SharedBlock�̕��i���O
//    �t�@�C���ԍ����Ⴂ���̂��O
//    X���W
//    Y���W

int ComparePSNutCompForAnum( const void *p1, const void *p2 )
{
	PSNutComponent pElem1 = *(PSNutComponent*)(p1);
	PSNutComponent pElem2 = *(PSNutComponent*)(p2);

	int compValue;

	compValue= _tcscmp(pElem1->m_prefix.c_str(),pElem2->m_prefix.c_str());
	if(compValue !=0) return compValue;

	//keep����Ă�����̂��O
	if( pElem1->m_keep  &&  !pElem2->m_keep ) return -1;
	if(!pElem1->m_keep  &&   pElem2->m_keep ) return  1;

	if(pElem1->m_keep){	//keep���i���u�̔�r
		//�T�t�B�b�N�X�Ŕ�r
		compValue = pElem1->m_suffix - pElem2->m_suffix;
		if(compValue !=0) return compValue;
		//�d�����i���O
		if(  pElem1->m_powComp && !pElem2->m_powComp) return -1;
		if( !pElem1->m_powComp &&  pElem2->m_powComp) return  1;
		////blocks���傫�����̂��O
		//compValue = pElem1->m_blocks - pElem2->m_blocks;
		//if(compValue !=0) return compValue;
	}else{	//keep����Ă��Ȃ����̓��m�̔�r
		//��d�����i���O
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



////Prefix -> keep����Ă�����̂��O -> powComp�łȂ� -> �y�[�W -> X ->Y�Ŕ�r����B
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
//	//keep�t���O���ݒ肳��Ă�����̓��m�Ȃ�T�t�B�b�N�X�����������̂��O�ɗ���
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
//	//�T�t�B�b�N�X���ݒ肳��Ă��Ȃ����̓��m�Ȃ�A�y�[�W�A�ʒu�Ŕ�r����
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

//Prefix -> Suffix�Ŕ�r����B
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


//m_complist �̕��i����A�����Ώۂ�m_compArray�ɂ܂Ƃ߂�B
void SNut::pickupComponent(unsigned int option)
{

	bool newlyOnly = ((option & NUTOPTION_NEWONLY)!=0);
	bool ignore_layer = ((option & NUTOPTION_EN_LAYER)==0);

	NutCompList listCompP;

	NutCompListIterator ite = m_complist.begin();
	while(ite != m_complist.end()){
		SNutComponent* pComp = *ite;
		ite++;
		if(pComp->m_prefix.size()==0) continue;	//���i�ԍ��v���t�B�N�X�̂Ȃ����͖̂���
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
			if(prefixHit && m_prefixFilterDontMode) continue;	//���O���[�h�Ńv���t�B�b�N�X���q�b�g�����疳��
		    if(!prefixHit && !m_prefixFilterDontMode) continue;	//���胂�[�h�Ńv���t�B�b�N�X���q�b�g���Ȃ��Ƃ��͖���
		}


		//�d�����i���ǂ����̔��f
		//orgname��4�����ȏ�ŁA�擪��"VCC","VDD","GND","VEE","VSS"�̂Ƃ��d�����i�Ƃ���B
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




		//���̃v���t�B�b�N�X�̎n�܂���݂��Ă���
		nextPrefixStartPos++;
		while(nextPrefixStartPos<m_compArrayCount){
			if(m_compArray[nextPrefixStartPos]->m_prefix != currentPrefix) break;
			nextPrefixStartPos++;
		}
		//keep����Ă��Ȃ��O���[�v�̎n�܂���݂��Ă���
		notkeepStart = prefixStartPos;
		while(notkeepStart<nextPrefixStartPos){
			if(!m_compArray[notkeepStart]->m_keep) break;
			notkeepStart++;
		}


		if(currentPrefix==_T("U")){
			TRACE(_T("\n"));
		}

		if(newonly){
			//�����̓d�����i�Ɋ֘A�t�����s��
			for(int i=prefixStartPos;i<notkeepStart;i++){
				//if(m_compArray[i]->m_set){i++; continue;}
				if(m_compArray[i]->m_powComp){//keep�O���[�v�ɓd�����i������������
					bool setFunctionComponent = false;	//�d�����i�ɋ@�\���i���֘A�t�����Ă�����true�ɂȂ�B

					//�����̋@�\���i�Ƃ̊֘A�t�������݂�
					for(int j=i+1;j<notkeepStart;j++){
						if(m_compArray[i]->m_suffix == m_compArray[j]->m_suffix && !m_compArray[j]->m_powComp){
							m_compArray[j]->m_setPowComp = true;
							setFunctionComponent = true;
						}
					}

					//�����̋@�\���i�Ɗ֘A�Â����ł��Ȃ������ꍇ�͐V�K���i�Ƃ̊֘A�Â����s��
					//�����Ŋ֘A�Â����V�K���i��keep�t���O�𗧂ĂāA�������i�Ɠ��������Ƃ���B
					if(!setFunctionComponent){
						for(int j=notkeepStart;j<nextPrefixStartPos;j++){
							if(m_compArray[j]->m_keep)continue;
							if(m_compArray[j]->m_powComp)continue;
							if(m_compArray[i]->m_name == m_compArray[j]->m_name){
								m_compArray[j]->m_suffix = m_compArray[i]->m_suffix;
								m_compArray[j]->m_block = 0;
								m_compArray[j]->m_keep = true;	//���������� keep�t���O�𗧂ĂĂ��� ���������� 
								m_compArray[j]->m_setPowComp = true;
								setFunctionComponent = true;
								break;
							}
						}
					}
				}
			}

			//�����̕����u���b�N���i�𖄂߂�
			//�O�̃X�e�b�v�ŁA�����d�����i�Ɗ֘A�Â���ꂽ�V�K���i���ΏۂƂȂ邽�߁A
			//���[�v�͌��݂̃v���t�B�b�N�X�S�̂ɋy��
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

					//�������i�Ŗ��߂邱�Ƃ����݂�
					//�T�t�B�b�N�X�������Ȃ�OK�Ƃ���B
					//���O��u���b�N�ԍ��A�u���b�N���ɖ������Ȃ����Ƃ͊����ԍ��̐ӔC
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

					//�������i�Ŗ��߂��Ȃ��ꍇ�͐V�K���i�Ŗ��߂�
					//���O����v�����OK�Ƃ���B
					//�u���b�N���ɖ������Ȃ����Ƃ͍�}�̐ӔC
					int unusedBlock =0;
					for(int j=notkeepStart; j<nextPrefixStartPos && blocksetcount<blocks; j++){
						if(m_compArray[j]->m_checked) continue;
						if(m_compArray[j]->m_powComp) continue;
						if(m_compArray[i]->m_name == m_compArray[j]->m_name){
							m_compArray[j]->m_suffix = m_compArray[i]->m_suffix;
							//���g�p�u���b�N�ԍ���T��
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

					//�������i�Ŗ��߂邱�Ƃ����݂�
					//�T�t�B�b�N�X�������Ȃ�OK�Ƃ���B
					//���O��u���b�N�ԍ��ɖ������Ȃ����Ƃ͊����ԍ��̐ӔC
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

					//�������i�Ŗ��߂��Ȃ��ꍇ�͐V�K���i�Ŗ��߂�
					//���O����v�����OK�Ƃ���B
					//�u���b�N���ɖ������Ȃ����Ƃ͍�}�̐ӔC
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





		//�V�K���i�̓d�����i�ȊO�̏���
		//�僋�[�v�͏�����Ԃ̐V�K���i�͈̔͂��������A�d�����i�Ƃ̊֘A�Â���keep�t���O�𗧂ĂĂ���ꍇ������B
		//���̂��߁A���̃��[�v�ł�keep�t���O���`�F�b�N���āA�t���O�������Ă�����̂͏��O���Ă���B
		int unusedSuffix=init;
		for(int i=notkeepStart;i<nextPrefixStartPos;i++){
			if(m_compArray[i]->m_checked) continue;
			if(m_compArray[i]->m_powComp) continue;
			if(m_compArray[i]->m_keep)continue;		//keep�t���O�������Ă�����̂͏������Ȃ�
			TRACE(_T("newly suffix %d\n"),m_compArray[i]->m_suffix);

			if(newonly){
				//�������i�Ŏg���Ă��Ȃ��T�t�B�b�N�X�𓾂�
				bool used = true;
				while(used){
					used = false;
					for(int j=prefixStartPos;j<notkeepStart;j++){
						if(m_compArray[j]->m_suffix == unusedSuffix){
							used = true;
							unusedSuffix += step;	//�g���Ă�����X�e�b�v�Ԃ�i�߂čēx�`�F�b�N��
							break;
						}
					}
				}
			}

			m_compArray[i]->m_checked = true;
			m_compArray[i]->m_block = 0;
			m_compArray[i]->m_suffix = unusedSuffix;

			//�����u���b�N���i�͖��߂�
			if(m_compArray[i]->m_blocks>1){
				int blocks = m_compArray[i]->m_blocks;
				int block = 1;
				for(int j=i+1;j<nextPrefixStartPos && block<blocks;j++){
					if(m_compArray[j]->m_checked) continue;
					if(m_compArray[j]->m_powComp) continue;
					if(m_compArray[i]->m_keep)continue;		//keep�t���O�������Ă�����̂͏������Ȃ�
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
					if(m_compArray[i]->m_keep)continue;		//keep�t���O�������Ă�����̂͏������Ȃ�
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


		//�V�K���i�̓d�����i�̏���
		for(int i=notkeepStart;i<nextPrefixStartPos;i++){
			if(!m_compArray[i]->m_powComp) continue;
			if(m_compArray[i]->m_keep)continue;
			TRACE(_T("newly powercomp suffix %d\n"),m_compArray[i]->m_suffix);
			int suffix;
			bool setSuffix = false;
			//�ݒ�ς݂̓d�����i�ȊO���X�L�������Ċ֘A�t�����ł�����̂�T��
			for(int j=prefixStartPos; j<i; j++){
				if(m_compArray[j]->m_powComp) continue;
				if(m_compArray[j]->m_setPowComp) continue; //���łɓd�����i�Ɗ֘A�t�����Ă�����̂͏��O
				if(m_compArray[j]->m_name == m_compArray[i]->m_name){
					if(!setSuffix){		//�ŏ��Ɋ֘A�t���ł���@�\���i��������܂ł̏���
						suffix = m_compArray[j]->m_suffix;
						m_compArray[j]->m_setPowComp = true;
						setSuffix = true;		//�u���������v�t���O
					}else{				//�������񌩂�������A�������O�œ������i�ԍ��̋@�\���i�Ɋ֘A�t���t���O�𗧂ĂĂ���
						if(suffix == m_compArray[j]->m_suffix){
							m_compArray[j]->m_setPowComp = true;
						}
					}
				}
			}
			if(setSuffix){	//�@�\���i�Ƃ̊֘A�t�����ł����ꍇ
				m_compArray[i]->m_suffix = suffix;
			}else{			//�@�\���i�Ƃ̊֘A�t�����ł��Ȃ������ꍇ
				if(newonly){
					bool used = true;
					while(used){
						used = false;
						//�������i�Ŏg���Ă��Ȃ��T�t�B�b�N�X�𓾂�
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
		if(m_compArray[n]->m_prefix != currentPrefix){	//�X�L�������Ă��ăv���t�B�b�N�X���ς������
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
