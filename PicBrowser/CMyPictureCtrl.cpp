#include "stdafx.h"
#include "CMyPictureCtrl.h"


CMyPictureCtrl::CMyPictureCtrl():CStatic()
{
	int CurX = GetSystemMetrics(SM_CXFULLSCREEN);
	int CurY = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect cur_rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &cur_rect, 0);

	m_FullScreenRect = CRect(0, 0, CurX, CurY);

	//初始化各个BOOL
	m_bIsFullScreen = FALSE;
	m_bIsDragging = FALSE;
	m_bIsZoomed = FALSE;
	m_bIsMoved = FALSE;
	m_bIsReachTop = m_bIsReachLeft = m_bIsReachRight = m_bIsReachBottom = FALSE;
	m_bIsTrack = m_bHasTimer = FALSE;
	m_nAnimationDiffHeight = m_nAnimationDiffWidth = 0;
	
	//初始化各个Point
	m_PointDiff = 0;
	m_OriginPointLT = m_DrawRect.TopLeft();
	m_OriginPointRB = m_DrawRect.BottomRight();

	m_uiZoomScale = 0;
}


BEGIN_MESSAGE_MAP(CMyPictureCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

LRESULT CMyPictureCtrl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	GetClientRect(&m_ClientRect);
	this->GetClientRect(&m_PicCtrlRect);
	m_DrawRect = m_PicCtrlRect;
	m_OriginRect = m_DrawRect;
	m_ImgRect = m_PicCtrlRect;

	m_FunctionRect = CRect(m_ClientRect.left,
		m_ClientRect.bottom - m_PicCtrlRect.bottom,
		m_ClientRect.right,
		m_ClientRect.bottom);
	ScreenToClient(&m_FunctionRect);
	return 0UL;
}

void CMyPictureCtrl::OnPaint()
{
	if (!m_cImage.IsNull())
	{
		CPaintDC pDC(this);
		CDC memDC;
		CBitmap membitmap;
		memDC.CreateCompatibleDC(&pDC);
		memDC.SetStretchBltMode(HALFTONE);
		membitmap.CreateCompatibleBitmap(&pDC, m_PicCtrlRect.Width(), m_PicCtrlRect.Height());
		memDC.SelectObject(&membitmap);
		memDC.FillSolidRect(&m_PicCtrlRect, pDC.GetBkColor());
		m_cImage.Draw(memDC, m_DrawRect, m_ImgRect);

		pDC.BitBlt(0, 0, m_PicCtrlRect.Width(), m_PicCtrlRect.Height(), &memDC, 0, 0, SRCCOPY);

		ReleaseDC(&pDC);
		memDC.DeleteDC();
		membitmap.DeleteObject();
	}
}

void CMyPictureCtrl::UpdateDrawRect()
{
	GetWindowRect(&m_PicCtrlRect);
	ScreenToClient(&m_PicCtrlRect);
	int nImgWidth = m_ImgRect.Width();
	int nImgHeight = m_ImgRect.Height();

	int nActualWidth = m_PicCtrlRect.Width();
	int nActualHeight = m_PicCtrlRect.Height();

	float fscaleimgHW = static_cast<float>(nImgHeight) / static_cast<float>(nImgWidth);
	float fscaleimgWH = static_cast<float>(nImgWidth) / static_cast<float>(nImgHeight);

	int cur_W = 0, cur_H = 0;
	float fscale = 1.0;
	if (nImgWidth > nActualWidth&& nImgHeight <= nActualHeight)//图像宽度大于客户区且图像高小于等于客户区
	{
		cur_W = nActualWidth;
		cur_H = static_cast<int>(cur_W * fscaleimgHW);
		m_DrawRect = CRect((nActualWidth - cur_W) / 2,
			(nActualHeight - cur_H) / 2,
			(nActualWidth + cur_W) / 2,
			(nActualHeight + cur_H) / 2);
	}
	else if (nImgWidth <= nActualWidth && nImgHeight > nActualHeight)//图像宽度小于等于客户区且图像高于客户区
	{
		cur_H = nActualHeight;
		cur_W = static_cast<int>(cur_H * fscaleimgWH);
		m_DrawRect = CRect((nActualWidth - cur_W) / 2,
			(nActualHeight - cur_H) / 2,
			(nActualWidth + cur_W) / 2,
			(nActualHeight + cur_H) / 2);
	}
	else if (nImgWidth <= nActualWidth && nImgHeight < nActualHeight)//图像宽度小于等于客户区且图像高小于客户区
	{
		m_DrawRect = CRect((nActualWidth - nImgWidth) / 2,
			(nActualHeight - nImgHeight) / 2,
			(nActualWidth + nImgWidth) / 2,
			(nActualHeight + nImgHeight) / 2);
	}
	else//图像宽大于客户区且图像高大于客户区
	{

		float fscaleCWH = static_cast<float>(m_PicCtrlRect.Width()) / static_cast<float>(m_PicCtrlRect.Height());
		float fscaleIWH = static_cast<float>(m_ImgRect.Width()) / static_cast<float>(m_ImgRect.Height());
		bool Choose = (fscaleCWH - fscaleIWH) > 0 ? false : true;
		if (Choose)
		{

			fscaleimgHW - fscaleimgWH > 0 ? cur_H = nActualHeight : cur_W = nActualWidth;
			fscaleimgHW - fscaleimgWH > 0 ? cur_W = static_cast<int>(cur_H * fscaleimgWH) : cur_H = static_cast<int>(cur_W * fscaleimgHW);
		}
		else
		{
			fscaleimgHW - fscaleimgWH < 0 ? cur_W = nActualWidth : cur_H = nActualHeight;
			fscaleimgHW - fscaleimgWH < 0 ? cur_H = static_cast<int>(cur_W * fscaleimgHW) : cur_W = static_cast<int>(cur_H * fscaleimgWH);
		}
		m_DrawRect = CRect((nActualWidth - cur_W) / 2,
			(nActualHeight - cur_H) / 2,
			(nActualWidth + cur_W) / 2,
			(nActualHeight + cur_H) / 2);
	}
	m_OriginRect = m_DrawRect;

	if (m_bIsZoomed)
	{
		ZoomPic();
	}
	else
	{
		Invalidate();
		UpdateWindow();
	}
}

