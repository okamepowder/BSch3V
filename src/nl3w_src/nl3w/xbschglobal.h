/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHGLOBAL_H
#define XBSCHGLOBAL_H

//#include "qstring.h"
//#include "cfgdata.h"

#define MAX_LIB 64	// ���C�u�����̍ő吔

class SCompIndex;
class SCompLib;

//////////////////////////////////////////////////////////////////////
// �啶������������ʂ��Ȃ��������r
int strcmp_i(const char* s1,const char* s2);

//////////////////////////////////////////////////////////////////////
//�R���t�B�M�����[�V�����t�@�C���֌W
//extern SCfgData g_cfg;
//extern string g_strCfgFileName;

//////////////////////////////////////////////////////////////////////
//���C�u�����֘A

//���C�u�����̏��̍����ւ�
bool g_ReplaceLibrary(int index,SCompLib* pCompLib);

//�R���t�B�M�����[�V�����t�@�C�����烉�C�u�����̏��𓾂ēǂݍ���
bool g_ReadLibrary();

//���C�u�����̃������̉�����s��
void g_FreeLibrary();

//���C�u�������畔�i�C���f�b�N�X�𓾂�
const SCompIndex* g_SearchComponentIndex(const char* pszName,int* pnLib,int* pnIndex,SCompLib* pLibOptional);

//���C�u��������_�����]���i�̃C���f�b�N�X�𓾂�
const SCompIndex* g_SearchLogicalInvertComponentIndex(const char* pszName);

int g_LibraryCount();
const SCompLib* g_LibraryInfo(int nLib); 

#endif