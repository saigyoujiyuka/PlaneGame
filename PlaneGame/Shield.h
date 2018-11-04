#pragma once
#include "GameObject.h"
class CShield :public CGameObject
{
public:
	CShield(void);
	~CShield(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	void SetPoint(CPoint t);

	static BOOL LoadImage();
	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + SLD_WIDTH, m_ptPos.y + SLD_HEIGHT));
	}
public:
	static const int SLD_WIDTH = 100;
	static const int SLD_HEIGHT = 100;                      //战机的宽高
private:
	static CImageList m_Images;
};