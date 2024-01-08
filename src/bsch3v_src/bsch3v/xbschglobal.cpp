/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "shlwapi.h"
#ifdef _BSCHCAPTURE
	#include "bsch.h"
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <list>
#include <string>
using namespace std;

//#include <qstring.h>
//#include <qcstring.h>

#include "xbsch.h"
#include "coord.h"
#include "xbschglobal.h"
#include "complib.h"
//#include "cfgdata.h"


////////////////////////////////////////////////////////////////////////
//// �啶������������ʂ��Ȃ��������r
//// Visual C++ �ɂ� stricmp�����邪�W����C++�ɂ͖����悤�Ȃ̂ł����Œ�`
//int strcmp_i(const char* s1,const char* s2)
//{
//	int c1;
//	int c2;
//
//	while(1){
//		if(*s1=='\0' && *s2=='\0') return 0; 
//		c1 = toupper(*s1++);
//		c2 = toupper(*s2++);
//		if(c1 != c2) return c1-c2;
//	}
//}



//////////////////////////////////////////////////////////////////////
//�R���t�B�M�����[�V�����t�@�C���֌W
//SCfgData g_cfg;
//string g_strCfgFileName;	//�R���t�B�M�����[�V�����t�@�C����

//////////////////////////////////////////////////////////////////////
//���C�u�����֘A
static SCompLib* g_pCompLib[MAX_LIB];	//���i���C�u����
static int s_libraryCount=0;

#ifdef _BSCHCAPTURE

//���C�u�����̏��̍����ւ�
bool g_ReplaceLibrary(int index,SCompLib* pCompLib)
{
	if(index<0 || index>=s_libraryCount) return false;
	if(!pCompLib) return false;

	delete g_pCompLib[index];
	g_pCompLib[index] = pCompLib;

	return true;
}



//�R���t�B�M�����[�V�����t�@�C�����烉�C�u�����̏��𓾂ēǂݍ���
bool g_ReadLibrary()
{
//	string filename;
	int n;
	TCHAR szKey[32];

	for(n=0;n<MAX_LIB;n++) g_pCompLib[n]=NULL;
	int entryNum=0;
	n=0;
	while(entryNum<MAX_LIB){
		wsprintf(szKey,_T("LIB%d"),entryNum);			//�G���g��������̍쐬
		entryNum++;
		CString strPath=AfxGetApp()->GetProfileString(_T("Library"),szKey);//���C�u�������̎擾
		if(strPath.IsEmpty()){		//������i���C�u�������j���l���ł��Ȃ�������break
			break;
		}else{
			if(PathIsRelative(strPath)){
				TCHAR fullPath[_MAX_PATH];
				PathCombine(fullPath,((CBSchApp*)AfxGetApp())->m_szIniFileDir,strPath);
				strPath = fullPath;
			}
			g_pCompLib[n]=new SCompLib;
			if(!g_pCompLib[n]->readLibraryFile(strPath)){
				delete g_pCompLib[n];
				g_pCompLib[n]=NULL;
			}else{
				n++;
			}
		}
	}
	s_libraryCount = n;
	return true;
}

//���C�u�����̃������̉�����s��
void g_FreeLibrary()
{
	int n;
	for(n=0;n<s_libraryCount;n++){
		delete g_pCompLib[n];
		g_pCompLib[n] = NULL;
	}
	while(n<MAX_LIB) g_pCompLib[n++]=NULL;
	s_libraryCount=0;
}

//���C�u�������畔�i�C���f�b�N�X�𓾂�
const SCompIndex* g_SearchComponentIndex(const TCHAR* pszName,int *pnLib,int *pnIndex,SCompLib* pLibOptional)
{
	int i,n;
	int nCount;
	const SCompIndex* pCompIndex;
	for(i=0;i<=s_libraryCount;i++){
		SCompLib* pLib = NULL;
		if(i<s_libraryCount){
			pLib = g_pCompLib[i];
		}else{
			pLib=pLibOptional;
		}
		if(pLib==NULL) return NULL;
		nCount=pLib->count();	//���C�u�����ɓo�^����Ă��镔�i��
		for(n=0;n<nCount;n++){
			pCompIndex=pLib->compIndex(n);
			if(!_tcsicmp(pszName,pCompIndex->name())){
				if(pnLib)  *pnLib=i;
				if(pnIndex) *pnIndex=n;
				return pCompIndex;
			}
		}
	}
	return NULL;
}

//���C�u��������_�����]���i�̃C���f�b�N�X�𓾂�
const SCompIndex* g_SearchLogicalInvertComponentIndex(const TCHAR* pszName)
{
	if(pszName==NULL)return NULL;

	//���̕��i�̃��C�u�����ԍ��ƃC���f�b�N�X���̎擾
	int nCompLibNum;
	const SCompIndex* pIndex		= g_SearchComponentIndex(pszName,&nCompLibNum,NULL,NULL);
	if(pIndex == NULL) return NULL;

	wstring strInvertName=pszName;
	int n=strInvertName.length();
	if(n>1 && (strInvertName[n-1]=='i' || strInvertName[n-1]=='I')){
		//������i�Ȃ疖��������
		strInvertName=strInvertName.substr(0,n-1);
	}else{
		//������i�łȂ����i��ǉ�
		strInvertName=strInvertName+_T("i");
	}

	int nInvCompLibNum;
	const SCompIndex* pInvIndex	= g_SearchComponentIndex(strInvertName.c_str(),&nInvCompLibNum,NULL,NULL);
	if(pInvIndex == NULL || nInvCompLibNum != nCompLibNum) return NULL;


	//���̑��K�������̃`�F�b�N
	//�s����
	if(pIndex->pinCount() != pInvIndex->pinCount()) return NULL;

	//�u���b�N��
	if(pIndex->block() != pInvIndex->block()) return NULL;

	//���i�T�C�Y
	if(pIndex->size() != pInvIndex->size()) return NULL;	

	return pInvIndex;
}

//�Ǘ����Ă��郉�C�u�����̐���Ԃ�
int g_LibraryCount(){return s_libraryCount;}

//���C�u�����ւ̃|�C���^��Ԃ�
const SCompLib* g_LibraryInfo(int nLib){
	if(nLib>=0 && nLib<s_libraryCount) return g_pCompLib[nLib];
	else return NULL;
}
#else
//���C�u�������畔�i�C���f�b�N�X�𓾂�
const SCompIndex* g_SearchComponentIndex(const TCHAR* pszName,int *pnLib,int *pnIndex,SCompLib* pLibOptional)
{
	return NULL;
}

#endif