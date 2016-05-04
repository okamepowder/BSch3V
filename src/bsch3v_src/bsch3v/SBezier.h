#pragma once

#include "coord.h"

class SBezier
{
public:
	SBezier(void);
	~SBezier(void);

	static const int m_nBezierNodes = 32;

	static SPoint intermediatePoint(SPoint p0,SPoint p1,int n);

	//2�[�_��2����_�ō\�������x�W�F�Ȑ��̍��W�����߂�B
	//�������� m_nBezierNodes
	//arrayNodes�́A(m_nBezierNodes +1)�̗̈悪�K�v
	static void CreateNodes(SPoint& ep1,SPoint& ep2,SPoint& ct1,SPoint& ct2,SPoint* arrayNodes);
};
