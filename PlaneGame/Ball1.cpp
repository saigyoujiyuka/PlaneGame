#include "stdafx.h"
#include "Ball1.h"
#include "resource.h"

CImageList CBall1::m_Images;

CBall1::CBall1(int x, int y, int nMontion) :CGameObject(x, y), m_nMotion(nMontion)
{

}

CBall1::~CBall1(void)
{
}
BOOL CBall1::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y + m_nMotion * 8;
	}

	if (m_ptPos.y > GAME_HEIGHT + BALL_HEIGHT)
		return FALSE;
	if (m_ptPos.y < -BALL_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}

BOOL CBall1::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_BALL1, RGB(0, 0, 0), 15, 60, 1);
}