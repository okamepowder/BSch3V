/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素部品クラス
****************************************************************************/
#ifndef XBSCHCOMPONENT_H
#define XBSCHCOMPONENT_H

#include "xbsch.h"
#include "xbschobj.h"
#include "complib.h"



//    (0) 0      (1) 90     (2) 180    (3) 270
//     →         ←         ←         →
//    ●Ｔ○     ○Ｌ●     ○Ｂ○     ○Ｒ○     
//  ↓Ｌ↑Ｒ↓ ↓Ｂ→Ｔ↓ ↑Ｒ↓Ｌ↑ ↑Ｔ←Ｂ↑
//    ○Ｂ○     ○Ｒ○     ○Ｔ●     ●Ｌ○     
//     →         ←         ←         →
//
//    (4) M0     (5) M90    (6) M180   (7) M270
//     ←         →         →         ←
//    ○Ｔ●     ●Ｌ○     ○Ｂ○     ○Ｒ○     
//  ↓Ｒ↑Ｌ↓ ↓Ｔ←Ｂ↓ ↑Ｌ↓Ｒ↑ ↑Ｂ←Ｔ↑
//    ○Ｂ○     ○Ｒ○     ●Ｔ○     ○Ｌ●     
//     ←         →         →         ←


class SXBSchComponent:public SXBSchObj
{
public:
	enum BSCHCOMP_DIR{
		COMPONENT_DIR_0		=0,
		COMPONENT_DIR_90	=1,
		COMPONENT_DIR_180	=2,
		COMPONENT_DIR_270	=3,
		COMPONENT_DIR_M0	=4,
		COMPONENT_DIR_M90	=5,
		COMPONENT_DIR_M180	=6,
		COMPONENT_DIR_M270	=7
	};

	enum BSCHCOMP_TEXTPOS{
		COMPONENT_DEFAULT_REFNUM_X	=2,
		COMPONENT_DEFAULT_REFNUM_Y	=10,
		COMPONENT_DEFAULT_NAME_X	=2,
		COMPONENT_DEFAULT_NAME_Y	=20
	};

	enum BSCHCOMP_PINLTRB{
		COMPONENT_HASLPIN	=1,
		COMPONENT_HASTPIN	=2,
		COMPONENT_HASRPIN	=4,
		COMPONENT_HASBPIN	=8,
	};

	//コンストラクタ
	SXBSchComponent();
	//デストラクタ
	virtual ~SXBSchComponent();
	SXBSchComponent(const SXBSchComponent&);				//コピーコンストラクタ
	SXBSchComponent& operator=(const SXBSchComponent&);	//代入演算子
	bool readEmbLibCe3(SReadCE3&, SCompInfo*,wstring& );
	bool writeEmbLibCe3(SWriteCE3& wce3);

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

    //IDの取得
	unsigned id(){return ID_COMPONENT;}
	unsigned objectFilterValue(){return OBJMASK_COMPONENT;} 
protected:
	wstring	m_name;			//名前
	SPoint	m_name_pos;		//名前の原点からのオフセット
	bool	m_name_hide;	//名前非表示
	bool	m_name_drawSizeIsValid;
	SSize	m_name_drawSize;
	bool	m_name_horiz;
	wstring	m_refnum;		//参照番号
	SPoint	m_refnum_pos;	//参照番号の原点からのオフセット
	bool	m_refnum_hide;	//番号非表示
	bool	m_refnum_drawSizeIsValid;
	SSize	m_refnum_drawSize;
	bool	m_refnum_horiz;
	int		m_block;		//ブロック番号
	int		m_dir;			//向き
	int		m_pinltrb;		//LTRBに10ピクセル長ピンが存在しているかどうかのフラグ
	SCompIndex* m_compInfoIndex;		//部品の情報へのポインタ
//	SCompIndex* m_embeddedCompInfoIndex;	//埋め込んだ部品の情報へのポインタ
	bool	m_useEmbeddedLib;
	wstring	m_orgname;		//冗長だけど、ライブラリから読み損ねた部品を残すために必要
	wstring	m_packagename;	//2009/01/06 パッケージ名
	wstring	m_mfrpnum;		//2009/01/07 製造者型式
	wstring	m_mfr;			//2009/01/07 製造者
	wstring	m_note;

protected:
	void	mirrorDir();		//m_dir変数を左右反転する
	void	rotateDir();		//m_dir変数を右回転する
	void	resetRefnumPos();	//参照番号の位置をデフォルトの位置に変更
	void	resetNamePos();		//部品名の位置をデフォルトの位置に変更
//	void	setPinLtrb();		//LTRBにピンが存在しているかどうかのフラグの設定



public:
	//bool keepOrder(){return true;}
	void	setPinLtrb();		//LTRBにピンが存在しているかどうかのフラグの設定
	
