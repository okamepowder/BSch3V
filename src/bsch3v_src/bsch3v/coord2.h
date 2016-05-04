/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef COORD2_H
#define COORD2_H

CSize SSize2CSize(const SSize& ssize);
SSize CSize2SSize(const CSize& csize);

CPoint SPoint2CPoint(const SPoint& spoint);
SPoint CPoint2SPoint(const CPoint& cpoint);

CRect SRect2CRect(const SRect& srect);
SRect CRect2SRect(const CRect& crect);

#endif