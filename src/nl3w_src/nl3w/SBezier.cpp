#include "stdafx.h"
#include "SBezier.h"

SBezier::SBezier(void)
{
}

SBezier::~SBezier(void)
{
}

SPoint SBezier::intermediatePoint(SPoint p0,SPoint p1,int n)
{
	return SPoint(
		(p1.x()-p0.x())*n/m_nBezierNodes+p0.x(),
		(p1.y()-p0.y())*n/m_nBezierNodes+p0.y());
}

#define NODECALC_MULT	32

void SBezier::CreateNodes(SPoint& ep1,SPoint& ep2,SPoint& ct1,SPoint& ct2,SPoint* arrayNodes)
{
	arrayNodes[0] = ep1;
	arrayNodes[m_nBezierNodes] = ep2;

	SPoint p0 = SPoint(ep1.x()*NODECALC_MULT, ep1.y()*NODECALC_MULT);
	SPoint p1 = SPoint(ep2.x()*NODECALC_MULT, ep2.y()*NODECALC_MULT);
	SPoint c0 = SPoint(ct1.x()*NODECALC_MULT, ct1.y()*NODECALC_MULT);
	SPoint c1 = SPoint(ct2.x()*NODECALC_MULT, ct2.y()*NODECALC_MULT);

	for(int i=1;i<m_nBezierNodes;i++){
		SPoint q0 =intermediatePoint(p0,c0,i);
		SPoint q1 =intermediatePoint(c0,c1,i);
		SPoint q2 =intermediatePoint(c1,p1,i);
		SPoint q3 =intermediatePoint(q0,q1,i);
		SPoint q4 =intermediatePoint(q1,q2,i);
		SPoint q5 =intermediatePoint(q3,q4,i);
		arrayNodes[i]=SPoint(q5.x()/NODECALC_MULT,q5.y()/NODECALC_MULT);
	}
}