void CMyPictureCtrl::SetImageFile(LPCTSTR lpImageFilePath)
{
	m_uiZoomScale = 0;
	m_PointDiff = 0;
	::SendMessage(::AfxGetMainWnd()->m_hWnd, WMU_ZOOM, m_uiZoomScale, 0);
	if (!m_cImage.IsNull())
		m_cImage.Destroy();
	HRESULT result = m_cImage.Load(lpImageFilePath);
	if (FAILED(result))
	{
		MessageBox(L"打开图像失败");
		return;
	}
	int imgWidth = m_cImage.GetWidth();
	int imgHeight = m_cImage.GetHeight();
	m_ImgRect = CRect(0, 0, imgWidth, imgHeight);
	UpdateDrawRect();
	m_OriginPointLT = m_DrawRect.TopLeft();
	m_OriginPointRB = m_DrawRect.BottomRight();
}

void CMyPictureCtrl::FullScreenModel(BOOL bCurModel)
{
	if (bCurModel)
	{
		m_bIsFullScreen = TRUE;
		CRect WindowRect;
		::GetWindowRect(::GetDesktopWindow(), &WindowRect);
		m_PicCtrlRect = m_ClientRect = WindowRect;
		Invalidate();
		UpdateWindow();
		
	}
	else
	{
		m_bIsFullScreen = FALSE;
		UpdateDrawRect();
	}
	//UpdateDrawRect();
}

void CMyPictureCtrl::OnSize(UINT nType, int cx, int cy)
{
	if (!m_cImage.IsNull())
	{
		if (m_bIsFullScreen)
			m_PicCtrlRect = m_FullScreenRect;
		else
			GetWindowRect(&m_PicCtrlRect);
		ScreenToClient(&m_PicCtrlRect);
		m_FunctionRect = CRect(m_ClientRect.left,
			m_PicCtrlRect.Height(),
			m_ClientRect.right,
			m_ClientRect.bottom);
		UpdateDrawRect();
	}
	return CStatic::OnSize(nType, cx, cy);
}

void CMyPictureCtrl::SetZoom(UINT uiScale)
{
	if (uiScale >= 0)
	{
		m_uiZoomScale = uiScale;
		m_bIsZoomed = TRUE;
		ZoomPic();
	}
}

