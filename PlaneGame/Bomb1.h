#pragma once
#include "GameObject.h"
class CBomb1 :
	public CGameObject
{
public:
	CBomb1(int x, int y);
	CBomb1(int x, int y, int dr);
	~CBomb1(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + 20, m_ptPos.y + BOMB_HEIGHT));
	}
private:
	static const int BOMB_HEIGHT = 50;   //子弹高度
	static const int BOMB_WIDTH = 15;
	static CImageList m_Images;
	int direct;  //子弹方向，0为正前方，负值为左，正值为右
};
