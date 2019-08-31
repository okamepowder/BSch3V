#include "stdafx.h"

#include "stdafx.h"
#include "global.h"
#include <stdio.h>
#include <assert.h>
#include <string>
#include <ctype.h>
using namespace std;
#include "SMarkupTextLine.h"

//SMarkupTextLine::SMarkupTextLine(void)
//{
//	m_strText = "";
//	m_fontName = "";
//	m_textSyle = (unsigned int)SMarkupTextLine::NO_STYLE;
//	m_charFlags = NULL;
//
//}

//SMarkupTextLine::~SMarkupTextLine(void)
//{
//	delete[]m_charFlags;
//}

typedef stack<wstring> StringStack;

// &�L���Ŏn�܂�G���R�[�h��������͂���B
// ��͌��ʂ�*decodeChar�ɓ���B
// ��T�|�[�g�G���R�[�h�̏ꍇ�́A*decodeChar�ɂ�&�L��������
// �߂�l�̃|�C���^�́A��͂����������Ƃ��́A;�L���̎��̕����A
//                     ��T�|�[�g�G���R�[�h�̂Ƃ��́A&�L���̎��̕���
static const TCHAR* analyzeAmpEncode(const TCHAR* src, wstring *decodeString,bool printMode)
{
	*decodeString = _T("&");
	const TCHAR* pc = src+1;
	const TCHAR* start = pc;
	int n=0;
	while(*pc!='\0' && *pc!=';'){
		n++;
		pc++;
	}
	if(*pc=='\0'){
		return src+1;
	}
	n++;
	if(_tcsncmp(start,_T("lt;"),n)==0){
		*decodeString = _T("<");
	}else
	if(_tcsncmp(start,_T("gt;"),n)==0){
		*decodeString = _T(">");
	}else
	if(_tcsncmp(start,_T("amp;"),n)==0){
		*decodeString = _T("&");
	}else
	if(_tcsncmp(start,_T("quot;"),n)==0){
		*decodeString = _T("\"");
	}else
	if(_tcsncmp(start,_T("DocPath;"),n)==0){
		*decodeString = g_docPath;
	}else
	if(_tcsncmp(start,_T("DocFileName;"),n)==0){
		TCHAR fname[_MAX_FNAME];
			::_tsplitpath(g_docPath.c_str(),NULL,NULL,fname,NULL);
		*decodeString = fname;
	}else
	if(_tcsncmp(start,_T("SaveTime;"),n)==0){
		*decodeString = g_docTimeStamp;
	}else
	if(_tcsncmp(start,_T("SaveDate;"),n)==0){
		*decodeString = g_docDate;
	}else
	if(printMode && (_tcsncmp(start,_T("PrintTime;"),n)==0)){
		*decodeString = g_printTime;
	}else{
		return src+1;
	}
	return pc+1;
}




static void setCharFlags(const TCHAR* src, int length, unsigned int* charFlags)
{
	int n;

	for(n=0;n<length+1;n++){
		charFlags[n]=0;
	}
	n=0;
	//TCHAR prevchar=src[0];
	//bool prevIsSjis1st = false;
	while(src[n]){
		TCHAR b = src[n];
		if(b==' '){
			charFlags[n] = (unsigned int)SMarkupTextLine::BREAKABLE;
		}else
		
		if(b<=0x04f9){
		
		}else

	
		if(g_bJapaneseLangEnv){

			const TCHAR* JPN_NON_BREAKABLE = _T("����ʃ������A�B�C�D�F�G�X�n�r�t�v�x�z�l�j�p�f�h�I�H"); 
			const int LENGTH_JPN_NON_BREAKABLE = 26;
			bool  non_breakable_flag = false;

			for(int ccnt = 0; ccnt<26; ccnt++){
				if(JPN_NON_BREAKABLE[ccnt]==b){
					non_breakable_flag = true;
					break;
				}
			}
			if(!non_breakable_flag){
				charFlags[n] = (unsigned int)SMarkupTextLine::BREAKABLE;
			}
		}
		n++;
	}
}


