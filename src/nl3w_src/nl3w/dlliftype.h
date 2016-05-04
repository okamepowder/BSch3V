/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#ifndef DLLIFTYPE_H
#define DLLIFTYPE_H

//DLL�ɓn���s���l�\���� (DLL�ɂ͉��L�̃l�b�g���X�g�\���̂Ɋ܂܂��)
struct SExportPinValue{
	const char* pszRefNum;
	const char* pszPinNum;
};

//DLL�ɓn���V���O���l�b�g�f�[�^�\����
struct SExportSingleNetValue{
	const char* pszNetName;
	int nPinCount;
	SExportPinValue* pArrayPinValue;
};

typedef SExportSingleNetValue* PSExportSingleNetValue;

//DLL�ɓn�����i���l�\����
struct SExportComponentInfo{
	const char* pszPrefix;	//���i�ԍ��̃v���t�B�N�X
	int   nSuffix;		//���i�ԍ��̃T�t�B�b�N�X
	const char* pszName;		//���i��
	const char* pszPackage;	//�p�b�P�[�W��
};

//typedef SExportComponentInfo* PSExportComponentInfo;

struct SExportNetlistInfo{
	int nComponentInfoCount;	//�i�[�������i���̐�
	SExportComponentInfo* array_CompInfo;	//���i���

	int nNetValueCount;			//�i�[�����l�b�g���̐�
	SExportSingleNetValue* array_NetValue;	//�l�b�g���
};



#endif