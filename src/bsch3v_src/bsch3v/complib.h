/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef COMPLIB_H
#define COMPLIB_H

#include "coord.h"

#include "SPtnObj.h"
#include "SPtnObjArc.h"
#include "SPtnObjCircle.h"
#include "SPtnObjLine.h"
#include "SPtnObjPolygon.h"
#include "SPtnObjText.h"


class SReadCE3;
class SWriteCE3;

#ifdef _BSCH
	extern bool g_notSaveBitPtn;
#endif

#define PIN_LENGTH 10


////////////////////////////////////////////////////////////////////
//部品ライブラリに関係する定義


enum PIN_TYPE
{
	PIN_TYPE_NOMAL		=0x00,
	PIN_TYPE_SMALL		=0x01,
	PIN_TYPE_ZLENG		=0x02,
	PIN_TYPE_CLOCK		=0x04,
	PIN_TYPE_NEGATIVE	=0x08,
	PIN_TYPE_NUMHIDE	=0x10
};

enum PIN_LOC
{
	PIN_LOC_L			=0x0,
	PIN_LOC_T			=0x1,
	PIN_LOC_R			=0x2,
	PIN_LOC_B			=0x3
};
/*
enum VECTPTN
{
	VECTPTN_L			=1,
	VECTPTN_C			=2,
	VECTPTN_PG			=3,
	VECTPTN_AR			=4
};
*/



typedef SPtnObj* PSPtnObj;
typedef list<SPtnObj*> SPtnObjList;
typedef list<SPtnObj*>::iterator SPtnObjListIterator;

//LB3ライブラリの読み込みで使うパターンクラス
class SPtn
{
protected:
	unsigned char* readBmp(SReadCE3& rce3);//,int x,int y,int nBuffSize);
	//bool readLine(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readCircle(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readCircleFill(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readArc(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);
	//bool readPolygon(SReadCE3& rce3,int* vBuff,int vBuffCount,int* count);

	//bool writeLine(SWriteCE3& wce3,int* buff,int* count);
	//bool writeCircle(SWriteCE3& wce3,int* buff,int* count);
	//bool writeCircleFill(SWriteCE3& wce3,int* buff,int* count);
	//bool writeArc(SWriteCE3& wce3,int* buff,int* count);
	//bool writePolygon(SWriteCE3& wce3,int* buff,int* count);


public:
	TCHAR*			m_pszName;		//パターン名
	SSize			m_size;			//サイズ
	int				m_nBuffsize;	//バッファのサイズ
	PSPtnObj*		m_ppvPtn;		//ベクトルパターンオブジェクトのポインタ配列
	int				m_vPtnCount;
	unsigned char*	m_pBuff;		//バッファへのポインタ
//	int*			m_pvBuff;		//ベクトル形式へのポインタ
//	int				m_nvBuffsize;	//ベクトル形式のバッファのサイズ

	SPtn(){							//コンストラクタ
		m_pszName=NULL;
		m_nBuffsize=0;
		m_pBuff=NULL;
//		m_nvBuffsize=0;
//		m_pvBuff=NULL;
		m_size=SSize(0,0);
		m_vPtnCount=0;
		m_ppvPtn=NULL;
	}

	SPtn(const SPtn&);				//コピーコンストラクタ
	SPtn& operator=(const SPtn&);	//代入演算子	

	virtual ~SPtn();				//デストラクタ
	bool readCe3(SReadCE3& rce3);
	bool writeCe3(SWriteCE3& wce3,LPCTSTR pszName);
	void setSize(const SSize& sizePixel);
};



////////////////////////////////////////////////////////////////////
//ピンクラスの定義
class SPin
{
protected:
	int			m_nBlock;	//m_nBlockの数だけピン番号文字列配列を管理している
	TCHAR**		m_ppszNum;	//ピン番号文字列配列へのポインタ
	TCHAR*		m_pszName;	//ピン名へのポインタ
	int			m_nType;	//ピンのタイプ
	int			m_nLTRB;	//位置 LTRB
	int			m_nOffset;	//位置 オフセット
	unsigned long m_pinNameDisplayFlag; //ピン名個別表示フラグ
	unsigned	m_flags;	//ファイルに保存されない動作時のフラグ

	enum{
		FLAGS_NC = 0x0001
	};

	//bool m_pinNameDrawCache;
	TCHAR*	m_pszPinNameDraw;



protected:
	void deleteBuff();

public:
	SPin();					//コンストラクタ
	~SPin();
	SPin(const SPin&);				//コピーコンストラクタ
	SPin& operator=(const SPin&);	//代入演算子

