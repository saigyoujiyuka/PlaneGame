#pragma once
#include "GameObject.h"

class CBall2 :public CGameObject
{
public:
	CBall2(int x, int y, int dr);
	~CBall2(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + BALL_HEIGHT, m_ptPos.y + BALL_HEIGHT));
	}
private:
	static const int BALL_HEIGHT = 20;
	static const int BALL_WIDTH = 20;
	static CImageList m_Images;
	int direct;  //子弹方向，0为前，负为左，正为右
};