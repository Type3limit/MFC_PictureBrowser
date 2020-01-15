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
	
	CImage m_cImage; //ͼ��洢

	CRect m_DrawRect;//ʵ�ʻ�������С
	CRect m_ImgRect;//ͼ���С
	CRect m_OriginRect;//ԭDraw_rect��С
	CRect m_PicCtrlRect;//pic contrl��С
	CRect m_FullScreenRect;//��Ļ��С
	CRect m_ClientRect;//�ͻ�����С
	CRect m_FunctionRect;//��������С�������ؼ����С��


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
	void UpdateDrawRect();//���»�����
	void ZoomPic();//�Ŵ�ͼƬ
	void ReturnAnimation();//����Ԥ��
	void StartAnimation();//��ʼ����
	
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
	void SetImageFile(LPCTSTR ImageFilePath);//�趨�ļ�·��
	void SetZoom(UINT Scale);//�Ŵ�ͼƬ
	void FullScreenModel(BOOL);//ȫ��״̬
};

