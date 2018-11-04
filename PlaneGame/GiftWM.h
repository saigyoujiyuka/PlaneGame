#pragma once
#include "GameObject.h"
class CGiftWM :public CGameObject
{
public:
	CGiftWM(void);
	~CGiftWM(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + GIFTWM_HEIGHT, m_ptPos.y + GIFTWM_HEIGHT));
	}

	int GetMontion() const
	{
		return m_nMotion;
	}
private:
	static const int GIFTWM_HEIGHT = 50;                    //敌机的宽高
	static CImageList m_Images;
	int    m_nMotion;//方向 1->向下 0->停止 -1->向上
					 //速度
	int m_V;

};