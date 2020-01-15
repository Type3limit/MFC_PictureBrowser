
// PicBrowserDlg.h : ͷ�ļ�
//

#pragma once
#include "CMyPictureCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"


// CPicBrowserDlg �Ի���
class CPicBrowserDlg : public CDialog
{
// ����
public:
	CPicBrowserDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICBROWSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	
	WINDOWPLACEMENT m_stWpOld;

	//��������
	CButton m_ScreenButton;
	CButton m_FileButton;
	CWnd * m_ZoomText;
	CSliderCtrl m_Slider;
	// PictureControl���Ʊ���
	CMyPictureCtrl m_cPictureCtrl;

	BOOL m_bIsFullScreen;
	
	CRect m_FileButtonRect;//�ļ���ť��С
	CRect m_ScreenButtonRect;//ȫ����ť��С
	CRect m_SliderRect;//�����С 
	
	UINT m_ZoomScale;


public:

	BOOL CPicBrowserDlg::PreTranslateMessage(MSG *pMsg);
	//void 
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnZoomChanged(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
public:

	
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonScreen();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


};
