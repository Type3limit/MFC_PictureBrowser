
// PicBrowserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PicBrowser.h"
#include "PicBrowserDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPicBrowserDlg 对话框



CPicBrowserDlg::CPicBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PICBROWSER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPicBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SCREEN, m_ScreenButton);
	DDX_Control(pDX, IDC_BUTTON_FILE, m_FileButton);
	DDX_Control(pDX, IDC_SLIDER, m_Slider);
	DDX_Control(pDX, IDC_STATIC_PIC, m_cPictureCtrl);
}

BEGIN_MESSAGE_MAP(CPicBrowserDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CPicBrowserDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_SCREEN, &CPicBrowserDlg::OnBnClickedButtonScreen)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_HSCROLL()
	ON_MESSAGE(WMU_ZOOM,&CPicBrowserDlg::OnZoomChanged)
END_MESSAGE_MAP()


// CPicBrowserDlg 消息处理程序

BOOL CPicBrowserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ZoomText = (CStatic*)GetDlgItem(IDC_STATIC_TEXT);
	m_stWpOld = { 0 };
	//初始化各个矩形
	m_FileButton.GetWindowRect(&m_FileButtonRect);
	ScreenToClient(&m_FileButtonRect);
	m_ScreenButton.GetWindowRect(&m_ScreenButtonRect);
	ScreenToClient(&m_ScreenButtonRect);
	m_Slider.GetWindowRect(&m_SliderRect);
	ScreenToClient(&m_SliderRect);

	m_bIsFullScreen = FALSE;
	//初始化放缩因子
	m_ZoomScale = 0;
	m_ZoomText->ShowWindow(SW_HIDE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPicBrowserDlg::OnCancel()
{
	if (IDOK == MessageBoxW(L"Sure To Quit?", L"Tips", MB_OKCANCEL))
	{
		DestroyWindow();
	}
}

void CPicBrowserDlg::OnOK()
{
	return;
}

void CPicBrowserDlg::OnClose()

{
	EndDialog(IDCANCEL);
}

void CPicBrowserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPicBrowserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPicBrowserDlg::OnBnClickedButtonFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter(L"图像文件|*.jpg;*.png;*.bmp;*.jpeg|JPG文件|*.jpg|PNG文件|*.png|BMP文件|*.bmp|JPEG文件|*.jpeg|ICO文件|*.ico||所有文件|*.*");
	CFileDialog FileOpen(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);

	if (IDOK != FileOpen.DoModal())
	{
		return;
	}

	CString FilePath = FileOpen.GetPathName();

	m_cPictureCtrl.SetImageFile(FilePath);

	if (m_ZoomScale != 0)
	{
		m_ZoomScale = 0;
		m_Slider.SetPos(m_ZoomScale);
	}
	m_FileButton.ShowWindow(SW_SHOW);
	m_ScreenButton.ShowWindow(SW_SHOW);
	m_Slider.ShowWindow(SW_SHOW);
	m_ZoomText->ShowWindow(SW_SHOW);
}

void CPicBrowserDlg::OnBnClickedButtonScreen()
{
	m_bIsFullScreen = TRUE;
	m_cPictureCtrl.FullScreenModel(m_bIsFullScreen);

	int cxScreen, cyScreen;
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	WINDOWPLACEMENT wpNew;
	CRect WindowRect;
	::GetWindowRect(::GetDesktopWindow(), &WindowRect);
	GetWindowPlacement(&m_stWpOld);
	CRect ClientRect;
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);
	ClientToScreen(&ClientRect);
	wpNew.length = sizeof(WINDOWPLACEMENT);
	wpNew.showCmd = SW_MAXIMIZE;
	wpNew.rcNormalPosition = WindowRect;
	SetWindowPos(&wndTopMost, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);

	LONG IStyle = ::GetWindowLong(this->m_hWnd, GWL_STYLE);

	::SetWindowLong(this->m_hWnd, GWL_STYLE, IStyle & ~WS_CAPTION);
	::SetWindowPos(this->m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER| SWP_NOACTIVATE | SWP_FRAMECHANGED);
	SetWindowPlacement(&wpNew);

	m_cPictureCtrl.MoveWindow(0, 0, WindowRect.Width(), WindowRect.Height());

	m_FileButton.ShowWindow(SW_HIDE);
	m_ScreenButton.ShowWindow(SW_HIDE);
	m_Slider.ShowWindow(SW_HIDE);
	m_ZoomText->ShowWindow(SW_HIDE);
}


void CPicBrowserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
}


BOOL CPicBrowserDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && m_bIsFullScreen)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			m_bIsFullScreen = FALSE;
			m_cPictureCtrl.FullScreenModel(m_bIsFullScreen);
			m_bIsFullScreen = FALSE;
			
			LONG IStyle = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
			::SetWindowLong(this->m_hWnd, GWL_STYLE, IStyle | WS_CAPTION);
			::SetWindowPos(this->m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER
				| SWP_NOACTIVATE | SWP_FRAMECHANGED);
			SetWindowPlacement(&m_stWpOld);

			m_FileButton.ShowWindow(SW_SHOW);
			m_ScreenButton.ShowWindow(SW_SHOW);
			m_Slider.ShowWindow(SW_SHOW);
			m_ZoomText->ShowWindow(SW_SHOW);
			return TRUE;
		}
		else
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPicBrowserDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
	//return TRUE;
}

void CPicBrowserDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CPicBrowserDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (pScrollBar == (CScrollBar*)&m_Slider)
	{
		m_ZoomScale = m_Slider.GetPos();

		UpdateData(FALSE);
		m_cPictureCtrl.SetZoom(m_ZoomScale);
	}
	else
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CPicBrowserDlg::OnZoomChanged(WPARAM wparam,LPARAM lparam)
{
		m_ZoomScale = wparam;
		m_Slider.SetPos(m_ZoomScale);
		CString text;
		text.Format(_T("%d %%"), m_ZoomScale);
		m_ZoomText->SetWindowTextW(text);
		return 0UL;
}
