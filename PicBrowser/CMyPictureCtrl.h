#pragma once
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <atlimage.h>

#define AnimationTimer 1 

#define WMU_ZOOM (WM_USER+0x1000)

class CMyPictureCtrl :public CStatic
{
private:
	
	CImage m_cImage; //图像存储

	CRect m_DrawRect;//实际绘制区大小
	CRect m_ImgRect;//图像大小
	CRect m_OriginRect;//原Draw_rect大小
	CRect m_PicCtrlRect;//pic contrl大小
	CRect m_FullScreenRect;//屏幕大小
	CRect m_ClientRect;//客户区大小
	CRect m_FunctionRect;//功能区大小（除本控件外大小）


	BOOL m_bIsFullScreen;
	BOOL m_bIsZoomed;
	BOOL m_bIsDragging;
	BOOL m_bIsTrack;
	BOOL m_bIsReachLeft;
	BOOL m_bIsReachRight;
	BOOL m_bIsReachTop;
	BOOL m_bIsReachBottom;
	BOOL m_bIsMoved;
	BOOL m_bHasTimer;

	CPoint m_PointDiff;
	CPoint m_OriginPoint;
	CPoint m_OriginPointLT;
	CPoint m_OriginPointRB;

	INT m_nAnimationDiffWidth;
	INT m_nAnimationDiffHeight;

	

	UINT m_uiZoomScale;
private: 
	void UpdateDrawRect();//更新绘制区
	void ZoomPic();//放大图片
	void ReturnAnimation();//动画预备
	void StartAnimation();//开始动画
	
protected:
	
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT);
	DECLARE_MESSAGE_MAP()

public:
	CMyPictureCtrl();
	void SetImageFile(LPCTSTR ImageFilePath);//设定文件路径
	void SetZoom(UINT Scale);//放大图片
	void FullScreenModel(BOOL);//全屏状态
};