// < �Ŏn�܂�^�O�̉�͂��s��
// �߂�l�̓T�|�[�g�^�O�̂Ƃ��́A   >�L���̎��̕����B
//         ��T�|�[�g�^�O�̂Ƃ��́A <�L�����|�C���g�B �����̓��삪analyzeAmpEncode()�ƈقȂ�
static const TCHAR* analyzeTag(const TCHAR* src, unsigned int *pType,bool* pCloseTag,  wstring* pstrOption)
{
	*pType = (unsigned int)SMarkupTextLine::NO_STYLE;
	*pstrOption = _T("");
	*pCloseTag = false;

	const TCHAR* pc = src;
	pc++;
	//pc�͊J�����ʂ̎��̕������|�C���g


	//�J�����ʂɑ����X�y�[�X��ǂݔ�΂�
	while(iswspace(*pc) && *pc!='\0'){
		pc++;
	}

	if(*pc=='\0')return src;


	if(*pc == '/'){
		*pCloseTag = true;
		pc++;
	}

	const TCHAR* start = pc;

	int n=0;
	while(*pc && *pc != '>'){
		n++;
		pc++;
	}
		
	//�����ʂ�������O�ɏI�[�ɒB������߂�B
	if(*pc=='\0') return src;

	const TCHAR* next = pc+1;	//next�͕����ʂ̎��̕������|�C���g����B
	
	pc--;
	//pc�͕����ʂ̒��O�̕������|�C���g

	//�����ʂ̑O�̃X�y�[�X���폜
	while(iswspace(*pc) && n>0){
		n--;
		pc--;
	}

	if(n==1){
		if(*start == 'O'){
			*pType = (unsigned int)SMarkupTextLine::OVERBAR;
		}else
		if(*start == 'U'){
			*pType = (unsigned int)SMarkupTextLine::UNDERLINE;
		}else
		if(*start == 'B'){
			*pType = (unsigned int)SMarkupTextLine::BOLD;
		}else
		if(*start == 'I'){
			*pType = (unsigned int)SMarkupTextLine::ITALIC;
		}else{
			*pType = (unsigned int)SMarkupTextLine::NO_STYLE;
		}
		//return next;
	}else if(n==3){
		if(_tcsncmp(start,_T("SUB"),3)==0){
			*pType = (unsigned int)SMarkupTextLine::SUB;
		}else
		if(_tcsncmp(start,_T("SUP"),3)==0){
			*pType = (unsigned int)SMarkupTextLine::SUP;
		}else{
			*pType = (unsigned int)SMarkupTextLine::NO_STYLE;
		}
		//return next;
	}else if(_tcsncmp(start,_T("FONT"),4)==0){
		if(*pCloseTag){
			if(n==4){
	            *pType = (unsigned int)SMarkupTextLine::FONT_NAME;
				//return next;
			}
		}else{
			start+=4;
			//start��"FONT"�ɑ����������|�C���g
			if(iswspace(*start)){
				n-=4;
				while(iswspace(*start) && n>0){
					start++;
					n--;
				}
				if(n>0){
					*pstrOption=wstring(start,n);
				}
	            *pType = (unsigned int)SMarkupTextLine::FONT_NAME;
				//return next;
			}
		}
	}
	if(*pType == (unsigned int)SMarkupTextLine::NO_STYLE){
		return src;
	}else{
		return next;
	}
}