	void SetPinNameDisplayFlag(unsigned long val){m_pinNameDisplayFlag = val;}
	unsigned long GetPinNameDisplayFlag(void ){return m_pinNameDisplayFlag;}

	
	void setBlock(int  nBlock);	//ブロック数をセットする。
	void setPinName(const TCHAR* name);				//ピン名をセットする。
	void setPinNum(int nBlock,const TCHAR* name);	//ピン番号をセットする。
	void setType(int type){ m_nType=type;}
	void setLtrb(int ltrb){ m_nLTRB=ltrb;}
	void setOffset(int offset){m_nOffset=offset;}
	const TCHAR* pinName() const {return m_pszName;}
	const TCHAR* pinNameDraw() const {return m_pszPinNameDraw;}
	const TCHAR* pinNum(int nBlock) const;
	int type() const {return m_nType;}
	int ltrb() const {return m_nLTRB;}
	int offset() const {return m_nOffset;}
	int block() const { return m_nBlock;}

	bool nc() const { return m_flags &  FLAGS_NC;}
	void set_nc(bool itisNC);

	void setupPinNameDraw();

	bool readCe3(SReadCE3& rce3,int block);
	bool writeCe3(SWriteCE3& rce3);
};

typedef SPin* PSPin;

typedef list<SPin*> SPinList;
typedef list<SPin*>::iterator SPinListIterator;


////////////////////////////////////////////////////////////////////
//部品情報クラスの定義
class SCompInfo
{
protected:
	void deleteBuff();

public:
	enum CITYPE{
		GENERAL			= 0,
		SHARED_BLOCK	= 1,	//主に複数ブロックの部品と一緒に使われる共通部分、オペアンプ、74ロジックの電源、リレーのコイル、スイッチの照光
		FIXED_BLOCK		= 2		//
	};

public:
	TCHAR*	m_pszRef;	//デフォルト部品番号
	SSize	m_size;		//部品のサイズ
	int		m_nBlock;	//パッケージあたりのブロック数
	int		m_nFixedBlock; //固定ブロック
	CITYPE	m_ciType;	
	SPtn*	m_pPtn;		//LB3から読み込んだパターンへのポインタ
	int		m_nPinCount;//ピン数
	PSPin*	m_ppPin;	//LB3から読み込んだピンクラスの配列へのポインタ
	wstring	m_note;		//ノート 2005/04/30
	wstring	m_mfr;		//製造者記入欄 2009/02/01
	wstring	m_mfrpn;	//製造者型式記入欄 2009/02/01
	wstring	m_pkg;		//パッケージ記入欄 2009/02/01

	SCompInfo(){		//コンストラクタ
		m_pszRef	=NULL;
		m_nBlock	=1;
		m_pPtn		=NULL;
		m_nPinCount	=0;
		m_ppPin		=NULL;
		m_note		=_T("");
		m_mfr		=_T("");
		m_mfrpn		=_T("");
		m_pkg		=_T("");
		m_nFixedBlock = 0;
		m_ciType	= CITYPE::GENERAL;
	}
	~SCompInfo();			//デストラクタ

	SCompInfo(const SCompInfo&);			//コピーコンストラクタ
	SCompInfo& operator=(const SCompInfo&);	//代入演算子

	bool readCe3(SReadCE3& rce3,wstring& name,wstring& ptnname);	//ファイルからの読み込み
	bool writeCe3(SWriteCE3& wce3,const TCHAR* pszName);

	void setCiType(CITYPE citype);
	void setRef(const TCHAR* ref);		//デフォルト部品番号の設定
	void setSize(const SSize& size,bool createPtn);	//サイズの設定 パターンを有する場合はパターンのサイズも変更
	void setUsePattern(bool usepattern);
	void setBlock(int block);
	void setFixedBlock(int fb);
	void addPin(SPin* ppin);
	void removePin(int ltrb,int offset,bool move);
	void insertNullPin(int ltrb,int offset);
	SPin* pinInfo(int ltrb,int offset);	//指定した位置のピン情報を得る
};

////////////////////////////////////////////////////////////////////
//部品インデックスクラスの定義
class SCompIndex
{
	TCHAR*				m_lpszName;	//部品名
public:
	SCompInfo*			m_pCompInfo;//対応する部品情報クラスへのポインタ