void CMyPictureCtrl::ZoomPic()
{
	if (m_uiZoomScale == 0)
	{
		m_bIsZoomed = FALSE;
		m_PointDiff = 0;
		UpdateDrawRect();
	}
	else
	{
		CRect Rect;

		if (m_uiZoomScale > 5)
		{
			Rect = CRect(
				m_OriginRect.TopLeft() + m_PointDiff,
				m_OriginRect.BottomRight() + m_PointDiff);
		}
		else
		{
			m_PointDiff = 0;
			Rect = m_OriginRect;
		}



		float fCurScale = static_cast<float>(1.0f + m_uiZoomScale * 0.01f);
		int nDrawWidth = static_cast<int>(Rect.Width() * fCurScale);
		int nDrawHeight = static_cast<int>(Rect.Height() * fCurScale);
		int nDiffX = (nDrawWidth - Rect.Width()) / 2;
		int nDiffY = (nDrawHeight - Rect.Height()) / 2;


		int nCurLeft = Rect.left - nDiffX;
		int nCurTop = Rect.top - nDiffY;
		int nCurRight = Rect.right + nDiffX;
		int nCurBottom = Rect.bottom + nDiffY;

		int nCurDiffL, nCurDiffR, nCurDiffT, nCurDiffB;
		int nDelta = 0;

		if (m_PointDiff.x != 0 || m_PointDiff.y != 0)
			m_bIsMoved = TRUE;
		else
			m_bIsMoved = FALSE;

		if (m_bIsMoved)
		{
			//横向判断
			if (nCurLeft > m_PicCtrlRect.left)//左侧超出时：
			{
				nCurDiffL = nCurLeft - m_PicCtrlRect.left;
				if (nCurRight < m_PicCtrlRect.right)//右侧超出
				{
					nDelta = m_PicCtrlRect.Width() - (nCurRight - nCurLeft);
					nCurLeft = m_PicCtrlRect.left + nDelta / 2;
					nCurRight = m_PicCtrlRect.right - nDelta / 2;
				}
				else//右侧未超出
				{
					nCurDiffR = nCurRight - m_PicCtrlRect.right;
					if (nCurDiffR > nCurDiffL)//右侧图像空余足够填补左侧空余
					{
						nDelta = nCurLeft - m_PicCtrlRect.left;
						nCurLeft = m_PicCtrlRect.left;
						nCurRight -= nDelta;
					}
					else//不足以填补空余
					{
						nDelta = (nCurDiffL - nCurDiffR) / 2;
						nCurLeft = m_PicCtrlRect.left + nDelta;
						nCurRight = m_PicCtrlRect.right - nDelta;
					}

				}
				goto heightjudge;
			}
			else if (nCurRight < m_PicCtrlRect.right)//右侧超出时：
			{
				nCurDiffR = m_PicCtrlRect.right - nCurRight;
				if (nCurLeft > m_PicCtrlRect.left)//左侧超出
				{
					nDelta = (m_PicCtrlRect.Width() - (nCurRight - nCurLeft)) / 2;
					nCurLeft = m_PicCtrlRect.left + nDelta;
					nCurRight = m_PicCtrlRect.right - nDelta;
				}
				else//左侧未超出
				{
					nCurDiffL = m_PicCtrlRect.left - nCurLeft;
					if (nCurDiffL > nCurDiffR)//左侧图像空余足够填补右侧
					{
						nDelta = m_PicCtrlRect.right - nCurRight;
						nCurRight = m_PicCtrlRect.right;
						nCurLeft += nDelta;
					}
					else//不足以填补
					{
						nDelta = (nCurDiffR - nCurDiffL) / 2;
						nCurLeft = m_PicCtrlRect.left + nDelta;
						nCurRight = m_PicCtrlRect.right - nDelta;
					}
				}
			}
		heightjudge:
			if (nCurTop > m_PicCtrlRect.top)//顶部超出时：
			{
				nCurDiffT = nCurTop - m_PicCtrlRect.top;
				if (nCurBottom < m_PicCtrlRect.bottom)//底部超出
				{
					nDelta = (m_PicCtrlRect.Height() - (nCurBottom - nCurTop)) / 2;
					nCurTop = m_PicCtrlRect.top + nDelta;
					nCurBottom = m_PicCtrlRect.bottom - nDelta;
					goto done;
				}

				else//底部未超出
				{
					nCurDiffB = nCurBottom - m_PicCtrlRect.bottom;
					if (nCurDiffB > nCurDiffT)//底部空余足以填补顶部
					{
						nDelta = nCurTop - m_PicCtrlRect.top;
						nCurTop = m_PicCtrlRect.top;
						nCurBottom -= nDelta;
					}
					else//不足以
					{
						nDelta = (nCurDiffT - nCurDiffB) / 2;
						nCurTop = m_PicCtrlRect.top + nDelta;
						nCurBottom = m_PicCtrlRect.bottom - nDelta;
					}
				}
			}
			else if (nCurBottom < m_PicCtrlRect.bottom)//底部超出时：
			{
				nCurDiffB = m_PicCtrlRect.bottom - nCurBottom;
				if (nCurTop > m_PicCtrlRect.top)//顶部超出
				{
					nDelta = (m_PicCtrlRect.Height() - (nCurBottom - nCurTop)) / 2;
					nCurTop = m_PicCtrlRect.top + nDelta;
					nCurBottom = m_PicCtrlRect.bottom - nDelta;
					goto done;
				}

				else//顶部未超出
				{
					nCurDiffT = m_PicCtrlRect.top - nCurTop;
					if (nCurDiffT > nCurDiffB)//顶部空间足以填补
					{
						nDelta = m_PicCtrlRect.bottom - nCurBottom;
						nCurBottom = m_PicCtrlRect.bottom;
						nCurTop += nDelta;
					}
					else//不足以填补
					{
						nDelta = (nCurDiffB - nCurDiffT) / 2;
						nCurTop = m_PicCtrlRect.top + nDelta;
						nCurBottom = m_PicCtrlRect.bottom - nDelta;
					}
				}
			}

		}
	done:
		m_DrawRect = CRect(nCurLeft, nCurTop, nCurRight, nCurBottom);
		m_OriginPointLT = m_DrawRect.TopLeft();
		m_OriginPointRB = m_DrawRect.BottomRight();
		Invalidate();
		UpdateWindow();
	}
	::SendMessage(::AfxGetMainWnd()->m_hWnd,WMU_ZOOM, m_uiZoomScale,0);
}

void CMyPictureCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (PtInRect(m_PicCtrlRect, point))
	{
		SetCapture();
	}

	if (m_bIsZoomed)
	{
		m_bIsDragging = TRUE;
	}
	else
	{
		m_bIsDragging = FALSE;
	}
	m_OriginPoint = point;
	return CStatic::OnLButtonDown(nFlags, point);
}

void CMyPictureCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bIsDragging = FALSE;
	m_bIsTrack = FALSE;
	if (m_bIsZoomed)
	{
		m_bIsReachLeft = (m_DrawRect.left >= m_PicCtrlRect.left ? TRUE : FALSE);
		m_bIsReachRight = (m_DrawRect.right <= m_PicCtrlRect.right ? TRUE : FALSE);
		m_bIsReachTop = (m_DrawRect.top >= m_PicCtrlRect.top ? TRUE : FALSE);
		m_bIsReachBottom = (m_DrawRect.bottom <= m_PicCtrlRect.bottom ? TRUE : FALSE);
		if (m_bIsReachBottom || m_bIsReachLeft || m_bIsReachRight || m_bIsReachTop)
		{
			if (m_bIsReachBottom && m_bIsReachTop);
			else if (m_bIsReachLeft && m_bIsReachRight);
			else
				ReturnAnimation();
		}
	}
	if (GetCapture() == this)
	{
		ReleaseCapture();
	}
	return CStatic::OnLButtonUp(nFlags, point);
}

void CMyPictureCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	m_bIsReachLeft = (m_DrawRect.left >= m_PicCtrlRect.left ? TRUE : FALSE);
	m_bIsReachRight = (m_DrawRect.right <= m_PicCtrlRect.right ? TRUE : FALSE);
	m_bIsReachTop = (m_DrawRect.top >= m_PicCtrlRect.top ? TRUE : FALSE);
	m_bIsReachBottom = (m_DrawRect.bottom <= m_PicCtrlRect.bottom ? TRUE : FALSE);
	if (m_bIsDragging)
	{
		CPoint point_buffer = point - m_OriginPoint;
		if ((m_bIsReachTop && point_buffer.y > 0)||(m_bIsReachBottom && point_buffer.y < 0))
		{
			point_buffer.y = 0;
		}
		if ((m_bIsReachLeft && point_buffer.x > 0)||(m_bIsReachRight && point_buffer.x < 0))
		{
			point_buffer.x = 0;
		}
	
		m_PointDiff += point_buffer;
		CPoint LT = m_OriginRect.TopLeft() + m_PointDiff;
		CPoint RB = m_OriginRect.BottomRight() + m_PointDiff;
		m_DrawRect = CRect(LT, RB);
		m_OriginPoint = point;
		ZoomPic();
	}
	else
	{
		return CStatic::OnMouseMove(nFlags, point);
	}
}

