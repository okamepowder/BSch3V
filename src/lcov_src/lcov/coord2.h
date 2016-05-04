/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#ifndef COORD2_H
#define COORD2_H

CSize SSize2CSize(const SSize& ssize);
SSize CSize2SSize(const CSize& csize);

CPoint SPoint2CPoint(const SPoint& spoint);
SPoint CPoint2SPoint(const CPoint& cpoint);

CRect SRect2CRect(const SRect& srect);
SRect CRect2SRect(const CRect& crect);

//ê¸ï™ë§ñ Ç…ì_Ç™éwíËãóó£ì‡Ç…Ç†ÇÈÇ©Ç«Ç§Ç©ÇÃîªíË
bool PointIsCloseToLineSide(const SPoint& ptPoint,const SPoint& ptLineBegin,const SPoint& ptLineEnd,int limit, SPoint& ptCross);

#endif