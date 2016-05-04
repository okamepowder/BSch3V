#pragma once

#include "coord.h"

class SBezier
{
public:
	SBezier(void);
	~SBezier(void);

	static const int m_nBezierNodes = 32;

	static SPoint intermediatePoint(SPoint p0,SPoint p1,int n);

	//2端点と2制御点で構成されるベジェ曲線の座標を求める。
	//分割数は m_nBezierNodes
	//arrayNodesは、(m_nBezierNodes +1)個の領域が必要
	static void CreateNodes(SPoint& ep1,SPoint& ep2,SPoint& ct1,SPoint& ct2,SPoint* arrayNodes);
};
