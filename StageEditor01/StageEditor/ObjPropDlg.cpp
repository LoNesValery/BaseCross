// ObjPropDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StageEditor.h"
#include "ObjPropDlg.h"
#include "afxdialogex.h"


// CObjPropDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CObjPropDlg, CDialogEx)

CObjPropDlg::CObjPropDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_OBJ_PROP, pParent),
	m_IsNew(false)
{

}

CObjPropDlg::~CObjPropDlg()
{
}

void CObjPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjPropDlg, CDialogEx)
END_MESSAGE_MAP()


// CObjPropDlg ���b�Z�[�W �n���h���[


BOOL CObjPropDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	if (m_IsNew) {
		SetWindowText(L"�V�K");
	}
	else {
		SetWindowText(L"�v���p�e�B");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