	SXBSchObj* duplicate();	//複製を行う
	//座標の設定  m_p2への設定を無効にしている
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}

	void move(int offsetx,int offsety);	//位置を動かす
	void mirrorH(int x); //X座標を指定して左右反転を行う
	virtual void mirrorH(); //左右反転を行う
	void rotate(const SPoint& p); //XY中心座標を指定して、回転を行う
	void rotate();	//回転を行う
	int qSelectWeight(const SPoint& p);				//点を指定して選択重要度を返す

	int isEditablePoint(const SPoint& pt,int& info);//指定点が操作可能なものの上にあるかどうか


	unsigned testSelection(const SRect& rc);		//範囲を指定して選択を行う
	unsigned testSelectionForDrag(const SRect& rc); //範囲を選択してドラッグ選択を行う
	bool qRedraw(const SRect&){return true;}		//描画が必要かどうかを返す qRedraw()は常に描画を強制する。
	
	SRect area();	//占有範囲を返す
	SRect bodyArea();	//占有範囲を返す

	SRect rotateRect();
	SSize size() const;
	SRect nameArea() const;
	SRect refnumArea() const;

	void setNameDrawSize(const SSize& size);	//描画サイズの設定
	void setRefnumDrawSize(const SSize& size);	//描画サイズの設定

	//部品番号、値の非表示
//#define XBSCHCOMPONET_NUMVAL_HIDE_VAL 1000000
	void SetHideVal(bool hide);
	void SetHideNum(bool hide);
	bool GetHideVal() const;
	bool GetHideNum() const;


public:
	void setName(const TCHAR* psz);
	const TCHAR* name() const {return m_name.c_str();}
	void setNameHorizontal(bool bHoriz);	//2005/04/09
	bool nameHorizontal() const {return m_name_horiz;}//2005/04/09

	void setNote(const TCHAR* psz);
	const TCHAR* note() const {return m_note.c_str();}
	void setNamePos(const SPoint& p){m_name_pos = p;}
	bool moveNamePos(int offsetx,int offsety);
	SPoint namePos(){return m_name_pos;}

	void setRefnum(const TCHAR* psz);
	const TCHAR* refnum() const {return m_refnum.c_str();}
	void setRefnumHorizontal(bool bHoriz);//2005/04/09
	bool refnumHorizontal() const {return m_refnum_horiz;}//2005/04/09
	void setRefnumPos(const SPoint& p){m_refnum_pos = p;}
	bool moveRefnumPos(int offsetx,int offsety);
	SPoint refnumPos(){return m_refnum_pos;}

	void setOrgName(const TCHAR* psz);
	const TCHAR* orgname() const {return m_orgname.c_str();}


	const SCompIndex* compInfoIndex() const {return m_compInfoIndex;}
	void setCompInfoIndex(const SCompIndex* pIndex);

	//>>>> Version 0.63
	void setPackageName(const TCHAR* psz);
	const TCHAR* packagename() const {return m_packagename.c_str();}

	void setMfr(const TCHAR* psz);
	const TCHAR* mfr() const {return m_mfr.c_str();}

	void setMFrPNum(const TCHAR* psz);
	const TCHAR* mfrpnum() const {return m_mfrpnum.c_str();}
	//<<<< Version 0.63

	
	const SPin*  pinLoc(int nIndex,int& nLtrb,SPoint& ptLoc) const; //ピンの根っこの位置情報を取得する
	const SPin*  pinEnd(int nIndex,int& nLtrb,SPoint& ptEnd) const; //ピンの先端の位置情報を取得する
	SPoint pinPosition(int)	const;//n番目のピンの先端の位置を得る。pinEnd()のユーティライズ

	SPin* pinInfo(int nIndex);

	int pinCount() const; //ピンの数を得る

	void setPinNC(int n,bool bSet);	//n番目のピンのNCフラグをbSetにする
	bool pinNC(int n) const ;		//n番目のピンのNCフラグを得る
	void presetPinNC();				//NCフラグをプリセットする


	void setBlock(int n);
	int block(){return m_block;}

	void setDir(int n);
	int dir(){return m_dir;}
	
	bool canAttribute(){return true;}	//アトリビュートの変更が可能な場合はtrueを返す。
};

#endif
