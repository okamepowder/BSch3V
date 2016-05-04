#pragma once
/****************************************************************************
     BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/



#include "BSch.h"

class SBSchDrawColor{
public:

	COLORREF colBG;
	COLORREF colJUNC;
	COLORREF colWIRE;
	COLORREF colBUS;
	COLORREF colDASH;
	COLORREF colTAG;
	COLORREF colTAG_NAME;
	COLORREF colCOMMENT;
	COLORREF colLABEL;
	COLORREF colXOR;
	COLORREF colPART;
	COLORREF colPART_PIN;
	COLORREF colPART_PINNAME;
	COLORREF colPART_PINNUM;
	COLORREF colPART_NAME;
	COLORREF colPART_NUM;
	COLORREF colPART_FRAME;
	COLORREF colSHEET_FRAME;
	COLORREF colSHEET_GRID;
	COLORREF colDARK;
	COLORREF colZLPIN;
	COLORREF colNC_MARK;
	COLORREF colQUICKPROPVIEW;
	COLORREF colTEMPFRAME_FOCUSED;
	COLORREF colTEMPFRAME_NOT_FOCUSED;
	COLORREF colFIND_MARK;

	SBSchDrawColor(){
		colBG			=	COL_BG;
		colJUNC			=	COL_JUNC;
		colWIRE			=	COL_WIRE;
		colBUS			=	COL_BUS;
		colDASH			=	COL_DASH;
		colTAG			=	COL_TAG;
		colTAG_NAME		=	COL_TAG_NAME;
		colCOMMENT		=	COL_COMMENT;
		colLABEL			=	COL_LABEL;
		colXOR			=	COL_XOR;
		colPART			=	COL_PART;
		colPART_PIN		=	COL_PART_PIN;
		colPART_PINNAME	=	COL_PART_PINNAME;	
		colPART_PINNUM	=	COL_PART_PINNUM;
		colPART_NAME		=	COL_PART_NAME;	
		colPART_NUM		=	COL_PART_NUM;
		colPART_FRAME	=	COL_PART_FRAME;
		colSHEET_FRAME	=	COL_SHEET_FRAME;
		colSHEET_GRID	=	COL_SHEET_GRID;
		colDARK			=	COL_DARK;
		colZLPIN			=	COL_ZLPIN;	//ZeroLengthPin
		colNC_MARK		=	COL_NC_MARK;
		colQUICKPROPVIEW =	COL_QUICKPROPVIEW;
		colTEMPFRAME_FOCUSED		= COL_TEMPFRAME_FOCUSED;
		colTEMPFRAME_NOT_FOCUSED	= COL_TEMPFRAME_NOT_FOCUSED;
		colFIND_MARK	= COL_FIND_MARK;

	}

};