BOOL CMyPictureCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int result = (zDelta / 120);
	if ((m_uiZoomScale + result) >= 0 && (m_uiZoomScale + result) <= 100)
		m_uiZoomScale += result;
	m_bIsZoomed = TRUE;
	ZoomPic();
	
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyPictureCtrl::ReturnAnimation()//此函数中算出偏移量，并在StartAnimation中实现具体操作
{
	m_nAnimationDiffWidth = m_nAnimationDiffHeight = 0;
	int nDiffL = abs(m_DrawRect.left - m_PicCtrlRect.left);
	int nDiffR = abs(m_DrawRect.right - m_PicCtrlRect.right);
	int nDiffT = abs(m_DrawRect.top - m_PicCtrlRect.top);
	int nDiffB = abs(m_DrawRect.bottom - m_PicCtrlRect.bottom);
	int nDeltaW = 0, nDeltaH = 0;
	//横向

	if (m_bIsReachLeft && !m_bIsReachRight)
	{
		nDiffR - nDiffL > 0 ? (nDeltaW = nDiffL) :
			(nDeltaW = (nDiffL - nDiffR) / 2);

	}
	else if (!m_bIsReachLeft && !m_bIsReachRight)
	{
		nDeltaW = 0;
	}
	else
	{
		nDiffL - nDiffR > 0 ? (nDeltaW = nDiffR) :
			(nDeltaW = (nDiffR - nDiffL) / 2);
	}
	m_nAnimationDiffWidth = nDeltaW;
	//纵向

	if (m_bIsReachTop && !m_bIsReachBottom)
	{
		nDiffB - nDiffT > 0 ? (nDeltaH = nDiffT) :
			(nDeltaH = (nDiffT - nDiffB) / 2);
	}
	else if (!m_bIsReachTop && !m_bIsReachBottom)
	{
		nDeltaH = 0;
	}
	else
	{
		nDiffB - nDiffT > 0 ? (nDeltaH = nDiffB) :
			(nDeltaH = (nDiffB - nDiffT) / 2);
	}
	m_nAnimationDiffHeight = nDeltaH;
	SetTimer(AnimationTimer, 45, 0);
	m_bHasTimer = TRUE;
}

void CMyPictureCtrl::OnTimer(UINT nIDEVENT)
{
	switch (nIDEVENT)
	{
	case AnimationTimer:
	{
		StartAnimation();
		break;
	}
	default:
		break;
	}
}

void CMyPictureCtrl::StartAnimation()
{
	int nCurWidth = m_DrawRect.Width();
	int nCurHeight = m_DrawRect.Height();
	int nCurLeft = m_DrawRect.left;
	int nCurTop = m_DrawRect.top;


	if ((m_nAnimationDiffWidth)-1 >= 0)
	{

		if (nCurLeft < m_PicCtrlRect.left && m_DrawRect.right < m_PicCtrlRect.right)
		{
			nCurLeft += 1;
			--m_nAnimationDiffWidth;
		}
		else if (nCurLeft > m_PicCtrlRect.left&& m_DrawRect.right > m_PicCtrlRect.right)
		{
			nCurLeft -= 1;
			--m_nAnimationDiffWidth;
		}
		else
		{
			int diffL = abs(nCurLeft - m_PicCtrlRect.left);
			int diffR = abs(m_PicCtrlRect.right - m_DrawRect.right);
			if (abs(diffL - diffR) <= 1)
				goto next;
			diffL > diffR ? (nCurLeft -= 1) : (nCurLeft += 1);
			--m_nAnimationDiffWidth;
		}

	}
next:
	if ((m_nAnimationDiffHeight - 1) >= 0)
	{
		if (nCurTop < m_PicCtrlRect.top && m_DrawRect.bottom < m_PicCtrlRect.bottom)
		{
			nCurTop += 1;
			--m_nAnimationDiffHeight;
		}
		else if (nCurTop > m_PicCtrlRect.top&& m_DrawRect.bottom > m_PicCtrlRect.bottom)
		{
			nCurTop -= 1;
			--m_nAnimationDiffHeight;
		}
		else
		{
			int diffT = abs(nCurTop - m_PicCtrlRect.top);
			int diffB = abs(m_PicCtrlRect.bottom - m_DrawRect.bottom);
			if (abs(diffT - diffB) <= 1)
				goto done;
			diffT > diffB ? (nCurTop -= 1) : (nCurTop += 1);
			--m_nAnimationDiffHeight;
		}
	}
done:
	m_DrawRect = CRect(nCurLeft, nCurTop, nCurLeft + nCurWidth, nCurTop + nCurHeight);
	Invalidate();
	UpdateWindow();
	if (!m_nAnimationDiffWidth && !m_nAnimationDiffHeight)
	{
		m_bHasTimer = FALSE;
		KillTimer(AnimationTimer);
	}

}
