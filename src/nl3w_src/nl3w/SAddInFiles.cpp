#include "stdafx.h"

#include "SAddInFiles.h"

SAddInFiles::SAddInFiles(void)
{
	m_nAddinCount = 0;
	for(int i=0;i<MAX_ADDIN;i++){
		m_hAddin[i]=NULL;
	}
}

SAddInFiles::~SAddInFiles(void)
{
	ReleaseAddin();


}

void SAddInFiles::ReleaseAddin(void)
{
	for(int i=0;i<MAX_ADDIN;i++){
		if(m_hAddin[i]==NULL){
			::FreeLibrary(m_hAddin[i]);
			m_hAddin[i] = NULL;
		}
	}
	m_nAddinCount = 0;
}

void SAddInFiles::update(void)
{
	ReleaseAddin();

	int i;
	for(i=0;i<MAX_ADDIN;i++){
		TCHAR szEntry[32];
		wsprintf(szEntry,_T("ADDIN%d"),i);			//�G���g��������̍쐬
		CString str=AfxGetApp()->GetProfileString(_T("ADDINLIB"),szEntry);
		if(str.IsEmpty()) break;		//������i�A�h�C�����j���l���ł��Ȃ�������break

		HINSTANCE hDll = ::LoadLibrary(str);
		if(!hDll) break;
		if(!::GetProcAddress( hDll,"typeName") || !::GetProcAddress(hDll,"fnWriteNetList")){
			::FreeLibrary(hDll);
			break;
		}
		m_hAddin[i]=hDll;
	}
	m_nAddinCount = i;

}

SAddInFiles g_addinFiles;
