#pragma once

class LCoVFunc
{
public:
	LCoVFunc(void){}
	~LCoVFunc(void){}
	static void DrawResourceBMP(CDC* pDC,int x,int y,int cx,int cy,UINT id,int sx,int sy);
	static void DrawPinType(CDC* pDC,int x,int y,int pintype);

};
