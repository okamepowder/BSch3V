// MarkerLineAtrb.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "BSch.h"
#include "MarkerLineAtrb.h"
#include ".\markerlineatrb.h"
#include "xbschline.h"


// CMarkerLineAtrb �_�C�A���O

IMPLEMENT_DYNAMIC(CMarkerLineAtrb, CDialog)
CMarkerLineAtrb::CMarkerLineAtrb(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkerLineAtrb::IDD, pParent)
{
	m_nColor = 0xFF00FF;
	m_nWidth = 3;
}

CMarkerLineAtrb::~CMarkerLineAtrb()
{
}

void CMarkerLineAtrb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMarkerLineAtrb, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
//	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SETCOLOR, OnBnClickedButtonSetcolor)
END_MESSAGE_MAP()


// CMarkerLineAtrb ���b�Z�[�W �n���h��

void CMarkerLineAtrb::OnBnClickedOk()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_nWidth = GetDlgItemInt(IDC_EDIT_LINEWIDTH);
	if(m_nWidth<SXBSchMarkerLine::MIN_MARKER_WIDTH || m_nWidth>SXBSchMarkerLine::MAX_MARKER_WIDTH){
		AfxMessageBox(IDS_INVALID_LINEWIDTH,MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	OnOK();
}

//void CMarkerLineAtrb::OnBnClickedCancel()
//{
//	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
//	OnCancel();
//}

BOOL CMarkerLineAtrb::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������

	CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LINEWIDTH);
	pSpin->SetRange(SXBSchMarkerLine::MIN_MARKER_WIDTH,SXBSchMarkerLine::MAX_MARKER_WIDTH);
	if(m_nWidth<SXBSchMarkerLine::MIN_MARKER_WIDTH ){
		m_nWidth=SXBSchMarkerLine::MIN_MARKER_WIDTH;
	}else if(m_nWidth>SXBSchMarkerLine::MAX_MARKER_WIDTH ){
		m_nWidth=SXBSchMarkerLine::MAX_MARKER_WIDTH;
	}
	pSpin->SetPos(m_nWidth);

	COLORREF colRef = (COLORREF)m_nColor;
	m_brush.CreateSolidBrush(colRef);

	CString str;
	str.Format(_T("%d .. %d"),SXBSchMarkerLine::MIN_MARKER_WIDTH,SXBSchMarkerLine::MAX_MARKER_WIDTH);
	SetDlgItemText(IDC_STATIC_RANGE_WIDTH,str);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

HBRUSH CMarkerLineAtrb::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO :  ������ DC �̑�����ύX���Ă��������B
	// TODO :  ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B

	int id=pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC && id==IDC_STATIC_COLOR){
		return (HBRUSH) m_brush;
	}
	return hbr;
}


#define BRIGHTNESS_LIMIT 700

void CMarkerLineAtrb::OnBnClickedButtonSetcolor()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CColorDialog dlg;
	COLORREF arrayTempColor[16];
	for(int i=0;i<16;i++){
		arrayTempColor[i] = m_arrayColor[i];
	}


	dlg.m_cc.Flags |= CC_RGBINIT;// | CC_PREVENTFULLOPEN;
	dlg.m_cc.rgbResult = (COLORREF)m_nColor;
	dlg.m_cc.lpCustColors = arrayTempColor;
	if(dlg.DoModal()==IDOK){
		COLORREF col = dlg.m_cc.rgbResult;
		int r = GetRValue(col);
		int g = GetGValue(col);
		int b = GetBValue(col);

		int br = r+g+b;
		if(br>BRIGHTNESS_LIMIT){
			r=r*BRIGHTNESS_LIMIT/br;
			g=g*BRIGHTNESS_LIMIT/br;
			b=b*BRIGHTNESS_LIMIT/br;
		}

		for(int i=0;i<16;i++){
			m_arrayColor[i] = arrayTempColor[i];
		}

		//if(r<b && r<g){
		//	if(r>BRIGHTNESS_LIMIT) r=BRIGHTNESS_LIMIT;
		//}else if(g<b && g<r){
		//	if(g>BRIGHTNESS_LIMIT) g=BRIGHTNESS_LIMIT;
		//}else{
		//	if(b>BRIGHTNESS_LIMIT) b=BRIGHTNESS_LIMIT;
		//}
		col = RGB(r,g,b);
		m_nColor = col;

		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(col);
		CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC_COLOR);
		pStatic->Invalidate();
	}
}
