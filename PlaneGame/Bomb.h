#pragma once
#include "gameobject.h"

class CBomb :
	public CGameObject
{
public:
	CBomb(int x,int y);
	CBomb(int x, int y, int dr);
	~CBomb(void);

	BOOL Draw(CDC* pDC,BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos,CPoint(m_ptPos.x+BOMB_WIDTH,m_ptPos.y+BOMB_HEIGHT));
	}
private:
	static const int BOMB_HEIGHT = 50;   //子弹高度
	static const int BOMB_WIDTH = 20; 
	static CImageList m_Images;
	int direct;  //子弹方向，0为正前方，负值为左，正值为右
};
