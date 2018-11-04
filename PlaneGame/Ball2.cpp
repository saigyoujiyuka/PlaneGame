#include "stdafx.h"
#include "Ball2.h"
#include "resource.h"

CImageList CBall2::m_Images;

CBall2::CBall2(int x, int y, int dr) :CGameObject(x, y), direct(dr)
{

}

CBall2::~CBall2(void)
{
}
BOOL CBall2::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		if(direct==0)
			m_ptPos.y = m_ptPos.y + 8;
		if (direct == 1)
		{
			m_ptPos.y = m_ptPos.y + 8;
			m_ptPos.x = m_ptPos.x + 2;
		}
		if (direct == -1)
		{
			m_ptPos.y = m_ptPos.y + 8;
			m_ptPos.x = m_ptPos.x - 2;
		}
		if (direct == 2)
		{
			m_ptPos.y = m_ptPos.y + 6;
			m_ptPos.x = m_ptPos.x + 5;
		}
		if (direct == -2)
		{
			m_ptPos.y = m_ptPos.y + 6;
			m_ptPos.x = m_ptPos.x - 5;
		}
	}

	if (m_ptPos.y > GAME_HEIGHT + BALL_HEIGHT)
		return FALSE;
	/*if (m_ptPos.y < -BALL_HEIGHT)
		return FALSE;*/
	if (m_ptPos.x < 0)
		return FALSE;
	if (m_ptPos.x > GAME_WIDTH + BALL_WIDTH)
		return false;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}

BOOL CBall2::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_BALL2, RGB(0, 0, 0), 20, 20, 1);
}