	SCompIndex(){			//コンストラクタ
		m_lpszName	=NULL;
		m_pCompInfo =NULL;
	}
	~SCompIndex(){			//デストラクタ
		delete[]m_lpszName;
	}
	void setName(const TCHAR* name);
	void setCompInfo(SCompInfo* pcinfo){m_pCompInfo = pcinfo;}

	SCompInfo* compInfo() const{return m_pCompInfo;}
	const TCHAR*	name() const{return m_lpszName;}			//部品名を得る
	const TCHAR* number() const;			//部品番号を得る

	const TCHAR* Mfr() const;			//製造者を得る		20090203
	const TCHAR* MfrPn() const;			//製造者型式を得る		20090203
	const TCHAR* Pkg() const;			//パッケージを得る		20090203

	SSize size() const;					//サイズを得る
	SSize sizePlace(int dir) const;		//配置時の図面上のサイズを得る
	int	  block() const;				//パッケージあたりのブロック数を得る
	SPtn*	pattern() const;			//パターンへのポインタを得る
	int	pinCount() const;				//ピンの数を得る
	SPin* pin(int index) const;			//ピンの情報を得る
	//ピンの情報を得る
	//向きの情報から、ピンの位置情報(LTRBと、原点からのオフセット)を返す
	SPin* pin(int index,int dir,int& nLtrb,SPoint& ptLoc) const;

};



typedef SCompIndex* PSCompIndex;

typedef list<SPtn*> SPtnList;
typedef list<SPtn*>::iterator SPtnListIterator;

typedef list<SCompInfo*> SCompInfoList;
typedef list<SCompInfo*>::iterator SCompInfoListIterator;

typedef list<SCompIndex*> SCompIndexList;
typedef list<SCompIndex*>::iterator SCompIndexListIterator;


////////////////////////////////////////////////////////////////////
//部品ライブラリクラス
class SCompLib
{
public:
	LPTSTR			m_lpszLibraryName;		//ライブラリ名
	wstring			m_prop_string;
	SPtnList		m_listPtn;				//読み込んだパターンのリスト
	SCompInfoList	m_listCompInfo;			//読み込み込んだ部品クラスのリスト
	SCompIndexList	m_listCompIndex;		//読み込み込んだ部品インデックスのリスト
	PSCompIndex*	m_ppCompIndex;			//読み込み込んだ部品インデックスのポインタ配列
	time_t			m_aux1;
	time_t			m_aux2;

	int				m_dataVersion;			//Version 0.80
static const int m_nConstDataVersion = 82;	//Version 0.80 ライブラリデータのバージョン



public:
	SCompLib(){										//コンストラクタ
		m_lpszLibraryName=NULL;
		m_ppCompIndex=NULL;
		m_prop_string=_T("");
		m_aux1 = 0;
		m_aux2 = 0;
		m_dataVersion = m_nConstDataVersion;
		//m_nCount=0;
	}
	~SCompLib();										//デストラクタ
	bool ReadLb3(SReadCE3& rce3,bool singleComponent = false);//LB3ライブラリの読み込み
	void deleteContent();//内容の破棄

	bool writeLibraryFile(LPCTSTR lpszFileName);	//ファイル書き込み
	bool readLibraryFile(LPCTSTR lpszFileName);		//ファイル読み込み

	void setLibraryName(LPCTSTR name);				//ライブラリ名の設定
	LPCTSTR libraryName() const ;					//ライブラリ名を得る
	int count() const{return m_listCompIndex.size();}	//管理部品数を得る
	SCompIndex* compIndex(int nIndexNum) const;			//n番目の部品のインデックスを得る

	static bool WriteAlias(SWriteCE3& wce3,LPCTSTR  alias,LPCTSTR  name);
	wstring uniqueName(const wstring& name);	//既存の部品名と重ならない名前を作る

private:
	bool ReadLb3Ptn(SReadCE3& rce3);//パターンの読み込み
	bool ReadLb3Comp(SReadCE3& rce3);
	bool ReadLb3Alias(SReadCE3& rce3);

	void clearPtnList();	//メンバー変数パターンリストのクリア


	bool isUniqueName(const wstring& name);	//既存の部品名と重複していないかどうか

	//bool WriteLb3Pin(SWriteCE3& wce3,const SPin& pin);							//ピンの書き込み
	//bool WriteLb3Ptn(SWriteCE3& wce3,const SPtn& ptn,const char*  name);			//パターンの書き込み
	//bool WriteLb3Comp(SWriteCE3& wce3,const SCompInfo& compinfo,SWriteCE3& wce3);	//部品の書き込み
	
};


#endif
