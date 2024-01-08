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
		wsprintf(szEntry,_T("ADDIN%d"),i);			//エントリ文字列の作成
		CString str=AfxGetApp()->GetProfileString(_T("ADDINLIB"),szEntry);
		if(str.IsEmpty()) break;		//文字列（アドイン名）が獲得できなかったらbreak

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
