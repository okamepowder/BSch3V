/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** �}�[�N�A�b�v������N���X
****************************************************************************/
#ifndef SMARKUPTEXTLINE_H
#define SMARKUPTEXTLINE_H

#include <list>
#include <stack>
using namespace std;

class SMarkupTextLine
{
public:

	enum TEXT_STYLE
	{ 
		NO_STYLE	= 0,
		FONT_NAME	= 0x0001,
		OVERBAR		= 0x0002,
		UNDERLINE	= 0x0004,
		BOLD		= 0x0008,
		ITALIC		= 0x0010,
		SUB			= 0x0020,
		SUP			= 0x0040,
		CR			= 0x8000	//�����ŉ��s�B
	};

	enum CHAR_PROPERTY
	{
		NO_CHAR_PROPERTY = 0,
		BREAKABLE   	 = 1,		//���̕����̑O�ŉ��s�\
		BREAKLINE		 = 2    	//���̕����őO�ŉ��s
	};

	string	m_strText;
	string	m_fontName;
	unsigned int m_textSyle;
	unsigned int *m_charFlags;
public:
	SMarkupTextLine(void)
	{
		m_strText = "";
		m_fontName = "";
		m_textSyle = (unsigned int)SMarkupTextLine::NO_STYLE;
		m_charFlags = NULL;

	}

	~SMarkupTextLine(void)
	{
		delete[]m_charFlags;
	}
};

typedef list<SMarkupTextLine*> MarkupTextLineList;
typedef list<SMarkupTextLine*>::iterator MarkupTextLineIterator;

void g_CreateMarkupTextLines(const char* src,MarkupTextLineList* pMarkupTextLineList);



#endif