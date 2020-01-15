
// PicBrowserDlg.h : 头文件
//

#pragma once
#include "CMyPictureCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"


// CPicBrowserDlg 对话框
class CPicBrowserDlg : public CDialog
{
// 构造
public:
	CPicBrowserDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICBROWSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	
	WINDOWPLACEMENT m_stWpOld;

	//部件控制
	CButton m_ScreenButton;
	CButton m_FileButton;
	CWnd * m_ZoomText;
	CSliderCtrl m_Slider;
	// PictureControl控制变量
	CMyPictureCtrl m_cPictureCtrl;

	BOOL m_bIsFullScreen;
	
	CRect m_FileButtonRect;//文件按钮大小
	CRect m_ScreenButtonRect;//全屏按钮大小
	CRect m_SliderRect;//滑块大小 
	
	UINT m_ZoomScale;


public:

	BOOL CPicBrowserDlg::PreTranslateMessage(MSG *pMsg);
	//void 
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
