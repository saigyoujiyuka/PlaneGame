#pragma once
#include "GameObject.h"
class CSkill :
	public CGameObject
{
public:
	CSkill(int x, int y);
	~CSkill(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + SK_WIDTH, m_ptPos.y + SK_HEIGHT));
	}
private:
	static const int SK_HEIGHT = 100;   //×Óµ¯¸ß¶È
	static const int SK_WIDTH = 128;
	static CImageList m_Images;
};
