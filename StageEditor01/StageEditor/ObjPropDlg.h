#pragma once


// CObjPropDlg �_�C�A���O

class CObjPropDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CObjPropDlg)
	bool m_IsNew;
public:
	CObjPropDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CObjPropDlg();
	void SetNew(bool b) {
		m_IsNew = b;
	}
// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OBJ_PROP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
