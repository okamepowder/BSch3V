/****************************************************************************
    BSch3V and Utility common file.
    Copyright (C) 2002-2005 H.Okada

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*****************************************************************************/


#ifndef SNLPIN_H
#define SNLPIN_H

class SNlPin  
{
public:
	SNlPin(){}
	virtual ~SNlPin(){}
	string		m_strRefNum;	//���i�ԍ�
	string		m_strPinNum;	//�s�����܂��̓s���ԍ�
	SPoint		m_point;		//�s���ʒu
	stringList  m_listSigName;	//�M�����̃��X�g
};

#endif