///�^�O�t���e�L�X�g�\�[�X���}�[�N�A�b�v�P�ʂɕ�������B
void g_CreateMarkupTextLines(const TCHAR* src,MarkupTextLineList* pMarkupTextLineList,bool printMode)
{
	MarkupTextLineIterator ite;
	unsigned int textStyle = 0;;
	StringStack fontNameStack;
	wstring currentFont = _T("");
	SMarkupTextLine* pmt = new SMarkupTextLine();


	//�}�[�N�A�b�v�e�L�X�g���C�����X�g�̑S����
	ite = pMarkupTextLineList->begin();
	while(ite != pMarkupTextLineList->end()){
		delete(*ite);
		ite++;
	}
	pMarkupTextLineList->clear();
	
	//int charCount = 0;
	//string tempString = "";
	while(*src != '\0'){
		if(*src=='<'){
			unsigned int changedStyle = textStyle;
			bool fontChanged = false;
			bool closeTag;
			unsigned int tagType;
			wstring fontName;
			src = analyzeTag(src,&tagType,&closeTag,&fontName);

			if(tagType!=SMarkupTextLine::NO_STYLE){
				if(closeTag){
					if(tagType==(unsigned int)SMarkupTextLine::FONT_NAME){
						if(!fontNameStack.empty()){
							currentFont =fontNameStack.top(); 
							fontNameStack.pop();
							fontChanged = true;
						}
					}else{
						changedStyle &= ~tagType;
					}
				}else{
					if(tagType==(unsigned int)SMarkupTextLine::FONT_NAME){
						if(!fontName.empty()){
							fontNameStack.push(currentFont);
							currentFont = fontName;
							fontChanged = true;
						}
					}else{
						if(tagType == (unsigned int)SMarkupTextLine::SUB){
							changedStyle &= ~(unsigned int)SMarkupTextLine::SUP;
							changedStyle |= (unsigned int)SMarkupTextLine::SUB;
						}else if(tagType == (unsigned int)SMarkupTextLine::SUP){
							changedStyle &= ~(unsigned int)SMarkupTextLine::SUB;
							changedStyle |= (unsigned int)SMarkupTextLine::SUP;
						}else{
							changedStyle |= tagType;
						}
					}
				}
				if(fontChanged || (changedStyle != textStyle)){
					textStyle = changedStyle; 
					if(pmt->m_strText.length()>0){
						pMarkupTextLineList->push_back(pmt);
						pmt = new SMarkupTextLine();
					}					
					pmt->m_fontName = currentFont;
					pmt->m_textSyle = textStyle;
				}
				continue;	//�^�O�Ƃ��ĔF���ł����ꍇ�͕����Ƃ��ēo�^���Ȃ��B����ȊO�͉��֔����ĕ����Ƃ��ēo�^
			}
		}
		if(*src=='\n'){
			pmt->m_textSyle |= (unsigned int)SMarkupTextLine::CR;
			pMarkupTextLineList->push_back(pmt);
			pmt = new SMarkupTextLine();
			pmt->m_fontName = currentFont;
			pmt->m_textSyle = textStyle;
			src++;
		}else if(*src!='\r'){
			TCHAR c;
			if(*src == '&'){
				wstring str;
				src = analyzeAmpEncode(src,&str,printMode);
				pmt->m_strText.append(str);
			}else{
				c = (iswspace(*src) ? ' ' : *src);
				src++;
				pmt->m_strText += c;
			}
		}else{
			src++;
		}
	}
	if(pmt->m_strText.length()>0){
		pMarkupTextLineList->push_back(pmt);
	}
	ite = pMarkupTextLineList->begin();
	while(ite != pMarkupTextLineList->end()){
		int length = (*ite)->m_strText.length();
		(*ite)->m_charFlags =(unsigned int*) new int[length+1];
		setCharFlags((*ite)->m_strText.c_str(),length, (*ite)->m_charFlags);
//#ifdef _DEBUG
//#ifdef _MFCBSCH
//		TRACE("%s STYLE:%04X FONT:%s\n",(*ite)->m_strText.c_str(),(*ite)->m_textSyle,(*ite)->m_fontName.c_str());
//		for(int dn = 0;dn<length;dn++){
//			TRACE("%d",((*ite)->m_charFlags)[dn]);
//		}
//		TRACE("\n");
//#endif
//#endif
		ite++;
	}

}
