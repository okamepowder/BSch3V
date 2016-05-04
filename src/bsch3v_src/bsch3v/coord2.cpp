/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "coord.h"
#include "coord2.h"

CSize SSize2CSize(const SSize& ssize)
{
	return CSize(ssize.w(),ssize.h());
}

SSize CSize2SSize(const CSize& csize)
{
	return SSize(csize.cx,csize.cy);
}

CPoint SPoint2CPoint(const SPoint& spoint)
{
	return CPoint(spoint.x(),spoint.y());
}

SPoint CPoint2SPoint(const CPoint& cpoint)
{
	return SPoint(cpoint.x,cpoint.y);
}

CRect SRect2CRect(const SRect& srect)
{
	return CRect(srect.l(),srect.t(),srect.r(),srect.b());
}

SRect CRect2SRect(const CRect& crect)
{
	return SRect(crect.left,crect.top,crect.Width(),crect.Height());
}
